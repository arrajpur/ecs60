#include "CPUTimer.h"
#include "dsexceptions.h"
#include "vector.h"

#include "LinkedList.h"
#include "CursorList.h"
#include "StackAr.h"
#include "StackLi.h"
#include "QueueAr.h"
#include "SkipList.h"

#include <iostream>
#include <fstream>
#include <string>

vector<CursorNode <int> > cursorSpace(250000);

int getChoice(void);
void RunList(string &filename);
void RunCursorList(string &filename);
void RunStackAr(string &filename);
void RunStackLi(string &filename);
void RunQueueAr(string &filename);
void RunSkipList(string &filename);




int main(void)
{
  string filename; // char array instead? just cut to it?
  int choice;
  CPUTimer ct;

  cout << "Filename >> ";
  cin >> filename;

  do
  {
    choice = getChoice();
    ct.reset();
    switch (choice)
    { 
      case 1 : RunList(filename); break;
      case 2 : RunCursorList(filename); break;
      case 3 : RunStackAr(filename); break;
      case 4 : RunStackLi(filename); break;
      case 5 : RunQueueAr(filename); break;
      case 6 : RunSkipList(filename); break; 
    } // switch (choice)

    cout << "CPU time: " << ct.cur_CPUTime() << endl;
  } while (choice > 0 && choice < 7);

  return 0;
}



int getChoice(void)
{
  int choice;

  cout << "\n      ADT Menu\n"
    << "0. Quit\n"
    << "1. LinkedList\n"
    << "2. CursorList\n"
    << "3. StackAr\n"
    << "4. StackLi\n"
    << "5. QueueAr\n"
    << "6. SkipList\n"
    << "Your choice >> ";
  cin >> choice;

  return choice;
}



void RunList(string &filename)
{
  ifstream in(filename.c_str());
  List<int> list;
  ListItr<int> itr = list.zeroth();

  char command;
  int current;

  in.ignore(1000, '\n');

  while (in >> command >> current)
    if (command == 'i')
    { 
      list.insert(current, itr);
      itr.advance();
    } 
    else
      list.remove(current);

/*
  itr = list.zeroth(); /////////////////////////////////

  while (!itr.isPastEnd()) /////////////////////////////////
  { /////////////////////////////////
    cout << itr.retrieve() << endl; /////////////////////////////////
    itr.advance(); /////////////////////////////////
  } /////////////////////////////////

  if (list.isEmpty()) /////////////////////////////////
    cout << "LOSER" << endl; /////////////////////////////////
*/
  in.close();
}

void RunCursorList(string &filename)
{
  ifstream in(filename.c_str());
  CursorList<int> cursList(cursorSpace);
  CursorListItr<int> cursItr = cursList.zeroth();

  char command;
  int current;

  in.ignore(1000, '\n');

  while (in >> command >> current)
    if (command == 'i')
    {
      cursList.insert(current, cursItr);
      cursItr.advance();
    }
    else 
      cursList.remove(current);

  /*cursItr = cursList.first(); /////////////////////////////////

  while (!cursItr.isPastEnd()) /////////////////////////////////
  { /////////////////////////////////
    cout << cursItr.retrieve() << endl; /////////////////////////////////
    cursItr.advance(); /////////////////////////////////
  } /////////////////////////////////

  if (cursList.isEmpty()) /////////////////////////////////
    cout << "LOSER" << endl; /////////////////////////////////*/

  in.close();
}

void RunStackAr(string &filename)
{
  ifstream in(filename.c_str());
  StackAr<int> stackAr(250000);

  char command;
  int current;

  in.ignore(1000, '\n');

  while (in >> command >> current)
    if (command == 'i')
      stackAr.push(current);
    else 
      stackAr.pop();

  /*
  while (!stackAr.isEmpty())
    cout << stackAr.topAndPop() << endl;
  */

  in.close();
}

void RunStackLi(string &filename)
{
  ifstream in(filename.c_str());
  StackLi<int> stackLi;

  char command;
  int current;

  in.ignore(1000, '\n');

  while (in >> command >> current)
    if (command == 'i')
      stackLi.push(current);
    else 
      stackLi.pop();

  /*
  while (!stackLi.isEmpty())
    cout << stackLi.topAndPop() << endl;
  */

  in.close();
}

void RunQueueAr(string &filename)
{
  ifstream in(filename.c_str());
  Queue<int> q(250000);

  char command;
  int current;

  in.ignore(1000, '\n');

  while (in >> command >> current)
    if (command == 'i')
      q.enqueue(current);
    else 
      q.dequeue();

  /*
  while (!q.isEmpty())
  {
    cout << q.getFront() << endl;
    q.dequeue();
  }
  */

  in.close();
}

void RunSkipList(string &filename)
{
  ifstream in(filename.c_str());
  SkipList<int> cristina(250000);

  char command;
  int current;

  in.ignore(1000, '\n');

  while (in >> command >> current)
    if (command == 'i')
      cristina.insert(current);
    else 
      cristina.deleteNode(current);

  /*
  while (!cristina.isEmpty())
  {
    cout << cristina.getFront() << endl;
    cristina.dequeue();
  }
  */

  in.close();
}
