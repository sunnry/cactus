//该方式使用OCR4A作为PWM反转的TOP值，所以A通道无法输出可调PWM，B通道和C通道可以借助A通道实现
//频率可调的PWM输出，从而驱动esc

//下面是servopulse函数部分(此函数意思:也就是說每次都是0.5ms高電平 1.98ms低電平 然後再0.52ms低電平 17ms延時也是低電平)
//servopulse 用软件模拟PPM信号用来初始化电调
//params: val1 代表量程最小0，最大180,0代表1ms,180代表2ms
void servopulse(int pin,int val1)//定义一个脉冲函数
{
  int myangle1=map(val1,0,180,1000,2000);
  digitalWrite(pin,HIGH);//将舵机接口电平至高
  delayMicroseconds(myangle1);//延时脉宽值的微秒数
  digitalWrite(pin,LOW);//将舵机接口电平至低
  delay(20-val1/1000);
}

//params: max_power 最大量程，最大值180
//params: min_power 最小量程 最小值0
void init_esc(int pin,int max_power,int mini_power){
  pinMode(pin,OUTPUT);
  //看电调说明书，设置油门行程时，一开始需要把遥控器打到最高点。i<=110大概是2杪多
  for(int i=0;i<110;i++){
    servopulse(pin,max_power);
  }

  //等电机发出B-B两声后（就是两秒后，大概就是这里了）把油门打到最低点
   for(int i=0;i<=55;i++){
     servopulse(pin,mini_power);//引用脉冲函数
   }
}

void start_esc(int pin,int power){
  if((pin>=7) && (pin <=8)){ //timer4
    pinMode(pin,OUTPUT);
    TCCR4B = TCCR4B & 0B11100000 | 0x4 | 0x8;
    OCR4A = 312;
    
    if(pin == 7){
      TCCR4A = TCCR4A | 0B10100011;
      OCR4B = power;
    }else if(pin == 8){
      TCCR4A = TCCR4A | 0B10001011;
      OCR4C = power;  
    }
  }
}

void setup() {
  pinMode(8,OUTPUT);  //将8设置成输出，避免产生的PPM信号对8产生干扰
  init_esc(7,90,0);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i =0;i<=32;i++){
    start_esc(7,68+i);
    delay(100);
  }
}
