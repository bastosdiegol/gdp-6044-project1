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
	UserLibrary* userLib = getUserLibrary(snotifyUserID);

	sizeOfLibary = userLib->songLibrary.getSize();

	if (sizeOfLibary > 0) {
		pLibraryArray = new cSong[sizeOfLibary];
		// The array and the size of the array are "returned" by reference to the caller. 

		// TODO: Copy all the songs over
		// Copies each user form the ordered to the array
		for (int i = 0; i < sizeOfLibary; i++) {
			UserSongInfo* songInfo = nullptr;
			songInfo = userLib->songLibrary.getAt(i);
			pLibraryArray[i] = (*songInfo->theSong);
			pLibraryArray[i].numberOfTimesPlayed = songInfo->numberOfTimesPlayed;
			pLibraryArray[i].rating = songInfo->rating;
		}
		return true;
	} else {
		return false;
	}
	
}

bool cSnotify::GetUsersSongLibraryAscendingByTitle(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary) {
	UserLibrary* userLib = getUserLibrary(snotifyUserID);

	sizeOfLibary = userLib->songLibrary.getSize();

	if (sizeOfLibary > 0) {
		pLibraryArray = new cSong[sizeOfLibary];

		tDLList<cSong*> songsByTitle;

		// Now we are going to iterate the songs array
		// But we are going to copy ordered
		for (int i = 0; i < sizeOfLibary; i++) {
			// Copies the song to the array to be ordered
			UserSongInfo* songInfo = userLib->songLibrary.getAt(i);
			cSong* theSong = new cSong();
			(*theSong) = (*songInfo->theSong);
			// Copies the song info from users lib to the song
			theSong->numberOfTimesPlayed = songInfo->numberOfTimesPlayed;
			theSong->rating = songInfo->rating;

			// First element
			if (songsByTitle.getSize() == 0) {
				songsByTitle.pushBack(theSong);
			} else {
				bool inserted = false;
				// Now we gonna iterate through the ordered array
				for (int j = 0; j < songsByTitle.getSize(); j++) {
					cSong* theSongInOrder = songsByTitle.getAt(j);
					// Checks if its different song
					if (theSong->getUniqueID() != theSongInOrder->getUniqueID()) {
						// Compares alphabetically the Title
						if (theSong->name.compare(theSongInOrder->name) < 0) {
							songsByTitle.addAt(j, theSong);
							inserted = true;
							break;
						} 
					} else {
						inserted = true; // Song already inserted
						break;
					}
				}
				// After the loop, if no insertion put at the end
				if (inserted == false) {
					songsByTitle.pushBack(theSong);
				}
			}
		}

		// Copies the ordered array to the reference variable
		for (int i = 0; i < sizeOfLibary; i++) {
			pLibraryArray[i] = (*songsByTitle.getAt(i));
		}

		// Clears the ordered array
		int index = songsByTitle.getSize() - 1;
		while (index >= 0) {
			// Delete the song pointer
			delete songsByTitle.getAt(index);
			// Removes the node pointing to null from the list
			songsByTitle.removeAt(index);
			index--;
		}

		return true;
	} else {
		return false;
	}
}

