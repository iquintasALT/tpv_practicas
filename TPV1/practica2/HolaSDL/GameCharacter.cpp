#include "GameCharacter.h"
#include "Game.h"

//constructora con parametros de GameCharacter
GameCharacter::GameCharacter(Point2D const iniPos, Game* gamePtr, Point2D const framePos, Point2D const size) :
	GameObject(iniPos, size.getXCoor(), size.getYCoor(), gamePtr) {
	initialPos = iniPos;
	dir = Vector2D(0, 0);
	texture = game->getTexture(TextureName::CharactersTexture);
	frameCoors = framePos;
}

//constructora por flujo de GameCharacter
GameCharacter::GameCharacter(ifstream& loadFile, Game* gamePtr) {
	loadFile >> width >> height >> currentPos >> initialPos >> dir >> frameCoors;
	//asignacion de Game en GameObject
	game = gamePtr;
	//asignacion de textura en GameCharacter
	texture = game->getTexture(TextureName::CharactersTexture);
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