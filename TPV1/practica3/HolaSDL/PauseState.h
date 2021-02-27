#ifndef PAUSESTATE_H_
#define PAUSESTATE_H_

#include "MenuState.h"

//clase SDLApplication para evitar dependecias circulares
class SDLApplication;

class PauseState : public MenuState
{
public:
	PauseState(SDLApplication* appPtr);

private:
	static void resumeGame(SDLApplication* appPtr);
	static void saveGame(SDLApplication* appPtr);
	static void returnMenuGame(SDLApplication* appPtr);
};
#endif

