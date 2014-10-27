#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize + 1]; // modified to make sorting easier
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{ 
  if (count < leafSize) 
  {
	// find the place for value
	// search and sort should be done outside the conditionals
	// as it has to be done for each case
	int index = -1;

	for (int i = 0; i < count; i++)
		if (values[i] > value)
		{
			index = i;
			break;
		}

	if (index != -1) 
	{
		// shift down array
		for (int n = count; n > index; n--)
			values[n] = values[n - 1];	

		values[index] = value;		
	}
	else // if value goes in last spot 
		values[count] = value;
	
	count++;
  }
  else 
  {
	if (leftSibling && leftSibling->getCount() < leafSize) 
	{ // lookLeft
		// this search and sort is extraneous, should be done first like in InternalNode
		int index = -1;

		for (int i = 0; i < count; i++)
			if (values[i] > value)
			{
				index = i;
				break;
			}
		
		if (index != -1) 
		{
			for (int n = count; n > index; n--)
				values[n] = values[n - 1];	

			values[index] = value;		
		}
		else 
			values[count] = value;
	
		count++;
		leftSibling->insert(values[0]);
		count--;	

		// shift array down
		// because values[o] has been given away
		for (int o = 0; o < count; o++)
			values[o] = values[o+1];
	}
	else if (rightSibling && rightSibling->getCount() < leafSize)
	{ // lookRight 
		int index = -1;

		for (int i = 0; i < count; i++)
			if (values[i] > value)
			{
				index = i;
				break;
			}

		if (index != -1) 
		{
			for (int n = count; n > index; n--)
				values[n] = values[n - 1];	

			values[index] = value;		
		}
		else 
			values[count] = value;
	
		count++;
		rightSibling->insert(values[count-1]);
		count--; 
	}
	else
	{ // split
		LeafNode * newLeaf = new LeafNode(leafSize, parent, this, rightSibling); 
		// again, extraneous sorting
		int index = -1;
		for (int i = 0; i < count; i++)
			if (values[i] > value)
			{
				index = i;
				break;
			}
		if (index != -1) 
		{
			for (int n = count; n > index; n--)
				values[n] = values[n - 1];	
	
			values[index] = value;		
		}
		else 
			values[count] = value;

		count++;
		
		// find last index of values to keep
		// works for even and odd leafSize
		int mine = (leafSize + 1 ) / 2 ;  
		mine--;

		// give away all values beyond that index
		for (int k = mine + 1; k < leafSize + 1; k++) 
		{
			newLeaf->insert(values[k]);
			count--; // not actually deleting
		}
		// reassign rightSibling's leftSibling 
		if (rightSibling)
			rightSibling->setLeftSibling(newLeaf);

		// set current leaf's rightSibling as new LeafNode
		rightSibling = newLeaf;
	
		// return new LeafNode	
		return newLeaf;
	}
  }
  return NULL; // to avoid warnings for now.
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


