#include <iostream>
#include "BTree.h"
#include "BTreeNode.h"
#include "LeafNode.h"
#include "InternalNode.h"
using namespace std;

BTree::BTree(int ISize, int LSize):internalSize(ISize), leafSize(LSize)
{
  root = new LeafNode(LSize, NULL, NULL, NULL);
} // BTree::BTree()


void BTree::insert(const int value)
{
  BTreeNode* newN = root->insert(value);

  if (newN)
  {
    //lookLeft // actually I don't think left right is necessary here, just
    //lookRight    // the split
    //root split
      {
        InternalNode* newR = new InternalNode(internalSize, leafSize,
          NULL, NULL, NULL);
        newR->insert(root, newN);
        root = newR;
      }
    // you need to make a new internal node here
  }
  // students must write this
} // BTree::insert()


void BTree::print()
{
  BTreeNode *BTreeNodePtr;
  Queue<BTreeNode*> queue(1000);

  queue.enqueue(root);
  while(!queue.isEmpty())
  {
    BTreeNodePtr = queue.dequeue();
    BTreeNodePtr->print(queue);
  } // while
} // BTree::print()
