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

bool cSnotify::AddSongToUserLibrary(unsigned int snotifyUserID, cSong* pNewSong, std::string& errorString) {
	cPerson* currentUser = FindUserBySnotifyID(snotifyUserID);
	if (currentUser == nullptr) {
		errorString = "User not found.";
		return false;
	}

	mSnotifyUsers->addSongToUser(snotifyUserID, pNewSong);
	return true;
}

bool cSnotify::RemoveSongFromUserLibrary(unsigned int snotifyUserID, unsigned int SnotifySongID, std::string& errorString) {
	cPerson* currentUser = FindUserBySnotifyID(snotifyUserID);
	if (currentUser == nullptr) {
		errorString = "User not found.";
		return false;
	}

	UserSongList* pUserSongList = mSnotifyUsers->getUserSongList(snotifyUserID);
	cSong* pUserList = mSnotifyUsers->getUserList(snotifyUserID);
	if (pUserList == nullptr) {
		errorString = "User does NOT have a song list yet. IMPOSSIBLE!";
		return false;
	}

	for (int i = 0; i < pUserSongList->size(); i++) {
		if (pUserList[i].getUniqueID() == SnotifySongID) {
			pUserList[i].deleted = true;
		}
	}
	return false;
}

bool cSnotify::UpdateRatingOnSong(unsigned int SnotifyUserID, unsigned int songUniqueID, unsigned int newRating) {
	cPerson* currentUser = FindUserBySnotifyID(SnotifyUserID);
	if (currentUser == nullptr) {
		//errorString = "User not found.";
		return false;
	}

	UserSongList* pUserSongList = mSnotifyUsers->getUserSongList(SnotifyUserID);
	cSong* pUserList = mSnotifyUsers->getUserList(SnotifyUserID);
	if (pUserList == nullptr) {
		//errorString = "User does NOT have a song list yet. IMPOSSIBLE!";
		return false;
	}

	for (int i = 0; i < pUserSongList->size(); i++) {
		if (pUserList[i].getUniqueID() == songUniqueID) {
			pUserList[i].rating = newRating;
			return true;
		}
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
	cPerson* currentUser = FindUserBySnotifyID(snotifyUserID);
	if (currentUser == nullptr) {
		return false;
	}

	// The UserSongList is my "intelligent array, I can get the list size and members
	UserSongList* pUserSongList = mSnotifyUsers->getUserSongList(snotifyUserID);
	// This is the acctual list of songs
	cSong* pUserList = mSnotifyUsers->getUserList(snotifyUserID);
	if (pUserList == nullptr) {
		// That means the user does NOT have a list of songs yet
		// At this point, this is impossible because every user start with a song list
		return false;
	}

	// Alloate a heap based array to hold all the songs...

	sizeOfLibary = pUserSongList->size();
	//cSong* pCopyOfLibrary = new cSong[sizeOfLibary];
	//pCopyOfLibrary = pUserList;
	// The array and the size of the array are "returned" by reference to the caller. 

	// TODO: Copy all the songs over
	pLibraryArray = pUserList;

	return true;
}

bool cSnotify::GetUsersSongLibraryAscendingByTitle(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary) {
	cPerson* currentUser = FindUserBySnotifyID(snotifyUserID);
	if (currentUser == nullptr) {
		return false;
	}

	// Sorting
	//std::cout << "is A in front of B? " << (("A" > "B") ? "Yes" : "No") << std::endl;
	//std::cout << "is B in front of A? " << (("B" > "A") ? "Yes" : "No") << std::endl;
	//std::cout << "is B in front of W? " << (("B" > "W") ? "Yes" : "No") << std::endl;
	//std::cout << "is W in front of B? " << (("W" > "B") ? "Yes" : "No") << std::endl;

	UserSongList* pUserSongList = mSnotifyUsers->getUserSongList(snotifyUserID);
	pUserSongList->getAlphabeticalSortByTittle();
	cSong* pUserList = mSnotifyUsers->getUserList(snotifyUserID);
	if (pUserList == nullptr) {
		return false;
	}
	
	sizeOfLibary = pUserSongList->size();	
	pLibraryArray = pUserList;

	return true;
}

bool cSnotify::GetUsersSongLibraryAscendingByArtist(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary) {
	cPerson* currentUser = FindUserBySnotifyID(snotifyUserID);
	if (currentUser == nullptr) {
		return false;
	}

	UserSongList* pUserSongList = mSnotifyUsers->getUserSongList(snotifyUserID);
	pUserSongList->getAlphabeticalSortByArtist();
	cSong* pUserList = mSnotifyUsers->getUserList(snotifyUserID);
	if (pUserList == nullptr) {
		return false;
	}

	sizeOfLibary = pUserSongList->size();
	pLibraryArray = pUserList;

	return true;
}

bool cSnotify::GetUsers(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {

	return false;
}

bool cSnotify::GetUsersByID(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {

	return false;
}

bool cSnotify::FindUsersFirstName(std::string firstName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {

	return false;
}

bool cSnotify::FindUsersLastName(std::string lastName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {

	return false;
}

bool cSnotify::FindUsersFirstLastNames(std::string firstName, std::string lastName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {

	return false;
}

cSong* cSnotify::GetSong(unsigned int SnotifyUserID, unsigned int songUniqueID, std::string& errorString) {
	cPerson* currentUser = FindUserBySnotifyID(SnotifyUserID);
	if (currentUser == nullptr) {
		errorString = "User not found.";
		return nullptr;
	}

	UserSongList* pUserSongList = mSnotifyUsers->getUserSongList(SnotifyUserID);
	cSong* pUserList = pUserSongList->getSongList();
	if (pUserList == nullptr) {
		errorString = "User does NOT have a song list yet. IMPOSSIBLE!";
		return nullptr;
	}
	
	cSong* pSong = new cSong();
	bool songFound = false;
	for (int i = 0; i < pUserSongList->size(); i++) {
		if (pUserList[i].getUniqueID() == songUniqueID) {
			pSong = new cSong(pUserList[i]);
			pSong->numberOfTimesPlayed++;
			songFound = true;
		}
		else {
			errorString = "Wrong songUniqueID: " + std::to_string(songUniqueID);
		}
	}

	if (!songFound) {
		return nullptr;
	}

	// An Array is also a pointer to the first member
	// But I don't want the first member, I want the songUniqueID song.
	// That's why I created the pSong

	// This is the pointer to the song on the userList songs
	return pSong;
}

bool cSnotify::GetCurrentSongRating(unsigned int snotifyUserID, unsigned int songUniqueID, unsigned int& songRating) {
	cPerson* currentUser = FindUserBySnotifyID(snotifyUserID);
	if (currentUser == nullptr) {
		//errorString = "User not found.";
		return false;
	}

	UserSongList* pUserSongList = mSnotifyUsers->getUserSongList(snotifyUserID);
	cSong* pUserList = pUserSongList->getSongList();
	if (pUserList == nullptr) {
		//errorString = "User does NOT have a song list yet. IMPOSSIBLE!";
		return false;
	}

	cSong* pSong = new cSong();
	for (int i = 0; i < pUserSongList->size(); i++) {
		if (pUserList[i].getUniqueID() == songUniqueID) {
			pSong = new cSong(pUserList[i]);
			songRating = pSong->rating;
			return true;
		}
		else {
			//errorString = "Wrong songUniqueID: " + std::to_string(songUniqueID);
		}
	}

	return false;
}

bool cSnotify::GetCurrentSongNumberOfPlays(unsigned int snotifyUserID, unsigned int songUniqueID, unsigned int& numberOfPlays) {
	cPerson* currentUser = FindUserBySnotifyID(snotifyUserID);
	if (currentUser == nullptr) {
		//errorString = "User not found.";
		return false;
	}

	UserSongList* pUserSongList = mSnotifyUsers->getUserSongList(snotifyUserID);
	cSong* pUserList = pUserSongList->getSongList();
	if (pUserList == nullptr) {
		//errorString = "User does NOT have a song list yet. IMPOSSIBLE!";
		return false;
	}

	cSong* pSong = new cSong();
	for (int i = 0; i < pUserSongList->size(); i++) {
		if (pUserList[i].getUniqueID() == songUniqueID) {
			pSong = new cSong(pUserList[i]);
			numberOfPlays = pSong->numberOfTimesPlayed;
			return true;
		}
		else {
			//errorString = "Wrong songUniqueID: " + std::to_string(songUniqueID);
		}
	}

	return false;
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
