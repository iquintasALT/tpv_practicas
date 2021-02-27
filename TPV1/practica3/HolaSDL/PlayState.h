#ifndef PLAYSTATE_H_
#define PLAYSTATE_H_

#include "GameState.h"

#include "GameObject.h"
#include "Ghost.h"
#include "SmartGhost.h"
#include "Pacman.h"
#include "GameMap.h"
#include "Texture.h"
#include "InfoBar.h"
#include "Prize.h"

#include "FileFormatError.h"
#include "FileNotFoundError.h"
#include "SDLError.h"

#include "SDL.h"
#include "SDL_image.h"
#include "checkML.h"
#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <cstdlib>
#include <ctime>
#include <list>

//clase SDLApplication para evitar dependecias circulares
class SDLApplication;

using namespace std;

const uint infoBarWidth = 150;
const int timeAfterDeath = 1500; //tiempo de retardo tras morir

class PlayState : public GameState
{
private:
	static const int maxLevels = 5;
	static const int maxLives = 3;
	static const int maxEnergy = 50 * 27; //energia tras activar powerUp
	static const int timeFrame = 5;
	static const int endScreenTime = 2000;
	static const int maxTimePrice = 500;
	GameMap* gameMap;
	Pacman* pacman;
	InfoBar* infoBar;
	list<Ghost*> ghosts;
	list<Prize*> prizes;
	list<list<GameObject*>::iterator> objsToErase;
	Point2D cellSize;
	int lives = maxLives, energy = 0, foodCount = 0;
	int maxFood = 0, level = 1;
	int timePrice = 0;

public:
	PlayState(SDLApplication* appPtr, string const path);
	virtual ~PlayState(); //CORREGIR
	void save(string const saveFile);
	bool collisionGhost(SDL_Rect& const pacmanPos);
	bool tryMove(SDL_Rect& const charRect, Vector2D dir, Point2D& newPos);
	void cellInteraction(SDL_Rect& const pacmanRect);
	void checkModule(Point2D& dir, Point2D& const currentPos, vector<Point2D>& const posDirs, vector<Point2D>& const posMoves);
	bool reproductionSmartGhost(SmartGhost* const ghost);
	void eraseGhost(list<GameObject*>::iterator it, Ghost* const ghost);
	void emptyBuffer();
private:
	void loadLevelFile(int const level);
	void loadNextLevel();
	void loadFromFile(string const path);
	virtual void update();
	virtual void render() const;
	virtual void handleEvent(SDL_Event& event);
	bool eraseCellFood(SDL_Rect& const point);
	void toroidalMove(Point2D& nextPos, Point2D const size, SDL_Rect& const map);
	void destroyObjects();
	void createPrize(SDL_Rect& const ghostPos, Point2D dir);
	void acquirePrize();
};
#endif

