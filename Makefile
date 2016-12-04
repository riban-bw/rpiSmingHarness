##########################################################
# Makefile:
#	RPiSmingTest Test harness for Sming Framework
#
#	Copyright (c) 2016 Brian Walton
##########################################################

ifneq ($V,1)
Q ?= @
endif


#DEBUG	= -g -O0
DEBUG	= -O3
CC	= g++
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe -fexceptions 

LDFLAGS	= -L/usr/local/lib
LDLIBS	= -lbcm2835

SRC	=	$(wildcard *.cpp)
OBJ	=	$(addprefix obj/,$(notdir $(SRC:.cpp=.o)))
TARGET	=	rpiSmingHarness


all: $(TARGET)

$(TARGET): $(OBJ)
	$Q echo [Link] $<
	$Q $(CC) $(CFLAGS) -o $@ $(OBJ) $(LDLIBS) 

obj/%.o: %.cpp
	$Q echo [Compile] $<
	$Q mkdir -p obj >/dev/null
	$Q $(CC) -c $(CFLAGS) $<  -o $@

clean:
	$Q rm -rf $(TARGET) obj .depend

depend: .depend

.depend: $(SRC)
	$Q echo [Depend]
	$Q rm -f .depend
	$Q $(CC) -MM $^ | sed 's/^/obj\//g' >> ./.depend;

include .depend
