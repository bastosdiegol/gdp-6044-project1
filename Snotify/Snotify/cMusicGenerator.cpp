#include "cMusicGenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cMusicGenerator::cMusicGenerator() {
	DEBUG_PRINT("cMusicGenerator::cMusicGenerator()\n");
}

cMusicGenerator::~cMusicGenerator() {
	DEBUG_PRINT("cMusicGenerator::~cMusicGenerator()\n");
	int index = v_songs.getSize() - 1;
	while (index >= 0) {
		// Delete the song pointer
		delete v_songs.getAt(index);
		// Removes the node pointing to null from the list
		v_songs.removeAt(index);
		index--;
	}
}

bool cMusicGenerator::LoadMusicInformation(std::string musicFileName, std::string& errorString) {
	DEBUG_PRINT("cMusicGenerator::LoadMusicInformation(%s)\n", musicFileName.c_str());
	std::cout << "MusicGenerator is loading songs..." << std::endl;
	// Open the file
	std::ifstream songsFile(musicFileName);
	if (!songsFile.is_open()) {
		errorString = "File couldn't be opened.";
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
		v_songs.pushBack(newSong);
	}

	std::cout << "MusicGenerator loaded " << lineCount << " songs." << std::endl;

	return true;
}

cSong* cMusicGenerator::getRandomSong(void) {
	// Creates a random number based on the v_songs size
	int randomNum = rand() % v_songs.getSize();
	// Return this song
	return v_songs.getAt(randomNum);
}

cSong* cMusicGenerator::findSong(std::string songName, std::string artist) {
	for (int i = 0; i < v_songs.getSize(); i++) {
		cSong* song = v_songs.getAt(i);
		if ((song->name == songName) &&
			(song->artist == artist)) {
			return song;
		}
	}
	return nullptr;
}
