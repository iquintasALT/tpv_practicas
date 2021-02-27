#ifndef MENUSTATE_H_
#define MENUSTATE_H_

#include "GameState.h"
#include "MenuButton.h"

class SDLApplication;

//clase de la que heredan todos los menus del juego. Tiene un metodo para crear botones
class MenuState : public GameState
{
protected:
	Texture* bgText; //fondo del menu
	Texture* buttons; //textura de los botones
	//constructora protected, por lo que es una clase abstracta
	MenuState(SDLApplication* appPtr, int const bgName);
	//metodo que crea botones que al ser pulsados realizan la funcion pasada en el callback cb
	virtual MenuButton* createButton(Point2D pos, int sizeY, int sizeX, Texture* texture, int frameX, void cb (SDLApplication* appPtr));
	virtual void render() const;
};

#endif