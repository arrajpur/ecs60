#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize + 1];
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
  }
  else 
  {
	if (leftSibling && leftSibling->getCount() < leafSize) 
	{
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

		for (int o = 0; o < count; o++)
			values[o] = values[o+1];
	}
	else if (rightSibling && rightSibling->getCount() < leafSize)
	{ 
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
	{
		LeafNode * newLeaf = new LeafNode(leafSize, parent, this, rightSibling); 

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

		int mine = (leafSize + 1 )/ 2 ;  
		mine--;

		for (int k = mine + 1; k < leafSize + 1; k++) 
		{
			newLeaf->insert(values[k]);
			count--; // not actually deleting
		}

		if (rightSibling)
			rightSibling->setLeftSibling(newLeaf);

		rightSibling = newLeaf;
		
		return newLeaf; // parent updates here
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


