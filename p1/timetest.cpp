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
using namespace std;

vector<CursorNode <int> > cursorSpace(250000) ;


int getChoice(void);
void RunList(string filename);
void RunCursorList(string filename);
void RunStackAr(string filename);
void RunStackLi(string filename);
void RunQueueAr(string filename);
void RunSkipList(string filename);

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
	cout << "ADT Menu" << endl;
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

}
void RunCursorList(string filename)
{

}
void RunStackAr(string filename)
{

}
void RunStackLi(string filename)
{

}
void RunQueueAr(string filename)
{

}
void RunSkipList(string filename)
{

}
