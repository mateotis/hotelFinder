#include<iostream>
#include<string>
#include<fstream>
#include<vector>

#include "hashMap.h"

using namespace std;

long HashMap::hashCode(const string key) // Hashing function
{
	long ascSum = 0;
	for(int i = 0; i < key.length(); i++) {
		if(int(key.at(i)) <= 127 && int(key.at(i)) >= 0) { // Ignore any funky characters - the hash will still work
			ascSum += int(key.at(i)) * pow(i+1, 2); // Takes every valid character and multiplies it by the square of its position in the string
		}
	}

	return ascSum % capacity; // The compression is achieved by simply taking the modulus with the size of the map
}

void HashMap::insert(const string key, const string value, int& increment, bool& insertSuccess) // Insertion through open addressing
{
	long hash = hashCode(key);
	long ogHash = hash; // Used for collision checks
	while(true) {
		if(nodeArray[hash] == nullptr) { // If we have an empty space, put the node there
			nodeArray[hash] = new HashNode(key, value);
			size++;
			insertSuccess = true;
			return;
		}
		else if(nodeArray[hash]->isAvailable()) {
			delete nodeArray[hash]; // First we delete the object at the hash index
			nodeArray[hash] = NULL; // Then we make sure the space is freed (segfaults otherwise)
			nodeArray[hash] = new HashNode(key, value);
			size++;
			insertSuccess = true;
			return;
		}
		else if(nodeArray[hash]->getKey() == key) { // If we run into an entry with the same key, then we don't need to add it again
			cerr << key << " already in database." << endl;
			return;
		}
		else { 
			if(hash == ogHash) { // Only increments upon first collision to get the accurate count of colliding keys
				increment++;
			}

			if(hash < capacity - 1) {
				hash++; // Linear open addressing
			}
			else if(hash == capacity - 1 && size != capacity) { // If we reached the end but there's still space in the array, start looking for it from the beginning
				hash = 0;
			}
			else { // If we're at capacity, then we obviously can't insert it
				cerr << "No place in array for element " << key << endl;
				return;
			}
		}	
	}

}

string HashMap::find(const string key)
{
	long hash = hashCode(key);
	int count = 0;
	bool wrappedAround = false; // Check if we've wrapped around the array once already
	while(nodeArray[hash] != nullptr) { // Uses the same open addressing scheme as insertion, stops looking upon finding the first empty space
		count++;
		if(nodeArray[hash]->getKey() == key && nodeArray[hash]->isAvailable() == false) {
			cout << "Comparisons made in hotel table: " << count << endl;
			return nodeArray[hash]->getValue();
		}
		else {
			if(hash < capacity - 1) {
				hash++;
			}
			else if(hash == capacity - 1 && wrappedAround == false) { // Only wrap around once - if we still can't find it, then it really isn't there
				hash = 0;
				wrappedAround = true;
			}
			else {
				return "Hotel not found!";
			}
		}	
	}
	return "Hotel not found!";
}

void HashMap::remove(const string key) {
	long hash = hashCode(key);
	int count = 0;
	bool wrappedAround = false;
	while(nodeArray[hash] != nullptr) {
		count++;
		if(nodeArray[hash]->getKey() == key && nodeArray[hash]->isAvailable() == false) {
			cout << "Comparisons made in hotel table: " << count << endl;
			nodeArray[hash]->makeAvailable();
			size--;
			cout << "Deleted " << key << " from hotel table." << endl;
			return;
		}
		else {
			if(hash < capacity - 1) {
				hash++;
			}
			else if(hash == capacity - 1 && wrappedAround == false) {
				hash = 0;
				wrappedAround = true;
			}
			else {
				cerr << "Deletion failed. Hotel not found in hotel table." << endl;
				return;
			}
		}	
	}
	cerr << "Deletion failed. Hotel not found in hotel table." << endl;			
}

void HashMap::dump(string fileName) { // Sorts the table through a vector and the STL sort() method, then writes it to a file
	vector<string> values;
	for(int i = 0; i < capacity; i++) { // Iterate over the entire table and insert values into a vector
		if(nodeArray[i] != nullptr && nodeArray[i]->isAvailable() == false) {
			string value = nodeArray[i]->getValue();
			values.push_back(value);
		}
	}
	sort(values.begin(), values.end()); // A regular O(nlogn) sort - begin() and end() ensure the entire vector is sorted
	
	ofstream fout;
	fout.open(fileName);
	if(!fout) {
		cerr << "Error: file could not be opened." << endl;
		return;
	}
	int count = 0;
	for(int j = 0; j < values.size(); j++) {
		fout << values.at(j) << '\n';
		count++;
	}
	fout.close();
	cout << "Sorted and dumped " << count << " entries into " << fileName << endl;
}