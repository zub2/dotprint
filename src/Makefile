# Makefile

APP=dotprint

OBJECTS=DotPrint.o CairoTTY.o PageSizeFactory.o PreprocessorFactory.o CmdLineParser.o
BINARY=$(APP)

CC=gcc
CXX=g++
CPPFLAGS=-Wall -pedantic -g3
CFLAGS=$(CPPFLAGS)

INCLUDES=`pkg-config --cflags glibmm-2.4 cairomm-1.0`
LIBS=`pkg-config --libs glibmm-2.4 cairomm-1.0`

#=========================================================================

.PHONY: all clean

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

%.o: %.cc
	$(CXX) -c $(CPPFLAGS) $(INCLUDES) $< -o $@

all: $(APP)

$(APP): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) -o $(APP)

clean:
	rm -f $(BINARY) $(OBJECTS)

# dependencies
DotPrint.o: DotPrint.cc CairoTTY.h
CairoTTY.o: CairoTTY.cc CairoTTY.h
