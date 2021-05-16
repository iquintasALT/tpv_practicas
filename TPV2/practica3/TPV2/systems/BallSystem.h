#pragma once
#include "../ecs/System.h"

struct Transform;
class SoundEffect;
class Vector2D;

class BallSystem: public System {
public:
	BallSystem();
	virtual ~BallSystem();
	void init() override;
	void update() override;

	void resetBall();
	void initBall();
	void setBallInfo(Vector2D pos, Vector2D vel);
private:
	Transform *ballTr_;
	SoundEffect *wallHit_;
};

