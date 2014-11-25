#include <iostream>

using namespace std;

bool checkMove(int &, int &, int &, int &);



int main(void)
{
  int rowOneCol = 1, moves[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; //moves is size nine so indexes 1-8 can be used
  bool moveGood = true;         //more easily (less confusion in the loops)

  for (int i = 1; i <= 8; i++) // current row being checked
  {
    for (int j = 1; j <= 8; j++) // current column being checked
    {
      for (int k = 1; k < i; k++) //checking all row-col choices up to now
      {
        moveGood = checkMove(k, moves[k], i, j);
        if (!moveGood)
          break;
      }
    
      if (moveGood)
      {
        moves[i] = j;
        break;
      } // if moveGood

      while (j >= 8 && i > 1)
      {
        i--;
        j = moves[i];
      }
        
      moveGood = true;
    } // for j
  } // for i

  for (int m = 1; m <= 7; m++)
    cout << moves[m] << ',';
  cout << moves[8] << endl;
  rowOneCol++;
  
  return 0;
}



bool checkMove(int & i, int & p_i, int & j, int & p_j)
{
  if ( (p_i - p_j) == (i - j) 
    || (p_i - p_j) == 0
    || (p_i - p_j) == (j - i)) 
      return false;
  
  return true;
}
