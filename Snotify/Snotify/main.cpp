#include <iostream>

#include "tDLList.h"
#include "cPerson.h"
#include "cSnotify.h"

int main(int argc, char* argv[]) {
	cSnotify g_snotify;

	g_snotify.LoadSongsFromFile();

	return 0;
	//for (int i = 0; i < g_snotify.v_songs.getSize(); i++) {
	//	//p = myArray.getAt(i);
	//	//if (p != nullptr) {
	//		std::cout << "Array[" << i << "] Song[" << g_snotify.v_songs.getAt(i)->name << "] Artist [" << g_snotify.v_songs.getAt(i)->artist << "]" << std::endl;
	//	//}
	//}
}