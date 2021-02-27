#include "GameMap.h"
#include "Game.h";

//constructora de GameMap
GameMap::GameMap(int fils, int cols, Game* gameP) {
	//asignamos las variables acorde a los parametros dados
	this->fils = fils;
	this->cols = cols;
	foodCount = 0;
	//asignamos el puntero al Game
	game = gameP;
	//creamos un array dinamico de punteros a MapCell del tamaño de las columnas
	cells = new MapCell * [cols];
	//por cada fila, le asignamos un array dinamicos de MapCell de tamaño filas
	for (int i = 0; i < cols; ++i) cells[i] = new MapCell[fils];
}

//destructora de GameMap
GameMap::~GameMap() {
	game = nullptr; //eliminamos puntero a Game
	//por cada array en fila, eliminamos sus elementos, y eliminamos la referencia
	for (int i = 0; i < cols; ++i) {
		delete[] cells[i];
		cells[i] = nullptr;
	}
	//eliminamos los elementos del array de arrays, y eliminamos su referencia
	delete[] cells;
	cells = nullptr;
}

//metodo para guardar la informacion de GameMap
void GameMap::save(ofstream& output) {
	output.open("..\\savedGame\\GameMap.txt");
	if (!output.is_open()) throw "ERROR: GameMap saved file not created correctly";
	output << fils << endl;
	output << cols << endl;

	for (int i = 0; i < fils; i++) { //por cada fila
		for (int j = 0; j < cols; j++) { //por cada columna
			output << int(cells[j][i]);
			if (j + 1 != fils) output << " ";
		}
		output << endl;
	}

	output.close();
}

//metodo para asignar un valor a cada casilla
void GameMap::writeCell(int const x, int const y, int const value) {
	cells[x][y] = (MapCell)value;
	//si la casilla es comida, aumentamos el contador
	if (cells[x][y] == MapCell::Food || cells[x][y] == MapCell::Vitamins)
		foodCount++;
}

//metodo para eliminar el contenido de una casilla cuando el Pacman pase sobre ella (si es posible)
bool GameMap::eraseCellContent(int const x, int const y) {
	bool vitamins = false; //booleano de control
	if (cells[x][y] != MapCell::Empty) {
		foodCount--; //si es Food, reduce contador
		if (cells[x][y] == MapCell::Vitamins) vitamins = true; //si es Vitamins, activa powerUp
	}
	cells[x][y] = MapCell::Empty; //en cualquier caso, vaciamos la celda (incluso si esta vacia)
	//avisamos si habia o no powerUp en la casilla
	return vitamins;
}

//metodo de renderizado del mapa
void GameMap::render() const {
	SDL_Rect dstRect;
	dstRect.h = game->getCellHeight(); //altura de la celda
	dstRect.w = game->getCellWidth(); //anchura de la celda
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