#include "ServoControl.h"

ServoControl::ServoControl(int pin){
  pinNum = pin;
  pos = 0;

  servo = new Servo();
  servo->attach(pinNum);
}


int ServoControl::getServoCurrentPos(void){
  return pos;
}


void ServoControl::setServoPos(int pos){
    int p = 0;
    for(p=0;p<=pos;p++){
      servo->write(p);
      delay(ROTATE_INTERNAL_DELAY);//delay some time to let servo reach position
    }
    pos = p;
}

