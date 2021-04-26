// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

struct Follow : public Component {
	bool followsPlayer;

	Follow(bool followsPlayer_) : followsPlayer(followsPlayer_) {};
	virtual ~Follow() {}
};