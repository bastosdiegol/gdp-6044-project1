#include "cSong.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

cSong::cSong() {
	this->rating = 0;
	this->numberOfTimesPlayed = 0;

	// Generate unique Snotify ID
	this->uniqueID = cSong::m_NEXT_Snotify_UniqueSongID;
	// Increment for next created user by a small random amount 
	const unsigned int MAX_ID_INCREEMNT = 5;
	cSong::m_NEXT_Snotify_UniqueSongID += (rand() % MAX_ID_INCREEMNT);
}

// static 
unsigned int cSong::m_NEXT_Snotify_UniqueSongID = 1;

cSong::~cSong() {
}
