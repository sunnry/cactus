
#include "esc_control.h"

//该方式使用OCRnA作为PWM反转的TOP值，所以A通道无法输出可调PWM，B通道和C通道可以借助A通道实现
//频率可调的PWM输出，从而驱动esc

//电机需要以INIT_ESC_MAX_POWER转动一次，然后再以ESC_SELF_TEST_POWER转动一次，才能证明电调初始化成功，否则需要控制板复位重新
//初始化电调，在电调不掉电的情况下，一般复位重新初始化都能成功。
esc_control::esc_control(int pin){
  pinNum = pin;
  //config esc 
  init_esc(INIT_ESC_MAX_POWER,INIT_ESC_MIN_POWER);

  //self test esc control
  start_esc(ESC_SELF_TEST_POWER);
  for(int i=0;i<30;i++){
    delay(200);
  }
  start_esc(ESC_STOP_POWER);
};

//下面是servopulse函数部分(此函数意思:也就是說每次都是0.5ms高電平 1.98ms低電平 然後再0.52ms低電平 17ms延時也是低電平)
//servopulse 用软件模拟PPM信号用来初始化电调
//params: duty 代表量程最小0，最大180,0代表1ms,180代表2ms
void esc_control::servopulse(int duty)//定义一个脉冲函数
{
  int myangle=map(duty,0,180,1000,2000);
  digitalWrite(pinNum,HIGH);//将舵机接口电平至高
  delayMicroseconds(myangle);//延时脉宽值的微秒数
  digitalWrite(pinNum,LOW);//将舵机接口电平至低
  delay(20-duty/1000);
};

//每次电调上电的时候都需要重新初始化，否则无法启动
//params: max_power 最大量程，最大值180
//params: min_power 最小量程 最小值0
void esc_control::init_esc(int max_power,int mini_power){
  pinMode(pinNum,OUTPUT);
  //看电调说明书，设置油门行程时，一开始需要把遥控器打到最高点。i<=110大概是2杪多
  for(int i=0;i<=110;i++){
    servopulse(max_power);
  }

  //等电机发出B-B两声后（就是两秒后，大概就是这里了）把油门打到最低点
  for(int i=0;i<=55;i++){
    servopulse(mini_power);//引用脉冲函数
  }
};

//电调启动函数，在初始化电调后，可以用该代码控制电机的转速
//params:power power=16 1ms power=32 2ms
void esc_control::start_esc(int power){
  if((pinNum>=7) && (pinNum <=8)){ //timer4
    pinMode(pinNum,OUTPUT);
    TCCR4B = TCCR4B & 0B11100000 | 0x5 | 0x18;  //16000000/1024/312=50
    OCR4A = 312;
    
    if(pinNum == 7){
      TCCR4A = TCCR4A | 0B10100011;  //open timer4 B channel
      OCR4B = power;
    }else if(pinNum == 8){
      TCCR4A = TCCR4A | 0B10001011;  //open timer4 C channel
      OCR4C = power;  
    }
  }
};


