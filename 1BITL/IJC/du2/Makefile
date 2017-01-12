all: tail tail2 wordcount wordcount-dynamic

CFLAGS= -std=c99 -Wall -pedantic -Wextra
CXXFLAGS= -std=c++11 -pedantic -Wall 

tail : tail.c
	gcc $(CFLAGS) $< -o $@
tail2: tail2.cc
	g++ $(CXXFLAGS) $< -o $@
wordcount: wordcount.c io.o libhtable.a
	gcc $(CFLAGS) -o $@ $^
wordcount-dynamic: wordcount.c io.o libhtable.so
	gcc $(CFLAGS) -o $@ $^
io.o: io.c io.h
	gcc $(CFLAGS) -c $< -o $@
hash_function.o: hash_function.c libhtable.h
	gcc $(CFLAGS) -fPIC -c $< -o $@
htab_clear.o: htab_clear.c libhtable.h
	gcc $(CFLAGS) -fPIC -c $< -o $@
htab_foreach.o: htab_foreach.c libhtable.h
	gcc $(CFLAGS) -fPIC -c $< -o $@
htab_free.o: htab_free.c libhtable.h
	gcc $(CFLAGS) -fPIC -c $< -o $@
htab_init2.o: htab_init2.c libhtable.h
	gcc $(CFLAGS) -fPIC -c $< -o $@
htab_init.o: htab_init.c libhtable.h
	gcc $(CFLAGS) -fPIC -c $< -o $@
htab_lookup_add.o: htab_lookup_add.c libhtable.h
	gcc $(CFLAGS) -fPIC -c $< -o $@
htab_remove.o: htab_remove.c libhtable.h
	gcc $(CFLAGS) -fPIC -c $< -o $@
libhtable.o: libhtable.c libhtable.h
	gcc $(CFLAGS) -fPIC -c $< -o $@
libhtable.a: htab_clear.o htab_foreach.o htab_free.o htab_init2.o htab_init.o htab_lookup_add.o htab_remove.o hash_function.o
	ar crs $@ $^
libhtable.so: htab_clear.o htab_foreach.o htab_free.o htab_init2.o htab_init.o htab_lookup_add.o htab_remove.o hash_function.o
	gcc $(CFLAGS) -shared -o $@ $^
clean:
	rm -f tail tail2 wordcount wordcount-dynamic libhtable.a libhtable.so io.o htab_clear.o htab_foreach.o htab_free.o htab_init2.o htab_init.o htab_lookup_add.o htab_remove.o hash_function.o

zip:
	zip xtomas32.zip *.c *.cc *.h Makefile
