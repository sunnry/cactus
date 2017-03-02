/*############################################################################
 + ��ˢ���������
 + ATMEGA8 At 8MHz
 + (c) 01.2007 Holger Buss
 + ֻ��˽����;
 + ׼ȷ��û�б�֤
 + δ����ͬ�ⲻ����ҵʹ��
 + �ô���ʹ��BL_Ctrl 1.0���Ѿ�������Ӳ��
 + www.mikrocontroller.com
/############################################################################*/

#include "main.h"

unsigned int  PWM = 0;
unsigned int  Strom = 0; //ca. in 0,1A    //����
unsigned char Strom_max = 0;              //������  
unsigned char Mittelstrom = 0;            //ƽ������
unsigned int  Drehzahl = 0;  // in 100UPM  60 = 6000  //ת��
unsigned int  KommutierDelay = 10; //������ʱ
unsigned int  I2C_Timeout = 0;
unsigned char SIO_Timeout = 0;
unsigned int  SollDrehzahl = 0;  //�ת�٣�����ת��
unsigned int  IstDrehzahl = 0;   //ʵ��ת��
unsigned int  DrehZahlTabelle[256];//vorberechnete Werte zur Drehzahlerfassung //ת�ٱ�
unsigned char ZeitFuerBerechnungen = 1; //��������ʱ��
unsigned char MotorAnwerfen = 0; //���������������
unsigned char MotorGestoppt = 1; //���ֹͣ
unsigned char MaxPWM = MAX_PWM;
unsigned int  CntKommutierungen = 0; //�������
unsigned int  SIO_Drehzahl = 0;      //�����ٶ�
unsigned char ZeitZumAdWandeln = 1;   //��ADת����ʱ��


//############################################################################
//
void SetPWM(void)
//############################################################################
{
    unsigned char tmp_pwm; 
    tmp_pwm = PWM;
    if(tmp_pwm > MaxPWM)    // ��������
        {
        tmp_pwm = MaxPWM;
        PORTC |= ROT; //��ɫLED��
        } 
    if(Strom > MAX_STROM)   // ��������
        {
        OCR1A = 0; OCR1B = 0; OCR2  = 0;
        PORTC |= ROT; //��ɫLED��
        Strom--;
        }
    else 
        {
        #ifdef  _32KHZ 
        OCR1A =  tmp_pwm; 
        OCR1B =  tmp_pwm; 
        OCR2  = tmp_pwm;
        #endif 

        #ifdef  _16KHZ //16kHZʱT1�õ���9λ���ٵ�pwm,���Ը�ֵҪ�ӱ���T2�õ�����λ���������üӱ�
        OCR1A = 2 * (int)tmp_pwm; 
        OCR1B = 2 * (int)tmp_pwm; 
        OCR2  = tmp_pwm;
        #endif 
        }
}

//############################################################################
//
void PWM_Init(void)  //PWM��ʼ��
//############################################################################
{
    PWM_OFF;
    TCCR1B = (1 << CS10) | (0 << CS11) | (0 << CS12) | (1 << WGM12) | 
             (0 << WGM13) | (0<< ICES1) | (0 << ICNC1);
             // T/C1 ��Ԥ��Ƶ,�½��ش������벶׽,��ʹ�����벶׽�������ƹ���,
             // 9λ����PWM,�Ƚ�ƥ��ʱ����OC1A/OC1B(����͵�ƽ)
}

//############################################################################
//
void Wait(unsigned char dauer)   //dauer ����ʱ��
//############################################################################
{
    dauer = (unsigned char)TCNT0 + dauer;
    while((TCNT0 - dauer) & 0x80);
}

//############################################################################
//
void Anwerfen(unsigned char pwm) //�������
//############################################################################
/*����ע�ͣ�
���������㷨
*/
{
    unsigned long timer = 300,i;
    DISABLE_SENSE_INT;   //�ر�ģ��Ƚ����ж�
    PWM = 5;
    SetPWM();//T1��T2��ʱ��������ֵ������pwmռ�ձ�
    Manuell();
    /*����ע�ͣ� 
����˳�����㷨��ע�⻻��ʱ����ͬ���޸ıȽ����˿ڼ������� 
�Ա��ڷ�������綯�Ƶ����л�����ʱ���Զ��л����ջ���ת״̬ 
*/////////////////////////////////////////////////// 
    Delay_ms(200);
    PWM = pwm;
    while(1)
        {
        for(i=0;i<timer; i++) 
            {
            //if(!UebertragungAbgeschlossen)  SendUart(); //UebertragungAbgeschlossen���ת��
            //else DatenUebertragung();  //���ݴ���
            Wait(100);  // warten �ȴ�
            } 
        timer-= timer/15+1;
        if(timer < 25) { if(TEST_MANUELL) timer = 25; else return; }

        Manuell();
        Phase++;
        Phase %= 6;
        AdConvert();  //��������
        PWM = pwm;
        SetPWM();
        if(SENSE) //���ģ��Ƚ����жϱ�־��λ,�̵���
            {
            PORTD ^= GRUEN;
            } 
        }
}

