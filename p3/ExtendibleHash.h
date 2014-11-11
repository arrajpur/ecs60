#ifndef EXTENDIBLE_HASH_H
  #define EXTENDIBLE_HASH_H

class ExtendibleLeaf
{
	int * values; 
	int maxSize;
	int valueSize;
public: 
	ExtendibleLeaf(int leafSize);
	bool insert(const int &object);
	int remove(int pos); 
	int search(const int &object);
	int * split(int b);
	int getValueSize();
};

class ExtendibleHash
{
  ExtendibleLeaf  **Directory;
  int bits;
  int size;
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

