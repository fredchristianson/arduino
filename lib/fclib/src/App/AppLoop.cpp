#include <stdlib.h>
#include "fclib/App.h"
#include "fclib/System.h"
#include <Esp.h>

namespace FCLIB
{

    AppLoop *singletonAppLoop = NULL;

    void AppLoop::addTask(LoopTask *task)
    {
        Logger log("AppLoop");
        switch (task->getStage())
        {
        case LOOP_BEGIN:
            singletonAppLoop->beforeTasks.add(task);
            break;
        case LOOP_NORMAL:
            log.never("Add LoopTask 0x%lx", task);
            singletonAppLoop->loopTasks.add(task);
            break;
        case LOOP_END:
            singletonAppLoop->afterTasks.add(task);
            break;
        }
    }

    void AppLoop::removeTask(LoopTask *task)
    {
        Logger log("AppLoop");
        log.never("remove loop task 0x%lx", task);
        switch (task->getStage())
        {
        case LOOP_BEGIN:
            singletonAppLoop->beforeTasks.remove(task);
            break;
        case LOOP_NORMAL:
            singletonAppLoop->loopTasks.remove(task);
            break;
        case LOOP_END:
            singletonAppLoop->afterTasks.remove(task);
            break;
        }
    }
    // todo: setup watchdog timer
    AppLoop::AppLoop() : AppComponent(), log("AppLoop")
    {
        running = true;
        singletonAppLoop = this;
    }

    AppLoop::~AppLoop()
    {
    }

    void AppLoop::run()
    {
        if (!this->running)
        {
            log.debug("app is stopped");
            return;
        }
        this->beforeLoop();
        getEventManager()->processEvents();
        this->runTasks();
        this->afterLoop();
    }

    void AppLoop::beforeLoop()
    {
        beforeTasks.updateTaskStatus();
        beforeTasks.runTasks();
    }
    void AppLoop::runTasks()
    {
        loopTasks.updateTaskStatus();
        loopTasks.runTasks();
    }
    void AppLoop::afterLoop()
    {
        afterTasks.updateTaskStatus();
        afterTasks.runTasks();
    }

    bool AppLoop::setup(Config *config)
    {

        return true;
    }
}