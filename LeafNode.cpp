#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;

/* DATA MEMBERS FOR REFERENCE
protected: (from BTreeNode)
  int count;
  int leafSize;
  InternalNode *parent;
  BTreeNode *leftSibling;
  BTreeNode *rightSibling;
private: (from LeafNode)
  int *values;
*/

LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()





int LeafNode::getMinimum() const
{
  if (count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()





LeafNode* LeafNode::insert(int value)
{
  //cout << this << " :  ins(" << value << ") : ";
  //if (count == leafSize)
    //cout << " FULL : ";
  /*for (int j = 0; j < count; j++)
    cout << " [" << j << "] : " << values[j] << " ";
  cout << endl;*/

  if (count == leafSize)
  {
    if (leftSibling && leftSibling->getCount() < leafSize)
      giveLeft(value);
    else if (rightSibling && rightSibling->getCount() < leafSize)
      giveRight(value);
    else // needs to split
      return split(value); 
  }
  else
  {
    putIn(value); // count handled separately so putIn never has count == leafSize
    count++; 
  }

  return NULL; // to avoid warnings for now.
}  // LeafNode::insert()





void LeafNode::giveLeft(int & value)
{
  if (value < values[0]) // if incoming value is less than min, hand it off
    leftSibling->insert(value);
  else
  {
    leftSibling->insert(values[0]);

    for (int i = 0; i < count - 1; i++) // shift to write over moved value
      values[i] = values[i + 1];
    count--;
    putIn(value);
    count++;
  }
} // LeafNode::giveLeft()





void LeafNode::giveRight(int & value)
{
  if (value > values[count - 1])
    rightSibling->insert(value); // if value is greater, just hand it off
  else // if last value stored is greater, hand it off and replace it with the new one
  {
    rightSibling->insert(values[count - 1]);
    count--;
    putIn(value);
    count++;
  }
} // LeafNode::giveRight()





int LeafNode::min(int & num1, int & num2) const
{
  if (num1 < num2)
    return num1;
  return num2;
} // LeafNode::min()





int LeafNode::max(int & num1, int & num2) const
{
  if (num1 > num2)
    return num1;
  return num2;
} // LeafNode::max()





void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()





void LeafNode::putIn(int & value)
{
  int i;

  for (i = count - 1; i >= 0 && value < values[i]; i--)
    values[i + 1] = values[i];
  values[i + 1] = value;
}





LeafNode* LeafNode::split(int value)
{
  //cout << "SPLIT : " << value;
  LeafNode* newRight = 
    new LeafNode(leafSize, this->parent, this, this->rightSibling);
  //cout << " : " << newRight << endl;

  /*(leafSize + 1) / 2 gives index of last value guaranteed to move to newRight
  ((leafSIze + 1) / 2) - 1 gives the value that could go depending on if the 
  new value or the old value stored at that position in values[] is larger */

  for (int i = leafSize - 1; i >= (leafSize + 1) / 2; i--)
    newRight->insert(values[i]);
  
  if (value > values[((leafSize + 1) / 2) - 1] ) // given value is greater than middle position
    newRight->insert(value);
  else 
  {
    newRight->insert( values[((leafSize + 1) / 2) - 1] );
    values[((leafSize + 1) / 2) - 1] = value;
  }
  
  count = (leafSize + 1) / 2;

  if (rightSibling)
    rightSibling->setLeftSibling(newRight);
  rightSibling = newRight;

  return newRight;
} // LeafNode::split()


