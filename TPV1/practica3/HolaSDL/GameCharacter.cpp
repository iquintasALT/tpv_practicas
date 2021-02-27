#include "GameCharacter.h"
#include "SDLApplication.h"

//constructora con parametros de GameCharacter
GameCharacter::GameCharacter(Point2D const iniPos, SDLApplication* app, Point2D const framePos, Point2D const size, PlayState* const _playState) :
	GameObject(iniPos, size.getXCoor(), size.getYCoor(), app) {
	initialPos = iniPos;
	dir = Vector2D(0, 0);
	texture = app->getTexture(TextureName::CharactersTexture);
	frameCoors = framePos;
	playState = _playState;
}

//constructora por flujo de GameCharacter
GameCharacter::GameCharacter(ifstream& loadFile, SDLApplication* appPtr, PlayState* const _playState) {
	loadFile >> width >> height >> currentPos >> initialPos >> dir >> frameCoors;
	//asignacion de SDLApplication en GameObject
	playState = _playState;
	app = appPtr;
	//asignacion de textura en GameCharacter
	texture = app->getTexture(TextureName::CharactersTexture);
}

//guardado de partida en archivo de GameCharacter
void GameCharacter::saveToFile(ofstream& saveFile){
	saveFile << width << " " << height << " " << currentPos << " " << initialPos << " " << dir << " " << frameCoors;
}

//metodo de renderizado del GameCharacter
void GameCharacter::render() const {
	SDL_Rect dstRect = getDestRect(); //tamaño del GameObject
	//usamos el frame del GameCharacter
	texture->renderFrame(dstRect, frameCoors.getXCoor(), frameCoors.getYCoor());
}