#include "CollisionSystem.h"

void CollisionSystem::update() {
	auto entities = manager_->getEntities();

	for (int i = 0; i < entities.size(); i++) {
		if (manager_->hasGroup<Asteroid_grp>(entities[i])) { // para cada asteroide
			auto asteroid = entities[i];
			auto asteroidTr_ = manager_->getComponent<Transform>(entities[i]);

			auto fighter = manager_->getHandler<Player_hdlr>();
			auto fighterTr_ = manager_->getComponent<Transform>(fighter);

			// comprobamos si colisiona con el caza
			if (Collisions::collides(asteroidTr_->pos_, asteroidTr_->width_, asteroidTr_->height_,
				fighterTr_->pos_, fighterTr_->width_, fighterTr_->height_)) {

				Message msg = Message(MsgId::LOSE_LIFE);
				manager_->send(msg);

				break; //salimos del bucle principal
			}
			else { // en caso de no colisionar con el caza
				for (int i = 0; i < entities.size(); i++) {
					// comprobamos si colisiona con alguna bala
					if (manager_->hasGroup<Bullet_grp>(entities[i]) && manager_->isActive(entities[i])) {
						auto bulletTr_ = manager_->getComponent<Transform>(entities[i]);
						// si colisiona
						if (Collisions::collides(asteroidTr_->pos_, asteroidTr_->width_, asteroidTr_->height_,
							bulletTr_->pos_, bulletTr_->width_, bulletTr_->height_)) {
							// aplicamos el comportamiento de colision correspondiente
							Message msg = Message(MsgId::BULLET_COLLIDES);
							msg.cData.bullet = entities[i]; msg.cData.asteroid = asteroid;
							manager_->send(msg);
						}
					}
				}
			}
		}
	}
}