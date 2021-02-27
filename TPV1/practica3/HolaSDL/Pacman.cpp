#include "Pacman.h"
#include "PlayState.h"

#pragma region Static_Consts
const string Pacman::identifier = "P "; //identificador
#pragma endregion

//constructora por flujo de Pacman
Pacman::Pacman(ifstream& loadFile, SDLApplication* const appPtr, PlayState* const playStatePtr) : GameCharacter(loadFile, appPtr, playStatePtr) {
	loadFile >> energy >> nextDir; //cargamos los parametros exclusivos del Pacman
}

//guardado de partida en archivo de Pacman
void Pacman::saveToFile(ofstream& saveFile) {
	saveFile << identifier; //marcamos el identificador de datos correspondiente
	GameCharacter::saveToFile(saveFile); //guardamos lo relativo a GameCharacter
	saveFile << " " << energy << " " << nextDir << endl; //guardamos lo exclusivo de Pacman
}

//método de actualización del Pacman
void Pacman::update() {
	//Si al moverse el Ghost, se solapa con el Pacman, este muere
	if (playState->collisionGhost(getDestRect())) {
		death();
	}
	else {
		//punto auxiliar para comprobaciones
		Point2D nextPos;
		//comprobamos si se puede mover al nuevo punto usando la nueva direccion
		if (playState->tryMove(getDestRect(), nextDir, nextPos)) {
			//si puede, actualizamos parametros
			dir = nextDir;
			currentPos = nextPos;
		}
		else if (playState->tryMove(getDestRect(), dir, nextPos)) {
			//comprobamos si se puede continuar en la direccion previa,
			//y actualizamos parametros en caso afirmativo
			currentPos = nextPos;
		}
		//comprobamos interacciones en la nueva posicion (comida y Ghosts)
		playState->cellInteraction(getDestRect());

		if (energy > 0) {
			energy--; //reducimos la energia en caso de que este con vitaminas
			if (energy == 0) setNewFrame(10); //cambiamos frame al dejar de tener el powerUp
		}
	}
}

//metodo para controlar eventos especificos del Pacman (direcciones)
bool Pacman::handleEvent(SDL_Event& const event) {
	if (event.type == SDL_KEYDOWN) { //si es una tecla
		switch (event.key.keysym.sym) {
		case SDLK_RIGHT: //direccion --> derecha
			nextDir.setXCoor(1);
			nextDir.setYCoor(0);
			break;
		case SDLK_LEFT: //direccion --> izquierda
			nextDir.setXCoor(-1);
			nextDir.setYCoor(0);
			break;
		case SDLK_UP: //direccion --> arriba
			//las filas van de "0 a n" no de "n a 0", por lo que para subir hay que restar
			nextDir.setXCoor(0);
			nextDir.setYCoor(-1);
			break;
		case SDLK_DOWN: //direccion --> abajo
			//las filas van de "0 a n" no de "n a 0", por lo que para bajar hay que sumar
			nextDir.setXCoor(0);
			nextDir.setYCoor(1);
			break;
		}
	}
	return false;
}

//metodo para actualizar el Pacman tras morir
void Pacman::death() {
	currentPos = initialPos; //lo colocamos en su posicion inicial
	dir = nextDir = Vector2D(0, 0); //reseteamos la direccion
	SDL_Delay(timeAfterDeath); //paramos 'x' tiempo la ejecucion
	playState->emptyBuffer(); //limpiamos buffer de eventos
}