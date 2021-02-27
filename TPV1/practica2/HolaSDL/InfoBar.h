#ifndef INFOBAR_H_
#define INFOBAR_H_

#include "Texture.h"
#include "checkML.h"
#include "GameObject.h"
#include <cmath>

class Game;

using namespace std;

class InfoBar : public GameObject
{
private:
	int* liveCount, * powerUpController; //punteros al numero de vias y a la energia
	int puntuation; //puntuacion

public:
	InfoBar(Game* const gameP, int* const liveCounter, int* const powerUpActive, int const points, Point2D const pos, Point2D const size);
	~InfoBar();
	//metodo para aumentar la puntuacion
	void addPuntuation() { puntuation++; };
	void setPowerUpRef(int* const powerUpActive) { powerUpController = powerUpActive; };
	void render() const;
};

#endif