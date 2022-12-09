#include "cSnotify.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif


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

	// Alloate a heap based array to hold all the songs...

//	sizeOfLibary = WhateverYouHaveToDoToGetThisValue();
//	pCopyOfLibrary = new cSong[sizeOfLibary];

	// The array and the size of the array are "returned" by reference to the caller. 

	// TODO: Copy all the songs over

	return true;
}

cSnotify::cSnotify() {
	DEBUG_PRINT("cSnotify::cSnotify()\n");
}

cSnotify::~cSnotify() {
	DEBUG_PRINT("cSnotify::~cSnotify()\n");
	int index = v_songs.getSize() - 1;
	while (index >= 0) {
		// Delete the song pointer
		delete v_songs.getAt(index);
		// Removes the node pointing to null from the list
		v_songs.removeAt(index);
		index--;
	}
	index = v_users.getSize() - 1;
	while (index >= 0) {
		// Delete the user pointer
		delete v_users.getAt(index);
		// Removes the node pointing to null from the list
		v_users.removeAt(index);
		index--;
	}
}

bool cSnotify::AddUser(cPerson* pPerson, std::string& errorString) {
	cPerson* newPerson = new cPerson();
	(*newPerson) = (*pPerson);
	// Checks if its nullptr
	if (newPerson != nullptr) {
		if (newPerson->first.empty() || newPerson->last.empty()) {
			errorString = "This person has no first or last name.";
			return false;
		} else if (newPerson->city.empty() || newPerson->province.empty()) {
			errorString = "This person has no province or city informed.";
			return false;
		} else if (newPerson->streetNumber < 0) {
			errorString = "This person street number is invalid. (Less than zero)";
			return false;
		} else if (newPerson->streetName.empty() || newPerson->streetType.empty()) {
			errorString = "This person street name or type is empty.";
			return false;
		} else {
			bool duplicated = false;
			// First Song
			if (v_users.getSize() == 0) {
				this->v_users.pushBack(newPerson);
				return true;
			} else {
				// Checks if it already exists
				for (int i = 0; i < v_users.getSize(); i++) {
					cPerson* thePerson = v_users.getAt(i);
					if (thePerson->SIN == newPerson->SIN) {
						duplicated = true;
						break;
					}
				}
				if (duplicated) {
					// Exists - Errors
					errorString = "A person with this same SIN number already existis on Snotify!";
					return false;
				} else {
					// Doesn't exists
					this->v_users.pushBack(newPerson);
					return true;
				}
			}
		}
	} else {
		errorString = "Dear user, if you want to add a pearson, please pass the person information...";
		return false;
	}
	return false;
}

bool cSnotify::UpdateUser(cPerson* pPerson, std::string& errorString) {
	bool found = false;
	cPerson* thePerson = nullptr;
	for (int i = 0; i < v_users.getSize(); i++) {
		thePerson = v_users.getAt(i);
		if (thePerson->SIN == pPerson->SIN && thePerson->getSnotifyUniqueUserID() == pPerson->getSnotifyUniqueUserID()) {
			found = true;
			break;
		}
	}
	if (found) {
		(*thePerson) = (*pPerson);
		return true;
	} else {
		errorString = "No existing user on Snotify has the same SIN and ID. Updated failed.";
		return false;
	}
}

bool cSnotify::AddSong(cSong* pSong, std::string& errorString) {
	cSong* newSong = new cSong();
	(*newSong) = (*pSong);
	// Checks if its nullptr
	if (newSong != nullptr) {
		if (newSong->artist.empty() || newSong->name.empty()) {
			errorString = "The song passed has no artist or name.";
			return false;
		}else{
			bool duplicated = false;
			// First Song
			if (v_songs.getSize() == 0) {
				this->v_songs.pushBack(newSong);
				return true;
			} else {
				// Checks if it already exists
				for (int i = 0; i < v_songs.getSize(); i++) {
					cSong* theSong = v_songs.getAt(i);
					if ((theSong->name.compare(newSong->name) == 0) &&
						(theSong->artist.compare(newSong->artist) == 0)) {
						duplicated = true;
					}
				}
				if (duplicated) {
					// Exists - Errors
					errorString = "This song already exists at Snotify!";
					return false;
				} else {
					// Doesn't exists
					this->v_songs.pushBack(newSong);
					return true;
				}
			}			
		}
	} else {
		errorString = "Dear user, if you want to add a song, please pass a song information...";
		return false;
	}
}

cPerson* cSnotify::FindUserBySIN(unsigned int SIN) {
	bool found = false;
	cPerson* thePerson = nullptr;
	for (int i = 0; i < v_users.getSize(); i++) {
		thePerson = v_users.getAt(i);
		if (thePerson->SIN == SIN) {
			found = true;
			break;
		}
	}
	if (found) {
		return thePerson;
	} else {
		return nullptr;
	}
}

cPerson* cSnotify::FindUserBySnotifyID(unsigned int SnotifyID) {
	bool found = false;
	cPerson* thePerson = nullptr;
	for (int i = 0; i < v_users.getSize(); i++) {
		thePerson = v_users.getAt(i);
		if (thePerson->getSnotifyUniqueUserID() == SnotifyID) {
			found = true;
			break;
		}
	}
	if (found) {
		return thePerson;
	} else {
		return nullptr;
	}
}

cSong* cSnotify::FindSong(std::string title, std::string artist) {
	bool found = false;
	cSong* theSong = nullptr;
	for (int i = 0; i < v_songs.getSize(); i++) {
		theSong = v_songs.getAt(i);
		if (theSong->name == title && theSong->artist == artist) {
			found = true;
			break;
		}
	}
	if (found) {
		return theSong;
	} else {
		return nullptr;
	}
}

cSong* cSnotify::FindSong(unsigned int uniqueID) {
	bool found = false;
	cSong* theSong = nullptr;
	for (int i = 0; i < v_songs.getSize(); i++) {
		theSong = v_songs.getAt(i);
		if (theSong->getUniqueID() == uniqueID) {
			found = true;
			break;
		}
	}
	if (found) {
		return theSong;
	} else {
		return nullptr;
	}
}
