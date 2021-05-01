// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <memory>
#include <vector>

// when you are not using the methods of a class, just
// say that it exists, that saves time when parsing files
class Manager;
class Entity;
class GameManager;

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