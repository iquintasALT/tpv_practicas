#pragma once

#include "../ecs/Component.h"

#include "../sdlutils/Texture.h"
#include "../utils/Vector2D.h"

struct Health : public Component {
	const int maxLifes_ = 3, heartSize = 30;
	int lifes_;
	Texture* texture_;
	
	Health(Texture* txt) : lifes_(maxLifes_), texture_(txt) {};
	virtual ~Health() {};
};