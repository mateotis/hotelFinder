#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<list>

//#include "hotelNode.h"

using namespace std;

class CityMap: public HashMap { // City table used for allinCity command - derived from the main hash map
	private:
		HashNode **nodeArray;
		int size;
		int capacity;

	public:
		CityMap(int capacity) : HashMap(capacity) // Same constructor
		{
			nodeArray = new HashNode*[capacity];
			this->capacity = capacity;
			this->size = 0;
		}
		void insert(const string key, const string value, int& increment);
		void find(const string key);
		void remove(string key);
		int getSize()
		{
			return this->size;
		}

};
