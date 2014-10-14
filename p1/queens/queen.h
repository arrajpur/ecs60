#ifndef QUEEN_H
#define QUEEN_H

#include"StackAr.h"
// Remove this class and make ReturnDangerList a seperate subroutine
class Queen 
{
	public:
	Queen();
	Queen(int c);
	~Queen();
	// These functions check if an INPUT queen is in danger from the current queen
	// returns diagonal positions only
	// vector will usually be size 2, sometimes 1
	StackAr<int> ReturnDangerList(int myrow, int queryrow);	
	bool InDanger(int row, int c, int myrow);
	int GetCol (void) const;
	void SetCol(int c); 
	private:
	int col;

};

class Possibility
{
	public:
	Possibility();
	Possibility(int i);
	Possibility(int i, StackAr<int> stack);

	int GetIndex (void) const;
	int GetNext (void); // pops, then tops;
//	bool operator!=(const Possibility &other) const;
	void Pop (void); // unused
	int Top (void); // unused
	bool amIEmpty(void) const; // I think this is unused? Debug function.
	private:
	int index;
	StackAr<int> safe; // stack of safe positions! easy to manipulate
};
#include "queen.cpp"
#endif
