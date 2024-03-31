#include "fclib/Config.h"
#include "fclib/File.h"
#include "fclib/Event.h"

using namespace FCLIB;

namespace FCLIB
{
    Logger confLogger("ConfigValue");
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
                Event::trigger(EventType::CHANGE_EVENT, this->section->getConfig(), this);
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
        setChanged(changed);
    }
    void ConfigValue::set(float value)
    {
        setChanged(value != floatValue);
        this->floatValue = value;
        this->type = FLOAT_TYPE;
        setChanged(changed);
    }
    void ConfigValue::set(bool value)
    {
        setChanged(value != boolValue);
        this->boolValue = value;
        this->type = BOOL_TYPE;
        setChanged(changed);
    }

    const String &ConfigValue::toString()
    {
        switch (type)
        {
        case STRING_TYPE:
            break;
        case INT_TYPE:
            stringValue = String(intValue);
            break;
        case FLOAT_TYPE:
            stringValue = String(floatValue);
            break;
        case BOOL_TYPE:
            stringValue = boolValue ? "True" : "False";
            break;
        }
        return stringValue;
    }

    int ConfigValue::toInt()
    {
        switch (type)
        {
        case STRING_TYPE:
            intValue = atoi(stringValue.c_str());
            break;
        case INT_TYPE:
            // have it
            break;
        case FLOAT_TYPE:
            intValue = floatValue;
            break;
        case BOOL_TYPE:
            intValue = boolValue ? 1 : 0;
            break;
        }
        return intValue;
    }

    float ConfigValue::toFloat()
    {
        switch (type)
        {
        case STRING_TYPE:
            floatValue = atof(stringValue.c_str());
            break;
        case INT_TYPE:
            floatValue = intValue;
            break;
        case FLOAT_TYPE:
            // have it
            break;
        case BOOL_TYPE:
            floatValue = boolValue ? 1 : 0;
            break;
        }
        return floatValue;
    }

    bool ConfigValue::toBool()
    {
        switch (type)
        {
        case STRING_TYPE:
            boolValue = !(stringValue.length() == 0 || stringValue.equalsIgnoreCase("false") || stringValue.equals("0"));
            break;
        case INT_TYPE:
            boolValue = intValue != 0;
            break;
        case FLOAT_TYPE:
            boolValue = floatValue != 0;
            break;
        case BOOL_TYPE:
            // have it
            break;
        }
        return boolValue;
    }
}