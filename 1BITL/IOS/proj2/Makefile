
CFLAGS= -std=gnu99 -Wall -Wextra -pedantic

all: proj2

.c.o:
	gcc $(CFLAGS) -c $< -o $@

error.o: error.c globals.h error.h
proj2: main.c error.o globals.h
	gcc $(CFLAGS) -pthread main.c error.o -o $@
clean : 
	rm -f error.o proj2
zip:
	zip xtomas32.zip *.c *.h Makefile
