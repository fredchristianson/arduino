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

        return new ConfigValue(NULL, name.c_str(), val.c_str());
    }

    ConfigFile::ConfigFile()
    {
        log.setModuleName("ConfigFile");
    }

    FCLIB::ConfigFile::~ConfigFile()
    {
        log.never("~ConfigFile");
    }

    bool FCLIB::ConfigFile::load(const char *filePath)
    {
        log.always("Load", filePath);
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
            log.always("\tgot line: %s", line.c_str());
            line.trim();
            if (line.startsWith("#"))
            {
                log.always("ignore comment");
            }
            else if (line.startsWith("["))
            {
                String sectionName = line.substring(1, line.length() - 1);
                if (sectionName.length() == 0)
                {
                    sectionName = "unnamed";
                }
                section = getSection(sectionName.c_str(), true);
                log.always("Config Section: %s", sectionName.c_str());
            }
            else
            {
                ConfigValue *value = parseLine(line);
                if (value != NULL)
                {
                    log.always("\t\tName: '%s'    Value: '%s'  Section: ['%s']", value->getName(), value->toString(), section->getName());
                    section->addValue(value);
                }
            }
        }
        log.always("clear changes");
        clearChanged();
        log.always("load complete");
        return true;
    }

    bool ConfigFile::save(const char *filePath)
    {
        FileWriter writer(filePath);
        for (int i = 0; i < sections.size(); i++)
        {
            ConfigSection *section = sections.getAt(i);
            String line = "[";
            line.concat(section->getName());
            line.concat("]");
            writer.writeLine(line);
            const List<ConfigValue> values = section->getValues();
            for (int j = 0; j < values.size(); j++)
            {
                const ConfigValue *val = values[j];
                line = val->getName();
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
        log.debug("Save config: %s", this->filePath.c_str());
        return this->save(this->filePath.c_str());
    }
}
