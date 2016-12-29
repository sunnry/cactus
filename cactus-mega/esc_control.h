#ifndef _esc_control_h_
#define _esc_control_h_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


class esc_control {
public:
    esc_control(int pin);
    void esc_control::start_esc(int power);

private:
    int pinNum;
    void init_esc(int max_power,int mini_power);
    void esc_control::servopulse(int duty);
};

#endif
