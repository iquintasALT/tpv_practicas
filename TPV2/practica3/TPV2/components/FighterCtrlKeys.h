#pragma once
#include <SDL_scancode.h>

#include "../ecs/Component.h"

struct FighterCtrlKeys : Component {

	FighterCtrlKeys(SDL_Scancode up, SDL_Scancode down, SDL_Scancode stop,
		float speed) : up_(up), down_(down), stop_(stop), speed_(speed) {}

	SDL_Scancode up_;
	SDL_Scancode down_;
	SDL_Scancode stop_;
	float speed_;
};

