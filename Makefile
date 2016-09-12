CC=avr-gcc
CFLAGS=-I$(INCDIR) -mmcu=atmega328p -Os -std=c99
LFLAGS=-Wl,-u,vfprintf -lprintf_flt -lm

INCDIR=./include
_DEPS=main.h switches.h treadmill.h uart.h serialParser.h
DEPS=$(patsubst %,$(INCDIR)/%,$(_DEPS))

ODIR=./obj
_OBJ=main.o switches.o treadmill.o uart.o serialParser.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

ELF=main.elf

CONVERTER=avr-objcopy
CONVFLAGS=-O ihex -R .eeprom
HEX=main.hex

INSTALLER=avrdude
PORT=/dev/ttyACM0
IFLAGS=-c arduino -p ATMEGA328P -b 115200 -P $(PORT) -U flash:w:$(HEX)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o $(ELF) $^ $(CFLAGS) $(LFLAGS)

install:
	$(CONVERTER) $(CONVFLAGS) $(ELF) $(HEX)
	$(INSTALLER) $(IFLAGS)

clean:
	rm -f $(ODIR)/*.o *~ core $(ELF) $(HEX)
