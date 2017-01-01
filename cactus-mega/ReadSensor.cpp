#include "ReadSensor.h"

ReadSensor::ReadSensor(){
   //analogReference(INTERNAL);  //use internal adc reference
}


double ReadSensor::ReadBatteryVoltage(void){
  int value = analogRead(BATTERY_VOLTAGE_PIN);
  double voltage = value * (5.0/1024.0);
  return voltage;
}

double ReadSensor::ReadDHT11(void){
  return 0;
}

