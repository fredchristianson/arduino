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
        /* Interfaces */
        class BinaryReadComponent
        {
        public:
            virtual bool isOn() = 0;
        };

        class BinaryWriteComponent
        {
        public:
            virtual void setOn(bool onState = true) = 0;
        };

        class BinaryRWComponent : public BinaryReadComponent, public BinaryWriteComponent
        {
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

        protected:
            virtual bool setupPin() = 0;
            int8 pin;
        };

        class InputPinComponent : public PinComponent, public TaskAction
        {
        public:
            InputPinComponent();
            virtual ~InputPinComponent();

            int getState();

        protected:
            void doTask() override;
            virtual bool setupPin() override;

            int currentState;
            int debounceState;
            unsigned long lastStateChangeMsecs;
            unsigned long debounceMsecs;
            IntervalTimer logTimer;
        };

        class OutputPinComponent : public PinComponent, public BinaryWriteComponent
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

        class Button : public InputPinComponent, public BinaryReadComponent
        {
        public:
            Button();
            virtual ~Button();

            bool isPressed();

            bool isOn() override { return isPressed(); }

        protected:
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