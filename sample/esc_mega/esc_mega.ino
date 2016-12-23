int sp1=6;//定义ppm 接口 对应主控板的P16
int pulsewidth;//定义脉宽变量
int val;
int val1;
int myangle1;
//下面是servopulse函数部分(此函数意思:也就是說每次都是0.5ms高電平 1.98ms低電平 然後再0.52ms低電平 17ms延時也是低電平)
void servopulse(int pin,int val1)//定义一个脉冲函数
{
  //myangle1=map(val1,0,180,500,2480);
  myangle1=map(val1,0,180,1000,2000);
  digitalWrite(pin,HIGH);//将舵机接口电平至高
  delayMicroseconds(myangle1);//延时脉宽值的微秒数
  digitalWrite(pin,LOW);//将舵机接口电平至低
  delay(20-val1/1000);
}
//servopulse函数部分到此结束
void setup()
{
  pinMode(sp1,OUTPUT);//设定舵机接口为输出接口
  //设置两组串口波特率
  //Serial.begin(9600);
 
  //Serial.println("servu=o_seral_simple ready" ) ;
 
  //看电调说明书，设置油门行程时，一开始需要把遥控器打到最高点。i<=110大概是2杪多
  for(int i=0;i<=110;i++)
    {
      servopulse(sp1,180);//引用脉冲函数
    }
 
//等电机发出B-B两声后（就是两秒后，大概就是这里了）把油门打到最低点
    for(int i=0;i<=55;i++)
    {
      servopulse(sp1,0);//引用脉冲函数
    }
 
//后面提示后，就可以控制电机转动了
    for(int i=0;i<=180;i++)
    {
      servopulse(sp1,180);//引用脉冲函数
    }

    //Serial.println("hello world" ) ;
 
}
 
void loop()//将0到9的数转化为0到180角度，并让LED闪烁相应数的次数
{
  val='8';
/*
  if(val>'0'&& val<='9')
  {
    val1=val-'0';//将特征量转化为数值变量
    val1=map(val1,0,9,0,180);//将角度转化为500-2480的脉宽值
    Serial.print("moving servo to ");
    Serial.print(val1,DEC);
    Serial.println();
    for(int i=0;i<=50;i++)//给予舵机足够的时间让它转到指定角度
    {
      servopulse(sp1,val1);//引用脉冲函数
    }
  }
  */
}
