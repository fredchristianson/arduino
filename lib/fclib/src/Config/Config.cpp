#include "fclib/Config.h"
#include "fclib/File.h"
#include "Config.h"

using namespace FCLIB;

namespace FCLIB
{
    Config::Config() : log("Config")
    {
    }

    Config::~Config()
    {
        for (int i = 0; i < this->sections.size(); i++)
        {
            ConfigSection *val = this->sections.get(i);
            delete val;
        }
    }

    ConfigSection *Config::getSection(const char *name, bool createIfNeeded)
    {
        for (int i = 0; i < sections.size(); i++)
        {
            if (sections[i]->name.equalsIgnoreCase(name))
            {
                return sections[i];
            }
        }
        ConfigSection *section = createSection(name);
        return section;
    }

    ConfigSection *Config::createSection(const char *name)
    {
        ConfigSection *section = new ConfigSection(this, name);
        sections.add(section);
        return section;
    }

    String Config::get(const char *name, const char *defaultValue)
    {
        return get("default", name, defaultValue);
    }

    String Config::get(const char *sectionName, const char *name, const char *defaultValue)
    {
        ConfigSection *section = getSection(sectionName);
        ConfigValue *value = NULL;
        if (section != NULL)
        {
            value = section->getValue(name);
        }
        if (value == NULL)
        {
            section = getSection("default");
            value = section->getValue(name);
        }
        return value == NULL ? defaultValue : value->toString();
    }

    int Config::get(const char *name, int defaultValue)
    {
        return get("default", name, defaultValue);
    }

    int Config::get(const char *sectionName, const char *name, int defaultValue)
    {
        ConfigSection *section = getSection(sectionName);
        ConfigValue *value = NULL;
        if (section != NULL)
        {
            value = section->getValue(name);
        }
        if (value == NULL)
        {
            section = getSection("default");
            value = section->getValue(name);
        }
        return value == NULL ? defaultValue : value->toInt();
    }

    float Config::get(const char *name, float defaultValue)
    {
        return get("default", name, defaultValue);
    }

    float Config::get(const char *sectionName, const char *name, float defaultValue)
    {
        ConfigSection *section = getSection(sectionName);
        ConfigValue *value = NULL;
        if (section != NULL)
        {
            value = section->getValue(name);
        }
        if (value == NULL)
        {
            section = getSection("default");
            value = section->getValue(name);
        }
        return value == NULL ? defaultValue : value->toFloat();
    }

    bool Config::get(const char *name, bool defaultValue)
    {
        return get("default", name, defaultValue);
    }

    bool Config::get(const char *sectionName, const char *name, bool defaultValue)
    {
        ConfigSection *section = getSection(sectionName);
        ConfigValue *value = NULL;
        if (section != NULL)
        {
            value = section->getValue(name);
        }
        if (value == NULL)
        {
            section = getSection("default");
            value = section->getValue(name);
        }
        return value == NULL ? defaultValue : value->toBool();
    }

    void Config::set(const char *name, const char *newValue, const char *sectionName)
    {
        ConfigSection *section = getSection(sectionName, true);
        section->set(name, newValue);
    }

    void Config::set(const char *name, int newValue, const char *sectionName)
    {
        ConfigSection *section = getSection(sectionName, true);
        section->set(name, newValue);
    }

    void Config::set(const char *name, float newValue, const char *sectionName)
    {
        ConfigSection *section = getSection(sectionName, true);
        section->set(name, newValue);
    }

    void Config::set(const char *name, bool newValue, const char *sectionName)
    {
        ConfigSection *section = getSection(sectionName, true);
        section->set(name, newValue);
    }

    bool Config::isChanged()
    {
        for (int i = 0; i < sections.size(); i++)
        {
            if (sections[i]->isChanged())
            {
                return true;
            }
        }
        return false;
    }

    void Config::clearChanged()
    {
        for (int i = 0; i < sections.size(); i++)
        {
            sections[i]->clearChanged();
        }
    }
}