#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"

#include "../components/Transform.h"

class BulletsSystem : public System {
public:
	// - añadir una bala al juego, como en la práctica 1. La rotación de la bala
	// sería vel.angle(Vector2D(0.0f,-1.0f))
	void shoot(Vector2D pos, Vector2D vel, double width, double height) {
		auto bullet = manager_->addEntity();

		// creamos la bala con la rotacion del caza en su posicion correspondiente
		Vector2D bPos = (pos + Vector2D(width / 2, height / 2)) -
			Vector2D(0.0f, bulletH / 2.0f + 5.0f + 12.0f).rotate(vel.angle(Vector2D(0.0f, -1.0f)))
			- Vector2D(2.0f, 10.0f);
		// aplicamos la velocidad con la misma rotacion del caza
		Vector2D bVel = Vector2D(0.0f, -1.0f).rotate(vel.angle(Vector2D(0.0f, -1.0f)) * (vel.magnitude() + 5.0f));

		manager_->addComponent<Transform>(bullet, bPos, bVel, bulletW, bulletH, vel.angle(Vector2D(0.0f, -1.0f)));
		manager_->addComponent<Image>(bullet, &sdlutils().images().at("fire"));
		manager_->setGroup<Bullet_grp>(bullet, true);
	}

	// - desactivar la bala “b”
	// - pasamos una referencia al asteroid aunque no se usa de momento (en el futuro
	// se puede usar para tener comportamientos distintos depende del tipo de
	// asteroid, etc).
	void onCollisionWithAsteroid(Entity* b, Entity* a) {
		manager_->setActive(b, false); // desactivamos la bala
		sdlutils().soundEffects().at("bang").play(); // sonido
	}

	// - si el juego está parado no hacer nada.
	// - mover las balas y desactivar las que se salen de la ventana
	void update() override {
		auto entities = manager_->getEntities();
		for (int i = 0; i < entities.size(); ++i) {
			if (manager_->hasGroup<Bullet_grp>(entities[i])) {
				Transform* bullet_tr_ = manager_->getComponent<Transform>(entities[i]);
				auto& pos = bullet_tr_->pos_;

				pos.set(pos + bullet_tr_->vel_);

				// si se sale del marco de juego por alguno de sus limites, desactivamos el objeto
				if (pos.getX() > sdlutils().width() || pos.getX() + bullet_tr_->width_ < 0 ||
					pos.getY() > sdlutils().height() || pos.getY() + bullet_tr_->height_ < 0)
					manager_->setActive(entities[i], false);
			}
		}
	}

private:
	const float bulletH = 20.0f, bulletW = 5.0f;
};