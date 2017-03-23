;*****************************************
;* BlueESC X1                            *
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
.equ	CnFET		= 3
.equ	green_led	= 2
.equ	red_led		= 1
.equ	rcp_in		= 0	;i r/c pulse input

.equ	INIT_PB		= 0
.equ	DIR_PB		= (1<<CnFET)+(1<<green_led)+(1<<red_led)

.equ	CnFET_port	= PORTB

.MACRO RED_on
	sbi	PORTB, red_led
.ENDMACRO
.MACRO RED_off
	cbi	PORTB, red_led
.ENDMACRO
.MACRO GRN_on
	sbi	PORTB, green_led
.ENDMACRO
.MACRO GRN_off
	cbi	PORTB, green_led
.ENDMACRO

;*********************
; PORT C definitions *
;*********************
.equ	mux_temperature = 7	; 
.equ	mux_voltage	= 6	;
.equ	i2c_clk		= 5	; ADC5/SCL
.equ	i2c_data	= 4	; ADC4/SDA
.equ  mux_current = 3 ;x
.equ	mux_c		= 2	; ADC2 phase input
.equ	mux_b		= 1	; ADC1 phase input
.equ	mux_a		= 0	; ADC0 phase input

.equ	O_POWER		= 180	;
.equ	O_GROUND	= 33	; 

.equ	INIT_PC		= (1<<i2c_clk)+(1<<i2c_data)
.equ	DIR_PC		= 0

.equ	adc_refs	= (1<<REFS0)	; Need 5V ref. of temp. will overflow

;*********************
; PORT D definitions *
;*********************
.equ	CpFET		= 7
;.equ	sense_star = 6 ; (center star)
.equ	BnFET		= 5
.equ	BpFET		= 4
.equ	AnFET		= 3
.equ	ApFET		= 2
.equ	txd		= 1
.equ	rxd		= 0

.equ	INIT_PD		= (1<<txd)+(1<<ApFET)+(1<<BpFET)+(1<<CpFET)
.equ	DIR_PD		= (1<<AnFET)+(1<<BnFET)+(1<<ApFET)+(1<<BpFET)+(1<<CpFET)+(1<<txd)

.equ	AnFET_port	= PORTD
.equ	BnFET_port	= PORTD
.equ	ApFET_port	= PORTD
.equ	BpFET_port	= PORTD
.equ	CpFET_port	= PORTD
