#include "ExtendibleHash.h"
#include <iostream>
using namespace std;



int Ehash(int value, int bits)
{
  return value >> (18 - bits) ;
} // Ehash()




ExtendibleHash::ExtendibleHash(const int & notFound, int b, int LSize) :
  bits(b), LeafSize(LSize)
{
  int startSize = 1;
  size = startSize;
  potentialLeafSpaces = 2;
  not_found = notFound;

  for (int i = 0; i < bits; i++)
    startSize *= 2;
  Directory = new ExtendibleLeaf*[startSize]; // access things using Directory[i]->whatever();

  ExtendibleLeaf* first = new ExtendibleLeaf(LeafSize);

  for (int i = 0; i < potentialLeafSpaces; i++)
    Directory[i] = first;

} // ExtendibleHash()



void ExtendibleHash::insert(const int &object)
{
  if (!Directory[Ehash(object, bits)]->insert(object))
    split(object);
} // insert()



void ExtendibleHash::remove(const int &object)
{
  Directory[Ehash(object, bits)]->remove(object);
}  // remove()



const int & ExtendibleHash::find(const int &object)
{
  if (Directory[Ehash(object, bits)]->find(object))
    return object;
  // if not found return notFound;
  return not_found;
}  // find()



void ExtendibleHash::split(const int &object)
{
  // while ExtentibleLeaf split returns NULL, call the expandDong() function to expand the directory
  ExtendibleLeaf * newLeaf;

  for (newLeaf = Directory[Ehash(object, bits)]->split(object, bits); 
    !newLeaf; // while newLeaf is null; happens when sig bits are too few to make split
    newLeaf = Directory[Ehash(object, bits)]->split(object, bits))
      expandDong();

  Directory[newLeaf->getBitPattern(bits)] = newLeaf;
  size++;
}  // split()



void ExtendibleHash::expandDong()
{
  ExtendibleLeaf **NewDirectory = 
    new ExtendibleLeaf*[potentialLeafSpaces + potentialLeafSpaces];

  for (int i = 0; i < potentialLeafSpaces; i++)
  {
    NewDirectory[i + i] = Directory[i]; // save some time on that multiplication yeh
    NewDirectory[i + i + 1] = Directory[i];
  }

  Directory = NewDirectory;
  bits++;
  potentialLeafSpaces += potentialLeafSpaces;
} // expandDong()





//------------------------------
//-  ExtendibleLeaf functions  -
//------------------------------

ExtendibleLeaf::ExtendibleLeaf(int lSize) :
  LeafSize(lSize)
{
  size = 0;
  values = new int[LeafSize];
} // ExtendibleLeaf()



bool ExtendibleLeaf::insert(const int & value)
{
  if (size == LeafSize)
    return false;

  values[size] = value;
  size++;

  return true;
} // insert()



void ExtendibleLeaf::remove(const int & value)
{
  for (int i = 0; i < size; i++)
    if (value == values[i])
    {
      values[i] = values[size - 1];
      size--;
      break;
    }
} // remove()



bool ExtendibleLeaf::find(const int & value) const
{
  for (int i = 0; i < size; i++)
    if (value == values[i])
      return true;

  return false;
} // find()



ExtendibleLeaf* ExtendibleLeaf::split(const int & value, int & bits)
{
  int referenceBits = Ehash(values[0], bits);
  int i; // needs declaration here to test for successful transfer later;
  
  ExtendibleLeaf* newLeaf = NULL;

  for (i = 1; i < size; i++)
    if (referenceBits != Ehash(values[i], bits))
    {
      referenceBits = Ehash(values[i], bits);
      newLeaf = new ExtendibleLeaf(LeafSize);
      newLeaf->insert(values[i]);
      values[i] = values[size - 1];
      size--;
      i--;
      break;
    }

  if (!newLeaf) // if all of values[] bits matched, meaning newLeaf never instantiated
  {
    if (referenceBits != Ehash(value, bits))
    {
      newLeaf = new ExtendibleLeaf(LeafSize);
      newLeaf->insert(value);
    }

    return newLeaf;
  }

  for (/* i = i */; i < size; i++) // i starting from where the first for loop left off
    if (referenceBits == Ehash(values[i], bits)) // if values match the first not match
    {
      newLeaf->insert(values[i]);
      values[i] = values[size - 1];
      size--;
      i--;
    }

  if (referenceBits == Ehash(value, bits)) // check the inserted value
    newLeaf->insert(value);
  else
    this->insert(value);

  return newLeaf;
}



int ExtendibleLeaf::getBitPattern(int & bits) const
{
  if (size > 0) // should always be the case
    return Ehash(values[0], bits);

  return 0;
} // getBitPattern()
