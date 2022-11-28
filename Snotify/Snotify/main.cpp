#include <iostream>

#include "tDLList.h"
#include "cPerson.h"

int main(int argc, char* argv[]) {
	tDLList<cPerson*> myArray;
	
	cPerson* p;
	p = new cPerson();
	myArray.pushBack(p);
	p = new cPerson();
	myArray.pushBack(p);
	p = new cPerson();
	myArray.pushBack(p);
	p = new cPerson();
	myArray.pushBack(p);
	p = new cPerson();
	myArray.pushBack(p);


	std::cout << "List Size: " << myArray.getSize() << std::endl;

	for (int i = 0; i < myArray.getSize(); i++) {
		//p = myArray.getAt(i);
		//if (p != nullptr) {
			std::cout << "Array[" << i << "] = " << myArray.getAt(i)->getSnotifyUniqueUserID() << std::endl;
		//}
	}
}