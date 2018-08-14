#include "IoThread.h"
#include "DS18B20.h"

IoThread::IoThread(OneWire &one_wire) :
_one_wire(one_wire)
{
    
}

// byte wort_temp_address[8] = {0x28,0xFF,0x56,0xB0,0xA4,0x16,0x04,0x9F};
// byte air1_temp_address[8] = {0x28,0xFF,0x0E,0x3F,0xA1,0x16,0x04,0x42};
// byte air2_temp_address[8] = {0x28,0xFF,0xE9,0xB6,0xA4,0x16,0x04,0xB6};
// byte room_temp_address[8] = {0x28,0xFF,0x3F,0xB1,0xA4,0x16,0x05,0xAC};


void IoThread::Run()
{
    DS18B20 sensors[] = {
 //       DS18B20(_one_wire, 0x8B0416A47ABBFF28),
        DS18B20(_one_wire, 0x9F0416A4B056FF28),
        DS18B20(_one_wire, 0x420416A13F0EFF28),
        DS18B20(_one_wire, 0xB60416A4B6E9FF28),
        DS18B20(_one_wire, 0xAC0516A4B13FFF28)
    };

    for (int i=0; i<4; i++)
    {
        sensors[i].Init(DS18B20::CONFIG_RES_12BIT);
    }

    while (1)
    {
        for (int i=0; i<4; i++)
        {
            float temperature_buf;
            if (sensors[i].GetTemperatureC(&temperature_buf))
            {
                _temperature_readings[i] = temperature_buf;
            }
        } 
    }
}


float IoThread::GetTemperature(TemperatureSensor sensor)
{
    return _temperature_readings[(int)sensor];
}