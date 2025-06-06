#include <iostream>
#include <memory>

#include "./game/Game.h"

void start() {
	Game g;

	g.init();
	g.start();
}


int main(int x, char** args) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try {
		start();
	}
	catch (const std::string& e) { // catch exceptions thrown as strings
		std::cerr << e << std::endl;
	}
	catch (const char* e) { // catch exceptions thrown as char*
		std::cerr << e << std::endl;
	}
	catch (const std::exception& e) { // catch exceptions thrown as a sub-type of std::exception
		std::cerr << e.what();
	}
	catch (...) {
		std::cerr << "Caught and exception of unknown type ...";
	}

	return 0;
}
