#ifndef MENUBUTTON_H_
#define MENUBUTTON_H_

#include "GameObject.h"
#include "GameState.h"
#include "EventHandler.h"
#include "Texture.h"

using CallBackonClick = void(SDLApplication* appPtr);

#pragma once
//clase boton que sera usada en menus
class MenuButton : public GameObject, public EventHandler
{
private:
	Texture* texture; //textura del propio boton
	int frameX, frameY; //frame del boton en la textura
	CallBackonClick (*callback); //callback a ejecutar tras ser presionado

public:
	//constructora de boton
	MenuButton(Point2D pos, int objHeight, int objWidth, int nFrameX, SDLApplication* appPtr, Texture* texturePtr, CallBackonClick (*cb))
		: GameObject(pos, objHeight, objWidth, appPtr) {
		texture = texturePtr; callback = cb;
		frameX = nFrameX; frameY = 0;
	}
	//renderizado de boton
	virtual void render() const { texture->renderFrame(getDestRect(), frameX, frameY); }
	virtual bool handleEvent(SDL_Event& const event);
	virtual void update();
};
#endif