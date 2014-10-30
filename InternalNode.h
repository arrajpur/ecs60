#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  void adjustMinimum(void);
  int getMinimum()const;
  void giveLeft(BTreeNode* node);
  void giveRight(BTreeNode* node);
  InternalNode* insert(int value); // returns pointer to new InternalNode
    // if it splits else NULL 
    // this one will handle splits from leaves
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);
  void putIn(BTreeNode* node);
  InternalNode* split(BTreeNode* nodeCausingSplit);
}; // InternalNode class

#endif
