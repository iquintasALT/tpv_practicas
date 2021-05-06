#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SoundEffect.h"
#include "../utils/Vector2D.h"
#include "../components/Image.h"

#include "../components/Transform.h"

#include "GameCtrlSystem.h"
#include "BulletsSystem.h"

class FighterSystem : public System {
public:
	// - poner el caza en el centro con velocidad 0 y rotación 0. No hace falta
	// desactivar la entidad (no dibujarla si el juego está parado en RenderSystem).
	// - avisar al GameCtrlSystem que se ha muerto el caza (se puede también avisar
	// directamente en el CollisionSystem)
	void onCollisionWithAsteroid();

	// - crear la entidad del caza, añadir sus componentes (Transform, Health, etc.)
	// y asociarla con el handler correspondiente.
	void init() override;

	// - si el juego está parado no hacer nada.
	// - actualizar la velocidad del caza y moverlo como en la práctica 1.
	void update() override;
	void render() override;
	void receive(const Message& msg) override;

private:
	const float thrust = 0.2f, speedLimit = 3.0f;
	const float deAcceleration = 0.995f;

	const int nextBullet = 250; // in ms
	int msToNextBullet;
	Transform* player_tr_ = nullptr;
	SoundEffect* thrust_sfx_ = nullptr, * crash_sfx_= nullptr;
	Entity* fighter_ = nullptr;

	bool isRunning = false;
};