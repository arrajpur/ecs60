#ifndef EXTENDIBLE_HASH_H
  #define EXTENDIBLE_HASH_H


int Ehash(int value, int bits);

class ExtendibleLeaf;

class ExtendibleHash
{
  ExtendibleLeaf  **Directory;
  int bits;
  int size;
  int potentialLeafSpaces;
  int LeafSize;
  int not_found;

public:
  ExtendibleHash(const int & notFound, int b, int LSize = 2);
  void insert(const int &object);
  void remove(const int &object);
  const int & find(const int &object);
  void split(const int &object);
  void expandDong(); // for the doubling of size thing
  // determineDirectoryPos()
  int GetBits()const { return bits; }

}; // class ExtendibleHashing





class ExtendibleLeaf
{
  int *values;
  int LeafSize;
  int size;
  // int significantBits; // the number of bits to check

public:
  ExtendibleLeaf(int lSize); // might want to take bits out of here; they don't really get used
  bool insert(const int & value);
  void remove(const int & value);
  bool find(const int & value) const;
  ExtendibleLeaf* split(const int & value, int & bits);
  int getBitPattern(int & bits) const;
}; // class ExtendibleLeaf

#include "ExtendibleHash.cpp"

#endif

