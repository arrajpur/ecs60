#include "ExtendibleHash.h"

// the important point is in how Ehash works
// and exactly what it returns
// shifts binary number to the right (18 - bits), then returns that number in decimal
int Ehash(int value, int bits)
{
//  std::cout << "Ehash()" << std::endl;
  return value >> (18 - bits) ;
  // does this convert entire number to bitwise
  // return only the n bits 
  // or convert number to 18 bit number?
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
//	std::cout << "ExtendibleHash::ExtendibleHash()" << std::endl;
//	std::cout << "Exp is " << exp << " bits is " << bits << std::endl;
	int exp = pow(bits);
	Directory = new ExtendibleLeaf * [exp];

	for (int i = 0; i < exp; i++) 
	{
		Directory[i] = new ExtendibleLeaf(LeafSize);
	} 
	// what is notFound for?
	// Constructor has an initialization list, nothing to do here
} // ExtendibleHash()

void ExtendibleHash::insert(const int &object)
{
//	std::cout << "ExtendibleHash::insert()" << std::endl;
	// to find the place for the object
	// use Ehash 
	// convert those bits of interestto decimal number
	// Directory[number] = object
	//
	// if ExtendibleLeaf freaks, call split()	
	// end
	// the insert should return a bool
	// telling whether it was successful or not
	// not successful == full
//	std::cout << "Segfault? ExtendibleHash::insert() start" << std::endl;
	int index = Ehash(object, bits);
//	std::cout << "Segfault? ExtendibleHash::insert() after Ehash() index is "<< index << std::endl;
	if (!Directory[index]->insert(object))
	{
//		std::cout << "(Hash) Insert call to split" << std::endl;
//		std::cout << "Segfault? ExtendibleHash::insert() in if" << std::endl;
		split(object);
	}
//	std::cout << "(Hash) Inserted object " << object << " at index " << index << std::endl;
//	std::cout << "Segfault? ExtendibleHash::insert() end" << std::endl;
} // insert()


void ExtendibleHash::remove(const int &object)
{
//	std::cout << "ExtendibleHash::remove()" << std::endl;
	int index = Ehash(object, bits);
//	std::cout << "index is " << index << " size is " << size << std::endl;
	int found = Directory[index]->search(object);
//	std::cout << "Segfault?" << std::endl;	
	if (found) 
	{
		Directory[index]->remove(found);
	}
	// copy code from find() up to "if found" line
	// take the index of the leaf, and the index of the position in the leaf
	// delete the number from that leaf
	// using an ExtendibleLeaf sub remove()
	// which will involve shifting arrays
	// the ExtendibleLeaf sub will return the value removed!
}  // remove()


const int & ExtendibleHash::find(const int &object)
{
//	std::cout << "ExtendibleHash::find()" << std::endl;

	int index = Ehash(object, bits);
//	std::cout << "index is " << index << " object is " << object << " bits are " << bits << std::endl;
	int found = Directory[index]->search(object);
	
//	std::cout << "found is " << found << std::endl;
	if (found != -1)
		return object;
	else
		return minusone;
	// convert number to bitwise via Ehash
	// consider only [bits] number of bits
	// go to that index in Directory by converting it
	// to a decimal number
	// search the values for the number -- write an ExtendibleLeaf sub that does this
	// that sub return the index which it was found at in the leaf
	// if found, return the input number (tricky!)
	// if false, return -1
}  // find()



