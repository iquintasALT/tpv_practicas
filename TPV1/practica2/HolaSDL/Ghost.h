#ifndef GHOST_H_
#define GHOST_H_

#include "GameCharacter.h"
#include "Vector2D.h"
#include "Texture.h"
#include "checkML.h"
#include <vector>
#include <fstream>

//clase Game para evitar dependecias circulares
class Game;

using namespace std;

class Ghost : public GameCharacter
{
private:
	static const string identifier; //identificador

public:
	//constructora con parametros de Ghost
	Ghost(Point2D const point, Game* const gamePtr, Point2D const framePos, Point2D const size)
		: GameCharacter(point, gamePtr, framePos, size) {};
	Ghost(ifstream& loadFile, Game* const gamePtr);
	virtual ~Ghost() {};
	virtual void saveToFile(ofstream& saveFile);
	virtual void update();
	//metodo que recoloca al Ghost tras morir
	void death() { currentPos = initialPos; };
	//getter de su posicion actual
	Point2D getDir() const { return dir; };

protected:
	//parte de 0 para que haga el primer cambio de posicion
	int timeToChangeDir = 0; //timer de cambio de direccion
	static const Vector2D dirs[4];
};

#endif