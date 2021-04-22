#pragma once

#include "../ecs/System.h"

class CollisionSystem : public System {
public:
	// - si el juego está parado no hacer nada.
	// - comprobar colisiones como en la práctica 1 y avisar a los sistemas
	// correspondientes en caso de colisiones
	void update() override {
		auto entities = entity_->getMngr()->getEntities();

		for (int i = 0; i < entities.size(); i++) {
			if (entities[i]->hasGroup<Asteroid_grp>()) { // para cada asteroide
				auto asteroid = entities[i];
				auto asteroidTr_ = entities[i]->getComponent<Transform>();

				auto fighter = entity_->getMngr()->getHandler<Player_hdlr>();
				auto fighterTr_ = fighter->getComponent<Transform>();

				auto& pos = asteroidTr_->getPos();
				auto w = asteroidTr_->getW();
				auto h = asteroidTr_->getH();

				// comprobamos si colisiona con el caza
				if (Collisions::collides(pos, w, h, fighterTr_->getPos(), fighterTr_->getW(), fighterTr_->getH())) {
					// en caso de que colisione, quitamos una vida
					auto health = fighter->getComponent<Health>();
					health->eraseLife();

					// reseteamos el numero de asteroides
					asteroidsManager_->resetNumAsteroids();

					// cambiamos el estado acorde al numero de vidas actual
					if (health->getLifes() > 0)
						state_->setState(GameState::PAUSED);
					else
						state_->setState(GameState::GAMEOVER);

					//desactivamos todas las entidades para su posterior eliminacion
					for (auto entity : entities)
						entity->setActive(false);

					//reactivamos caza y manager tras desactivar todas las entidades
					fighter->setActive(true);
					fighter->removeComponent<FighterCtrl>();
					fighter->removeComponent<Gun>();
					entity_->getMngr()->getHandler<Manager_hdlr>()->setActive(true);

					// recolocamos el caza y reseteamos su velocidad y rotacion
					fighterTr_->getPos().set(sdlutils().width() / 2.0f - (fighterTr_->getW() / 2),
						sdlutils().height() / 2.0f - (fighterTr_->getH() / 2));
					fighterTr_->getVel().set(Vector2D(0, 0));
					fighterTr_->setRot(0);

					// sonido de choque
					crash_sfx_->play();

					break; //salimos del bucle principal
				}
				else { // en caso de no colisionar con el caza
					for (int i = 0; i < entities.size(); i++) {
						// comprobamos si colisiona con alguna bala
						if (entities[i]->hasGroup<Bullet_grp>() && entities[i]->isActive()) {
							auto bulletTr_ = entities[i]->getComponent<Transform>();
							// si colisiona
							if (Collisions::collides(pos, w, h, bulletTr_->getPos(), bulletTr_->getW(), bulletTr_->getH())) {
								// aplicamos el comportamiento de colision correspondiente
								asteroidsManager_->onCollision(asteroid);
								entities[i]->setActive(false); // desactivamos la bala
								destroy_sfx_->play(); // sonido de explosion de disparo
							}
						}
					}
				}
	}
};