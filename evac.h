// Author: Sean Davis
#ifndef evacH
#define evacH

#include "EvacRunner.h"
#define MIN(a,b) (a)<(b)?(a):(b)

class Queue
{
public:
  int size;
  int inQueue;
  int *IDs;
  int front;
  int end;

  Queue(int siz) 
  { 
    size = siz; 
    front = end = inQueue = 0;
    IDs = new int[size];
  }

  void add(int id) 
  { 
    IDs[end] = id;
    end++;
    inQueue++;
  }

  int see() // returns -2 if empty 
  {
    if (inQueue)
      return IDs[front];
    return -2;
  }

  int remove()  // returns -2 if empty 
  { 
    if (inQueue)
    {
      inQueue--;
      front++;
      return IDs[front - 1]; 
      // hopefully this works right; if not change to front - 1 
    }
    return -2;


  }

  ~Queue() 
  { 
    delete IDs;
  }
}; // class Queue





class Road4: public Road2
{
public:
  /*
  int destinationCityID;
  int sourceCityID;
  int peoplePerHour;
  int peopleThisHour;
  */
  int ID;
  int oppositeID; // the ID of the road going in the opposite direction
  bool evenID;
  int depth;
}; // class Road4





// I really wanted to name this class SimCity but I gotta keep it real I guess
class City4
{
public:
  int ID;
  int population;
  int evacuees;
  Road4 *roads; 
  int roadCount;
  int depth;
  int closestEvacCityID;
}; // class City4





class Evac
{
public:
  int numCities;
  int numRoads;
  City4 *cities;
  int *roadFlowUsed;
  Road4 **roads; // array of pointers so nothing has be stored twice
  int *cityIDByDepth; 
  int *roadIDByDepth;
  // for both of those arrays, pos 0 has max depth, pos n has min depth
  int roadIDDepthPos;
  int cityIDDepthPos;
  int deepestNotFullCityPos;
  int deepestNotUsedRoadPos;
  //EvacRoute
  int timer;


  Evac(City *simCities, int numSimCities, int numSimRoads);
  void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount); // student sets evacRoutes and routeCount
  void bfs(int * evacIDs, int numEvacs, Queue * b);
  int pushOutDFS(int evacueesIncoming, int currentCityID, int timer, 
    EvacRoute *evacRoutes, int &routeCount, int recursionLimit);

}; // class Evac





#endif
