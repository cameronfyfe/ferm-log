/**
 * @brief OneWire driver using UART
 * 
 * @file UARTOneWire.h
 * @author cameronf
 * @date 2018-04-25
 */
#ifndef UARTONEWIRE_H
#define UARTONEWIRE_H

#include "mbed.h"
#include "OneWire.h"


class UARTOneWire : public OneWire, public UARTSerial {

public:

	UARTOneWire(PinName tx, PinName rx);

	virtual void SendByte(uint8_t byte);
	virtual uint8_t ReadByte();

    virtual void SendBytes(uint8_t *bytes, int len);
    virtual void ReadBytes(uint8_t *bytes, int len);

	virtual bool Reset();

private:

	void SendBit(bool bit);
	bool ReadBit();
};

#endif // UARTONEWIRE_H