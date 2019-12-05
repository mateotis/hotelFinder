#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<cmath> // For sqrt()
#include<algorithm> // For string find()
#include<chrono> // For execution time measurement
#include<list> // For the list-based implementation of the city table

#include "hashMap.h"
#include "cityMap.h"
//#include "hotelNode.h"

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

