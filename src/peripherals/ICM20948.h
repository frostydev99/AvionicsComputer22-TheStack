/*
 * ICM20948.h
 *
 *  Created on: Feb 3, 2022
 *  Author: Charlie Tribble and Samay Govani
 */

#include <Wire.h>

// ICM20948 Register Values
#define ICM20948_PWR_MGMT_1          0x06
#define ICM20948_ODR_ALIGN_EN        0x09

#define ICM20948_ACCEL_OUT           0x2D // accel data registers begin
#define ICM20948_GYRO_OUT            0x33 // gyro data registers begin

#define ICM20948_REG_BANK_SEL        0x7F

#define ICM20948_RESET               0x80

// Register bits
#define ICM20948_SLEEP               0x40

struct Vector
{
    int16_t x;
    int16_t y;
    int16_t z;
};

class ICM20948
{

	int I2C_addr;

    TwoWire *_wire;

public:

    ICM20948(int address);

    bool init();

    void readSensorData();

    void printVector(Vector print);

    void printBuffer();

    Vector getGyroRawValues(); // TODO FOR TESTING ONLY
    Vector getAccRawValues();  //TODO FOR TESTING ONLY

private:

    uint8_t currBank;
    uint8_t regVal;
    uint8_t SensorRegister[14];

    void switchBank(uint8_t newBank);

    uint8_t writeRegister8(uint8_t bank, uint8_t reg, uint8_t val);
    uint8_t writeRegister16(uint8_t bank, uint8_t reg, int16_t val);
    uint8_t readRegister8(uint8_t bank, uint8_t reg);
    int16_t readRegister16(uint8_t bank, uint8_t reg);
    int16_t processHighLowBytes(uint8_t Hbyte, uint8_t Lbyte);

    uint8_t resetSensor();
    void sleep(bool sleep);
    // TODO FOR TESTING ONLY Vector getGyroRawValues();
    int16_t getTempRawValues();

    void complementaryFilter();

};


