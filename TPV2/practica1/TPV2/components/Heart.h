#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../utils/Vector2D.h"


class Heart : public Component {
public:
	Heart(Texture* text, Vector2D pos) :
		text_(text),
		pos_(pos) {};

	virtual ~Heart() {};

	void eraseLife() {
		lifes_--;
	}

	void resetLifes() {
		lifes_ = maxLifes_;
	}

	int getLifes() {
		return lifes_;
	}

	void render() override {
		Vector2D posAux = pos_;
		for (int i = 0; i < lifes_; i++) {
			SDL_Rect dest = build_sdlrect(posAux, 30, 30);
			text_->render(dest, 0);
			posAux.setX(pos_.getX() + 35 * (i + 1));
		}
	}


private:
	const int maxLifes_ = 3;
	Texture* text_;
	Vector2D pos_;
	int lifes_ = 3;
};