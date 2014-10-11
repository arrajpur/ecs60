timetest : timetest.o
	g++ -Wall timetest.o -o timetest
timetest.o :  CPUTimer.h LinkedList.h StackAr.h dsexceptions.h CursorList.h QueueAr.h SkipList.h StackLi.h vector.h
	g++ -Wall -c timetest.cpp

clean:
	rm timetest.o timetest
