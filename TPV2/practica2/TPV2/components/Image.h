#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"

struct Image: public Component {
	Texture *tex_;

	Image(Texture* tex) : tex_(tex) {}
	virtual ~Image() {}
};