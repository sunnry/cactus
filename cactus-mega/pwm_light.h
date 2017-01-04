#ifndef _PWMLIGHT_H_
#define _PWMLIGHT_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define  PWM_LIGHT_ONE    5//OC3A ATMEGA2560 PIN5  ARDUINO PWM 5
#define  PWM_LIGHT_TWO    2//OC3B ATMEGA2560 PIN6  ARDUINO PWM 2
#define  PWM_LIGHT_THREE  3//OC3C ATMEGA2560 PIN7  ARDUINO PWM 3
//#define  PWM_LIGHT_FOUR   13//OC1C ATMEGA2560 PIN26 ARDUINO PWM 13   this conflict with servo, do we really need it?

class PwmLight{
  public:
    PwmLight(); //number: the pwm lights opened

    setLightLumination(int pin,int duty);

};





#endif
