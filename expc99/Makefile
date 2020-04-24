PLATFORM=ArduinoMEGA

DEVICE=/dev/ttyACM0

CC=avr-gcc
CPPFLAGS=-DF_CPU=16000000UL -D$(PLATFORM) -I.
CFLAGS=-Wall -std=c99 -Os -mmcu=$(MCU) -fshort-enums
LDFLAGS=-std=c99 -mmcu=$(MCU)

# Platform dependent parameters
ifeq ($(PLATFORM),ArduinoONE)
MCU=atmega328p
PROGRAMMER=arduino
DUDEOPTS=
else ifeq ($(PLATFORM),ArduinoMEGA)
MCU=atmega2560
PROGRAMMER=wiring
DUDEOPTS=-D
endif




SOURCES = adc.c motor.c shielditic.c ticker.c vaction.c \
          main.c pin.c switch.c timer.c \
	  test_timer.c test_timer2.c test_pin.c test_ticker.c test_adc.c \
	  test_switch_1.c test_switch_2.c test_switch_3.c

vpath %.c test

# Default
.PHONY: all tests
all:	main
tests:  test_timer test_timer2 test_pin test_ticker test_adc \
	test_switch_1 test_switch_2 test_switch_3



# Utils
.PHONY: clean veryclean
clean:
	\rm -f *~ *.o *.s *.hex *.d

veryclean: clean
	\rm -f  \#*\# main test_*[!c]

# Regles genèriques
%.hex: %
	avr-objcopy -Oihex  $< $@

implanta_%: %.hex
	avrdude $(DUDEOPTS) -c $(PROGRAMMER) -p $(MCU) -P $(DEVICE) -U $<

%.s: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -S $<

%.d: %.c
	@set -e; echo "redo $@"; rm -f $@; \
        $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
        sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# Link rules
main: vaction.o timer.o motor.o
test_adc: adc.o ticker.o shielditic.c
test_ticker: ticker.o shielditic.o
test_timer: pin.o timer.o
test_timer2: pin.o timer.o
test_pin: pin.o
test_switch_1: switch.o shielditic.o
test_switch_2: switch.o shielditic.o
test_switch_3: switch.o shielditic.o ticker.o
test_switch_4: switch.o shielditic.o ticker.o adc.o

# Makefile rules

include $(SOURCES:.c=.d)
