#include <stdlib.h>
#include <Arduino.h>
#include "fclib/System.h"
#include "fclib/Logging.h"
#include "fclib/Config.h"
#include "./Formatter.h"
#include "./Destination.h"

using namespace FCLIB;
using namespace FCLIB::Util;

char lastErrorMessage[100];
long lastErrorTime = 0;

ILogDestination *dest = new LogSerialDestination();
ILogFormatter *formatter = new LogDefaultFormatter();

Logger::Logger(const char *moduleName, LogLevel level)
{
    this->setModuleName(moduleName);
    this->level = level;
}

Logger::~Logger()
{
}

void Logger::write(int level, const char *message, va_list args) const
{
    if (dest == NULL)
    {
        Serial.println("no Log dest");
        return;
    }
    if (!shouldLog(level, message))
    {
        return;
    }

    const char *output = message;
    if (formatter)
    {
        output = formatter->format(this->moduleName.c_str(), level, message, args);
    }

    if (dest)
    {
        dest->write(output);
    }
}

void Logger::write(int level, const char *message, ...) const
{
    va_list args;
    va_start(args, message);
    write(level, message, args);
}

void Logger::debug(const char *message, ...) const
{
    va_list args;
    va_start(args, message);
    write(DEBUG_LEVEL, message, args);
}

void Logger::info(const char *message, ...) const
{
    va_list args;
    va_start(args, message);
    write(INFO_LEVEL, message, args);
}

void Logger::warn(const char *message, ...) const
{
    va_list args;
    va_start(args, message);
    write(WARN_LEVEL, message, args);
}

void Logger::error(const char *message, ...) const
{
    va_list args;
    va_start(args, message);
    write(ERROR_LEVEL, message, args);
}

void Logger::always(const char *message, ...) const
{
    va_list args;
    va_start(args, message);
    write(ALWAYS_LEVEL, message, args);
}

void Logger::never(const char *message, ...) const
{
    // ignore
}

void Logger::conditional(bool test, int level, const char *message, ...) const
{
    if (test)
    {
        va_list args;
        va_start(args, message);
        write(DEBUG_LEVEL, message, args);
    }
}

void Logger::errorNoRepeat(const char *message, ...) const
{
    if (this->lastError.equals(message))
    {
        return;
    }
    va_list args;
    va_start(args, message);
    write(ERROR_LEVEL, message, args);
}

void Logger::showMemory(const char *label) const
{
    Board *board = Board::get();
    write(INFO_LEVEL, "%s: stack=%d,  heap=%d, max block size=%d, fragmentation=%d",
          label, board->getFreeContStack(), board->getFreeHeap(), board->getMaxFreeBlockSize(), board->getHeapFragmentation());
}

void Logger::showMemory(int level, const char *label) const
{
    write(DEBUG_LEVEL, label);
}

LogLevel Logger::getLevel() const
{
    return this->level;
}

void Logger::setLevel(LogLevel level)
{
    Serial.printf("Set level %s: %d\n", this->moduleName, level);
    this->level = level;
}

Logger *Logger::setModuleName(const char *name)
{
    this->moduleName = name;
    this->ucModuleName = name;
    this->ucModuleName.toUpperCase();

    return this;
}

bool Logger::shouldLog(int level, const char *message) const
{
    if (level == NEVER_LEVEL)
    {
        return false;
    }
    if (level == ALWAYS_LEVEL)
    {
        return true;
    }
    LogLevel moduleLevel = this->getModuleLoggerLevel();
    return (moduleLevel == ALWAYS_LEVEL) || (moduleLevel >= level);
}

namespace FCLIB
{
    class LogLevelListItem
    {
    public:
        LogLevelListItem(const char *name, LogLevel level)
        {
            // Serial.printf("LogLevel %s ==> %d", name, level);
            this->name = name;
            this->name.toUpperCase();
            this->level = level;
            this->next = NULL;
        }

