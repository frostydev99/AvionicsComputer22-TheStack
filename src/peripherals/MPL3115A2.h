


#ifndef MPL3115A2_h
#define MPL3115A2_h

#define MPL3115A2_ADDRESS (0x60) //Default I2C address of sensor
#define MPL3115A2_WHOAMI  (0x0C)//WhoAmI register of barometer
#define MPL3115A2_REGISTER_PRESSURE_MSB (0x01)//Pressure register MSB
#define MPL3115A2_REGISTER_TEMPERATURE_MSB (0x04)
#define MPL3115A2_REGISTER_DR_STATUS (0x00)
#define MPL3115A2_CTRL_REG1 (0x26)

#include "Arduino.h"

class MPL3115A2 {

private:

    uint8_t sensorRegisters[6];

    float pressure = 0;
    float altitude = 0;
    float temperature = 0;

    float maxAltitudeReading = 0;
    float zeroAltitude = 0;

    float rawToPressure(uint8_t msb, uint8_t csb, uint8_t lsb);
    //float rawToAltitude(uint8_t msb, uint8_t csb, uint8_t lsb);
    float rawToTemperature(uint8_t msb, uint8_t lsb);

    float readPressure();
    float readAltitude();
    float readTemperature();

    void toggleOneShot();

    uint8_t read8I2C(uint8_t regAddress);
    void write8I2C(uint8_t regAddress, uint8_t value);

public:
    MPL3115A2();

    void init();
    void disable();

    void setModeBarometer();
    void setModeAltimeter();
    void setOverSampleRate(uint8_t oversampleVal);

    void setZeroAltitude();

    uint8_t* getRawSensorRegisters();
    float getPressure();
    float getAltitude();
    float getTemperature();

    void readSensorData();

};

#endif
