#ifndef PACMAN_H_
#define PACMAN_H_

#include "GameCharacter.h"
#include "Vector2D.h"
#include "Texture.h"
#include "checkML.h"
#include <fstream>

//clase Game para evitar dependecias circulares
class Game;

using namespace std;

class Pacman : public GameCharacter
{
private:
	static const string identifier; //identificador
	Vector2D nextDir = Vector2D(0, 0); //siguiente direccion
	int energy = 0; //energia (powerUp)
public:
	//constructora con parametros de Pacman
	Pacman(Point2D const point, Game* const gamePtr, Point2D const framePos, Point2D const size)
		: GameCharacter(point, gamePtr, framePos, size) {};
	Pacman(ifstream& loadFile, Game* const gamePtr);
	virtual ~Pacman() {};
	virtual void saveToFile(ofstream& saveFile);
	virtual void update();
	void handleEvent(SDL_Event& const event);
	void death();
	#pragma region Getters&Setters
	int getEnergy() const { return energy; };
	int* getEnergyRef() { return &energy; };
	void setNewFrame(int const frame) { frameCoors.setYCoor(frame); };
	void setEnergy(int const maxEnergy) { energy = maxEnergy; };
	#pragma endregion
};

#endif