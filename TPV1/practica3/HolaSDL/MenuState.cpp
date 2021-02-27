#include "MenuState.h"
#include "SDLApplication.h"

//constructora del menu
MenuState::MenuState(SDLApplication* appPtr, int const bgName) : GameState(appPtr) {
	bgText = app->getTexture((TextureName)bgName); //agregamos la textura del fondo
	buttons = app->getTexture(TextureName::ButtonsTexture); //agregamos la textura de los botones
}

//metodo general para crear botones en los menus
MenuButton* MenuState::createButton(Point2D pos, int sizeY, int sizeX, Texture* texture, int frameX, void cb(SDLApplication* appPtr)) {
	MenuButton* button = new MenuButton(pos, sizeY, sizeX, frameX, app, texture, cb);
	stage.push_back(button);
	eventHandlers.push_back(button);
	return button;
}

//renderizado del menu
void MenuState::render() const {
	bgText->render(SDL_Rect{ 0, 0, winWidth, winHeight });
	GameState::render();
}