#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include "SDL.h"

//clase de la que heredaran otras clases que deban gestionar eventos (Pacman Y MenuButton)
class EventHandler
{
public:
	//este bool se usara para la destruccion automatica de los gameObjects, por ejemplo cuando se pulse un boton de cambio de escena
	virtual bool handleEvent(SDL_Event& const event) = 0;
	virtual ~EventHandler() {}
};
#endif

