#**************************************
#*   Program :               Makefile
#*   Author :                Adrián Tomašov, FIT VUT
#*   Login :                 xtomas32
#*   Skupina :               1BIB(2015/2016)
#*   Created :               04.03.2016
#*   Compiled:               gcc 4.8.4
#*   Project :               IJC-DU1
#*
#*   Notes :        zkompiluje program
#*
#***************************************/

all: primes primes-inline steg-decode

CC=gcc
CFLAGS= -O2 -std=c99  -Wall -pedantic -lm



error.o:error.h error.c

eratosthenes.o: eratosthenes.c eratosthenes.h bit_array.h error.h
primes.o:primes.c bit_array.h error.h eratosthenes.h
ppm.o: ppm.c ppm.h error.h
steg-decode.o:steg-decode.c error.h ppm.h bit_array.h eratosthenes.h

primes-inline.o:primes.c bit_array.h error.h eratosthenes.h
	$(CC) -DUSE_INLINE -c primes.c -o $@ $(CFLAGS)
eratosthenes-inline.o:
	$(CC)  -DUSE_INLINE -c eratosthenes.c -o $@ ${CFLAGS}

primes:primes.o error.o eratosthenes.o
	$(CC) -o $@ primes.o error.o eratosthenes.o $(CFLAGS)
steg-decode:error.o ppm.o eratosthenes.o steg-decode.o
	$(CC) -o $@  error.o ppm.o eratosthenes.o steg-decode.o  $(CFLAGS)
primes-inline:primes-inline.o error.o eratosthenes-inline.o
	$(CC) -o $@ primes-inline.o error.o eratosthenes-inline.o $(CFLAGS) -DUSE_INLINE




clean:
	rm -f *.o primes primes-inline steg-decode
