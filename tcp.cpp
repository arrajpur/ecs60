#include <iostream> // remove this later
#include "mynew.h"
#include "tcp.h"
#include "SplayTree.h"
//#include "Port.h"
//#include "Packet.h"


/*********************************************************************************************************************
 *
 *
 *              Packet functions
 *
 *
 *********************************************************************************************************************/




Packet::Packet()
{
  packetNum = -1;
  packet = NULL;
}



Packet::Packet(int n, const char p[PACKET_SIZE + 1])
{
  packetNum = n;
  if (p)
  {
    packet = new char[PACKET_SIZE + 1];
    strcpy(packet, p);
  }
}


/*
Packet::~Packet()
{
  if (packet)
    delete packet;
}
*/



bool Packet::operator<(const Packet & comp) const
{
  return packetNum < comp.getPacketNum();
}



bool Packet::operator==(const Packet & comp) const
{
  return packetNum == comp.getPacketNum();
}



int Packet::getPacketNum() const
{
  return packetNum;
}



char * Packet::getPacket() const
{
  return packet;
}




/*********************************************************************************************************************
 *
 *
 *              Port functions
 *
 *
 *********************************************************************************************************************/


Port::Port()
{
  portNum = -1;
  lastPacketNum = -1;
  not_found = Packet(-1, NULL);
  packets = NULL;
}



Port::Port(int pN, bool needed)
{
  portNum = pN;
  lastPacketNum = -1; //since they can probably be 0
  not_found = Packet(-1, NULL);

  if (needed)
    packets = new SplayTree<Packet>(not_found);
  else
    packets = NULL;
}


/*
Port::~Port()
{
  if (packets)
  {
    packets->makeEmpty();
    delete packets;
  }
}
*/


bool Port::operator<(const Port & comp) const
{
  return portNum < comp.getPortNum();
}



bool Port::operator==(const Port & comp) const
{
  return portNum == comp.getPortNum();
}



int Port::getPortNum(void) const
{
  return portNum;
}



int Port::getLastPacketNum(void) const
{
  return lastPacketNum;
}



Packet Port::getLowestPacket(void)
{
  return packets->findMin();
}



void Port::deleteLowestPacket(void)
{
  packets->remove(packets->findMin());
}



void Port::setLastPacketNum(int lPN)
{
  lastPacketNum = lPN;
}



SplayTree<Packet> * Port::getPackets(void)
{
  return packets;
}




bool Port::empty(void)
{
  return packets->isEmpty();
}



void Port::insert(int packetNum, const char * packet)
{
  if (packetNum <= lastPacketNum)
    return;

  //food need 2 make dinner 11/23/14 748PM

  Packet newPacket = *(new Packet(packetNum, packet));
  packets->insert(newPacket);
  /*cout << portNum << ":" << endl;
  packets->printTree();
  cout << endl << endl;*/
}




/*********************************************************************************************************************
 *
 *
 *              TCP functions
 *
 *
 *********************************************************************************************************************/



TCP::TCP(int numPorts)
{
  not_found = Port(-1, false);
  ports = new SplayTree<Port>(not_found);
}  // TCP()





TCP::~TCP() 
{
  /*
  ports->makeEmpty();
  delete ports;
  */
}





void TCP::receive(int portNum, int packetNum, const char packet[PACKET_SIZE + 1])
{
  Port findPort(portNum, false), newPort;
  Port searchResult = ports->find(findPort);

  if (searchResult.getPortNum() == -1)
  {
    newPort = *(new Port(portNum, true)); // watch out here
    ports->insert(newPort);
  }
  else
    newPort = searchResult;

  newPort.insert(packetNum, packet);
} // receive()





int TCP::getStream(int portNum, char stream[100000])
{
  int numPackets = 0;
  Port findPort(portNum, false),
    streamPort = ports->find(findPort);
  //streamPort.setLastPacketNum();
  //SplayTree<Packet> * packets = streamPort.getPackets();
  //cout << portNum << ": " << endl;
  while (!(streamPort.empty()))
  {
    Packet curr = streamPort.getLowestPacket();
    //cout << !streamPort.empty() << endl;
    memcpy(stream + (PACKET_SIZE * numPackets), 
      curr.getPacket(), 257);
    //packets->remove(packets->findMin());*/
    //strcat(stream, streamPort.getLowestPacket());
    streamPort.setLastPacketNum(curr.getPacketNum());
    numPackets++; // something with this isn't right
    streamPort.deleteLowestPacket();
  }

  streamPort.getPackets()->makeEmpty();
  //cout << endl;
  // getStream() should fill the stream with current packets that have
  // packet numbers greater than the last packet past in the last 
  // stream for this port.  The packets must be ascending order, though
  // not necessarily consecutive.  Return the size of the stream in bytes.
  
   return numPackets * PACKET_SIZE;
}