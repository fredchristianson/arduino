#ifndef _FCLIB_CONFIG_H_
#define _FCLIB_CONFIG_H_
#include <WString.h>
#include "fclib/Logging.h"
#include "fclib/Test.h"
#include "fclib/List.h"

namespace FCLIB
{
    class ConfigFile;
    class ConfigSection;
    class Config;

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
        ConfigValue(ConfigSection *section, const char *name, const char *value)
        {
            this->section = section;
            this->name = name;
            set(value);
            this->changed = false;
        }

        ConfigValue(ConfigSection *section, const char *name, int value)
        {
            this->section = section;
            this->name = name;
            set(value);
            this->changed = false;
        }

        ConfigValue(ConfigSection *section, const char *name, float value)
        {
            this->section = section;
            this->name = name;
            set(value);
            this->changed = false;
        }

        ConfigValue(ConfigSection *section, const char *name, bool value)
        {
            this->section = section;
            this->name = name;
            set(value);
            this->changed = false;
        }

        void set(const char *value);
        void set(int value);
        void set(float value);
        void set(bool value);

        const String &toString();
        int toInt();
        float toFloat();
        bool toBool();
        bool isChanged() const { return changed; }

        String name;
        ValueType type;

        void setChanged(bool isChanged = true);
        String stringValue;
        int intValue;
        float floatValue;
        bool boolValue;
        bool changed;
        ConfigSection *section;
    };

    struct ConfigSection
    {
        ConfigSection(Config *config, const char *name);
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
        bool isChanged();
        String name;
        List<ConfigValue> values;
        Logger log;
        void clearChanged();
        Config *getConfig() { return config; }

    private:
        void removeValue(ConfigValue *val);
        Config *config;
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

        void set(const char *name, const char *value, const char *section = "default");
        void set(const char *name, int value, const char *section = "default");
        void set(const char *name, float value, const char *section = "default");
        void set(const char *name, bool value, const char *section = "default");
        bool isChanged();

        ConfigValue *getConfigValue(const char *name, const char *section = "default")
        {
            return this->getSection(section, true)->getValue(name);
        }

        virtual bool save()
        {
            log.debug("Derivfed class did not implement save()");
            return true;
        } // derived classes may save

        void clearChanged();

    protected:
        List<ConfigSection> sections;
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
        bool save() override;

    private:
        String filePath;
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