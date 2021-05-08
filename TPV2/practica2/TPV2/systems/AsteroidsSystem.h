#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/SoundEffect.h"
#include "../utils/Vector2D.h"

/// <summary>
/// Sistema que controla la generacion y destruccion de asteroides
/// </summary>
class AsteroidsSystem : public System {
public:
	void update() override;

	void render() override;

	void receive(const Message& msg) override;

private:
	const int newAsteroidSpawnTime = 10000;
	const int ms = 50;
	int msToNextAsteroid = 0;

	int numOfAsteroids_ = 0;
	bool isRunning = false;


	void addAsteroids(int n);
	//metodo que genera un asteroide a partir de la posicion de uno anterior
	void newAsteroidChild(int gen, Entity* hit_asteroid);
	//metodo que genera un asteroide en una posición aleatoria en los bordes de la ventana
	void newAsteroid();

	//metodo para conseguir el movimiento toroidal
	void toroidalAsteroid(Entity* asteroid);
	//metodo para que algunos asteroides sigan al jugador
	void followAsteroid(Entity* asteroid);

	//metodo llamado cuando colsionan una bala y un asteroide
	void onCollisionWithBullet(Entity* a, Entity* b);
};