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
        log.always("Load %s", filePath);
        this->filePath = filePath;
        FileReader reader(filePath);
        if (!reader.isOpen())
        {
            return false;
        }
        String line;
        ConfigSection *section = getSection("default", true);
        log.always("got section %x", section);
        while (reader.readLine(line) && LoopTime::ok())
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
                    log.always("\t\tName: '%s'    Value: '%s'  Section: %x ['%s']", value->getName(), value->toString(), section, section->getName());
                    section->addValue(value);
                }
            }
        }
        log.always("clear changes");
        clearChanged();
        log.never("load complete");
        return true;
    }

    bool ConfigFile::save(const char *filePath)
    {
        return true;
        log.never("save file %s", filePath);
        FileWriter writer(filePath);
        for (int i = 0; i < sections.size() && LoopTime::ok(); i++)
        {
            ConfigSection *section = sections.getAt(i);
            log.never("save section 0x%lx %s", section, section->getName());
            String line = "[";
            line.concat(section->getName());
            line.concat("]");
            writer.writeLine(line);
            for (int j = 0; j < section->valueCount() && LoopTime::ok(); j++)
            {
                const ConfigValue *val = section->getValueAt(j);
                log.conditional(val, ALWAYS_LEVEL, "save value 0x%lx %s=%s", val, val->getName(), val->toString());
                log.conditional(val == NULL, ALWAYS_LEVEL, "value is NULL");
                line = val->getName();
                line += "=";
                line += val->toString();
                writer.writeLine(line);
            }
            writer.writeLine("");
        }
        LoopTime::check("ConfigFile::save()");
        log.never("clearChanged()");
        clearChanged();
        log.never("save done");
        return true;
    }

    bool ConfigFile::save()
    {
        log.debug("Save config: %s", this->filePath.c_str());
        return this->save(this->filePath.c_str());
    }
}
