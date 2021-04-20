#pragma once

#include "../ecs/Component.h"
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/Texture.h"


class Heart : public Component {
	const int maxLifes_ = 3, heartSize = 30;
	int lifes_;
	
	Heart() : lifes_(maxLifes_) {};
	virtual ~Heart() {};
};