DEVICE=/dev/ttyACM0
CC=avr-gcc
CPPFLAGS=-DF_CPU=16000000UL -I..
CFLAGS=-Wall -std=c99 -Os -mmcu=atmega328p -fshort-enums 
LDFLAGS=-std=c99 -mmcu=atmega328p


VPATH = ..

# Default
.PHONY: all
all:	test_timer test_timer2 test_pin test_ticker


# Utils
.PHONY: clean veryclean
clean:
	\rm -f *~ *.o *.s *.hex

veryclean: clean
	\rm -f  test_*[!c]



# Regles genèriques
%.hex: %
	avr-objcopy -Oihex  $< $@

implanta_%: %.hex
	avrdude -c arduino -p atmega328p -P $(DEVICE) -U $<

%.s: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -S $<


# Dependències
test_ticker: ticker.o shielditic.o
test_timer2: pin.o timer.o
test_timer: pin.o timer.o
test_pin: pin.o

motor.o: motor.c pin.h motor.h
pin.o: pin.c pin.h
shielditic.o: shielditic.c shielditic.h
timer.o: timer.c timer.h
vaction.o: vaction.c pt.h lc.h lc-switch.h timer.h pin.h shielditic.h \
 vaction.h
ticker.o: ticker.h
