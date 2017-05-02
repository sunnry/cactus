//before start esc, you must send it a stop sinnal, otherwise you can not start esc

#include <Servo.h>

Servo servo;
Servo stop_servo;
Servo revert_servo;

void setup() {

  revert_servo.attach(6);
  revert_servo.writeMicroseconds(1400); //revert signal

  stop_servo.attach(8);
 stop_servo.writeMicroseconds(1500);  //stop signal
  
  servo.attach(7);
  servo.writeMicroseconds(1600);  //forward signal
 
}

void loop() {
   
}
