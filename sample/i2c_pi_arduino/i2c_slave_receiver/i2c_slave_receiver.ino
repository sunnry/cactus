
#include <Wire.h>

int led = 50;
int state = 0;
int startFlag = 1;
int stopFlag = 2;
int waitFlag = 0;

void setup() {
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);  
  
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(sendEvent);
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  char c = Wire.read();
  
  if(c == startFlag){
    digitalWrite(led,LOW);
    state = startFlag;
  }
  if(c== stopFlag){
    digitalWrite(led,HIGH);
    state = stopFlag;
  } 
}

void sendEvent(){
  digitalWrite(led,HIGH);
  
  if(state == waitFlag){
    Wire.write(waitFlag); //send 0,iam  waiting  start
  }
  else if(state == 1) //1 started 
    Wire.write(0x55);
  else if(state == 2) // 2 ended
    Wire.write(0xFF);
}

