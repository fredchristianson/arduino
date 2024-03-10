#include <cstdarg>
#ifndef LOGGER_H
#define LOGGER_H

namespace FCLIB
{
    enum LogLevel
    {
        DEBUG_LEVEL = 100,
        INFO_LEVEL = 80,
        WARN_LEVEL = 60,
        ERROR_LEVEL = 40,
        ALWAYS_LEVEL = 1,
        TEST_LEVEL = -1, // only & always matches during tests
        NEVER_LEVEL = -2,
        CONDITION_LEVEL = -3,
        DEFAULT_LEVEL = -4
    };

    void setModuleLoggerLevel(const char *moduleName, LogLevel level);
    void setDefaultLoggerLevel(LogLevel level);

    class Logger
    {
    public:
        static void setModuleLevel(const char *moduleName, LogLevel level);
        static Logger &create(const char *moduleName, LogLevel level = DEBUG_LEVEL) { return *(new Logger(moduleName, level)); }

        Logger(const char *moduleName, LogLevel level = DEFAULT_LEVEL);

        virtual ~Logger();

        virtual void write(int level, const char *message, va_list args) const;
        virtual void write(int level, const char *message, ...) const;

        virtual void debug(const char *message, ...) const;
        virtual void info(const char *message, ...) const;
        virtual void warn(const char *message, ...) const;
        virtual void error(const char *message, ...) const;
        virtual void always(const char *message, ...) const;
        virtual void never(const char *message, ...) const;
        virtual void conditional(bool test, int level, const char *message, ...) const;
        virtual void errorNoRepeat(const char *message, ...) const;
        virtual void showMemory(const char *label = "Memory") const;
        virtual void showMemory(int level, const char *label = "Memory") const;

        virtual LogLevel getLevel() const;
        virtual void setLevel(LogLevel level);

        virtual bool shouldLog(int level, const char *m_name, const char *message) const;

        Logger *setModuleName(const char *name);

    private:
        LogLevel getModuleLoggerLevel() const;

        const char *moduleName;
        const char *lastError;
        LogLevel level;
    };

}
#endif
