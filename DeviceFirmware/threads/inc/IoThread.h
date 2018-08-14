#ifndef IO_THREAD_H
#define IO_THREAD_H

#include "OneWire.h"

class IoThread {

public:

    typedef enum {
        WORT_TEMP = 0,
        AIR_TEMP1 = 1,
        AIR_TEMP2 = 2,
        ROOM_TEMP = 3
    } TemperatureSensor;

    IoThread(OneWire &one_wire);

    void Run();

    float GetTemperature(TemperatureSensor sensor);

private:

    OneWire &_one_wire;
    float _temperature_readings[4];

};

#endif//IO_THREAD_H