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
	int index = v_songs.getSize() -1;
	// All songs pointers are going to be deleted at ~cMusicGenerator
	// So here we just delete our list
	v_songs.~tDLList();
}

bool cSnotify::AddSong(cSong* pSong, std::string& errorString) {
	// Checks if its nullptr
	if (pSong != nullptr) {
		if (pSong->artist.empty() || pSong->name.empty()) {
			errorString = "The song passed has no artist or name.";
			return false;
		}else{
			bool duplicated = false;
			// First Song
			if (v_songs.getSize() == 0) {
				this->v_songs.pushBack(pSong);
				return true;
			} else {
				// Checks if it already exists
				for (int i = 0; i < v_songs.getSize(); i++) {
					cSong* theSong = v_songs.getAt(i);
					if ((theSong->name.compare(pSong->name) == 0) && 
						(theSong->artist.compare(pSong->artist) == 0)) {
						duplicated = true;
					}
				}
				if (duplicated) {
					// Exists - Errors
					errorString = "This song already exists at Snotify!";
					return false;
				} else {
					// Doesn't exists
					this->v_songs.pushBack(pSong);
					return true;
				}
			}			
		}
	} else {
		errorString = "Dear user, if you want to add a song, please pass a song...";
		return false;
	}
}
