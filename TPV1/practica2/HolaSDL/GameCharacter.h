#ifndef GAMECHARACTER_H_
#define GAMECHARACTER_H_

#include "GameObject.h"
#include "Vector2D.h"
#include "Texture.h"
#include <fstream>
#include <list>

class Game;

class GameCharacter : public GameObject {
public:
	virtual ~GameCharacter() { texture = nullptr; }; //virtual por ser destructora
	virtual void saveToFile(ofstream& saveFile); //virtual para la herencia en los objetos de juego
	virtual void render() const; //virtual por convenio (el virtual viene de GameObject)
	//metodo para establecer el puntero en la lista que corresponde al objeto
	void setItList(list<GameObject*>::iterator it) { listPos = it; };

protected:
	Point2D initialPos; //posicion inicial del objeto de juego
	Vector2D dir; //direccion de movimiento
	Texture* texture; //puntero a textura
	Point2D frameCoors; //coordenadas del frame de la textura
	list<GameObject*>::iterator listPos; //iterador que apunta posicion del GO en la lista

	GameCharacter(Point2D const iniPos, Game* gamePtr, Point2D const framePos, Point2D const size);
	GameCharacter(ifstream& loadFile, Game* gamePtr);
};

#endif