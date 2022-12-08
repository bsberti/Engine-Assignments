#include "cMusicGenerator.h"

cMusicGenerator::cMusicGenerator() {
	if (!LoadMusicInformation("audiio.csv", errorString)) {
		std::cout << "Failed to load files. Error: " << errorString << std::endl;
	}
}

cMusicGenerator::~cMusicGenerator()
{
}

bool cMusicGenerator::LoadMusicInformation(
	std::string musicFileName, 
	std::string& errorString) {
	vMusic = readCSVFile(musicFileName);
	return false;
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

	// name,rank,count,prop100k,cum_prop100k,pctwhite,pctblack,pctapi,pctaian,pct2prace,pcthispanic
	// SMITH,1,2442977,828.19,828.19,70.9,23.11,0.5,0.89,2.19,2.4
	//
	// - rank is how popular the last name is, like 1st, 2nd, etc.
	// - count is how many people have that last name, so 2,442,977 people have last name "Smith"
	// - prop100k is the ratio per 100,000 people. Smith is 828.19, 
	//            meaning that there's a 828.19 out of 100,000 chance (0.82819% chance)
	//            that someone is named "Smith"

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
