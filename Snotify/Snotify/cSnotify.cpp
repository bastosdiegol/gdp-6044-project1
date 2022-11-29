#include "cSnotify.h"
#include <iostream>
#include <fstream>
#include <sstream>


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

bool cSnotify::LoadSongsFromFile(std::string filePath) {
	std::cout << "Now loading songs into the system..." << std::endl;
	// Open the file
	std::ifstream songsFile(filePath);
	if (!songsFile.is_open()) {
		std::cout << "Didn't open file" << std::endl;
		return false;
	}

	//	url, weekid, week_position, song, performer, songid, instance, previous_week_position, peak_position, weeks_on_chart
	//	http ://www.billboard.com/charts/hot-100/1958-08-02,1958-08-02,1,Poor Little Fool,Ricky Nelson,Poor Little FoolRicky Nelson,1,,1,1
	//	4th - song
	//	5th - performer

	std::string theLine;
	std::string token;
	std::string errorMessage;

	unsigned int lineCount = 0;
	// Discard the first line
	std::getline(songsFile, theLine);

	while (std::getline(songsFile, theLine)) {
		lineCount++;
		std::stringstream ssLine(theLine);

		unsigned int tokenCount = 0;

		cSong* newSong = new cSong();

		while (std::getline(ssLine, token, ',') && tokenCount < 5) {
			// 3rd Token = Song Name
			if (tokenCount == 3) {
				newSong->name = token;
			} else if (tokenCount == 4) { // 4th Token = Song Artist
				newSong->artist = token;
			}
			// Ignore the other parts of the line
			tokenCount++;
		}
		// Checks if the song was succesfully added - Print error message if anything goes wrong
		if (AddSong(newSong, errorMessage) == false) {
			std::cout << "Error when adding a song ID[" << newSong->getUniqueID() << "] at line " << lineCount << std::endl;
			std::cout << "|- " << errorMessage << std::endl;
		}
	}

	std::cout << "All songs loaded into the system." << std::endl;
	std::cout << "Songs added: " << lineCount << std::endl;

	return true;
}

cSnotify::cSnotify() {
}

cSnotify::~cSnotify() {
}

bool cSnotify::AddSong(cSong* pSong, std::string& errorString) {
	if (pSong != nullptr) {
		if (pSong->artist.empty() || pSong->name.empty()) {
			errorString = "The song passed has no artist or name.";
			return false;
		}else{
			this->v_songs.pushBack(pSong);
		}
	} else {
		errorString = "Dear user, if you want to add a song, please pass a song...";
		return false;
	}
	return true;
}
