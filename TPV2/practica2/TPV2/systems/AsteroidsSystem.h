#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/SoundEffect.h"
#include "../utils/Vector2D.h"

class AsteroidsSystem : public System {
public:
	void update() override;

	void render() override;

	void receive(const Message& msg) override;

private:
	const int newAsteroidSpawnTime = 5000;
	const int ms = 50;
	int msToNextAsteroid = 0;

	int numOfAsteroids_ = 0;
	bool isRunning = false;

	void addAsteroids(int n);
	void newAsteroidChild(int gen, Entity* hit_asteroid);
	void newAsteroid();

	void toroidalAsteroid(Entity* asteroid);
	void followAsteroid(Entity* asteroid);

	void onCollisionWithBullet(Entity* a, Entity* b);
};