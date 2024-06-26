#ifndef LOGGER_DESTINATION_H
#define LOGGER_DESTINATION_H

/* The only destination currently implemented is LogSerialDestination
 * which writes to the serial port.
 * Other implementations may write to file, network, or memory.
 */

#include <cstdio>
#include <string.h>
#include <stdarg.h>

namespace FCLIB {
class ILogDestination {
    public:
        virtual void write(const char * message)const=0;
};

class LogSerialDestination : public ILogDestination {
    public:
        LogSerialDestination() {
            initialize();
        }

        void write(const char* message) const override {
            if (message == NULL) {
                return;
            }
            Serial.println(message);
            Serial.flush();
        }
    private:
        static bool m_serialInitialized;
        static void initialize();
};

bool LogSerialDestination::m_serialInitialized = false;
void LogSerialDestination::initialize() {
    if (!m_serialInitialized)
    {
        Serial.begin(115200);

        while(!Serial){
            yield();
            
        }
        Serial.printf("\nSerial Logger Running\n--------------\n");
                
        m_serialInitialized = true;
    }
}



}
#endif