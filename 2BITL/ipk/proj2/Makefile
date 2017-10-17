all: ipk-scanner ipk-scanner-with-ipv6

src=ArpScannerParser.cpp  main.cpp  NetTools.cpp  Scanner.cpp  XmlCoder.cpp ArpScannerParser.h  NetStruct.h  Scanner.h  XmlCoder.h

CC=g++
FLAGS= -pthread -std=c++11 -fno-stack-protector

ipk-scanner: $(src)
	$(CC) -o $@ $^ $(FLAGS)

ipk-scanner-with-ipv6: $(src)
	$(CC) -o $@ $^ $(FLAGS) -DIPK_IPV6

pack:
	make -f Makefile-latex
	tar -cvzf xtomas32.tgz *.cpp *.h Makefile doc.pdf
	make -f Makefile-latex clean

clean:
	rm ipk-scanner ipk-scanner-with-ipv6 
