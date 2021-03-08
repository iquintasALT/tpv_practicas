#pragma once

#include <SDL.h>
#include <cassert>

#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "Transform.h"

class FighterCtrl : public Component {
public:
	FighterCtrl(SoundEffect* sfx) : tr_(nullptr), speed_(1.0), sfx_(sfx) {}

	virtual ~FighterCtrl() {}

	inline void setSpeed(float speed) {
		speed_ = speed;
	}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
	}

	void update() override {
		if (ih().keyDownEvent()) {

			if (ih().isKeyDown(SDL_SCANCODE_UP)) {
				auto& vel = tr_->getVel();
				auto newVel = vel + (Vector2D(0, -1).rotate(tr_->getRot()) * thrust).normalize();
				vel.set((newVel.magnitude() > speedLimit) ?
					(Vector2D(0, -1).rotate(tr_->getRot()) * thrust).normalize()
					: newVel);
				sfx_->play();
			}
			else if (ih().isKeyDown(SDL_SCANCODE_LEFT)) {
				tr_->setRot(tr_->getRot() - 5.0f);
			}
			else if (ih().isKeyDown(SDL_SCANCODE_RIGHT)) {
				tr_->setRot(tr_->getRot() + 5.0f);
			}
		}
	}

private:
	Transform* tr_;
	SoundEffect* sfx_;
	float speed_;
	const float thrust = .2f, speedLimit = 3.0f;
}
;

