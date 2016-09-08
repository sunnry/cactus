/*############################################################################
############################################################################*/

#ifndef BLMC_H_
#define BLMC_H_

extern volatile unsigned char Phase;  //��
extern volatile unsigned char ShadowTCCR1A; //TCCR1A�ľ���
extern volatile unsigned char CompInterruptFreigabe; //�Ƚ��ж��ͷ�

void Blc(void);
void Manuell(void);

#ifdef  _32KHZ 
 #define PWM_C_ON  {TCCR1A = 0xAD; TCCR2  = 0x49;DDRB = 0x0A;} 
 #define PWM_B_ON  {TCCR1A = 0xAD; TCCR2  = 0x49;DDRB = 0x0C;}
 #define PWM_A_ON  {TCCR1A = 0xAD; TCCR2  = 0x69;DDRB = 0x08;}
 #define PWM_OFF   {TCCR1A = 0x0D; TCCR2  = 0x49;PORTC &= ~0x0E;}
#endif 

#ifdef  _16KHZ 
 #define PWM_C_ON  {TCCR1A = 0xA2; TCCR2  = 0x41; DDRB = 0x0A;} 
 #define PWM_B_ON  {TCCR1A = 0xA2; TCCR2  = 0x41; DDRB = 0x0C;}
 #define PWM_A_ON  {TCCR1A = 0xA2; TCCR2  = 0x61; DDRB = 0x08;}
 #define PWM_OFF   {TCCR1A = 0x02; TCCR2  = 0x41;PORTC &= ~0x0E;}
#endif 


#define  STEUER_A_H {PWM_A_ON}
#define  STEUER_B_H {PWM_B_ON}
#define  STEUER_C_H {PWM_C_ON}

#define  STEUER_A_L {PORTD &= ~0x30; PORTD |= 0x08;}
#define  STEUER_B_L {PORTD &= ~0x28; PORTD |= 0x10;}
#define  STEUER_C_L {PORTD &= ~0x18; PORTD |= 0x20;}
#define  STEUER_OFF {PORTD &= ~0x38; PWM_OFF;}

#define SENSE_A ADMUX = 0;      //ADC0 PC0 ��NULL_A
#define SENSE_B ADMUX = 1;      //ADC1 PC1 ��NULL_B
#define SENSE_C ADMUX = 2;      //ADC2 PC2 ��NULL_C

#define ClrSENSE            ACSR |= 0x10    //���ģ��Ƚ����жϱ�־
#define SENSE               ((ACSR & 0x10)) //ģ��Ƚ����жϱ�־��λʱ,SENSEΪ1
#define SENSE_L             (!(ACSR & 0x20))//ģ��Ƚ������Ϊ0ʱ,SENSE_LΪ1
#define SENSE_H             ((ACSR & 0x20)) //ģ��Ƚ������Ϊ1ʱ,SENSE_HΪ1
#define ENABLE_SENSE_INT    {CompInterruptFreigabe = 1;ACSR |= 0x0A; } //ģ��Ƚ����ж�ʹ��,�½��ش���
#define DISABLE_SENSE_INT   {CompInterruptFreigabe = 0; ACSR &= ~0x08; } //ģ��Ƚ����жϽ�ֹ


#define SENSE_FALLING_INT    ACSR &= ~0x01 //ģ��Ƚ���������½��ز����ж�
#define SENSE_RISING_INT    ACSR |= 0x03   //ģ��Ƚ�������������ز����ж�
#define SENSE_TOGGLE_INT    ACSR &= ~0x03  //ģ��Ƚ�������ķ����仯�����ж�

#endif //BLMC_H_

