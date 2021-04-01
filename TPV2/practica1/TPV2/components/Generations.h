// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../sdlutils/Texture.h"

#include "Transform.h"

class Generations : public Component {
public:
	Generations(int lives) : lives_(lives) {}

	virtual ~Generations() {}

	inline int& getLives() { return lives_; }

private:
	int lives_;
};

