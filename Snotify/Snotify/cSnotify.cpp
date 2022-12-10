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

UserLibrary* cSnotify::getUserLibrary(unsigned int SnotifyUserID) {
	bool userFound = false;
	bool songIndex;
	UserLibrary* theLib = nullptr;
	// Iterates through usersSongLibrary array
	for (int i = 0; i < v_usersSongLib.getSize(); i++) {
		theLib = v_usersSongLib.getAt(i);
		// Checks if userID matches
		if (theLib->theUser->getSnotifyUniqueUserID() == SnotifyUserID) {
			userFound = true;
			break;
		}
	}
	if (userFound) { // The user Lib
		// Returns it
		return theLib;
	} else {
		return nullptr;
	}
}

UserSongInfo* cSnotify::getUserSongInfo(unsigned int SnotifyUserID, unsigned int songUniqueID) {
	UserLibrary* theLib = getUserLibrary(SnotifyUserID);
	if (theLib) {
		bool songFound = false;
		bool songIndex;
		UserSongInfo* userSongInfo = nullptr;
		
		// Iterates through usersSongLibrary array
		for (int i = 0; i < theLib->songLibrary.getSize(); i++) {
			userSongInfo = theLib->songLibrary.getAt(i);
			// Checks if songID matches
			if (userSongInfo->theSong->getUniqueID() == songUniqueID) {
				songFound = true;
				songIndex = i;
				break;
			}
		}
		if (songFound) {
			return userSongInfo;
		} else {
			return nullptr;
		}
	}
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
	index = v_usersSongLib.getSize() - 1;
	while (index >= 0) {
		// Delete the user lib pointer
		delete v_usersSongLib.getAt(index);
		// Removes the node pointing to null from the list
		v_usersSongLib.removeAt(index);
		index--;
	}
}

