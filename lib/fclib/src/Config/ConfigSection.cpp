#include "fclib/Config.h"
#include "fclib/File.h"
#include "Config.h"

using namespace FCLIB;

namespace FCLIB
{
    ConfigSection::ConfigSection(const char *name) : log("ConfigSection")
    {
        this->name = name;
    }

    ConfigSection::~ConfigSection()
    {
        for (int i = 0; i < this->values.size(); i++)
        {
            ConfigValue *val = this->values.get(i);
            delete val;
        }
    }

    ConfigValue *ConfigSection::getValue(const char *name)
    {
        for (int i = 0; i < values.size(); i++)
        {
            ConfigValue *val = values[i];
            if (val->name.equalsIgnoreCase(name))
            {
                return val;
            }
        }
        return NULL;
    }

    const char *ConfigSection::get(const char *name, const char *defaultValue)
    {
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            return defaultValue;
        }
        return val->toString().c_str();
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
        ConfigValue *val = this->getValue(value->name.c_str());
        if (val != NULL)
        {
            removeValue(val);
        }
        log.always("Add config value: %s=%s  [%s]", value->name.c_str(), value->toString().c_str(), this->name.c_str());
        this->values.add(value);
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
        while (i < values.size())
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
    }

    void ConfigSection::set(const char *name, const char *newValue)
    {
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            log.always("add conf value: [%s] %s=%s", this->name.c_str(), name, newValue);
            val = new ConfigValue(name, newValue);
            addValue(val);
        }
        else
        {
            log.always("replace conf value: [%s] %s=%s", this->name.c_str(), name, newValue);
            val->set(newValue);
        }
    }

    void ConfigSection::set(const char *name, bool newValue)
    {
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            log.always("add conf value: [%s] %s=%s", this->name.c_str(), name, newValue ? "true" : "false");
            val = new ConfigValue(name, newValue);
            addValue(val);
        }
        else
        {
            log.always("replace conf value: [%s] %s=%s", this->name.c_str(), name, newValue ? "true" : "false");
            val->set(newValue);
        }
    }

    void ConfigSection::set(const char *name, int newValue)
    {
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            log.always("add conf value: [%s] %s=%d", this->name.c_str(), name, newValue);
            val = new ConfigValue(name, newValue);
            addValue(val);
        }
        else
        {
            log.always("replace conf value: [%s] %s=%d", this->name.c_str(), name, newValue);
            val->set(newValue);
        }
    }

    void ConfigSection::set(const char *name, float newValue)
    {
        ConfigValue *val = this->getValue(name);
        if (val == NULL)
        {
            log.always("add conf value: [%s] %s=%f", this->name.c_str(), name, newValue);
            val = new ConfigValue(name, newValue);
            addValue(val);
        }
        else
        {
            log.always("replace conf value: [%s] %s=%f", this->name.c_str(), name, newValue);
            val->set(newValue);
        }
    }

    bool ConfigSection::isChanged()
    {
        for (int i = 0; i < values.size(); i++)
        {
            ConfigValue *val = values[i];
            if (val->isChanged())
            {
                return true;
            }
        }
        return false;
    }
}