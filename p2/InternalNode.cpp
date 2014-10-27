#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize + 1];
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
		int correct_child = -1;
		for (int i = 0; i < count; i++) 
		{
			if (children[i]->getMinimum() > value)
			{
				if (i > 0)
					correct_child = i - 1;  
				else
					correct_child = i;
				break;
			}
		}
		if (correct_child == -1) 
			correct_child = count - 1;

		int prevcount = children[correct_child]->getCount(); 
		bool isLeftFull = false; 
		if (!children[correct_child]->getLeftSibling() || (children[correct_child]->getLeftSibling())->getCount() == leafSize) {
			isLeftFull = true;
		}

		BTreeNode * test = children[correct_child]->insert(value);
//		if (keys[correct_child] == children[correct_child]->getMinimum() && prevcount != children[correct_child]->getCount())
//			keys[correct_child+1] = children[correct_child+1]->getMinimum();
//
			cout << "Keys of correct child is " << keys[correct_child] << endl;
			cout << "correct child min is " << children[correct_child]->getMinimum() << endl;
		if (children[correct_child]->getCount() == prevcount && isLeftFull && rightSibling)
		{
			// passed right
			if (correct_child != count -1) {
				cout << "Internal knows we passed right " << endl;
				keys[correct_child+1] = children[correct_child+1]->getMinimum();
			}
			else
			{ 
				cout << "rightSibling has another parent!" << endl;
				((InternalNode*)rightSibling)->setKey();
			}
		} 
		keys[correct_child] = children[correct_child]->getMinimum();
//		keys[correct_child+1] = children[correct_child+1]->getMinimum();

		if (test) 
		{
			int newmin = test->getMinimum();
	
			int index = -1;
		
			for (int i = 0; i < count; i++)
			{
				if (children[i]->getMinimum() > newmin) 
				{
					index = i;
					break;
				}
			}
			if (index != -1)
			{
				for (int n = count; n > index; n--) 
				{
					children[n] = children[n-1];
					keys[n] = keys[n-1];
				}
				children[index] = test;
				keys[index] = newmin;
			}
			else 
			{
				children[count] = test;
				keys[count] = newmin;
			}
			count++;

			if (count > internalSize) 
			{
				//look Left
				//look Right
				//Split
				if (leftSibling && leftSibling->getCount() < internalSize)
				{
					cout << "Internal node look left" << endl;
					((InternalNode*)leftSibling)->insert(children[0]); // sibling insert! need to write
					count--;
					for (int o = 0; o < count; o++)
					{
						children[o] = children[o+1];
						keys[o] = keys[o+1];
					}
				}
				else if (rightSibling && rightSibling->getCount() < internalSize)
				{
				// check right
					cout << "Internal node look right" << endl;
				
					((InternalNode*)rightSibling)->insert(children[count - 1]);
					count--;
				}
				else
				{
				// split
					cout << "Internal node split" << endl;	
					InternalNode * newInternal = new InternalNode(internalSize, leafSize, parent, this, rightSibling);	
					int mine = (internalSize + 1) / 2;
					mine--;
					for (int k = mine + 1; k < internalSize + 1; k++)
					{
						((InternalNode*)newInternal)->insert(children[k]); // again that other function
						count--;
					}
					if (rightSibling)
						rightSibling->setLeftSibling(newInternal);

					rightSibling = newInternal;
					return newInternal;
						
				}
			}
			
		}
		else 
		{
			// child did not split
			// internalNode will not split
			return NULL;
		}
//	else 
//	{
		// check left
//	}
	
  return NULL; // to avoid warnings for now.
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  // students must write this

//temporary
//
	cout << "Creating new root, InternalNode insert" << endl;
	children[0] = oldRoot;
	children[1] = node2;  
	count = 2;
	keys[0] = children[0]->getMinimum();
	keys[1] = children[1]->getMinimum();
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  // students may write this
	int index = -1;
	for (int i = 0; i < count; i++)
	{
		if (children[i]->getMinimum() > newNode->getMinimum())
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		for (int n = count; n > index; n--)
		{
			children[n] = children[n - 1];
			keys[n] = keys[n - 1];	
		}
		children[index] = newNode;
		keys[index] = newNode->getMinimum();
	} 
	else 
	{
		children[count] = newNode;
		keys[count] = newNode->getMinimum();
	}
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
{
	cout << "setKey count is " << count << endl; 
	keys[0] = children[0]->getMinimum();
}
