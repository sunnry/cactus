#include "Arduino_I2C_ESC.h"
#include <Wire.h>

namespace {
  uint8_t _buffer[9];
}

Arduino_I2C_ESC::Arduino_I2C_ESC(uint8_t address, uint8_t poleCount) {
	_address = address;
  _poleCount = poleCount;
}

// Read the incoming data buffer from an ESC
void Arduino_I2C_ESC::readBuffer(uint8_t address, uint8_t buffer[]) {
  Wire.beginTransmission(address);
  Wire.write(0x02); // Data start register
  Wire.endTransmission();
    
  Wire.requestFrom(address,uint8_t(9));
  uint8_t i = 0;
  while(Wire.available()) {
    buffer[i] = Wire.read();
    i++;
  }
}

// Send motor speed command to ESC
void Arduino_I2C_ESC::set(int16_t throttle) {  
  Wire.beginTransmission(_address);
  Wire.write(0x00);
  Wire.write(throttle>>8);
  Wire.write(throttle);  
  Wire.endTransmission();
}

// Send motor speed command to ESC
void Arduino_I2C_ESC::setPWM(int16_t pwm) {  
  set(map(pwm,1100,1900,-32767,32767));
}

void Arduino_I2C_ESC::update() {  
  _buffer[8] = 0x00; // Reset last byte so we can check for alive

  readBuffer(_address,_buffer);
  
  _rpm = (_buffer[0] << 8) | _buffer[1];
  _voltage_raw = (_buffer[2] << 8) | _buffer[3];
  _temp_raw = (_buffer[4] << 8) | _buffer[5];
  _current_raw = (_buffer[6] << 8) | _buffer[7];
  _identifier = _buffer[8];

  _rpm = float(_rpm)/((uint16_t(millis())-_rpmTimer)/1000.0f)*60/float(_poleCount);
  _rpmTimer = millis();
}

bool Arduino_I2C_ESC::isAlive() {
  return (_identifier == 0xab);
}

float Arduino_I2C_ESC::voltage() {
	return float(_voltage_raw)/65536.0f*5.0f*3.13f;
}

float Arduino_I2C_ESC::current() {
  return float(_current_raw*100)/65536.0f*5.0f;
}

float Arduino_I2C_ESC::temperature() {
  int tempture =0;
  //热敏电阻温度表，index表示温度，里面的值表示对应的电阻
  float tempture_table[] = {27.8, 26.6, 25.46, 24.38, 23.35, 22.37, 21.44, 20.55, 19.70, 18.90, 18.13, 
                                  17.39, 16.69, 16.02, 15.39, 14.78, 14.20, 13.64, 13.11, 12.61, 12.12,
                                  11.66, 11.22, 10.79, 10.39, 10.00,  9.63,  9.27,  8.93,  8.61,  8.30,
                                  8.00,  7.71,  7.43,  7.71,  6.92,   6.67,  6.44,  6.22,  6.00,  5.80,
                                  5.60,  5.41,  5.23,  5.05,  4.88,   4.72,  4.57,  4.42,  4.27,  4.13,
                                  4.00,  3.87,  3.75,  3.63,  3.51,   3.40,  3.30,  3.19,  3.09,  3.00,
                                  2.91,  2.82,  2.73,  2.65,  2.57,   2.49,  2.42,  2.35,  2.28,  2.21,
                                  2.15,  2.09,  2.03,  1.97,  1.91,   1.86,  1.80,  1.75,  1.70,  1.66,
                                  1.61,  1.57,  1.52,  1.48,  1.44,   1.40,  1.37,  1.33,  1.29,  1.26,
                                  1.23,  1.19,  1.16,  1.13,  1.10,   1.07,  1.05,  1.02,  0.99,  0.97};

  float voltage = float(_temp_raw)/65536.0f*5.0f; 
  float res = (10.0f*(5.0f - voltage))/voltage;

  for(int i=0;i<100;i++){
    if(res > tempture_table[i]){
      tempture = i;
      break;
    }
  }
  return tempture;
}

int16_t Arduino_I2C_ESC::rpm() {
  return _rpm;
}
