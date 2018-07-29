/**
 * @brief DS18B20 temperature sensor driver (OneWire)
 * 
 * @file DS18B20.h
 * @author cameronf
 * @date 2018-04-25
 */
#ifndef DS18B20_H
#define DS18B20_H

#include "mbed.h"
#include "OneWire.h"



class DS18B20 {

public:

typedef enum {
    CONFIG_RES_9BIT = 0,
    CONFIG_RES_10BIT = 1,
    CONFIG_RES_11BIT = 2,
    CONFIG_RES_12BIT = 3
} ResolutionConfig;
typedef struct {
    uint8_t reg;
    uint8_t low_byte_bit_mask;
    uint16_t conversion_time;
} ResolutionConfigStruct;

	DS18B20(OneWire &one_wire);
	DS18B20(OneWire &one_wire, uint64_t rom_code);

	bool Init(ResolutionConfig config = CONFIG_RES_9BIT);

	bool GetTemperatureC(float *temperature);
	bool GetTemperatureF(float *temperature);

	static int GetNumOfRomCodes(OneWire &one_wire);
	static int ListRomCodes(OneWire &one_wire, uint64_t rom_codes[], int array_size);

private:

	static uint8_t CRC08(uint8_t *addr, uint8_t len);

	OneWire &_one_wire;

	const bool _only_slave;
	const uint64_t _rom_code;

    ResolutionConfigStruct _resolution_config;
};

#endif // ONEWIRE_H