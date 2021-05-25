#pragma once
#include "../ecs/System.h"

struct Transform;

class CollisionSystem: public System {
public:
	CollisionSystem();
	virtual ~CollisionSystem();
	void init() override;
	void update() override;
private:
	Transform *leftFighterTr_;
	Transform *rightFighterTr_;
};

