#include "Pacman.h"
#include "Game.h"

//constructora de Pacman
Pacman::Pacman(Point2D const point, Texture* const textureP, Game* const gameP, int const maxLives) {
	//asignamos las variables respecto a los parametros introducidos
	currentPos = initialPos = point;
	dir = nextDir = Vector2D(0, 0);
	lives = maxLives;
	energy = 0;
	pacmanYFrame = 10;
	//adignamos los punteros a Game y la textura
	game = gameP;
	texture = textureP;
}

//destructora de Pacman
Pacman::~Pacman() {
	//eliminamos referencia al Game y la textura
	game = nullptr;
	texture = nullptr;
}

//metodo para guardar la informacion de Pacman en un archivo
void Pacman::save(ofstream& output) {
	output.open("..\\savedGame\\Pacman.txt");
	if (!output.is_open()) throw string("ERROR: Pacman saved file not created correctly");
	output << currentPos.getXCoor() << " " << currentPos.getYCoor() << endl;
	output << initialPos.getXCoor() << " " << initialPos.getYCoor() << endl;
	output << lives << endl;
	output << energy << endl;

	output.close();
}

//metodo para la carga del estado de Pacman en la partida guardada
void Pacman::load(ifstream& input) {
	int x, y, xIni, yIni, liveCount, energyCount;
	input >> x >> y >> xIni >> yIni >> liveCount >> energyCount;
	currentPos = { x, y };
	initialPos = { xIni, yIni };
	lives = liveCount; energy = energyCount;
	if (energy > 0) pacmanYFrame = 11;

	input.close();
}


//metodo para actualizar el Pacman tras morir
void Pacman::death() {
	currentPos = initialPos; //lo colocamos en su posicion inicial
	dir = nextDir = Vector2D(0, 0); //reseteamos la direccion
	lives--; //restamos vidas
	//si esta muerto, acabamos partida
	if (lives == 0) game->gameOver();
	SDL_Delay(timeAfterDeath); //paramos 'x' tiempo la ejecucion
	//vaciamos la lista de eventos que estaban a la espera de ser revisados (input Pacman)
	game->emptyBuffer();
}

//método de actualización del Pacman
void Pacman::update() {
	//Si al moverse el Ghost, se solapa con el Pacman, este muere
	if (game->collisionGhost(currentPos, energy)) death();
	else {
		//punto auxiliar para comprobaciones
		Point2D nextPos = (currentPos + nextDir);
		//en caso de salirse del tablero, hace un movimiento toroidal
		toroidalMove(nextPos);
		//comprobamos si se puede mover al nuevo punto usando la nueva direccion
		if (game->nextCell(nextPos)) {
			//si puede, actualizamos parametros
			dir = nextDir;
			currentPos = nextPos;
		}
		else {
			//comprobamos si se puede continuar en la direccion previa,
			//y actualizamos parametros en caso afirmativo
			nextPos = (currentPos + dir);
			toroidalMove(nextPos);
			if (game->nextCell(nextPos))
				currentPos = nextPos;
		}

		//comprobamos interacciones en la nueva posicion (comida y Ghosts)
		cellInteraction();
		//reducimos enrgia si esta activa
		if (energy > 0) {
			energy--;
			if (energy == 0) {
				pacmanYFrame = 10;
			}
		}
	}
}

//metodo de renderizado del Pacman
void Pacman::render() const {
	SDL_Rect dstRect; //tamaño del Pacman
	dstRect.h = game->getCellHeight(); //altura del Pacman
	dstRect.w = game->getCellWidth(); //anchura del Pacman
	dstRect.x = currentPos.getXCoor() * dstRect.w; //columnas
	dstRect.y = currentPos.getYCoor() * dstRect.h; //filas
	//usamos el frame del Pacman
	texture->renderFrame(dstRect, 0, pacmanYFrame);
}

//metodo para controlar eventos especificos del Pacman (direcciones)
void Pacman::handleEvent(SDL_Event& const event) {
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
}

//metodo privado para simular el movimiento toroidal
void Pacman::toroidalMove(Point2D& nextPos) {
	//vuelta en la X (columnas)
	if (nextPos.getXCoor() < 0)
		nextPos.setXCoor(game->getCols() - 1);
	else if (nextPos.getXCoor() > game->getCols() - 1)
		nextPos.setXCoor(0);
	//vuelta en la Y (filas)
	else if (nextPos.getYCoor() < 0)
		nextPos.setYCoor(game->getFils() - 1);
	else if (nextPos.getYCoor() > game->getFils() - 1)
		nextPos.setYCoor(0);
}

//metodo de interaccion con la celda
void Pacman::cellInteraction() {
	if (!game->collisionGhost(currentPos, energy)) { //si no colisiona con un fantasma
		//si obtuvo vitamina, activamos powerUp y actualizamos frame
		if (game->eraseCellFood(currentPos)) {
			energy = maxEnergy;
			pacmanYFrame = 11;
		}
	}
	else death();
}