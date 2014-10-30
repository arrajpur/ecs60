#include <iostream>
#include "InternalNode.h"

using namespace std;

/* DATA MEMBERS FOR REFERENCE
private: (from InternalNode)
  int internalSize;
  BTreeNode **children;
  int *keys;
protected: 
  int count;
  int leafSize;
  InternalNode *parent;
  BTreeNode *leftSibling;
  BTreeNode *rightSibling;
*/

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()





int InternalNode::getMinimum() const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()





void InternalNode::giveLeft(BTreeNode* node)
{
  if (node->getMinimum() < keys[0]) // if incoming node is less than min, hand it off
  {
    ((InternalNode*)leftSibling)->insert(node);
    /*(children[0])->setLeftSibling(node);
    node->setRightSibling(children[0]);*/
  }
  else
  {
    ((InternalNode*)leftSibling)->insert(children[0]);
    /*(children[0])->setLeftSibling(node);
    node->setRightSibling(children[0]);*/
    for (int i = 0; i < count - 1; i++) // shift to write over moved value
    {
      children[i] = children[i + 1];
      keys[i] = keys[i + 1];
    }
    count--;
    putIn(node);
  }
} // LeafNode::giveLeft() 





void InternalNode::giveRight(BTreeNode* node)
{
  if (node->getMinimum() > keys[count - 1])
  {
    ((InternalNode*)rightSibling)->insert(node); // if value is greater, just hand it off
    ((InternalNode*)rightSibling)->adjustMinimum();
  }
  else // if last value stored is greater, hand it off and replace it with the new one
  {
    ((InternalNode*)rightSibling)->insert(children[count - 1]);
    ((InternalNode*)rightSibling)->adjustMinimum();
    count--;
    putIn(node);
  }
} // LeafNode::giveRight() 





InternalNode* InternalNode::insert(int value)
{
  int i; 
  bool flag = false;
  BTreeNode* newN;

  for (i = count - 1; i >= 0; i--)
    if (value > keys[i])
    {
      newN = (children[i])->insert(value);
      //keys[i] = children[i]->getMinimum();
      flag = true;
      break;
    }
  if (!flag)
  {
    newN = (children[0])->insert(value);
    //keys[0] = children[0]->getMinimum();
  }
  
  for (int j = 0; j < count; j++)
    keys[j] = (children[j])->getMinimum();

  if (newN)
  {
    if (count < internalSize)
      putIn(newN);
    else // no room
    {
      if (leftSibling && leftSibling->getCount() < internalSize)
        giveLeft(newN);
      else if (rightSibling && rightSibling->getCount() < internalSize)
        giveRight(newN);
      else // need to split this shit
        return split(newN);
    }
  }
  /* be prepared for the return of a new node here and the 
      resulting cascade of splits that could occur */
  for (int j = 0; j < count; j++)
    keys[j] = (children[j])->getMinimum();
  
  
  return NULL; // to avoid warnings for now.
} // InternalNode::insert()





void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  putIn(oldRoot);
  putIn(node2);
  /*
  oldRoot->setParent(this);
  node2->setParent(this);
  */
  oldRoot->setRightSibling(node2);
  node2->setLeftSibling(oldRoot);
  // add sibling things here: oldRoot is left, node2 is right /always/
} // InternalNode::insert()





void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  putIn(newNode);
} // InternalNode::insert()





void InternalNode::adjustMinimum(void)
{
  keys[0] = children[0]->getMinimum();
}




/*void adjustKeys(void)
{
  for (i = 0; i < count; i++)
    keys[i] = children[i]
}*/





void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()





void InternalNode::putIn(BTreeNode* node)
{
  int i, currentNodeMin = node->getMinimum();

  for (i = count - 1; i >= 0 && currentNodeMin < keys[i]; i--)
  {
    children[i + 1] = children[i];
    keys[i + 1] = keys[i];
    //cout << "putIn : " << children[i + 1] << " :  " << keys[i + 1] << endl;
  }

  count++;
  node->setParent(this);
  children[i + 1] = node;
  keys[i + 1] = currentNodeMin;
  
  //cout << "putIn(f) : " << children[i + 1] << " :  " << keys[i + 1] << endl;
}




InternalNode* InternalNode::split(BTreeNode* nodeCausingSplit)
{
  int i;

  //cout << "SPLIT : " << nodeCausingSplit << " : " << nodeCausingSplit->getMinimum() << endl;
  InternalNode* newRight = 
    new InternalNode(internalSize, leafSize, this->parent, this, this->rightSibling);

  /*(internalSize + 1) / 2 gives index of last value guaranteed to move to newRight
  ((internalSize + 1) / 2) - 1 gives the value that could go depending on if the 
  new value or the old value stored at that position in keys[] is larger */

  for (i = internalSize - 1; i >= (internalSize + 1) / 2; i--)
  {
    newRight->insert(children[i]);
    (children[i])->setParent(newRight);
  }
  
  if (nodeCausingSplit->getMinimum() > keys[((internalSize + 1) / 2) - 1] ) // given value is greater than middle position
  {
    nodeCausingSplit->setParent(newRight);
    newRight->insert(nodeCausingSplit);
    (children[i + 1])->setLeftSibling(nodeCausingSplit);
    nodeCausingSplit->setRightSibling(children[i + 1]);
    nodeCausingSplit->setLeftSibling(children[((internalSize + 1) / 2) - 1]);
    (children[((internalSize + 1) / 2) - 1])->setRightSibling(nodeCausingSplit);
  }
  else 
  {
    newRight->insert( children[((internalSize + 1) / 2) - 1] );
    (children[((internalSize + 1) / 2) - 1])->setParent(newRight);

    (children[i + 1])->setLeftSibling(children[((internalSize + 1) / 2) - 1]);
    (children[((internalSize + 1) / 2) - 1])->setRightSibling(children[i + 1]);
    (children[((internalSize + 1) / 2) - 1])->setLeftSibling(nodeCausingSplit);
    nodeCausingSplit->setRightSibling(children[((internalSize + 1) / 2) - 1]);

    children[((internalSize + 1) / 2) - 1] = nodeCausingSplit;
    keys[((internalSize + 1) / 2) - 1] = nodeCausingSplit->getMinimum();
  }
  
  /*if (internalSize % 2 == 0)*/
    count = ((internalSize + 1) / 2);
  /*else
    count = ((internalSize + 1) / 2) + 1;*/

  if (rightSibling)
    rightSibling->setLeftSibling(newRight);
  rightSibling = newRight;

  return newRight;
} // LeafNode::split()