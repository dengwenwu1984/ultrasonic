# MSP430 Makefile
#######################################
# user configuration:
#######################################
# TARGET: name of the output file
TARGET = main.elf
# MCU: part number to build for
MCU = msp430g2231
# SOURCES: list of input source sources
SOURCES = main.c timer.c pulse.c fsm.c
# OBJECTS
OBJECTS = $(SOURCES:.c=.o)
# define flags
CFLAGS = -mmcu=$(MCU) -g -Os -Wall
LDFLAGS = -mmcu=$(MCU) -g 
PFLAGS = rf2500
#######################################
# end of user configuration
#######################################
#
#######################################
# binaries
#######################################
CC      	= msp430-gcc
RM      	= rm -f
PROGRAMMER	= mspdebug
#######################################


all: $(SOURCES) $(TARGET)

# o file
%.o: %.c
	$(CC) $(CFLAGS) -c $(SOURCES)

# elf file
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

# flash MCU
flash: $(TARGET)
	$(PROGRAMMER) $(PFLAGS) 'erase' 'load $(TARGET)' 'exit'

# remove build artifacts and executables
clean:
	$(RM) $(TARGET) *.o *~

.PHONY: all clean
