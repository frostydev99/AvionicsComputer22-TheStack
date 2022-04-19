#include "Arduino.h"
#include "MPL3115A2.h"
#include "Wire.h"

MPL3115A2::MPL3115A2(){

}

/*
 * Function for initializing the barometer
 */
void MPL3115A2::init(){

	uint8_t whoami = 0;
	Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
	Wire.write(MPL3115A2_WHOAMI);//Accessing WHOAMI register of barometer
	Wire.endTransmission(false);//Ending transmission, no longer writing to bus
	Wire.requestFrom(MPL3115A2_ADDRESS, 1, false);//Request one byte from barometer
	whoami = Wire.read();//read the byte from barometer whoami

}

void MPL3115A2::disable(){

}


void MPL3115A2::setModeBarometer() {

    uint8_t setting = 0;
    setting = read8I2C(MPL3115A2_CTRL_REG1);

    setting &= ~(1<<7);   // clear ALT bit
    write8I2C(MPL3115A2_CTRL_REG1, setting);

}

void MPL3115A2::setModeAltimeter() {

    uint8_t setting = 0;
    setting = read8I2C(MPL3115A2_CTRL_REG1);

    setting |= (1<<7);   // set ALT bit
    write8I2C(MPL3115A2_CTRL_REG1, setting);

}

/*
 *
 */
void MPL3115A2::setOverSampleRate(uint8_t oversampleVal) {

    if(oversampleVal > 7) oversampleVal = 7; //OS cannot be larger than B0111
    oversampleVal <<= 3;                      // align it for the CTRL_REG1 register

    uint8_t setting = read8I2C(MPL3115A2_CTRL_REG1);    //Read current settings
    setting &= B11000111;                      			//Clear out old OS bits
    setting |= oversampleVal;                 			//Mask in new OS bits
    write8I2C(MPL3115A2_CTRL_REG1, setting);

}


void MPL3115A2::setOverSample6ms() {
	setOverSampleRate(OS_6MS);
}

void MPL3115A2::setOverSample10ms() {
	setOverSampleRate(OS_10MS);
}

void MPL3115A2::setOverSample18ms() {
	setOverSampleRate(OS_18MS);
}

void MPL3115A2::setOverSample34ms() {
	setOverSampleRate(OS_34MS);
}

void MPL3115A2::setOverSample66ms() {
	setOverSampleRate(OS_66MS);
}

void MPL3115A2::setOverSample130ms() {
	setOverSampleRate(OS_130MS);
}

void MPL3115A2::setOverSample258ms() {
	setOverSampleRate(OS_258MS);
}

void MPL3115A2::setOverSample512ms() {
	setOverSampleRate(OS_512MS);
}


/*
 * Clears then sets the OST bit which causes the sensor to immediately take
 * another reading, needed to sample faster than 1Hz
 */
void MPL3115A2::toggleOneShot() {

	uint8_t ctrl_reg1 = read8I2C(MPL3115A2_CTRL_REG1); // Read the barometer's current settings
	ctrl_reg1 &= ~(1<<1); 							   // Clear OST bit
	write8I2C(MPL3115A2_CTRL_REG1, ctrl_reg1);

	ctrl_reg1 = read8I2C(MPL3115A2_CTRL_REG1); 		   // Read the settings to be safe
	ctrl_reg1 |= (1<<1); 							   // Set the OST bit
	write8I2C(MPL3115A2_CTRL_REG1, ctrl_reg1);

}

/*
 * Polls the barometer for its status, pressure, and temperature registers
 */
