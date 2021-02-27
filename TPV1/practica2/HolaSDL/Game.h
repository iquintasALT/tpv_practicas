#ifndef GAME_H_
#define GAME_H_

#include "GameObject.h"
#include "Ghost.h"
#include "SmartGhost.h"
#include "Pacman.h"
#include "GameMap.h"
#include "Texture.h"
#include "InfoBar.h"
#include "SDL.h"
#include "SDL_image.h"
#include "checkML.h"
#include "FileFormatError.h"
#include "FileNotFoundError.h"
#include "SDLError.h"
#include "Prize.h"
#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <cstdlib>
#include <ctime>
#include <list>

using namespace std;

using uint = unsigned int;

static enum class TextureName {
	WallTexture, VitaminsTexture, FoodTexture, CharactersTexture,
	PuntuationTexture, BackgroundTexture, HeartTexture, BatteryTexture, WinTexture, LoseTexture
};

struct TextureAtribs {
	string fileName;
	int fils;
	int cols;
};


const uint winWidth = 906; //906-150 / 28 = 27
const uint winHeight = 783; //783 / 29 = 27
const uint infoBarWidth = 150;

const int timeAfterDeath = 1500; //tiempo de retardo tras morir

class Game
{
private:
	static const int NUM_TEXTURE = 10;
	static const TextureAtribs textureAtribs[NUM_TEXTURE];
	static const int maxLives = 3;
	static const int maxEnergy = 50 * 27; //energia tras activar powerUp
	static const int timeFrame = 5;
	static const int endScreenTime = 2000;
	static const int maxTimePrice = 500;
	SDL_Window* window;
	SDL_Renderer* renderer;
	array <Texture*, NUM_TEXTURE> textures;
	GameMap* gameMap;
	Pacman* pacman;
	InfoBar* infoBar;
	list<Ghost*> ghosts;
	list<GameObject*> gObjs;
	list<Prize*> prizes;
	list<list<GameObject*>::iterator> objsToErase;
	Point2D cellSize;
	bool exit;
	int lives = maxLives, energy = 0, foodCount = 0;
	int maxFood = 0, level = 1;
	int timePrice = 0;

public:
	Game();
	~Game();
	void run();
	void save(string const saveFile);
	bool collisionGhost(SDL_Rect& const pacmanPos);
	bool tryMove(SDL_Rect& const charRect, Vector2D dir, Point2D& newPos);
	void cellInteraction(SDL_Rect& const pacmanRect);
	void checkModule(Point2D& dir, Point2D& const currentPos, vector<Point2D>& const posDirs, vector<Point2D>& const posMoves);
	bool reproductionSmartGhost(SmartGhost* const ghost);
	void eraseGhost(list<GameObject*>::iterator it, Ghost* const ghost);
	void emptyBuffer();
	//getter para obtener una textura del juego
	Texture* getTexture(TextureName const textureName) const { return textures[(int)textureName]; };

private:
	string menu() const;
	void loadLevelFile(int const level);
	void loadNextLevel(int const level);
	void loadFromFile(string const path);
	void update();
	void render() const;
	void handleEvents();
	bool eraseCellFood(SDL_Rect& const point);
	void toroidalMove(Point2D& nextPos, Point2D const size, SDL_Rect& const map);
	void destroyObjects();
	void endScreen() const;
	void createPrize(SDL_Rect& const ghostPos, Point2D dir);
	void acquirePrize();
};

#endif