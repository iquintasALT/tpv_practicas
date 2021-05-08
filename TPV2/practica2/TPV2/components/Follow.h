#pragma once

#include "../ecs/Component.h"

struct Follow : public Component {
	bool followsPlayer;

	Follow(bool followsPlayer_) : followsPlayer(followsPlayer_) {};
	virtual ~Follow() {}
};