#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<chrono>
#include<functional>
#include<list>

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

vector<string> keyMaker(string param) {
	string hotelName = param.substr(0,param.find(','));
	param.erase(0,param.find(',') + 1);
	string cityName = param.substr(0,param.find(','));
	param.erase(0,param.find(',') + 1);
	string key = hotelName + ',' + cityName;
	vector<string> keys{hotelName, cityName}; // Storing the two keys we need (hotel+city and city) in a vector
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
			//string content = "Name: " + name + '\n' + "City: " + city + '\n' + "Rating: " + rating + '\n' + "Price: " + price + '\n' + "Country: " + country + '\n' + "Address: " + address;
			string content = name + ',' + city + ',' + rating + ',' + price + ',' + country + ',' + address;
			return content;
		}
		string getName() {
			return name;
		}
		bool operator==(const Hotel& h) { // Overloaded == to make STL list remove() work 
			return name == h.name && city == h.city;
		}
};

class HashNode
{
	private:
		string key;
		Hotel value;
		list<Hotel> city;

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
			//numCities++;
			//cout << "Number of cities: " << numCities << endl;
			//this->value = city;
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
		void listAdd(Hotel h) {
			for(auto it = city.begin(); it != city.end(); ++it) {
				if(it->getName() == h.getName()) {
					cerr << h.getName() << " already in city table." << endl;
					return;
				}
			}
			city.push_back(h);
			//cout << "Added hotel " << h.getEntry() << " to list." << endl;
		}
		void listRemove(string name) {
			for(auto it = city.begin(); it != city.end(); ++it) {
				cout << "in list remove iteration looking at " << it->getName() << endl;
				cout << "looking for " << name << endl;
				if(it->getName() == name) {
					city.erase(it);
					cout << "Removed " << name << " from city table." << endl;
					break;
				}
			}			
		}
		void listPrint() {
			for(auto c : city) {
				cout << c.getEntry() << "\n";
			}
			cout << "Number of hotels in city: " << city.size() << endl;
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
					//cout << "ASCII value of character: " << int(key.at(i)) << endl;
					ascSum += int(key.at(i)) * pow(i+1, 2);
				}
				else {
					ascSum += 37;
				}
			}

			//cout << "Initial hash: " << ascSum % capacity << endl;
			//cout << capacity << endl;
			return ascSum % capacity;
		}
		
		void insert(const string key, const string value, int& increment)
		{
			// Insert the key and value in Hash Map using Open Addressing Linear Probing
			long hash = hashCode(key);
			long ogHash = hash;
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
					//cerr << key << " already in database." << endl;
					return;
				}
				else { 
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

		void remove(const string key) {
			long hash = hashCode(key);
			int count = 0;
			while(nodeArray[hash] != nullptr) {
				count++;
				if(nodeArray[hash]->getKey() == key) {
					cout << endl << "Comparisons made: " << count << endl;
					
					//cityTable.remove(key);
					delete nodeArray[hash];
					nodeArray[hash] = NULL; // To make sure the space is freed
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
					}
				}	
			}
			cerr << "Deletion failed. Hotel not found." << endl;			
		}

		void dump(string fileName, long tableSize) {
			vector<string> values;
			for(int i = 0; i < tableSize; i++) {
				if(nodeArray[i] != nullptr) {
					string value = nodeArray[i]->getValue();
					values.push_back(value);
				}
			}
			sort(values.begin(), values.end());
			ofstream fout;
			fout.open(fileName);
			if(!fout){
				cerr << "Error: File could not be opened." << endl;
				return;
			}
			int count = 0;
			for(int j = 0; j < values.size(); j++) {
				fout << values.at(j) << '\n';
				count++;
				//cout << values.at(j) << endl;
			}
			fout.close();
			cout << "Sorted and dumped database into " << fileName << endl;
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

class CityMap: public HashMap {
	private:
		HashNode **nodeArray;		// Array of Pointers to Hash Nodes
		int size;					//Current Size of HashMap
		int capacity;				// Total Capacity of HashMap

	public:
		CityMap(int capacity) : HashMap(capacity)
		{
			nodeArray = new HashNode*[capacity];
			this->capacity = capacity;
			this->size = 0;
		} 

		void insert(const string key, const string value, int& increment)
		{
			// Insert the key and value in Hash Map using linked lists
			long hash = hashCode(key);
			long ogHash = hash;
			cout << key << " : " << hash << endl;

			while(true) {
				if(nodeArray[hash] == nullptr) { // If we have an empty space, put the node there
					//cout << "Inserting " << key << " at " << hash << endl;
					//cout << "Data to be inserted: " << value << endl;
					list<Hotel> city;
					nodeArray[hash] = new HashNode(key, value, city);
					size++;
					//cout << "Inserted " << key << endl;
					return;
				}
				else if(nodeArray[hash]->getKey() == key) { // If it's not a nullptr, then there must already be a list of hotels there--we have to check if it's the same city, not just the same hash
					Hotel h(value);
					nodeArray[hash]->listAdd(h);
					size++;
					//cerr << key << " already in database." << endl;
					return;
				}
				else { 
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
		string find(const string key)
		{
			// Search for a key in HashMap and return its value
			long hash = hashCode(key);
			int count = 0;
			while(nodeArray[hash] != nullptr) {
				count++;
				if(nodeArray[hash]->getKey() == key) {
					cout << endl << "Comparisons made: " << count << endl;
					nodeArray[hash]->listPrint();
					return key + " found.";
				}
				else {
					if(hash < capacity) {
						hash++;
					}
					else {
						return "City not found!";
					}
				}	
			}
			return "City not found!";
		}
		void remove(const string key) {
			vector<string> keys = keyMaker(key);
			//string cityName = key.substr(key.find(',') + 1, key.end());
			long hash = hashCode(keys.at(1));
			int count = 0;
			cout << keys.at(0) << " AND " << keys.at(1) << endl;
			while(nodeArray[hash] != nullptr) {
				cout << "In remove loop" << endl;
				count++;
				if(nodeArray[hash]->getKey() == keys.at(1)) {
					cout << "Found matching list" << endl;
					nodeArray[hash]->listRemove(keys.at(0));
					cout << endl << "Comparisons made: " << count << endl;
					size--;
					cout << "Deleted " + keys.at(0) << endl;
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

    fin.seekg(0, fin.beg); // Reset file iterator to the top
    int increment = 0;
    int incrementCity = 0;

	long tableSize = nearestPrime(lineCount); // Calculate hash table size
	HashMap hotelTable(tableSize);
	CityMap cityTable(tableSize);
	getline(fin,line);  //skip first line
	while(!fin.eof())
	{
		string hotelName, cityName, hotelCityKey, value;
		getline(fin,hotelName, ',');
		getline(fin,cityName, ',');
		getline(fin,value);
		hotelCityKey = hotelName + ',' + cityName;
		value = hotelCityKey + ',' + value;
		//cout<<key<<":"<<value<<endl;
		//cout << endl << "before main insert" << endl;
		hotelTable.insert(hotelCityKey,value, increment);
		//cout << endl << "between main insert" << endl;
		cityTable.insert(cityName,value,incrementCity);
		//cout << endl << "after main insert" << endl;

	}
	fin.close();
    //cout << lineCount << endl;

	cout<<"Hash Map size = "<<hotelTable.getSize()<<endl;
	cout << "Number of hash increments: " << increment << endl;
	cout<<"City Map size = "<<cityTable.getSize()<<endl;
	cout << "Number of hash increments: " << incrementCity << endl;

	//hotelTable.dump("sortedList.txt", tableSize);

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
			auto start = chrono::high_resolution_clock::now(); // Records time with the most accurate clock available
			cout << hotelTable.find(param) << endl;
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start); // Converts recorded time to ms
			cout << "Time taken: " << duration.count() << " ms" << endl;
			//cout << param << endl;
		}
		else if(command == "add") {
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
		else if(command == "delete") {
			//vector<string> keys = keyMaker(param);
			auto start = chrono::high_resolution_clock::now();
			hotelTable.remove(param);
			cityTable.remove(param);
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
			cout << "Time taken: " << duration.count() << " ms" << endl;
		}
		else if(command == "dump") {
			auto start = chrono::high_resolution_clock::now();
			hotelTable.dump(param, tableSize);
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
			cout << "Time taken: " << duration.count() << " ms" << endl;
		}
		else if(command == "allinCity") {
			auto start = chrono::high_resolution_clock::now();
			cout << cityTable.find(param) << endl;
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
			cout << "Time taken: " << duration.count() << " ms" << endl;
		}
		else {
			cerr << "Invalid command." << endl;
		}
		
		//cout<<"Information for "<<input<<" is: "<<hotelTable.insert(input)<<endl;
		//cout << hotelTable.remove(input) << endl;
	}
	exit(0);
}

