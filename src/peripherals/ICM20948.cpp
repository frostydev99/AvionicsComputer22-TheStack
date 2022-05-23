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

	for(uint8_t i = 0; i < 14; i++){
		Serial.print(SensorRegister[i]);
		Serial.print("\t");
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
 * Set the GYRO_FS_SEL register to configure the sensitivity range of measurements
 * @param value is 0-3 to set the sensitivity
 */
void ICM20948::setGyroScale(uint8_t value) {

    if(value > 3) value = 3; 			// GYRO_FS_SEL cannot be larger than B011
    value <<= 1;           				// align it for the GYRO_CONFIG_1 register

	switchBank(2);						// register ID is for bank 2

	uint8_t setting = 0;
	setting = read8I2C(GYRO_CONFIG_1);

	setting &= B11111001;				// clear out old GYRO_FS_SEL bits
	setting |= value;					// mask in new GYRO_FS_SEL bits

	write8I2C(GYRO_CONFIG_1, setting);

}


/*
 * Set the ACCEL_FS_SEL register to configure the sensitivity range of measurements
 * @param value is 0-3 to set the sensitivity
 * 0: +-2g
 * 1: +-4g
 * 2: +-8g
 * 3: +-16g
 */
void ICM20948::setAccScale(uint8_t value) {

    if(value > 3) value = 3; 			// ACCEL_FS_SEL cannot be larger than B011
    value <<= 1;           				// align it for the ACCEL_CONFIG_1 register

	switchBank(2);						// register ID is for bank 2

	uint8_t setting = 0;
	setting = read8I2C(ACCEL_CONFIG_1);

	setting &= B11111001;				// clear out old ACCEL_FS_SEL bits
	setting |= value;					// mask in new ACCEL_FS_SEL bits

	write8I2C(ACCEL_CONFIG_1, setting);

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
 * Read all data from the 14 IMU data registers (ACCELXYZ, GYROXYZ, TEMP)
 * and save it to a buffer in this class for parsing/reading
 */
void ICM20948::readSensorData(){

	switchBank(0); 				// Should be done every time

	_wire->beginTransmission(I2C_addr);
	_wire->write(ICM20948_ACCEL_OUT);
	_wire->endTransmission(false);

	//_wire->requestFrom(I2C_addr, 14);

	for(uint8_t i = 0; i < 14; i++){
		this->SensorRegister[i] = _wire->read();
	}

	// Set globals
	rawAccelX = processHighLowBytes(SensorRegister[0], SensorRegister[1]);
	rawAccelY = processHighLowBytes(SensorRegister[2], SensorRegister[3]);
	rawAccelZ = processHighLowBytes(SensorRegister[4], SensorRegister[5]);
	rawGyroX  = processHighLowBytes(SensorRegister[6], SensorRegister[7]);
	rawGyroY  = processHighLowBytes(SensorRegister[8], SensorRegister[9]);
	rawGyroZ  = processHighLowBytes(SensorRegister[10], SensorRegister[11]);

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
 * @return the pointer to the array of raw sensor registers
 */
uint8_t* ICM20948::getRawSensorRegisters() {
	return SensorRegister;
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

void ICM20948::setPlusMinus250DPS() {
	setGyroScale(PLUS_MINUS_250DPS);
}

void ICM20948::setPlusMinus500DPS() {
	setGyroScale(PLUS_MINUS_500DPS);
}

void ICM20948::setPlusMinus1000DPS() {
	setGyroScale(PLUS_MINUS_1000DPS);
}

void ICM20948::setPlusMinus2000DPS() {
	setGyroScale(PLUS_MINUS_2000DPS);
}

void ICM20948::setPlusMinus2Gs() {
	setAccScale(PLUS_MINUS_2G);
}

void ICM20948::setPlusMinus4Gs() {
	setAccScale(PLUS_MINUS_4G);
}

void ICM20948::setPlusMinus8Gs() {
	setAccScale(PLUS_MINUS_8G);
}

void ICM20948::setPlusMinus16Gs() {
	setAccScale(PLUS_MINUS_16G);
}


int16_t ICM20948::getRawAccelX() {
	return rawAccelX;
}

int16_t ICM20948::getRawAccelY() {
	return rawAccelY;
}

int16_t ICM20948::getRawAccelZ() {
	return rawAccelZ;
}

int16_t ICM20948::getRawGyroX() {
	return rawGyroX;
}

int16_t ICM20948::getRawGyroY() {
	return rawGyroY;
}

int16_t ICM20948::getRawGyroZ() {
	return rawGyroZ;
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
    //_wire->requestFrom(I2C_addr,1);
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
    //_wire->requestFrom(I2C_addr,2);
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


/*
 *
 */
uint8_t ICM20948::read8I2C(uint8_t regAddress) {

    _wire->beginTransmission(ICM20948_ADDRESS);
    _wire->write(regAddress);        			// Address to read data from
    _wire->endTransmission(false);   		// Send data to I2C with option for a repeated start
    _wire->requestFrom(ICM20948_ADDRESS, 1);   // Request the data
    return Wire.read();

}

/*
 *
 */
void ICM20948::write8I2C(uint8_t regAddress, uint8_t value) {

	_wire->beginTransmission(ICM20948_ADDRESS);
	_wire->write(regAddress);
	_wire->write(value);
	_wire->endTransmission(true);

}
