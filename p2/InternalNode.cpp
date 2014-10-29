#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize + 1]; // modified to make sorting easier
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
// find correct child for value
	int correct_child = -1;

	for (int i = 0; i < count; i++) 
		if (children[i]->getMinimum() > value)
		{
			if (i > 0)
				correct_child = i - 1;  
			else
				correct_child = i;

			break; 
		}

	// if value is bigger than all the mins of the children, add it to the last child
	if (correct_child == -1) 
		correct_child = count - 1;

	// for child rightSibling pass check
	int prevcount = children[correct_child]->getCount(); 
	int prevmin = children[correct_child]->getMinimum();
	bool isLeftFull = false; // leftSibling of the child

	// insert value to the correct child
	BTreeNode * test = children[correct_child]->insert(value);

	// if there is no leftSibling or it's full, isLeftFull = true
	if (correct_child == 0 || (children[correct_child]->getMinimum() != prevmin && children[correct_child]->getCount() <= prevcount)) 
		isLeftFull = true;

	// if child passed to rightSibling
	// Need to update key of rightSibling's parent
	// as new value in rightSibling will be the new minimum
	if (isLeftFull)
	{
		// if child's rightSibling is also a child of this InternalNode
		if (correct_child != count - 1) 
			keys[correct_child+1] = children[correct_child+1]->getMinimum();
		else if (rightSibling)// if child's rightSibling has a different parent
			((InternalNode*)rightSibling)->setKey();
	} 

	// update key of current child
	keys[correct_child] = children[correct_child]->getMinimum();

	// if child split
	if (test) 
	{
		// find place for new child, insert into children[]
		// and keep children[] sorted
		int index = -1;
		
		// find new child's place
		for (int i = 0; i < count; i++)
			if (children[i]->getMinimum() > test->getMinimum()) 
			{
				index = i;
				break;
			}

		// shift array down to make room
		if (index != -1)
		{
			for (int n = count; n > index; n--) 
			{
				children[n] = children[n-1];
				keys[n] = keys[n-1];
			}

			children[index] = test;
			keys[index] = test->getMinimum();
		}
		else // child added at end
		{
			children[count] = test;
			keys[count] = test->getMinimum();
		}
	
		count++;
		
		if (count > internalSize) 
		{
			if (leftSibling && leftSibling->getCount() < internalSize)
			{ // lookLeft
				((InternalNode*)leftSibling)->insert(children[0]); 
				count--;

				for (int o = 0; o < count; o++)
				{
					children[o] = children[o+1];
					keys[o] = keys[o+1];
				}
			}
			else if (rightSibling && rightSibling->getCount() < internalSize)
			{ // lookRight
				((InternalNode*)rightSibling)->insert(children[count - 1]);
				count--;
			}
			else
			{ // split
				InternalNode * newInternal = new InternalNode(internalSize, leafSize, parent, this, rightSibling);	
				// find up to what index this InternalNode will keep
				int mine = (internalSize + 1) / 2;
				mine--;
				
				// shift every child beyond that index to new InternalNode
				for (int k = mine + 1; k < internalSize + 1; k++)
				{
					((InternalNode*)newInternal)->insert(children[k]); 
					count--;
				}
				
				// reassign rightSibling's leftSibling pointer
				if (rightSibling)
					rightSibling->setLeftSibling(newInternal);
				
				// set own rightSibling as the new InternalNode
				rightSibling = newInternal;

				// return new InternalNode
				return newInternal;
			}
		}
			
	}
	
  return NULL; // to avoid warnings for now.
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
	children[0] = oldRoot;
	children[1] = node2;  

	count = 2;

	keys[0] = children[0]->getMinimum();
	keys[1] = children[1]->getMinimum();
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
	// find place for new child
	int index = -1;

	for (int i = 0; i < count; i++)
		if (children[i]->getMinimum() > newNode->getMinimum())
		{
			index = i;
			break;
		}

	if (index != -1)
	{
		// shift down array to make room
		for (int n = count; n > index; n--)
		{
			children[n] = children[n - 1];
			keys[n] = keys[n - 1];	
		}

		children[index] = newNode;
		keys[index] = newNode->getMinimum();
	} 
	else // if child goes in last place
	{
		children[count] = newNode;
		keys[count] = newNode->getMinimum();
	}

	if (parent)
		parent->setKey();
	count++;
} // InternalNode::insert()

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

void InternalNode::setKey(void)
{ // set the key of child's rightSibling's parent if it is not current node
  // allows access to that parent's keys array to be updated
	keys[0] = children[0]->getMinimum();
}
