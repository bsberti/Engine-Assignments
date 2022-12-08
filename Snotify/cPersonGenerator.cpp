#include "cPersonGenerator.h"

// Warning C26812 : Prefer 'enum class' over 'enum' (Enum.3)
#pragma warning( disable : 26812 )

#include <fstream>
#include <sstream>		// String Stream
#include <string>


cPersonGenerator::cPersonGenerator() {
	if (!LoadCensusFiles("names.csv", "surnames.csv", "streetnames.csv", errorString)) {
		std::cout << "Failed to load files. Error: " << errorString << std::endl;
	}
}

cPersonGenerator::~cPersonGenerator() {

}

bool cPersonGenerator::LoadCensusFiles(
	std::string babyNameFile,
	std::string surnameFile,
	std::string streetNameFile,
	std::string& errorString)
{
	// TODO: Put YOUR amazing code here!
	vNames = readCSVFile(babyNameFile, 0);
	vGender = readCSVFile(babyNameFile, 1);
	vSurnames = readCSVFile(surnameFile, 0);
	vStreetName = readCSVFile(streetNameFile, 1);
	vStreetType = readCSVFile(streetNameFile, 2);
	vStreetDirection = readCSVFile(streetNameFile, 3);

	return true;
}

cPerson* cPersonGenerator::generateRandomPerson(void) {
	cPerson* newPerson = new cPerson();

	int nameId = rand() % vNames->size();
	newPerson->first = vNames->get(nameId);
	newPerson->middle = vNames->get(rand() % vNames->size());
	newPerson->last = vSurnames->get(rand() % vSurnames->size());

	const unsigned int MAX_AGE = 101;
	newPerson->age = (rand() % MAX_AGE);

	if (rand() % 10 == 1) {
		newPerson->gender = NON_BINARY;
	}
	else if (rand() % 10 == 2) {
		newPerson->gender = RATHER_NOT_SAY_UNKNOWN;
	}
	else {
		if (std::stoi(vGender->get(nameId)) == 0)
			newPerson->gender = MALE;
		else {
			newPerson->gender = FEMALE;
		}
	}

	const unsigned int MAX_STREET_NUMBER = 1500;
	newPerson->streetNumber = (rand() % MAX_STREET_NUMBER);
	int streetId = rand() % vStreetName->size();
	newPerson->streetName = vStreetName->get(streetId);
	newPerson->streetType = vStreetType->get(streetId);
	newPerson->streetDirection = vStreetDirection->get(streetId);

	std::string SIN;
	int length = 9;

	while (SIN.size() < (length - 1)) {
		SIN += rand() % length;
	}

	return newPerson;
}

// Here's a simple way to load the comma delimited files:
VectorString* cPersonGenerator::readCSVFile(std::string filePath, int tokenId)
{
	VectorString* result = new VectorString();

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

	unsigned int lineCount = 0;
	while (std::getline(namesFile, theLine))
	{
		lineCount++;
		std::stringstream ssLine(theLine);

		std::string token;
		unsigned int tokenCount = 0;
		while (std::getline(ssLine, token, ','))
		{
			if (tokenCount == tokenId)
			{
				//std::cout << token << std::endl;
				result->pushBack(token);
			}
			// Ignore the other parts of the line
			tokenCount++;
		}
	}

	std::cout << "Lines read = " << lineCount << std::endl;

	return result;
}

unsigned int cPersonGenerator::getNumberOfNamesLoaded(void) {
	return vNames->size();
}

unsigned int cPersonGenerator::getNumberOfSurnamesLoaded(void) {
	return vSurnames->size();
}

unsigned int cPersonGenerator::getNumberOfStreetsLoaded(void) {
	return vStreetName->size();
}

