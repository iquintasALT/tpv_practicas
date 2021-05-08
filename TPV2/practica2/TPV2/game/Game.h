#pragma once
#include <memory>
#include <vector>

// when you are not using the methods of a class, just
// say that it exists, that saves time when parsing files
class Manager;
class System;

class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();
private:
	std::unique_ptr<Manager> mngr_;
	std::vector<System*> systems;
};
