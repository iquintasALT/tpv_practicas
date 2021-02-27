#include "SmartGhost.h"
#include "PlayState.h"

#pragma region Static_Consts
const int SmartGhost::times[numStates] = { 1350, 8100, 2700, 405 };
const string SmartGhost::identifier = "S ";
#pragma endregion

//constructora por flujo de SmartGhost
SmartGhost::SmartGhost(ifstream& loadFile, SDLApplication* const appPtr, PlayState* const playStatePtr) : Ghost(loadFile, appPtr, playStatePtr) {
	loadFile >> edad >> cuarentena >> deathCountdown;
	if (deathCountdown > 0) state = State::Dead;
	else if (cuarentena > 0) state = State::Quarantine;
	else if (edad >= times[(int)State::Child]) state = State::Adult;
}

//guardado de partida en archivo de SmartGhost
void SmartGhost::saveToFile(ofstream& saveFile) {
	saveFile << identifier; //marcamos el identificador de datos correspondiente
	GameCharacter::saveToFile(saveFile); //guardamos lo relativo a GameCharacter
	//(no usamos el metodo heredado de SmartGhost para evitar conflictos con el identificador)
	saveFile << " " << timeToChangeDir << " " << edad << " " << cuarentena << " " << deathCountdown << endl;
};

//metodo de actualizacion de la posicion del Ghost
void SmartGhost::update() {
	//hacemos los cambios necesarios en el estado
	handleState(state); 
	//50*27
	if (state == State::Child) { //si es niño, se comporta como un fantasma normal
		Ghost::update();
		edad++; //incrementamos edad
	}
	else if (state != State::Dead) { //si es adulto
		if (timeToChangeDir == 0) { //en caso de que pueda cambiar de direccion
			//array constante de direcciones de Ghost
			Vector2D const dirs[4] = { {1,0}, {-1,0}, {0,-1}, {0,1} };
			//vector de posibles direcciones
			vector<Vector2D> possibleDirs, possibleMoves;
			//para cada direccion
			for (const Vector2D& elem : dirs) {
				Point2D nextPos;
				//si no es la opuesta y si esa celda esta disponible
				if (!(elem == dir.reverse()) && playState->tryMove(getDestRect(), elem, nextPos)) {
					possibleDirs.push_back(elem); //guardo la dirección
					possibleMoves.push_back(nextPos); //guardamos el movimiento correspondiente a la direccion
				}
			}

			//si no hay direcciones posibles, vuelve en la direccion contraria a la que iba
			if (possibleDirs.size() == 0) dir = dir.reverse();
			else //en caso contrario, nos quedamos con la direccion que tenga menor modulo con el fantasma
				playState->checkModule(dir, currentPos, possibleDirs, possibleMoves);

			timeToChangeDir = 26; //reiniciamos el timer
		}
		else { //si aun no puede cambiar de direccion
			//sigue moviendose en su direccion salvo que no se pueda (hay pared
			Point2D nextPos;
			if (playState->tryMove(getDestRect(), dir, nextPos))
				currentPos = nextPos;
			timeToChangeDir--; //restamos el timer
		}

		if (state != State::Quarantine && state != State::Dead) { //comprobamos si se puede reproducir
			if (playState->reproductionSmartGhost(this))
				state = State::Quarantine; //si se reproduce, se pone en cuarentena
		}
		else { //si no se puede reproducir (cuarentena)
			cuarentena++; //aumentamos el contador de cuarentena
			if (cuarentena >= times[(int)State::Quarantine]) { //cuando haya llegado al final
				state = State::Adult; //le quitamos la cuarentena
				cuarentena = 0; //reseteamos el timer
			}
		}
		edad++; //incrementamos edad
	}
	else //en caso de estar muerto, incrementamos el contador de muerte
		deathCountdown++;
}

//metodo de renderizado del SmartGhost
void SmartGhost::render() const {
	//si es adulto, se ejecuta el render por defecto
	if (edad >= times[(int)State::Child]) GameCharacter::render();
	else { //en caso contrario, reducimos la dimension del rectangulo a la mitad (en renderizado)
		SDL_Rect dstRect = getDestRect();
		dstRect.w /= 2; dstRect.h /= 2;
		dstRect.x += dstRect.w / 2; dstRect.y += dstRect.h / 2;
		texture->renderFrame(dstRect, frameCoors.getXCoor(), frameCoors.getYCoor());
	}
}

//metodo de manejo del estado del SmartGhost
void SmartGhost::handleState(State& state) {
	if (state == State::Child && edad >= times[(int)State::Child])
		state = State::Adult; //si ha llegado a cierta edad como niño, pasa a ser adulto
	else if ((state == State::Adult || state == State::Quarantine) && edad >= times[(int)State::Adult])
		state = State::Dead; //si ha llegado a cierta edad como adulto, pasa a estar muerto
	if (state == State::Dead && deathCountdown >= times[(int)State::Dead])
		playState->eraseGhost(listPos, this); //tras haber pasado cierto tiempo muerto, se elimina
}
