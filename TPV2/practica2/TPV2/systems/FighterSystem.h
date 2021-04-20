#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"

class FighterSystem : public System {
public:
	// - poner el caza en el centro con velocidad 0 y rotación 0. No hace falta
	// desactivar la entidad (no dibujarla si el juego está parado en RenderSystem).
	// - avisar al GameCtrlSystem que se ha muerto el caza (se puede también avisar
	// directamente en el CollisionSystem)
	void onCollisionWithAsteroid(Entity* a);

	// - crear la entidad del caza, añadir sus componentes (Transform, Health, etc.)
	// y asociarla con el handler correspondiente.
	void init() override {
		auto player = manager_->addEntity();
		manager_->addComponent<Transform>(player, Vector2D());
		manager_->addComponent<Heart>(player, Vector2D(sdlutils().width() / 2.0f - 25.0f, sdlutils().height() / 2.0f - 25.0f),
			Vector2D(), 50.0f, 50.0f);
		manager_->setHandler<Player_hdlr>(player);
	}

	// - si el juego está parado no hacer nada.
	// - actualizar la velocidad del caza y moverlo como en la práctica 1.
	void update() override {

	}

private:
	const float deAcceleration = 0.995f;
};