#ifndef HASHMAP_H
#define HASHMAP_H

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<cmath> // For pow()

#include "hotelFinder.h"

using namespace std;

class HashMap
{
	private:
		HashNode **nodeArray; // The container for our hash map: an array of node pointers
		int size;
		int capacity;
	public:
		HashMap(int capacity)
		{
			nodeArray = new HashNode*[capacity];
			this->capacity = capacity;
			this->size = 0;
		} 
		long hashCode(const string key);
		void insert(const string key, const string value, int& increment);
		string find(const string key);
		void remove(const string key);
		void dump(string fileName, long tableSize);

		int getSize()
		{
			return this->size;
		}

		~HashMap()
		{
			delete[] this->nodeArray;
		}
};

#endif