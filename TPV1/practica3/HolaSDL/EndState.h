#ifndef ENDSTATE_H_
#define ENDSTATE_H_

#include "MenuState.h"

class SDLApplication;

using namespace std;
//estado de fin de juego
class EndState : public MenuState
{
public:
	EndState(SDLApplication* appPtr, bool win);

private:
	//funciones que se pasaran a los botones en su creacion
	static void returnMainMenu(SDLApplication* appPtr);
	static void leaveGame(SDLApplication* appPtr);
};
#endif