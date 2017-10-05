all: ftrest ftrestd

CFLAGS= -std=c++11 #-pedantic -Wall
CC=g++

ftrest: ClientArgParser.cpp Connection.cpp HttpHeader.cpp client.cpp
	$(CC) $(CFLAGS) -o $@ $^ 
ftrestd: Connection.cpp HttpHeader.cpp ServerArgParser.cpp ServerFunctions.cpp server.cpp
	$(CC) $(CFLAGS) -o $@ $^ 

pack:
	tar -cvzf xtomas32.tgz *.cpp *.h Makefile readme.md

clean:
	rm *.o
