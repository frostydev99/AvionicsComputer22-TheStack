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
ICM20948::ICM20948(uint8_t address)
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
void ICM20948::printBuffer(){
//	for(int i = 0; i<14;i++){
//		Serial.print(SensorRegister[i]);
//		Serial.print("\t");
//	}

			// Accelerometer x-axis
			Serial.write(65); // A
			Serial.write(88); // X
			Serial.write(SensorRegister[0]);
			Serial.write(SensorRegister[1]);

			// Accelerometer y-axis
			Serial.write(65); // A
			Serial.write(89); // Y
			Serial.write(SensorRegister[2]);
			Serial.write(SensorRegister[3]);

			// Accelerometer z-axis
			Serial.write(65); // A
			Serial.write(90); // Z
			Serial.write(SensorRegister[4]);
			Serial.write(SensorRegister[5]);

			// Gyro x-axis
			Serial.write(71); // G
			Serial.write(88); // X
			Serial.write(SensorRegister[6]);
			Serial.write(SensorRegister[7]);

			// Gyro y-axis
			Serial.write(71); // G
			Serial.write(89); // Y
			Serial.write(SensorRegister[8]);
			Serial.write(SensorRegister[9]);

			// Gyro z-axis
			Serial.write(71); // G
			Serial.write(90); // Z
			Serial.write(SensorRegister[10]);
			Serial.write(SensorRegister[11]);



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
	regVal = this->readRegister8(0, ICM20948_PWR_MGMT_1);
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
			this->SensorRegister[i] = _wire->read();
		}
	}
}

/*
 * TODO comment this boy
 * @param Hbyte TODO
 * @param Lbyte TODO
 * @return TODO
 */
int16_t ICM20948::processHighLowBytes(uint8_t Hbyte, uint8_t Lbyte){
	return (int16_t) ((Hbyte << 8)|Lbyte); // TODO what is this casting?
}

/*
 * TODO comment this boy
 * @return A Vector containing the raw accel data
 */
Vector ICM20948::getAccRawValues(){
	Vector accRaw;
	accRaw.x = processHighLowBytes(SensorRegister[0],SensorRegister[1]); // TODO what is this casting?
	accRaw.y = processHighLowBytes(SensorRegister[2],SensorRegister[3]); // TODO what is this casting?
	accRaw.z = processHighLowBytes(SensorRegister[4],SensorRegister[5]); // TODO what is this casting?
	return accRaw;
}

/*
 * TODO comment this boy
 * @return A Vector containing the raw gyro data
 */
Vector ICM20948::getGyroRawValues(){
	Vector gyroRaw;
	gyroRaw.x = processHighLowBytes(SensorRegister[6],SensorRegister[7]); // TODO what is this casting?
	gyroRaw.y = processHighLowBytes(SensorRegister[8],SensorRegister[9]); // TODO what is this casting?
	gyroRaw.z = processHighLowBytes(SensorRegister[10],SensorRegister[11]); // TODO what is this casting?
	return gyroRaw;
}

/*
 * TODO comment this boy
 * @return Temperature in ??? TODO
 */
int16_t ICM20948::getTempRawValues(){
	return processHighLowBytes(SensorRegister[12],SensorRegister[13]);
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

uint8_t ICM20948::readRegister8(uint8_t bank, uint8_t reg){
    switchBank(bank);
    uint8_t regValue = 0;
    _wire->beginTransmission(I2C_addr);
    _wire->write(reg);
    _wire->endTransmission(false);
    _wire->requestFrom(I2C_addr,1);
    if(_wire->available()){
        regValue = _wire->read();
    }
    return regValue;
}
int16_t ICM20948::readRegister16(uint8_t bank, uint8_t reg){
    switchBank(bank);
    uint8_t MSByte = 0, LSByte = 0;
    int16_t reg16Val = 0;
    _wire->beginTransmission(I2C_addr);
    _wire->write(reg);
    _wire->endTransmission(false);
    _wire->requestFrom(I2C_addr,2);
    if(_wire->available()){
        MSByte = _wire->read();
        LSByte = _wire->read();
    }
    reg16Val = (MSByte<<8) + LSByte;
    return reg16Val;
}
uint8_t ICM20948::writeRegister16(uint8_t bank, uint8_t reg, int16_t val){
    switchBank(bank);
    int8_t MSByte = (int8_t)((val>>8) & 0xFF);
    uint8_t LSByte = val & 0xFF;
    _wire->beginTransmission(I2C_addr);
    _wire->write(reg);
    _wire->write(MSByte);
    _wire->write(LSByte);

    return _wire->endTransmission();
}
