##### Configuration zone #############################################

# program release
RELEASE=1.0

# user space local install root directory
# assume libaire and libdev is installed here
USRLOCALROOT=$(HOME)/.avrlocal

# testing platform
PLATFORM=ArduinoONE

# device file to upload tests and program
DEVICE=/dev/ttyACM0

# program modules (file suffix not needed)
SRC_MODS = main vaction

# program tests/examples
SRC_TESTS = test_rtc1307_1

# Link rules for tests/examples (may have specific platform requirements to run)
# any test depends on libaire
main: vaction.o -laire -ldev


##### Internal configs ##########################################


# processing tools and processing flags

CC=avr-gcc
CPPFLAGS=
CFLAGS=-Wall -std=gnu99 -Os -fshort-enums
LDFLAGS=-std=c99 
AR=avr-ar
STRIP=avr-strip

# Non modificable names

#SRC_DIST=libdev-$(RELEASE)


# Platform and shield dependent parameters

ifeq ($(PLATFORM),ArduinoONE)
  MCU = atmega328p
  PROGRAMMER = arduino
  DUDEOPTS =
  FREQ = 16000000UL
else ifeq ($(PLATFORM),ArduinoMEGA)
  MCU = atmega2560
  PROGRAMMER = wiring
  DUDEOPTS = -D
  FREQ = 16000000UL
endif


# Processing parameters

CFLAGS   +=  -mmcu=$(MCU)
LDFLAGS  +=  -mmcu=$(MCU) 
CPPFLAGS +=  -D$(PLATFORM) -DF_CPU=$(FREQ) \
             -I$(SRCDIR) -I$(USRLOCALROOT)/include

# Project structure

DEPSDIR = .deps
SRCDIR  = ../src
TESTDIR = ../test
BUILDDIR = .

# Sets of files

SOURCES = $(SRC_MODS) $(SRC_TESTS)
DEPFILES = $(addprefix $(DEPSDIR)/, $(addsuffix .d, $(SOURCES)))

# Search paths

VPATH = $(SRCDIR):$(TESTDIR)

# Library search

.LIBPATTERNS: lib%.a
vpath %.a $(USRLOCALROOT)/lib


##### Main targets #######################################################

.PHONY: all tests clean veryclean

.DEFAULT_GOAL := all

# main program

all: main

# tests

tests:  $(SRC_TESTS)

# Package distribution

# $(SRC_DIST).tar.gz: $(SRC_LIB) $(PUBLIC_HEADERS)
# 	install -D -m 444 --strip-program=$(STRIP) -s -t tmpdist/lib $(SRC_LIB)
# 	for f in $(PUBLIC_HEADERS); \
#            do install -D -m 444  -t tmpdist/include $(SRCDIR)/$$f; \
#         done
# 	tar zcf $(SRC_DIST).tar.gz -C tmpdist .
# 	rm -rf tmpdist


# dist:   $(SRC_DIST).tar.gz

# Utility targets

clean:
	@\rm -f *~ *.o *.s *.hex

veryclean: clean
	@\rm -f  \#*\# main $(SRC_TESTS) $(SRC_DIST).tar.gz
	@\rm -rf $(DEPSDIR)



##### Generic rules ########################################

%.hex: %
	avr-objcopy -Oihex  $< $@

upload_%: %.hex
	avrdude $(DUDEOPTS) -c $(PROGRAMMER) -p $(MCU) -P $(DEVICE) -U $<

%.s: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -S $<

$(DEPSDIR)/%.d: %.c | $(DEPSDIR)
	@set -e; rm -f $@; \
        $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
        sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(DEPSDIR):
	 @mkdir -p $@


##### Automagic dependencies ###############################

-include $(DEPFILES)
