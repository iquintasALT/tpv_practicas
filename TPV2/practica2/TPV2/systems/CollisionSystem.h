#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../utils/Collisions.h"

class CollisionSystem : public System {
public:
	void update() override;
};