#include <iostream>

#include "tDLList.h"
#include "cPerson.h"
#include "cSnotify.h"

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
	//for (int i = 0; i < g_snotify.v_users.getSize(); i++) {
	//	cPerson* p = g_snotify.v_users.getAt(i);
	//	std::cout << "Person " << i << ": " << p->first << " " << p->middle << " " << p->last << std::endl;
	//}
	cPerson* allUsers;
	cSong* userLib;
	unsigned int size;
	g_snotify.FindUsersFirstName("Suzanne", allUsers, size);

	//541991212
	cPerson* thePerson = g_snotify.FindUserBySIN(183170504);
	// 52689
	cSong* theSong = g_snotify.FindSong(4309);
	if (!g_snotify.AddSongToUserLibrary(thePerson->getSnotifyUniqueUserID(), theSong, errorMsg)) {
		std::cout << "Song Lib add failed: " << errorMsg << std::endl;
	} else {
		std::cout << "Song added to the user library." << std::endl;
	}
	// 37105
	theSong = g_snotify.FindSong(44642);
	if (!g_snotify.AddSongToUserLibrary(thePerson->getSnotifyUniqueUserID(), theSong, errorMsg)) {
		std::cout << "Song Lib add failed: " << errorMsg << std::endl;
	} else {
		std::cout << "Song added to the user library." << std::endl;
	}
	theSong = g_snotify.FindSong(53275);
	if (!g_snotify.AddSongToUserLibrary(thePerson->getSnotifyUniqueUserID(), theSong, errorMsg)) {
		std::cout << "Song Lib add failed: " << errorMsg << std::endl;
	} else {
		std::cout << "Song added to the user library." << std::endl;
	}
	

	theSong = g_snotify.GetSong(thePerson->getSnotifyUniqueUserID(), 4309, errorMsg);
	if (theSong == nullptr) {
		std::cout << "Song not found: " << errorMsg << std::endl;
	}
	theSong = g_snotify.GetSong(thePerson->getSnotifyUniqueUserID(), 4309, errorMsg);
	if (theSong == nullptr) {
		std::cout << "Song not found: " << errorMsg << std::endl;
	}

	g_snotify.GetUsersSongLibrary(thePerson->getSnotifyUniqueUserID(), userLib, size);

	thePerson->first = "Diego";
	thePerson->middle = "Lima";
	thePerson->last = "Bastos";
	

	if (!g_snotify.UpdateRatingOnSong(thePerson->getSnotifyUniqueUserID(), theSong->getUniqueID(), 5)) {
		std::cout << "Song rating update failed: " << errorMsg << std::endl;
	} else {
		std::cout << "Song rating updated." << std::endl;
	}

	//if (!g_snotify.RemoveSongFromUserLibrary(thePerson->getSnotifyUniqueUserID(), theSong->getUniqueID(), errorMsg)) {
	//	std::cout << "Song Lib del failed: " << errorMsg << std::endl;
	//} else {
	//	std::cout << "Song removed to the user library." << std::endl;
	//}

	//if (!g_snotify.UpdateUser(thePerson, errorMsg)) {
	//	std::cout << "Update failed: " << errorMsg << std::endl;
	//} else {
	//	std::cout << "User name updated." << std::endl;
	//}

	//thePerson = g_snotify.FindUserBySnotifyID(3956);
	//thePerson->first = "Diego";
	//thePerson->middle = "Lima";
	//thePerson->last = "Lastos";
	//g_snotify.UpdateUser(thePerson, errorMsg);

	g_snotify.FindUsersLastName("BINDER", allUsers, size);

	if (!g_snotify.DeleteSong(theSong->getUniqueID(), errorMsg)) {
		std::cout << "Delete song failed: " << errorMsg << std::endl;
	} else {
		std::cout << "Song deleted successfully." << std::endl;
	}

	//if (!g_snotify.DeleteUser(thePerson->getSnotifyUniqueUserID(), errorMsg)) {
	//	std::cout << "Delete user failed: " << errorMsg << std::endl;
	//} else {
	//	std::cout << "User deleted successfully." << std::endl;
	//}

	return 0;
}