bool cSnotify::AddUser(cPerson* pPerson, std::string& errorString) {
	cPerson* newPerson = new cPerson();
	(*newPerson) = (*pPerson);
	// Checks if its nullptr
	if (newPerson != nullptr) {
		if (newPerson->age <= 0) {
			errorString = "This person has an invalid age.";
			return false;
		} else if (newPerson->first.empty() || newPerson->last.empty()) {
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
				// Creates a song library for new user
				UserLibrary* newlib = new UserLibrary();
				// Links the library to the user
				newlib->theUser = newPerson;
				// Adds it to the array
				this->v_usersSongLib.pushBack(newlib);
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
					// Creates a song library for new user
					UserLibrary* newlib = new UserLibrary();
					// Links the library to the user
					newlib->theUser = newPerson;
					// Adds it to the array
					this->v_usersSongLib.pushBack(newlib);
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
	if (pPerson->age <= 0) {
		errorString = "The person has an invalid age.";
		return false;
	} else if (pPerson->first.empty() || pPerson->last.empty()) {
		errorString = "This person has no first or last name.";
		return false;
	} else if (pPerson->city.empty() || pPerson->province.empty()) {
		errorString = "This person has no province or city informed.";
		return false;
	} else if (pPerson->streetNumber < 0) {
		errorString = "This person street number is invalid. (Less than zero)";
		return false;
	} else if (pPerson->streetName.empty() || pPerson->streetType.empty()) {
		errorString = "This person street name or type is empty.";
		return false;
	}
	// Iterates through users array
	for (int i = 0; i < v_users.getSize(); i++) {
		thePerson = v_users.getAt(i);
		// Checks if SIN and ID matches
		if (thePerson->SIN == pPerson->SIN && thePerson->getSnotifyUniqueUserID() == pPerson->getSnotifyUniqueUserID()) {
			found = true;
			break;
		}
	}
	if (found) {
		// Copy the new date over
		(*thePerson) = (*pPerson);
		return true;
	} else {
		errorString = "No existing user on Snotify has the same SIN and ID. Updated failed.";
		return false;
	}
}

bool cSnotify::DeleteUser(unsigned int SnotifyUserID, std::string& errorString) {
	cPerson* thePerson = FindUserBySnotifyID(SnotifyUserID);
	if (thePerson) {
		UserLibrary* userLib = getUserLibrary(SnotifyUserID);
		// Removes every song on the user library
		if (userLib) {
			int index = userLib->songLibrary.getSize() - 1;
			while (index >= 0) {
				// Delete the song pointer
				delete userLib->songLibrary.getAt(index);
				// Removes the node pointing to null from the list
				userLib->songLibrary.removeAt(index);
				index--;
			}
		}
		// Deletes the library
		v_usersSongLib.remove(userLib);
		delete userLib;
		// Deletes the user
		v_users.remove(thePerson);
		delete thePerson;
	} else {
		errorString = "User not found.";
		return false;
	}
	return true;
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

bool cSnotify::UpdateSong(cSong* pSong, std::string& errorString) {
	cSong* theSong = FindSong(pSong->getUniqueID());
	if (theSong) {

	}
	return false;
}

bool cSnotify::DeleteSong(unsigned int UniqueSongID, std::string& errorString) {
	UserLibrary* userLib;
	UserSongInfo* userSongInfo;
	// Gets the song
	cSong* theSong = FindSong(UniqueSongID);
	// Checks if the song exists
	if (theSong) {
		// Now lets iterates through all libraries and remove them
		for (int i = 0; i < v_usersSongLib.getSize(); i++) {
			userLib = v_usersSongLib.getAt(i);
			for (int j = 0; j < userLib->songLibrary.getSize(); j++) {
				userSongInfo = userLib->songLibrary.getAt(j);
				// Checks if theres Song if
				if (userSongInfo) {
					if (userSongInfo->theSong->getUniqueID() == UniqueSongID) {
						// Removes the SongInfo on User Lib
						delete userSongInfo;
						// Clears the slot on the array
						userLib->songLibrary.removeAt(j);
					}
				}
			}
		} // All data removed from user libraries
		// Now we remove the song
		v_songs.remove(theSong);
		delete theSong;
		return true;
	} else {
		errorString = "The song to be deleted doesn't exist on the Snotify.";
		return false;
	}
	return true;
}

bool cSnotify::AddSongToUserLibrary(unsigned int snotifyUserID, cSong* pNewSong, std::string& errorString) {
	// Gets the user library
	UserLibrary* theLib = getUserLibrary(snotifyUserID);
	if (theLib) {
		// Creates a new song info node
		UserSongInfo* newSongInfo = new UserSongInfo();
		// Links the song to it
		newSongInfo->theSong = pNewSong;
		newSongInfo->rating = 0;
		newSongInfo->numberOfTimesPlayed = 0;
		// Pushes it back to the userSongLibrary
		theLib->songLibrary.pushBack(newSongInfo);
		return true;
	} else {
		errorString = "No user library was found with the informed userID.";
		return false;
	}
}

bool cSnotify::RemoveSongFromUserLibrary(unsigned int snotifyUserID, unsigned int SnotifySongID, std::string& errorString) {
	// Gets the user library
	UserLibrary* theLib = getUserLibrary(snotifyUserID);
	// Gets the user song info
	UserSongInfo* userSongInfo = getUserSongInfo(snotifyUserID, SnotifySongID);
	if (theLib) { // The user Lib
		if (userSongInfo) { // The song info on user lib
			theLib->songLibrary.remove(userSongInfo);
			delete userSongInfo;
			return true;
		} else {
			errorString = "No song was found on the user library with the informed songID.";
			return false;
		}
	} else {
		errorString = "No user library was found with the informed userID.";
		return false;
	}
}

bool cSnotify::UpdateRatingOnSong(unsigned int SnotifyUserID, unsigned int songUniqueID, unsigned int newRating) {
	// Gets the user song info
	UserSongInfo* userSongInfo = getUserSongInfo(SnotifyUserID, songUniqueID);
	// Checks if its not null
	if (userSongInfo) {
		// Updates new Rating
		userSongInfo->rating = newRating;
		return true;
	} else {
		return false;
	}
}

cSong* cSnotify::GetSong(unsigned int SnotifyUserID, unsigned int songUniqueID, std::string& errorString) {
	cSong* theSong = new cSong();
	// Copies the value of the song
	(*theSong) = (*FindSong(songUniqueID));
	if (theSong) {
		// Checks if the user has the song on his library
		UserSongInfo* userSongInfo = getUserSongInfo(SnotifyUserID, songUniqueID);
		if (userSongInfo) {
			// Increments the numberOfTimesPlayed on the user library
			userSongInfo->numberOfTimesPlayed++;
			// Updates the rating and numberOfTimesPlayed on the song
			theSong->rating = userSongInfo->rating;
			theSong->numberOfTimesPlayed = userSongInfo->numberOfTimesPlayed;
			// Returns the song
			return theSong;
		} else {
			errorString = "The informed user doesn't have that song on his library.";
			return nullptr;
		}
	} else {
		errorString = "No song matching the informed ID was found.";
		return nullptr;
	}
}

bool cSnotify::GetCurrentSongRating(unsigned int snotifyUserID, unsigned int songUniqueID, unsigned int& songRating) {
	// Gets the user song info
	UserSongInfo* userSongInfo = getUserSongInfo(snotifyUserID, songUniqueID);
	// Checks if its not null
	if (userSongInfo) {
		// Pass the rating by reference
		songRating = userSongInfo->rating;
		return true;
	} else {
		return false;
	}
}

bool cSnotify::GetCurrentSongNumberOfPlays(unsigned int snotifyUserID, unsigned int songUniqueID, unsigned int& numberOfPlays) {
	// Gets the user song info
	UserSongInfo* userSongInfo = getUserSongInfo(snotifyUserID, songUniqueID);
	// Checks if its not null
	if (userSongInfo) {
		// Pass the numberOfPlays by reference
		numberOfPlays = userSongInfo->numberOfTimesPlayed;
		return true;
	} else {
		return false;
	}
}

cPerson* cSnotify::FindUserBySIN(unsigned int SIN) {
	bool found = false;
	cPerson* thePerson = nullptr;
	// Iterates through users array
	for (int i = 0; i < v_users.getSize(); i++) {
		thePerson = v_users.getAt(i);
		// Checks if SIN matches
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
	// Iterates through users array
	for (int i = 0; i < v_users.getSize(); i++) {
		thePerson = v_users.getAt(i);
		// Checks if ID matches
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
	// Iterates through songs array
	for (int i = 0; i < v_songs.getSize(); i++) {
		theSong = v_songs.getAt(i);
		// Checks if title and artist matches
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
	// Iterates through songs array
	for (int i = 0; i < v_songs.getSize(); i++) {
		theSong = v_songs.getAt(i);
		// Checks if ID matches
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
