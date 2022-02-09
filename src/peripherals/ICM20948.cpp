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
 * @return TODO
 */
uint8_t ICM20948::resetSensor()
{
	uint8_t ack = writeRegister8(0, ICM20948_PWR_MGMT_1, ICM20948_RESET);
	delay(10); // wait for registers to reset
	return (ack == 0);
}

/*
 * TODO comment this boy
 * @param sleep TODO
 */
void ICM20948::sleep(bool sleep)
{
	regVal = readRegister8(0, ICM20948_PWR_MGMT_1);
	if (sleep)
	{
		regVal |= ICM20948_SLEEP; // TODO what this doing?
	}
	else
	{
		regVal &= ~ICM20948_SLEEP; // TODO what this doing?
	}
	writeRegister8(0, ICM20948_PWR_MGMT_1, regVal);
}

/*
 * TODO comment this boy
 */
void ICM20948::readSensorData(){
	switchBank(0); // TODO do we need to set this every time?
	_wire->beginTransmission(I2C_addr);
	_wire->write(ICM20948_ACCEL_OUT);
	_wire->endTransmission(false);
	_wire->requestFrom(I2C_addr,14);
	if(_wire->available()){
		for(int i=0; i<14;i++){
			this->buffer[i] = _wire->read();
		}
	}
}

/*
 * TODO comment this boy
 * @param Hbyte TODO
 * @param Lbyte TODO
 * @return TODO
 */
int16_t ICM20948::processHighLowBytes(int8_t Hbyte, int8_t Lbyte){
	return (int16_t) ((Hbyte << 8)|Lbyte) * 1.0; // TODO what is this casting?
}

/*
 * TODO comment this boy
 * @return A Vector containing the raw accel data
 */
Vector ICM20948::getAccRawValues(){
	Vector accRaw;
	accRaw.x = processHighLowBytes(buffer[0],buffer[1]) * 1.0; // TODO what is this casting?
	accRaw.y = processHighLowBytes(buffer[2],buffer[3]) * 1.0; // TODO what is this casting?
	accRaw.z = processHighLowBytes(buffer[4],buffer[5]) * 1.0; // TODO what is this casting?
	return accRaw;
}

/*
 * TODO comment this boy
 * @return A Vector containing the raw gyro data
 */
Vector ICM20948::getGyroRawValues(){
	Vector gyroRaw;
	gyroRaw.x = processHighLowBytes(buffer[6],buffer[7])   * 1.0; // TODO what is this casting?
	gyroRaw.y = processHighLowBytes(buffer[8],buffer[9])   * 1.0; // TODO what is this casting?
	gyroRaw.z = processHighLowBytes(buffer[10],buffer[11]) * 1.0; // TODO what is this casting?
	return gyroRaw;
}

/*
 * TODO comment this boy
 * @return Temperature in ??? TODO
 */
int16_t ICM20948::getTempRawValues(){
	return processHighLowBytes(buffer[12],buffer[13]);
}

/*
 * TODO comment this boy
 * @param Vector to print
 */
void ICM20948::printVector(Vector print){
	Serial.print(print.x);
	Serial.print(",");
	Serial.print(print.y);
	Serial.print(",");
	Serial.print(print.z);
	Serial.print("\n");
}

/*
 * TODO comment this boy
 * @param bank TODO
 * @param reg TODO
 * @param val TODO
 * @return TODO
 */
uint8_t ICM20948::writeRegister8(uint8_t bank, uint8_t reg, uint8_t val){
	switchBank(bank); // TODO do we need to set this every time?
	_wire->beginTransmission(I2C_addr);
	_wire->write(reg);
	_wire->write(val);

	return _wire->endTransmission();
}
