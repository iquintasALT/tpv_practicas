// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../sdlutils/Texture.h"

#include "Transform.h"

class FramedImage : public Component {
public:
	FramedImage(Texture* tex) :
		tr_(nullptr), //
		tex_(tex) //
	{}

	virtual ~FramedImage() {}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
	}

	void render() override {
		SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH());
		tex_->render(dest, tr_->getRot());
	}

private:
	Transform* tr_;
	Texture* tex_;
};