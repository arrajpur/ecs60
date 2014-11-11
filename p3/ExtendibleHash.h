#ifndef EXTENDIBLE_HASH_H
  #define EXTENDIBLE_HASH_H

class ExtendibleLeaf
{
	int * values; // values, stored as numerals or bitwise?
	// some sort of "am I full" variable
	// possibly LeafSize? 
	int maxSize;
	int valueSize;
public: // are the const int &object params right?
	ExtendibleLeaf(int leafSize);
	bool insert(const int &object);
	int remove(int pos); 
	int search(const int &object);
	int * split(int b);
};

class ExtendibleHash
{
  ExtendibleLeaf  **Directory;
  int bits;
  int size; // what is this used for? // size of Directory probably
  int LeafSize;
  int minusone;

public:
  ExtendibleHash(const int & notFound, int s, int LSize = 2);
  void insert(const int &object);
  void remove(const int &object);
  const int & find(const int &object);
  void split(const int &object);
  int GetBits()const {return bits;}
}; // class ExtendibleHashing

#include "ExtendibleHash.cpp"

#endif

