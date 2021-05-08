#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SoundEffect.h"
#include "../utils/Vector2D.h"

class Transform;
/// <summary>
/// Sistema que gestiona el caza
/// </summary>
class FighterSystem : public System {
public:
	void init() override;

	void update() override;

	void render() override;

	void receive(const Message& msg) override;

private:

	const float thrust = 0.2f, speedLimit = 3.0f;
	const float deAcceleration = 0.995f;

	const int nextBullet = 250; // in ms
	int msToNextBullet;

	Transform* player_tr_ = nullptr;
	SoundEffect* thrust_sfx_ = nullptr, * crash_sfx_= nullptr;
	Entity* fighter_ = nullptr;

	bool isRunning = false;

	void onCollisionWithAsteroid();
	void fighterToroidalMove();
};