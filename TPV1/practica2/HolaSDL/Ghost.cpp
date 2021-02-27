#include "Ghost.h"
#include "Game.h"

#pragma region Static_Consts
//array constante de direcciones de Ghost
const Vector2D Ghost::dirs[4] = { {1,0}, {-1,0}, {0,-1}, {0,1} };
const string Ghost::identifier = "G ";
#pragma endregion

//constructora por flujo de Ghost
Ghost::Ghost(ifstream& loadFile, Game* const gamePtr) : GameCharacter(loadFile, gamePtr) { 
	loadFile >> timeToChangeDir;
}

//guardado de partida en archivo de Ghost
void Ghost::saveToFile(ofstream& saveFile){
	saveFile << identifier; //marcamos el identificador de datos correspondiente
	GameCharacter::saveToFile(saveFile); //guardamos lo relativo a GameCharacter
	saveFile << " " << timeToChangeDir << endl; //guardamos lo exclusivo de Ghost
}

//metodo de actualizacion de la posicion del Ghost
void Ghost::update() {
	if (timeToChangeDir == 0) { //si le toca cambiar de direccion
		//vector de posibles direcciones
		vector<Vector2D> possibleDirs, possibleMoves;
		//para cada direccion
		for (const Vector2D& elem : dirs) {
			Point2D nextPos;
			//si no es la opuesta y si esa celda esta disponible
			if (!(elem == dir.reverse()) && game->tryMove(getDestRect(), elem, nextPos)) {
				possibleDirs.push_back(elem); //guardo la dirección
				possibleMoves.push_back(nextPos); //guardamos el movimiento correspondiente a la direccion
			}
		}
		//si no hay direcciones posibles, vuelve en la direccion contraria a la que iba
		if (possibleDirs.size() == 0) dir = dir.reverse();
		else { //si hay direcciones
			//elegimos aleatoriamente entre las posibles, y asignamos la que corresponda
			int i = rand() % possibleDirs.size();
			dir = possibleDirs[i];
			currentPos = possibleMoves[i];
		}

		timeToChangeDir = 26; //reiniciamos el timer
	}
	else { //si aun no puede cambiar de direccion
		//sigue moviendose en su direccion salvo que no se pueda (hay pared)
		Point2D nextPos;
		if (game->tryMove(getDestRect(), dir, nextPos)) 
			currentPos = nextPos;

		timeToChangeDir--; //restamos el timer
	}
}