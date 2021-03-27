#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

#include "State.h"
#include "AsteroidsManager.h"
#include "Transform.h"

class GameCtrl : public Component {
public:
	GameCtrl() : state_(nullptr) {}

	virtual ~GameCtrl() {}

	void init() override {
		state_ = entity_->getComponent<State>();
		asteroidsMngr_ = entity_->getComponent<AsteroidsManager>();
		assert(state_ != nullptr && asteroidsMngr_ != nullptr);
	}

	void update() override {
		if (ih().isKeyDown(SDL_SCANCODE_SPACE) && state_->getState() != states::RUNNING) {
			state_->setState(state_->getState() > states::RUNNING ? states::NEWGAME : states::RUNNING);
			for (int i = 0; i < 10; i++) {
				asteroidsMngr_->generateAsteroid();
			}
		}
	}

private:
	State* state_;
	AsteroidsManager* asteroidsMngr_;
};
