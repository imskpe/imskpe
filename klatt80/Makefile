#
# Makefile for klatt software synthesizer
#
#

#
# before installing, change these parameters to suit your local site.
#

BIN = /home/pg/jpi/bin
MAN = /home/pg/jpi/man/man1

CC = gcc
CFLAGS = -g


all: klatt

clean:
	/bin/rm -f *.o *~ klatt

install: klatt
	cp klatt $(BIN)
	cp klatt.man $(MAN)/klatt.1

klatt: klatt.o parwave.o 
	$(CC) $(CFLAGS) -o klatt klatt.o parwave.o -lm

klatt.o : klatt.c parwave.h
	$(CC) $(CFLAGS) -c klatt.c

parwave.o : parwave.c parwave.h
	$(CC) $(CFLAGS) -c parwave.c



