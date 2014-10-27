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
{ // this all looks like it works!!!
//  cout << "at insert count is "<< count <<endl; 
  if (count < leafSize) 
  {
	// need to update keys of parent
	
	// find index where value should go
	// insert value, shift down array
	// <--- works!
	int index = -1;
	for (int i = 0; i < count; i++)
	{
		if (values[i] > value)
		{
			index = i;
			break;
		}
	}
	if (index != -1) 
	{
		// shift down array
		for (int n = count; n > index; n--)
		{
			values[n] = values[n - 1];	
		}

		values[index] = value;		
	}
	else 
		values[count] = value;
	
	count++;
	// ---> above sorted insert works
  }
  else 
  {
	// check left
	
	if (leftSibling && leftSibling->getCount() < leafSize) 
	{
//		cout << "Look left" << endl;
		
		int index = -1;
		for (int i = 0; i < count; i++)
		{
			if (values[i] > value)
			{
				index = i;
				break;
			}
		}
		if (index != -1) 
		{
			// shift down array
			for (int n = count; n > index; n--)
			{
				values[n] = values[n - 1];	
			}

			values[index] = value;		
		}
		else 
			values[count] = value;
	
		count++;
		// lookLeft, update Parent
		// give away smallest value to leftSibling
		// will have to update parent after insert
		leftSibling->insert(values[0]);
		count--;	
		for (int o = 0; o < count; o++)
		{
			values[o] = values[o+1];
		}
	}
	else if (rightSibling && rightSibling->getCount() < leafSize)
	{ // this function is messed up // Not anymore! 
		cout << "Look right" << endl;		
		// insert value
		int index = -1;
		for (int i = 0; i < count; i++)
		{
			if (values[i] > value)
			{
				index = i;
				break;
			}
		}
		if (index != -1) 
		{
			// shift down array
			for (int n = count; n > index; n--)
			{
				values[n] = values[n - 1];	
			}

			values[index] = value;		
		}
		else 
			values[count] = value;
	
		count++;
		// lookRight, update Parent
		rightSibling->insert(values[count-1]);
		count--; 
//		for (int o = 0; o < count; o++)
//		{
//			values[o] = values[o+1];
//		}
	}
	else
	{
		// split
//		cout << "At split count is " << count << endl;
		LeafNode * newLeaf = new LeafNode(leafSize, parent, this, rightSibling); // ???
		// insert value to sort

		int index = -1;
		for (int i = 0; i < count; i++)
		{
//			cout << "split debug: values[i] is " << values[i] << endl;
			if (values[i] > value)
			{
				index = i;
				break;
			}
		}
		if (index != -1) 
		{
//			cout << "split debug: index is not -1 and is " << index << endl;
			// shift down array
			for (int n = count; n > index; n--)
			{
				values[n] = values[n - 1];	
			}
	
			values[index] = value;		
		}
		else 
			values[count] = value;
//		cout << "values[count] = " << values[count] << endl;
		count++;

//		int mine = leafSize / 2; // works for odd, not even
		int mine = (leafSize + 1 )/ 2 ; // these two lines work for both! 
		mine--;
//		cout << "Mine is " << mine << endl;
		// store half of values in newLeaf
		for (int k = mine + 1; k < leafSize + 1; k++) // will mine+1 work?
		{
			newLeaf->insert(values[k]);
			count--; // not actually deleting
//			cout << "Inserting values[k] : " << values[k] << endl;
//			cout << "in for, count is " << count << endl;
		}
//		cout << "Checking rightSibling" << endl;
		// then check rightSibling
		if (rightSibling)
			rightSibling->setLeftSibling(newLeaf);

		rightSibling = newLeaf;
		
		if (newLeaf->getLeftSibling()) 
			cout << "newLeaf has a left sibling" << endl;
		if (newLeaf->getRightSibling()) 
			cout << "newLeaf has a right sibling" << endl;
		return newLeaf; // parent updates here
	}
  }
  // students must write this
  return NULL; // to avoid warnings for now.
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


