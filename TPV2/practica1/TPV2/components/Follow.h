// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"

#include "Transform.h"

class Follow : public Component {
public:
	Follow(Transform* fighter_tr) :
		tr_(nullptr),
		fighter_tr_(fighter_tr)
	{
	}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
	}

	void update() override{
		auto& vel =  tr_->getVel();
		vel = vel.rotate(vel.angle(fighter_tr_->getPos() - tr_->getPos()) > 0 ? 1.0f : -1.0f);
	}

	virtual ~Follow() {
	}

private:
	Transform* tr_;
	Transform* fighter_tr_;
};
