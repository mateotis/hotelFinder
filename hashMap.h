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
		void dump(string fileName);

		int getSize()
		{
			return this->size;
		}

		void clear() // Releases all dynamically acquired memory; called when quitting the program
		{
			int cnt = 0;
			for(int i = 0; i < capacity; i++) {
				if(nodeArray[i] != nullptr) { // Finds every node, even 'available' ones, since they technically still take up memory
					delete nodeArray[i];
					nodeArray[i] = NULL;
					cnt++;
				}
			}
			delete[] this->nodeArray; // After every node was deleted, we can delete the array itself
			cout << "Deleted " << cnt << " hotel nodes." << endl;
		}
};

#endif