//############################################################################
//
unsigned char SollwertErmittlung(void) //�趨ֵ�Ĳ���ֵ
//############################################################################
{
    static unsigned int sollwert = 0;
    unsigned int ppm;
    if(!I2C_Timeout)   // bei Erreichen von 0 ist der Wert ung�ltig
        {
        if(SIO_Timeout)  // es gibt g�ltige SIO-Daten
            {
            //sollwert =  (MAX_PWM * (unsigned int) SIO_Sollwert) / 200;  // skalieren auf 0-200 = 0-255
            }
        else
            if(PPM_Timeout)  // es gibt g�ltige PPM-Daten
                {
                ppm = PPM_Signal;
                if(ppm > 300) ppm =   0;  // ung�ltiges Signal
                if(ppm > 200) ppm = 200;
                if(ppm <= MIN_PPM) sollwert = 0;
                else 
                    {
                    sollwert = (int) MIN_PWM + ((MAX_PWM - MIN_PWM) * (ppm - MIN_PPM)) / (190 - MIN_PPM);
                    }
                PORTC &= ~ROT;
                }
            else   // Kein g�ltiger Sollwert
                {
                if(!TEST_SCHUB) { if(sollwert) sollwert--; }   
                PORTC |= ROT; 
                }
			
        }
    else // I2C-Daten sind g�ltig
        {
        sollwert = I2C_RXBuffer; 
        PORTC &= ~ROT;
        }
    if(sollwert > MAX_PWM) sollwert = MAX_PWM;
    return(sollwert); 
}