void ExtendibleHash::split(const int &object)
{
//	std::cout << "ExtendibleHash::split()" << std::endl;
	// need to find index of where we should insert
	int index = Ehash(object, bits);
	
	// object is the number that needs to be inserted?
	// while leaf can't split
	// double 
	// when splitting, use an ExtendibleLeaf function to run through the array
	// but use an ExtendibleHash function to re-insert the removed value with updated bits
	int * splits = NULL;
	do {
//	while(splits == NULL) 
//	{
		bits++;
		ExtendibleLeaf ** tempDir = new ExtendibleLeaf* [pow(bits)];
		int j = 0;
		for (int i = 0; i < size; i++)
		{
			tempDir[j] = Directory[i];
			j++;
			tempDir[j] = Directory[i];
			j++;
		}
		Directory = tempDir;	
//		std::cout << "j is " << j << std::endl;
		index = Ehash(object, bits);	
		splits = Directory[index]->split(bits);

		if (splits)
		{
			int pattern = Ehash(splits[1], bits);
			Directory[pattern] = new ExtendibleLeaf(LeafSize);
			for (int m = 0; m < splits[0]; m++)
			{
				int num = Directory[index]->remove(splits[m]);
				Directory[Ehash(num, bits)]->insert(num);
			}
			Directory[Ehash(object, bits)]->insert(object); 
		}
		size = pow(bits);
		// double directory size
		// observe problem leaf (index)
		// determine which pattern to split out
		// split out that pattern
		// insert that pattern into new leaf
		// update bits and size
		// update index if necessary (if object fits split pattern)
//	}
//	} while (!Directory[index]->insert(object));
	} while (!splits);
	
}  // split()


ExtendibleLeaf::ExtendibleLeaf(int leafSize) 
{
//	std::cout << "ExtendibleLeaf::ExtendibleLeaf()" << std::endl;
//	std::cout << "Segfault? ExtendibleLeaf::ExtendibleLeaf() start" << std::endl;
	maxSize = leafSize;
	valueSize = 0;
	values = new int[leafSize];
//	std::cout << "maxSize is "<< maxSize << std::endl; // for some reason maxSize access causes segfault?
}

bool ExtendibleLeaf::insert(const int &object)
{
//	std::cout << "ExtendibleLeaf::insert()" << std::endl;
//	std::cout << "maxSize is "<< maxSize << std::endl; // for some reason maxSize access causes segfault?
//	std::cout << "valueSize is "<< valueSize << std::endl; // for some reason valueSize access causes segfault?
//	std::cout << "Segfault? ExtendibleLeaf::insert() start" << std::endl;
	if (maxSize == valueSize)
		return false;

	// insert here
//	std::cout << "Segfault? ExtendibleLeaf::insert() before assignment" << std::endl;
	values[valueSize] = object; // will this segfault?
	valueSize++;
//	std::cout << "Segfault? ExtendibleLeaf::insert() after assignment" << std::endl;
	
//	std::cout << "Segfault? ExtendibleLeaf::insert() end" << std::endl;
//	std::cout << "(Leaf) Inserted object " << object << " at valueSize " << valueSize - 1 << std::endl;
	return true; 
}

int ExtendibleLeaf::remove(int pos)
{
	// using an ExtendibleLeaf sub remove()
	// which will involve shifting arrays
	// the ExtendibleLeaf sub will return the value removed!
//	std::cout << "ExtendibleLeaf::remove()" << std::endl;
	int removed = values[pos];

	values[pos] = values[valueSize - 1];	

	valueSize--;
//	std::cout << "(Leaf) Removed " << removed << " valueSize is now " << valueSize << std::endl;
	return removed; // stupid way of doing this, we pass in the object to be removed!
}

int ExtendibleLeaf::search(const int &object)
{
	// search the values for the number -- write an ExtendibleLeaf sub that does this
	// that sub return the index which it was found at in the leaf
//	std::cout << "ExtendibleLeaf::search()" << std::endl;
//	std::cout << "valueSize is " << valueSize << std::endl;
	for (int i = 0; i < valueSize; i++) 
	{
//		std::cout << "debug(): values[i] " << values[i] << std::endl;
		if (values[i] == object)
			return i;
	}

	return -1; // placeholder
}

int * ExtendibleLeaf::split(int b) 
{
//	std::cout << "ExtendibleLeaf::split()" << std::endl;
	// find first item that can split out
	// return array of positions to split out?
	// and NULL if nothing
	int pattern = Ehash(values[0], b);
	int * split_out = new int [valueSize];
	int splitter = -1;
	int size = 0;
	for (int i = 1; i < valueSize; i++) 
	{
		int test = Ehash(values[i], b);
		if (test != pattern)
		{
			if (splitter == -1)
				splitter = test;
			
			if (test == splitter)
			{
				split_out[size+1] = i;
				size++;
			}
		}
		
	}
	split_out[0] = size;
	if (size == 0)
		return NULL;
	else
		return split_out;		
}

