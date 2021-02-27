#ifndef INFOBAR_H_
#define INFOBAR_H_

#include "Texture.h"
#include "checkML.h"
#include <cmath>

class Game;

using namespace std;

class InfoBar
{
private:
	Game* game; //puntero a juego
	int* liveCount, * powerUpController; //punteros al numero de vias y a la energia
	int puntuation; //puntuacion

public:
	InfoBar(Game* gameP, int* const liveCounter, int* const powerUpActive);
	~InfoBar();
	void addPuntuation() { puntuation++; }; //metodo para aumentar la puntuacion
	void render() const;
};

#endif