#include "ParseConfig.h"
#include <cstring>

char *parseTools::removeWhitespace(char *s)
{
    if (s == NULL)
        return s;
    char *start = s;
    char *b = s;
    while (*b != '\0')
    {
        if (isspace(*b))
        {
            b++;
            continue;
        }
        *(s++) = *(b++);
    }
    *s = '\0';
    return start;
}

char *parseTools::trim(char *s)
{
    if (s == NULL || *s == '\0')
        return s;

    char *b = s;
    while (isspace(*b)) 
        b++;
    s = b;
    if (*s == '\0')
        return s;

    b = s + strlen(s);
    while (isspace(*(--b)))
    {
        *(b) = '\0';
        if (b == s)
            break;
    }
    return s;
}

bool parseTools::isCommentOrWhitespace(const char *line, const char *comment)
{
    if (strstr(line, comment) == line)
        return true;
    const char *p = line;
    while (*p != '\0')
        if (!isspace(*(p++)))
            return false;
    return true;
}

bool parseTools::prepParsableLine(char *line, char *&property, int &valueCount, char *&values, const char *head, const char *delim, const char *tail)
{
    if (!validateLineFormat(line, head, delim, tail))
    {
        Serial.printf("--INVALID LINE--   %s\n", line);
        return false;
    }

    property = line + strlen(head);
    values = splitAt(line, "=");
    char *p = strstr(values, tail);
    *p = '\0';
    if (strlen(property) == 0 || strlen(values) == 0)
    {
        Serial.printf("--INVALID LINE--   %s%s%s%s%s\n", head, property, delim, values, tail);
        return false;
    }

    int arraySize = 1;
    char *count = splitAt(property, ":");
    if (count != NULL) // if there is a series of N values, i.e. <token:N=0,1,2>    :abc5
    {
        if (*property == '\0' || *count == '\0' || (atoi(count) == 0 && strcmp(count, "0") != 0))
        {
            Serial.printf("--INVALID ARRAY FORMATION---  %s:%s\n", property, count);
            return false;
        }

        arraySize = atoi(count);
    }
    valueCount = arraySize;
    return true;
}

bool parseTools::validateLineFormat(const char *line, const char *head, const char *delim, const char *tail)
{
    if (strstr(line, head) != line)
        return false;
    if (strstr(line, delim) == NULL)
        return false;
    if (strstr(line, tail) != line + strlen(line) - 1)
        return false;

    return true;
}

char *parseTools::splitAt(char *str, const char *delim)
{
    char *substr = strstr(str, delim);
    if (substr == NULL)
        return substr;
    *substr = '\0';
    return substr + strlen(delim);
}
