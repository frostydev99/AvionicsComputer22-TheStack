/*
 * ICM20948.h
 *
 *  Created on: Feb 3, 2022
 *  Author: Charlie Tribble and Samay Govani
 */

#include "Arduino.h"
#include <Wire.h>

#define ICM20948_ADDRESS			 0x68	// default I2C address (AD0 pin pulled low)

// ICM20948 register addresses
#define ICM20948_PWR_MGMT_1          0x06
#define ICM20948_ODR_ALIGN_EN        0x09

#define GYRO_CONFIG_1				 0x01	// register ID in bank 2
#define ACCEL_CONFIG_1				 0x14	// register ID in bank 2

#define ICM20948_ACCEL_OUT           0x2D 	// accel data registers begin
#define ICM20948_GYRO_OUT            0x33 	// gyro data registers begin

#define ICM20948_REG_BANK_SEL        0x7F

#define ICM20948_RESET               0x80


// ICM20948 register data bits
#define ICM20948_SLEEP               0x40

#define PLUS_MINUS_250DPS			 0		// 131 	 = 131  LSB / (dps)
#define PLUS_MINUS_500DPS			 1		// 131/2 = 65.5 LSB / (dps)
#define PLUS_MINUS_1000DPS			 2		// 131/4 = 32.8 LSB / (dps)
#define PLUS_MINUS_2000DPS			 3		// 131/8 = 16.4 LSB / (dps)

#define PLUS_MINUS_2G				 0		// 2^14 = 16384 LSB/g
#define PLUS_MINUS_4G				 1		// 2^13 = 8192  LSB/g
#define PLUS_MINUS_8G				 2		// 2^12 = 4096  LSB/g
#define PLUS_MINUS_16G				 3		// 2^11 = 2048  LSB/g



struct Vector
{
    int16_t x;
    int16_t y;
    int16_t z;
};

class ICM20948
{

	uint8_t I2C_addr = ICM20948_ADDRESS;	// default

    TwoWire *_wire;

public:

    ICM20948(uint8_t address);

    bool init();
    void setGyroScale(uint8_t value);
    void setAccScale(uint8_t value);

    void readSensorData();

    void printVector(Vector print);

    void printBuffer();

    Vector getGyroRawValues(); // TODO FOR TESTING ONLY
    Vector getAccRawValues();  //TODO FOR TESTING ONLY

    uint8_t* getRawSensorRegisters();

    int16_t getTempRawValues();

    void setPlusMinus250DPS();
    void setPlusMinus500DPS();
    void setPlusMinus1000DPS();
    void setPlusMinus2000DPS();

    void setPlusMinus2Gs();
    void setPlusMinus4Gs();
    void setPlusMinus8Gs();
    void setPlusMinus16Gs();

    int16_t getRawAccelX();
    int16_t getRawAccelY();
    int16_t getRawAccelZ();
    int16_t getRawGyroX();
    int16_t getRawGyroY();
    int16_t getRawGyroZ();


private:

    uint8_t currBank = 0;
    uint8_t regVal = 0;
    uint8_t SensorRegister[14];			// last polled sensor register values stored here

    int16_t rawAccelX = 0;
    int16_t rawAccelY = 0;
    int16_t rawAccelZ = 0;
    int16_t rawGyroX = 0;
    int16_t rawGyroY = 0;
    int16_t rawGyroZ = 0;

    float accelX = 0;
    float accelY = 0;
    float accelZ = 0;
    float gyroX = 0;
    float gyroY = 0;
    float gyroZ = 0;


    void switchBank(uint8_t newBank);

    uint8_t writeRegister8(uint8_t bank, uint8_t reg, uint8_t val);
    uint8_t writeRegister16(uint8_t bank, uint8_t reg, int16_t val);
    uint8_t readRegister8(uint8_t bank, uint8_t reg);
    int16_t readRegister16(uint8_t bank, uint8_t reg);
    int16_t processHighLowBytes(uint8_t Hbyte, uint8_t Lbyte);

    uint8_t resetSensor();
    void sleep(bool sleep);
    // TODO FOR TESTING ONLY Vector getGyroRawValues();


    void complementaryFilter();

    uint8_t read8I2C(uint8_t regAddress);
    void write8I2C(uint8_t regAddress, uint8_t value);

};


