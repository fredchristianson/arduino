#include "fclib/Config.h"
#include "fclib/File.h"

#include "fclib/Logging.h"

using namespace FCLIB;

namespace FCLIB
{

	ConfigValue *parseLine(String &line)
	{
		line.trim();
		if (line[0] == '#')
		{
			return NULL;
		}

		int pos = line.indexOf('=');
		if (pos < 0)
		{
			return NULL;
		}

		String name = line.substring(0, pos);
		String val = line.substring(pos + 1);
		name.trim();
		val.trim();

		return new ConfigValue(name.c_str(), val.c_str());
	}

	ConfigFile::ConfigFile()
	{
		log.setModuleName("ConfigFile");
	}

	FCLIB::ConfigFile::~ConfigFile()
	{
	}

	bool FCLIB::ConfigFile::load(const char *filePath)
	{
		log.debug("Load", filePath);
		FileReader reader(filePath);
		if (!reader.isOpen())
		{
			return false;
		}
		String line;
		String section = "default";
		while (reader.readLine(line))
		{
			log.debug("\tgot line: %s", line.c_str());
			line.trim();
			if (line.startsWith("#"))
			{
				log.debug("ignore comment");
			}
			else if (line.startsWith("["))
			{
				section = line.substring(1, line.length() - 1);
				log.debug("Config Section: %s", section.c_str());
			}
			else
			{
				ConfigValue *value = parseLine(line);
				if (value != NULL)
				{
					value->section = section;
					log.debug("\t\tName: '%s'    Value: '%s'  Section: ['%s']", value->name.c_str(), value->value.c_str(), value->section.c_str());
					this->addValue(value);
				}
			}
		}
		return true;
	}
}
