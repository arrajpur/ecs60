// Aparna Rajpurkar Aaron Lewis
#ifndef evacH
#define evacH

#include "EvacRunner.h"

class MyRoad
{
public:
	int ID;
	int toID; // city
	int fromID; // city
	int capacity;
	int peoplePerHour;
	int depth;
	
	MyRoad();
	MyRoad(const Road orig, int from);

	inline bool operator< (const MyRoad& rhs);
	inline bool operator> (const MyRoad& rhs);
};

class MyCity
{
public:
	int ID;
	int population;
	int evacuees;
	MyRoad * roads;
	int roadCount;
	bool isEvac;

	MyCity();
	MyCity(const City * orig);
};

class Evac
{
public:
  Evac(City *cities, int numCities, int numRoads);
  void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount); // student sets evacRoutes and routeCount

	void BFS(int * evacIDs); // sets min_index

	void quicksort(MyRoad * array, int left, int right);

	int DFS(int flow, MyRoad road, int recurse);

	int min2(int num1, int num2);

private:
	MyCity * allCities;
	MyCity * evacCities;

	int * roadCapacities; // index by roadID

	int * roadPeople; // peoplePerHour

	int numRoad;
	int totalEvacuees;
	int numCities;	
	int numEvacCities;
	int time;
}; // class Evac



#endif
