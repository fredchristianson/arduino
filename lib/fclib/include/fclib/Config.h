#ifndef _FCLIB_CONFIG_H_
#define _FCLIB_CONFIG_H_
#include <WString.h>
#include "fclib/LinkedList.h"
#include "fclib/Logging.h"
#include "fclib/Test.h"

namespace FCLIB
{
    class ConfigFile;

    struct ConfigValue
    {
    public:
        ConfigValue(const char *name, const char *value, const char *section = NULL);

        String name;
        String value;
        String section; // not used

        bool matchesName(const char *name) const
        {
            return this->name.equalsIgnoreCase(name);
        }

        bool matchesSection(const char *section) const
        {
            if (section == NULL)
            {
                return this->isDefaultSection();
            }
            else
            {
                String match(section);
                match.trim();
                if (match.length() == 0 || match.equalsIgnoreCase("default"))
                {
                    return this->isDefaultSection();
                }
                return this->section.equalsIgnoreCase(match);
            }
        }

        bool isDefaultSection() const
        {
            return section.length() == 0 || section.equalsIgnoreCase("default");
        }
    };

    class Config
    {
    public:
        Config();
        virtual ~Config();

        String getString(const char *name, const char *defaultValue = NULL);
        String getString(const char *section, const char *name, const char *defaultValue);

        void set(const char *name, const char *value, const char *section = "default");

    protected:
        LinkedList<ConfigValue *> values;
        void addValue(ConfigValue *value);
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