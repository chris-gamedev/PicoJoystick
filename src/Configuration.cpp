#include "Configuration.h"

File Configurator_::openFileWithMessages(const char *filename, const char *mode)
{

    if (!LittleFS.begin())
    {
        Serial.printf("--COULD NOT OPEN FILE SYSTEM--\n");
    }
    else
        Serial.printf("File System opened successfully\n");

    if (LittleFS.exists(filename))
    {
        Serial.printf("%s exists\n", filename);
        // create new config here
    }

    File f = LittleFS.open(filename, mode);
    if (!f)
    {
        Serial.printf("%s - file not opened for moode \"%s\"", filename, mode);
        // handle error here
    }

    return f;
}

bool Configurator_::parsableLine(String line, int lineCounter)
{
    if (line.length() == 0 || line.indexOf('#') == 0) // comment & whitespace
        return false;

    if (line.indexOf('<') != 0 || line.indexOf('=') == -1 || line.indexOf('>') != line.length() - 1)
    {
        Serial.printf("--INVALID LINE-- at line:%d   %s\n", lineCounter, line.c_str());
        return false;
    }
    return true;
}

std::vector<uint16_t> Configurator_::buildTokenVector(String *token, String value, int lineCounter)
{
    int arraySize = 1;
    std::vector<uint16_t> vValues;
    if (token->indexOf(':') != -1) // if there is a series of values, i.e. <token:3=0,1,2>
    {
        String indexer = token->substring(token->indexOf(':') + 1); // safe even if there isn't one
        if (token->indexOf(':') == 0 || token->indexOf(':') == token->length() - 1 || String(indexer.toInt()) != indexer)
        {
            Serial.printf("--INVALID ARRAY FORMATION-- at line:%d  %s:%s\n", lineCounter, token->c_str(), value.c_str());
            return vValues;
        }

        arraySize = indexer.toInt();
        *token = token->substring(0, token->indexOf(':'));
    }
    String v;
    value = "," + value;
    while (value.indexOf(',') == 0)
    {
        value = value.substring(value.indexOf(',') + 1);
        if (value.indexOf(',') != -1)
        {
            v = value.substring(0, value.indexOf(','));
            value = value.substring(value.indexOf(','));
        }
        else
            v = value;
        v.trim();
        if (String(v.toInt()) != v)
            break;

        vValues.push_back(v.toInt());
    }

    if (vValues.size() != arraySize)
    {
        Serial.printf("--INVALID NUMBER OF ARRAY VALUES: EXPECTED %d-- at line:%d  %s:%s\n", arraySize, lineCounter, token->c_str(), value.c_str());
        vValues.clear();
        return vValues;
    }

    return vValues;
}

int Configurator_::importConfigFile(const char *filename, Configuration *pconfig)
{
    int errCode = 0;

    File f = openFileWithMessages(filename, "r");

    int lineCounter = 0;
    while (f.available())
    {
        lineCounter++;
        String line = f.readStringUntil('\n');
        line.trim();

        if (!parsableLine(line, lineCounter))
            continue;

        String token = line.substring(line.indexOf('<') + 1, line.indexOf('='));
        String value = line.substring(line.indexOf('=') + 1, line.indexOf('>'));
        token.trim();
        token.toUpperCase();
        value.trim();
        std::vector<uint16_t> arrayValues;
        bool valueIsString = false;

        if (value[0] == '"') // handle strings
        {
            if (value.lastIndexOf('"') != value.length() - 1)
            {
                Serial.printf("--INVALID STRING-- at line:%d  %s\n", lineCounter, line.c_str());
                continue;
            }
            valueIsString = true;
            value = value.substring(value.indexOf('"') + 1, value.lastIndexOf('"'));
        }
        else // handle arrays and scalar
        {
            arrayValues = buildTokenVector(&token, value, lineCounter);
            if (arrayValues.size() == 0)
            {
                Serial.printf("Skipping %s\n", token.c_str());
                continue;
            }
        }

        // send token and value to the config.
        if (valueIsString)
        { // string
            Serial.printf("%s = String(%s)\n", token.c_str(), value.c_str());
            saveTokenToConfig(token, value, pconfig);
        }
        else
        { // array
            Serial.printf("%s[%d] = { ", token.c_str(), arrayValues.size());
            for (auto it : arrayValues)
                Serial.printf("%d,", it);
            Serial.printf(" }\n");
            saveTokenToConfig(token, arrayValues, pconfig);
        }
    }

    Serial.printf("Total lines: %d\n", lineCounter);

    f.close();
    LittleFS.end();

    return errCode;
}

