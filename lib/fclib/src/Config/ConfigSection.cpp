#include "fclib/Config.h"
#include "fclib/File.h"
#include "fclib/Util.h"

using namespace FCLIB;

namespace FCLIB
{
    ConfigSection::ConfigSection(Config *config, const char *name) : log("ConfigSection")
    {
        log.debug("Config section %s", name);
        this->name = name;
        this->config = config;
    }

    ConfigSection::~ConfigSection()
    {
        log.never("~ConfigSection() %s", name);
        values.deleteAll();
    }

    void ConfigSection::clearChanged()
    {
        log.never("clearChanged() section 0x%lx", this);
        for (int i = 0; i < this->values.size() && LoopTime::ok(); i++)
        {
            log.never("call clearChanged val %d of %d", i, this->values.size());
            ConfigValue *val = this->values[i];
            log.conditional(val == NULL, ALWAYS_LEVEL, "val is NULL");
            log.never("value 0x%lx", val);
            log.never("\tname=%s", val->getName());
            val->clearChanged();
            log.never("value cleared");
        }
        LoopTime::check("ConfigSection::clearChanged()");

        log.debug("all cleared");
    }

    ConfigValue *ConfigSection::getValue(const char *name)
    {
        for (int i = 0; i < values.size() && LoopTime::ok(); i++)
        {
            ConfigValue *val = values[i];
            if (Util::equalIgnoreCase(val->getName(), name))
            {
                return val;
            }
        }
        LoopTime::check("ConfigSection::getValue()");

        return NULL;
    }

    const char *ConfigSection::get(const char *name, const char *defaultValue)
    {
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            return defaultValue;
        }
        return val->toString();
    }

    int ConfigSection::get(const char *name, int defaultValue)
    {
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            return defaultValue;
        }
        return val->toInt();
    }

    float ConfigSection::get(const char *name, float defaultValue)
    {
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            return defaultValue;
        }
        return val->toFloat();
    }

    bool ConfigSection::get(const char *name, bool defaultValue)
    {
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            return defaultValue;
        }
        return val->toBool();
    }

    void ConfigSection::addValue(ConfigValue *value)
    {
        log.always("add value");
        value->setSection(this);
        ConfigValue *val = this->getValue(value->getName());
        log.always("current %x", val);

        if (val != NULL)
        {
            log.always("remove");
            removeValue(val);
            log.always("\tremoved");
        }
        log.always("Add config value: %s=%s  [%s]", value->getName(), value->toString(), this->name.c_str());
        this->values.add(value);
        log.always("added");
    }

    void ConfigSection::deleteValue(const char *value)
    {
        ConfigValue *val = this->getValue(value);
        if (val != NULL)
        {
            removeValue(val);
        }
    }

    void ConfigSection::removeValue(ConfigValue *val)
    {
        int i = 0;
        while (i < values.size() && LoopTime::ok())
        {
            if (values[i] == val)
            {
                values.remove(i);
                delete val;
            }
            else
            {
                i++;
            }
        }
        LoopTime::check("ConfigSection::removeValue()");
    }

    void ConfigSection::set(const char *name, const char *newValue)
    {
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            log.debug("add conf value: [%s] %s=%s", this->name.c_str(), name, newValue);
            val = new ConfigValue(this, name, newValue);
            addValue(val);
        }
        else
        {
            log.debug("replace conf value: [%s] %s=%s", this->name.c_str(), name, newValue);
            val->setSection(this);
            val->set(newValue);
        }
    }

    void ConfigSection::set(const char *name, bool newValue)
    {
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            log.debug("add conf value: [%s] %s=%s", this->name.c_str(), name, newValue ? "true" : "false");
            val = new ConfigValue(this, name, newValue);
            addValue(val);
        }
        else
        {
            log.debug("replace conf value: [%s] %s=%s", this->name.c_str(), name, newValue ? "true" : "false");
            val->setSection(this);

            val->set(newValue);
        }
    }

    void ConfigSection::set(const char *name, int newValue)
    {
        log.never("set %s=%d", name, newValue);
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            log.never("add conf value: [%s] %s=%d", this->name.c_str(), name, newValue);
            val = new ConfigValue(this, name, newValue);
            addValue(val);
        }
        else
        {
            val->setSection(this);

            log.never("replace conf value: [%s] %s=%d", this->name.c_str(), name, newValue);

            val->set(newValue);
        }
        log.never("set done");
    }

    void ConfigSection::set(const char *name, float newValue)
    {
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            log.debug("add conf value: [%s] %s=%f", this->name.c_str(), name, newValue);
            val = new ConfigValue(this, name, newValue);
            addValue(val);
        }
        else
        {
            val->setSection(this);

            log.debug("replace conf value: [%s] %s=%f", this->name.c_str(), name, newValue);

            val->set(newValue);
        }
    }

    bool ConfigSection::isChanged()
    {
        for (int i = 0; i < values.size() && LoopTime::ok(); i++)
        {
            ConfigValue *val = values[i];
            if (val->isChanged())
            {
                return true;
            }
        }
        LoopTime::check("ConfigSection::isChanged()");

        return false;
    }
}