#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/SoundEffect.h"
#include "../utils/Vector2D.h"

class AsteroidsSystem : public System {
public:
	// - añade n asteroides al juego como en la práctica 1.
	void addAsteroid();
	void addAsteroid(int gen, Entity* hit_asteroid);
	void addAsteroids(int n);

	// - desactivar el asteroide “a” y crear 2 asteroides como en la práctica 1.
	// - pasamos una referencia a la bala aunque no se usa de momento (en el futuro
	// se puede usar para tener comportamientos distintos depende del tipo de
	// bala, etc.)
	// - si no hay más asteroides avisar al GameCtrlSystem
	void onCollisionWithBullet(Entity* a, Entity* b);

	void resetAsteroids() { numOfAsteroids_ = 0; };

	// - si el juego está parado no hacer nada.
	// - mover los asteroides como en la práctica 1.
	void update() override;
	void render() override;
	void asteroidOppositeSide(Entity* asteroid);
	void asteroidFollow(Entity* asteroid);

private:
	const int newAsteroidSpawn = 5000;
	std::size_t numOfAsteroids_;
	int numAsteroids = 0;
	int msToNextAsteroid = 0;
	int ms = 50;
};