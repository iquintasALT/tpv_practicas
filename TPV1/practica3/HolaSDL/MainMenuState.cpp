#include "MainMenuState.h"
#include "SDLApplication.h"

//funciones que se pasaran como callbacks a botones
#pragma region CallBackFunctions
//funcion para comenzar el juego
void MainMenuState::playGame(SDLApplication* appPtr) {
	appPtr->startGame();
}
//funcion para cargar una partida guardada
void MainMenuState::loadGame(SDLApplication* appPtr) {
	appPtr->loadGame();
}
//funcion para salir del juego
void MainMenuState::leaveGame(SDLApplication* appPtr) {
	appPtr->exitGame();
}
#pragma endregion
//constructora de MainMenuState que crea los botones necesarios
MainMenuState::MainMenuState(SDLApplication* appPtr) : MenuState(appPtr, (int)TextureName::MenuBgTexture) {
	createButton(Point2D{ 130,650 }, 50, 200, app->getTexture(TextureName::ButtonsTexture), 0, playGame);
	createButton(Point2D{ 380,650 }, 50, 200, app->getTexture(TextureName::ButtonsTexture), 1, loadGame);
	createButton(Point2D{ 630,650 }, 50, 200, app->getTexture(TextureName::ButtonsTexture), 2, leaveGame);
}