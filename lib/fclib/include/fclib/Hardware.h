#ifndef __FCLIB_HARDWARE_
#define __FCLIB_HARDWARE_
#include <c_types.h>
#include "fclib/Logging.h"
#include "fclib/Task.h"
#include "fclib/Event.h"

namespace FCLIB
{
#define UNDEFINED_PIN -1
#define DEBOUNCE_MSECS 50
    namespace HW // Hardware
    {

        class IBinarySensor
        {
        public:
            virtual bool isOn() const = 0;
            virtual bool isOff() const = 0;
        };
        /* Implementations */
        class ComponentBase
        {
        public:
            ComponentBase();
            virtual ~ComponentBase();

        protected:
            Logger log;
        };

        class PinComponent : public ComponentBase
        {
        public:
            PinComponent();
            virtual ~PinComponent();

            void setPin(int8 pin);
            int8 getPin() const { return pin; }

        protected:
            virtual bool setupPin() = 0;
            int8 pin;
        };

        class InputPinComponent : public PinComponent, public TaskAction
        {
        public:
            InputPinComponent();
            virtual ~InputPinComponent();

            virtual int getState() const;

        protected:
            void doTask() override;
            virtual bool setupPin() override;
            virtual void onChange();
            virtual void onHigh();
            virtual void onLow();
            int currentState;
            int debounceState;
            unsigned long lastStateChangeMsecs;
            unsigned long debounceMsecs;
            IntervalTimer logTimer;
        };

        class OutputPinComponent : public PinComponent
        {
        public:
            OutputPinComponent();
            virtual ~OutputPinComponent();

            void high();
            void low();

            void setState(bool high);
            bool getState() { return state; }

            virtual void setOn(bool onState = true) { setState(onState); };

        protected:
            virtual bool setupPin() override;
            bool state;
        };

        class Button : public InputPinComponent
        {
        public:
            Button();
            virtual ~Button();

            bool isPressed();

            bool isOn() { return isPressed(); }

        protected:
        };

        class IMotion : public IEventSource, public IBinarySensor
        {
        public:
            virtual bool isDetected() const = 0;
        };
        class Motion : public InputPinComponent, public IMotion
        {
        public:
            Motion();
            virtual ~Motion();

            bool isDetected() const override;
            bool isOn() const override { return getState() != 0; }
            bool isOff() const override { return getState() == 0; }

        protected:
            virtual void onHigh();
            virtual void onLow();
        };

        class MultiMotion : public IMotion
        {
        public:
            MultiMotion();
            virtual ~MultiMotion();
            void addPin(int8 pin);
            void removePin(int8 pin);
            void removeAll();
            bool isDetected() const override;
            bool isOn() const override { return isDetected() != 0; }
            bool isOff() const override { return isDetected() == 0; }

            void any();
            void all();

        protected:
            bool requireAll;
            void updateState();
            List<Motion> motion;
            Logger log;
            EventListener listener;
            bool hasMotion;
        };

        class Led : public OutputPinComponent
        {
        public:
            Led();
            virtual ~Led();

            void toggle() { setState(!getState()); }

        protected:
        };

    }
}

#endif