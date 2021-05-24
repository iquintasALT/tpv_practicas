#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"

/// <summary>
/// Sistema que controla las balas disparadas por el caza
/// </summary>
class BulletsSystem : public System {
public:
	void update() override;

	//metodo para disparar una bala
	void shoot(int id_);

private:
	const float bulletH = 20.0f, bulletW = 5.0f;
	bool isRunning = false;
	
	
	//metodo llamado cuando se produce una colision con un asteroide
	void onCollisionWithAsteroid(Entity* b, Entity* a);
};