        bool isExactMatch(const String &ucModule)
        {
            return this->name.equalsIgnoreCase(ucModule);
        }

        bool isBetterMatch(const String &ucModule, LogLevelListItem *other)
        {
            int otherLen = 0;
            if (other != NULL)
            {
                otherLen = other->name.length();
            }
            if (ucModule.indexOf(this->name) >= 0)
            {
                return this->name.length() > otherLen;
            }
            return false;
        }

        ~LogLevelListItem() { delete next; }

        LogLevelListItem *next;
        String name;
        LogLevel level;
    };

    LogLevel defaultLogLevel = INFO_LEVEL;
    LogLevelListItem *moduleLevelList = NULL;

    void setModuleLoggerLevel(const char *moduleName, LogLevel level)
    {
        LogLevelListItem *search = moduleLevelList;
        LogLevelListItem *prev = NULL;
        while (search != NULL)
        {
            // Serial.printf("test module name %s %s", moduleName, search->name);

            if (search->name.equalsIgnoreCase(moduleName))
            {
                search->level = level;
                // Serial.printf("update log level %s to %d", moduleName, level);
                return;
            }
            prev = search;
            search = search->next;
        }
        LogLevelListItem *newItem = new LogLevelListItem(moduleName, level);
        if (prev == NULL)
        {
            // Serial.printf("first log level %s to %d", moduleName, level);

            moduleLevelList = newItem;
        }
        else
        {
            // Serial.printf("add log level %s to %d", moduleName, level);

            prev->next = newItem;
        }
    }
    void setDefaultLoggerLevel(LogLevel level)
    {
        defaultLogLevel = level;
    }
}
LogLevel Logger::getModuleLoggerLevel() const
{
    // Serial.printf("getlevel: %s", this->moduleName);

    if (this->level != DEFAULT_LEVEL)
    {
        return this->level;
    }

    LogLevelListItem *search = moduleLevelList;
    LogLevelListItem *bestMatch = NULL;
    LogLevel level = DEFAULT_LEVEL;

    while (search != NULL && level == DEFAULT_LEVEL)
    {
        if (search->isExactMatch(this->ucModuleName))
        {
            level = search->level;
            return level;
        }
        else if (search->isBetterMatch(this->ucModuleName, bestMatch))
        {
            bestMatch = search;
        }
        search = search->next;
    }
    if (bestMatch == NULL || bestMatch->level == DEFAULT_LEVEL)
    {

        return defaultLogLevel;
    }

    return bestMatch->level;
}

LogLevel getLogLevel(const String &text)
{
    String upper(text);
    upper.toUpperCase();
    if (text.indexOf("DEBUG") >= 0)
    {
        return DEBUG_LEVEL;
    }
    else if (text.indexOf("INFO") >= 0)
    {
        return INFO_LEVEL;
    }
    else if (text.indexOf("WARN") >= 0)
    {
        return WARN_LEVEL;
    }
    else if (text.indexOf("ERROR") >= 0)
    {
        return ERROR_LEVEL;
    }
    else if (text.indexOf("ALWAYS") >= 0)
    {
        return ALWAYS_LEVEL;
    }
    else if (text.indexOf("NEVER") >= 0)
    {
        return NEVER_LEVEL;
    }
    else if (text.indexOf("TEST") >= 0)
    {
        return TEST_LEVEL;
    }
    return DEFAULT_LEVEL;
}
void FCLIB::configureLogging(ConfigSection *config)
{
    Logger log("Logger Conf", DEBUG_LEVEL);
    for (int i = 0; i < config->values.size(); i++)
    {
        ConfigValue *value = config->values[i];
        LogLevel level = getLogLevel(value->toString());
        if (value->name.equalsIgnoreCase("default"))
        {
            log.always("default: %s", value->toString().c_str());
            setDefaultLoggerLevel(level);
        }
        else
        {
            log.always("%s: %s", value->name.c_str(), value->toString().c_str());
            setModuleLoggerLevel(value->name.c_str(), level);
        }
    }
}