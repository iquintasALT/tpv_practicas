#pragma once

#include "../ecs/System.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../systems/GameCtrlSystem.h"

class RenderSystem : public System {
public:
	// - dibujar asteroides, balas y caza (sólo si el juego no está parado).
	// - dibujar el marcador y las vidas (siempre).
	// - dibujar el mensaje correspondiente si el juego está parado (como en la
	// práctica 1)
	// - Otra posibilidad es definir un método render en la clase System, y distribuir
	// este código en los sistemas correspondientes en lugar de tener un sólo
	// sistema para rendering
	void update() override;
	//void render() override {
	//	//renderizamos la imagen acorde a su posicion y dimensiones
	//	SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH());
	//	tex_->render(dest, tr_->getRot());
	//}
	void render();
	//// cada vez que cambia de frame
	//if (sdlutils().currRealTime() - lastUpdate > ms) {
	//	if (cols_ == actCol_ + 1) { // si ha llegado al final de la fila (nCols)
	//		// actualizamos la fila en caso de que sea el final o no
	//		actRow_ = (rows_ == actRow_ + 1 ? 0 : actRow_ + 1);
	//		actCol_ = 0; // reseteamos columna
	//	}
	//	else actCol_++;

	//	// actualizamos timer
	//	lastUpdate = sdlutils().currRealTime();
	//}

	//// calculamos el frame a renderizar
	//src_.x = src_.w * actCol_;
	//src_.y = src_.h * actRow_;

	//SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH());
	//tex_->render(src_, dest, tr_->getRot());
};