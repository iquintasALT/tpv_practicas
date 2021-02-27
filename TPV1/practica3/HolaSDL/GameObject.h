#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "SDL.h"
#include "Vector2D.h"

//clase SDLApplication para evitar dependecias circulares
class SDLApplication;

class GameObject {
public:
	GameObject(): currentPos(Point2D(0,0)), height(0), width(0), app(nullptr) {};
	//constructor con parametros (lo llamara o bien GameMap o GameCharacter)
	GameObject(Point2D const pos, int const objHeight, int const objWidth, SDLApplication* appPtr) :
		currentPos(pos), height(objHeight), width(objWidth), app(appPtr) {};
	virtual ~GameObject() { app = nullptr; };
	virtual void render() const = 0; //abstracto obliga a definir un render en sus hijos
	virtual void update() {}; //vacio no obliga a construirlo en clases que no lo usen
	//metodo que devuelve el rectangulo del objeto
	virtual SDL_Rect getDestRect() const { return SDL_Rect{ currentPos.getXCoor(), currentPos.getYCoor(), width, height }; };

protected:
	Point2D currentPos; //posicion actual
	int height, width; //alto y ancho del objeto
	SDLApplication* app; //puntero a juego
};
#endif