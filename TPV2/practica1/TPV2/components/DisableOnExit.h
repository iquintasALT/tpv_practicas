#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"

#include "Transform.h"

class DisableOnExit : public Component
{
	DisableOnExit(int const width, int const height) : tr_(nullptr), winHeight(height), winWidth(width) {};

	virtual ~DisableOnExit() {};

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
	}

	void update() override {
		auto& pos = tr_->getPos();

		if (pos.getX() > winWidth || pos.getX() + tr_->getW() < 0 ||
			pos.getY() > winHeight || pos.getY() + tr_->getH() < 0)
			entity_->setActive(false);
	}

private:
	Transform* tr_;
	int winWidth, winHeight;
};