#include "ExtendibleHash.h"

int Ehash(int value, int bits)
{
  return value >> (18 - bits) ;
} // Ehash()

int pow(int n)
{
	int exp = 1;
      	for (int j = 1; j <= n; j++) {
		exp*=2;
	}	
	return exp;
}

ExtendibleHash::ExtendibleHash(const int & notFound, int b, int LSize) :
  bits(b), LeafSize(LSize)
{
	size = pow(bits);
	minusone = -1;
	int exp = pow(bits);
	Directory = new ExtendibleLeaf * [exp];

	for (int i = 0; i < exp; i++) 
	{
		Directory[i] = new ExtendibleLeaf(LeafSize);
	} 
} // ExtendibleHash()

void ExtendibleHash::insert(const int &object)
{
	int index = Ehash(object, bits);
//	std::cout << "Hash insert() object " << object << " bits " << bits << " index " << index << " size " << size<<std::endl;
	if (!Directory[index]->insert(object))
	{
//		std::cout << "Hash insert() calls split" << std::endl;
		split(object);
	}
} // insert()


void ExtendibleHash::remove(const int &object)
{
	int index = Ehash(object, bits);
	int found = Directory[index]->search(object);
	if (found != -1) 
	{
		Directory[index]->remove(found);
	}
}  // remove()


const int & ExtendibleHash::find(const int &object)
{

	int index = Ehash(object, bits);
	int found = Directory[index]->search(object);

//	std::cout << "find() object " << object << " found " << found << std::endl;	

	if (found != -1)
		return object;
	else
		return minusone;
}  // find()



void ExtendibleHash::split(const int &object)
{
	int original_index = Ehash(object, bits);
//	std::cout << "original_index is " << original_index << std::endl;
//	int index = -1;	
	int * splits = NULL;
//	do {
//
	splits = Directory[original_index]->split(bits);

	if (splits != NULL)
	{
		int pattern = Ehash(splits[1], bits);
		Directory[pattern] = new ExtendibleLeaf(LeafSize);
		for (int m = 1; m <= splits[0]; m++)
		{
			int rindex = Directory[original_index]->search(splits[m]);
			int num = Directory[original_index]->remove(rindex);
			Directory[pattern]->insert(num);
		}
		Directory[Ehash(object, bits)]->insert(object); 

	}

	while(splits == NULL)
	{
		bits++;
		ExtendibleLeaf ** tempDir = new ExtendibleLeaf* [pow(bits)];
		int j = 0;
		for (int i = 0; i < size; i++)
		{
			tempDir[j] = Directory[i];
			j++;
			tempDir[j] = Directory[i];
			j++;

//			if (i == original_index)
//				index = j - 1;
		}
//		Directory = tempDir;
	
//		index = Ehash(object, bits);	
//		std::cout << "Hash split, index is " << original_index << " leaf size is " << Directory[original_index]->getValueSize() << std::endl;
//		splits = Directory[index]->split(bits);
		splits = Directory[original_index]->split(bits);

		if (splits != NULL)
		{
			int pattern = Ehash(splits[1], bits);
//			Directory[pattern] = new ExtendibleLeaf(LeafSize);
			tempDir[pattern] = new ExtendibleLeaf(LeafSize);
//			std::cout << "Hash splits(), in if, pattern is " << pattern << std::endl;
			for (int m = 1; m <= splits[0]; m++)
			{
				int rindex = Directory[original_index]->search(splits[m]);
		//		if (rindex == -1)	
		//			std::cout << "rindex is -1 !!!" << std::endl;
				int num = Directory[original_index]->remove(rindex);
				tempDir[pattern]->insert(num);
			}
//			std::cout << "Hash split() inserting object to " << Ehash(object, bits) << std::endl;
			tempDir[Ehash(object, bits)]->insert(object); 
		}
		Directory = tempDir;
		size = pow(bits);
		original_index = Ehash(object, bits);
	}
//	} while (!splits);
	
}  // split()


ExtendibleLeaf::ExtendibleLeaf(int leafSize) 
{
	maxSize = leafSize;
	valueSize = 0;
	values = new int[leafSize];
}

bool ExtendibleLeaf::insert(const int &object)
{
//	std::cout << "\tLeaf insert maxSize is " << maxSize << " valueSize is " << valueSize << " before insert, object is " << object << std::endl;
	
	if (maxSize == valueSize)
		return false;

	values[valueSize] = object; // will this segfault?
	valueSize++;
	return true; 
}

int ExtendibleLeaf::remove(int pos)
{
//	std::cout << "Leaf remove() valueSize is " << valueSize << std::endl;
	int removed = values[pos];

	values[pos] = values[valueSize - 1];	
	
	valueSize--;

//	std::cout << "Removed " << removed << " valueSize is now " << valueSize << std::endl;

	return removed; 
}

int ExtendibleLeaf::search(const int &object)
{
	for (int i = 0; i < valueSize; i++) 
	{
		if (values[i] == object)
		{
//			std::cout << "leaf search: found " << object << " at " << i << std::endl;
			return i;
		}
	}

	return -1; // placeholder
}

int * ExtendibleLeaf::split(int b) 
{
	int pattern = Ehash(values[0], b);
	int * split_out = new int [valueSize];
	int splitter = -1;
	int size = 0;
//	std::cout << "Leaf split() pattern is " << pattern << std::endl;
	for (int i = 1; i < valueSize; i++) 
	{
		int test = Ehash(values[i], b);
//		std::cout << "Leaf split() test is " << test << std::endl;
		if (test != pattern)
		{
			if (splitter == -1)
				splitter = test;
			
			if (test == splitter)
			{
				split_out[size+1] = values[i];
				size++;
			}
		}
		
	}
	split_out[0] = size;
//	std::cout << "\tLeaf split() size is " << size << std::endl;
	if (size == 0)
		return NULL;
	else
		return split_out;		
}

int ExtendibleLeaf::getValueSize ()
{
	return valueSize;
}
