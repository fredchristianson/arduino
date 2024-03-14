#ifndef _FCLIB_CONFIG_H_
#define _FCLIB_CONFIG_H_
#include <WString.h>
#include "fclib/LinkedList.h"
#include "fclib/Logging.h"
#include "fclib/Test.h"

namespace FCLIB
{
    class ConfigFile;

    enum ValueType
    {
        STRING_TYPE,
        INT_TYPE,
        FLOAT_TYPE,
        BOOL_TYPE
    };

    struct ConfigValue
    {
    public:
        ConfigValue(const char *name, const char *value)
        {
            this->name = name;
            set(value);
        }

        ConfigValue(const char *name, int value)
        {
            this->name = name;
            set(value);
        }

        ConfigValue(const char *name, float value)
        {
            this->name = name;
            set(value);
        }

        ConfigValue(const char *name, bool value)
        {
            this->name = name;
            set(value);
        }

        void set(const char *value);
        void set(int value);
        void set(float value);
        void set(bool value);

        const String &toString();

        int toInt();

        float toFloat();
        bool toBool();

        String name;
        ValueType type;

        String stringValue;
        int intValue;
        float floatValue;
        bool boolValue;
    };

    struct ConfigSection
    {
        ConfigSection(const char *name);
        ~ConfigSection();
        void set(const char *name, const char *value);
        void set(const char *name, int value);
        void set(const char *name, float value);
        void set(const char *name, bool value);

        ConfigValue *getValue(const char *name);
        void deleteValue(const char *name);

        const char *get(const char *name, const char *defaultValue);
        int get(const char *name, int defaultValue);
        float get(const char *name, float defaultValue);
        bool get(const char *name, bool defaultValue);

        void addValue(ConfigValue *value);

        String name;
        LinkedList<ConfigValue *> values;
        Logger log;

    private:
        void removeValue(ConfigValue *val);
    };

    class Config
    {
    public:
        Config();
        virtual ~Config();

        ConfigSection *getSection(const char *name, bool createIfNeeded = false);

        String get(const char *name, const char *defaultValue = NULL);
        String get(const char *section, const char *name, const char *defaultValue);

        int get(const char *name, int defaultValue = 0);
        int get(const char *section, const char *name, int defaultValue);

        float get(const char *name, float defaultValue = 0);
        float get(const char *section, const char *name, float defaultValue);

        bool get(const char *name, bool defaultValue = false);
        bool get(const char *section, const char *name, bool defaultValue);

        template <typename T>
        T get(const char *name, const char *section = "default");

        void set(const char *name, const char *value, const char *section = "default");
        void set(const char *name, int value, const char *section = "default");
        void set(const char *name, float value, const char *section = "default");
        void set(const char *name, bool value, const char *section = "default");

    protected:
        LinkedList<ConfigSection *> sections;
        ConfigSection *createSection(const char *name);
        Logger log;
    };

    class ConfigFile : public Config
    {
    public:
        ConfigFile();
        virtual ~ConfigFile();

        bool load(const char *filePath);
        bool save(const char *filePath);
    };

    namespace TEST
    {
        namespace CONFIG
        {
            class ConfigTestSuite : public TestSuite
            {
            public:
                ConfigTestSuite();
                virtual ~ConfigTestSuite();

            protected:
                virtual void runTests() override;
                void run();
            };
        }
    }
}
#endif