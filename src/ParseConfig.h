#ifndef PARSECONFIG_H
#define PARSECONFIG_H
#include <Arduino.h>
#include <vector>

namespace parseTools
{   char * removeWhitespace(char * s);
    char * trim(char * s);
    bool isCommentOrWhitespace(const char *line, const char *comment = "#");
    /// @brief Confirms a line begins with head, contains delim, and ends with tail
    /// @return
    bool validateLineFormat(const char *line, const char *head = "<", const char *delim = "=", const char *tail = ">");
    template <typename T>
    bool binStringToInt(const char *binString, T &integer);

    /// @brief Splits string at delimiter string.  inserts '\0' at first char of delimiter.  Not modified if not found. returns pointer to first char after delim.
    /// @param str whole string.  will be modified
    /// @param delim string to split at.
    /// @return pointer to first char after delim.  NULL if not found.
    char *splitAt(char *str, const char *delim);
    /// @brief strips a line of 'head' and 'tail' and splits line at 'delim' (mandatory), and at ':' (optional).  ':' is valid in the first split string and must be followed by an int, intended to indicate the count of tokens in the second string.
    /// @param line char[] full line buffer. will be modified.
    /// @param property pointer to first token in the line buffer
    /// @param valueCount (optional) int second token (subtoken of first) indicating N tokens in the third.  Will be 1 if there is no :N present.
    /// @param values pointer to the third token in the line buffer, either single value or string of values.
    /// @param head string stripped from start of line
    /// @param delim primary delimiter, default '='
    /// @param tail string stripped from end of line
    /// @return t/f success / fail
    bool prepParsableLine(char *line, char *&property, int &valueCount, char *&values, const char *head = "<", const char *delim = "=", const char *tail = ">");

    template <typename T>
    bool binStringToInt(const char *binString, T &integer)
    {
        char buf[strlen(binString) + 1];
        strcpy(buf, binString);
        binString = removeWhitespace(buf);

        if (strncmp(binString, "0b", 2) != 0 || strlen(binString) != sizeof(T) * 8 + 2)
            return false;
        T temp = 0;
        const char *bindigits = binString + 2;
        for (int j = 0, i = strlen(bindigits) - 1; i >= 0; i--, j++)
            if (bindigits[i] >= '0' && bindigits[i] < '2')
                temp += (bindigits[i] - '0') * pow(static_cast<T>(2), j);
            else
                return false;

        integer = temp;
        return true;
    }

    template <typename T>
    std::enable_if_t<std::is_integral_v<T>, bool>
    buildIntTokenVector(const char *line, std::vector<T> &vValues, const char *delim = ",", int expected = -1, uint8_t base = 10)
    {
        vValues.clear();
        bool noErr = true;
        char valStr[strlen(line) + 1];
        strcpy(valStr, line);
        char *endPtr;
        long int vint;
        char *v = strtok(valStr, ",");
        while (v != NULL)
        {
            char buf[strlen(v) + 1];
            strcpy(buf, v);
            v = trim(buf);
            vint = strtol(v, &endPtr, base);
            if ((vint == 0 && strcmp(v, "0") != 0) || *endPtr != '\0' || vint > std::numeric_limits<T>::max() || vint < std::numeric_limits<T>::min()) {
                Serial.printf("-ERROR-  -   Possible invalid value in string \"%s\"\n", v);
                noErr = false;
                break;
            }
            vValues.push_back(static_cast<T>(vint));
            v = strtok(NULL, delim);
        }

        if (expected != -1 && vValues.size() != expected)
        {
            Serial.printf("--INVALID NUMBER OF ARRAY VALUES--   :%s\n", line);
            Serial.printf("vector.size() == %d, expected = %d\n", vValues.size(), expected);
            vValues.clear();
            return false;
        }
        if (!noErr) 
            vValues.clear();
        else
            Serial.printf("successfully constructed vector : %s\n", line);
        
        return noErr;
    }
}
#endif