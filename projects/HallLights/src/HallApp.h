#ifndef _HALLAPP_H_
#define _HALLAPP_H_
#include "fclib/App.h"
#include "fclib/Config.h"

using namespace FCLIB;

class HallApp : public App
{
public:
    HallApp();
    virtual ~HallApp();

protected:
    virtual bool beginSetup();
    virtual bool endSetup();
    virtual bool setupNetwork();
    virtual bool setupDevices();
    virtual bool loopExecute();
};

#endif