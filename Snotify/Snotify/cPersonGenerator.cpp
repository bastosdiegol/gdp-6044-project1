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
	v_maleNames.~tDLList();
	v_femaleNames.~tDLList();
	v_nobinaryNames.~tDLList();
	v_surnames.~tDLList();
	v_streetnames[0].~tDLList();
	v_streetnames[1].~tDLList();
	v_streetnames[2].~tDLList();
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
			v_femaleNames.pushBack(token);
			femaleNames++;
		} else { // "M"
			v_maleNames.pushBack(token);
			maleNames++;
		}
	}
	DEBUG_PRINT("PersonGenerator loaded %d female names.\n", femaleNames);
	DEBUG_PRINT("PersonGenerator loaded %d male names.\n", maleNames);
	//------
	DEBUG_PRINT("PersonGenerator is now looking for non-binary names...\n");
	// Filter non-binary names
	// Iterates through all female names and compares with males
	for (int i = 0; i < v_femaleNames.getSize(); i++) {
		// Get current female name
		token = v_femaleNames.getAt(i);
		// Compares with male
		if (v_maleNames.exists(token)) {
			// Adds on non-binary names
			v_nobinaryNames.pushBack(token);
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
		v_surnames.pushBack(token);
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
		v_streetnames[0].pushBack(token); // StreetName
		std::getline(ssLine, token, ',');
		v_streetnames[1].pushBack(token); // StreetType
		std::getline(ssLine, token, ',');
		v_streetnames[2].pushBack(token); // PostDirection
		streetnames++;
	}
	DEBUG_PRINT("PersonGenerator loaded %d street names.\n", streetnames);
	streetnamesFile.close();

	DEBUG_PRINT("PersonGenerator is loading provinces and cities...\n");
	LoadProvincesAndCities();
	DEBUG_PRINT("PersonGenerator loaded all provinces and cities.\n");

	return true;
}

unsigned int cPersonGenerator::getNumberOfNamesLoaded(void) {
	DEBUG_PRINT("cPersonGenerator::getNumberOfNamesLoaded()\n");
	return v_femaleNames.getSize() + v_maleNames.getSize() - v_nobinaryNames.getSize();
}

unsigned int cPersonGenerator::getNumberOfSurnamesLoaded(void) {
	DEBUG_PRINT("cPersonGenerator::getNumberOfSurnamesLoaded()\n");
	return v_surnames.getSize();
}

unsigned int cPersonGenerator::getNumberOfStreetsLoaded(void) {
	DEBUG_PRINT("cPersonGenerator::getNumberOfStreetsLoaded()\n");
	return v_streetnames->getSize();
}

cPerson* cPersonGenerator::generateRandomPerson(void) {
	//DEBUG_PRINT("cPersonGenerator::generateRandomPerson()\n");
	cPerson* newPerson = new cPerson();
	// Generates Random Gender
	switch (rand() % 4) {
	case 0: newPerson->gender = cPerson::eGenderType::MALE;
		break;
	case 1: newPerson->gender = cPerson::eGenderType::FEMALE;
		break;
	case 2: newPerson->gender = cPerson::eGenderType::NON_BINARY;
		break;
	case 3: newPerson->gender = cPerson::eGenderType::RATHER_NOT_SAY_UNKNOWN;
		break;
	}
	// Generates Random Name Based on Gender
	switch (newPerson->gender) {
	case cPerson::eGenderType::MALE: 
		newPerson->first = v_maleNames.getAt(rand() % v_maleNames.getSize());
		newPerson->middle = v_maleNames.getAt(rand() % v_maleNames.getSize());
		break;
	case cPerson::eGenderType::FEMALE:
		newPerson->first = v_femaleNames.getAt(rand() % v_femaleNames.getSize());
		newPerson->middle = v_femaleNames.getAt(rand() % v_femaleNames.getSize());
		break;
	case cPerson::eGenderType::NON_BINARY:
		newPerson->first = v_nobinaryNames.getAt(rand() % v_nobinaryNames.getSize());
		newPerson->middle = v_nobinaryNames.getAt(rand() % v_nobinaryNames.getSize());
		break;
	case cPerson::eGenderType::RATHER_NOT_SAY_UNKNOWN:
		if ( (rand() % 2) == 0) {
			newPerson->first = v_maleNames.getAt(rand() % v_maleNames.getSize());
		} else {
			newPerson->first = v_femaleNames.getAt(rand() % v_femaleNames.getSize());
		}
		if ((rand() % 2) == 0) {
			newPerson->middle = v_maleNames.getAt(rand() % v_maleNames.getSize());
		} else {
			newPerson->middle = v_femaleNames.getAt(rand() % v_femaleNames.getSize());
		}
		break;
	}
	// Generates Random Surname
	newPerson->last = v_surnames.getAt(rand() % v_surnames.getSize());
	// Generates Random Age
	newPerson->age = 8 + (rand() % 101);
	// Generates Random SIN
	newPerson->SIN = 100000000 + (rand() % 999999999);
	// Generates Random Postal Code
	newPerson->postalCode[0] = 'A' + rand() % 26;
	newPerson->postalCode[1] = '0' + rand() % 10;
	newPerson->postalCode[2] = 'A' + rand() % 26;
	newPerson->postalCode[3] = '0' + rand() % 10;
	newPerson->postalCode[4] = 'A' + rand() % 26;
	newPerson->postalCode[5] = '0' + rand() % 10;
	// Generates Random Street
	int streetID = rand() % v_streetnames->getSize();
	newPerson->streetNumber		= streetID;
	newPerson->streetName		= v_streetnames[0].getAt(streetID);
	newPerson->streetType		= v_streetnames[1].getAt(streetID);
	newPerson->streetDirection	= v_streetnames[2].getAt(streetID);
	// Generates Random Province and City
	int randomProvince = rand() % 13;
	newPerson->province = v_ProvincesAndCities[randomProvince].getAt(0);
	newPerson->city = v_ProvincesAndCities[randomProvince].getAt(1 + (rand() % (v_ProvincesAndCities[randomProvince].getSize()-1)));

	return newPerson;
}

