#include "Arduino.h"
#include "MPL3115A2.h"
#include "Wire.h"

MPL3115A2::MPL3115A2(){
    this->pressure = 0;
    this->altitude = 0;
    this-> maxAltitudeReading = 0;
    this-> zeroAltitude = 0;
    this->reachedApogee = false;
    this->hasLaunched = false;
}

/*
 * Function for initializing the barometer
 */
void MPL3115A2::init(){
	uint8_t whoami;// = read8(MPL3115A2_WHOAMI);
	Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
	Wire.write(MPL3115A2_WHOAMI);//Accessing WHOAMI register of barometer
	Wire.endTransmission(false);//Ending transmission, no longer writing to bus
	Wire.requestFrom(MPL3115A2_ADDRESS, 1, false);//Request one byte from barometer
	whoami = Wire.read();//read the byte from barometer whoami
}

void MPL3115A2::disable(){

}

/*
 * Clears then sets the OST bit which causes the sensor to immediately take
 * another reading, needed to sample faster than 1Hz
 */
void MPL3115A2::toggleOneShot() {
	uint8_t ctrl_reg1 = read8I2C(MPL3115A2_CTRL_REG1); // Read the barometer's current settings
	ctrl_reg1 &= ~(1<<1); // Clear OST bit
	write8I2C(MPL3115A2_CTRL_REG1, ctrl_reg1);

	ctrl_reg1 = read8I2C(MPL3115A2_CTRL_REG1); // Read the settings to be safe
	ctrl_reg1 |= (1<<1); // Set the OST bit
	write8I2C(MPL3115A2_CTRL_REG1, ctrl_reg1);
}

/*
 * Polls the barometer for its status, pressure, and temperature registers
 */
void MPL3115A2::pollSensor(uint8_t* buffer) {
	toggleOneShot();

	Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
	Wire.write(MPL3115A2_REGISTER_DR_STATUS);//Accessing status register of the barometer
	Wire.endTransmission(false);//Ending transmission, no longer writing to bus
	Wire.requestFrom(MPL3115A2_ADDRESS, 6, false);//Request three bytes from barometer

	buffer[0] = Wire.read(); // Barometer status
	buffer[1] = Wire.read(); // Pressure MSB
	buffer[2] = Wire.read(); // Pressure CSB
	buffer[3] = Wire.read(); // Pressure LSB
	buffer[4] = Wire.read(); // Temperature MSB
	buffer[5] = Wire.read(); // Temperature LSB
}

/*
 * Function for reading the current pressure
 * @return pressure in Pascals
 */
float MPL3115A2::readPressure() {
	toggleOneShot();

    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(MPL3115A2_REGISTER_PRESSURE_MSB);//Accessing Pressure register of barometer
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus
    Wire.requestFrom(MPL3115A2_ADDRESS, 3, false);//Request three bytes from barometer

    uint8_t msb, csb, lsb;
    msb = Wire.read();
    csb = Wire.read();
    lsb = Wire.read();

    // Obtain integer part of the pressure reading
    uint32_t integerPart = (msb << 10) | (csb << 2) | (lsb >> 6);

    // Get fractional part
    uint8_t fractionalPart = ((lsb >> 4) & 3);

    // Integer and fractional parts combined
    float reading = integerPart + (fractionalPart * 0.25);
    return reading;
}

/*
 * Function for reading the current temperature
 * @return temperature in Celsius
 */
float MPL3115A2::readTemperature() {
	toggleOneShot();

	Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
	Wire.write(MPL3115A2_REGISTER_TEMPERATURE_MSB);//Accessing Pressure register of barometer
	Wire.endTransmission(false);//Ending transmission, no longer writing to bus
	Wire.requestFrom(MPL3115A2_ADDRESS, 2, false);//Request three bytes from barometer

	uint8_t lsb;
	int8_t msb = Wire.read();
	lsb = Wire.read();

	uint8_t fractionalPart = (lsb >> 4);
	float reading = msb + (fractionalPart * 0.0625);
	return reading;
}

/*
 * Function for reading the current altitude
 * @return altitude in meters
 */
float MPL3115A2::readAltitude() {
	toggleOneShot();

    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(MPL3115A2_REGISTER_PRESSURE_MSB);//Accessing Pressure register of barometer
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus
    Wire.requestFrom(MPL3115A2_ADDRESS, 3, false);//Request three bytes from barometer
    uint32_t alt = Wire.read();
    alt <<= 8;
    alt |= Wire.read();
    alt <<= 8;
    alt |= Wire.read();

    float reading = alt;
    return reading / 65536.0;
}

/*
 * Function for setting the zero altitude
 * @return void
 */
void MPL3115A2::setZeroAltitude(){
	this->zeroAltitude = this->readAltitude();
}

/*
 * Function for updating the altitude and pressure readings
 * also checks for launch and apogee
 * @return void
 */
void MPL3115A2::update() {
    this->pressure = readPressure();
    this->altitude = readAltitude();

    if(this->altitude > this->maxAltitudeReading){
       	this->maxAltitudeReading = this->altitude;//sets maximum altitude if current is greater than max
    }

    if(!this->reachedApogee){
    this->reachedApogee = checkApogee();//checks for apogee if it hasnt been reached yet
    }

    if(!this->hasLaunched){
        this->hasLaunched = checkLaunched();//checks for launch if it hasnt happened yet
    }

    Serial.print("Pressure: ");
    Serial.println(this->pressure);
    Serial.print("Altitude: ");
    Serial.println(this->altitude);

}

/*
 * Function for checking if apogee has been reached
 * @return true if current alt is significantly less than maximum alt
 */
bool MPL3115A2::checkApogee(){
	return (this->altitude < this->maxAltitudeReading - 5);
}

/*
 * Function for checking if rocket has launched
 * @return true if alt is greater than 5m
 */
bool MPL3115A2::checkLaunched(){
	return  getAltitude() > 5;
}


float MPL3115A2::getPressure() {
    return this->pressure;
}
float MPL3115A2::getAltitude() {
    return this->altitude - this->zeroAltitude;
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

