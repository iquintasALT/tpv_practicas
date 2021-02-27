#ifndef GHOST_H_
#define GHOST_H_

#include "Vector2D.h"
#include "Texture.h"
#include "checkML.h"
#include <vector>
#include <fstream>

//clase Game para evitar dependecias circulares
class Game;

using namespace std;

class Ghost
{
private:
	//array constante de direcciones de Ghost
	Vector2D const dirs[4] = { {1,0}, {-1,0}, {0,-1}, {0,1} };
	Point2D currentPos, initialPos; //posiciones actual e inicial
	Vector2D dir; //direccion de movimiento
	Texture* texture; //puntero a textura
	int ghostFrameY; //frame del sprite sheet que le corresponde
	Game* game; //puntero a Game

public:
	Ghost(Point2D const point, int const ghostNum, Texture* const textureP, Game* const gameP);
	~Ghost();
	void save(ofstream& output);
	void load(ifstream& input);
	void update();
	void render() const;
	//metodo que recoloca al Ghost tras morir
	void death() { currentPos = initialPos; };
	//getter de su posicion actual
	Point2D getPos() const { return currentPos; }

private:
	void toroidalMove(Point2D& nextPos);
};

#endif