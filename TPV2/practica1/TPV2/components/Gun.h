#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

#include "Transform.h"
#include "Image.h"
#include "DisableOnExit.h"

class Gun : public Component
{
public:
	Gun(SoundEffect* sfx) : tr_(nullptr), bullet_sfx_(sfx) {};

	virtual ~Gun() { }

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
	}

	void update() override {
		if (sdlutils().currRealTime() - msToNextBullet > nextBullet) {
			if (ih().isKeyDown(SDL_SCANCODE_S)) {
				createBullet();
				bullet_sfx_->play();
				msToNextBullet = sdlutils().currRealTime();
			}
		}
	}

private:
	Transform* tr_;
	SoundEffect* bullet_sfx_;

	const float bulletH = 20.0f, bulletW = 5.0f;
	const int nextBullet = 250;
	int msToNextBullet = 0;

	void createBullet();
};

