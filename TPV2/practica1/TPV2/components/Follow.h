// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/Texture.h"

#include "Transform.h"

class Follow : public Component {
public:
	Follow() : tr_(nullptr) {}

	virtual ~Follow() {}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
	}

	void update() override {
		Transform* fighter_tr_ = entity_->getMngr()->getHandler<Player_hdlr>()->getComponent<Transform>();
		auto& vel = tr_->getVel();
		vel = vel.rotate(vel.angle(fighter_tr_->getPos() - tr_->getPos()) > 0 ? 1.0f : -1.0f);
	}

private:
	Transform* tr_;
};
