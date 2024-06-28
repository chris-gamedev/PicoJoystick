#include "Configuration.h"

File Configurator_::openConfigFile(const char *filename, const char *mode)
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

    File f = openConfigFile(filename, "r");

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
        else  // handle arrays and scalar
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

void Configurator_::saveConfigToFile(const char *filename, Configuration *pconfig)
{
    File f = openConfigFile(filename, "w");
    if (!f)
        return;
    f.printf("#Custom Configuration: %s\n", filename);
    f.print("#\n");
    f.print("#\n");
    f.printf("<global_menuHotkey_on=%d>\n", pconfig->global_menuHotkey_on);
    tokenizeArrayToFile("joystick_joyValueMap", f, pconfig->joystick_joyValueMap);
    tokenizeArrayToFile("joystick_buttonValueMap", f, pconfig->joystick_buttonValueMap);
    f.printf("<joystick_transmitToHost=%d>\n", pconfig->joystick_transmitToHost);
    tokenizeArrayToFile("drawKeyPresses_macroMap", f, pconfig->drawKeypresses_macroMap);
    f.printf("<funThings_on=%d>\n", pconfig->funThings_on);

    f.close();
    LittleFS.end();
}