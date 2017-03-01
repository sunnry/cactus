#ifndef _MAIN_H
 #define _MAIN_H

#define MOTORADRESSE      2   // BLCD�ĵ�ַ (1-4)
#define MANUELL_PWM       0//30  // PWM��ֵ: 0-255  //this value is to used test when no any control input,when set 0 it is unavailable
#define TEST_MANUELL      0//30  // ����Ӳ��  ֵ: 0-255 (PWM)
#define TEST_SCHUB        0	  // ���Ի�����ģʽ       ֵ: 0 = ���� 1 = ����
#define POLANZAHL        12   // ��������,����������ٶȵĵ�����
#define MAX_PWM         255
#define MIN_PWM           3
#define MIN_PPM          10   // PPM-Signal���ص�Լ��
#define FILTER_PPM        7   // ���ٸ�PPM�źŽ��й���(ֵ0~30)

#define SIO_DEBUG         0   // ����ֵ�ڴ��нӿ����
#define X3D_SIO           1	  // serielles Protokoll des X3D (38400Bd) Achtung: dann muss SIO_DEBUG = 0 sein
//��X3D�Ĵ���Э�飨38400Bd��ע������SIO_DEBUG = 0

#define _16KHZ    // ����Ƶ��Ϊ16KHz
//#define _32KHZ  

#ifdef _16KHZ
#define MAX_STROM        130  // PWM���ֵ,���Ϊ13A
#define LIMIT_STROM      65  // PWM����ֵ,���Ϊ6.5Aʱ��PWMֵ
#endif

#ifdef _32KHZ
#define MAX_STROM        130  // PWM���ֵ,���Ϊ13A
#define LIMIT_STROM       50  // PWM����ֵ,���Ϊ5Aʱ��PWMֵ
#endif

#define SYSCLK	8000000L	//ʱ��Ƶ��,��λHz

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/boot.h>
#include <avr/wdt.h>

#include "twislave.h"
#include "old_macros.h"
#include "analog.h"
//#include "uart.h"
#include "timer0.h"
#include "BLMC.h"

#include "PPM_Decode.h"

extern unsigned int  I2C_Timeout;
extern unsigned char SIO_Timeout;
extern unsigned int  PWM;
extern unsigned int  Strom;     //ca. in 0,1A
extern unsigned char Strom_max;
extern unsigned char Mittelstrom;         //ƽ������
extern unsigned int  CntKommutierungen;   //�������
extern unsigned char MotorAnwerfen;       //�������
extern unsigned char MotorGestoppt;       //���ֹͣ
extern unsigned char ZeitZumAdWandeln;    //Ad��ת��ʱ��
extern unsigned char MaxPWM;

#define ROT    0x08
#define GRUEN  0x80	

#if defined(__AVR_ATmega8__)
#  define OC1   PB1
#  define DDROC DDRB
#  define OCR   OCR1A
#  define PWM10 WGM10
#  define PWM11 WGM11
#endif 


#endif //_MAIN_H





