#pragma once

#include "../ecs/Component.h"

class Generations : public Component {
	int gen_;

	Generations() : gen_(0) {}
	Generations(int gen) : gen_(gen) {}
	virtual ~Generations() {}
};