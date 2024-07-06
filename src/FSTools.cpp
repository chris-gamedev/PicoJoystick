#include "FSTools.h"


//TODO
//TODO  Convert all to C Strings

File fstools::openFileWithMessages(const char *filename, const char *mode)
{

    if (!FSTOOLS_FS.begin())
    {
        Serial.printf("--COULD NOT OPEN FILE SYSTEM--\n");
    }
    else
        Serial.printf("File System opened successfully\n");

    if (FSTOOLS_FS.exists(filename))
    {
        Serial.printf("%s exists\n", filename);
        // create new config here
    }

    File f = FSTOOLS_FS.open(filename, mode);
    if (!f)
    {
        Serial.printf("%s - file not opened for moode \"%s\"", filename, mode);
        // handle error here
    }

    return f;
}

void fstools::printFileSystemInfoToSerial()
{

    FSTOOLS_FS.begin();
    FSInfo64 fsinfo;
    FSTOOLS_FS.info64(fsinfo);
    Serial.printf("---------<<  FILE SYSTEM INFORMATION  >>---------\n");
    Serial.printf("           totalBytes\t%d\n", fsinfo.totalBytes);
    Serial.printf("            usedBytes\t%d\n", fsinfo.usedBytes);
    Serial.printf("            blockSize\t%d\n", fsinfo.blockSize);
    Serial.printf("             pageSize\t%d\n", fsinfo.pageSize);
    Serial.printf("         maxOpenFiles\t%d\n", fsinfo.maxOpenFiles);
    Serial.printf("        maxPathLength\t%d\n", fsinfo.maxPathLength);
    Serial.printf("-------------------------------------------------\n");
    Serial.printf("\n\n");
    FSTOOLS_FS.end();
}

void fstools::printFileToSerial(File f)
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

void fstools::printFileToSerial(const char *name)
{

    File f = openFileWithMessages(name, "r");
    if (!f)
        return;
    printFileToSerial(f);
    f.close();
    FSTOOLS_FS.end();
}

std::vector<String> fstools::getDirListOnlyFiles(String path)
{
    if (!FSTOOLS_FS.begin())
    {
        Serial.printf("Unable to mount filesystem.\n");
    }
    Dir dir = FSTOOLS_FS.openDir(path.c_str());

    std::vector<String> filenames;
    while (dir.next())
    {
        if (dir.isFile())
            filenames.push_back(dir.fileName());
    }
    FSTOOLS_FS.end();
    return filenames;
}

void fstools::listFilesToSerialRcrsv(String path)
{
    Dir dir = FSTOOLS_FS.openDir(path);
    String pad = "-------------------------------";
    int l = path.length();
    int padsizeL = pad.length() - l / 2;
    int padsizeR = pad.length() - (l - l / 2);
    Serial.printf("%.*s--<  %s  >--%.*s\n", padsizeL, pad.c_str(), path.c_str(), padsizeR, pad.c_str());
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
            listFilesToSerialRcrsv(path + dir.fileName() + "/");
    }
}

void fstools::printAllFilesInDirectoryToSerial(String path)
{
    if (!LittleFS.begin())
    {
        Serial.printf("Failed to mount file system\n");
        return;
    }
    printAllFilesInDirectoryToSerialRcrsv(path);
    LittleFS.end();
}


void fstools::printFileTreeToSerial(String path, String dirName, String treeString)
{
    if (!LittleFS.begin())
    {
        Serial.printf("Failed to mount file system\n");
        return;
    }

    printFileTreeToSerialRcrsv(path, dirName, treeString);
    LittleFS.end();
}

void fstools::listFilesToSerial(String path)
{
    
    if (!LittleFS.begin())
    {
        Serial.printf("Failed to mount file system\n");
        return;
    }
    listFilesToSerialRcrsv(path);
    LittleFS.end();
}

void fstools::printAllFilesInDirectoryToSerialRcrsv(String path)
{
    Dir dir = FSTOOLS_FS.openDir(path);
    while (dir.next())
    {
        Serial.printf("NAME: %20s\t", (path + dir.fileName()).c_str());
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
            printAllFilesInDirectoryToSerialRcrsv(path + dir.fileName() + "/");
    }
}

void fstools::printFileTreeToSerialRcrsv(String path, String dirName, String treeString)
{

    // 0x2502 │  0x2500 ─   0x251C ├   0x252c ┬   0x2514 └
    Dir dir = FSTOOLS_FS.openDir(path);
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

