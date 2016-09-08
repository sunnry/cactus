/*****************************************************************************
 
*****************************************************************************/
#include "main.h"
volatile unsigned int CountMilliseconds = 0; //�������
volatile unsigned char Timer0Overflow;       //T/C0 ���
 
enum {
  STOP             = 0,
  CK               = 1,
  CK8              = 2,
  CK64             = 3,
  CK256            = 4,
  CK1024           = 5,
  T0_FALLING_EDGE  = 6,
  T0_RISING_EDGE   = 7
};


//SIGNAL(SIG_OVERFLOW0)
ISR(TIMER0_OVF_vect)  //changed by sunny
{
 static unsigned char cnt;
 Timer0Overflow++;
 if(!cnt--)
  {
   cnt = 3;
   CountMilliseconds += 1;
   if(I2C_Timeout) I2C_Timeout--;
   //if(PPM_Timeout) PPM_Timeout--;
   if(SIO_Timeout) SIO_Timeout--;
  } 
}


void Timer0_Init(void)
{
 TCCR0  = TIMER_TEILER;  //8��Ƶ
// TCNT0 = -TIMER_RELOAD_VALUE;  // reload
 TIM0_START;
 TIMER2_INT_ENABLE;
}


unsigned int SetDelay(unsigned int t)
{
  return(CountMilliseconds + t - 1);                                             
}

char CheckDelay (unsigned int t)
{
  return(((t - CountMilliseconds) & 0x8000) >> 8);
}

void Delay_ms(unsigned int w)
{
 unsigned int akt;
 akt = SetDelay(w);
 while (!CheckDelay(akt));
}
