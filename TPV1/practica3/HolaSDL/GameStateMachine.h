#ifndef GAMESTATEMACHINE_H_
#define GAMESTATEMACHINE_H_

#include "GameState.h"
#include <stack>

using namespace std;

class GameStateMachine
{
private:
	stack<GameState*> states; //pila de estados

public:
	~GameStateMachine();
	void changeState(GameState* gs);
	//metodo para obtener el estado en el que nos encontramos
	GameState* currentState() { return states.top(); };
	void popState();
	//metodo para agregar nuevo estado a la cola
	void pushState(GameState* gs) { states.push(gs); };
};
#endif