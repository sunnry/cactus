#ifndef _SERVOCONTROL_H_
#define _SERVOCONTROL_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Servo.h> 

#define SERVO1_PIN_NUM      11   //p35 on rov main control board, and pin24 on atmega2560
//#define SERVO2_PIN_NUM

#define ROTATE_INTERNAL_DELAY   15

class ServoControl{
  public:
    ServoControl(int pin);

    void setServoPos(int pos);

    int getServoCurrentPos(void);

  private:
    int pinNum;
    int pos;
    Servo *servo;
};

#endif