//############################################################################
//������
int main (void)
//############################################################################
{
    char altPhase = 0;
    int test = 0;
    unsigned int MinUpmPulse,Blink,TestschubTimer;
    unsigned int Blink2,MittelstromTimer,DrehzahlMessTimer,MotorGestopptTimer;

    DDRC  = 0x08;//PC3��������ƺ�ɫled 
    PORTC = 0x08;//PC3����ߵ�ƽ����ɫled��
    DDRD  = 0xBA;//10111010A-B-C-����Ϊ�����TXD,LED_GRN(PD7)����Ϊ���,MITTEL/RXD/INT0Ϊ����
    PORTD = 0x00;//LED_GRN�͵�ƽ����ɫ���� 
    DDRB  = 0x0E;////A+B+C+����Ϊ���,����Ϊ���룬ppm��ICPΪ����
    PORTB = 0x31;//I2C����(SDA/SCL)Ϊ�ߵ�ƽ��ICP��������ʹ��,�����͵�ƽ����Ϊ�������ܽ�ֹ������p��������ͨ
    
    Timer0_Init();
    sei ();//��ȫ���ж�
    
    //�̵���˸������ʶ�ĵ����ַ������4���4������ж�
    for(test=0;test<=MOTORADRESSE;test++)
        {
        PORTD |= GRUEN;//�̵���
        Delay_ms(50);
        PORTD &= ~GRUEN;//�̵��� 
        Delay_ms(50);
        }	

    Delay_ms(100);
   
    UART_Init();
    PWM_Init();	

    InitIC2_Slave(0x50);
    InitPPM();

    Blink             = SetDelay(101);    
    Blink2            = SetDelay(102);
    MinUpmPulse       = SetDelay(103); //��Сת������
    MittelstromTimer  = SetDelay(254); //�е������ʱ��
    DrehzahlMessTimer = SetDelay(1005);//���ټ�ʱ��
    TestschubTimer    = SetDelay(1006);//�������Լ�ʱ��
    while(!CheckDelay(MinUpmPulse));
    PORTD |= GRUEN;//LED_GRN�ߵ�ƽ���̵���
    PWM = 0;

    SetPWM();

    SFIOR = 0x08;  // ģ��Ƚ�����·������ʹ��
    ADMUX = 1;     //ADC1��Ϊģ��Ƚ��������븺��

    MinUpmPulse = SetDelay(10);
    //DebugOut.Analog[1] = 1;
    PPM_Signal = 0;

    // zum Test der Hardware; Motor dreht mit konstanter Drehzahl ohne Regelung///����Ӳ��,���ת�ٲ��ܿ���
    if(TEST_MANUELL)    Anwerfen(TEST_MANUELL);  // kommt von dort nicht wieder//���ٴ�û���ؽ�ʱ
                                                 //Anwerfen������������    
    while (1)
        {
        if(!TEST_SCHUB)   PWM = SollwertErmittlung();//�ֵ���� 
        //I2C_TXBuffer = PWM; // Antwort �ber I2C-Bus
        if(MANUELL_PWM)   PWM = MANUELL_PWM;

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if(Phase != altPhase)   // es gab eine Kommutierung im Interrupt�����λ�����ھɵ���λ������
            {
            MotorGestoppt = 0;
            ZeitFuerBerechnungen = 0;    // direkt nach einer Kommutierung ist Zeitֱ����ĳ��֮�����ʵ��ʱ�� 
            MinUpmPulse = SetDelay(50);  // Timeout, falls ein Motor stehen bleibt��ͣ�����ֹͣ
            altPhase = Phase;//����λ=����λ 
            }
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if(!PWM)    // Sollwert == 0
            {
            MotorAnwerfen = 0;      // kein Startversuch����������
            ZeitFuerBerechnungen = 0;//�������õ�ʱ�� 
            // nach 1,5 Sekunden den Motor als gestoppt betrachten ��1��5˲��֮����ֹͣ���
            if(CheckDelay(DrehzahlMessTimer)) 
                {
                DISABLE_SENSE_INT;
                MotorGestoppt = 1;  //���ֹͣ��־λ��
                STEUER_OFF;
                } 
            }
        else 
            {
            if(MotorGestoppt) MotorAnwerfen = 1;	// Startversuch��������
            MotorGestopptTimer = SetDelay(1500);
            }

        if(MotorGestoppt && !TEST_SCHUB) PWM = 0;
        SetPWM();
        // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if(!ZeitFuerBerechnungen++)//�������õ�ʱ��
            {
            if(MotorGestoppt) PORTD |= GRUEN; //else PORTD &= ~GRUEN; //else PORTD &= ~GRUEN;//LED_GRN�ߵ�ƽ��������������
            if(SIO_DEBUG)
                {
                //DebugAusgaben();  // welche Werte sollen angezeigt werden?
                if(!UebertragungAbgeschlossen)  SendUart(); 
                else DatenUebertragung();
                }
            // Berechnen des Mittleren Stroms zur (langsamen) Strombegrenzung�������ڵ�ƽ�������������ģ���������
            if(CheckDelay(MittelstromTimer))   
                {
                MittelstromTimer = SetDelay(50);  // alle 50msÿ��50ms
                if(Mittelstrom <  Strom) Mittelstrom++;// Mittelwert des Stroms bildenƽ�������γ�
                else if(Mittelstrom >  Strom) Mittelstrom--;
       
                if(Mittelstrom > LIMIT_STROM)// Strom am Limit?
                    {
                    MaxPWM--;// dann die Maximale PWM herunterfahren����޶�pwm�ر�
                    PORTC |= ROT; //#define ROT    0x08�� LED_ROT��
                    }
                else 
                    {
                    if(MaxPWM < MAX_PWM) MaxPWM++;
                    }
                }

            if(CheckDelay(DrehzahlMessTimer))   // Ist-Drehzahl bestimmenʵ��ת�ٲⶨ
                {
                DrehzahlMessTimer = SetDelay(10);
                SIO_Drehzahl = (6 * CntKommutierungen) / (POLANZAHL / 2);//���ӻ���
                CntKommutierungen = 0;
                if(PPM_Timeout == 0) // keine PPM-Signale
					ZeitZumAdWandeln = 1;
                }


            if(CheckDelay(TestschubTimer))  
                {
                TestschubTimer = SetDelay(1500);
                if(TEST_SCHUB)//   #define TEST_SCHUB    0  ��ִ��
                    {
                    switch(test) 
                        {
                        case 0: PWM = 50; test++; break;
                        case 1: PWM = 130; test++; break;
                        case 2: PWM = 60;  test++; break;
                        case 3: PWM = 140; test++; break;
                        case 4: PWM = 150; test = 0; break;
                        default: test = 0;
                        } 
                    }
                }
            // Motor Stehen geblieben���ֹͣ����
            if((CheckDelay(MinUpmPulse) && SIO_Drehzahl == 0) || MotorAnwerfen) 
                {
                MotorGestoppt = 1;    //���ֹͣ��־1
                DISABLE_SENSE_INT;
                MinUpmPulse = SetDelay(100);         
                if(MotorAnwerfen)
                    {
                    PORTC &= ~ROT;//LED_ROT��
                    MotorAnwerfen = 0;//���������־0
                    Anwerfen(10);//����
                    PORTD |= GRUEN;//LED_GRN��
                    MotorGestoppt = 0;    //���ֹͣ��־0  
                    Phase--;
                    PWM = 1;
                    SetPWM();
                    SENSE_TOGGLE_INT;//�����ƽ�仯���ж�
                    ENABLE_SENSE_INT;//���Ƚ����ж�
                    MinUpmPulse = SetDelay(100);
                    while(!CheckDelay(MinUpmPulse)); // kurz Synchronisieren
                    PWM = 10;
                    SetPWM();
                    MinUpmPulse = SetDelay(200);
                    while(!CheckDelay(MinUpmPulse)); // kurz Durchstarten
                    MinUpmPulse = SetDelay(1000);
                    altPhase = 7;
                    }
                }
            } // ZeitFuerBerechnungen
        } // while(1) - Hauptschleife
}

