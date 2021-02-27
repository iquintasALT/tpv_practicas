#include "PauseState.h"
#include "SDLApplication.h"

//funciones que se pasaran como callbacks a botones
#pragma region CallBackFunctions
//funcion para volver al estado anterior
void PauseState::resumeGame(SDLApplication* appPtr) {
	appPtr->resumeGame();
}
//funcion para guardar la partida
void PauseState::saveGame(SDLApplication* appPtr) {
	appPtr->saveGame();
}
//funcion para volver al menu principal
void PauseState::returnMenuGame(SDLApplication* appPtr) {
	appPtr->returnMainMenu();
}
#pragma endregion
//constructora de PauseState que crea los botones necesarios
PauseState::PauseState(SDLApplication* appPtr) : MenuState(appPtr, (int)TextureName::MenuBgTexture) {
	bgText = app->getTexture(TextureName::PauseBgTexture);
	createButton(Point2D{ 130, 650 }, 50, 200, appPtr->getTexture(TextureName::ButtonsTexture), 4, resumeGame);
	createButton(Point2D{ 380, 650 }, 50, 200, appPtr->getTexture(TextureName::ButtonsTexture), 5, saveGame);
	createButton(Point2D{ 630, 650 }, 50, 200, appPtr->getTexture(TextureName::ButtonsTexture), 3, returnMenuGame);
}
