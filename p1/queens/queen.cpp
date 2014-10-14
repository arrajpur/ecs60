#include<iostream>
#include"StackAr.h"
#include"queen.h"
using namespace std;

// this class will be removed
Queen::Queen() { }
Queen::~Queen() {}
Queen::Queen(int c) 
{
	col = c;
}

// Save this function
StackAr<int> Queen::ReturnDangerList(int myrow, int queryrow) 
{
	StackAr<int> dangerPos(2);
	int diff = queryrow - myrow; // assumes we move DOWN the board

	if (diff < 0) { // check if diff is -, debug
		cout << "diff is negative!" << endl;
	} // remove this 
	
	if (col - diff > 0)
		dangerPos.push(col - diff);
	if (col + diff < 9)
		dangerPos.push(col + diff);

	return dangerPos; // FIXME return reference!
}

bool Queen::InDanger(int row, int c, int myrow) 
{
	if (c == col) 
		return true;
	
	if (row == myrow)
		return true;

	// possibly wrong syntax
	StackAr<int> diagonal = ReturnDangerList(myrow, row);
	while(!diagonal.isEmpty())
		if (diagonal.topAndPop() == c)	
			return true;

	return false;
}

int Queen::GetCol(void) const 
{
	return col;
}	

void Queen::SetCol(int c) 
{
	col = c;
}

Possibility::Possibility() { }

// Not used
Possibility::Possibility(int i)
{
	index = i;
}

Possibility::Possibility(int i, StackAr<int> stack)
{
	index = i;
	safe = stack;
}

int Possibility::GetIndex (void) const
{
	return index;
}

int Possibility::GetNext (void)
{
	if (safe.isEmpty()) 
		return -1;
	safe.pop();
	if (safe.isEmpty())
		return -1;
	return safe.top();
}

// Not used
void Possibility::Pop (void)
{
	safe.pop();
}

// Not used
int Possibility::Top (void)
{
	return safe.top();
}
// Not used
bool Possibility::amIEmpty(void) const 
{
	if (safe.isEmpty()) {
		return true;
	}
	cout << "Not Empty supposedly! Top value is " << safe.top() << endl;
	return false;
}
