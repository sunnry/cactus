;*****************************************
;* BlockESC V3.0                            *
;* 2014-08-26                            *
;* Fuses should be lfuse=0x3f hfuse=0xca *
;*****************************************

.equ	F_CPU		= 16000000
.equ	USE_INT0	= 0
.equ	USE_I2C		= 1
.equ	USE_UART	= 0
.equ	USE_ICP		= 1
.equ	SIMPLE_I2C	= 1

.equ	DEAD_LOW_NS	= 300
.equ	DEAD_HIGH_NS	= 300
.equ	MOTOR_ADVANCE	= 15
.equ	CHECK_HARDWARE	= 1

;*********************
; PORT B definitions *
;*********************
;.equ			= 5	(sck)
;.equ			= 4	(miso)
.equ	ApFET		= 3
.equ	BpFET		= 2
.equ	CpFET		= 1
.equ	rcp_in		= 0	;i r/c pulse input

.equ	INIT_PB		= (1<<ApFET)+(1<<BpFET)+(1<<CpFET)
.equ	DIR_PB		= (1<<ApFET)+(1<<BpFET)+(1<<CpFET)

.equ	ApFET_port	= PORTB
.equ	BpFET_port  = PORTB
.equ	CpFET_port  = PORTB

;*********************
; PORT C definitions *
;*********************
.equ	mux_voltage = 7	; 
.equ	mux_current	= 6	;

.equ	i2c_clk		= 5	; ADC5/SCL
.equ	i2c_data	= 4	; ADC4/SDA
.equ	red_led		= 3 ; red led light
.equ	mux_c		= 2	; ADC2 phase input
.equ	mux_b		= 1	; ADC1 phase input
.equ	mux_a		= 0	; ADC0 phase input

.equ	O_POWER		= 180	;
.equ	O_GROUND	= 33	; 

.equ	INIT_PC		= (1<<i2c_clk)+(1<<i2c_data)+(1<<red_led)
.equ	DIR_PC		= (1<<red_led)

.equ	adc_refs	= (0<<REFS1)+(0<<REFS0)	; Need 5V ref. of temp. will overflow

.MACRO RED_on
	sbi	PORTC, red_led
.ENDMACRO
.MACRO RED_off
	cbi	PORTC, red_led
.ENDMACRO
;*********************
; PORT D definitions *
;*********************
.equ	green_led   = 7
;.equ	sense_star = 6 ; (center star)
.equ	CnFET		= 5
.equ	BnFET		= 4
.equ	AnFET		= 3
.equ	txd		= 1
.equ	rxd		= 0

.equ	INIT_PD		= (1<<txd)+ (1<<green_led)
.equ	DIR_PD		= (1<<AnFET)+(1<<BnFET)+(1<<CnFET)+(1<<txd)+(1<<green_led)

.equ	AnFET_port	= PORTD
.equ	BnFET_port	= PORTD
.equ	CnFET_port	= PORTD

.MACRO GRN_on
	sbi	PORTD, green_led
.ENDMACRO
.MACRO GRN_off
	cbi	PORTD, green_led
.ENDMACRO
