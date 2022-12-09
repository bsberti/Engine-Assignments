#include "cMusicGenerator.h"

cMusicGenerator::cMusicGenerator() {
	if (!LoadMusicInformation("audiio.csv", errorString)) {
		std::cout << "Failed to load audio file. Error: " << errorString << std::endl;
	}
}

cMusicGenerator::~cMusicGenerator()
{
}

bool cMusicGenerator::LoadMusicInformation(
	std::string musicFileName, 
	std::string& errorString) {
	vMusic = readCSVFile(musicFileName);
	return true;
}

cSong* cMusicGenerator::getRandomSong(void) {
	return vMusic->get(rand() % vMusic->size());
}

VectorSong* cMusicGenerator::readCSVFile(std::string filePath) {
	VectorSong* result = new VectorSong();

	// Open the file
	std::ifstream namesFile(filePath);
	if (!namesFile.is_open())
	{
		std::cout << "Didn't open file" << std::endl;
		return result;
	}

	std::string theLine;
	cSong currentSound;
	unsigned int lineCount = 0;
	while (std::getline(namesFile, theLine))
	{
		lineCount++;
		std::stringstream ssLine(theLine);
		std::string token;

		std::getline(ssLine, token, ',');
		std::getline(ssLine, token, ',');
		currentSound.artist = token;
		std::getline(ssLine, token, ',');
		currentSound.name = token;
		currentSound.rating = 0;
		currentSound.numberOfTimesPlayed = 0;

		currentSound.uniqueID = lineCount;

		result->pushBack(currentSound);
	}

	std::cout << "Lines read = " << lineCount << std::endl;

	return result;
}

cSong* cMusicGenerator::findSong(std::string songName, std::string artist)
{
	return nullptr;
}
