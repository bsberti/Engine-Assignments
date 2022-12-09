#include "cSnotify.h"

cSnotify mainSnotify;
cPersonGenerator pGenerator;
cMusicGenerator mGenerator;

const unsigned int MAX_SNOTIFY_SONGS = 100;

int main(int argc, char* argv[]) {
	cPerson* p1 = pGenerator.generateRandomPerson();
	cPerson* p2 = pGenerator.generateRandomPerson();
	cPerson* p3 = pGenerator.generateRandomPerson();

	cSong* s = mGenerator.getRandomSong();

	std::string error;
	mainSnotify.AddUser(p1, error);
	mainSnotify.AddUser(p2, error);
	mainSnotify.AddUser(p3, error);

	p1->first = "Name Diff";
	mainSnotify.UpdateUser(p1, error);

	if (!mainSnotify.DeleteUser(p2->getSnotifyUniqueUserID(), error)) {
		std::cout << error << std::endl;
	}

	cPerson* newP1 = mainSnotify.FindUserBySnotifyID(p1->getSnotifyUniqueUserID());
	// We cannot return a pointer to an deleted user
	//cPerson* newP2 = mainSnotify.FindUserBySnotifyID(p2->getSnotifyUniqueUserID());
	cPerson* newP3 = mainSnotify.FindUserBySnotifyID(p3->getSnotifyUniqueUserID());

	for (int i = 0; i < MAX_SNOTIFY_SONGS; i++) {
		if (!mainSnotify.AddSong(mGenerator.getRandomSong(), error)) {
			std::cout << error << std::endl;
		}
	}

	int breakPoint = 1;
}