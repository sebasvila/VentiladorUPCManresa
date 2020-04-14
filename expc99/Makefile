DEVICE=/dev/ttyACM0
CC=avr-gcc
CPPFLAGS=-DF_CPU=16000000UL
CFLAGS=-Wall -std=c99 -Os -mmcu=atmega328p -fshort-enums
LDFLAGS=-std=c99 -mmcu=atmega328p

# Default
.PHONY: all
all:	main


# Utils
.PHONY: clean veryclean
clean:
	\rm -f *~ *.o *.s *.hex

veryclean: clean
	\rm -f  \#*\# main


# Regles genèriques
%.hex: %
	avr-objcopy -Oihex  $< $@

implanta_%: %.hex
	avrdude -c arduino -p atmega328p -P $(DEVICE) -U $<

%.s: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -S $<

# Dependències

main: vaction.o timer.o shielditic.o
main.o: main.c pt.h lc.h lc-switch.h vaction.h pin.h
motor.o: motor.c pin.h motor.h
pin.o: pin.c pin.h
shielditic.o: shielditic.c shielditic.h
timer.o: timer.c timer.h
vaction.o: vaction.c pt.h lc.h lc-switch.h timer.h pin.h shielditic.h \
 vaction.h
ticker.o: ticker.h
adc.o: adc.h
