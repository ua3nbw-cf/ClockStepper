/**
 @file FaBoRTC_PCF2129.h
 @brief This is a library for the FaBo RTC I2C Brick.

   http://fabo.io/215.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#ifndef FABORTC_PCF2129_H
#define FABORTC_PCF2129_H

#include <Arduino.h>
#include <Wire.h>

#define PCF2129_SLAVE_ADDRESS 0x51 ///< I2C Slave Address

/// @name Register Addresses
/// @{
#define PCF2129_CONTROL_REGISTERS 0x00
#define PCF2129_SECONDS 0x03
#define PCF2129_MINUTES 0x04
#define PCF2129_HOURS 0x05
#define RTC_WATCHDG_TIM_CTL  0x10

#define PCF2129_CONTROL_1_SEC 0x01 //1sec
#define PCF2129_CONTROL_1_60H 0x02 //1⁄60 Hz
#define PCF2129_CONTROL_12_24 0x04 //12 hour mode

/// @}

/**
 @class Time
 @brief RTC Time class
*/
class Time {
  public:
    Time (uint8_t hours, uint8_t minutes, uint8_t seconds);
    uint8_t hour()   const { return hh; }
    uint8_t minute() const { return mm; }
    uint8_t second() const { return ss; }
  protected:
    uint8_t hh,mm,ss;
};


/**
 @class FaBoRTC_PCF2129
 @brief FaBo RTC I2C Controll class
*/
class FaBoRTC_PCF2129 {
  public:
    FaBoRTC_PCF2129(uint8_t addr = PCF2129_SLAVE_ADDRESS);
    bool searchDevice(void);
    void configure(void);
    uint8_t getSeconds(void);
    void setSeconds(uint8_t seconds);
    uint8_t getMinutes(void);
    void setMinutes(uint8_t minutes);
    uint8_t getHours(void);
    void setHours(uint8_t hours);
 
    
    Time now(void);
    void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
    
    void set12mode(void);
    void set24mode(void);
    void set1_60Hz(void);
    void set1_sec(void);
  private:
    uint8_t _i2caddr;
    uint8_t bcdToDec(uint8_t value);
    uint8_t decToBcd(uint8_t value);
    uint8_t readI2c(uint8_t address);
    void writeI2c(uint8_t address, uint8_t data);
    uint8_t readCtrl(void);
    void writeCtrl(uint8_t data);
};

#endif // FABORTC_PCF2129_H