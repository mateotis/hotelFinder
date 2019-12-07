#ifndef CITYMAP_H
#define CITYMAP_H

#include<iostream>
#include<string>
#include<vector>
#include<list>

#include "hotelFinder.h"

using namespace std;

class CityMap: public HashMap { // City table used for allinCity command - derived from the main hash map; uses its hash function
	private:
		HashNode **nodeArray;
		int size;
		int capacity;

	public:
		CityMap(int capacity) : HashMap(capacity) // Calls base constructor
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
		~CityMap()
		{
			int cnt = 0;
			for(int i = 0; i < capacity; i++) {
				if(nodeArray[i] != nullptr) {
					delete nodeArray[i];
					nodeArray[i] = NULL;
					cnt++;
				}
			}
			delete[] this->nodeArray;
		}

};

#endif