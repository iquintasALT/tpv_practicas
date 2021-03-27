// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"

#include "Transform.h"

class Generations : public Component {
public:
	Generations() :
		lives_(3)
	{
	}

	Generations(int lives) :
		lives_(lives)
	{
	}

	int& getLives() {
		return lives_;
	}

	virtual ~Generations() {
	}

private:
	int lives_;
};

