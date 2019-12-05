#ifndef HOTELFINDER_H
#define HOTELFINDER_H

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
		void listAdd(Hotel h);
		void listRemove(string name);
		void listPrint();
		int listSize();

};

bool isPrime(long number);
long nearestPrime(long lineCount);
vector<string> keyMaker(string param);

#endif