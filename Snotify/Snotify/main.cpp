#include <iostream>

#include "tDLList.h"
#include "cPerson.h"
#include "cSnotify.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

int main(int argc, char* argv[]) {
	cSnotify g_snotify;
	std::string errorMsg;
	g_snotify.m_musicGen.LoadMusicInformation(SONGS_FILE, errorMsg);
	int duplicated = 0;
	int songsToTransfer = 1000;
	std::cout << "Now transfering random songs from MusicGenerator to Snotify..." << std::endl;
	std::cout << "Trying to add " << songsToTransfer << " random songs to the Snotify." << std::endl;
	for (int i = 0; i < songsToTransfer; i++) {
		cSong* randomSong = g_snotify.m_musicGen.getRandomSong();
		if (!g_snotify.AddSong(randomSong, errorMsg)) {
			duplicated++;
		}
	}
	if (duplicated > 0) {
		std::cout << "Of these, " << duplicated << " songs were duplciated and skipped." << std::endl;
		std::cout << "Total songs added: " << songsToTransfer - duplicated << "." << std::endl;
	}
	g_snotify.m_personGen.LoadCensusFiles(NAME_FILE, SURNAME_FILE, STREETNAME_FILE, errorMsg);
	return 0;
}