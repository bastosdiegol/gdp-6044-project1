#include "cPerson.h"

// Warning C26812 : Prefer 'enum class' over 'enum' (Enum.3)
#pragma warning( disable : 26812 )

cPerson::cPerson()
{
	// In here, set the default information
	this->gender = cPerson::RATHER_NOT_SAY_UNKNOWN;
	this->streetNumber = 0;
	this->age = -1;
	this->SIN = 0;

	// Generate unique Snotify ID
	this->m_Snotify_UniqueUserID = cPerson::m_NEXT_Snotify_UniqueUSerID;
	// Increment for next created user by a small random amount 
	const unsigned int MAX_ID_INCREEMNT = 5;
	cPerson::m_NEXT_Snotify_UniqueUSerID += (rand() % MAX_ID_INCREEMNT);
}

cPerson::cPerson(const cPerson& rhs) {
	this->age = rhs.age;
	this->city = rhs.city;
	this->first = rhs.first;
	this->gender = rhs.gender;
	this->last = rhs.last;
	this->middle = rhs.middle;
	this->postalCode[0] = rhs.postalCode[0];
	this->postalCode[1] = rhs.postalCode[1];
	this->postalCode[2] = rhs.postalCode[2];
	this->postalCode[3] = rhs.postalCode[3];
	this->postalCode[4] = rhs.postalCode[4];
	this->postalCode[5] = rhs.postalCode[5];
	this->province = rhs.province;
	this->SIN = rhs.SIN;
	this->streetDirection = rhs.streetDirection;
	this->streetName = rhs.streetName;
	this->streetNumber = rhs.streetNumber;
	this->streetType = rhs.streetType;
	// Generate unique Snotify ID
	this->m_Snotify_UniqueUserID = cPerson::m_NEXT_Snotify_UniqueUSerID;
	// Increment for next created user by a small random amount 
	const unsigned int MAX_ID_INCREEMNT = 5;
	cPerson::m_NEXT_Snotify_UniqueUSerID += (rand() % MAX_ID_INCREEMNT);
}

// The 1st Snotify user will have ID: 10,000,000
// static 
unsigned int cPerson::m_NEXT_Snotify_UniqueUSerID = 1;


cPerson::~cPerson()
{
	
}

std::string cPerson::getGenderAsString(void)
{
	switch (this->gender)
	{
	case cPerson::MALE:
		return "MALE";
		break;
	case cPerson::FEMALE:
		return "FEMALE";
		break;
	case cPerson::NON_BINARY:
		return "NON_BINARY";
		break;
	case cPerson::RATHER_NOT_SAY_UNKNOWN:
		return "RATHER_NOT_SAY_UNKNOWN";
		break;
//	default:
//		break;
	}

	// This should never happen
//	return "RATHER_NOT_SAY_UNKNOWN";
	return "OMG! CALL THE DEV TEAM!! SOMEONE SCREWED UP!!!";
}

unsigned int cPerson::getSnotifyUniqueUserID(void) {
	return m_Snotify_UniqueUserID;
}
