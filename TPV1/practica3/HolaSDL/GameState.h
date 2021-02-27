#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <list>
#include "GameObject.h"
#include "EventHandler.h"

//clase SDLApplication para evitar dependecias circulares
class SDLApplication;

class GameState
{
protected:
	list<GameObject*> stage; //lista de GameObjects
	list<EventHandler*> eventHandlers; //lista de eventHandlers
	SDLApplication* app; //puntero a la aplicacion

	//clase abstracta por tener constructor protected
	GameState(SDLApplication* appPtr) { app = appPtr; };

public:
	//destructora que destruye los GameObjects
	virtual ~GameState() { for (GameObject* o : stage) delete o; stage.clear(); eventHandlers.clear(); };
	//metodo que actualiza los GameObjects
	virtual void update() { for (GameObject* o : stage) o->update(); };
	//metodo que renderiza los GameObjects
	virtual void render() const { for (GameObject* o : stage) o->render(); };
	//metodo que maneja los eventos de los EventHandlers
	virtual void handleEvent(SDL_Event& event) { for (EventHandler* h : eventHandlers) if (h->handleEvent(event)) return; };
};
#endif