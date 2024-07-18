#ifndef FSTOOLS_H
#define FSTOOLS_H

#include <FS.h>
#include <vector>

#define FSTOOLS_LITTLEFS
// #define FSTOOLS_SDFS

#ifdef FSTOOLS_LITTLEFS
#include <LittleFS.h>
#ifndef FSTOOLS_FS
#define FSTOOLS_FS LittleFS
#endif
#endif

#ifdef FSTOOLS_SDFS
#include <SDFS.h>
#ifndef FSTOOLS_FS
#define FSTOOLS_FS SDFS
#endif
#endif

namespace fstools
{
    /// @brief Print filesystem information to serial.
    void printFileSystemInfoToSerial();
    /// @brief Mount Filesystem and open file, generating error strings to Serial.
    /// @param filename full path to file
    /// @param mode mode string
    /// @return File object, regardless of file object's state.
    File openFileWithMessages(const char *filename, const char *mode);
    /// @brief Mount filesystem and print file contents to serial.
    /// @param name full path to file.
    void printFileToSerial(const char *name);
    /// @brief Print file contents and line count to Serial.
    /// @param f File object to print.
    void printFileToSerial(File f);
    /// @brief gather filenames within a given directory as strings.  Non-recursive.  omits directory names.
    /// @param path full path to directory
    /// @return vector of string filenames.
    std::vector<const char *> getDirListOnlyFiles(const char *path = "/");
    /// @brief List all files and file details recursively beginning at a path.  Assumes mounted filesystem.
    /// @param path full path to starting directory.
    void listFilesToSerialRcrsv(const char *path = "/");
    /// @brief List all files and details recursively beginning at a path.  Mounts filesystem and reports error
    /// @param path path to begin
    void listFilesToSerial(const char *path = "/");
    /// @brief print all file contents to Serial starting at path recursively. mounts FS and reports error
    /// @param path full path to start directory
    void printAllFilesInDirectoryToSerial(const char *path = "/");
    /// @brief print all file contents to Serial starting at path recursively.  Assumes mounted filesystem.
    /// @param path full path to start directory
    void printAllFilesInDirectoryToSerialRcrsv(const char * path = "/");
    /// @brief Draw filetree to Serial output as graphical text.  recursive.  Assumes mounted filesystem.
    /// @param path starting path. default is "/"
    /// @param dirName directory name for printing.  required due to limitatations on viewing current filename from Dir object.
    /// @param treeString ongoing left-padding of filetree.
    void printFileTreeToSerialRcrsv(const char *path = "/", const char * dirName = "/", const char * treeString = "");
    void printFileTreeToSerialRcrsv(String path, String dirName, String treeString); // OLD
    /// @brief Draw filetree to Serial output as graphical text. Mounts FS and gives error message.
    /// @param path starting path. default is "/"
    /// @param dirName directory name for printing.  required due to limitatations on viewing current filename from Dir object.
    /// @param treeString ongoing left-padding of filetree.
    void printFileTreeToSerial(String path, String dirName, String treeString); // OLD
    void printFileTreeToSerial(const char *path = "/", const char *dirName = "/", const char *treeString = "");

    template <std::size_t N>
    inline void tokenizeContainerToFileInt(File f, const std::array<uint8_t, N> &arr, int size = -1, const char *head = "", const char *tail = "")
    {
        size = arr.size();
        f.printf("<%s:%d=", head, size);
        for (int i = 0; i < size; i++)
        {
            f.printf("%d", arr[i]);
            if (i != size - 1)
                f.printf(",");
            else
                f.printf(">\n");
        }
    }
}

#endif // FSTOOLS_H