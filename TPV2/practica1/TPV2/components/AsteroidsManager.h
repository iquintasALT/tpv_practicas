#pragma once

#include "../ecs/Component.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"

#include "State.h"
#include "Generations.h"
#include "ShowAtOppositeSide.h"
#include "FramedImage.h"
#include "Follow.h"
#include "Transform.h"

class AsteroidsManager : public Component {
public:
	AsteroidsManager() : numAsteroids(0), msToNextAsteroid(0), state_(nullptr) {}

	virtual ~AsteroidsManager() {}

	void init() override {
		state_ = entity_->getComponent<State>();

		assert(state_ != nullptr);
	}

	void update() override {
		if (state_->getState() == states::RUNNING && sdlutils().currRealTime() - msToNextAsteroid > newAsteroidSpawn) {
			generateAsteroid();
			msToNextAsteroid = sdlutils().currRealTime();
		}
	}

	inline int getNumAsteroids() { return numAsteroids; }

	void generateAsteroid();

	void generateAsteroid(int lives, Entity* prev_asteroid);
	
	void onCollision(Entity* hit_asteroid);

private:
	State* state_;

	int numAsteroids;
	const int newAsteroidSpawn = 5000;
	int msToNextAsteroid;
};
