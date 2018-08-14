

#include "mbed.h"
#include "UARTOneWire.h"
#include "DS18B20.h"
#include "IoThread.h"
#include "LiquidCrystal_I2C.h"

int main()
{
    UARTOneWire one_wire(D8, D2);
    DS18B20 sensor(one_wire);

    Thread os_io_thread;
    IoThread io_thread(one_wire);
    os_io_thread.start(&io_thread, &IoThread::Run);

    float temps[4] = {0};

    LiquidCrystal_I2C lcd(0x27, 20, 4);
    lcd.begin();
    lcd.backlight();
    lcd.print("Hello, world!");
 

    while(1)
    {
        temps[0] = io_thread.GetTemperature(IoThread::WORT_TEMP);
        temps[1] = io_thread.GetTemperature(IoThread::AIR_TEMP1);
        temps[2] = io_thread.GetTemperature(IoThread::AIR_TEMP2);
        temps[3] = io_thread.GetTemperature(IoThread::ROOM_TEMP);
        wait(5.0);
    }
}