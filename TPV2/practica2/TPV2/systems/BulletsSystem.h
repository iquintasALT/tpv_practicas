#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"

class BulletsSystem : public System {
public:
	void update() override;

	void render() override;

	void receive(const Message& msg) override;

private:
	const float bulletH = 20.0f, bulletW = 5.0f;
	bool isRunning = false;
	
	void shoot(Vector2D pos, Vector2D vel, double rot, double width, double height);
	void onCollisionWithAsteroid(Entity* b, Entity* a);
};