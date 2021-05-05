#pragma once

#include "../ecs/Component.h"

#include "../sdlutils/Texture.h"

struct Health : public Component {
	Texture* texture_;

	const int maxLifes_ = 3, heartSize = 30;
	int lifes_;
	
	Health(Texture* txt) : lifes_(maxLifes_), texture_(txt) {};
	virtual ~Health() {};
};