bool cSnotify::GetUsersSongLibraryAscendingByArtist(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary) {
	UserLibrary* userLib = getUserLibrary(snotifyUserID);

	sizeOfLibary = userLib->songLibrary.getSize();

	if (sizeOfLibary > 0) {
		pLibraryArray = new cSong[sizeOfLibary];

		tDLList<cSong*> songsByTitle;

		// Now we are going to iterate the songs array
		// But we are going to copy ordered
		for (int i = 0; i < sizeOfLibary; i++) {
			// Copies the song to the array to be ordered
			UserSongInfo* songInfo = userLib->songLibrary.getAt(i);
			cSong* theSong = new cSong();
			(*theSong) = (*songInfo->theSong);
			// Copies the song info from users lib to the song
			theSong->numberOfTimesPlayed = songInfo->numberOfTimesPlayed;
			theSong->rating = songInfo->rating;

			// First element
			if (songsByTitle.getSize() == 0) {
				songsByTitle.pushBack(theSong);
			} else {
				bool inserted = false;
				// Now we gonna iterate through the ordered array
				for (int j = 0; j < songsByTitle.getSize(); j++) {
					cSong* theSongInOrder = songsByTitle.getAt(j);
					// Checks if its different song
					if (theSong->getUniqueID() != theSongInOrder->getUniqueID()) {
						// Compares alphabetically the Artist
						if (theSong->artist.compare(theSongInOrder->artist) < 0) {
							songsByTitle.addAt(j, theSong);
							inserted = true;
							break;
						}
					} else {
						inserted = true; // Song already inserted
						break;
					}
				}
				// After the loop, if no insertion put at the end
				if (inserted == false) {
					songsByTitle.pushBack(theSong);
				}
			}
		}

		// Copies the ordered array to the reference variable
		for (int i = 0; i < sizeOfLibary; i++) {
			pLibraryArray[i] = (*songsByTitle.getAt(i));
		}

		// Clears the ordered array
		int index = songsByTitle.getSize() - 1;
		while (index >= 0) {
			// Delete the song pointer
			delete songsByTitle.getAt(index);
			// Removes the node pointing to null from the list
			songsByTitle.removeAt(index);
			index--;
		}

		return true;
	} else {
		return false;
	}
}

bool cSnotify::GetUsers(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	// Gets the size of the user array
	sizeOfUserArray = v_users.getSize();
	// Instantiate the array with that size
	pAllTheUsers = new cPerson[sizeOfUserArray];
	// Copies each user to the array
	for (int i = 0; i < v_users.getSize(); i++) {
		pAllTheUsers[i] = (*v_users.getAt(i));
	}
	return true;
}

bool cSnotify::GetUsersByID(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	// Gets the size of the user array
	sizeOfUserArray = v_users.getSize();
	// Instantiate the array with that size
	pAllTheUsers = new cPerson[sizeOfUserArray];

	tDLList<cPerson*> usersByID;

	// Now we are going to iterate the users array and copy each node to v_usersByID
	// But we are going to copy ordered
	for (int i = 0; i < v_users.getSize(); i++) {
		// First element
		if (i == 0) {
			usersByID.pushBack(v_users.getAt(i));
		} else {
			cPerson* thePerson = v_users.getAt(i);
			bool inserted = false;
			// Now we gonna iterate through the ordered array
			for (int j = 0; j < usersByID.getSize(); j++) {
				cPerson* thePersonInOrder = usersByID.getAt(j);
				// The current person the be added has lower ID
				if (thePerson->getSnotifyUniqueUserID() < thePersonInOrder->getSnotifyUniqueUserID()) {
					usersByID.addAt(j, thePerson);
					inserted = true;
					break;
				}
			}
			// After the loop no user was found with higher ID
			if (inserted == false) {
				usersByID.pushBack(thePerson);
			}
		}
	}

	// Copies each user form the ordered to the array
	for (int i = 0; i < usersByID.getSize(); i++) {
		pAllTheUsers[i] = (*usersByID.getAt(i));
	}

	// Clears the ordered array
	int index = usersByID.getSize() - 1;
	while (index >= 0) {
		// Delete the song pointer
		delete usersByID.getAt(index);
		// Removes the node pointing to null from the list
		usersByID.removeAt(index);
		index--;
	}

	return true;
}

