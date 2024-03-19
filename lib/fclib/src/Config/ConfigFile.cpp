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
        this->filePath = filePath;
        FileReader reader(filePath);
        if (!reader.isOpen())
        {
            return false;
        }
        String line;
        ConfigSection *section = getSection("default", true);
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
                String sectionName = line.substring(1, line.length() - 1);
                section = getSection(sectionName.c_str(), true);
                log.debug("Config Section: %s", sectionName.c_str());
            }
            else
            {
                ConfigValue *value = parseLine(line);
                if (value != NULL)
                {
                    log.debug("\t\tName: '%s'    Value: '%s'  Section: ['%s']", value->name.c_str(), value->toString().c_str(), section->name.c_str());
                    section->addValue(value);
                }
            }
        }
        clearChanged();
        return true;
    }

    bool ConfigFile::save(const char *filePath)
    {
        FileWriter writer(filePath);
        for (int i = 0; i < sections.size(); i++)
        {
            ConfigSection *section = sections[i];
            String line = "[";
            line.concat(section->name);
            line.concat("]");
            writer.writeLine(line);
            for (int j = 0; j < section->values.size(); j++)
            {
                ConfigValue *val = section->values[j];
                line = val->name;
                line += "=";
                line += val->toString();
                writer.writeLine(line);
            }
            writer.writeLine("");
        }
        clearChanged();

        return true;
    }

    bool ConfigFile::save()
    {
        log.always("Save config: %s", this->filePath.c_str());
        return this->save(this->filePath.c_str());
    }
}
