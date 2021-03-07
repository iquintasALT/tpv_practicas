#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"

#include "Transform.h"

class ShowAtOppositeSide : public Component
{
public:
	ShowAtOppositeSide(int const width, int const height) : tr_(nullptr), winHeight(height), winWidth(width) {};

	virtual ~ShowAtOppositeSide() {};

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
	}

	void update() override {
		auto& pos = tr_->getPos();
		//toroidal en el eje X
		if (pos.getX() > winWidth)
			pos.setX(0 - tr_->getW());
		else if(pos.getX() + tr_->getW() < 0)
			pos.setX(winWidth);
		//toroidal en el eje Y
		if (pos.getY() > winHeight)
			pos.setY(0 - tr_->getH());
		else if (pos.getY() + tr_->getH() < 0)
			pos.setY(winHeight);
	}

private:
	Transform* tr_;
	int winWidth, winHeight;
};

