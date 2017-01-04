#include "pwm_light.h"

PwmLight::PwmLight(){
  
};

PwmLight::setLightLumination(int pin,int duty){
  analogWrite(pin,duty);
}