template <std::size_t N>
inline void Configurator_::tokenizeArrayToFile(String name, File f, const std::array<uint8_t, N> &arr)
{
    int size = arr.size();
    f.printf("<%s:%d=", name.c_str(), size);
    for (int i = 0; i < size; i++)
    {
        f.printf("%d", arr[i]);
        if (i != size - 1)
            f.printf(",");
        else
            f.printf(">\n");
    }
}

bool Configurator_::saveConfigToFile(const char *filename, Configuration *pconfig)
{
    String fullFilename = "/config/" + String(filename);
    File f = openFileWithMessages(fullFilename.c_str(), "w"); // littleFS begins here
    if (!f)
        return false;
    f.printf("#Custom Configuration: %s\n", fullFilename.c_str());
    f.print("#\n");
    f.print("#\n");
    f.printf("<global_menuHotkey_on=%d>\n", pconfig->global_menuHotkey_on);
    tokenizeArrayToFile("joystick_joyValueMap", f, pconfig->joystick_joyValueMap);
    tokenizeArrayToFile("joystick_buttonValueMap", f, pconfig->joystick_buttonValueMap);
    f.printf("<joystick_transmitToHost=%d>\n", pconfig->joystick_transmitToHost);
    tokenizeArrayToFile("drawKeyPresses_macroMap", f, pconfig->drawKeypresses_macroMap);
    f.printf("<funThings_on=%d>\n", pconfig->funThings_on);

    f.close();
    bool fileCreated = LittleFS.exists(fullFilename.c_str());
    LittleFS.end();

    return fileCreated;
}

void Configurator_::printFileSystemInfoToSerial()
{
    LittleFS.begin();
    FSInfo64 fsinfo;
    LittleFS.info64(fsinfo);
    Serial.printf("---------<<  FILE SYSTEM INFORMATION  >>---------\n");
    Serial.printf("           totalBytes\t%d\n", fsinfo.totalBytes);
    Serial.printf("            usedBytes\t%d\n", fsinfo.usedBytes);
    Serial.printf("            blockSize\t%d\n", fsinfo.blockSize);
    Serial.printf("             pageSize\t%d\n", fsinfo.pageSize);
    Serial.printf("         maxOpenFiles\t%d\n", fsinfo.maxOpenFiles);
    Serial.printf("        maxPathLength\t%d\n", fsinfo.maxPathLength);
    Serial.printf("-------------------------------------------------\n");
    Serial.printf("\n\n");
    LittleFS.end();
}

void Configurator_::printFileToSerial(const char *name)
{

    File f = openFileWithMessages(name, "r");
    if (!f)
        return;
    printFileToSerial(f);
    f.close();
    LittleFS.end();
}

void Configurator_::printFileToSerial(File f)
{

    String pad = "--------------------------------------------------";
    String str1 = "--<<   Printing File:  /" + String(f.fullName()) + "   >>--";
    int l = str1.length();
    int padsizeL = pad.length() - l / 2;
    int padsizeR = pad.length() - (l - l / 2);
    int lineCounter = 0;
    Serial.printf("\n%.*s%s%.*s\n", padsizeL, pad.c_str(), str1.c_str(), padsizeR, pad.c_str());
    String line;
    while (f.available())
    {
        // Serial.printf("in the loop\n");
        lineCounter++;
        line = f.readStringUntil('\n');
        // line.trim();
        Serial.printf("%s\n", line.c_str());
    }

    String str2 = "--<<   Line Count:" + String(lineCounter) + " - Closing   >>--";
    l = str2.length();
    padsizeL = pad.length() - l / 2;
    padsizeR = pad.length() - (l - l / 2);
    Serial.printf("%.*s%s%.*s\n\n", padsizeL, pad.c_str(), str2.c_str(), padsizeR, pad.c_str());
}

