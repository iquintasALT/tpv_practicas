#pragma once

#include "../ecs/Component.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"

#include "../components/State.h"
#include "../components/Generations.h"
#include "../components/FramedImage.h"
#include "../components/Follow.h"
#include "../components/Transform.h"

class AsteroidsManager : public Component {
public:
	AsteroidsManager() : numAsteroids(0), msToNextAsteroid(0), state_(nullptr) {}

	virtual ~AsteroidsManager() {}

	void init() override {
		state_ = entity_->getComponent<State>();

		assert(state_ != nullptr);
	}

	inline int getNumAsteroids() { return numAsteroids; }

	void update() override {
		if (state_->getState() == states::RUNNING && sdlutils().currRealTime() - msToNextAsteroid > 5000) {

			generateAsteroid();
			msToNextAsteroid = sdlutils().currRealTime();
		}
	}

	void generateAsteroid(int lives = 3)
	{
		auto asteroid = entity_->getMngr()->addEntity();
		////componentes
		asteroid->addComponent<Transform>(Vector2D(sdlutils().rand().nextInt() % sdlutils().width(),
			sdlutils().rand().nextInt() % sdlutils().height()),
			Vector2D(1.0f, 0.0f), 50.0f, 50.0f, 0.0f);

		asteroid->addComponent<Generations>(lives);
		////70% TIPO A, 30% TIPO B
		if (sdlutils().rand().nextInt() % 100 < 30) {
			asteroid->addComponent<Follow>(entity_->getMngr()->getHandler<Player_hdlr>()->getComponent<Transform>());
			asteroid->addComponent<FramedImage>(&sdlutils().images().at("asteroid_gold"), 5, 6);
		}
		else asteroid->addComponent<FramedImage>(&sdlutils().images().at("asteroid"), 5, 6);

		asteroid->setGroup<Asteroid_grp>(asteroid);
	}
	
	void onCollision(Entity* hit_asteroid) {
		int lives = hit_asteroid->getComponent<Generations>()->getLives();
		hit_asteroid->setActive(false);

		if (lives > 0)
			this->generateAsteroid(--lives);
	}

private:
	int numAsteroids;
	int msToNextAsteroid;
	State* state_;
};
