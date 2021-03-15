// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <memory>



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
	void generateAsteroid(Entity* fighter);
	std::unique_ptr<Manager> mngr_;
	std::unique_ptr<GameManager> gameManager;
};
