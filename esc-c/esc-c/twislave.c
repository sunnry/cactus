/*############################################################################
Slaveadr = 0x52 = Vorne, 0x54 = Hinten, 0x56 = Rechts, 0x58 = Links
############################################################################*/

#include <avr/io.h>
#include <compat/twi.h>
#include "main.h"

unsigned char I2C_RXBuffer;
unsigned char Byte_Counter=0;

//############################################################################
//I2C (TWI) Interface Init
void InitIC2_Slave(uint8_t adr)
//############################################################################
{ 
    //TWAR = 0b00001000; //ǰ
    //TWAR = 0b00001010; //��
    //TWAR = 0b00001100; //��
    TWAR = 0b00001110; //��
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
}

//############################################################################
//ISR, die bei einem Ereignis auf dem Bus ausgel�st wird. Im Register TWSR befindet
//sich dann ein Statuscode, anhand dessen die Situation festgestellt werden kann.
ISR(TWI_vect)
//############################################################################
/*����ע�ͣ�
TWI�жϣ�ʵ�ֽ���1���ֽڵ�8λ������Ϣ���洢��I2C_RXBuffer��
*/

{   
    switch (TWSR & 0xF8)
        {  
        case SR_SLA_ACK:  
            TWCR |= (1<<TWINT);
            Byte_Counter=0;
            return;
        // Daten Empfangen
        case SR_PREV_ACK:
            I2C_RXBuffer = TWDR;
            I2C_Timeout = 500;
            TWCR |= (1<<TWINT);
            return;
        // Daten Senden
        case SW_SLA_ACK:  
            if (Byte_Counter==0)
                {
                TWDR = Mittelstrom;
                Byte_Counter++;
                }
            else
                {
                TWDR = MaxPWM;
                }
            TWCR |= (1<<TWINT);
            return; 
        // Daten Senden
        case SW_DATA_ACK:
            if (Byte_Counter==0)
                {
                TWDR = Mittelstrom;
                Byte_Counter++;
                }
            else
                {
                TWDR = MaxPWM;
                }
            TWCR |= (1<<TWINT);
            return; 
        // Bus-Fehler zur�cksetzen
        case TWI_BUS_ERR_2:
            TWCR |=(1<<TWSTO) | (1<<TWINT); 
        // Bus-Fehler zur�cksetzen   
        case TWI_BUS_ERR_1:
            TWCR |=(1<<TWSTO) | (1<<TWINT); 
        }
    TWCR =(1<<TWEA) | (1<<TWINT) | (1<<TWEN) | (1<<TWIE); // TWI Reset
}


