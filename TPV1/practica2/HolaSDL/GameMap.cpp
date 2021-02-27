#include "GameMap.h"
#include "Game.h";

//constructora con parametros de GameMap
GameMap::GameMap(int fils, int cols, Game* gameP) :
	GameObject(Point2D(0, 0), winHeight, winWidth - infoBarWidth, gameP) {
	//asignamos las variables acorde a los parametros dados
	this->fils = fils;
	this->cols = cols;
	//creamos un array dinamico de punteros a MapCell del tamaño de las columnas
	cells = new MapCell * [this->cols];
	//por cada fila, le asignamos un array dinamicos de MapCell de tamaño filas
	for (int i = 0; i < this->cols; ++i) cells[i] = new MapCell[this->fils];
}

//constructora por flujo de GameMap
GameMap::GameMap(ifstream& loadFile, Game* gameP) { //currentPos se asigna a (0, 0) por la constructora por defecto
	loadFile >> width >> height >> fils >> cols;
	//constructora por defecto GO, añadimos datos a posteriori
	game = gameP;
	//creamos un array dinamico de punteros a MapCell del tamaño de las columnas
	cells = new MapCell * [cols];
	//por cada fila, le asignamos un array dinamicos de MapCell de tamaño filas
	for (int i = 0; i < cols; ++i) cells[i] = new MapCell[fils];
}

//destructora de GameMap
GameMap::~GameMap() {
	//por cada array en fila, eliminamos sus elementos, y eliminamos la referencia
	for (int i = 0; i < cols; ++i) {
		delete[] cells[i];
		cells[i] = nullptr;
	}
	//eliminamos los elementos del array de arrays, y eliminamos su referencia
	delete[] cells;
	cells = nullptr;
}

//guardado de partida en archivo de GameMap
void GameMap::saveToFile(ofstream& saveFile){
	//guardamos lo relativo a GameObject
	saveFile << width << " " << height << endl;
	//guardamos filas y columnas
	saveFile << fils << " " << cols << endl;

	for (int i = 0; i < fils; i++) { //por cada fila
		for (int j = 0; j < cols; j++) { //por cada columna
			saveFile << int(cells[j][i]); //guardamos el valor de la celda
			if (j + 1 != fils) saveFile << " ";
		}
		saveFile << endl;
	}
}

//metodo para asignar un valor a cada casilla
void GameMap::writeCell(int const x, int const y, int const value, int& foodCount) {
	cells[x][y] = (MapCell)value;
	//si la casilla es comida, aumentamos el contador
	if (cells[x][y] == MapCell::Food || cells[x][y] == MapCell::Vitamins)
		foodCount++;
}

//metodo para eliminar el contenido de una casilla cuando el Pacman pase sobre ella (si es posible)
bool GameMap::eraseCellContent(SDL_Rect& const point, int& foodCount) {
	//onbtenemos la coordenada del centro del rectangulo
	Point2D pacmanCenter = SDLPointToMapCoors(Point2D(point.x + (point.w / 2), point.y + (point.h / 2)));

	bool vitamins = false; //booleano de control
	if (cells[pacmanCenter.getXCoor()][pacmanCenter.getYCoor()] != MapCell::Empty) {
		foodCount--; //si es Food o Vitamins, reduce contador
		if (cells[pacmanCenter.getXCoor()][pacmanCenter.getYCoor()] == MapCell::Vitamins) vitamins = true; //si es Vitamins, activa powerUp
	}
	cells[pacmanCenter.getXCoor()][pacmanCenter.getYCoor()] = MapCell::Empty; //en cualquier caso, vaciamos la celda (incluso si esta vacia)
	//avisamos si habia o no powerUp en la casilla
	return vitamins;
}

//metodo para detectar si un rectangulo intersecciona con alguna pared del mapa
bool GameMap::intersectWall(SDL_Rect& const rectObj) const{
	//cogemos las coordenadas del mapa de la esquina superior izquierda e inferior derecha
	Point2D topLeft = SDLPointToMapCoors(Point2D(rectObj.x, rectObj.y));
	Point2D botRight = SDLPointToMapCoors(Point2D(rectObj.x + rectObj.w - 1, rectObj.y + rectObj.h - 1));

	//recorremos todas las casillas que cubre el rectangulo en busca de una interseccion con una pared
	bool intersect = false;
	int i = topLeft.getXCoor();
	while (i <= botRight.getXCoor() && !intersect) {
		int j = topLeft.getYCoor();
		while (j <= botRight.getYCoor() && !intersect) {
			intersect = (cells[i][j] == MapCell::Wall);
			j++;
		}
		i++;
	}

	return intersect;
}

//metodo de renderizado del mapa
void GameMap::render() const {
	SDL_Rect dstRect;
	dstRect.h = height / fils; //altura de la celda
	dstRect.w = width / cols; //anchura de la celda
	for (int i = 0; i < cols; i++) { //por cada fila
		for (int j = 0; j < fils; j++) { //por cada columna
			if (cells[i][j] != MapCell::Empty) {
				dstRect.x = i * dstRect.w; //fila
				dstRect.y = j * dstRect.h; //columna
				//obtenemos la textura acorde al contenido del mapa
				Texture* texture = game->getTexture((TextureName)((int)cells[i][j] - 1));
				texture->render(dstRect, SDL_FLIP_NONE);
			}
		}
	}
}

//metodo para convertir un punto de la ventana (en pixel) a una coordenada del mapa
Point2D GameMap::SDLPointToMapCoors(Point2D& const point) const {
	return Point2D(point.getXCoor() / (width / cols), point.getYCoor() / (height / fils));
}

//metodo para convertir una coordenada del mapa a un punto de la ventana (en pixel)
Point2D GameMap::MapCoorsToSDLPoint(Point2D& const mapCoor) const {
	return Point2D(mapCoor.getXCoor() * (width / cols), mapCoor.getYCoor() * (height / fils));
}