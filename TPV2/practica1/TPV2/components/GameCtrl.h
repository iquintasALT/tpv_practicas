#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

#include "State.h"
#include "Transform.h"
#include "FramedImage.h"
#include "Follow.h"


class GameCtrl : public Component {
public:
	GameCtrl() : state_(nullptr) {}

	virtual ~GameCtrl() {}

	void init() override {
		state_ = entity_->getComponent<State>();
		assert(state_ != nullptr);
	}

	void update() override {
		if (ih().isKeyDown(SDL_SCANCODE_SPACE) && state_->getState() != states::RUNNING) {
			state_->setState(state_->getState() > states::RUNNING ? states::NEWGAME : states::RUNNING);
			for (int i = 0; i < 10; i++) {
				auto asteroid = entity_->getMngr()->addEntity();
				asteroid->addComponent<Transform>(Vector2D(sdlutils().rand().nextInt() % sdlutils().width(),
					sdlutils().rand().nextInt() % sdlutils().height()),
					Vector2D(1.0f, 0.0f), 50.0f, 50.0f, 0.0f);
				asteroid->addComponent<FramedImage>(&sdlutils().images().at("asteroid"), 5, 6);
				asteroid->addComponent<Follow>(entity_->getMngr()->getHandler<Player_hdlr>()->getComponent<Transform>());
				asteroid->setGroup<Asteroid_grp>(asteroid);
			}
		}
	}

private:
	State* state_;
};
