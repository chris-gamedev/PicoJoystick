#include "FSTools.h"

// TODO
// TODO  Convert all to C Strings

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

    const char *pad = "--------------------------------------------------";
    const char *head = "--<<   Printing File:  /";
    const char *tail = "   >>--";
    char Str1[strlen(head) + strlen(tail) + strlen(f.fullName()) + 1];
    sprintf(Str1, "%s%s%s", head, f.fullName(), tail);
    int l = strlen(Str1);
    int padsizeL = strlen(pad) - l / 2;
    int padsizeR = strlen(pad) - (l - l / 2);
    int lineCounter = 0;
    Serial.printf("\n%.*s%s%.*s\n", padsizeL, pad, Str1, padsizeR, pad);
    String line;
    while (f.available())
    {
        lineCounter++;
        Serial.printf("%s\n", f.readStringUntil('\n').c_str());
    }
    head = "--<<   Line Count:";
    tail = " - Closing   >>--";
    char buff[7];
    itoa(lineCounter, buff, 10);
    char Str2[strlen(head) + strlen(tail) + strlen(buff) + 1];
    sprintf(Str2, "%s%s%s", head, buff, tail);

    l = strlen(Str2);
    padsizeL = strlen(pad) - l / 2;
    padsizeR = strlen(pad) - (l - l / 2);
    Serial.printf("%.*s%s%.*s\n\n", padsizeL, pad, Str2, padsizeR, pad);
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

std::vector<const char *> fstools::getDirListOnlyFiles(const char *path)
{
    if (!FSTOOLS_FS.begin())
    {
        Serial.printf("Unable to mount filesystem.\n");
    }
    Dir dir = FSTOOLS_FS.openDir(path);

    std::vector<const char *> filenames;
    while (dir.next())
    {
        if (dir.isFile())
            filenames.push_back(dir.fileName().c_str());
    }
    FSTOOLS_FS.end();
    return filenames;
}

void fstools::listFilesToSerialRcrsv(const char *path)
{
    Dir dir = FSTOOLS_FS.openDir(path);
    const char *pad = "-------------------------------";
    int l = strlen(path);
    int padsizeL = strlen(pad) - l / 2;
    int padsizeR = strlen(pad) - (l - l / 2);
    Serial.printf("%.*s--<  %s  >--%.*s\n", padsizeL, pad, path, padsizeR, pad);
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
        char fname[LFS_NAME_MAX + 1];
        if (dir.isDirectory())
        {
            strcpy(fname, dir.fileName().c_str());
            
            char newPath[strlen(path) + strlen(fname) + 3];
            sprintf(newPath, "%s%s%c", path, fname, '/');
            listFilesToSerialRcrsv(newPath);
        }
    }
}

void fstools::printAllFilesInDirectoryToSerial(const char *path)
{
    if (!LittleFS.begin())
    {
        Serial.printf("Failed to mount file system\n");
        return;
    }
    printAllFilesInDirectoryToSerialRcrsv(path);
    LittleFS.end();
}

void fstools::printFileTreeToSerial(const char *path, const char *dirName, const char *treeString)
{
    if (!LittleFS.begin())
    {
        Serial.printf("Failed to mount file system\n");
        return;
    }

    printFileTreeToSerialRcrsv(path, dirName, treeString);
    LittleFS.end();
}

void fstools::listFilesToSerial(const char *path)
{

    if (!LittleFS.begin())
    {
        Serial.printf("Failed to mount file system\n");
        return;
    }
    listFilesToSerialRcrsv(path);
    LittleFS.end();
}

void fstools::printAllFilesInDirectoryToSerialRcrsv(const char *path)
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
        char fname[LFS_NAME_MAX + 1];
        if (dir.isDirectory())
        {
            strcpy(fname,  dir.fileName().c_str());
            char newPath[strlen(path) + strlen(fname) + 2];
            sprintf(newPath, "%s%s%c", path, fname, '/');
            printAllFilesInDirectoryToSerialRcrsv(newPath);
        }
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////  OLD BUT WORKING  ////////////////////////////////////////////////////

void fstools::printFileTreeToSerialRcrsv(const char *path, const char *dirName, const char *treeString)
{

    // 0x2502 │  0x2500 ─   0x251C ├   0x252c ┬   0x2514 └
    Dir dir = FSTOOLS_FS.openDir(path);
    bool firstFile = true;
    bool dirIsFile;
    char newDirName[strlen(dirName) + 3];
    sprintf(newDirName, "─%s─", dirName);

    if (!dir.next())
    {
        Serial.printf("%s──(empty)\n", newDirName);
        return;
    }

    Serial.printf("%s", newDirName);
    char newTreeString[strlen(treeString) + strlen(newDirName) + 1];
    int padsize = (strlen(newDirName) - 3 > 0) ? strlen(newDirName) - 3 : 1;
    char pad[padsize];
    for (int i = 0; i < padsize; i++)
        pad[i] = ' ';
    pad[padsize - 1] = '\0';
    sprintf(newTreeString, "%s%s", treeString, pad);
    // for (int i = 0; i < static_cast<int>(strlen(newDirName)) - 4; i++)
    //     treeString += " ";
    char fname[LFS_NAME_MAX + 1];
    while (1)
    {
        strcpy(fname,  dir.fileName().c_str());

        char filenameString[strlen(fname) + 3];
        int size = dir.fileSize();
        dirIsFile = dir.isFile();
        bool lastFile = !dir.next();
        if (firstFile && lastFile) {
            // sprintf(filenameString, "──%s", fname);
            strcat(filenameString, "──");
            strcat(filenameString, fname);
        }
        else if (firstFile) {
            // sprintf(filenameString, "┬─%s", fname);
            strcat(filenameString, "┬─");
            strcat(filenameString, fname);

        }
        else if (lastFile){
            // sprintf(filenameString, "└─%s", fname);
            strcat(filenameString, "└─");
            strcat(filenameString, fname);

        }
        else{
            // sprintf(filenameString, "├─%s", fname);
            strcat(filenameString, "├─");
            strcat(filenameString, fname);

        }
        if (!firstFile)
            Serial.printf("%s", newTreeString);

        if (dirIsFile)
        {
            Serial.printf("%s  (%dB)\n", filenameString, size);
        }
        else
        {
            const char *limb = "│";
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
            int treeStrLength = strlen(newTreeString) + 2;
            char finalTreeString[treeStrLength];
            sprintf(finalTreeString, "%s%s", newTreeString, limb);

            char newFullPath[strlen(path) + strlen(fname) + 2];
            sprintf(newFullPath, "%s%s/", path, fname);

            printFileTreeToSerialRcrsv(newFullPath, fname, finalTreeString);
        }

        firstFile = false;
        if (lastFile)
            break;
    }
}
