#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<cmath> // For sqrt()
#include<algorithm> // For string find()
#include<chrono> // For execution time measurement
#include<list> // For the list-based implementation of the city table

#include "hotelFinder.h"
#include "hashMap.h"
#include "cityMap.h"

using namespace std;

// Hash node list methods
void HashNode::listAdd(Hotel h) { // Add a hotel to a city
	for(auto it = city.begin(); it != city.end(); ++it) { // Making use of the beautiful auto type introduced in C++11, which automatically determines the right type, to initialise the iterator
		if(it->getName() == h.getName()) {
			cerr << h.getName() << " already in city table." << endl;
			return;
		}
	}
	city.push_back(h); // Only add it if we didn't already find it
}

bool HashNode::listRemove(string name) { // Remove a hotel from the city table
	for(auto it = city.begin(); it != city.end(); ++it) {
		if(it->getName() == name) {
			city.erase(it);
			if(city.empty() == true) { // If the city list is now empty, make the node available for overwriting
				this->available = true;
			}
			return true;
		}
	}
	cerr << name << " was not found in the city table." << endl;
	return false;
}

void HashNode::listPrint() {
	for(auto it = city.begin(); it != city.end(); ++it) {
		cout << it->getEntry() << "\n";
	}
	cout << "Number of hotels in city: " << city.size() << endl;
}

// Utility functions
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
	long tableSize = lineCount*1.333 + 1; // Array is made 33.3% larger than the input size to minimise collisions; +1 is added to ensure we find the larger prime
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

int main(int argc, char* args[])
{

	string inputFile = "";
	for(int i = 0; i < argc; i++) {
		string str(args[i]);
		if (str == "-f") {
			string inputFileStr(args[i+1]); // The parameter after -f is the input file name
			inputFile = inputFileStr; // Since inputFileStr is only valid within the loop
		}
	}

	if(inputFile == "") {
		cerr << "Please specify an input file." << endl;
		return -1;
	}

	ifstream fin;
	fin.open(inputFile);
	if(!fin) {
		cerr << "Error: file could not be opened." << endl;
		return -1;
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
		bool insertSuccess = false; // Passed to the hotel table insert; set to true if element is successfully inserted
		bool printComps = false; // Doesn't print comparison numbers for initial insertions to avoid spam

		getline(fin,hotelName, ','); // Parse based on commas: hotel name, city name, then the rest of the value
		getline(fin,cityName, ',');
		getline(fin,value);

		hotelCityKey = hotelName + ',' + cityName; // Combined key for the main hash table
		value = hotelCityKey + ',' + value; // The whole entry

		hotelTable.insert(hotelCityKey,value,increment,insertSuccess,printComps);
		if(insertSuccess == true) { // Only try inserting into the city table if we could insert in the hotel table; this ensures sync between the two
			cityTable.insert(cityName,value,incrementCity,printComps);
		}
	}
	fin.close();

	// Some post-insert stats
	cout<< "Hotel table size: " <<hotelTable.getSize()<<endl;
	cout << "Number of hash increments in hotel table: " << increment << endl;
	cout<< "City table size: " <<cityTable.getSize()<<endl;
	cout << "Number of hash increments in city table: " << incrementCity << endl;

	string input;
	while(true)
	{
		cout<<"Enter command: ";
		getline(cin,input);
		string command, param;

		if(input == "quit") { // On returning, the destructors for hotelTable and cityTable are called automatically, which releases dynamically acquired memory
			return 0;
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
		}
		else if(command == "add") { // For add, we first create the keys and then insert the entry into both tables
			vector<string> keys = keyMaker(param);
			bool insertSuccess = false;
			bool printComps = true; // Prints comparison number on manual insertion
			string hotelCityKey = keys.at(0) + ',' + keys.at(1);
			auto start = chrono::high_resolution_clock::now();

			hotelTable.insert(hotelCityKey,param,increment,insertSuccess,printComps);
			if(insertSuccess == true) {
				cityTable.insert(keys.at(1),param,incrementCity,printComps);
			}

			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
			cout << "Time taken: " << duration.count() << " ms" << endl;
		}
		else if(command == "delete") { // For delete, both tables take the parameter as is, since even our city table needs to know which hotel to delete
			auto start = chrono::high_resolution_clock::now();

			hotelTable.remove(param);
			cityTable.remove(param);

			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
			cout << "Time taken: " << duration.count() << " ms" << endl;
		}
		else if(command == "dump") { // For dump, the param is the file name to dump the database into
			auto start = chrono::high_resolution_clock::now();

			hotelTable.dump(param);

			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
			cout << "Time taken: " << duration.count() << " ms" << endl;
		}
		else if(command == "allinCity") { // For allinCity, the param (city name) is enough
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
	return 0;
}

