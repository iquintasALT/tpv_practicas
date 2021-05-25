#pragma once
#include <SDL_stdinc.h>

#include "../ecs/System.h"
#include "../utils/Vector2D.h"

struct Entity;

class FighterSystem: public System {
public:
	FighterSystem();
	virtual ~FighterSystem();
	void init() override;
	void update() override;

	// used to set the paddle position of the other layer
	void setPositionFighter(Uint8 id, Vector2D pos, float rot);
	void resetFighterPosition();

private:
	void moveFighter(Entity *e);
	void fighterToroidalMove(Entity* e);

	Entity *leftFighter_;
	Entity *rightFighter_;
	const float thrust = 0.2f, speedLimit = 3.0f;
	const float deAcceleration = 0.995f;

	const int nextBullet = 250; // in ms
	int msToNextBullet = 0;
};

