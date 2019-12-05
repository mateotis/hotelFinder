#pragma once

#include<iostream>
#include<string>
#include<algorithm> // For string find()
#include<list> // For the list-based implementation of the city table

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