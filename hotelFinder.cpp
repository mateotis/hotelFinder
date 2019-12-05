#include<iostream>
#include<fstream>
#include<vector>
#include<cmath> // For sqrt()
#include<algorithm> // For string find()
#include<chrono> // For execution time measurement
#include<list> // For the list-based implementation of the city table

using namespace std;


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
	long tableSize = lineCount*1.333; // Array is made 33.3% larger than the input size to minimise collisions
	while(true) {
		if(!isPrime(tableSize)) {
			tableSize++;
		}
		else {
			return tableSize;
		}
	}
}

vector<string> keyMaker(string param) { // Splits input to hotel and city name and stores them in a vector
	string hotelName = param.substr(0,param.find(','));
	param.erase(0,param.find(',') + 1);
	string cityName = param.substr(0,param.find(','));
	param.erase(0,param.find(',') + 1);
	vector<string> keys{hotelName, cityName};
	return keys;
}

class Hotel {
	private:
		string name;
		string city;
		string rating;
		string price;
		string country;
		string address;
	public:
		Hotel() : name(""),city(""),rating(""),price(""),country(""),address("") {} // Default constructor; necessary as HashNode inevitably calls this when constructed
		Hotel(string entry) { // Constructs the Hotel object by breaking down the input string into the variables
			this->name = entry.substr(0,entry.find(',')); // The variables are separated by commas
			entry.erase(0,entry.find(',') + 1); // The found variable is erased from the overall string, which means the comma check can be used to find the next variable
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
			//string content = "Name: " + name + '\n' + "City: " + city + '\n' + "Rating: " + rating + '\n' + "Price: " + price + '\n' + "Country: " + country + '\n' + "Address: " + address;
			string content = name + ',' + city + ',' + rating + ',' + price + ',' + country + ',' + address;
			return content;
		}
		string getName() { // Used in the city table lists to check if a hotel is already in there
			return name;
		}
};

class HashNode // Node that, with an extra variable, works with both hash tables
{
	private:
		string key; // Either hotel+city or city
		Hotel value; // Used for the main hash table
		list<Hotel> city; // Used for the city table

