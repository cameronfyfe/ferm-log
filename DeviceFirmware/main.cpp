

#include "mbed.h"
#include "UARTOneWire.h"
#include "DS18B20.h"

int main()
{
    UARTOneWire one_wire(D8, D2);
    DS18B20 sensor(one_wire);

    float temp_c = 0.0;

    bool init_ret = false;
    bool success = false;

    while (1)
    {
        init_ret = sensor.Init(DS18B20::CONFIG_RES_9BIT);
        success = sensor.GetTemperatureC(&temp_c);

        init_ret = sensor.Init(DS18B20::CONFIG_RES_10BIT);
        success = sensor.GetTemperatureC(&temp_c);

        init_ret = sensor.Init(DS18B20::CONFIG_RES_11BIT);
        success = sensor.GetTemperatureC(&temp_c);

        init_ret = sensor.Init(DS18B20::CONFIG_RES_12BIT);
        success = sensor.GetTemperatureC(&temp_c);
    }
}