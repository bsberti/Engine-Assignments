#include "cSnotify.h"

cSnotify mainSnotify;
cPersonGenerator pGenerator;
cMusicGenerator mGenerator;

std::string error;
cPerson* p1;
cPerson* p2;
cPerson* p3;

const unsigned int MAX_SNOTIFY_SONGS = 100;

int main(int argc, char* argv[]) {

	{ //Testing person generator
		p1 = pGenerator.generateRandomPerson();
		p2 = pGenerator.generateRandomPerson();
		p3 = pGenerator.generateRandomPerson();

		mainSnotify.AddUser(p1, error);
		mainSnotify.AddUser(p2, error);
		mainSnotify.AddUser(p3, error);
		mainSnotify.AddUser(pGenerator.generateRandomPerson(), error);
	}

	{ // Update Info
		p1->first = "Bruno";
		p1->middle = "de Souza";
		p1->last = "Berti";
		p1->age = 31;
		p1->streetNumber = 627;
		p1->streetName = "Third Street";
		p1->streetDirection = "EAST";
		p1->gender = MALE;
		//char postalCode[6] = { 'N', '5', 'V', '2', 'C', '2' };
		//p1->postalCode = postalCode;
		p1->SIN = 123456789;
	}

	if (!mainSnotify.UpdateUser(p1, error)) {
		std::cout << error << std::endl;
	}

	if (!mainSnotify.DeleteUser(p2->getSnotifyUniqueUserID(), error)) {
		std::cout << error << std::endl;
	}

	cPerson* newPerson1 = mainSnotify.FindUserBySnotifyID(p1->getSnotifyUniqueUserID());
	int newPerson1SnotifyID = newPerson1->getSnotifyUniqueUserID();

	// We cannot return a pointer to an deleted user
	//cPerson* p2 = mainSnotify.FindUserBySnotifyID(p2->getSnotifyUniqueUserID());
	
	cPerson* newPerson2 = mainSnotify.FindUserBySnotifyID(p3->getSnotifyUniqueUserID());
	int newPerson2SnotifyID = newPerson2->getSnotifyUniqueUserID();

	// Getting songs from bilboard list
	for (int i = 0; i < MAX_SNOTIFY_SONGS; i++) {
		if (!mainSnotify.AddSong(mGenerator.getRandomSong(), error)) {
			std::cout << error << std::endl;
		}
	}

	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	std::cout << "Adding randon songs from Billboard list to ~~~~" << newPerson1->first << "~~~~ Song List." << std::endl;
	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	for (int i = 0; i < 10; i++) {
		cSong* s = mainSnotify.vSnotifyMusic->get(rand() % mainSnotify.vSnotifyMusic->size() - 1);
		if (!mainSnotify.AddSongToUserLibrary(newPerson1SnotifyID, s, error)) {
			std::cout << error << std::endl;
		}
		else {
			std::cout << "- Music: " << s->name <<
				", Artist: " << s->artist <<
				", ID: " << s->getUniqueID() <<
				" added to [" << newPerson1->first << "] Song List." <<
				std::endl;
		}
	}

	cSong* newPerson1SongList;
	unsigned int newPerson1SongListSize;
	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	std::cout << "Listing ~~~~" << newPerson1->first << "~~~~ Song List." << std::endl;
	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	if (!mainSnotify.GetUsersSongLibrary(newPerson1SnotifyID, newPerson1SongList, newPerson1SongListSize)) {
		std::cout << "Failed to get user song library." << std::endl;
	}
	else {
		for (int i = 0; i < newPerson1SongListSize; i++) {
			if (!newPerson1SongList[i].deleted) {
				std::cout << "- Music: " << newPerson1SongList[i].name <<
					", Artist: " << newPerson1SongList[i].artist <<
					" from [" << newPerson1->first << "] Song List.";

				// Seting rating
				// newPerson1SongList[i].rating = rand() % 5;

				// Seting number of times played
				newPerson1SongList[i].numberOfTimesPlayed = rand() % 10;

				std::cout << " Rating: " << newPerson1SongList[i].rating << 
					", Played " << newPerson1SongList[i].numberOfTimesPlayed << " time(s)." << std::endl;
			}
			else {
				std::cout << "SongList[" << i << "] from user list was deleted." << std::endl;
			}
		}
	}

	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	std::cout << "Deleting 3 randon songs from ~~~~" << newPerson1->first << "~~~~ Song List." << std::endl;
	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	for (int i = 0; i < 3; i++) {
		cSong* songToDelete = &newPerson1SongList[rand() % newPerson1SongListSize];
		if (mainSnotify.RemoveSongFromUserLibrary(newPerson1SnotifyID, songToDelete->getUniqueID(), error)) {
			std::cout << error << std::endl;
		}
	}

	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	std::cout << "Listing ~~~~" << newPerson1->first << "~~~~ Song List (AFTER DELETING)." << std::endl;
	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	if (!mainSnotify.GetUsersSongLibrary(newPerson1SnotifyID, newPerson1SongList, newPerson1SongListSize)) {
		std::cout << "Failed to get user song library." << std::endl;
	}
	else {
		for (int i = 0; i < newPerson1SongListSize; i++) {
			if (!newPerson1SongList[i].deleted) {
				std::cout << "- Music: " << newPerson1SongList[i].name <<
					", Artist: " << newPerson1SongList[i].artist <<
					" from [" << newPerson1->first << "] Song List.";

				std::cout << " Rating: " << newPerson1SongList[i].rating <<
					", Played " << newPerson1SongList[i].numberOfTimesPlayed << " time(s)." << std::endl;

			}
			else {
				std::cout << "SongList[" << i << "] from user list was deleted." << std::endl;
			}
		}
	}

	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	std::cout << "Playing 5 randon songs (if NOT deleted) from ~~~~" << newPerson1->first << "~~~~ Song List." << std::endl;
	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	for (int i = 0; i < 5; i++) {
		cSong* randomSongToPlay = &newPerson1SongList[rand() % newPerson1SongListSize];
		cSong* randomPlayedSong = mainSnotify.GetSong(newPerson1SnotifyID, randomSongToPlay->getUniqueID(), error);
		if (randomPlayedSong == nullptr) {
			std::cout << error << std::endl;
		}
		else {
			if (!randomPlayedSong->deleted) {
				std::cout << "- Music: " << randomPlayedSong->name <<
					", Artist: " << randomPlayedSong->artist <<
					", Played " << randomPlayedSong->numberOfTimesPlayed << " time(s). " << 
					"From [" << newPerson1->first << "] Song List." <<
					std::endl;
			}
			else {
				std::cout << "Tried to play song: " << randomPlayedSong->name <<
					" but it's deleted" << std::endl;
			}
		}
	}

	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	std::cout << "Getting ~~~~" << newPerson1->first << "~~~~ Song List (AFTER PLAYING)." << std::endl;
	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	if (!mainSnotify.GetUsersSongLibrary(newPerson1SnotifyID, newPerson1SongList, newPerson1SongListSize)) {
		std::cout << "Failed to get user song library." << std::endl;
	}

	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	std::cout << "Listing ~~~~" << newPerson1->first << "~~~~ song's rating of Song List." << std::endl;
	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	for (int i = 0; i < newPerson1SongListSize; i++) {
		cSong* songToShowRating = &newPerson1SongList[i];
		unsigned int songRating;
		if (!mainSnotify.GetCurrentSongRating(newPerson1SnotifyID, songToShowRating->getUniqueID(), songRating)) {
			std::cout << "Failed to get song rating." << std::endl;
		}
		else {
			if (!newPerson1SongList[i].deleted) {
				std::cout << "- Music: " << newPerson1SongList[i].name <<
					", Artist: " << newPerson1SongList[i].artist <<
					", Rating: " << songRating << " stars." <<
					std::endl;
			}
			else {
				std::cout << "SongList[" << i << "] from user list was deleted." << std::endl;
			}
		}
	}

	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	std::cout << "Listing ~~~~" << newPerson1->first << "~~~~ song's number of times played of Song List." << std::endl;
	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	for (int i = 0; i < newPerson1SongListSize; i++) {
		cSong* songToShowTimesPlayed = &newPerson1SongList[i];
		unsigned int songTimesPlayed;
		if (!mainSnotify.GetCurrentSongNumberOfPlays(newPerson1SnotifyID, songToShowTimesPlayed->getUniqueID(), songTimesPlayed)) {
			std::cout << "Failed to get song rating." << std::endl;
		}
		else {
			if (!newPerson1SongList[i].deleted) {
				std::cout << "- Music: " << newPerson1SongList[i].name <<
					", Artist: " << newPerson1SongList[i].artist <<
					", Played " << songTimesPlayed << " time(s)." <<
					std::endl;
			}
			else {
				std::cout << "SongList[" << i << "] from user list was deleted." << std::endl;
			}
		}
	}

	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	std::cout << "SORTING ~~~~" << newPerson1->first << "~~~~ Song List." << std::endl;
	std::cout << "-----------------------------------------------------------------------------" << std::endl;
	if (!mainSnotify.GetUsersSongLibraryAscendingByTitle(newPerson1SnotifyID, newPerson1SongList, newPerson1SongListSize)) {
		std::cout << "Failed to get user song library." << std::endl;
	}
	else {
		for (int i = 0; i < newPerson1SongListSize; i++) {
			if (!newPerson1SongList[i].deleted) {
				std::cout << "- Music: " << newPerson1SongList[i].name <<
					", Artist: " << newPerson1SongList[i].artist <<
					std::endl;
			}
			else {
				std::cout << "SongList[" << i << "] from user list was deleted." << std::endl;
			}
		}
	}
	



	int breakPoint = 1;
}