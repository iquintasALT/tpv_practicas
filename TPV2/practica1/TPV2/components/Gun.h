#pragma once

#include <SDL.h>
#include <cassert>

#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "Image.h"
#include "DisableOnExit.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"

class Gun : public Component
{
public:
	Gun(SoundEffect* sfx) :
		tr_(nullptr), sfx_(sfx) {};

	virtual ~Gun() {
	}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
	}

	void update() override {

		if (sdlutils().currRealTime() - msToNextBullet > 250) {
			if (ih().isKeyDown(SDL_SCANCODE_S)) {
				createBullet();
				sfx_->play();
				msToNextBullet = sdlutils().currRealTime();
			}
		}
	}

	void createBullet() {
		auto bullet = entity_->getMngr()->addEntity();

		Vector2D bPos = (tr_->getPos() + Vector2D(tr_->getW() / 2, tr_->getH() / 2)) + Vector2D(bulletW / 2.0f, bulletH / 2.0f) -
			Vector2D(0.0f, bulletH / 2.0f + 5.0f + 12.0f).rotate(tr_->getRot()) - Vector2D(2.0f, 10.0f);

		Vector2D bVel = Vector2D(0.0f, -1.0f).rotate(tr_->getRot()) * (tr_->getVel().magnitude() + 5.0f);

		bullet->addComponent<Transform>(bPos, bVel, bulletW, bulletH, tr_->getRot());
		bullet->addComponent<Image>(&sdlutils().images().at("fire"));
		bullet->addComponent<DisableOnExit>(sdlutils().width(), sdlutils().height());
	}

private:
	Transform* tr_;
	SoundEffect* sfx_;
	const int bulletH = 20.0f;
	const int bulletW = 5.0f;
	int msToNextBullet = 0;
};

