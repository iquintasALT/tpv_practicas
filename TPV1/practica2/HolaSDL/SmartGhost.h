#ifndef SMARTGHOST_H_
#define SMARTGHOST_H_

#include "Ghost.h"
#include "Vector2D.h"
#include "Texture.h"
#include "checkML.h"
#include <vector>
#include <fstream>

//clase Game para evitar dependecias circulares
class Game;

//maquina de estados del SmartGhost
enum class State { Child, Adult, Quarantine, Dead };

using namespace std;

class SmartGhost : public Ghost
{
private:
	static const int numStates = 4;
	static const int times[numStates]; //tiempos que se esta en cada estado
	static const string identifier; //identificador
	State state = State::Child; //estado del SmartGhost
	int edad = 0; //contador de edad
	int cuarentena = 0; //contador de cuarentena
	int deathCountdown = 0; //contador de tiempo tras muerte

public:
	//constructora con parametros de SmartGhost
	SmartGhost(Point2D const point, Game* const gamePtr, Point2D const framePos, Point2D const size)
		: Ghost(point, gamePtr, framePos, size) {};
	SmartGhost(ifstream& loadFile, Game* const gamePtr);
	virtual ~SmartGhost() {};
	virtual void saveToFile(ofstream& saveFile);
	virtual void update();
	virtual void render() const;
	#pragma region Getters&Setters
	Point2D getPos() const { return currentPos; };
	State getState() const { return state; };
	void changeState(State const stat) { state = stat; };
	#pragma endregion

private:
	void handleState(State& state);
};

#endif
