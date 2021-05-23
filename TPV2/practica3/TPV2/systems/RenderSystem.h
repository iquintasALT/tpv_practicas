#pragma once
#include "../ecs/System.h"

struct Transform;
struct Image;

class RenderSystem: public System {
public:
	RenderSystem();
	virtual ~RenderSystem();
	void init() override;
	void update() override;
private:

	void drawFighter(Transform *tr, Image* img);
	void drawBullets();
	void drawScore();
	void drawMsgs();
	void drawNames();

	Transform *leftFighterTr_;
	Transform *rightFighterlTr_;
	Image *leftFighterImg_;
	Image *rightFighterlImg_;
};

