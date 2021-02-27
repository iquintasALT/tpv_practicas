#ifndef GAME_H_
#define GAME_H_

#include "Ghost.h"
#include "Pacman.h"
#include "GameMap.h"
#include "Texture.h"
#include "InfoBar.h"
#include "SDL.h"
#include "SDL_image.h"
#include "checkML.h"
#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

using uint = unsigned int;

enum class TextureName {
	WallTexture, VitaminsTexture, FoodTexture, CharactersTexture,
	PuntuationTexture, BackgroundTexture, HeartTexture, BatteryTexture
};

struct TextureAtribs {
	string fileName;
	int fils;
	int cols;
};

const int NUM_GHOSTS = 4;
const int NUM_TEXTURE = 8;
const TextureAtribs textureAtribs[NUM_TEXTURE] = { {"..\\images\\wall3.png",1,1}, {"..\\images\\food2.png",1,1},
				{"..\\images\\vitamins.png",1,1}, {"..\\images\\characters1.png",4,14}, {"..\\images\\digits.png",1,10},
				{"..\\images\\background.jpg",1,1}, {"..\\images\\heart.png",1,1}, {"..\\images\\battery.png",1,1} };

const uint winWidth = 910;
const uint winHeight = 790;

int const maxLives = 3;
int const maxEnergy = 50; //energia tras activar powerUp
int const timeAfterDeath = 2000; //tiempo de retardo tras morir

class Game
{
private:
	int const timeFrame = 150;
	int const endScreenTime = 2000;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int cellWidth, cellHeight;
	array <Texture*, NUM_TEXTURE> textures;
	GameMap* gameMap;
	Pacman* pacman;
	array<Ghost*, NUM_GHOSTS> ghosts;
	InfoBar* infoBar;
	bool exit, victory;

public:
	Game() = delete;
	Game(int level);
	~Game();
	void run();
	void saveGame();
	bool collisionGhost(Point2D const pacmanPos, int const energia) const;
	bool eraseCellFood(Point2D const point);
	//metodo para comprobar que la celda indicada no es Wall
	bool nextCell(Point2D const pos) const { return (gameMap->readCell(pos.getXCoor(), pos.getYCoor()) != MapCell::Wall); };
	//metodo para perder partida
	void gameOver() { exit = true; victory = false; };
	//metodo de impresion de la pantalla final
	void endScreen();
	void emptyBuffer();
#pragma region Getters&Setters
	Texture* getTexture(TextureName const textureName) const { return textures[(int)textureName]; };
	int getCols() const { return gameMap->getCols(); };
	int getFils() const { return gameMap->getFils(); };
	int getCellHeight() const { return cellHeight; };
	int getCellWidth() const { return cellWidth; };
#pragma endregion

private:
	void loadObjects(int level);
	void loadFromFile();
	void update();
	void render() const;
	void handleEvents();
	void removeSaveFile();
};

#endif