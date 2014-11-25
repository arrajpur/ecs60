
#include "mynew.h"
#include "tcp.h"
#include "QuadraticProbing.h"
#include "BinaryHeap.h"
//s #include "SplayTree.h"
#include<cstring>


bool Port::operator !=(const Port& rhs) const
{
	if (key != rhs.key)
		return true;

	return false;
}

bool Port::operator<(const Port& rhs) const
{
	if (key < rhs.key)
		return true;
	return false;
}

Packet::Packet()
{
	num = 0;
}

Packet::~Packet()
{
}

Packet::Packet(int n, const char ** packet)
{
	// can't change allocation here
	num = n;
	msg = new char * ;	
	(*msg) = new char[PACKET_SIZE];
	std::memcpy(*msg, *packet, PACKET_SIZE);
}
/*
const Packet& Packet::operator=(const Packet& rhs)
{
	if (this != &rhs)
	{
		num = rhs.num;
		msg = rhs.msg; // somehow does not cause error
	}
	return *this;
}
*/
bool Packet::operator !=(const Packet& rhs) const
{
	if (num != rhs.num)
		return true;
	return false;	
}

bool Packet::operator <(const Packet& rhs) const
{
	if (num < rhs.num)
		return true;
	
	return false;
}

char * Packet::returnMsg(void)
{
	return (char *) *msg;
}

int Packet::getNum(void) 
{
	return num;
}

TCP::TCP(int numPort)
{
	Port notfound;
	notfound.key = -1;
	notfound.value = -1;

	mytable = new QuadraticHashTable<Port>(notfound, numPort*2);
/*b*/	bheaps = new BinaryHeap<Packet>*[numPort];
/*b*/	bheapSize = 0;

//s	splays = new SplayTree<Packet>*[numPort];
//s	splaySize = 0;

	currentMins = new short [numPort];
	for (int j = 0; j < numPort; j++) 
	{
		currentMins[j] = 0;
	}
}  // TCP()

TCP::~TCP() {
}


void TCP::receive(int portNum, int packetNum, const char packet[257])
{
	Port newPort;
	newPort.key = portNum;
/*b*/	newPort.value = bheapSize;
//	newPort.value = splaySize;

	Port tmpPort = mytable->find(newPort);

	Packet p(packetNum, &packet);
	
	if (tmpPort.key != -1) 
	{
		if (packetNum > currentMins[tmpPort.value])
		{
/*b*/			bheaps[tmpPort.value]->insert(p);

//s			splays[tmpPort.value]->insert(p);
		}
	}
	else
	{
		mytable->insert(newPort);
		
/*b*/		bheaps[bheapSize] = new BinaryHeap<Packet>(1000);
/*b*/		bheaps[bheapSize]->insert(p);
/*b*/		bheapSize++;

//s		const char * notfoundstr = "\0";
//s		Packet notfound(0, &notfoundstr);
//s		splays[splaySize] = new SplayTree<Packet>(notfound);
//s		splays[splaySize]->insert(p);
//s		splaySize++;
	}
} // receive()

int TCP::getStream(int portNum, char stream[100000])
{
  // getStream() should fill the stream with current packets that have
  // packet numbers greater than the last packet past in the last 
  // stream for this port.  The packets must be ascending order, though
  // not necessarily consecutive.  Return the size of the stream in bytes.
	Port findMe;

	findMe.key = portNum;
	//findMe.value = -1;
  
	Port index = mytable->find(findMe);

 	if (index.value > -1)
	{
		int packetCount = 0;
		Packet foundPacket;
/*b*/		while (!bheaps[index.value]->isEmpty())
//s		while (!splays[index.value]->isEmpty())
		{
			// original method
/*b*/			foundPacket = bheaps[index.value]->findMin();
/*b*/			bheaps[index.value]->deleteMin();	

//s			foundPacket = splays[index.value]->findMin();
//s			splays[index.value]->remove(foundPacket);

			currentMins[index.value] = foundPacket.getNum();
			std::memcpy(&stream[packetCount * PACKET_SIZE], foundPacket.returnMsg(), PACKET_SIZE); // WORKS!!!!

			packetCount++;
		}
		return packetCount * PACKET_SIZE;		
	} 
   
   return 0;
}
