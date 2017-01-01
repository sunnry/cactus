#ifndef _READSENSOR_H_
#define _READSENSOR_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define  BATTERY_VOLTAGE_PIN  A4   //PF4

class ReadSensor{

public:
  ReadSensor();

  double ReadBatteryVoltage(void);

  double ReadDHT11(void);
 
};

#endif
