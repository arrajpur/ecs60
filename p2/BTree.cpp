// Aaron Lewis Aparna Rajpurkar
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
	// insert into root
	BTreeNode * test = root->insert(value);
	
	// if root split
	if (test) 
	{
		InternalNode * newRoot = new InternalNode(internalSize, leafSize, NULL, NULL, NULL);
		newRoot->insert(root, test);
		root = newRoot;
	}
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
