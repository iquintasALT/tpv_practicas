#ifndef SDLAPPLICATION_H_
#define SDLAPPLICATION_H_

#include "SDL.h"
#include "Texture.h"
#include "GameStateMachine.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "PauseState.h"
#include "EndState.h"
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

enum class TextureName {
	WallTexture, VitaminsTexture, FoodTexture, CharactersTexture,
	PuntuationTexture, InfobarBgTexture, HeartTexture, BatteryTexture,
	WinTexture, LoseTexture, MenuBgTexture, ButtonsTexture, PauseBgTexture
};

const uint winWidth = 906; //906 - 150 / 28 = 27
const uint winHeight = 783; //783 / 29 = 27

class SDLApplication
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	unordered_map<TextureName, Texture*> textures;
	GameStateMachine* stateMachine;
	bool exit = false;

public:
	SDLApplication();
	~SDLApplication();
	void run();
	void handleEvents();
	void render();
	Texture* getTexture(TextureName const textureName) const { return textures.find(textureName)->second; };
	//metodo que se ejecuta tras presionar 'exit' en el menu principal/pantalla final
	//funciones que seran llamadas por los botones
	void exitGame() { exit = true; } //salimos de partida
	void returnMainMenu();
	void startGame();
	void resumeGame();
	void loadGame();
	void saveGame();
	//metodo para pasar al menu de pausa desde el juego
	void pauseGame() { stateMachine->pushState(new PauseState(this)); }
	//metodo para mostrar la pantalla final con respecto a si se ha ganado o no
	void endGame(const bool win) { stateMachine->pushState(new EndState(this, win)); }

private:
	string getPath();
	void readTextureFile();
};
#endif

