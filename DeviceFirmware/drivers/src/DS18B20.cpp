/**
 * @brief DS18B20 temperature sensor driver (OneWire)
 * 
 * @file DS18B20.cpp
 * @author cameronf
 * @date 2018-04-25
 */
#include "mbed.h"
#include "DS18B20.h"
#include "OneWire.h"

// Configuration Register
const DS18B20::ResolutionConfigStruct RESOLUTION_CONFIGS[] = {
        {.reg = 0x1F, .low_byte_bit_mask = 0xF8, .conversion_time = 94},  // 9 bit
        {.reg = 0x3F, .low_byte_bit_mask = 0xFC, .conversion_time = 188}, // 10 bit
        {.reg = 0x5F, .low_byte_bit_mask = 0xFE, .conversion_time = 375}, // 11 bit
        {.reg = 0x7F, .low_byte_bit_mask = 0xFF, .conversion_time = 750}  // 12 bit
};

// ROM Commands
const uint8_t ROM_CMD_SEARCH_ROM	= 0xF0; // Iterate through Rom codes
const uint8_t ROM_CMD_READ_ROM		= 0x33; // Read 64 bit ROM code (single slave only)
const uint8_t ROM_CMD_MATCH_ROM		= 0x55; // Address a specific slave (cmd followed by 64 bit ROM code)
const uint8_t ROM_CMD_SKIP_ROM		= 0xCC; // Address all slaves (or one slave if single slave bus)
const uint8_t ROM_CMD_ALARM_SEARCH	= 0xEC; // Same as Search Rom Cmd but only slaves with set alarms respond

// Function Commands
const uint8_t FUNC_CMD_CONVERT_T		= 0x44;
const uint8_t FUNC_CMD_WRITE_SCRATCHPAD	= 0x4E;
const uint8_t FUNC_CMD_READ_SCRATCHPAD	= 0xBE;
const uint8_t FUNC_CMD_COPY_SCRATCHPAD	= 0x48;
const uint8_t FUNC_CMD_RECALL_EE		= 0xB8;

// Scratchpad byte indices
const uint8_t T_LO		= 0;
const uint8_t T_HI		= 1;
const uint8_t PAD_CRC	= 8;


// Constructor for sensor on bus with no other slaves
DS18B20::DS18B20(OneWire &one_wire) :
_one_wire(one_wire),
_only_slave(true),
_rom_code((uint64_t)0x00)
{

}


// Constructor for sensor on bus with multiple slaves
DS18B20::DS18B20(OneWire &one_wire, uint64_t rom_code) :
_one_wire(one_wire),
_only_slave(false),
_rom_code(rom_code)
{

}


// Initialize temperature sensor (set resolution)
bool DS18B20::Init(ResolutionConfig config)
{
    // Set resolution configuration
    _resolution_config = RESOLUTION_CONFIGS[(int)config];

	_one_wire.Reset();
	_one_wire.SendByte(ROM_CMD_SKIP_ROM);
	_one_wire.SendByte(FUNC_CMD_WRITE_SCRATCHPAD);
	_one_wire.SendByte(0x00);						// User byte 0 - Unused
	_one_wire.SendByte(0x00);						// User byte 1 - Unused
	_one_wire.SendByte(_resolution_config.reg);

	// Check for detect pulse after reset signal
	bool detect = _one_wire.Reset();
	
	return detect;
}


// Return temperature in celcius
bool DS18B20::GetTemperatureC(float *temperature)
{
	uint8_t scratchpad[9];

	if (_only_slave)
	{	
		// Copy temperature reading to scratchpad
		_one_wire.Reset();
		_one_wire.SendByte(ROM_CMD_SKIP_ROM);
		_one_wire.SendByte(FUNC_CMD_CONVERT_T);

        // Wait for ADC conversion
        wait_ms(_resolution_config.conversion_time + 10);

		// Read scratchpad
		_one_wire.Reset();
		_one_wire.SendByte(ROM_CMD_SKIP_ROM);
		_one_wire.SendByte(FUNC_CMD_READ_SCRATCHPAD);
		for (int i=0; i<9; i++)
		{
			scratchpad[i] = _one_wire.ReadByte();
		}
	}
	else
	{
		// TODO: read temperature by ROM code
	}

	*temperature = 0.0625 * ((scratchpad[T_HI] << 8) | (scratchpad[T_LO] & _resolution_config.low_byte_bit_mask));

	// Make sure scratchpad isn't 0s since if bus is pulled low the entire time
	// the scratchpad is read as 0s and the CRC still checks out since CRC(0s)=0
	bool blank_pad = true;
	for (int i=0; i<8; i++)
	{
		if (scratchpad[i] != 0x00)
		{
			blank_pad = false;
			break;
		}
	}

	return (CRC08(scratchpad, 8) == scratchpad[PAD_CRC] && !blank_pad);
}


// Return temperature in fahrenheit
bool DS18B20::GetTemperatureF(float *temperature)
{
	bool success = this->GetTemperatureC(temperature);

	*temperature = 1.8 * *temperature + 32;

	return success;
}


// Return number of DS18B20s on the OneWire bus
int DS18B20::GetNumOfRomCodes(OneWire &one_wire)
{
	// TODO
}


// List ROM codes of all DS18B20s on the OneWire bus
int DS18B20::ListRomCodes(OneWire &one_wire, uint64_t rom_codes[], int array_size)
{
	// TODO
}


// Calculate 8 bit Maxim CRC
uint8_t DS18B20::CRC08(uint8_t *addr, uint8_t len)
{
	uint8_t crc=0;

	for (uint8_t i=0; i<len;i++)
	{
		uint8_t inbyte = addr[i];
		for (uint8_t j=0; j<8; j++)
		{
			  uint8_t mix = (crc ^ inbyte) & 0x01;
			  crc >>= 1;
			  if (mix)
			  {
					crc ^= 0x8C;
			  }
			  inbyte >>= 1;
		}
	}
	return crc;
}

