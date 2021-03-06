#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "Transform.h"

class DeAcceleration : public Component
{
public:
	DeAcceleration() :
		tr_(nullptr) {};

	virtual ~DeAcceleration() {
	}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
	}

	void update() override {
		tr_->setVel(tr_->getVel() * 0.955f);
	}

private:
	Transform* tr_;
};