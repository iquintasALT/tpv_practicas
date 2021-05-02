#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"

struct Image: public Component {
	Texture *tex_;
	
	int rows_, cols_;
	int actRow_, actCol_;
	int lastUpdate;

	Image(Texture* tex, int fils = -1, int cols = -1) : tex_(tex), rows_(fils), cols_ (cols), 
		actRow_(0), actCol_(0), lastUpdate(0) {}
	virtual ~Image() {}
};