bool cSnotify::FindUsersFirstName(std::string firstName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	tDLList<cPerson*> usersByFirstName;

	// Now we are going to iterate the users array and copy each node to v_usersByID
	// But we are going to copy ordered
	for (int i = 0; i < v_users.getSize(); i++) {
		cPerson* thePerson = nullptr;
		thePerson = v_users.getAt(i);
		if (thePerson->first == firstName) {
			// First element
			if (usersByFirstName.getSize() == 0) {
				usersByFirstName.pushBack(thePerson);
			} else {
				bool inserted = false;
				// Now we gonna iterate through the ordered array
				for (int j = 0; j < usersByFirstName.getSize(); j++) {
					cPerson* thePersonInOrder = nullptr;
					thePersonInOrder = usersByFirstName.getAt(j);
					// Checks if its different user
					if (thePerson->getSnotifyUniqueUserID() != thePersonInOrder->getSnotifyUniqueUserID()) {
						// Compares alphabetically the Middle name
						if (thePerson->middle.compare(thePersonInOrder->middle) < 0) {
							usersByFirstName.addAt(j, thePerson);
							inserted = true;
							break;
							// If its equal, compare the Last name
						} else if (thePerson->middle.compare(thePersonInOrder->middle) == 0 &&
							thePerson->last.compare(thePersonInOrder->last) < 0) {
							usersByFirstName.addAt(j, thePerson);
							inserted = true;
							break;
						}
					} else {
						inserted = true; // User already exists
						break;
					}
				}
				// After the loop, if no insertion put at the end
				if (inserted == false) {
					usersByFirstName.pushBack(thePerson);
				}
			}
		}
		
	}
	
	// Checks if theres person with the desired name
	if (usersByFirstName.getSize() > 0) {
		// Gets the size of the user array
		sizeOfUserArray = usersByFirstName.getSize();
		// Instantiate the array with that size
		pAllTheUsers = new cPerson[sizeOfUserArray];

		// Copies each user form the ordered to the array
		for (int i = 0; i < usersByFirstName.getSize(); i++) {
			pAllTheUsers[i] = (*usersByFirstName.getAt(i));
		}

		// Clears the ordered array
		int index = usersByFirstName.getSize() - 1;
		while (index >= 0) {
			// Delete the song pointer
			delete usersByFirstName.getAt(index);
			// Removes the node pointing to null from the list
			usersByFirstName.removeAt(index);
			index--;
		}

		return true;
	} else {
		sizeOfUserArray = 0;
		return false;
	}
	
}

bool cSnotify::FindUsersLastName(std::string lastName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	tDLList<cPerson*> usersByLastName;

	// Now we are going to iterate the users array and copy each node to v_usersByID
	// But we are going to copy ordered
	for (int i = 0; i < v_users.getSize(); i++) {
		cPerson* thePerson = nullptr;
		thePerson = v_users.getAt(i);
		if (thePerson->last == lastName) {
			// First element
			if (usersByLastName.getSize() == 0) {
				usersByLastName.pushBack(thePerson);
			} else {
				bool inserted = false;
				// Now we gonna iterate through the ordered array
				for (int j = 0; j < usersByLastName.getSize(); j++) {
					cPerson* thePersonInOrder = nullptr;
					thePersonInOrder = usersByLastName.getAt(j);
					// Checks if its different user
					if (thePerson->getSnotifyUniqueUserID() != thePersonInOrder->getSnotifyUniqueUserID()) {
						// Compares alphabetically the Middle name
						if (thePerson->middle.compare(thePersonInOrder->middle) < 0) {
							usersByLastName.addAt(j, thePerson);
							inserted = true;
							break;
							// If its equal, compare the Last name
						} else if (thePerson->middle.compare(thePersonInOrder->middle) == 0 &&
							thePerson->last.compare(thePersonInOrder->last) < 0) {
							usersByLastName.addAt(j, thePerson);
							inserted = true;
							break;
						}
					} else {
						inserted = true; // User already exists
						break;
					}
				}
				// After the loop, if no insertion put at the end
				if (inserted == false) {
					usersByLastName.pushBack(thePerson);
				}
			}
		}

	}

	// Checks if theres person with desired last name
	if (usersByLastName.getSize() > 0) {
		// Gets the size of the user array
		sizeOfUserArray = usersByLastName.getSize();
		// Instantiate the array with that size
		pAllTheUsers = new cPerson[sizeOfUserArray];

		// Copies each user form the ordered to the array
		for (int i = 0; i < usersByLastName.getSize(); i++) {
			pAllTheUsers[i] = (*usersByLastName.getAt(i));
		}

		// Clears the ordered array
		int index = usersByLastName.getSize() - 1;
		while (index >= 0) {
			// Delete the song pointer
			delete usersByLastName.getAt(index);
			// Removes the node pointing to null from the list
			usersByLastName.removeAt(index);
			index--;
		}

		return true;
	} else {
		sizeOfUserArray = 0;
		return false;
	}
	
}