void MPL3115A2::readSensorData() {

	setModeAltimeter();

	toggleOneShot();		// Only do this once before reading from the sensor

	Wire.beginTransmission(MPL3115A2_ADDRESS);		//Beginning transmission with barometer
	Wire.write(MPL3115A2_REGISTER_DR_STATUS);		//Accessing status register of the barometer
	Wire.endTransmission(false);					//Ending transmission
	Wire.requestFrom(MPL3115A2_ADDRESS, 6, false);	//Request 6 bytes from barometer

	sensorRegisters[0] = Wire.read(); 				// Barometer status
	sensorRegisters[1] = Wire.read(); 				// Pressure MSB
	sensorRegisters[2] = Wire.read(); 				// Pressure CSB
	sensorRegisters[3] = Wire.read(); 				// Pressure LSB
	sensorRegisters[4] = Wire.read(); 				// Temperature MSB
	sensorRegisters[5] = Wire.read(); 				// Temperature LSB

	readAltitude();
	readTemperature();

	rawDataToFourBytes();

}

/*
 * Function for reading the current pressure
 * @return pressure in Pascals
 */
float MPL3115A2::readPressure() {

	//toggleOneShot();		Only do this once before reading from the sensor

    //Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    //Wire.write(MPL3115A2_REGISTER_PRESSURE_MSB);//Accessing Pressure register of barometer
    //Wire.endTransmission(false);//Ending transmission, no longer writing to bus
    //Wire.requestFrom(MPL3115A2_ADDRESS, 3, false);//Request three bytes from barometer

	float reading = rawToPressure(sensorRegisters[1], sensorRegisters[2], sensorRegisters[3]);

    return reading;
}


/*
 * Take an altitude measurement from the sensor, simply calls readPressure because
 * MPL3115 will store pressure or altitude in the same register locations
 * depending on if set to pressure or altitude mode
 * @return altitude in meters above sea level converted from the pressure sensor
 * using the standard atmospheric model
 */
float MPL3115A2::readAltitude() {

    //toggleOneShot();    !!! ONLY DO THIS ONCE BEFORE READING !!!

    float altitude = rawToAltitude(sensorRegisters[1], sensorRegisters[2], sensorRegisters[3]);

    return altitude;

}


/*
 * Function for reading the current temperature
 * @return temperature in Celsius
 */
float MPL3115A2::readTemperature() {

	//toggleOneShot();		Only do this once before reading from the sensor

	//Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
	//Wire.write(MPL3115A2_REGISTER_TEMPERATURE_MSB);//Accessing Pressure register of barometer
	//Wire.endTransmission(false);//Ending transmission, no longer writing to bus
	//Wire.requestFrom(MPL3115A2_ADDRESS, 2, false);//Request three bytes from barometer

	float reading = rawToTemperature(sensorRegisters[4], sensorRegisters[5]);

	return reading;
}


/*
 * From the raw sensor registers, converts the pressure to a float. The sensor is in pressure mode
 * and returns the raw data as a Q18.2 unsigned integer.
 */
float MPL3115A2::rawToPressure(uint8_t msb, uint8_t csb, uint8_t lsb) {

	float pressure = 0.0;

    // Obtain integer part of the pressure reading
    uint32_t integerPart = (msb << 10) | (csb << 2) | (lsb >> 6);

    // Get fractional part
    uint8_t fractionalPart = ((lsb >> 4) & 3);

    // Integer and fractional parts combined
    pressure = integerPart + (fractionalPart * 0.25);

    this->pressure = pressure;

	return pressure;
}

/*
 * From the raw sensor registers, converts the altitude to a float. The sensor is in altitude mode
 * and returns the raw data as a Q16.4 signed integer.
 */
float MPL3115A2::rawToAltitude(uint8_t msb, uint8_t csb, uint8_t lsb) {
	float altitude = 0.0;

	// Integer part of the altitude reading
	int16_t integerPart = (msb << 8) | csb;
	// Fractional part of the altitude reading
	uint8_t fractionalPart = (lsb >> 4);

	altitude = integerPart + (fractionalPart * 0.0625);

	this->altitude = altitude;
	return altitude;
}

/*
 *
 */
float MPL3115A2::rawToTemperature(uint8_t msb, uint8_t lsb) {

	float temperature = 0.0;
	int16_t integerPart = msb;

	// Get fractional part
	uint8_t fractionalPart = ((lsb & B11110000) >> 4);

	// Integer and fractional parts combined
	temperature = integerPart + (fractionalPart * 0.0625);

	this->temperature = temperature;

	return temperature;

}


