
#include "mynew.h"
#include "tcp.h"
#include "QuadraticProbing.h"
#include "BinaryHeap.h"
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

}

Packet::~Packet()
{
//	delete [] msg;	
	//delete [] msg;
	
//	delete *msg;
//	delete msg;
}

Packet::Packet(int n, const char ** packet)
{
	// can't change allocation here
	num = n;
	msg = new char*[PACKET_SIZE];
	(*msg) = new char[PACKET_SIZE];
	std::strcpy(*msg, *packet);
}

const Packet& Packet::operator=(const Packet& rhs)
{
	if (this != &rhs)
	{
		num = rhs.num;
		msg = rhs.msg; // somehow does not cause error
	}
	return *this;
}

bool Packet::operator <(const Packet& rhs) const
{
	if (num < rhs.num)
		return true;
	
	return false;
}

void Packet::printMsg(void) const
{
	for (int i = 0; i < 256; i++)
	{
		std::cout << (*msg)[i] ;
	}
	std::cout << std::endl;
}

char * Packet::returnMsg(void)
{
	return (char *) *msg;
}

char ** Packet::Msg(void)
{
	return (char**) msg;
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
//	mytable = new QuadraticHashTable<Port>(notfound, numPort); // uses more mem

	bheaps = new BinaryHeap<Packet>*[numPort];
	bheapSize = 0;

	currentMins = new int [numPort];

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
	newPort.value = bheapSize;

	Port tmpPort = mytable->find(newPort);

	Packet p(packetNum, &packet);
	
	if (tmpPort.key != -1) 
	{
		if (packetNum > currentMins[tmpPort.value])
		{
			bheaps[tmpPort.value]->insert(p);
		}
	}
	else
	{
		mytable->insert(newPort);
		
		bheaps[bheapSize] = new BinaryHeap<Packet>(1000);
		bheaps[bheapSize]->insert(p);

		bheapSize++;

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
	findMe.value = -1;
  
	Port index = mytable->find(findMe);

 	if (index.value > -1)
	{
		int packetCount = 0;
		while (!bheaps[index.value]->isEmpty())
		{
			// original method
			Packet foundPacket = bheaps[index.value]->findMin();
			bheaps[index.value]->deleteMin();	

//			Packet foundPacket; // works
//			bheaps[index.value]->deleteMin(foundPacket); // works
			currentMins[index.value] = foundPacket.getNum();
			std::memcpy(&stream[packetCount * PACKET_SIZE], foundPacket.returnMsg(), PACKET_SIZE); // WORKS!!!!


			packetCount++;
		}
		return packetCount * PACKET_SIZE;		
	} 
   
   return 0;
}