bool cSnotify::FindUsersFirstLastNames(std::string firstName, std::string lastName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	tDLList<cPerson*> usersByLastName;

	// Now we are going to iterate the users array and copy each node to v_usersByID
	// But we are going to copy ordered
	for (int i = 0; i < v_users.getSize(); i++) {
		cPerson* thePerson = v_users.getAt(i);
		if (thePerson->first == firstName && thePerson->last == lastName) {
			// First element
			if (usersByLastName.getSize() == 0) {
				usersByLastName.pushBack(thePerson);
			} else {
				bool inserted = false;
				// Now we gonna iterate through the ordered array
				for (int j = 0; j < usersByLastName.getSize(); j++) {
					cPerson* thePersonInOrder = usersByLastName.getAt(j);
					// Checks if its different user
					if (thePerson->getSnotifyUniqueUserID() != thePersonInOrder->getSnotifyUniqueUserID()) {
						// Compares alphabetically the Middle name
						if (thePerson->middle.compare(thePersonInOrder->middle) < 0) {
							usersByLastName.addAt(j, thePerson);
							inserted = true;
							break;
							// If its equal, compare the Last name
						} else if (thePerson->middle.compare(thePersonInOrder->middle) == 0 &&
							thePerson->last.compare(thePersonInOrder->last) < 0) {
							usersByLastName.addAt(j, thePerson);
							inserted = true;
							break;
						}
					} else {
						inserted = true; // User already exists
						break;
					}
				}
				// After the loop, if no insertion put at the end
				if (inserted == false) {
					usersByLastName.pushBack(thePerson);
				}
			}
		}

	}

	// Checks if theres person with desired last name
	if (usersByLastName.getSize() > 0) {
		// Gets the size of the user array
		sizeOfUserArray = usersByLastName.getSize();
		// Instantiate the array with that size
		pAllTheUsers = new cPerson[sizeOfUserArray];

		// Copies each user form the ordered to the array
		for (int i = 0; i < usersByLastName.getSize(); i++) {
			pAllTheUsers[i] = (*usersByLastName.getAt(i));
		}

		// Clears the ordered array
		int index = usersByLastName.getSize() - 1;
		while (index >= 0) {
			// Delete the song pointer
			delete usersByLastName.getAt(index);
			// Removes the node pointing to null from the list
			usersByLastName.removeAt(index);
			index--;
		}

		return true;
	} else {
		sizeOfUserArray = 0;
		return false;
	}
}

UserLibrary* cSnotify::getUserLibrary(unsigned int SnotifyUserID) {
	bool userFound = false;
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
					cPerson* thePerson = nullptr;
					thePerson = v_users.getAt(i);
					if (thePerson->SIN == newPerson->SIN || thePerson->getSnotifyUniqueUserID() == newPerson->getSnotifyUniqueUserID()) {
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
					cSong* theSong = nullptr;
					theSong = v_songs.getAt(i);
					if (((theSong->name.compare(newSong->name) == 0) &&
						(theSong->artist.compare(newSong->artist) == 0)) ||
						theSong->getUniqueID() == newSong->getUniqueID()) {
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
	UserLibrary* userLib = nullptr;
	UserSongInfo* userSongInfo = nullptr;
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


