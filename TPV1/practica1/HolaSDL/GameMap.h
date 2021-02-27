#ifndef GAMEMAP_H_
#define GAMEMAP_H_

#include "Texture.h"
#include "checkML.h"
#include <fstream>

//clase Game para evitar dependecias circulares
class Game;

using namespace std;

//enumerado de tipos de casillas
enum class MapCell { Empty, Wall, Food, Vitamins };

class GameMap
{
private:
	MapCell** cells; //array dinámico bidimensional de tipos de casillas
	int fils, cols; //numero de filas y columnas
	int foodCount; //contador de comidas
	Game* game; //puntero a Game

public:
	GameMap(int fils, int cols, Game* gameP);
	~GameMap();
	void save(ofstream& output);
	void writeCell(int const x, int const y, int const value);
	bool eraseCellContent(int const x, int const y);
	//metodo para leer el contenido de una casilla del mapa
	MapCell readCell(int const x, int const y) const { return cells[x][y]; };
	void render() const;
	#pragma region Getters
	int getCols() const { return cols; };
	int getFils() const { return fils; };
	int getFoodCount() const { return foodCount; };
	#pragma endregion
};

#endif