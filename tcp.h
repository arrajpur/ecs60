// Authors: Aaron Lewis Aparna Rajpurkar
 
#ifndef TCP_H
#define	TCP_H

#include <iostream> // remove this later
#include "mynew.h"
#include "SplayTree.h"
#include <string.h>
//#include "Port.h"
//#include "Packet.h"
#define PACKET_SIZE 256





class Packet
{
  int packetNum;
  char * packet;

public:
  Packet();
  Packet(int n, const char p[PACKET_SIZE]);
  //~Packet();
  bool operator<(const Packet & comp) const;
  bool operator==(const Packet & comp) const;
  int getPacketNum() const;
  char * getPacket() const;
}; // PACKET





class Port
{
  int portNum;
  int lastPacketNum;
  SplayTree<Packet> * packets;
  Packet not_found;

public:
  Port();
  Port(int pN, bool needed);
  //~Port();
  bool operator<(const Port & comp) const;
  bool operator==(const Port & comp) const;
  int getPortNum(void) const;
  int getLastPacketNum(void) const;
  void setLastPacketNum(int lPN);
  Packet getLowestPacket(void);
  void deleteLowestPacket(void);
  SplayTree<Packet> * getPackets(void);
  bool empty(void);
  void insert(int packetNum, const char * packet);
}; // PORT





class TCP 
{
  SplayTree<Port> * ports;
  Port not_found;
 
public:
  TCP(int numPorts);
  virtual ~TCP();
  void receive(int portNum, int packetNum, const char packet[257]);
  int getStream(int portNum, char stream[100000]);
}; // TCP

#endif	/* TCP_H */

