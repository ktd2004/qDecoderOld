##############################################################
## Makefile for 'qDecoder'                                  ##
##                                                          ##
##    Official distribution : ftp://ftp.hongik.com          ##
##        Technical contact : nobreak@hongik.com            ##
##                                                          ##
##                          Developed by 'Seung-young, Kim' ##
##                                                          ##
##                               (c) Hongik Internet, Inc.  ##
##############################################################

#
# Define
#
LIBNAME = libqDecoder.a

# Which compiler
CC      = gcc

# System library directory
LIBDIR		= /usr/lib/
HEADERDIR	= /usr/include/

# Where are include files kept
INCLUDE = -I./

# Options for release
CFLAGS  = -Wall

# Which library archiver
AR	= ar
ARFLAGS = -q

# Which ranlib
RANLIB	= ranlib

OBJ  = $(OBJ1) $(OBJ2)
OBJ1 = qDecoder.o
OBJ2 = 

## Make Library
all: $(OBJ)
	$(AR) $(ARFLAGS) $(LIBNAME) $(OBJ)
	$(RANLIB) $(LIBNAME)

reall: clean all

## Compile Module
.c.o:
	$(CC) $(INCLUDE) $(CFLAGS) -c -o $@ $<

## Install Module
install: all
	cp qDecoder.h $(HEADERDIR)/qDecoder.h
	cp $(LIBNAME) $(LIBDIR)/$(LIBNAME)

deinstall:
	rm -f $(HEADERDIR)/qDecoder.h
	rm -f $(LIBDIR)/$(LIBNAME)

## Clear Module
clean:
	rm -f $(OBJ) $(LIBNAME)
