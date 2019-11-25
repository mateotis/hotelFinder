#include<iostream>
#include<fstream>
#include<cmath>

using namespace std;

class Hotel {
	private:
		string name;
		string city;
		string rating;
		string price;
		string country;
		string address;
	public:
		Hotel() : name(""),city(""),rating(""),price(""),country(""),address("") {} // Dummy initialiser to get the HashNode initialisation working
		Hotel(string entry) { // Constructs the Hotel object by breaking down the input string into the variables
			this->name = entry.substr(0,entry.find(',')); // The variables are separated by commas
			entry.erase(0,entry.find(',') + 1);
			this->city = entry.substr(0,entry.find(','));
			entry.erase(0,entry.find(',') + 1);
			this->rating = entry.substr(0,entry.find(','));
			entry.erase(0,entry.find(',') + 1);
			this->price = entry.substr(0,entry.find(','));
			entry.erase(0,entry.find(',') + 1);
			this->country = entry.substr(0,entry.find(','));
			entry.erase(0,entry.find(',') + 1);
			this->address = entry.substr(0,entry.find(','));
			entry.erase(0,entry.find(',') + 1);
		}
		string getEntry() {
			string content = "Name: " + name + '\n' + "City: " + city + '\n' + "Rating: " + rating + '\n' + "Price: " + price + '\n' + "Country: " + country + '\n' + "Address: " + address;
			return content;
		}
};

class HashNode
{
	private:
		string key;
		Hotel value;
	public:
		HashNode(string key, string value)
		{
			this->key = key;
			Hotel h(value);
			this->value = h;
		}
		string getKey()
		{
			return this->key;
		}
		string getValue()
		{
			//string content = "Name: " + this->value.name + '\n' + "City: " + this->value.city + '\n' + "Rating: " + this->value.rating + '\n' + "Price: " + this->value.price + '\n' + "Country: " + this->value.country + '\n' + "Address: " + this->value.address;
			string content = this->value.getEntry();
			return content;
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

		string find(const string key)
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

		string remove(const string key) {
			long hash = hashCode(key);
			int count = 0;
			while(nodeArray[hash] != nullptr) {
				count++;
				if(nodeArray[hash]->getKey() == key) {
					cout << endl << "Comparisons made: " << count << endl;
					delete nodeArray[hash];
					nodeArray[hash] = NULL; // To make sure the space is freed
					size--;
					return "Deleted " + key;
				}
				else {
					if(hash < capacity) {
						hash++;
					}
					else {
						return "Deletion failed. Hotel not found.";
					}
				}	
			}
			return "Deletion failed. Hotel not found.";			
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
	HashMap hotelTable(tableSize);
	getline(fin,line);  //skip first line
	while(!fin.eof())
	{
		string hotelName, cityName, key, value;
		getline(fin,hotelName, ',');
		getline(fin,cityName, ',');
		getline(fin,value);
		key = hotelName + ',' + cityName;
		value = key + ',' + value;
		//cout<<key<<":"<<value<<endl;
		hotelTable.insert(key,value);

	}
	fin.close();
    cout << lineCount << endl;
   	cout << tableSize << endl;

	cout<<"Hash Map size = "<<hotelTable.getSize()<<endl;

	string input;
	while(true)
	{
		cout<<"Enter command: ";
		getline(cin,input);
		string command, param;
		if(input == "quit") {
			break;
		}
		else if(input.find(' ') == string::npos) {
			cerr << "Invalid command." << endl;
			continue;
		}
		else {
			command = input.substr(0, input.find(' ')); // First word is the command
			param = input.substr(command.length() + 1, input.length() - command.length()); // Second word is the parameter			
		}

		if(command == "find") {
			cout << hotelTable.find(param) << endl;
			cout << param << endl;
		}
		else if(command == "add") {
			string hotelName = param.substr(0,param.find(','));
			param.erase(0,param.find(',') + 1);
			string cityName = param.substr(0,param.find(','));
			param.erase(0,param.find(',') + 1);
			string key = hotelName + ',' + cityName;
			string value = key + ',' + param;
			hotelTable.insert(key,value);
			cout << key << " inserted." << endl;
		}
		else if(command == "delete") {
			string hotelName = param.substr(0,param.find(','));
			param.erase(0,param.find(',') + 1);
			string cityName = param.substr(0,param.find(','));
			param.erase(0,param.find(',') + 1);
			string key = hotelName + ',' + cityName;
			cout << hotelTable.remove(key) << endl;
		}
		else if(command == "dump") {
			//TODO
		}
		else if(command == "allinCity") {
			//TODO
		}
		else {
			cerr << "Invalid command." << endl;
		}
		
		//cout<<"Information for "<<input<<" is: "<<hotelTable.insert(input)<<endl;
		//cout << hotelTable.remove(input) << endl;
	}
	exit(0);
}

