/**
 * @brief OneWire interface class to be inherited and implemented
 * 
 * @file OneWire.h
 * @author cameronf
 * @date 2018-04-25
 */
#ifndef ONEWIRE_H
#define ONEWIRE_H

#include "mbed.h"


class OneWire {

public:

	virtual void SendByte(uint8_t byte) = 0;
	virtual uint8_t ReadByte() = 0;

    virtual void SendBytes(uint8_t *bytes, int len) = 0;
    virtual void ReadBytes(uint8_t *bytes, int len) = 0;

	virtual bool Reset() = 0;

};

#endif // ONEWIRE_H