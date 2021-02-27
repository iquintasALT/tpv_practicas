#ifndef MAINMENUSTATE_H_
#define MAINMENUSTATE_H_

#include "MenuState.h"

class SDLApplication;

using namespace std;
class MainMenuState : public MenuState
{
public:
	MainMenuState(SDLApplication* appPtr);

private:
	static void playGame(SDLApplication* appPtr);
	static void loadGame(SDLApplication* appPtr);
	static void leaveGame(SDLApplication* appPtr);
};
#endif