void cPersonGenerator::LoadProvincesAndCities(void) {
	v_ProvincesAndCities[0].pushBack("Alberta");
	v_ProvincesAndCities[0].pushBack("Banff");
	v_ProvincesAndCities[0].pushBack("Brooks");
	v_ProvincesAndCities[0].pushBack("Calgary");
	v_ProvincesAndCities[0].pushBack("Edmonton");
	v_ProvincesAndCities[0].pushBack("Fort McMurray");
	v_ProvincesAndCities[0].pushBack("Grande Prairie");
	v_ProvincesAndCities[0].pushBack("Jasper");
	v_ProvincesAndCities[0].pushBack("Lake Louise");
	v_ProvincesAndCities[0].pushBack("Lethbridge");
	v_ProvincesAndCities[0].pushBack("Medicine Hat");
	v_ProvincesAndCities[0].pushBack("Red Deer");
	v_ProvincesAndCities[0].pushBack("Saint Albert");

	v_ProvincesAndCities[1].pushBack("British Columbia");
	v_ProvincesAndCities[1].pushBack("Barkerville");
	v_ProvincesAndCities[1].pushBack("Burnaby");
	v_ProvincesAndCities[1].pushBack("Campbell River");
	v_ProvincesAndCities[1].pushBack("Chilliwack");
	v_ProvincesAndCities[1].pushBack("Courtenay");
	v_ProvincesAndCities[1].pushBack("Cranbrook");
	v_ProvincesAndCities[1].pushBack("Dawson Creek");
	v_ProvincesAndCities[1].pushBack("Delta");
	v_ProvincesAndCities[1].pushBack("Esquimalt");
	v_ProvincesAndCities[1].pushBack("Fort Saint James");
	v_ProvincesAndCities[1].pushBack("Fort Saint John");
	v_ProvincesAndCities[1].pushBack("Hope");
	v_ProvincesAndCities[1].pushBack("Kamloops");
	v_ProvincesAndCities[1].pushBack("Kelowna");
	v_ProvincesAndCities[1].pushBack("Kimberley");
	v_ProvincesAndCities[1].pushBack("Kitimat");
	v_ProvincesAndCities[1].pushBack("Langley");
	v_ProvincesAndCities[1].pushBack("Nanaimo");
	v_ProvincesAndCities[1].pushBack("Nelson");
	v_ProvincesAndCities[1].pushBack("New Westminster");
	v_ProvincesAndCities[1].pushBack("North Vancouver");
	v_ProvincesAndCities[1].pushBack("Oak Bay");
	v_ProvincesAndCities[1].pushBack("Penticton");
	v_ProvincesAndCities[1].pushBack("Powell River");
	v_ProvincesAndCities[1].pushBack("Prince George");
	v_ProvincesAndCities[1].pushBack("Prince Rupert");
	v_ProvincesAndCities[1].pushBack("Quesnel");
	v_ProvincesAndCities[1].pushBack("Revelstoke");
	v_ProvincesAndCities[1].pushBack("Rossland");
	v_ProvincesAndCities[1].pushBack("Trail");
	v_ProvincesAndCities[1].pushBack("Vancouver");
	v_ProvincesAndCities[1].pushBack("Vernon");
	v_ProvincesAndCities[1].pushBack("Victoria");
	v_ProvincesAndCities[1].pushBack("West Vancouver");
	v_ProvincesAndCities[1].pushBack("White Rock");

		
	v_ProvincesAndCities[2].pushBack("Manitoba");
	v_ProvincesAndCities[2].pushBack("Brandon");
	v_ProvincesAndCities[2].pushBack("Churchill");
	v_ProvincesAndCities[2].pushBack("Dauphin");
	v_ProvincesAndCities[2].pushBack("Flin Flon");
	v_ProvincesAndCities[2].pushBack("Kildonan");
	v_ProvincesAndCities[2].pushBack("Saint Bonifa");
	v_ProvincesAndCities[2].pushBack("Swan River");
	v_ProvincesAndCities[2].pushBack("Thompson");
	v_ProvincesAndCities[2].pushBack("Winnipeg");
	v_ProvincesAndCities[2].pushBack("York Factory");

	v_ProvincesAndCities[3].pushBack("New Brunswick");
	v_ProvincesAndCities[3].pushBack("Bathurst");
	v_ProvincesAndCities[3].pushBack("Caraquet");
	v_ProvincesAndCities[3].pushBack("Dalhousie");
	v_ProvincesAndCities[3].pushBack("Fredericton");
	v_ProvincesAndCities[3].pushBack("Miramichi");
	v_ProvincesAndCities[3].pushBack("Moncton");
	v_ProvincesAndCities[3].pushBack("Saint John");

	v_ProvincesAndCities[4].pushBack("Newfoundland and Labrador");
	v_ProvincesAndCities[4].pushBack("Argentia");
	v_ProvincesAndCities[4].pushBack("Bonavista");
	v_ProvincesAndCities[4].pushBack("Channel-Port aux Basques");
	v_ProvincesAndCities[4].pushBack("Corner Brook");
	v_ProvincesAndCities[4].pushBack("Ferryland");
	v_ProvincesAndCities[4].pushBack("Gander");
	v_ProvincesAndCities[4].pushBack("Grand Falls–Windsor");
	v_ProvincesAndCities[4].pushBack("Happy Valley–Goose Bay");
	v_ProvincesAndCities[4].pushBack("Harbour Grace");
	v_ProvincesAndCities[4].pushBack("Labrador City");
	v_ProvincesAndCities[4].pushBack("Placentia");
	v_ProvincesAndCities[4].pushBack("Saint Anthony");
	v_ProvincesAndCities[4].pushBack("St. John’s");
	v_ProvincesAndCities[4].pushBack("Wabana");

	v_ProvincesAndCities[5].pushBack("Northwest Territories");
	v_ProvincesAndCities[5].pushBack("Fort Smith");
	v_ProvincesAndCities[5].pushBack("Hay River");
	v_ProvincesAndCities[5].pushBack("Inuvik");
	v_ProvincesAndCities[5].pushBack("Tuktoyaktuk");
	v_ProvincesAndCities[5].pushBack("Yellowknife");

	v_ProvincesAndCities[6].pushBack("Nova Scotia");
	v_ProvincesAndCities[6].pushBack("Baddeck");
	v_ProvincesAndCities[6].pushBack("Digby");
	v_ProvincesAndCities[6].pushBack("Glace Bay");
	v_ProvincesAndCities[6].pushBack("Halifax");
	v_ProvincesAndCities[6].pushBack("Liverpool");
	v_ProvincesAndCities[6].pushBack("Louisbourg");
	v_ProvincesAndCities[6].pushBack("Lunenburg");
	v_ProvincesAndCities[6].pushBack("Pictou");
	v_ProvincesAndCities[6].pushBack("Port Hawkesbury");
	v_ProvincesAndCities[6].pushBack("Springhill");
	v_ProvincesAndCities[6].pushBack("Sydney");
	v_ProvincesAndCities[6].pushBack("Yarmouth");

	v_ProvincesAndCities[7].pushBack("Nunavut");
	v_ProvincesAndCities[7].pushBack("Iqaluit");

	v_ProvincesAndCities[8].pushBack("Ontario");
	v_ProvincesAndCities[8].pushBack("Bancroft");
	v_ProvincesAndCities[8].pushBack("Barrie");
	v_ProvincesAndCities[8].pushBack("Belleville");
	v_ProvincesAndCities[8].pushBack("Brampton");
	v_ProvincesAndCities[8].pushBack("Brantford");
	v_ProvincesAndCities[8].pushBack("Brockville");
	v_ProvincesAndCities[8].pushBack("Burlington");
	v_ProvincesAndCities[8].pushBack("Cambridge");
	v_ProvincesAndCities[8].pushBack("Chatham");
	v_ProvincesAndCities[8].pushBack("Chatham-Kent");
	v_ProvincesAndCities[8].pushBack("Cornwall");
	v_ProvincesAndCities[8].pushBack("Elliot Lake");
	v_ProvincesAndCities[8].pushBack("Etobicoke");
	v_ProvincesAndCities[8].pushBack("Fort Erie");
	v_ProvincesAndCities[8].pushBack("Fort Frances");
	v_ProvincesAndCities[8].pushBack("Gananoque");
	v_ProvincesAndCities[8].pushBack("Guelph");
	v_ProvincesAndCities[8].pushBack("Hamilton");
	v_ProvincesAndCities[8].pushBack("Iroquois Falls");
	v_ProvincesAndCities[8].pushBack("Kapuskasing");
	v_ProvincesAndCities[8].pushBack("Kawartha Lakes");
	v_ProvincesAndCities[8].pushBack("Kenora");
	v_ProvincesAndCities[8].pushBack("Kingston");
	v_ProvincesAndCities[8].pushBack("Kirkland Lake");
	v_ProvincesAndCities[8].pushBack("Kitchener");
	v_ProvincesAndCities[8].pushBack("Laurentian Hills");
	v_ProvincesAndCities[8].pushBack("London");
	v_ProvincesAndCities[8].pushBack("Midland");
	v_ProvincesAndCities[8].pushBack("Mississauga");
	v_ProvincesAndCities[8].pushBack("Moose Factory");
	v_ProvincesAndCities[8].pushBack("Moosonee");
	v_ProvincesAndCities[8].pushBack("Niagara Falls");
	v_ProvincesAndCities[8].pushBack("Niagara-on-the-Lake");
	v_ProvincesAndCities[8].pushBack("North Bay");
	v_ProvincesAndCities[8].pushBack("North York");
	v_ProvincesAndCities[8].pushBack("Oakville");
	v_ProvincesAndCities[8].pushBack("Orillia");
	v_ProvincesAndCities[8].pushBack("Oshawa");
	v_ProvincesAndCities[8].pushBack("Ottawa");
	v_ProvincesAndCities[8].pushBack("Parry Sound");
	v_ProvincesAndCities[8].pushBack("Perth");
	v_ProvincesAndCities[8].pushBack("Peterborough");
	v_ProvincesAndCities[8].pushBack("Picton");
	v_ProvincesAndCities[8].pushBack("Port Colborne");
	v_ProvincesAndCities[8].pushBack("Saint Catharines");
	v_ProvincesAndCities[8].pushBack("Saint Thomas");
	v_ProvincesAndCities[8].pushBack("Sarnia-Clearwater");
	v_ProvincesAndCities[8].pushBack("Sault Sainte Marie");
	v_ProvincesAndCities[8].pushBack("Scarborough");
	v_ProvincesAndCities[8].pushBack("Simcoe");
	v_ProvincesAndCities[8].pushBack("Stratford");
	v_ProvincesAndCities[8].pushBack("Sudbury");
	v_ProvincesAndCities[8].pushBack("Temiskaming Shores");
	v_ProvincesAndCities[8].pushBack("Thorold");
	v_ProvincesAndCities[8].pushBack("Thunder Bay");
	v_ProvincesAndCities[8].pushBack("Timmins");
	v_ProvincesAndCities[8].pushBack("Toronto");
	v_ProvincesAndCities[8].pushBack("Trenton");
	v_ProvincesAndCities[8].pushBack("Waterloo");
	v_ProvincesAndCities[8].pushBack("Welland");
	v_ProvincesAndCities[8].pushBack("West Nipissing");
	v_ProvincesAndCities[8].pushBack("Windsor");
	v_ProvincesAndCities[8].pushBack("Woodstock");
	v_ProvincesAndCities[8].pushBack("York");

	v_ProvincesAndCities[9].pushBack("Prince Edward Island");
	v_ProvincesAndCities[9].pushBack("Borden");
	v_ProvincesAndCities[9].pushBack("Cavendish");
	v_ProvincesAndCities[9].pushBack("Charlottetown");
	v_ProvincesAndCities[9].pushBack("Souris");
	v_ProvincesAndCities[9].pushBack("Summerside");

	v_ProvincesAndCities[10].pushBack("Quebec");
	v_ProvincesAndCities[10].pushBack("Asbestos");
	v_ProvincesAndCities[10].pushBack("Baie-Comeau");
	v_ProvincesAndCities[10].pushBack("Beloeil");
	v_ProvincesAndCities[10].pushBack("Cap-de-la-Madeleine");
	v_ProvincesAndCities[10].pushBack("Chambly");
	v_ProvincesAndCities[10].pushBack("Charlesbourg");
	v_ProvincesAndCities[10].pushBack("Châteauguay");
	v_ProvincesAndCities[10].pushBack("Chibougamau");
	v_ProvincesAndCities[10].pushBack("Côte-Saint-Luc");
	v_ProvincesAndCities[10].pushBack("Dorval");
	v_ProvincesAndCities[10].pushBack("Gaspé");
	v_ProvincesAndCities[10].pushBack("Gatineau");
	v_ProvincesAndCities[10].pushBack("Granby");
	v_ProvincesAndCities[10].pushBack("Havre-Saint-Pierre");
	v_ProvincesAndCities[10].pushBack("Hull");
	v_ProvincesAndCities[10].pushBack("Jonquière");
	v_ProvincesAndCities[10].pushBack("Kuujjuaq");
	v_ProvincesAndCities[10].pushBack("La Salle");
	v_ProvincesAndCities[10].pushBack("La Tuque");
	v_ProvincesAndCities[10].pushBack("Lachine");
	v_ProvincesAndCities[10].pushBack("Laval");
	v_ProvincesAndCities[10].pushBack("Lévis");
	v_ProvincesAndCities[10].pushBack("Longueuil");
	v_ProvincesAndCities[10].pushBack("Magog");
	v_ProvincesAndCities[10].pushBack("Matane");
	v_ProvincesAndCities[10].pushBack("Montreal");
	v_ProvincesAndCities[10].pushBack("Montréal-Nord");
	v_ProvincesAndCities[10].pushBack("Percé");
	v_ProvincesAndCities[10].pushBack("Port-Cartier");
	v_ProvincesAndCities[10].pushBack("Quebec");
	v_ProvincesAndCities[10].pushBack("Rimouski");
	v_ProvincesAndCities[10].pushBack("Rouyn-Noranda");
	v_ProvincesAndCities[10].pushBack("Saguenay");
	v_ProvincesAndCities[10].pushBack("Saint-Eustache");
	v_ProvincesAndCities[10].pushBack("Saint-Hubert");
	v_ProvincesAndCities[10].pushBack("Sainte-Anne-de-Beaupré");
	v_ProvincesAndCities[10].pushBack("Sainte-Foy");
	v_ProvincesAndCities[10].pushBack("Sainte-Thérèse");
	v_ProvincesAndCities[10].pushBack("Sept-Îles");
	v_ProvincesAndCities[10].pushBack("Sherbrooke");
	v_ProvincesAndCities[10].pushBack("Sorel-Tracy");
	v_ProvincesAndCities[10].pushBack("Trois-Rivières");
	v_ProvincesAndCities[10].pushBack("Val-d’Or");
	v_ProvincesAndCities[10].pushBack("Waskaganish");

	v_ProvincesAndCities[11].pushBack("Saskatchewan");
	v_ProvincesAndCities[11].pushBack("Batoche");
	v_ProvincesAndCities[11].pushBack("Cumberland House");
	v_ProvincesAndCities[11].pushBack("Estevan");
	v_ProvincesAndCities[11].pushBack("Flin Flon");
	v_ProvincesAndCities[11].pushBack("Moose Jaw");
	v_ProvincesAndCities[11].pushBack("Prince Albert");
	v_ProvincesAndCities[11].pushBack("Regina");
	v_ProvincesAndCities[11].pushBack("Saskatoon");
	v_ProvincesAndCities[11].pushBack("Uranium City");

	v_ProvincesAndCities[12].pushBack("Yukon");
	v_ProvincesAndCities[12].pushBack("Dawson");
	v_ProvincesAndCities[12].pushBack("Watson Lake");
	v_ProvincesAndCities[12].pushBack("Whitehorse");
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