void Configurator_::listFilesToSerialRcrsv(String dirname)
{
    Dir dir = LittleFS.openDir(dirname);
    String pad = "-------------------------------";
    int l = dirname.length();
    int padsizeL = pad.length() - l / 2;
    int padsizeR = pad.length() - (l - l / 2);
    Serial.printf("%.*s--<  %s  >--%.*s\n", padsizeL, pad.c_str(), dirname.c_str(), padsizeR, pad.c_str());
    Serial.printf("%20s\t%7s\t%9s %9s%s\n", "-<   NAME", "SIZE(B)", "Modified", "Created", "   >-");
    Serial.printf("    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    while (dir.next())
    {
        Serial.printf("%20s\t", dir.fileName().c_str());
        if (dir.isDirectory())
            Serial.printf(" <DIR>  ");
        else if (dir.fileSize())
        {
            File f = dir.openFile("r");
            Serial.printf("%7d ", f.size());
        }
        Serial.printf("%9lld %9lld\n", dir.fileTime(), dir.fileCreationTime());
    }
    Serial.printf("\n\n");

    dir.rewind();
    while (dir.next())
    {
        if (dir.isDirectory())
            listFilesToSerialRcrsv(dirname + dir.fileName() + "/");
    }
}

void Configurator_::printAllFilesInDirectoryToSerialRcrsv(String dirname)
{
    Dir dir = LittleFS.openDir(dirname);
    while (dir.next())
    {
        Serial.printf("NAME: %20s\t", (dirname + dir.fileName()).c_str());
        if (dir.fileSize())
        {
            File f = dir.openFile("r");
            Serial.printf("SIZE: %dB\n", f.size());
            printFileToSerial(f);
            Serial.printf("\n\n");
        }
        else if (dir.isDirectory())
            Serial.printf("<DIR>\n");
    }
    dir.rewind();
    while (dir.next())
    {
        if (dir.isDirectory())
            printAllFilesInDirectoryToSerialRcrsv(dirname + dir.fileName() + "/");
    }
}

void Configurator_::printFileTreeToSerialRcrsv(String path, String dirName, String treeString)
{

    // 0x2502 │  0x2500 ─   0x251C ├   0x252c ┬   0x2514 └
    Dir dir = LittleFS.openDir(path);
    bool firstFile = true;
    bool dirIsFile;
    dirName = "─" + dirName + "─";

    if (!dir.next())
    {
        Serial.printf("%s──(empty)\n", dirName.c_str());
        return;
    }

    Serial.printf("%s", dirName.c_str());
    for (int i = 0; i < static_cast<int>(dirName.length()) - 4; i++)
        treeString += " ";

    while (1)
    {
        String filename = dir.fileName();
        String filenameString = filename;
        int size = dir.fileSize();
        dirIsFile = dir.isFile();
        bool lastFile = !dir.next();
        if (firstFile && lastFile)
            filenameString = "──" + filenameString;
        else if (firstFile)
            filenameString = "┬─" + filenameString;
        else if (lastFile)
            filenameString = "└─" + filenameString;
        else
            filenameString = "├─" + filenameString;
        if (!firstFile)
            Serial.printf("%s", treeString.c_str());

        if (dirIsFile)
        {
            Serial.printf("%s  (%dB)\n", filenameString.c_str(), size);
        }
        else
        {
            String limb = "│";
            if (firstFile && lastFile)
            {
                Serial.printf("─");
                limb = " ";
            }
            else if (firstFile)
                Serial.printf("┬");
            else if (lastFile)
            {
                Serial.printf("└");
                limb = " ";
            }
            else
                Serial.printf("├");
            printFileTreeToSerialRcrsv(path + filename + "/", filename, treeString + limb);
        }

        firstFile = false;
        if (lastFile)
            break;
    }
}
