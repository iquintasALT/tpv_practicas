#include "BulletsSystem.h"

#include "../components/Transform.h"
#include "../components/Image.h"

//metodo para disparar una bala
void BulletsSystem::shoot(int id) {
	// se escoge el caza segun la id y se accede a su transform
	Transform* pl_tr = (id == 0 ? manager_->getComponent<Transform>(manager_->getHandler<LeftFighter>()) : 
		manager_->getComponent<Transform>(manager_->getHandler<RightFighter>()));

	auto bullet = manager_->addEntity();

	// creamos la bala con la rotacion del caza en su posicion correspondiente
	Vector2D bPos = (pl_tr->pos_ + Vector2D(pl_tr->width_ / 2, pl_tr->height_ / 2)) -
		Vector2D(0.0f, bulletH / 2.0f + 5.0f + 12.0f).rotate(pl_tr->rotation_)
		- Vector2D(2.0f, 10.0f);

	// aplicamos la velocidad con la misma rotacion del caza
	Vector2D bVel = Vector2D(0.0f, -1.0f).rotate(pl_tr->rotation_) * (pl_tr->vel_.magnitude() + 5.0f);

	// creamos la bala
	manager_->addComponent<Transform>(bullet, bPos, bVel, bulletW, bulletH, pl_tr->rotation_);
	
	manager_->addComponent<Image>(bullet, (id == 0 ? &sdlutils().images().at("bullet") : &sdlutils().images().at("bullet2")));

	// se establece un grupo dependiendo del id
	if (id == 0) manager_->setGroup<LeftBullet>(bullet, true);
	else manager_->setGroup<RightBullet>(bullet, true);

	sdlutils().soundEffects().at("fire").play(); // sonido
}

// metodo para desactivar las balas al terminar una ronda
void BulletsSystem::resetBullets()
{
	for (auto bullet : manager_->getEntities())
		if (manager_->hasGroup<LeftBullet>(bullet) || manager_->hasGroup<RightBullet>(bullet))
			manager_->setActive(bullet, false);
}

void BulletsSystem::update() {
	auto entities = manager_->getEntities();
	for (int i = 0; i < entities.size(); ++i) {
		if (manager_->hasGroup<LeftBullet>(entities[i]) || manager_->hasGroup<RightBullet>(entities[i])) {
			Transform* bullet_tr_ = manager_->getComponent<Transform>(entities[i]);
			auto& pos = bullet_tr_->pos_;

			bullet_tr_->pos_ = pos + bullet_tr_->vel_;

			// si se sale del marco de juego por alguno de sus limites, desactivamos el objeto
			if (pos.getX() > sdlutils().width() || pos.getX() + bullet_tr_->width_ < 0 ||
				pos.getY() > sdlutils().height() || pos.getY() + bullet_tr_->height_ < 0)
				manager_->setActive(entities[i], false);
		}
	}
}