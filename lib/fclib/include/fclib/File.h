#ifndef _FCLIB_FILE_H_
#define _FCLIB_FILE_H_
#include <WString.h>
#include <LittleFS.h>
#include "fclib/Logging.h"

namespace FCLIB
{
    class FileBase;

    class FileSystem
    {
    public:
        static FileSystem *get();

        void unmount();
        void format();

        bool isMounted() const;

    protected:
        FileSystem();
        ~FileSystem();

        friend FileBase;

    private:
        bool mounted;
    };

    class FileBase
    {
    public:
        FileBase(const char *path);
        virtual ~FileBase();

        bool open(const char *mode)
        {
            this->file = LittleFS.open(this->path, mode);
            return this->file;
        }

        bool openRead() { return this->open("r"); }
        bool openWrite() { return this->open("w"); }
        bool openAppend() { return this->open("a"); }

        bool exists() const { return LittleFS.exists(this->path); }

        void close();
        bool isOpen() { return this->file; }

    protected:
        File file;
        String path;
        FileSystem *fileSystem;
        Logger log;
    };

    class FileReader : public FileBase
    {
    public:
        FileReader(const char *path);
        virtual ~FileReader();

        bool readLine(String &line);

    private:
        char *buffer;
        int bufferPos;
        int bufferLen;
    };

    class FileWriter : public FileBase
    {
    public:
        FileWriter(const char *path, bool append = false);
        virtual ~FileWriter();

        bool writeLine(String &line);
        bool writeLine(const char *line);

    private:
    };
}
#endif