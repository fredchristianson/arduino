#include "fclib/Config.h"
#include "fclib/File.h"
#include "Config.h"

using namespace FCLIB;

FCLIB::ConfigValue::ConfigValue(const char *name, const char *value, const char *section)
{
	this->name = name;
	this->value = value;
	this->section = section;
}

FCLIB::Config::Config() : log("Config")
{
}

FCLIB::Config::~Config()
{
	for (int i = 0; i < this->values.size(); i++)
	{
		ConfigValue *val = this->values.get(i);
		delete val;
	}
}

String FCLIB::Config::getString(const char *name, const char *defaultValue)
{
	return this->getString("default", name, defaultValue);
}

String FCLIB::Config::getString(const char *section, const char *name, const char *defaultValue)
{
	ConfigValue *value = NULL;
	ConfigValue *defaultSectionValue = NULL;
	log.debug("Get value as string: %s for section %s", name, section);
	for (int i = 0; i < this->values.size() && value == NULL; i++)
	{
		ConfigValue *val = this->values.get(i);
		if (val->matchesName(name))
		{
			if (val->matchesSection(section))
			{
				value = val;
			}
			else if (val->isDefaultSection())
			{
				defaultSectionValue = val;
			}
		}
		log.debug("\tcheck value: %s [%s]", val->name.c_str(), val->section.c_str());
	}
	if (value != NULL)
	{
		return value->value;
	}
	else if (defaultSectionValue != NULL)
	{
		return defaultSectionValue->value;
	}
	return defaultValue;
}

void FCLIB::Config::addValue(ConfigValue *value)
{
	log.debug("Add config value: %s=%s  [%s]", value->name.c_str(), value->value.c_str(), value->section.c_str());
	this->values.add(value);
}

void FCLIB::Config::set(const char *name, const char *newValue, const char *section)
{
	ConfigValue *value = NULL;
	log.debug("set value: %s=%s for section %s", name, newValue, section);
	bool found = false;
	for (int i = 0; i < this->values.size() && value == NULL; i++)
	{
		ConfigValue *val = this->values.get(i);
		if (val->matchesName(name) && val->matchesSection(section))
		{
			if (found)
			{
				// only keep the first
				val->section = "***delete***";
			}
			else
			{
				val->value = newValue;
				found = true;
			}
		}
	}
}