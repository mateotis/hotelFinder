#include<iostream>
#include<string>
#include<vector>
#include<list>
#include<algorithm>

#include "hashMap.h"
#include "cityMap.h"

using namespace std;

void CityMap::insert(const string key, const string value, int& increment) // Insertion using both separate chaining and open addressing
{
	long hash = hashCode(key);
	long ogHash = hash;

	while(true) {
		if(nodeArray[hash] == nullptr) { // If we have an empty space, put the node there
			list<Hotel> city; // Empty list used to identify the right hash node constructor
			nodeArray[hash] = new HashNode(key, value, city);
			size++;
			return;
		}
		else if(nodeArray[hash]->isAvailable()) { // If we see an available node (aka an empty city list), delete and replace it with this
			delete nodeArray[hash];
			nodeArray[hash] = NULL;
			list<Hotel> city;
			nodeArray[hash] = new HashNode(key, value, city);
			size++;
			return;
		}
		else if(nodeArray[hash]->getKey() == key) { // If it's not a nullptr, then there must already be a list of hotels there--we have to check if it's the same city, not just the same hash
			Hotel h(value);
			nodeArray[hash]->listAdd(h); // We call the node's list addition function
			size++;
			return;
		}
		else { // If neither, linear probing
			if(hash == ogHash) {
				increment++;
			}
			
			hash++;
			if(hash >= capacity) {
				cout << "No place in array for element " << key << endl;
				return;
			}
		}	
	}

}

void CityMap::find(const string key) // The main function for allinCity - finds the input city and prints the hotels in it
{
	long hash = hashCode(key);
	int count = 0;
	while(nodeArray[hash] != nullptr) {
		count++;
		if(nodeArray[hash]->getKey() == key && nodeArray[hash]->isAvailable() == false) { // First we find the right node
			cout << "Comparisons made in city table: " << count << endl;
			nodeArray[hash]->listPrint(); // Then we print the contents of its list
			return;
		}
		else {
			if(hash < capacity) {
				hash++;
			}
			else {
				cerr << "City not found!" << endl;
				return;
			}
		}	
	}
	cerr << "City not found!" << endl;
}

void CityMap::remove(const string key) { // Removal, like finding, has two steps: first we find the city the hotel is in, then we remove it from its list
	vector<string> keys = keyMaker(key); // Since we need both hotel and city name separately
	long hash = hashCode(keys.at(1));
	int count = 0;
	while(nodeArray[hash] != nullptr) {
		count++;
		if(nodeArray[hash]->getKey() == keys.at(1) && nodeArray[hash]->isAvailable() == false) {
			if(nodeArray[hash]->listRemove(keys.at(0)) == true) { // Only report success if we actually found an element to delete
				size--;
				cout << "Deleted " + keys.at(0) << " from the city table." << endl;
			}
			cout << "Comparisons made in city table: " << count << endl;
			return;
		}
		else {
			if(hash < capacity) {
				hash++;
			}
			else {
				cerr << "Deletion failed. City not found in city table." << endl;
			}
		}	
	}
	cerr << "Deletion failed. Hotel not found in city table." << endl;			
}