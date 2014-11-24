tcp.out : mynew.o tcp.o tcpRunner.o  
	g++ -ansi -Wall -g -o tcp.out mynew.o tcp.o tcpRunner.o  

mynew.o : mynew.cpp mynew.h 
	g++ -ansi -Wall -g -c mynew.cpp

tcp.o : tcp.cpp mynew.h tcp.h 
	g++ -ansi -Wall -g -c tcp.cpp

tcpRunner.o : tcpRunner.cpp CPUTimer.h tcp.h mynew.h 
	g++ -ansi -Wall -g -c tcpRunner.cpp

clean : 
	rm -f tcp.out mynew.o  tcp.o  tcpRunner.o   
