#pragma once

#include "../ecs/System.h"

class RenderSystem : public System {
public:
	// - dibujar asteroides, balas y caza (sólo si el juego no está parado).
	// - dibujar el marcador y las vidas (siempre).
	// - dibujar el mensaje correspondiente si el juego está parado (como en la
	// práctica 1)
	// - Otra posibilidad es definir un método render en la clase System, y distribuir
	// este código en los sistemas correspondientes en lugar de tener un sólo
	// sistema para rendering
	void update() override {
		// MARCADOR?

		// VIDAS

		// ASTEROIDES

		// BALAS

		// CAZA

		// MENSAJES

	}

	void render() override {
		//renderizamos la imagen acorde a su posicion y dimensiones
		SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH());
		tex_->render(dest, tr_->getRot());
	}
};