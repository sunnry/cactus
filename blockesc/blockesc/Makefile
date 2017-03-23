# This Makefile is compatible with both BSD and GNU make

ASM?= avra
SHELL = /bin/bash

.SUFFIXES: .inc .hex

ALL_TARGETS = afro.hex afro2.hex afro_hv.hex afro_nfet.hex arctictiger.hex birdie70a.hex blueesc.hex bs_nfet.hex bs.hex bs40a.hex dlu40a.hex dlux.hex hk200a.hex hm135a.hex kda.hex mkblctrl1.hex rb50a.hex rb70a.hex rct50a.hex tbs.hex tp.hex tp_8khz.hex tp_i2c.hex tp_nfet.hex tp70a.hex tgy6a.hex tgy.hex
AUX_TARGETS = diy0.hex

MOTOR_ID?= 0	# MK-style I2C motor ID, or UART motor number

all: $(ALL_TARGETS)

$(ALL_TARGETS): tgy.asm boot.inc
$(AUX_TARGETS): tgy.asm boot.inc

.inc.hex:
	@test -e $*.asm || ln -s tgy.asm $*.asm
	@echo "$(ASM) -fI -o $@ -D $*_esc -D MOTOR_ID=$(MOTOR_ID) -e $*.eeprom -d $*.obj $*.asm"
	@set -o pipefail; $(ASM) -fI -o $@ -D $*_esc -D MOTOR_ID=$(MOTOR_ID) -e $*.eeprom -d $*.obj $*.asm 2>&1 | grep -v 'PRAGMA directives currently ignored'
	@test -L $*.asm && rm -f $*.asm || true

test: all

clean:
	-rm -f $(ALL_TARGETS) *.obj *.eep.hex *.eeprom *.cof

binary_zip: $(ALL_TARGETS)
	TARGET="tgy_`date '+%Y-%m-%d'`_`git rev-parse --verify --short HEAD`.zip"; \
	git archive -9 -o "$$TARGET" HEAD && \
	zip -9 "$$TARGET" $(ALL_TARGETS) && ls -l "$$TARGET"

program_tgy_%: %.hex
	avrdude -c stk500v2 -b 9600 -P /dev/ttyUSB0 -u -p m8 -U flash:w:$<:i

program_usbasp_%: %.hex
	avrdude -c usbasp -B.5 -p m8 -U flash:w:$<:i

program_avrisp2_%: %.hex
	avrdude -c avrisp2 -p m8 -U flash:w:$<:i

program_dragon_%: %.hex
	avrdude -c dragon_isp -p m8 -P usb -U flash:w:$<:i

program_dapa_%: %.hex
	avrdude -c dapa -p m8 -U flash:w:$<:i

program_uisp_%: %.hex
	uisp -dprog=dapa --erase --upload --verify -v if=$<

bootload_usbasp:
	avrdude -c usbasp -u -p m8 -U hfuse:w:`avrdude -c usbasp -u -p m8 -U hfuse:r:-:h | sed -n '/^0x/{s/.$$/a/;p}'`:m

read: read_tgy

read_tgy:
	avrdude -c stk500v2 -b 9600 -P /dev/ttyUSB0 -u -p m8 -U flash:r:flash.hex:i -U eeprom:r:eeprom.hex:i

read_usbasp:
	avrdude -c usbasp -u -p m8 -U flash:r:flash.hex:i -U eeprom:r:eeprom.hex:i

read_avrisp2:
	avrdude -c avrisp2 -p m8 -P usb -v -U flash:r:flash.hex:i -U eeprom:r:eeprom.hex:i

read_dragon:
	avrdude -c dragon_isp -p m8 -P usb -v -U flash:r:flash.hex:i -U eeprom:r:eeprom.hex:i

read_dapa:
	avrdude -c dapa -p m8 -v -U flash:r:flash.hex:i -U eeprom:r:eeprom.hex:i

read_uisp:
	uisp -dprog=dapa --download -v of=flash.hex

build_blueesc_addresses:
	for MOTOR_ID in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16; do \
		make clean; \
		export MOTOR_ID; \
		make blueesc.hex || exit -1; \
		mv blueesc.hex blueesc"_id"$$MOTOR_ID."hex" || exit -1; \
	done

build_afro_nfet_addresses:
	for MOTOR_ID in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16; do \
		make clean; \
		export MOTOR_ID; \
		make afro_nfet.hex || exit -1; \
		mv afro_nfet.hex afro_nfet"_id"$$MOTOR_ID."hex" || exit -1; \
	done

build_blueesc_addresses_zip:
	make build_blueesc_addresses; \
	TARGET="blueesc_firmware_`date '+%Y-%m-%d'`_`git rev-parse --verify --short HEAD`.zip"; \
	git archive -9 -o "$$TARGET" HEAD && \
	zip -9 "$$TARGET" blueesc_id*.hex && ls -l "$$TARGET"

build_afro_nfet_addresses_zip:
	make build_afro_nfet_addresses; \
	TARGET="afro_nfet_firmware_`date '+%Y-%m-%d'`_`git rev-parse --verify --short HEAD`.zip"; \
	git archive -9 -o "$$TARGET" HEAD && \
	zip -9 "$$TARGET" afro_nfet_id*.hex && ls -l "$$TARGET"

clean_addresses:
	-rm -f blueesc_id*.hex
	-rm -f afro_nfet_id*.hex
