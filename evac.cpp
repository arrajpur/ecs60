#include <cstdlib>
#include "evac.h"
#include "EvacRunner.h"

using namespace std;

Evac::Evac(City *simCities, int numSimCities, int numSimRoads)
{
  numCities = numSimCities;
  numRoads = numSimRoads + numSimRoads; 
  // there are 100000 bidirectional roads, 200000 IDs

  cities = new City4[numCities];
  roads = new Road4*[numRoads];
  roadFlowUsed = new int[numRoads];
  roadIDByDepth = new int[numRoads];
  cityIDByDepth = new int[numCities];
  roadIDDepthPos = numRoads - 1;
  cityIDDepthPos = numCities - 1;

  for (int i  = 0; i < numSimCities; i++)
  {
    cities[i].ID = simCities[i].ID;
    cities[i].population = simCities[i].population;
    cities[i].roadCount = simCities[i].roadCount;
    cities[i].depth = -1; // needed in bfs

    cities[i].roads = new Road4[cities[i].roadCount];
    for (int j = 0; j < cities[i].roadCount; j++)
    {
      cities[i].roads[j].ID = simCities[i].roads[j].ID;
      cities[i].roads[j].destinationCityID = simCities[i].roads[j].destinationCityID;
      cities[i].roads[j].sourceCityID = simCities[i].ID;
      cities[i].roads[j].peoplePerHour = simCities[i].roads[j].peoplePerHour;
      /*if (i < 25)
        cout << simCities[i].roads[j].peoplePerHour << endl;*/
      if (cities[i].roads[j].ID / 2 == (cities[i].roads[j].ID + 1) / 2)
      {
        cities[i].roads[j].evenID = true;
        cities[i].roads[j].oppositeID = cities[i].roads[j].ID + 1;
      }
      else // the road has an odd ID
      {
        cities[i].roads[j].evenID = false;
        cities[i].roads[j].oppositeID = cities[i].roads[j].ID - 1;
      }
      cities[i].roads[j].depth = -1; // needed in bfs

      if (!roads[cities[i].roads[j].ID])
        roads[cities[i].roads[j].ID] = &cities[i].roads[j];
    }
  }
} // Evac()



void Evac::evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount)
{
  for (int i = 0; i < cities[evacIDs[0]].roadCount; i++)
    cout << cities[evacIDs[0]].roads[i].peoplePerHour << endl;
  bool moreToEvacuate = true, currentWhileDoesNotCompleteEvac = false;
  routeCount = 0;
  timer = 1;
  Queue qCities(numCities);
  bfs(evacIDs, numEvacs, &qCities);

  //cout << "1" << endl;
  while (moreToEvacuate)
  {
    //cout << "2" << endl;
    for (int i = 0; i < numEvacs; i++)
    {
      //cout << "3" << endl;
      if (-cities[evacIDs[i]].evacuees < cities[evacIDs[i]].population)
      {
        //cout << "4" << endl;
        for (int j = 0; j < cities[evacIDs[i]].roadCount; j++)
        {
          //cout << "5" << endl;
          //if (cities[cities[evacIDs[i]].roads[j].destinationCityID].depth)
          int numEvacueesSending = cities[evacIDs[i]].roads[j].peoplePerHour;
          /*if (routeCount < 25)
            cout << "to send: " << numEvacueesSending << endl;*/
          int sentSuccessfully = pushOutDFS(numEvacueesSending, 
            cities[evacIDs[i]].roads[j].destinationCityID, timer,
            evacRoutes, routeCount, 5);
          cities[evacIDs[i]].evacuees -= sentSuccessfully;
          if (routeCount < 25)
            cout << "Sent : " << sentSuccessfully << endl;

          roadFlowUsed[cities[evacIDs[i]].roads[j].ID] = sentSuccessfully;
          roadFlowUsed[cities[evacIDs[i]].roads[j].oppositeID] = 
            cities[evacIDs[i]].roads[j].peoplePerHour;

          /*if (routeCount <= 25 || routeCount >= numCities * 1000)
            cout << routeCount 
              << " sent successfully: " << sentSuccessfully
              << " time: " << timer << endl;*/
          evacRoutes[routeCount].roadID = cities[evacIDs[i]].roads[j].ID;
          evacRoutes[routeCount].time = timer;
          evacRoutes[routeCount].numPeople = sentSuccessfully;
          routeCount++;
          //cout << "6" << endl;
          if (cities[cities[evacIDs[i]].roads[j].destinationCityID].depth == 0
            && -cities[cities[evacIDs[i]].roads[j].destinationCityID].evacuees <
                cities[cities[evacIDs[i]].roads[j].destinationCityID].population)
          {
            moreToEvacuate = true;
            currentWhileDoesNotCompleteEvac = true;
          }
          //cout << "7" << endl;
          //you want a tight == bound here
          /*if (-cities[evacIDs[i]].evacuees < cities[evacIDs[i]].population)
          {
            moreToEvacuate = true;
            currentWhileDoesNotCompleteEvac = true;
          }
          else if (!currentWhileDoesNotCompleteEvac)
            moreToEvacuate = false;*/
          if (-cities[evacIDs[i]].evacuees >= cities[evacIDs[i]].population)
            break;
          //cout << "8" << endl;
        }
      }
      //cout << "9" << endl;
      // reset roadFlowUsed
      if (-cities[evacIDs[i]].evacuees < cities[evacIDs[i]].population)
      {
        moreToEvacuate = true;
        currentWhileDoesNotCompleteEvac = true;
      }
      else if (!currentWhileDoesNotCompleteEvac)
        moreToEvacuate = false;
      //cout << "9" << endl;
    }
    //cout << "10 - .01" << endl;
    delete roadFlowUsed;
    roadFlowUsed = new int[numRoads];
    timer++;
    //cout << "10" << endl;
  }
  //cout << ":)" << endl;
///////////////////////////////////////////////////////////////////////////////////
  // reset evacuees of evacCities to 0
///////////////////////////////////////////////////////////////////////////////////
} // evacuate




