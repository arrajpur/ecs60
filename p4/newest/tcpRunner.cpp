#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "CPUTimer.h"
#include "tcp.h"
#include "mynew.h"
using namespace std;
extern int maxRAM;
extern int currentRAM;
#define PACKET_SIZE 256

typedef struct
{
  int time;
  int portNum;
  int *packetNums;
  int packetCount;
  char messageChar;
  int messageCharPos;
} Message;

int readFile(const char *filename, int *numPorts, int portNums[1000], char data[1000], 
  int segmentCounts[1000], int *segmentPos[1000], Message *messages)
{
  ifstream inf(filename);
  int maxSegments, messageCount = 0, packetNums[1000], packetCount;
  char line[1000], *ptr, *stream;
  
  stream = new char[1000000];
  srand(1);
  inf >> *numPorts >> maxSegments;
  
  for(int i = 0; i < *numPorts; i++)
  {
    inf >> portNums[i] >> segmentCounts[i];
    segmentPos[i] = new int[segmentCounts[i]];
    
    for(int j = 0; j < segmentCounts[i]; j++)
      inf >> segmentPos[i][j];
  }
  
  inf.ignore(100, '\n');  // eat up '\n'
  inf.read(data, 1000);
  inf.ignore(100, '\n');  // eat up '\n'
  
  while(inf >> messages[messageCount].time >> messages[messageCount].portNum)
  {
    packetCount = 0;
    messages[messageCount].packetCount = 0;
    inf.getline(line, 1000);
    ptr = strtok(line, " \n");

    while(ptr)
    {
      packetNums[packetCount++]
        = atoi(ptr);
      ptr = strtok(NULL, " \n");
    }  // while more on line
    
    if(packetCount > 0)
      messages[messageCount].packetNums = new int[packetCount];
      
    for(int i = 0; i < packetCount; i++)
    {
      memcpy(&stream[i * PACKET_SIZE], 
        &data[segmentPos[messages[messageCount].portNum][packetNums[i] - 1]], PACKET_SIZE);
      messages[messageCount].packetNums[i] = packetNums[i];
    } // for each packet

    if(packetCount > 0)
    {
      messages[messageCount].messageCharPos = rand() % (packetCount * PACKET_SIZE); 
      messages[messageCount].messageChar = stream[ messages[messageCount].messageCharPos];
    }
    
      messages[messageCount].packetCount = packetCount;
    messageCount++;
  }  // while more lines in file
  
  delete [] stream; 
  return messageCount;
}   // readFile())
  
 
int main(int argc, char **argv)
{
  int numPorts, portNums[1000], segmentCounts[1000], *segmentPos[1000], 
    messageCount, streamSize;
  Message *messages;
	char data[1000], packet[257], *stream;
  initializeNew();
  messages = new Message[1000000];
  stream = new char[100000];
  
  messageCount = readFile(argv[1], &numPorts, portNums, data, segmentCounts, 
    segmentPos,  messages);
  packet[PACKET_SIZE] = '\0';
  CPUTimer ct;
  maxRAM = currentRAM = 0;
  ct.reset();
  TCP *tcp = new TCP(numPorts);
  
  for(int i = 0; i < messageCount; i++)
  {
    if(messages[i].time >= 0)
    {
      memcpy(packet,  &data[segmentPos[messages[i].portNum][messages[i].packetNums[0] - 1]]
        , PACKET_SIZE);
      tcp->receive(portNums[messages[i].portNum], messages[i].packetNums[0], packet);
    }  // if 
    else
    {
      streamSize = tcp->getStream(portNums[messages[i].portNum], stream);
      if(streamSize != messages[i].packetCount * PACKET_SIZE)
        cout << "Error in stream size for message #"  << i 
          << " port index: " << messages[i].portNum <<  " port: "
          << portNums[messages[i].portNum] << " yours: "
          << streamSize << " expected: " << messages[i].packetCount * PACKET_SIZE << endl;
      else
        if(messages[i].packetCount > 0 
          &&stream[messages[i].messageCharPos] != messages[i].messageChar)
          cout << "Error in stream contents for message #"  << i 
            << " port index: " << messages[i].portNum << " port: " 
            << portNums[messages[i].portNum] << endl;
    } // else a test message
  }

  cout << "CPU Time: " << ct.cur_CPUTime() <<  " RAM: " << maxRAM <<  endl; 
  return 0;
}
 
