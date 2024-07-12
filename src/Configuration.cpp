#include "Configuration.h"


std::vector<uint16_t> Configurator_::buildTokenVector(String &token, String &value, int lineCounter)
{
    int arraySize = 1;
    std::vector<uint16_t> vValues;
    if (token.indexOf(':') != -1) // if there is a series of values, i.e. <token:3=0,1,2>
    {
        String indexer = token.substring(token.indexOf(':') + 1); // safe even if there isn't one
        if (token.indexOf(':') == 0 || token.indexOf(':') == token.length() - 1 || String(indexer.toInt()) != indexer)
        {
            Serial.printf("--INVALID ARRAY FORMATION-- at line:%d  %s:%s\n", lineCounter, token.c_str(), value.c_str());
            return vValues;
        }

        arraySize = indexer.toInt();
        token = token.substring(0, token.indexOf(':'));
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
        if (!(String(v.toInt()) == v && v.toInt() <= std::numeric_limits<uint16_t>::max()))
            break;

        vValues.push_back(v.toInt());
    }

    if (vValues.size() != arraySize)
    {
        Serial.printf("--INVALID NUMBER OF ARRAY VALUES: EXPECTED %d-- at line:%d  %s:%s\n", arraySize, lineCounter, token.c_str(), value.c_str());
        vValues.clear();
        return vValues;
    }


    return vValues;
}

bool Configurator_::prepParsableLine(String &line, String &token, String &value, int lineCounter)
{

    if (line.indexOf('<') != 0 || line.indexOf('=') == -1 || line.indexOf('>') != line.length() - 1)
    {
        Serial.printf("--INVALID LINE-- at line:%d   %s\n", lineCounter, line.c_str());
        return false;
    }
    token = line.substring(line.indexOf('<') + 1, line.indexOf('='));
    value = line.substring(line.indexOf('=') + 1, line.indexOf('>'));
        token.trim();
        token.toUpperCase();
        value.trim();

    return true;
}

int Configurator_::importConfigFile(const char *filename, Configuration *pconfig)
{
    int errCode = 0;

    File f = fstools::openFileWithMessages(filename, "r");

    int lineCounter = 0;
    while (f.available())
    {
        lineCounter++;
        String line = f.readStringUntil('\n');
        line.trim();

        if (line.length() == 0 || line.indexOf('#') == 0) // comment & whitespace
            continue;

        String token;
        String value;
        if (!prepParsableLine(line, token, value, lineCounter))
        {
            errCode = -1;
            continue;
        }

    
        std::vector<uint16_t> vIntValues;
        vIntValues.reserve(14);
        String stringValue;
        bool valueIsString = false;

        if (value[0] == '"') // handle strings
        {
            if (value.lastIndexOf('"') != value.length() - 1)
            {
                Serial.printf("--INVALID STRING-- at line:%d  %s\n", lineCounter, line.c_str());
                errCode = -1;
                continue;
            }
            valueIsString = true;
            value = value.substring(value.indexOf('"') + 1, value.lastIndexOf('"'));
        }
        else // handle arrays and scalar
        {
            vIntValues = buildTokenVector(token, value, lineCounter);
            if (vIntValues.size() == 0)
            {
                Serial.printf("Skipping %s\n", token.c_str());
                errCode = -1;
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
            Serial.printf("%s[%d] = { ", token.c_str(), vIntValues.size());
            for (auto it : vIntValues)
                Serial.printf("%d,", it);
            Serial.printf(" }\n");
            saveTokenToConfig(token, vIntValues, pconfig);
        }
    }

    Serial.printf("Total lines: %d\n", lineCounter);

    f.close();
    LittleFS.end();

    return errCode;
}

template <std::size_t N>
void Configurator_::assignTokenValuesToArray(const String &token, std::array<uint8_t, N> &arr, const std::vector<uint16_t> &values)
{
    if (values.size() != arr.size())
    {
        Serial.printf("%s : --INVALID NUMBER OF VALUES--\n", token.c_str());
        return;
    }
    int i = 0;
    for (auto it : values)
        arr[i++] = static_cast<uint8_t>(it);
}

template <std::size_t N>
inline void Configurator_::tokenizeContainerToFileInt(const char * name, File f, const std::array<uint8_t, N> &arr)
{
    int size = arr.size();
    f.printf("<%s:%d=", name, size);
    for (int i = 0; i < size; i++)
    {
        f.printf("%d", arr[i]);
        if (i != size - 1)
            f.printf(",");
        else
            f.printf(">\n");
    }
}

bool Configurator_::saveConfigToFile(const char *filename, const Configuration &config)
{
    char fullFilename[strlen("/config/") + strlen(filename) + 1];
    sprintf(fullFilename, "%s%s", "/config/", filename);
    File f = fstools::openFileWithMessages(fullFilename, "w"); // littleFS begins here
    if (!f)
        return false;
    f.printf("#Custom Configuration: %s\n", fullFilename);
    f.print("#\n");
    f.print("#\n");
    f.printf("<global_menuHotkey_on=%d>\n", config.global_menuHotkey_on);
    tokenizeContainerToFileInt("joystick_joyValueMap", f, config.joystick_joyValueMap);
    tokenizeContainerToFileInt("joystick_buttonValueMap", f, config.joystick_buttonValueMap);
    f.printf("<joystick_transmitToHost=%d>\n", config.joystick_transmitToHost);
    tokenizeContainerToFileInt("drawKeyPresses_macroMap", f, config.drawKeypresses_macroMap);
    f.printf("<funThings_on=%d>\n", config.funThings_on);

    f.close();
    bool fileCreated = LittleFS.exists(fullFilename);
    LittleFS.end();

    return fileCreated;
}




void Configurator_::saveTokenToConfig(const String &token, const String &value, Configuration *pconfig)
{
    auto t = configTokenMap.find(token);
    if (t == configTokenMap.end())
    {
        Serial.printf("--TOKEN %s NOT FOUND IN CONFIGURATION--\n", token.c_str());
        return;
    }
}

void Configurator_::saveTokenToConfig(const String &token, const std::vector<uint16_t> &values, Configuration *pconfig)

{
    auto t = configTokenMap.find(token);
    if (t == configTokenMap.end())
    {
        Serial.printf("--TOKEN %s NOT FOUND IN CONFIGURATION--\n", token.c_str());
        return;
    }
    int i = 0;
    switch (t->second)
    {
    case ConfigTokens::joystick_joyValueMap:
        assignTokenValuesToArray(token, pconfig->joystick_joyValueMap, values);
        break;
    case ConfigTokens::joystick_buttonValueMap:
        assignTokenValuesToArray(token, pconfig->joystick_buttonValueMap, values);
        break;
    case ConfigTokens::drawKeyPresses_macroMap:
        assignTokenValuesToArray(token, pconfig->drawKeypresses_macroMap, values);
        break;
    case ConfigTokens::global_menuHotKey_ON:
        pconfig->global_menuHotkey_on = static_cast<bool>(values[0]);
        break;
    case ConfigTokens::joystick_transmitToHost:
        pconfig->joystick_transmitToHost = static_cast<bool>(values[0]);
        break;
    case ConfigTokens::funThings_on:
        pconfig->funThings_on = static_cast<bool>(values[0]);
        break;
    }
    Serial.printf("%s, size:%d -- seemed to be successful\n", token.c_str(), values.size());
}
