// Authors: Put your names here NOW!
 
#ifndef TCP_H
#define	TCP_H

#include "mynew.h"
#include "QuadraticProbing.h"
#include "MyQuadraticProbing.h"
#include "BinaryHeap.h"
#define PACKET_SIZE 256

class Port
{
	public:
	int key; // hash on
	int value; // return
};

class Packet
{
public:
	Packet();
	Packet(int n, const char ** packet);
	const Packet& operator=(const Packet& rhs);
	bool operator <(const Packet& rhs) const;
	void printMsg(void) const;
	char * returnMsg(void);
	char ** Msg (void);
	int getNum(void);
private:
	int num; // packetNum: heap on this
	char ** msg; //
};

class TCP 
{
 
public:
  TCP(int numPorts);
  virtual ~TCP();
  void receive(int portNum, int packetNum, const char packet[257]);
  int getStream(int portNum, char stream[100000]);

private:
	QuadraticHashTable<int> * table;
	MyQuadraticHashTable<Port> * mytable;
	BinaryHeap<Packet> ** bheaps;
	int bheapSize;
	int * sizes;
	int * currentMins;
};

#endif	/* TCP_H */
