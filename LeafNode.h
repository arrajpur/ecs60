#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
  // else NULL
  int min(int & num1, int & num2) const; // necessary? 
  int max(int & num1, int & num2) const; // necessary?
  void print(Queue <BTreeNode*> &queue);
  void giveLeft(int & value);
  void giveRight(int & value);
  void putIn(int & value);
  LeafNode* split(int value);
}; //LeafNode class

#endif


// only thing to add is probably something dealing with updating parents min values