void Evac::bfs(int * evacIDs, int numEvacs, Queue * q)
{
  int roadIDDepthPos = numRoads - 1;
  int cityIDDepthPos = numCities - 1;

  for (int i = 0; i < numEvacs; i++)
  {
    for (int j = 0; j < cities[evacIDs[i]].roadCount; j++)
    {
      for (int k = 0; k < numEvacs; k++)
      {
        if (cities[evacIDs[i]].roads[j].depth == -1 
          && cities[evacIDs[i]].roads[j].destinationCityID == evacIDs[k])
        {
          /*cities[evacIDs[i]].roads[j].depth = 0;
          cityIDByDepth[cityIDDepthPos--] = evacIDs[i];*/
          if (cities[evacIDs[i]].roads[j].evenID)
          {
            roads[cities[evacIDs[i]].roads[j].ID + 1]->depth = 0;
            roadIDByDepth[roadIDDepthPos--] = cities[evacIDs[i]].roads[j].ID + 1;
          }
          else // odd ID
          {
            roads[cities[evacIDs[i]].roads[j].ID - 1]->depth = 0;
            roadIDByDepth[roadIDDepthPos--] = cities[evacIDs[i]].roads[j].ID - 1;
          }
          break;
        }
      }
    }
    cities[evacIDs[i]].depth = 0;
    cityIDByDepth[cityIDDepthPos--] = cities[evacIDs[i]].ID;
  }

  for (int i = 0; i < numEvacs; i++)
  {
    for (int j = 0; j < cities[evacIDs[i]].roadCount; j++)
    {
      if (cities[cities[evacIDs[i]].roads[j].destinationCityID].depth == -1)
      {
        cities[cities[evacIDs[i]].roads[j].destinationCityID].depth = 1;
        cityIDByDepth[cityIDDepthPos--] 
          = cities[evacIDs[i]].roads[j].destinationCityID;
        cities[evacIDs[i]].roads[j].depth = 1;
        roadIDByDepth[roadIDDepthPos--] = cities[evacIDs[i]].roads[j].ID;
        // max out back roads?
        q->add(cities[evacIDs[i]].roads[j].destinationCityID); // can change to a roadID queue
        /*cout << "thing added: " << cities[evacIDs[i]].roads[j].destinationCityID
          << " in queue: " << q->inQueue << endl;
        cout << "thing: " << q->IDs[q->end - 1] << endl;*/
      }
      if (cities[evacIDs[i]].roads[j].depth == -1)
      {
        cities[evacIDs[i]].roads[j].depth = 1;
        roadIDByDepth[roadIDDepthPos--] = cities[evacIDs[i]].roads[j].ID; // ?
      }
    }
  }

  while (q->front != q->end) // while queue not empty
  {
    //cout << "while1" << endl;
    //cout << "queue front: " << q->front << endl;
    //cout << "queue first item: " << q->IDs[1] << endl;
    int currentCityID = q->remove();
    //cout << "while2 currentCityID: " << currentCityID << endl;
    
    for (int i = 0; i < cities[currentCityID].roadCount; i++)
    {
      //cout << "for i " << i << endl;
      if (cities[cities[currentCityID].roads[i].destinationCityID].depth == -1)
      {
        //cout << "i1" << endl;
        cities[cities[currentCityID].roads[i].destinationCityID].depth =
          cities[currentCityID].depth + 1;
        cities[cities[currentCityID].roads[i].destinationCityID].closestEvacCityID =
          cities[currentCityID].closestEvacCityID;
        //cout << "i2" << endl;
        cityIDByDepth[cityIDDepthPos--] = 
          cities[currentCityID].roads[i].destinationCityID;
        //cout << "i3" << endl;
        cities[currentCityID].roads[i].depth = cities[currentCityID].depth + 1;
        roads[cities[currentCityID].roads[i].oppositeID]->depth = -2;
        //cout << "i4" << endl;
        roadIDByDepth[roadIDDepthPos--] = cities[currentCityID].roads[i].ID;
        //cout << "i5" << endl;
        q->add(cities[currentCityID].roads[i].destinationCityID);
        //cout << "in queue: " << q->inQueue << endl;
        //cout << "i6" << endl;
      }
        //cout << "i7" << endl;
      else if (cities[currentCityID].roads[i].depth == -1)
      {
        //cout << "i8" << endl;
        cities[currentCityID].roads[i].depth = cities[currentCityID].depth + 1;
        //cout << "i9" << endl;
        roadIDByDepth[roadIDDepthPos--] = cities[currentCityID].roads[i].ID;
        //cout << "i10" << endl;
      }
      //cout << "i1i end" << endl;
    }
    //cout << "while end" << endl;
  }

  /*if (cityIDDepthPos)
    cout << "cityIDDepthPos: " << cityIDDepthPos << endl;
  else
    cout << "zero1" << endl;
  if (roadIDDepthPos)
    cout << "roadIDDepthPos: " << roadIDDepthPos << endl;
  else
    cout << "zero2" << endl;*/

  /*cout << cities[cityIDByDepth[0]].depth << endl;
  cout << cities[cities[cityIDByDepth[0]].roads[0].destinationCityID].depth << endl;
  for (int i = 0; i < cities[cityIDByDepth[0]].roadCount; i++)
    cout << cities[cities[cityIDByDepth[0]].roads[i].destinationCityID].depth << " ";
  cout << endl << roads[roadIDByDepth[roadIDDepthPos + 1]]->depth << endl;*/
}




