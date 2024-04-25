#include "fclib/Config.h"
#include "fclib/File.h"
#include "fclib/Util.h"

using namespace FCLIB;

namespace FCLIB
{
    Config::Config() : log("Config")
    {
    }

    Config::~Config()
    {
        sections.deleteAll();
    }

    ConfigSection *Config::getSection(const char *name, bool createIfNeeded)
    {
        log.debug("find section %s", name);
        for (int i = 0; i < sections.size(); i++)
        {
            log.debug("\ttry %s", sections[i]->getName());
            if (Util::equalIgnoreCase(sections[i]->getName(), name))
            {
                return sections[i];
            }
        }
        LoopTime::check("Config::getSection");
        ConfigSection *section = createSection(name);
        log.debug("return section %x", section);
        return section;
    }

    ConfigSection *Config::createSection(const char *name)
    {
        log.info("Create new section: %s", name);
        ConfigSection *section = new ConfigSection(this, name);
        sections.add(section);
        log.never("section created");
        return section;
    }

    const char *Config::get(const char *name, const char *defaultValue)
    {
        return get("default", name, defaultValue);
    }

    const char *Config::get(const char *sectionName, const char *name, const char *defaultValue)
    {
        ConfigSection *section = getSection(sectionName);
        ConfigValue *value = NULL;
        if (section != NULL)
        {
            log.debug("get section value [%s]%s", section->getName(), name);
            value = section->getValue(name);
        }
        if (value == NULL)
        {
            log.debug("get default value");
            section = getSection("default");
            value = section->getValue(name);
        }
        log.conditional(value == NULL, DEBUG_LEVEL, "no value, use default %s", defaultValue);
        log.conditional(value != NULL, DEBUG_LEVEL, "return value %s", value->toString());
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
        for (int i = 0; i < sections.size() && LoopTime::ok(); i++)
        {
            if (sections[i]->isChanged())
            {
                return true;
            }
        }
        LoopTime::check("Config::isChanged");
        return false;
    }

    void Config::clearChanged()
    {
        for (int i = 0; i < sections.size() && LoopTime::ok(); i++)
        {
            log.never("clear changed %s", sections[i]->getName());
            sections[i]->clearChanged();
            log.never("\tsection cleared");
        }
        LoopTime::check("Config::clearChanges");
    }
}