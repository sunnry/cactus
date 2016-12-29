#ifndef _esc_control_h_
#define _esc_control_h_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define LEFT_ESC_PIN  7
#define RIGHT_ESC_PIN 8

#define INIT_ESC_MAX_POWER  25
#define INIT_ESC_MIN_POWER  0

#define ESC_SELF_TEST_POWER 16
#define ESC_STOP_POWER 15

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
