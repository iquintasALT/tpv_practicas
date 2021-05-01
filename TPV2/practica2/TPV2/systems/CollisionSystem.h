#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../utils/Collisions.h"

#include "../components/Transform.h"
#include "../components/Health.h"

#include "FighterSystem.h"
#include "BulletsSystem.h"
#include "AsteroidsSystem.h"

class CollisionSystem : public System {
public:
	// - si el juego está parado no hacer nada.
	// - comprobar colisiones como en la práctica 1 y avisar a los sistemas
	// correspondientes en caso de colisiones
	void update() override;
};