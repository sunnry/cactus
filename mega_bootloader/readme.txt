1. how to write arduino bootloader to atmega1280 
   the ATmegaBOOT_168_atmega1280.hex is the arduino bootloader get from arduino software(1.6), you
can use avrisp mkii to write it in software of atmelstudio, i use atmelstudio6.2, before writing
image you need to rewrite fuse of atmega1280 with the information of boards.txt in arduino software
the fuses configuration for atmega1280 is like below:
   mega.bootloader.low_fuses=0xFF
   mega.menu.cpu.atmega1280.bootloader.high_fuses=0xDA
   mega.menu.cpu.atmega1280.bootloader.extended_fuses=0xF5

after rewrite fuses then you can write bootloader image ATmegaBOOT_168_atmega1280.hex to atmega1280,
after write bootloader if you want to write arduino app correctly you need to plugout avrisp mkii header
from arduino board.


2. how to compile arduino app image and write it to atmega1280(same with other avr).
   add build.path = "path your hex path" to preferences.txt and then compile app in arduino ide,
it will create hex files (examples: Blink.ino.hex and Blinkino.with_bootloader.hex) one with bootloader
also, you write the hex file with bootloader use avrisp mkii using atmel studio, then ok.