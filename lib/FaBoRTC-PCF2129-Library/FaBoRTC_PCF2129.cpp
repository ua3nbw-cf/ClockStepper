/**
 @file FaBoRTC_PCF2129.cpp
 @brief This is a library for the FaBo RTC I2C Brick.

   http://fabo.io/215.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#include "FaBoRTC_PCF2129.h"

/**
 @brief Constructor
*/
FaBoRTC_PCF2129::FaBoRTC_PCF2129(uint8_t addr) {
  _i2caddr = addr;
  }

/**
 @brief Search Device
 @retval true device connected
 @retval false device error
*/
bool FaBoRTC_PCF2129::searchDevice(void) {
  return !(readI2c(_i2caddr) >>7);
}

/**
 @brief Configure Device
*/
void FaBoRTC_PCF2129::configure(void) {
  set1_60Hz();
  set12mode();
  Wire.beginTransmission(_i2caddr);
  Wire.write(RTC_WATCHDG_TIM_CTL); //0x10 //режим прерывания
  Wire.write(0x23); //
  Wire.endTransmission();
}

/**
 @brief Get Seconds from RTC
 @param [out] seconds seconds 
*/
uint8_t FaBoRTC_PCF2129::getSeconds(void) {
  return bcdToDec(readI2c(PCF2129_SECONDS));
}

/**
 @brief Set Seconds to RTC
 @param [in] seconds seconds 
*/
void FaBoRTC_PCF2129::setSeconds(uint8_t seconds) {
  if ( seconds>59 && seconds<0 ) {
    seconds = 0;
  }
  writeI2c(PCF2129_SECONDS, (decToBcd(seconds) + 0x80));
}

/**
 @brief Get Minutes from RTC
 @param [out] minutes minutes 
*/
uint8_t FaBoRTC_PCF2129::getMinutes(void) {
  return bcdToDec(readI2c(PCF2129_MINUTES));
}

/**
 @brief Set Minutes to RTC
 @param [in] minutes minutes 
*/
void FaBoRTC_PCF2129::setMinutes(uint8_t minutes) {
  if ( minutes>59 && minutes<0 ) {
    minutes = 0;
  }
  writeI2c(PCF2129_MINUTES, decToBcd(minutes));
}

/**
 @brief Get Hours from RTC
 @param [out] hours hours 
*/
uint8_t FaBoRTC_PCF2129::getHours(void) {
  return bcdToDec(readI2c(PCF2129_HOURS));
}

/**
 @brief Set Hours to RTC
 @param [in] hours hours 
*/
void FaBoRTC_PCF2129::setHours(uint8_t hours) {
  set24mode();
  if ( hours>23 && hours<0 ) {
    hours = 0;
  }
  writeI2c(PCF2129_HOURS, decToBcd(hours));
}


/**
 @brief Read from RTC
 @param [out] Time Time 
*/
Time FaBoRTC_PCF2129::now(void) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(PCF2129_SECONDS);
  Wire.endTransmission();
  Wire.requestFrom(_i2caddr, (uint8_t)3);
  while(!Wire.available());
  uint8_t seconds = bcdToDec(Wire.read());
  uint8_t minutes = bcdToDec(Wire.read());
  uint8_t hours = bcdToDec(Wire.read());
  
  Wire.read(); // blank read weekdays


  return Time(hours, minutes, seconds);
}


/**
 @brief Set to RTC
 @param [in] Time Time 
*/
void FaBoRTC_PCF2129::setTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(PCF2129_SECONDS);
  Wire.write(decToBcd(seconds) + 0x80);
  Wire.write(decToBcd(minutes));
  Wire.write(decToBcd(hours));
  Wire.write(0x00);
  Wire.endTransmission();
}





/**
 @brief Set to 12 hour mode
*/
void FaBoRTC_PCF2129::set12mode(void) {  //0x04  //1 = 12 hour mode
  uint8_t ctrl;
  ctrl = readCtrl();
  ctrl |= PCF2129_CONTROL_12_24;
  writeCtrl(ctrl);
}

/**
 @brief Set to 24 hour mode
*/
void FaBoRTC_PCF2129::set24mode(void) {  //0x04 //0 = 24 hour mode
  uint8_t ctrl;
  ctrl = readCtrl();
  ctrl &= ~(PCF2129_CONTROL_12_24);
  writeCtrl(ctrl);
}

/**
 @brief Set to 12 hour mode
*/
void FaBoRTC_PCF2129::set1_60Hz(void) { 
  uint8_t ctrl;
  ctrl = readCtrl();
  ctrl |= PCF2129_CONTROL_1_60H; //0x02 //1 = Minute interrupt disabled, 1 = enabled 
  writeCtrl(ctrl);
  ctrl = readCtrl();
  ctrl &= ~(PCF2129_CONTROL_1_SEC); //0x01 //0  = Second interrupt disabled, 1 = enabled
  writeCtrl(ctrl);

}

/**
 @brief Set to 24 hour mode
*/
void FaBoRTC_PCF2129::set1_sec(void) { 
  uint8_t ctrl;
  ctrl = readCtrl();
  ctrl |= PCF2129_CONTROL_1_SEC; //0x01 //1  = Second interrupt disabled, 1 = enabled
  writeCtrl(ctrl);
}

////////////////////////////////////////////////////////////////

/**
 @brief BCD to DEC
 @param [in] value BCD value 
 @param [out] value DEC value 
*/
uint8_t FaBoRTC_PCF2129::bcdToDec(uint8_t value) {
  return (uint8_t) ( (value/16*10) + (value%16) );
}

/**
 @brief DEC to BCD
 @param [in] value DEC value 
 @param [out] value BCD value 
*/
uint8_t FaBoRTC_PCF2129::decToBcd(uint8_t value) {
  return (uint8_t) ( (value/10*16) + (value%10) );
}

/**
 @brief Read I2C Data
 @param [in] address register address 
 @param [out] data read data 
*/
uint8_t FaBoRTC_PCF2129::readI2c(uint8_t address) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(_i2caddr, (uint8_t)1);
  while(!Wire.available());
  return Wire.read();
}

/**
 @brief Write I2C Data
 @param [in] address register address 
 @param [in] data write data 
*/
void FaBoRTC_PCF2129::writeI2c(uint8_t address, uint8_t data) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

/**
 @brief Read Control Register
 @param [out] data register data 
*/
uint8_t FaBoRTC_PCF2129::readCtrl(void) {
  return readI2c(PCF2129_CONTROL_REGISTERS);
}

/**
 @brief Write Control Register
 @param [in] data register data 
*/
void FaBoRTC_PCF2129::writeCtrl(uint8_t data) {
  writeI2c(PCF2129_CONTROL_REGISTERS, data);
}

////////////////////////////////////////////////////////////////

/**
 @brief Constructor
*/
Time::Time(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  hh = hours;
  mm = minutes;
  ss = seconds;
}

