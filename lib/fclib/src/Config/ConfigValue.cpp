#include "fclib/Config.h"
#include "fclib/File.h"
#include "fclib/Event.h"

using namespace FCLIB;

namespace FCLIB
{
    Logger confLogger("ConfigValue");

    ConfigValue::~ConfigValue()
    {
    }

    void ConfigValue::set(const char *value)
    {
        setChanged(!this->stringValue.equals(value));
        this->stringValue = value;
        this->stringValue.trim();
        this->type = STRING_TYPE;
        setChanged(changed);
    }

    void ConfigValue::setChanged(bool isChanged)
    {
        changed = isChanged;
        if (changed)
        {
            confLogger.never(" change %lx", this->section);
            Event::trigger(EventType::CHANGE_EVENT, this, this);
            if (this->section != NULL)
            {
                confLogger.never("section change %x %x", this->section, this->section->getConfig());
                Event::trigger(EventType::CHANGE_EVENT, this->section, this);
                if (this->section->getConfig() != NULL)
                {
                    Event::trigger(EventType::CHANGE_EVENT, this->section->getConfig(), this);
                }
            }
        }
        else
        {
            confLogger.never("no change");
        }
    }
    void ConfigValue::set(int value)
    {
        setChanged(intValue != value);
        this->intValue = value;
        this->type = INT_TYPE;
    }
    void ConfigValue::set(float value)
    {
        setChanged(value != floatValue);
        this->floatValue = value;
        this->type = FLOAT_TYPE;
    }
    void ConfigValue::set(bool value)
    {
        setChanged(value != boolValue);
        this->boolValue = value;
        this->type = BOOL_TYPE;
    }

    const char *ConfigValue::toString() const
    {
        String sval = "";
        switch (type)
        {
        case STRING_TYPE:
            sval = stringValue;
            break;
        case INT_TYPE:
            sval = String(intValue);
            break;
        case FLOAT_TYPE:
            sval = String(floatValue);
            break;
        case BOOL_TYPE:
            sval = boolValue ? "True" : "False";
            break;
        }
        String *sv = (String *)&stringValue;
        *sv = sval;
        return stringValue.c_str();
    }

    int ConfigValue::toInt()
    {
        int val = 0;
        switch (type)
        {
        case STRING_TYPE:
            val = atoi(stringValue.c_str());
            break;
        case INT_TYPE:
            val = intValue;
            break;
        case FLOAT_TYPE:
            val = floatValue;
            break;
        case BOOL_TYPE:
            val = boolValue ? 1 : 0;
            break;
        }
        return val;
    }

    float ConfigValue::toFloat()
    {
        float val = 0;
        switch (type)
        {
        case STRING_TYPE:
            val = atof(stringValue.c_str());
            break;
        case INT_TYPE:
            val = intValue;
            break;
        case FLOAT_TYPE:
            val = floatValue;
            break;
        case BOOL_TYPE:
            val = boolValue ? 1 : 0;
            break;
        }
        return val;
    }

    bool ConfigValue::toBool()
    {
        bool val = false;
        switch (type)
        {
        case STRING_TYPE:
            val = !(stringValue.length() == 0 || stringValue.equalsIgnoreCase("false") || stringValue.equals("0"));
            break;
        case INT_TYPE:
            val = intValue != 0;
            break;
        case FLOAT_TYPE:
            val = floatValue != 0;
            break;
        case BOOL_TYPE:
            val = boolValue;
            break;
        }
        return val;
    }
}