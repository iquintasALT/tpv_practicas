#include "BulletsSystem.h"

void BulletsSystem::shoot(Vector2D pos, Vector2D vel, double rot, double width, double height) {
	auto bullet = manager_->addEntity();

	// creamos la bala con la rotacion del caza en su posicion correspondiente
	Vector2D bPos = (pos + Vector2D(width / 2, height / 2)) -
		Vector2D(0.0f, bulletH / 2.0f + 5.0f + 12.0f).rotate(rot)
		- Vector2D(2.0f, 10.0f);
	// aplicamos la velocidad con la misma rotacion del caza
	Vector2D bVel = Vector2D(0.0f, -1.0f).rotate(rot) * (vel.magnitude() + 5.0f);

	manager_->addComponent<Transform>(bullet, bPos, bVel, bulletW, bulletH, rot);
	manager_->addComponent<Image>(bullet, &sdlutils().images().at("fire"));
	manager_->setGroup<Bullet_grp>(bullet, true);

	sdlutils().soundEffects().at("fire").play(); // sonido
}

void BulletsSystem::onCollisionWithAsteroid(Entity* b, Entity* a) {
	manager_->setActive(b, false); // desactivamos la bala
	sdlutils().soundEffects().at("bang").play(); // sonido
}

void BulletsSystem::update() {
	auto entities = manager_->getEntities();
	for (int i = 0; i < entities.size(); ++i) {
		if (manager_->hasGroup<Bullet_grp>(entities[i])) {
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

void BulletsSystem::render()
{
	if (manager_->getSystem<GameCtrlSystem>()->getGameState() == GameState::RUNNING) {
		for (Entity* e : manager_->getEntities()) {
			if (manager_->hasGroup<Bullet_grp>(e)) {
				Image* image = manager_->getComponent<Image>(e);
				Transform* tr_ = manager_->getComponent<Transform>(e);

				SDL_Rect dest = build_sdlrect(tr_->pos_, tr_->width_, tr_->height_);
				image->tex_->render(dest, tr_->rotation_);
			}
		}
	}
}

void BulletsSystem::receive(const Message& msg)
{
	switch (msg.id_) {
	case MsgId::SHOOT_BULLET:
		shoot(msg.bData.pos, msg.bData.vel, msg.bData.rot, msg.bData.width, msg.bData.height);
		break;
	case MsgId::BULLET_COLLIDES:
		onCollisionWithAsteroid(msg.cData.bullet, msg.cData.asteroid);
		break;
	}
}