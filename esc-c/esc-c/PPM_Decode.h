/*############################################################################
############################################################################*/

extern volatile unsigned int PPM_Signal;
extern volatile unsigned char PPM_Timeout;
void InitPPM(void);


#define INT0_ENABLE      GIMSK |= 0x40           
#define INT0_DISABLE     GIMSK &= ~0x40           
#define TIM0_START       TIMSK |= 0x01                        
#define TIM0_STOPP       TIMSK &= ~0x01                        
#define ICP_INT_ENABLE   TIMSK |= 0x20     // T/C1 ���벶׽�ж�ʹ��                     
#define ICP_INT_DISABLE  TIMSK &= ~0x20    // T/C1 ���벶׽�жϽ�ֹ                     
#define TIMER1_INT_ENABLE   TIMSK |= 0x04  // T/C1 ����ж�ʹ��                      
#define TIMER1_INT_DISABLE  TIMSK &= ~0x04 // T/C1 ����жϽ�ֹ                         
#define TIMER2_INT_ENABLE   TIMSK |= 0x40                        
#define TIMER2_INT_DISABLE  TIMSK &= ~0x40                        
#define INT0_POS_FLANKE   MCUCR |= 0x01
#define INT0_ANY_FLANKE   MCUCR |= 0x01
#define INT0_NEG_FLANKE   MCUCR &= ~0x01
#define CLR_INT0_FLAG     GIFR &= ~0x40
#define INIT_INT0_FLANKE  MCUCR &= ~0x03; MCUCR |= 0x02;
#define TIMER0_PRESCALER  TCCR0
#define ICP_POS_FLANKE    TCCR1B |= (1<<ICES1) //�����ش���
#define ICP_NEG_FLANKE    TCCR1B &= ~(1<<ICES1)//�½��ش���
