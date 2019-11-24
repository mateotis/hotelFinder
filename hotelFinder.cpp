#include<iostream>
#include<fstream>
#include<cmath>

using namespace std;
class HashNode
{
	private:
		string key;
		string value;
	public:
		HashNode(string key, string value)
		{
			this->key = key;
			this->value = value;
		}
		string getKey()
		{
			return this->key;
		}
		string getValue()
		{
			return this->value;
		}
		void setValue(string value)
		{
			this->value = value;
		}

};

class HashMap
{
	private:
		HashNode **nodeArray;		// Array of Pointers to Hash Nodes
		int size;					//Current Size of HashMap
		int capacity;				// Total Capacity of HashMap
	public:
		HashMap(int capacity)
		{
			nodeArray = new HashNode*[capacity];
			this->capacity = capacity;
			this->size = 0;
		} 
		long hashCode(const string key)
		{
			//Create Hashcode for key
			long ascSum = 0;
			for(int i = 0; i < key.length(); i++) {
				//cout << int(key.at(i)) << endl;
				if(int(key.at(i)) <= 127 && int(key.at(i)) >= 0) { // Ignore any funky characters - the hash will still work
					ascSum += int(key.at(i)) * pow(i, 2);
				}
				else {
					continue;
				}
			}

			return ascSum % capacity;
		}
		
		void insert(const string key, const string value)
		{
			// Insert the key and value in Hash Map using Open Addressing Linear Probing
			long hash = hashCode(key);
			while(true) {
				if(nodeArray[hash] == nullptr) {
					//cout << "Inserting " << key << " at " << hash << endl;
					nodeArray[hash] = new HashNode(key, value);
					size++;
					break;
				}
				else { hash++; }	
			}

		}

		string search(const string key)
		{
			// Search for a key in HashMap and return its value
			long hash = hashCode(key);
			int count = 0;
			while(nodeArray[hash] != nullptr) {
				count++;
				if(nodeArray[hash]->getKey() == key) {
					cout << endl << "Comparisons made: " << count << endl;
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

		int getSize()
		{
			return this->size;
		}

		~HashMap()
		{
			delete[] this->nodeArray;
		}
};

bool isPrime(long number) { // Checks whether number is a prime through the square root primality test
	if(number <= 2) {
		return false;
	}
	else {
		for(int i = 2; i <= sqrt(number); i++) {
			if(number % i == 0) {
				return false;
			}
		}
		return true;
	}
}

long nearestPrime(long lineCount) { // Finds the nearest larger prime to a number
	long tableSize = lineCount*1.333;
	while(true) {
		if(!isPrime(tableSize)) {
			tableSize++;
		}
		else {
			return tableSize;
		}
	}
}

int main(void)
{
	ifstream fin;
	fin.open("hotels10k.csv");
	if(!fin){
		cout<<"Can not open the file...!";
		exit(-1);
	}

	string line;
	long lineCount = 0;

	getline(fin,line);
	while(!fin.eof()) { // Gets number of entry lines; used when calculating hash table size
		getline(fin,line);
		lineCount++;
	}

    fin.seekg (0, fin.beg); // Reset file iterator to the top

	long tableSize = nearestPrime(lineCount); // Calculate hash table size
	HashMap myHashMap(tableSize);
	getline(fin,line);  //skip first line
	while(!fin.eof())
	{
		string key, value;
		getline(fin,key, ',');
		getline(fin,value);
		cout<<key<<":"<<value<<endl;
		myHashMap.insert(key,value);

	}
	fin.close();
    cout << lineCount << endl;
   	cout << tableSize << endl;

	cout<<"Hash Map size = "<<myHashMap.getSize()<<endl;

	string choice;
	while(true)
	{
		cout<<"Enter Hotel Name:";
		getline(cin,choice);
		//cout << myHashMap.hashCode(choice) << endl;
		if (choice == "exit")
			break;
		
		cout<<"Information for "<<choice<<" is: "<<myHashMap.search(choice)<<endl;
	}
	exit(0);
}

