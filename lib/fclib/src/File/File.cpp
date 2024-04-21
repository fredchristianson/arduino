#include "fclib/File.h"

using namespace FCLIB;

FCLIB::FileBase::FileBase(const char *path) : log("File")
{
    this->fileSystem = FileSystem::get();
    this->path = path;
}

FCLIB::FileBase::~FileBase()
{
    this->close();
}

void FCLIB::FileBase::close()
{
    if (this->file)
    {
        this->file.close();
    }
}

FCLIB::FileReader::FileReader(const char *path) : FileBase(path)
{
    log.setModuleName("FileReader");
    this->openRead();
    this->buffer = new char[256];
    this->bufferLen = 255;
    this->bufferPos = 0;
    memset(this->buffer, 0, this->bufferLen);
}

FCLIB::FileReader::~FileReader()
{
    delete buffer;
    file.close();
}

bool FCLIB::FileReader::readLine(String &line)
{
    log.never("readLine");
    line.clear();
    if (this->bufferPos < this->bufferLen)
    {
        int count = this->file.read((uint8_t *)this->buffer + this->bufferPos, this->bufferLen - this->bufferPos);
        log.never("read %d bytes", count);

        if (count > 0)
        {
            this->bufferPos += count;
            this->buffer[this->bufferPos] = 0;
        }
    }
    log.never("Get line buffer:");
    log.never(this->buffer);
    char *pos = this->buffer;
    if (*pos == 0)
    {
        log.never("EOF");
        return false; // EOF
    }
    while (*pos != 0 && *pos != '\r' && *pos != '\n')
    {
        pos++;
    }
    if (pos > this->buffer)
    {
        line.concat(this->buffer, pos - this->buffer);
    }
    while (*pos == '\r' || *pos == '\n')
    {
        pos++;
    }
    int keepLen = this->bufferLen - (pos - this->buffer);
    memcpy(this->buffer, pos, keepLen);
    this->buffer[keepLen] = 0;
    this->bufferPos = keepLen;
    log.never("Got line: %s", line.c_str());
    return true;
}

FCLIB::FileWriter::FileWriter(const char *path, bool append) : FileBase(path)
{
    log.setModuleName("FileWriter");
    if (append)
    {
        this->openAppend();
    }
    else
    {
        this->openWrite();
    }
}

FCLIB::FileWriter::~FileWriter()
{
    file.close();
}

bool FCLIB::FileWriter::writeLine(String &string)
{
    return writeLine(string.c_str());
}
bool FCLIB::FileWriter::writeLine(const char *string)
{
    if (!file)
    {
        return false;
    }

    file.write(string);
    log.debug("Write: '%s'", string);
    file.write("\r\n");
    return true;
}

FCLIB::FileSystem *singletonFileSysetem;

FCLIB::FileSystem *FCLIB::FileSystem::get()
{
    if (singletonFileSysetem == NULL)
    {
        singletonFileSysetem = new FileSystem();
    }
    return singletonFileSysetem;
}

FCLIB::FileSystem::FileSystem()
{
    this->mounted = LittleFS.begin();
    if (this->mounted)
    {
        singletonFileSysetem = this;
    }
}
FCLIB::FileSystem::~FileSystem()
{
}

bool FCLIB::FileSystem::isMounted() const
{
    return this->mounted;
}

bool FCLIB::FileSystem::remove(const char *path)
{
    return LittleFS.remove(path);
}

void FCLIB::FileSystem::unmount()
{

    LittleFS.end();
    delete singletonFileSysetem;
    singletonFileSysetem = NULL;
}