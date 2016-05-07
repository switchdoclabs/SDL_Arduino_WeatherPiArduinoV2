/*************************************************** 
  This is a library for the AM2315 Humidity & Temp Sensor

  SwitchDoc Labs May 2016 - Fixed Delays and Fixed Double Read problem

  Designed specifically to work with the AM2315 sensor from Adafruit
  ----> https://www.adafruit.com/products/1293

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  

Additional improvements added by Daniel Sandoval/EasternStarGeek  
8 July, 2013.  (Parent Library: Adafruit_AM2315)

BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "ESG_AM2315.h"
#include <util/delay.h>

ESG_AM2315::ESG_AM2315() {
}

boolean ESG_AM2315::readData(float *dataArray) {
  uint8_t reply[10];
  
  Wire.beginTransmission(AM2315_I2CADDR);
  Wire.write(AM2315_READREG);
  Wire.endTransmission();

  delay(50);

  // for reasons unknown we have to send the data twice :/
  // whats the bug here?
  Wire.beginTransmission(AM2315_I2CADDR);
  Wire.write(AM2315_READREG);
  Wire.write(0x00);  // start at address 0x0
  Wire.write(4);  // request 4 bytes data
  Wire.endTransmission();
 
  delay(50); 

  Wire.requestFrom(AM2315_I2CADDR, 8);
  for (uint8_t i=0; i<8; i++) {
    reply[i] = Wire.read();
    //Serial.println(reply[i], HEX);
  }
  
  if ((reply[0] == AM2315_READREG) &&
  (reply[1] == 4))  {
  
  humidity = reply[2];
  humidity *= 256;
  humidity += reply[3];
  humidity /= 10;
 
  dataArray[0] = humidity;

  temp = reply[4];
  temp *= 256;
  temp += reply[5];
  temp /= 10;

// leave in C 
//  dataArray[1] = (temp * 1.8)+32; 
  dataArray[1] = temp;
  return true;
}

else  { 
dataArray[0] = NAN;
dataArray[1] = NAN;
return false;
}

}

/*********************************************************************/
