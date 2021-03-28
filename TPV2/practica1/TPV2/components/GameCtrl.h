#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

#include "State.h"
#include "AsteroidsManager.h"
#include "Heart.h"

class GameCtrl : public Component {
public:
	GameCtrl() : state_(nullptr), asteroidsMngr_(nullptr), heart_(nullptr) {}

	virtual ~GameCtrl() {}

	void init() override {
		state_ = entity_->getComponent<State>();
		asteroidsMngr_ = entity_->getComponent<AsteroidsManager>();
		heart_ = entity_->getMngr()->getHandler<Player_hdlr>()->getComponent<Heart>();

		assert(state_ != nullptr && asteroidsMngr_ != nullptr && heart_ != nullptr);
	}

	void update() override {
		if (ih().isKeyUp(SDL_SCANCODE_SPACE) && state_->getState() != states::RUNNING) {
			state_->setState(state_->getState() > states::RUNNING ? states::NEWGAME : states::RUNNING);

			if (state_->getState() == states::RUNNING)
				for (int i = 0; i < 10; i++)
					asteroidsMngr_->generateAsteroid();
			else if (state_->getState() == states::NEWGAME) 
				heart_->resetLifes();
		}
	}

private:
	State* state_;
	AsteroidsManager* asteroidsMngr_;
	Heart* heart_;
};
