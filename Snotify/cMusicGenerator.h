#ifndef _cMusicGenerator_HG_
#define _cMusicGenerator_HG_

#include "cSong.h"
#include <iostream>
#include <fstream>
#include <sstream>

class VectorSong {
private:
	cSong* arr;
	int capacity;
	int current;

public:
	VectorSong() {
		arr = new cSong[1];
		capacity = 1;
		current = 0;
	};

	~VectorSong() {
		delete[] arr;
	};

	void pushBack(cSong data) {
		if (current == capacity) {
			cSong* temp = new cSong[2 * capacity];

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

	cSong* get(int index) {
		cSong* pSong = new cSong();
		if (index < current) {
			pSong = &arr[index];
			return pSong;

		}
	}

	bool updateSong(cSong* newData) {
		for (int i = 0; i < capacity; i++) {
			if (newData->getUniqueID() <= arr[i].getUniqueID()) {
				arr[i] = (*newData);
				return true;
			}
		}

		return false;
	}

	void pop() { current--; }

	int size() { return current; }

	int getcapacity() { return capacity; }
};

class cMusicGenerator {
private:
	VectorSong* vMusic;

	std::string errorString;
public:
	cMusicGenerator();
	~cMusicGenerator();

	bool LoadMusicInformation(std::string musicFileName, std::string& errorString);

	cSong* getRandomSong(void);
	
	VectorSong* readCSVFile(std::string filePath);

	// Returns 0, NULL, or nullptr if no song is found
	// This must be an EXACT MATCH for what's in the billboard100 file. 
	// So case sensitive, etc. 
	cSong* findSong(std::string songName, std::string artist);
};

#endif
