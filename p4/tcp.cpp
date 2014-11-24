
#include "mynew.h"
#include "tcp.h"
#include "QuadraticProbing.h"
#include "MyQuadraticProbing.h"
#include "BinaryHeap.h"
#include<cstring>

Packet::Packet()
{

}

Packet::Packet(int n, const char ** packet)
{
	num = n;
//	msg = new const char*(*packet); // deep copy
	msg = new char*[PACKET_SIZE];
	(*msg) = new char[PACKET_SIZE];
	std::strcpy(*msg, *packet);
//	std::cout << "Packet(): msg address is " << msg << " packet address is " << packet << std::endl;
//	msg = packet;
}

const Packet& Packet::operator=(const Packet& rhs)
{
	if (this != &rhs)
	{
		num = rhs.num;
		msg = new char*[PACKET_SIZE];
		(*msg) = new char[PACKET_SIZE];
		std::strcpy(*msg, *(rhs.msg));
//		msg = new const char*[PACKET_SIZE];
		
//		std::strcpy((char * ) *msg, *(rhs.msg)); // segfaults
//		msg = new const char*(*rhs.msg); // deep copy
//		msg = new const char*[PACKET_SIZE];
//		for (int i = 0; i < PACKET_SIZE; i++) {
//			(*msg)[i] = (*(rhs.msg))[i];
//		}
		msg = rhs.msg; // NOTE: ptrs may cause problems
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

	table = new QuadraticHashTable<int>(-1, numPort * 2); 
 	mytable = new MyQuadraticHashTable<Port>(notfound, numPort*2); 

	bheaps = new BinaryHeap<Packet>*[numPort];
	bheapSize = 0;
	sizes = new int [numPort];

	for (int i = 0; i < numPort; i++) 
	{
		sizes[i] = 0;
	}

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
//	std::cout << "receive(): portNum [" << portNum << "] packetNum [" << packetNum << "]" << std::endl;
//	std::cout << "receive(): portNum [" << portNum << "] packetNum [" << packetNum << "] packet:" << std::endl << packet << std::endl << std::endl;

/*	Hash debug
 *
 *	int tmp = table->find(portNum);
	if (tmp > -1)
	{
		// do nothing with hash
	}
	else 
	{
		table->insert(portNum);
	} */

	Port newPort;
	newPort.key = portNum;
	newPort.value = bheapSize;

	Port tmpPort = mytable->find(newPort);

	Packet p(packetNum, &packet);
	
	if (tmpPort.key != -1) 
	{
		// do nothing with hash
//		std::cout << "Port is " << portNum << std::endl;
		if (packetNum > currentMins[tmpPort.value])
		{
			bheaps[tmpPort.value]->insert(p);
			sizes[tmpPort.value]++;
		}
	}
	else
	{
//		std::cout << "tmpPort.key is " << tmpPort.key << " bheapSize is " << bheapSize << std::endl;
//		std::cout << "\tinserted to " << bheapSize << std::endl;

		mytable->insert(newPort);
		
		Packet debug;


		bheaps[bheapSize] = new BinaryHeap<Packet>(1000);
		bheaps[bheapSize]->insert(p);
//		if (bheapSize > 0 )
//		{
//		debug = bheaps[bheapSize - 1]->findMin();	
//		std::cout << "receieve(): previous packet at " << bheapSize - 1 << " inserted was:" << std::endl << debug.returnMsg() << std::endl; 
//		}
		sizes[bheapSize]++;
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
//		std::cout << "getStream(): found portNum " << index.key << std::endl;
		int packetCount = 0;
//		while (!bheaps[index.value]->isEmpty())
		while (sizes[index.value] > 0)
		{
//			(bheaps[index.value]->findMin()).printMsg();
			Packet foundPacket = bheaps[index.value]->findMin();
			bheaps[index.value]->deleteMin();	

			sizes[index.value]--;
			currentMins[index.value] = foundPacket.getNum();
//			std::cout << "getStream(): found packetNum " << foundPacket.getNum() << " at bheap index " << index.value << " msg index is " << foundPacket.Msg() << std::endl;
//			std::cout << "getStream(): found packetNum " << foundPacket.getNum() << " at bheap index " << index.value << " msg index is " << foundPacket.Msg() << std::endl
//						<< "packet:" << std::endl << foundPacket.returnMsg() << std::endl << std::endl;
//			std::memcpy(stream, foundPacket.Msg(), PACKET_SIZE);
			std::memcpy(&stream[packetCount * PACKET_SIZE], foundPacket.returnMsg(), PACKET_SIZE); // WORKS!!!!

//			std::strcat(stream, foundPacket.returnMsg());

			packetCount++;
//			foundPacket.printMsg();
//		strcat(stream, foundPacket.returnMsg());
		}
		return packetCount * PACKET_SIZE;		
	} 
   
   return 0;
}
