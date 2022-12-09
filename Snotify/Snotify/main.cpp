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
	cMusicGenerator* m_musicGen = new cMusicGenerator();
	cPersonGenerator* m_personGen = new cPersonGenerator();

	std::string errorMsg;
	m_musicGen->LoadMusicInformation(SONGS_FILE, errorMsg);
	int duplicated = 0;
	int songsToTransfer = 1000;
	std::cout << "Now transfering random songs from MusicGenerator to Snotify..." << std::endl;
	std::cout << "Trying to add " << songsToTransfer << " random songs to the Snotify." << std::endl;
	for (int i = 0; i < songsToTransfer; i++) {
		if (!g_snotify.AddSong(m_musicGen->getRandomSong(), errorMsg)) {
			duplicated++;
		}
	}
	if (duplicated > 0) {
		std::cout << "Of these, " << duplicated << " songs were duplciated and skipped." << std::endl;
		std::cout << "Total songs added: " << songsToTransfer - duplicated << "." << std::endl;
	}
	delete m_musicGen;
	m_personGen->LoadCensusFiles(NAME_FILE, SURNAME_FILE, STREETNAME_FILE, errorMsg);
	std::cout << "Now generating random persons from PersonGenerator to Snotify..." << std::endl;
	std::cout << "Trying to add " << songsToTransfer << " random persons to the Snotify." << std::endl;
	duplicated = 0;
	for (int i = 0; i < songsToTransfer; i++) {
		cPerson* randomPerson = m_personGen->generateRandomPerson();
		if (!g_snotify.AddUser(randomPerson, errorMsg)) {
			duplicated++;
		}
	}
	std::cout << "Of these, " << duplicated << " persons were duplciated or had invalid data and was skipped." << std::endl;
	std::cout << "Total persons added: " << songsToTransfer - duplicated << "." << std::endl;
	std::cout << "Number of Names Loaded:" << m_personGen->getNumberOfNamesLoaded() << std::endl;
	std::cout << "Number of Streets Loaded:" << m_personGen->getNumberOfStreetsLoaded() << std::endl;
	std::cout << "Number of Surames Loaded:" << m_personGen->getNumberOfSurnamesLoaded() << std::endl;
	//541991212
	cPerson* thePerson = g_snotify.FindUserBySIN(183170504);
	thePerson->first = "Diego";
	thePerson->middle = "Lima";
	thePerson->last = "Bastos";
	if (!g_snotify.UpdateUser(thePerson, errorMsg)) {
		std::cout << "Update failed: " << errorMsg << std::endl;
	}
	return 0;
}