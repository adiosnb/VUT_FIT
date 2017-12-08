all: popser

CXXFLAGS= -Wall -pedantic -std=c++11
CC= g++


ArgParser.o: ArgParser.cpp ArgParser.h Exception.h MailMsg.h
Connection.o: Connection.cpp Connection.h debug.h HostConnection.h \
 MsgBuffer.h Exception.h MailMsg.h
Exception.o: Exception.cpp Exception.h MailMsg.h
HostConnection.o: HostConnection.cpp HostConnection.h MsgBuffer.h
Host.o: Host.cpp Host.h ArgParser.h MsgBuffer.h HostConnection.h \
 Pop3Server.h MailDir.h MailMsg.h Exception.h debug.h
MailDir.o: MailDir.cpp MailDir.h MailMsg.h Exception.h
MailDirLogger.o: MailDirLogger.cpp MailDirLogger.h debug.h Exception.h \
 MailMsg.h
MailMsg.o: MailMsg.cpp MailMsg.h debug.h Exception.h MailDirLogger.h
main.o: main.cpp ArgParser.h Exception.h MailMsg.h Connection.h debug.h \
 HostConnection.h MsgBuffer.h Server.h Pop3Server.h MailDir.h Host.h \
 MailDirLogger.h
md5.o: md5.cpp md5.h
MsgBuffer.o: MsgBuffer.cpp MsgBuffer.h debug.h Exception.h MailMsg.h
Pop3Server.o: Pop3Server.cpp Pop3Server.h ArgParser.h MailDir.h MailMsg.h \
 Exception.h md5.h debug.h
Server.o: Server.cpp Server.h Connection.h debug.h HostConnection.h \
 MsgBuffer.h ArgParser.h Pop3Server.h MailDir.h MailMsg.h Host.h \
 Exception.h

popser:ArgParser.o Connection.o Exception.o HostConnection.o Host.o MailDir.o MailDirLogger.o MailMsg.o MsgBuffer.o Pop3Server.o Server.o md5.o main.o
	$(CC) $(CXXFLAGS) -o $@ $^

clean:
	rm -rvf *.o popser

tar:
	make clean
	make -f Makefile-latex
	tar -cvf xtomas32.tar *.h *.cpp Makefile manual.pdf README
	make -f Makefile-latex clean
