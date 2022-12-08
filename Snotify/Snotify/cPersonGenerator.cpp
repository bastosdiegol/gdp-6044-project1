#include "cPersonGenerator.h"

// Warning C26812 : Prefer 'enum class' over 'enum' (Enum.3)
#pragma warning( disable : 26812 )

#include <iostream>
#include <fstream>
#include <sstream>		// String Stream
#include <string>

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cPersonGenerator::cPersonGenerator() {
	DEBUG_PRINT("cPersonGenerator::cPersonGenerator()\n");
}

cPersonGenerator::~cPersonGenerator() {
	DEBUG_PRINT("cPersonGenerator::~cPersonGenerator()\n");
	m_maleNames.~tDLList();
	m_femaleNames.~tDLList();
	m_nobinaryNames.~tDLList();
	m_surnames.~tDLList();
	m_streetnames[0].~tDLList();
	m_streetnames[1].~tDLList();
	m_streetnames[2].~tDLList();
}

bool cPersonGenerator::LoadCensusFiles(
	std::string babyNameFile,
	std::string surnameFile,
	std::string streetNameFile,
	std::string& errorString)
{
	DEBUG_PRINT("cPersonGenerator::LoadCensusFiles(%s, %s, %s)\n", babyNameFile.c_str(), surnameFile.c_str(), streetNameFile.c_str());

	std::string theLine;
	std::string token;
	std::string gender;

	std::ifstream namesFile(babyNameFile);
	std::ifstream surnamesFile(surnameFile);
	std::ifstream streetnamesFile(streetNameFile);

	unsigned int maleNames		= 0;
	unsigned int femaleNames	= 0;
	unsigned int nonbinaryNames = 0;
	unsigned int surnames		= 0;
	unsigned int streetnames	= 0;

	DEBUG_PRINT("PersonGenerator is loading names...\n");
	// ***************
	// *    NAMES    *
	// ***************
	// Open the name file
	if (!namesFile.is_open()) {
		errorString = "Names file couldn't be opened.\n";
		return false;
	}

	while (std::getline(namesFile, theLine)) {
		std::stringstream ssLine(theLine);

		// Gets the name and gender token
		std::getline(ssLine, token, ',');
		std::getline(ssLine, gender, ',');

		// Checks name gender and adds on the properly list
		if (gender.compare("F") == 0) {
			m_femaleNames.pushBack(token);
			femaleNames++;
		} else { // "M"
			m_maleNames.pushBack(token);
			maleNames++;
		}
	}
	DEBUG_PRINT("PersonGenerator loaded %d female names.\n", femaleNames);
	DEBUG_PRINT("PersonGenerator loaded %d male names.\n", maleNames);
	//------
	DEBUG_PRINT("PersonGenerator is now looking for non-binary names...\n");
	// Filter non-binary names
	// Iterates through all female names and compares with males
	for (int i = 0; i < m_femaleNames.getSize(); i++) {
		// Get current female name
		token = m_femaleNames.getAt(i);
		// Compares with male
		if (m_maleNames.exists(token)) {
			// Adds on non-binary names
			m_nobinaryNames.pushBack(token);
			nonbinaryNames++;
		}
	}
	DEBUG_PRINT("PersonGenerator loaded %d non-binary names.\n", nonbinaryNames);
	namesFile.close();

	// ***************
	// *   SURNAMES  *
	// ***************
	// Open the surname file
	if (!surnamesFile.is_open()) {
		errorString = "Surname file couldn't be opened.\n";
		return false;
	}
	DEBUG_PRINT("PersonGenerator is loading surnames...\n");
	// Discard the first line
	std::getline(surnamesFile, theLine);
	while (std::getline(surnamesFile, theLine)) {
		std::stringstream ssLine(theLine);

		// Gets the surname token
		std::getline(ssLine, token, ',');
		m_surnames.pushBack(token);
		surnames++;
	}
	DEBUG_PRINT("PersonGenerator loaded %d surnames.\n", surnames);
	surnamesFile.close();

	// ***************
	// * STREETNAMES *
	// ***************
	// Open the surname file
	if (!streetnamesFile.is_open()) {
		errorString = "Street names file couldn't be opened.\n";
		return false;
	}
	DEBUG_PRINT("PersonGenerator is loading street names...\n");
	// Discard the first line
	std::getline(streetnamesFile, theLine);
	while (std::getline(streetnamesFile, theLine)) {
		// FullStreetName,StreetName,StreetType,PostDirection
		std::stringstream ssLine(theLine);
		// Discard FullStreetName
		std::getline(ssLine, token, ',');
		// Gets Street tokens
		std::getline(ssLine, token, ',');
		m_streetnames[0].pushBack(token); // StreetName
		std::getline(ssLine, token, ',');
		m_streetnames[1].pushBack(token); // StreetType
		std::getline(ssLine, token, ',');
		m_streetnames[2].pushBack(token); // PostDirection
		streetnames++;
	}
	DEBUG_PRINT("PersonGenerator loaded %d street names.\n", streetnames);
	streetnamesFile.close();

	return true;
}

cPerson* cPersonGenerator::generateRandomPerson(void) {
	cPerson* newPerson = new cPerson();
	switch (rand() % 3) {
	case 0: newPerson->gender = cPerson::eGenderType::MALE;
		break;
	case 1: newPerson->gender = cPerson::eGenderType::FEMALE;
		break;
	case 2: newPerson->gender = cPerson::eGenderType::NON_BINARY;
		break;
	case 3: newPerson->gender = cPerson::eGenderType::RATHER_NOT_SAY_UNKNOWN;
		break;
	}
	return nullptr;
}



// Here's a simple way to load the comma delimited files:
bool readCSVFile(void)
{
	// Open the file
	std::ifstream namesFile("Names_2010Census.csv");
	if (!namesFile.is_open())
	{
		std::cout << "Didn't open file" << std::endl;
		return false;
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
			if (tokenCount == 0)
			{
				std::cout << token << std::endl;
			}
			// Ignore the other parts of the line
			tokenCount++;
		}
	}

	std::cout << "Lines read = " << lineCount << std::endl;

	return true;
}