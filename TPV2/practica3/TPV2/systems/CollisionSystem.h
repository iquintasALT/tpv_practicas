#pragma once
#include "../ecs/System.h"

struct Transform;
class SoundEffect;

class CollisionSystem: public System {
public:
	CollisionSystem();
	virtual ~CollisionSystem();
	void init() override;
	void update() override;
private:
	//Transform *ballTr_;
	Transform *leftPaddelTr_;
	Transform *rightPaddelTr_;
	SoundEffect *paddleHit_;
};

