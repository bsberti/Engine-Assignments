#include "cSnotify.h"
#include "cMusicGenerator.h"
#include "cPersonGenerator.h"

cSnotify mainSnotify;
cPersonGenerator pGenerator;
cMusicGenerator mGenerator;

int main(int argc, char* argv[]) {
	cPerson* p = pGenerator.generateRandomPerson();
	cSong* s = mGenerator.getRandomSong();

	int breakPoint = 1;
}