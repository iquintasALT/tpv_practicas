#ifndef PACMAN_H_
#define PACMAN_H_

#include "Vector2D.h"
#include "Texture.h"
#include "checkML.h"
#include <fstream>

//clase Game para evitar dependecias circulares
class Game;

using namespace std;

class Pacman
{
private:
	Point2D currentPos, initialPos; //posicion actual e inicial
	Vector2D dir, nextDir; //direccion actual y nueva guardada
	Texture* texture; //puntero a textura
	Game* game; //puntero a Game
	int lives, energy, pacmanYFrame; //nº vidas y energia

public:
	Pacman(Point2D const point, Texture* const textureP, Game* const gameP, int const maxLives);
	~Pacman();
	void save(ofstream& output);
	void load(ifstream& input);
	void death();
	void update();
	void render() const;
	void handleEvent(SDL_Event& const event);
	int* getLivesPoint() { return &lives; };
	int* getEnergyPoint() { return &energy; };

private:
	void toroidalMove(Point2D& nextPos);
	void cellInteraction();
};

#endif