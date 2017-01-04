#include "CurrentCheck.h"

CurrentCheck::CurrentCheck(){
  
}

double CurrentCheck::CheckControlMCUPart(void){
   int adc = analogRead(MCU_PART_MAX272_PIN);
   double value = adc * 5.0 / 1024.0 / 0.5;  //0.5v resprent 1A
   return value;
}

