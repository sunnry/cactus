#ifndef _CURRENTCHECK_H_
#define _CURRENTCHECK_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define  MCU_PART_MAX272_PIN      A7 //PF7 90

class CurrentCheck{
  public:
    CurrentCheck();

    double CheckControlMCUPart(void);
};





#endif
