#pragma once

#include <SDL.h>
#include <cassert>

#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "Transform.h"

class Rotate : public Component {
public:
	Rotate() :
		tr_(nullptr) {
	}
	virtual ~Rotate() {
	}


	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
	}

	void update() override {
		if (ih().keyDownEvent()) {
			 if (ih().isKeyDown(SDL_SCANCODE_LEFT)) {
				 tr_->setRot(tr_->getRot() - 5.0f);
		}
			else if (ih().isKeyDown(SDL_SCANCODE_RIGHT)) {
				 tr_->setRot(tr_->getRot() + 5.0f);
			}
		}
	}

private:
	Transform* tr_;
};