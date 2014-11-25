// Authors: Put your names here NOW!
// use unsigned short for 2 bytes vs 4 bytes for port nums
// 32000 for signed short
#ifndef TCP_H
#define	TCP_H

#include "mynew.h"
#include "QuadraticProbing.h"
#include "BinaryHeap.h"
//s #include "SplayTree.h"
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
//	const Packet& operator=(const Packet& rhs);
	bool operator <(const Packet& rhs) const;
	bool operator !=(const Packet& rhs) const;
	char * returnMsg(void);
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
	QuadraticHashTable<Port> * mytable;

/*b*/	BinaryHeap<Packet> ** bheaps;
/*b*/	int bheapSize;
	short * currentMins;
	
//s	SplayTree<Packet> ** splays;
//s	int splaySize;
//s	Packet notfound;
};

#endif	/* TCP_H */
