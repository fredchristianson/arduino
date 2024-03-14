#include "fclib/Config.h"
#include "fclib/File.h"
#include "Config.h"

using namespace FCLIB;

namespace FCLIB
{

	void ConfigValue::set(const char *value)
	{
		this->stringValue = value;
		this->stringValue.trim();
		this->type = STRING_TYPE;
	}
	void ConfigValue::set(int value)
	{
		this->intValue = value;
		this->type = INT_TYPE;
	}
	void ConfigValue::set(float value)
	{
		this->floatValue = value;
		this->type = FLOAT_TYPE;
	}
	void ConfigValue::set(bool value)
	{
		this->boolValue = value;
		this->type = BOOL_TYPE;
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