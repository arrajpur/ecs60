// Authors: Put your names here NOW!
// use unsigned short for 2 bytes vs 4 bytes for port nums
// 32000 for signed short
#ifndef TCP_H
#define	TCP_H

#include "mynew.h"
#include "QuadraticProbing.h"
//#include "SeparateChaining.h"
#include "BinaryHeap.h"
#define PACKET_SIZE 256

class Port
{
	public:
	int key; // hash on
	int value; // return
	bool operator !=(const Port& rhs) const;
	bool operator<(const Port& rhs) const;
};

class Packet
{
public:
	Packet();
	~Packet();
	Packet(int n, const char ** packet);
	const Packet& operator=(const Packet& rhs);
	bool operator <(const Packet& rhs) const;
	void printMsg(void) const;
	char * returnMsg(void);
	char ** Msg (void);
	int getNum(void);
private:
	int num; // packetNum: heap on this
	char ** msg; 
};

class TCP 
{
 
public:
  TCP(int numPorts);
  virtual ~TCP();
  void receive(int portNum, int packetNum, const char packet[257]);
  int getStream(int portNum, char stream[100000]);

private:
//	slightly modified: pass x.key instead of x to hash(). Implemented operator != for Port.
	QuadraticHashTable<Port> * mytable;

	BinaryHeap<Packet> ** bheaps;
	int bheapSize;
	int * currentMins;
};

#endif	/* TCP_H */
