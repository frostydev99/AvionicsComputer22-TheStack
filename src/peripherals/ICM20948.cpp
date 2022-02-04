/*
 * ICM20948.cpp
 *
 *  Created on: Feb 3, 2022
 *  Author: Charlie Tribble and Samay Govani
 */

#include "ICM20948.h"

/*
 * TODO comment this boy
 * @param address
 */
ICM20948::ICM20948(int address)
{
	_wire = &Wire;
	I2C_addr = address;
}

/*
 * TODO comment this boy
 * @param bank TODO
 */
void ICM20948::switchBank(uint8_t bank)
{
	if (bank != currBank)
	{
		currBank = bank;
		_wire->beginTransmission(I2C_addr);
		_wire->write(ICM20948_REG_BANK_SEL);
		_wire->write(currBank << 4);
		_wire->endTransmission(false);
	}
}

/*
 * TODO comment this boy
 * @return TODO
 */
bool ICM20948::init()
{
	currBank = 0;
	if (!resetSensor())
	{
		return false;
	}
	sleep(false);
	writeRegister8(2, ICM20948_ODR_ALIGN_EN, 1);
	return true; //TODO should this return true?
}

/*
 * TODO comment this boy
 * @param bank TODO
 * @param reg TODO
 * @param val TODO
 * @return TODO
 */
uint8_t ICM20948::writeRegister8(uint8_t bank, uint8_t reg, uint8_t val){
	switchBank(bank);
	_wire->beginTransmission(I2C_addr);
	_wire->write(reg);
	_wire->write(val);

	return _wire->endTransmission();
}
