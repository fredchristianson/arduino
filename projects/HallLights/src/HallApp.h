#ifndef _HALLAPP_H_
#define _HALLAPP_H_
#include "fclib/App.h"
#include "fclib/Config.h"

using namespace FCLIB;

class HallAppSetup : public AppSetup
{
public:
    bool beginSetup();
};

class HallAppLoop : public AppLoop
{
public:
    bool beginSetup();

protected:
    bool loopExecute() override;
};

class HallApp : public App
{
public:
    HallApp();
    virtual ~HallApp();

protected:
    AppLoop *createLoop() override;
    AppSetup *createSetup() override;
};

#endif