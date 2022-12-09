#ifndef _cPersonGenerator_HG_
#define _cPersonGenerator_HG_

#include <string>
#include "cPerson.h"
#include <iostream>

class VectorString {
private:
	std::string* arr;
	int capacity;
	int current;

public:
	VectorString() {
		arr = new std::string[1];
		capacity = 1;
		current = 0;
	};

	~VectorString() {
		delete[] arr;
	};

	void pushBack(std::string data) {
		if (current == capacity) {
			std::string* temp = new std::string[2 * capacity];

			for (int i = 0; i < capacity; i++) {
				temp[i] = arr[i];
			}

			delete[] arr;
			capacity *= 2;
			arr = temp;
		}

		arr[current] = data;
		current++;
	}

	std::string get(int index) {
		if (index < current)
			return arr[index];
	}

	void pop() { current--; }

	int size() { return current; }

	int getcapacity() { return capacity; }

	void print() {
		for (int i = 0; i < current; i++) {
			std::cout << arr[i] << " ";
		}
		std::cout << std::endl;
	}
};

class cPersonGenerator {
private:
	VectorString* vNames;
	VectorString* vGender;
	VectorString* vSurnames;

	VectorString* vStreetName;
	VectorString* vStreetType;
	VectorString* vStreetDirection;

	std::string errorString;
public:
	cPersonGenerator();
	~cPersonGenerator();

	// This takes three files:
	// - One of the "Popular Baby Names" found here: https://www.ssa.gov/oact/babynames/limits.html
	//   (example: "yob2001.txt" which are babies born in the year 2001
	// - The "Names_2010Census.csv" file, from:
	//   "Frequently Occurring Surnames from the 2010 Census" from here:
	//   https://www.census.gov/topics/population/genealogy/data/2010_surnames.html
	// - The "Street_Names.csv" file, from:
	//   "Street Names (San Francisco)" from here: 
	//   https://catalog.data.gov/dataset/street-names-37fec/resource/d655cc82-c98f-450a-b9bb-63521c870503
	// Returns:
	// - true of successful
	// - false if there's any issue (can't find file, or whatever)
	//   If false, then the "errorString" (passed by reference) should be loaded with some sort of 
	//   description of what went wrong (file didn't open, parse issue, Skynet became self aware, etc.)
	//   Use your discretion (and creativity) for this. 
	bool LoadCensusFiles(
		std::string babyNameFile,
		std::string surnameFile,
		std::string streetNameFile,
		std::string& errorString);

	VectorString* readCSVFile(std::string filePath, int tokenId);

	unsigned int getNumberOfNamesLoaded(void);
	unsigned int getNumberOfSurnamesLoaded(void);
	unsigned int getNumberOfStreetsLoaded(void);

	// Randomly generates a person from the data
	// Returns 0, NULL, or nullptr if it can't do this. 
	cPerson* generateRandomPerson(void);
};


#endif 
