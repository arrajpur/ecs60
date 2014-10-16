#include<iostream>
#include "LinkedList.h"
#include "StackAr.h"
#include "CursorList.h"
#include "QueueAr.h"
#include "StackLi.h"
#include "vector.h"
#include "SkipList.h"
#include "CPUTimer.h"
#include "dsexceptions.h"
#include<string> 
#include<fstream>
using namespace std;

vector<CursorNode <int> > cursorSpace(250000) ;


int getChoice(void);
void RunList(string filename);
void RunCursorList(string filename);
void RunStackAr(string filename);
void RunStackLi(string filename);
void RunQueueAr(string filename);
void RunSkipList(string filename);


/* Everything APPEARS to be working
 * Clean up and test properly against timetest.out
 */

int main () 
{
	string file;
	int choice;
	CPUTimer ct;

	cout << "Filename >> " ;
	cin >> file;
	do
	{
		choice = getChoice();
		ct.reset();
		switch(choice) 
		{
			case 1: RunList(file); break;
			case 2: RunCursorList(file); break;
			case 3: RunStackAr(file); break;
			case 4: RunStackLi(file); break;
			case 5: RunQueueAr(file); break;
			case 6: RunSkipList(file); break;
		}	
		cout << "CPU time: " << ct.cur_CPUTime() << endl;
	} while (choice > 0);
}

int getChoice() 
{
	int option;
	cout << endl << "      ADT Menu" << endl;
	cout << "0. Quit" << endl; 
	cout << "1. LinkedList" << endl; 
	cout << "2. CursorList" << endl; 
	cout << "3. StackAr" << endl; 
	cout << "4. StackLi" << endl; 
	cout << "5. QueueAr" << endl; 
	cout << "6. SkipList" << endl; 
	cout << "Your choice >> ";
	cin >> option;
	return option;
}

void RunList(string filename)
{
	ifstream fh;
	char c;
	int num;
	List<int> L;
	ListItr<int> index = L.zeroth(); 

	fh.open(filename.c_str());
	string header;

	getline(fh, header);

	while(fh >> c >> num) 
	{
		if (c == 'i') 
			L.insert(num, index);
		else
			if (c == 'd') 
				L.remove(num);
	}

	fh.close(); 		
}
void RunCursorList(string filename)
{
	ifstream fh;
	char c;
	int num;
	CursorList<int> C(cursorSpace);
	CursorListItr<int> index = C.zeroth();	

	fh.open(filename.c_str());
	string header;

	getline(fh, header);
	while(fh >> c >> num) 
	{
		if (c == 'i') 
	 		C.insert(num, index);	
		else
			if (c == 'd') 
				C.remove(num);		
	}
	fh.close(); 		
}

void RunStackAr(string filename)
{
	ifstream fh;
	char c;
	int num;
	StackAr<int> S(250000);

	fh.open(filename.c_str());
	string header;

	getline(fh, header);

	while(fh >> c >> num) 
	{
		if (c == 'i') 
			S.push(num);
		else
			if (c == 'd') 
				S.pop();		
	}
	fh.close(); 		

}

void RunStackLi(string filename)
{
	ifstream fh;
	char c;
	int num;
	StackLi<int> Sl;

	fh.open(filename.c_str());
	string header;

	getline(fh, header);
	while(fh >> c >> num) 
	{
		if (c == 'i') 
			Sl.push(num);
		else
			if (c == 'd') 
				Sl.pop();			
	}
	fh.close(); 		
}

void RunQueueAr(string filename)
{
	ifstream fh;
	char c;
	int num;
	Queue<int> Q(250000);

	fh.open(filename.c_str());
	string header;

	getline(fh, header);
	while(fh >> c >> num) 
	{
		if (c == 'i') 
			Q.enqueue(num);
		else
			if (c == 'd') 
				Q.dequeue();		
	}
	fh.close(); 		

}

void RunSkipList(string filename)
{
	ifstream fh;
	char c;
	int num;
//	SkipList<int> Sk(250000);
	SkipList<int> Sk(-1, 250000);

	fh.open(filename.c_str());
	string header;

	getline(fh, header);
	while(fh >> c >> num) 
	{
		if (c == 'i') 
			Sk.insert(num);
		else
			if (c == 'd') 
				Sk.deleteNode(num);			
	}
	fh.close(); 		

}