int Evac::pushOutDFS(int evacueesIncoming, int currCityID, int timer, 
  EvacRoute *evacRoutes, int &routeCount, int recursionLimit)
{
  int evacueesKeeping, evacueesToSend, evacsSentSuccessThisTime,
    totalEvacueesSentSuccessfully = 0;
  //recursion stopper would go here
  if (cities[currCityID].evacuees + evacueesIncoming 
    <= cities[currCityID].population)
  {
    cities[currCityID].evacuees += evacueesIncoming;
    return evacueesIncoming;
  }

  if (!recursionLimit)
    return 0;

  evacueesKeeping = cities[currCityID].population - cities[currCityID].evacuees;
  cities[currCityID].evacuees += evacueesKeeping;
  evacueesToSend = evacueesIncoming - evacueesKeeping;



  for (int i = 0; i < cities[currCityID].roadCount; i++)
  {
    if (cities[currCityID].roads[i].peoplePerHour 
      - roadFlowUsed[cities[currCityID].roads[i].ID] == 0)
      continue;

    // maybe add something about depth of next city
    if (evacueesToSend + roadFlowUsed[cities[currCityID].roads[i].ID] 
      <= cities[currCityID].roads[i].peoplePerHour)
    {
      evacsSentSuccessThisTime = pushOutDFS(evacueesToSend, 
        cities[currCityID].roads[i].destinationCityID, timer, 
        evacRoutes, routeCount, recursionLimit - 1);
    }
    else // leftover roadflow < evacueesToSend
    {
      evacsSentSuccessThisTime = pushOutDFS(
        cities[currCityID].roads[i].peoplePerHour 
          - roadFlowUsed[cities[currCityID].roads[i].ID], 
        cities[currCityID].roads[i].destinationCityID, timer, 
        evacRoutes, routeCount, recursionLimit - 1);
    }
    if (routeCount < 25)
      cout << "Success! " << evacsSentSuccessThisTime << endl;

    if (routeCount > numCities * 1000)
      cout << routeCount << " Shit" << endl;
    evacRoutes[routeCount].roadID = cities[currCityID].roads[i].ID;
    evacRoutes[routeCount].time = timer;
    evacRoutes[routeCount].numPeople = evacsSentSuccessThisTime;
    routeCount++;

    roadFlowUsed[cities[currCityID].roads[i].ID] += evacsSentSuccessThisTime;
    //if (cities[currCityID].roads[i].depth != 0) dont: can loop
    roadFlowUsed[cities[currCityID].roads[i].oppositeID]
      = cities[currCityID].roads[i].peoplePerHour;

    totalEvacueesSentSuccessfully += evacsSentSuccessThisTime;
    evacueesToSend -= evacsSentSuccessThisTime;

    if (!evacueesToSend)
      break;
  }

  return evacueesKeeping + totalEvacueesSentSuccessfully;
}