	public:
		HashNode(string key, string value) // Node for the regular hash table
		{
			this->key = key;
			Hotel h(value);
			this->value = h;
		}
		HashNode(string key, string value, list<Hotel> city) { // Node for the city table, its value is a list of hotels in a city
			this->key = key;
			Hotel h(value);
			this->city = city;
			this->city.push_back(h);
		}
		string getKey()
		{
			return this->key;
		}
		string getValue()
		{
			string content = this->value.getEntry(); // Passed to the Hotel class
			return content;
		}
		void listAdd(Hotel h) { // Add a hotel to a city
			for(auto it = city.begin(); it != city.end(); ++it) { // Making use of the beautiful auto type introduced in C++11, which automatically determines the right type, to initialise the iterator
				if(it->getName() == h.getName()) {
					cerr << h.getName() << " already in city table." << endl;
					return;
				}
			}
			city.push_back(h);
			//cout << "Added hotel " << h.getEntry() << " to list." << endl;
		}
		void listRemove(string name) { // Remove a hotel from the city table
			for(auto it = city.begin(); it != city.end(); ++it) {
				//cout << "in list remove iteration looking at " << it->getName() << endl;
				//cout << "looking for " << name << endl;
				if(it->getName() == name) {
					city.erase(it);
					cout << "Removed " << name << " from city table." << endl;
					return;
				}
			}
			cerr << name << " was not found in the city table." << endl;
		}
		void listPrint() {
			for(auto it = city.begin(); it != city.end(); ++it) {
				cout << it->getEntry() << "\n";
			}
			cout << "Number of hotels in city: " << city.size() << endl;
		}
		int listSize() {
			return this->city.size();
		}

};

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
		long hashCode(const string key) // Hashing function
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
		
		void insert(const string key, const string value, int& increment) // Insertion through open addressing
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

		string find(const string key)
		{
			long hash = hashCode(key);
			int count = 0;
			while(nodeArray[hash] != nullptr) { // Uses the same open addressing scheme as insertion, stops looking upon finding the first empty space
				count++;
				if(nodeArray[hash]->getKey() == key) {
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

		void remove(const string key) {
			long hash = hashCode(key);
			int count = 0;
			while(nodeArray[hash] != nullptr) {
				count++;
				if(nodeArray[hash]->getKey() == key) {
					cout << "Comparisons made: " << count << endl;
					//cityTable.remove(key);
					delete nodeArray[hash]; // First we delete the object at the hash index
					nodeArray[hash] = NULL; // Then we make sure the space is freed (segfaults otherwise)
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

		void dump(string fileName, long tableSize) { // Sorts the table through a vector and the STL sort() method, then writes it to a file
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

		int getSize()
		{
			return this->size;
		}

		~HashMap()
		{
			delete[] this->nodeArray;
		}
};

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

		void insert(const string key, const string value, int& increment) // Insertion using both separate chaining and open addressing
		{
			long hash = hashCode(key);
			long ogHash = hash;
			//cout << key << " : " << hash << endl;

			while(true) {
				if(nodeArray[hash] == nullptr) { // If we have an empty space, put the node there
					//cout << "Inserting " << key << " at " << hash << endl;
					//cout << "Data to be inserted: " << value << endl;
					list<Hotel> city; // Empty list used to identify the right hash node constructor
					nodeArray[hash] = new HashNode(key, value, city);
					size++;
					//cout << "Inserted " << key << endl;
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
						//cout << key << " can't be put at " << hash << endl;
					}
					
					hash++;
					if(hash >= capacity) {
						cout << "No place in array for element " << key << endl;
						return;
					}
				}	
			}

		}

		void find(const string key) // The main function for allinCity - finds the input city and prints the hotels in it
		{
			long hash = hashCode(key);
			int count = 0;
			while(nodeArray[hash] != nullptr) {
				count++;
				if(nodeArray[hash]->getKey() == key) { // First we find the right node
					cout << "Comparisons made: " << count << endl;
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

		void remove(const string key) { // Removal, like finding, has two steps: first we find the city the hotel is in, then we remove it from its list
			vector<string> keys = keyMaker(key); // Since we need both hotel and city name separately
			//string cityName = key.substr(key.find(',') + 1, key.end());
			long hash = hashCode(keys.at(1));
			int count = 0;
			//cout << keys.at(0) << " AND " << keys.at(1) << endl;
			while(nodeArray[hash] != nullptr) {
				//cout << "In remove loop" << endl;
				count++;
				if(nodeArray[hash]->getKey() == keys.at(1)) {
					//cout << "Found matching list" << endl;
					nodeArray[hash]->listRemove(keys.at(0)); // We pass the hotel name to the list remover
					cout << "Comparisons made: " << count << endl;
					size--;
					cout << "Deleted " + keys.at(0) << endl;
					if(nodeArray[hash]->listSize() == 0) { // If the city list is now empty, we can delete the whole node
						delete nodeArray[hash];
						nodeArray[hash] = NULL;
					}
					return;
				}
				else {
					if(hash < capacity) {
						hash++;
					}
					else {
						cerr << "Deletion failed. Hotel not found." << endl;
					}
				}	
			}
			cerr << "Deletion failed. Hotel not found." << endl;			
		}
		int getSize()
		{
			return this->size;
		}

};


int main(void)
{
	ifstream fin;
	fin.open("hotels100k.csv");
	if(!fin) {
		cout<< "Error: file could not be opened.";
		exit(-1);
	}

	string line;
	long lineCount = 0;

	getline(fin,line);
	while(!fin.eof()) { // Gets number of entries; needs to be done before we create our hash tables
		getline(fin,line);
		lineCount++;
	}

    fin.seekg(0, fin.beg); // Reset file iterator to the top
    int increment = 0; // Variables that count collisions on insertion
    int incrementCity = 0;

	long tableSize = nearestPrime(lineCount); // Calculate hash table size
	HashMap hotelTable(tableSize); // Create our two tables, both the same size
	CityMap cityTable(tableSize);

	getline(fin,line); // Skip the first line
	while(!fin.eof())
	{
		string hotelName, cityName, hotelCityKey, value;
		getline(fin,hotelName, ','); // Parse based on commas: hotel name, city name, then the rest of the value
		getline(fin,cityName, ',');
		getline(fin,value);
		hotelCityKey = hotelName + ',' + cityName; // Combined key for the main hash table
		value = hotelCityKey + ',' + value; // The whole entry
		//cout<<key<<":"<<value<<endl;
		hotelTable.insert(hotelCityKey,value, increment); // Simultaneous insertions based on different keys
		cityTable.insert(cityName,value,incrementCity);

	}
	fin.close();
    //cout << lineCount << endl;

	// Some post-insert stats
	cout<<"Hash Map size = "<<hotelTable.getSize()<<endl;
	cout << "Number of hash increments: " << increment << endl;
	cout<<"City Map size = "<<cityTable.getSize()<<endl;
	cout << "Number of hash increments: " << incrementCity << endl;

	string input;
	while(true)
	{
		cout<<"Enter command: ";
		getline(cin,input);
		string command, param;

		if(input == "quit") {
			break;
		}
		else if(input.find(' ') == string::npos) { // All input must have two parts, separated by whitespace: a command word and a parameter
			cerr << "Invalid command." << endl;
			continue;
		}
		else {
			command = input.substr(0, input.find(' ')); // First word is the command
			param = input.substr(command.length() + 1, input.length() - command.length()); // The rest of the input is the parameter			
		}

		if(command == "find") { // For find, we just pass the param (which is a hotel+city key) as is
			auto start = chrono::high_resolution_clock::now(); // Records time with the most accurate clock available

			cout << hotelTable.find(param) << endl;

			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start); // Converts recorded time to ms
			cout << "Time taken: " << duration.count() << " ms" << endl;
			//cout << param << endl;
		}
		else if(command == "add") { // For add, we first create the keys and then insert the entry into both tables
			vector<string> keys = keyMaker(param);
			//string value = key + ',' + param;
			int increment = 0;
			int incrementCity = 0;
			string hotelCityKey = keys.at(0) + ',' + keys.at(1);
			auto start = chrono::high_resolution_clock::now();

			cout << hotelCityKey << endl << param << endl;
			hotelTable.insert(hotelCityKey,param,increment);
			cityTable.insert(keys.at(1),param,incrementCity);

			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
			cout << "Time taken: " << duration.count() << " ms" << endl;
		}
		else if(command == "delete") { // For delete, both tables take the parameter as is, since even our city table needs to know which hotel to delete
			//vector<string> keys = keyMaker(param);
			auto start = chrono::high_resolution_clock::now();

			hotelTable.remove(param);
			cityTable.remove(param);

			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
			cout << "Time taken: " << duration.count() << " ms" << endl;
		}
		else if(command == "dump") { // For dump, the table size is passed to know how much to iterate over
			auto start = chrono::high_resolution_clock::now();

			hotelTable.dump(param, tableSize);

			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
			cout << "Time taken: " << duration.count() << " ms" << endl;
		}
		else if(command == "allinCity") { // For allinCity, the param (file name) is enough
			auto start = chrono::high_resolution_clock::now();

			cityTable.find(param);

			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
			cout << "Time taken: " << duration.count() << " ms" << endl;
		}
		else {
			cerr << "Invalid command." << endl;
		}
		
	}
	exit(0);
}

