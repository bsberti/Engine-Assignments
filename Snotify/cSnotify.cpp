#include "cSnotify.h"

cSnotify::cSnotify() {
	mSnotifyUsers = new UserMap();
	vSnotifyMusic = new VectorSong();
}

cSnotify::~cSnotify() {
	//delete[] mSnotifyUsers;
	//delete[] vSnotifyMusic;
}

bool cSnotify::AddUser(cPerson* pPerson, std::string& errorString) {

	if (pPerson->first == "" || pPerson->last == "" || 
		pPerson->streetName == "" || pPerson->age <= 0 ||
		pPerson->gender == -1 || pPerson->SIN == 0) {
		errorString = "Missing Person information, please review.";
		return false;
	}
	else if (FindUserBySnotifyID(pPerson->getSnotifyUniqueUserID())) {
		errorString = "Person already exist on Snotify";
		return false;
	}

	mSnotifyUsers->pushBack(pPerson);
	return true;
}

bool cSnotify::UpdateUser(cPerson* pPerson, std::string& errorString) {
	if (!FindUserBySnotifyID(pPerson->getSnotifyUniqueUserID())) {
		errorString = "Person does not exist on Snotify";
		return false;
	}

	int currentPersonIndex = mSnotifyUsers->getMapIndex(pPerson->getSnotifyUniqueUserID());
	if (mSnotifyUsers->updatePerson(pPerson, currentPersonIndex)) {
		return true;
	}
	else {
		errorString = "Person ID does not exist on Snotify";
		return false;
	}
}

bool cSnotify::DeleteUser(unsigned int SnotifyUserID, std::string& errorString) {
	cPerson* currentPerson = FindUserBySnotifyID(SnotifyUserID);
	if (currentPerson == nullptr) {
		errorString = "Person does not exist on Snotify";
		return false;
	}

	int currentPersonIndex = mSnotifyUsers->getMapIndex(SnotifyUserID);
	currentPerson->deleted = true;
	if (mSnotifyUsers->updatePerson(currentPerson, currentPersonIndex)) {
		return true;
	}
	else {
		errorString = "Error on finding user on mapUsers";
		return false;
	}
}

bool cSnotify::AddSong(cSong* pSong, std::string& errorString) {
	if (pSong->artist == "" || pSong->name == "" ||
		pSong->getUniqueID() == 0) {
		errorString = "Missing Song information, please review.";
		return false;
	}
	else if (FindSong(pSong->getUniqueID())) {
		errorString = "Song already exist on Snotify";
		return false;
	}

	vSnotifyMusic->pushBack((*pSong));
	return true;
}

bool cSnotify::UpdateSong(cSong* pSong, std::string& errorString) {
	if (!FindSong(pSong->getUniqueID())) {
		errorString = "Song does not exist on Snotify";
		return false;
	}

	if (vSnotifyMusic->updateSong(pSong)) {
		return true;
	}
	else {
		errorString = "Song ID not found on Snotify";
		return false;
	}
}

bool cSnotify::DeleteSong(unsigned int UniqueSongID, std::string& errorString) {
	cSong* currentSong = FindSong(UniqueSongID);
	if (currentSong == nullptr) {
		errorString = "Song does not exist or deleted on Snotify";
		return false;
	}
	currentSong->deleted = true;
	if (vSnotifyMusic->updateSong(currentSong)) {
		return true;
	}
	else {
		errorString = "Song ID not found on Snotify";
		return false;
	}
	return false;
}

// This returns a COPY of the users library, in the form of a regular dynamic array.
bool cSnotify::GetUsersSongLibrary(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary) {
	// The caller would do something like this (assume the user ID = 28472382)
	//
	//	cSong* pTheSongs = 0;
	//	unsigned int arraySize = 0;
	//	GetUsersSongLibrary( 28472382, pTheSongs, arraySize );
	// 
	// Inside this method, you'd do something like this:

	// TODO: Find that user... 

	// Alloate a heap based array to hold all the songs...

//	sizeOfLibary = WhateverYouHaveToDoToGetThisValue();
//	pCopyOfLibrary = new cSong[sizeOfLibary];

	// The array and the size of the array are "returned" by reference to the caller. 

	// TODO: Copy all the songs over

	return true;
}

cSong* cSnotify::GetSong(unsigned int SnotifyUserID, unsigned int songUniqueID, std::string& errorString) {
	cPerson* currentUser = FindUserBySnotifyID(SnotifyUserID);
	if (currentUser == nullptr) {
		errorString = "User not found.";
		return nullptr;
	}

	cSong* userList = mSnotifyUsers->getUserList(SnotifyUserID);
	if (userList == nullptr) {
		errorString = "User does NOT have a song list yet. IMPOSSIBLE!";
		return nullptr;
	}

	return userList;
}

cPerson* cSnotify::FindUserBySIN(unsigned int SIN) {
	for (int i = 0; i < mSnotifyUsers->size(); i++) {
		cPerson* currentUser = mSnotifyUsers->getByArrayId(i);
		if (currentUser->SIN == SIN)
			return currentUser;
	}
	return nullptr;
}

cPerson* cSnotify::FindUserBySnotifyID(unsigned int SnotifyID) {
	for (int i = 0; i < mSnotifyUsers->size(); i++) {
		cPerson* currentUser = mSnotifyUsers->getByArrayId(i);
		if (currentUser->getSnotifyUniqueUserID() == SnotifyID &&
			!currentUser->deleted)
			return currentUser;
	}
	return nullptr;
}

cSong* cSnotify::FindSong(std::string title, std::string artist) {
	for (int i = 0; i < vSnotifyMusic->size(); i++) {
		cSong* currenSong = vSnotifyMusic->get(i);
		if (currenSong->name == title && currenSong->artist == artist)
			return currenSong;
	}
	return nullptr;
}

cSong* cSnotify::FindSong(unsigned int uniqueID) {
	for (int i = 0; i < vSnotifyMusic->size(); i++) {
		cSong* currenSong = vSnotifyMusic->get(i);
		if (currenSong->getUniqueID() == uniqueID &&
			!currenSong->deleted)
			return currenSong;
	}
	return nullptr;
}
