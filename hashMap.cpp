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
		//cout << int(key.at(i)) << endl;
		if(int(key.at(i)) <= 127 && int(key.at(i)) >= 0) { // Ignore any funky characters - the hash will still work
			//cout << "ASCII value of character: " << int(key.at(i)) << endl;
			ascSum += int(key.at(i)) * pow(i+1, 2); // Takes every valid character and multiplies it by the square of its position in the string
		}
	}

	//cout << "Initial hash: " << ascSum % capacity << endl;
	//cout << capacity << endl;
	return ascSum % capacity; // The compression is achieved by simply taking the modulus with the size of the map
}

void HashMap::insert(const string key, const string value, int& increment) // Insertion through open addressing
{
	long hash = hashCode(key);
	long ogHash = hash; // Used for collision checks
	while(true) {
		if(nodeArray[hash] == nullptr) { // If we have an empty space, put the node there
			//cout << "Inserting " << key << " at " << hash << endl;
			//cout << "Data to be inserted: " << value << endl;
			nodeArray[hash] = new HashNode(key, value);
			size++;
			//cout << "Inserted " << key << endl;
			return;
		}
		else if(nodeArray[hash]->isAvailable()) {
			delete nodeArray[hash]; // First we delete the object at the hash index
			nodeArray[hash] = NULL; // Then we make sure the space is freed (segfaults otherwise)
			nodeArray[hash] = new HashNode(key, value);
			size++;
			return;
		}
		else if(nodeArray[hash]->getKey() == key) { // If we run into an entry with the same key, then we don't need to add it again
			cerr << key << " already in database." << endl;
			return;
		}
		else { 
			if(hash == ogHash) { // Only increments upon first collision to get the accurate count of colliding keys
				increment++;
				//cout << key << " can't be put at " << hash << endl;
			}

			if(hash < capacity) {
				hash++; // Linear open addressing
			}
			else { // If we reach the end of the array without an open space, we can't insert it
				cout << "No place in array for element " << key << endl;
				return;
			}
		}	
	}

}

string HashMap::find(const string key)
{
	long hash = hashCode(key);
	int count = 0;
	while(nodeArray[hash] != nullptr) { // Uses the same open addressing scheme as insertion, stops looking upon finding the first empty space
		count++;
		if(nodeArray[hash]->getKey() == key && nodeArray[hash]->isAvailable() == false) {
			cout << "Comparisons made: " << count << endl;
			return nodeArray[hash]->getValue();
		}
		else {
			if(hash < capacity) {
				hash++;
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
	while(nodeArray[hash] != nullptr) {
		count++;
		if(nodeArray[hash]->getKey() == key) {
			cout << "Comparisons made: " << count << endl;
			//cityTable.remove(key);
			nodeArray[hash]->makeAvailable();
			//delete nodeArray[hash]; // First we delete the object at the hash index
			//nodeArray[hash] = NULL; // Then we make sure the space is freed (segfaults otherwise)
			size--;
			cout << "Deleted " + key << endl;
			return;
		}
		else {
			if(hash < capacity) {
				hash++;
			}
			else {
				cerr << "Deletion failed. Hotel not found." << endl;
				return;
			}
		}	
	}
	cerr << "Deletion failed. Hotel not found." << endl;			
}

void HashMap::dump(string fileName, long tableSize) { // Sorts the table through a vector and the STL sort() method, then writes it to a file
	vector<string> values;
	for(int i = 0; i < tableSize; i++) { // Iterate over the entire table and insert values into a vector
		if(nodeArray[i] != nullptr) {
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
		//cout << values.at(j) << endl;
	}
	fout.close();
	cout << "Sorted and dumped " << count << " entries into " << fileName << endl;
}