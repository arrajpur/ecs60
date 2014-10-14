#include<iostream>
#include"queen.h"
#include"StackAr.h"
#include"LinkedList.h"
using namespace std;

/* Eight Queens
 * Produces only one solution
 */

StackAr<int> ConstructSafeList (StackAr<int> soln, int lev);

int main()
{
	int level = 2;
	StackAr<Possibility> Options(8); // Should be 7
	StackAr<int> Solution(8);	// pop when it doesn't work

	Solution.push(1); // first Queen goes in 0 position

	while (level < 9 && level > 1) 
	{ // when we hit 9, we're done
// if we have backtracked ... 
		if (!Options.isEmpty() && (Options.top()).GetIndex() == level) 
		{
			Possibility tmp = Options.topAndPop();
			int soln = tmp.GetNext();
// if that Possibility is exhausted of solutions, go one level up
			while (soln == -1 && !Options.isEmpty()) 
			{
				tmp = Options.topAndPop();
				Solution.pop();
				soln = tmp.GetNext();

				level--;	
			}
			Options.push(tmp);

			Solution.push(soln);
			level++;
		}
		else // we have not backtracked 
		{
			StackAr<int> currSafe = ConstructSafeList(Solution, level);
			Possibility curr(level, currSafe);
			if (currSafe.isEmpty()) 
			{
				level--; // backtrack
				Solution.pop();
			}	
			else
			{
				Solution.push(currSafe.top());
				Options.push(curr);
				level++;
			}

		}
	}

// Prints out in reverse because stack
	cout << Solution.topAndPop();
	while (!Solution.isEmpty()) {
		cout << "," << Solution.topAndPop();
	}
	cout << endl;
}

// Horrible subroutine that somehow works and somehow doesn't use up too much time.
// There is definitely a better way of doing this
// At the very least, the stupid LinkedList bit needs to be removed!
// Also, remove the Queens class and just have a subroutine, as it's not used anymore
StackAr<int> ConstructSafeList (StackAr<int> soln, int lev)
{
	StackAr<int> current = soln; 
	StackAr<int> safe(7);
	
	int stackSize = lev - 1; // just to be clear, and it's LIFO

	List<int> pos;
	ListItr<int> it = pos.zeroth();
	for (int i = 1; i < 9; i++) 
		pos.insert(i, it);	
	while (!current.isEmpty()) 
	{
		int num = current.topAndPop();
		Queen tmp(num);
		StackAr<int> list = tmp.ReturnDangerList(stackSize, lev);
		pos.remove(num);
		if (!list.isEmpty())
			pos.remove(list.topAndPop());
		if (!list.isEmpty())
			pos.remove(list.topAndPop());
		stackSize--;
	}
	it = pos.first();

	while (!it.isPastEnd()) {
		safe.push(it.retrieve());
		it.advance(); // neccessary?
	}

	return safe;
}
