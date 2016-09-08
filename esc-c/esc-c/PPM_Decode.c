/*############################################################################
 + ��ˢ���������
 + ATMEGA8 with 8MHz
 + (c) 01.2007 �������˹
 + ֻ��˽����; / ����ҵ��;ʹ��
 + ����ŵ׼ȷ�Եı�֤
 + ��ҵ��;��Ҫ�ҵ�ͬ��
 + �ô���ʹ��BL_Ctrl 1.0���Ѿ�������Ӳ��
 + www.mikrocontroller.com
############################################################################*/
#include "main.h"
#include "PPM_Decode.h"

volatile unsigned int  PPM_Signal = 0;
volatile unsigned char Timer1Overflow = 0;
volatile unsigned char PPM_Timeout = 0;   // Ϊ0ʱ��Ч

//############################################################################
//
void InitPPM(void)
//############################################################################
{
 TCCR1B |= (1<<ICES1)|(1<<ICNC1); //ʹ�����벶׽����������,�߼���ƽ�������ش������벶׽
 ICP_POS_FLANKE;                  // ���벶׽�����ش���
 ICP_INT_ENABLE;                  // T/C1 ���벶׽�ж�ʹ��
 TIMER1_INT_ENABLE;               // T/C1 ����ж�ʹ��   
}

//############################################################################
//
//SIGNAL(SIG_OVERFLOW1)  // T/C1(16λ) ����ж�
ISR(TIMER1_OVF_vect)  //changed by sunny
//############################################################################
{
 Timer1Overflow++;
}

//############################################################################
//
//SIGNAL(SIG_INPUT_CAPTURE1) // T/C1 ���벶���ж� 
ISR(TIMER1_CAPT_vect)  //changed by sunny
//############################################################################
{
 static unsigned int tim_alt;
 static unsigned int ppm;
 if(TCCR1B & (1<<ICES1))   // �����ش������벶׽
  {
   Timer1Overflow = 0;
   tim_alt = ICR1; 
   ICP_NEG_FLANKE;
   PPM_Timeout = 100;
  }
 else                      // �½��ش������벶׽
  {
    ICP_POS_FLANKE;
#ifdef  _32KHZ 
    ppm = (ICR1 - tim_alt + (int) Timer1Overflow * 256) / 32;
#endif 
#ifdef  _16KHZ 
    ppm = (ICR1 - tim_alt + (int) Timer1Overflow * 512) / 32;
#endif 
    if(ppm < 280) ppm = 280;
    ppm -= 280;
        if(PPM_Signal < ppm)  PPM_Signal++;
        else if(PPM_Signal > ppm)  PPM_Signal--;
    if(FILTER_PPM)      ppm = (PPM_Signal * FILTER_PPM + ppm) / (FILTER_PPM + 1);  // PPM�źŹ���
    PPM_Signal = ppm;

    ZeitZumAdWandeln = 1;
  } 
}

//############################################################################
//
//SIGNAL(SIG_INTERRUPT0)  //�ⲿ�ж� INT0 
ISR(INT0_vect)  //changed by sunny
//############################################################################
{
 CLR_INT0_FLAG;      // ����жϱ�־ 
}