/*
 * Combine the 20-bit pressure (Q18.2 fixed point) and 12-bit temperature (Q8.4 fixed point)
 * into a uint32_t or array of 4 bytes for compact data transmission
 */
void MPL3115A2::rawDataToFourBytes() {

	uint32_t pressureRaw = (sensorRegisters[1] << 24) | (sensorRegisters[2] << 16)
			| (sensorRegisters[3] << 8);

	uint32_t temperatureRaw = (sensorRegisters[4] << 4) | (sensorRegisters[5] >> 4);

	uint32_t combinedRaw = pressureRaw | temperatureRaw;	// add them together (binary OR)

	// Set the combined bytes to the pressure and temperature combined array
	uint8_t * combinedBytes = (uint8_t *) &combinedRaw;
	uint8_t * pressureAndTempBytes = (uint8_t *) &pressureAndTemp;

	pressureAndTempBytes[0] = combinedBytes[0];		// big endian
	pressureAndTempBytes[1] = combinedBytes[1];
	pressureAndTempBytes[2] = combinedBytes[2];
	pressureAndTempBytes[3] = combinedBytes[3];

}


/*
 * This function is for using this barometer class for decoding the combined
 * raw pressure and temperature bytes and splitting them into the sensorRegisters
 * array to be parsed into the values as floats
 * @param combinedRaw is the four bytes of combined raw pressure and temperature sensor data
 */
void MPL3115A2::setDataFromCombinedRaw(uint32_t combinedRaw) {

	uint8_t * combinedBytes = (uint8_t *) &combinedRaw;

	uint8_t pressureMSB = combinedBytes[3];
	uint8_t pressureCSB = combinedBytes[2];
	uint8_t pressureLSB = combinedBytes[1] &= B11110000;	// keep only first 4 bits

	uint8_t temperatureMSB = (combinedBytes[1] << 4) | (combinedBytes[0] >> 4);
	uint8_t temperatureLSB = (combinedBytes[0] << 4);

	sensorRegisters[1] = pressureMSB;
	sensorRegisters[2] = pressureCSB;
	sensorRegisters[3] = pressureLSB;
	sensorRegisters[4] = temperatureMSB;
	sensorRegisters[5] = temperatureLSB;

	readAltitude();
	readTemperature();

}


/*
 * Function for setting the zero altitude
 * @return void
 */
void MPL3115A2::setZeroAltitude(){
	this->zeroAltitude = this->readAltitude();
}



float MPL3115A2::getPressure() {
    return this->pressure;
}

float MPL3115A2::getAltitude() {
    return this->altitude;
}

float MPL3115A2::getTemperature() {
	return this->temperature;
}

/*
 * @return the pointer to the array of raw sensor registers
 */
uint8_t* MPL3115A2::getRawSensorRegisters() {
	return sensorRegisters;
}


/*
 *
 */
uint32_t MPL3115A2::getPressureAndTempCombined() {
	return pressureAndTemp;
}

/*
 * Reads a byte from the given register address
 * @params regAddress - the address of the register to access
 * @return the byte read from the given register
 */
uint8_t MPL3115A2::read8I2C(uint8_t regAddress) {
	Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
	Wire.write(regAddress);//Accessing input register of barometer
	Wire.endTransmission(false);//Ending transmission, no longer writing to bus
	Wire.requestFrom(MPL3115A2_ADDRESS, 1, false);//Request one byte from barometer
	return Wire.read();//return read byte

}

/*
 * Writes a byte to a given register
 * @params regAddress - the register address to access
 * @params value - the byte to write to the given register
 * @return void
 */
void MPL3115A2::write8I2C(uint8_t regAddress, uint8_t value) {
	Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
	Wire.write(regAddress);//Accessing Control register 1 of barometer
	Wire.write(value);
	Wire.endTransmission(false);//Ending transmission, no longer writing to bus
}

