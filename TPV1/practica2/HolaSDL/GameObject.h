#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "SDL.h"
#include "Vector2D.h"

//clase Game para evitar dependecias circulares
class Game;

class GameObject {
public:
	GameObject(): currentPos(Point2D(0,0)), height(0), width(0), game(nullptr) {};
	//constructor con parametros (lo llamara o bien GameMap o GameCharacter)
	GameObject(Point2D const pos, int const objHeight, int const objWidth, Game* gamePtr) :
		currentPos(pos), height(objHeight), width(objWidth), game(gamePtr) {};
	virtual ~GameObject() { game = nullptr; };
	virtual void render() const = 0; //abstracto obliga a definir un render en sus hijos
	virtual void update() {}; //vacio no obliga a construirlo en clases que no lo usen
	//metodo que devuelve el rectangulo del objeto
	virtual SDL_Rect getDestRect() const { return SDL_Rect{ currentPos.getXCoor(), currentPos.getYCoor(), width, height }; };

protected:
	Point2D currentPos; //posicion actual
	int height, width; //alto y ancho del objeto
	Game* game; //puntero a juego
};
#endif