#include <Arduino.h>
#include "fclib/Persistance.h"
#include <esp.h>

using namespace FCLIB;

namespace FCLIB
{
    Persist::Persist() : log("Persist")
    {
        load("/persist.ini");
        listener.handle(EventType::CHANGE_EVENT, this, [this](Event *event)
                        { this->persistChanges(); });
    }
    Persist::~Persist()
    {
    }

    void Persist::persistChanges()
    {
        log.info("persistChanges");
        save("/persist.ini");
    }

    Persist *Persist::singleton = NULL;
    Persist *Persist::get()
    {
        if (singleton == NULL)
        {
            singleton = new Persist();
        }
        return singleton;
    }

    void Persist::store()
    {
        get()->persistChanges();
    }
    String Persist::get(const char *owner, const char *name, const char *defaultValue)
    {
        Config *cfg = get();
        return cfg->get(owner, name, defaultValue);
    }

    int Persist::get(const char *owner, const char *name, int defaultValue)
    {
        Config *cfg = get();
        return cfg->get(owner, name, defaultValue);
    }

    float Persist::get(const char *owner, const char *name, float defaultValue)
    {
        Config *cfg = get();
        return cfg->get(owner, name, defaultValue);
    }

    bool Persist::get(const char *owner, const char *name, bool defaultValue)
    {
        Config *cfg = get();
        return cfg->get(owner, name, defaultValue);
    }

    void Persist::set(const char *owner, const char *name, const char *value)
    {
        Config *cfg = get();
        cfg->set(name, value, owner);
    }
    void Persist::set(const char *owner, const char *name, int value)
    {
        Config *cfg = get();
        cfg->set(name, value, owner);
    }
    void Persist::set(const char *owner, const char *name, float value)
    {
        Config *cfg = get();
        cfg->set(name, value, owner);
    }
    void Persist::set(const char *owner, const char *name, bool value)
    {
        Config *cfg = get();
        cfg->set(name, value, owner);
    }
}