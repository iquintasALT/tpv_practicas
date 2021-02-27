#include "Ghost.h"
#include "Game.h"

//constructora de Ghost
Ghost::Ghost(Point2D const point, int const ghostNum, Texture* const textureP, Game* const gameP) {
	//asignamos las variables acorde a los parametros dados
	currentPos = initialPos = point;
	ghostFrameY = ghostNum * 2; //frame relativo al nº del Ghost
	dir = Vector2D(0, 0);
	//asignamos el puntero al Game y la textura
	game = gameP;
	texture = textureP;
}


//destructora de Ghost
Ghost::~Ghost() {
	//eliminamos referencia al Game y la textura
	game = nullptr;
	texture = nullptr;
}

//metodo para guardar la informacion de cada Ghost en un archivo
void Ghost::save(ofstream& output) {
	output << currentPos.getXCoor() << " " << currentPos.getYCoor() << " " 
		<< initialPos.getXCoor() << " " << initialPos.getYCoor() << endl;
}

//metodo para la carga del estado de Ghost en la partida guardada
void Ghost::load(ifstream& input) {
	int x, y, xIni, yIni;
	input >> x >> y >> xIni >> yIni;
	currentPos = Vector2D(x, y);
	initialPos = Vector2D(xIni, yIni);
}

//metodo de actualizacion de la posicion del Ghost
void Ghost::update() {
	//vector de posibles direcciones
	vector<Vector2D> possibleDirs;
	//para cada direccion
	for (const Vector2D& elem : dirs) {
		Point2D nextPos = elem + currentPos;
		toroidalMove(nextPos);
		//si no es la opuesta y si esa celda esta disponible
		if (!(elem == dir.reverse()) && (game->nextCell(nextPos))) 
			possibleDirs.push_back(elem); //guardo la dirección
	}

	if (possibleDirs.size() == 0) dir = dir.reverse();
	else {
		//rand entre las celdas posibles
		dir = possibleDirs[rand() % possibleDirs.size()];
		Point2D nextPos = currentPos + dir;
		//en caso de salirse del tablero, hace un movimiento toroidal
		toroidalMove(nextPos);
		currentPos = nextPos; //actualizamos posicion
	}
}

//metodo de renderizado de Ghost
void Ghost::render() const {
	SDL_Rect dstRect; //tamaño del Ghost
	dstRect.h = game->getCellHeight(); //altura del Ghost
	dstRect.w = game->getCellWidth(); //anchura del Ghost
	dstRect.x = currentPos.getXCoor() * dstRect.w; //columnas
	dstRect.y = currentPos.getYCoor() * dstRect.h; //filas
	//usamos el frame especifico de cada fantasma
	texture->renderFrame(dstRect, 0, ghostFrameY);
}

//metodo privado para simular el movimiento toroidal
void Ghost::toroidalMove(Point2D& nextPos) {
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