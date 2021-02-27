#include "EndState.h"
#include "SDLApplication.h"

//funciones que se pasaran como callbacks a botones
#pragma region CallBackFunctions
//funcion para volver al menu 
void EndState::returnMainMenu(SDLApplication* appPtr) {
	appPtr->returnMainMenu();
}
//funcion para salir del juego
void EndState::leaveGame(SDLApplication* appPtr) {
	appPtr->exitGame();
}
#pragma endregion

//constructora de EndState que crea los botones necesarios e imprime el fondo
EndState::EndState(SDLApplication* appPtr, bool win) : MenuState(appPtr, (int)TextureName::MenuBgTexture) {
	//si has ganado, se muestra la textura de ganar
	if(win) bgText = app->getTexture(TextureName::WinTexture);
	//si has perdido, se muestra la textura de perder
	else bgText = app->getTexture(TextureName::LoseTexture);
	createButton(Point2D{ 130,650 }, 50, 200, app->getTexture(TextureName::ButtonsTexture), 0, returnMainMenu);
	createButton(Point2D{ 630,650 }, 50, 200, app->getTexture(TextureName::ButtonsTexture), 2, leaveGame);
}