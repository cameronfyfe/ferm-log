/**
 * @brief OneWire driver using UART
 * 
 * @file UARTOneWire.cpp
 * @author cameronf
 * @date 2018-04-25
 */
#include "mbed.h"
#include "UARTOneWire.h"


const char ONEWIRE_TX_BITS[] = {0x00, 0xFF};
const char ONEWIRE_TX_RESET_PULSE = 0xF0;
const char ONEWIRE_TX_READ_BIT = 0xFF;

const float LOOPBACK_DELAY_S = 0.001;


UARTOneWire::UARTOneWire(PinName tx, PinName rx) : UARTSerial(tx, rx)
{
	// STM32F0 specific code
	// Set tx (PC_4) as open-drain
	GPIOA->OTYPER |= GPIO_OTYPER_OT_9;
}


// Send byte on bus
void UARTOneWire::SendByte(uint8_t byte)
{
	this->set_baud(115200);

	for (int i=0; i<8; i++)
	{
		SendBit((byte >> i) & (uint8_t)0x01);
	}
}


// Read byte from bus
uint8_t UARTOneWire::ReadByte()
{
	this->set_baud(115200);

	uint8_t byte = 0;
	for (int i=0; i<8; i++)
	{
		byte |= ((uint8_t)ReadBit() << i);
	}

	return byte;
}


// Reset slaves on bus
bool UARTOneWire::Reset()
{
	char rx_byte;

	// Use 9600 baud to fit reset/detect pulse in one byte frame
	this->set_baud(9600);

	// Send 1 bit reset pulse and allow slave device to respond with detect pulse during other 7 bits
	this->write(&ONEWIRE_TX_RESET_PULSE, 1);
	wait(LOOPBACK_DELAY_S);
	this->read(&rx_byte, 1);
	
	// If we read something other than loopback, assume it's the detect pulse from a slave device
	return (rx_byte != ONEWIRE_TX_RESET_PULSE);
}


// Send bit on bus
void UARTOneWire::SendBit(bool bit)
{
	char rx_byte;

	this->write(&ONEWIRE_TX_BITS[bit], 1);
	wait(LOOPBACK_DELAY_S);
	this->read(&rx_byte, 1);
}


// Trigger bit frame and read bit from bus
bool UARTOneWire::ReadBit()
{
	char rx_byte;

	this->write(&ONEWIRE_TX_READ_BIT, 1);
	wait(LOOPBACK_DELAY_S);
	this->read(&rx_byte, 1);

	return (rx_byte == 0xFF);
}
