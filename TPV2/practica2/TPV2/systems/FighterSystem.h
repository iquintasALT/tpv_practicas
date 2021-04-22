#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SoundEffect.h"
#include "../utils/Vector2D.h"

#include "../components/Transform.h"

#include "GameCtrlSystem.h"
#include "BulletsSystem.h"

class FighterSystem : public System {
public:
	// - poner el caza en el centro con velocidad 0 y rotación 0. No hace falta
	// desactivar la entidad (no dibujarla si el juego está parado en RenderSystem).
	// - avisar al GameCtrlSystem que se ha muerto el caza (se puede también avisar
	// directamente en el CollisionSystem)
	void onCollisionWithAsteroid(Entity* a) {
		// recolocamos el caza y reseteamos su velocidad y rotacion
		player_tr_->pos_.set(sdlutils().width() / 2.0f - (player_tr_->width_ / 2),
			sdlutils().height() / 2.0f - (player_tr_->height_ / 2));
		player_tr_->vel_.set(Vector2D(0, 0));
		player_tr_->rotation_ = 0;

		// sonido de choque
		crash_sfx_->play();

		manager_->getSystem<GameCtrlSystem>()->onFighterDeath();
	}

	// - crear la entidad del caza, añadir sus componentes (Transform, Health, etc.)
	// y asociarla con el handler correspondiente.
	void init() override {
		auto player = manager_->addEntity();
		player_tr_ = manager_->addComponent<Transform>(player, Vector2D(sdlutils().width() / 2.0f - 25.0f,
			sdlutils().height() / 2.0f - 25.0f), Vector2D(), 50.0f, 50.0f);
		manager_->addComponent<Health>(player, &sdlutils().images().at("heart"));
		manager_->setHandler<Player_hdlr>(player);

		thrust_sfx_ = &sdlutils().soundEffects().at("thrust");
		crash_sfx_ = &sdlutils().soundEffects().at("explosion");
	}

	// - si el juego está parado no hacer nada.
	// - actualizar la velocidad del caza y moverlo como en la práctica 1.
	void update() override {
		if (manager_->getSystem<GameCtrlSystem>()->getGameState() == GameState::RUNNING) {
			if (ih().keyDownEvent()) {
				if (ih().isKeyDown(SDLK_s) && sdlutils().currRealTime() - msToNextBullet > nextBullet) { // shoot
					manager_->getSystem<BulletsSystem>()->shoot(player_tr_->pos_, player_tr_->vel_,
						player_tr_->width_, player_tr_->height_);
					msToNextBullet = sdlutils().currRealTime();
				}

				if (ih().isKeyDown(SDL_SCANCODE_UP)) { // impulso
					auto& vel = player_tr_->vel_;
					auto newVel = vel + (Vector2D(0, -1).rotate(player_tr_->rotation_)).normalize() * thrust;
					// si se pasa del limite de veocidad, establecemos este como nueva velocidad
					vel.set((newVel.magnitude() > speedLimit) ?
						(Vector2D(0, -1).rotate(player_tr_->rotation_)).normalize() * speedLimit : newVel);

					thrust_sfx_->play(); // sonido de impulso
				}
				else if (ih().isKeyDown(SDL_SCANCODE_LEFT)) { // rotacion izquierda
					player_tr_->rotation_ = player_tr_->rotation_ - 5.0f;
				}
				else if (ih().isKeyDown(SDL_SCANCODE_RIGHT)) { // rotacion derecha
					player_tr_->rotation_ = player_tr_->rotation_ + 5.0f;
				}
			}

			player_tr_->pos_ = player_tr_->pos_ + player_tr_->vel_;
			player_tr_->vel_ = player_tr_->vel_ * deAcceleration;

			//toroidal en el eje X (fluido)
			if (player_tr_->pos_.getX() > sdlutils().width())
				player_tr_->pos_.setX(0 - player_tr_->width_);
			else if (player_tr_->pos_.getX() + player_tr_->width_ < 0)
				player_tr_->pos_.setX(sdlutils().width());

			//toroidal en el eje Y (fluido)
			if (player_tr_->pos_.getY() > sdlutils().height())
				player_tr_->pos_.setY(0 - player_tr_->height_);
			else if (player_tr_->pos_.getY() + player_tr_->height_ < 0)
				player_tr_->pos_.setY(sdlutils().height());
		}
	}

private:
	const float thrust = 0.2f, speedLimit = 3.0f;
	const float deAcceleration = 0.995f;

	const int nextBullet = 250; // in ms
	int msToNextBullet;

	Transform* player_tr_ = nullptr;
	SoundEffect* thrust_sfx_ = nullptr, * crash_sfx_= nullptr;
};