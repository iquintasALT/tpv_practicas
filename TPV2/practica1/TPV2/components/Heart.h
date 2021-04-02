#pragma once

#include "../ecs/Component.h"
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/Texture.h"


class Heart : public Component {
public:
	Heart(Texture* text) : text_(text), lifes_(maxLifes_) {};

	virtual ~Heart() {};

	inline void eraseLife() { lifes_--; }

	inline void resetLifes() { lifes_ = maxLifes_; }

	inline int getLifes() { return lifes_; }

	void render() override {
		Vector2D pos = Vector2D(0, 0);
		for (int i = 0; i < lifes_; i++) {
			pos.setX(35 * i);
			SDL_Rect dest = build_sdlrect(pos, 30, 30);
			text_->render(dest, 0);
		}
	}

private:
	Texture* text_;

	const int maxLifes_ = 3;
	int lifes_;
};