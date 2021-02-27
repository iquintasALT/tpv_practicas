#ifndef GAMEMAP_H_
#define GAMEMAP_H_

#include "GameObject.h"
#include "Vector2D.h"
#include "Texture.h"
#include "checkML.h"
#include <fstream>

//clase Game para evitar dependecias circulares
class Game;

using namespace std;

//enumerado de tipos de casillas
enum class MapCell { Empty, Wall, Food, Vitamins };

class GameMap: public GameObject{
private:
	MapCell** cells; //array dinámico bidimensional de tipos de casillas
	int fils, cols; //numero de filas y columnas

public:
	GameMap(int fils, int cols, Game* const gameP);
	GameMap(ifstream& loadFile, Game* const gameP);
	virtual ~GameMap();
	void saveToFile(ofstream& saveFile);
	void writeCell(int const x, int const y, int const value, int& foodCount);
	bool eraseCellContent(SDL_Rect& const point, int& foodCount);
	bool intersectWall(SDL_Rect& const rectObj) const;
	virtual void render() const;
	Point2D SDLPointToMapCoors(Point2D& const point) const;
	Point2D MapCoorsToSDLPoint(Point2D& const mapCoor) const;
	#pragma region Getters
	int getCols() const { return cols; };
	int getFils() const { return fils; };
	#pragma endregion
};

#endif