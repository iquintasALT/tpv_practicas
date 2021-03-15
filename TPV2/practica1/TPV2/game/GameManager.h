#pragma once
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"


enum states {
	NEWGAME, PAUSED, RUNNING, GAMEOVER
};

class GameManager
{
public:
	GameManager() { states_ = NEWGAME; }
	void render();
	void update();
private:
	states states_;
};

