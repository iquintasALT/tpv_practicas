#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../utils/Collisions.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/SoundEffect.h"

#include "Transform.h"
#include "State.h"
#include "Heart.h"

class CollisionsManager : public Component {
public:
	CollisionsManager(SoundEffect* crash, SoundEffect* destroy) : 
		state_(nullptr), asteroidsManager_(nullptr), crash_sfx_(crash), destroy_sfx_(destroy) {}

	virtual ~CollisionsManager() {}

	void init() override {
		state_ = entity_->getComponent<State>();
		asteroidsManager_ = entity_->getComponent<AsteroidsManager>();

		assert(state_ != nullptr && asteroidsManager_ != nullptr);
	}

	void update() override {
		auto entities = entity_->getMngr()->getEntities();

		for (int i = 0; i < entities.size(); i++) {
			if (entities[i]->hasGroup<Asteroid_grp>()) {
				auto asteroid = entities[i];
				auto asteroidTr_ = entities[i]->getComponent<Transform>();

				auto fighter = entity_->getMngr()->getHandler<Player_hdlr>();
				auto fighterTr_ = fighter->getComponent<Transform>();

				auto& pos = asteroidTr_->getPos();
				auto w = asteroidTr_->getW();
				auto h = asteroidTr_->getH();

				if (Collisions::collides(pos, w, h, fighterTr_->getPos(), fighterTr_->getW(), fighterTr_->getH())) {
					auto health = fighter->getComponent<Heart>();
					health->eraseLife();

					if (health->getLifes() > 0)
						state_->setState(states::PAUSED);
					else
						state_->setState(states::GAMEOVER);

					for (auto entity : entities)
						entity->setActive(false);

					//reactivamos caza y manager tras desactivar todos los items de juego para que se destruyan en refresh
					fighter->setActive(true);
					entity_->getMngr()->getHandler<Manager_hdlr>()->setActive(true);

					fighterTr_->getPos().set(sdlutils().width() / 2.0f - (fighterTr_->getW() / 2),
						sdlutils().height() / 2.0f - (fighterTr_->getH() / 2));
					fighterTr_->getVel().set(Vector2D(0, 0));
					fighterTr_->setRot(0);

					crash_sfx_->play();

					break;
				}
				else {
					for (int i = 0; i < entities.size(); i++) {
						if (entities[i]->hasGroup<Bullet_grp>() && entities[i]->isActive()) {
							auto bulletTr_ = entities[i]->getComponent<Transform>();
							if (Collisions::collides(pos, w, h, bulletTr_->getPos(), bulletTr_->getW(), bulletTr_->getH())) {
								asteroidsManager_->onCollision(asteroid);
								entities[i]->setActive(false);
								destroy_sfx_->play();
							}
						}
					}
				}
			}
		}
	}

private:
	State* state_;
	AsteroidsManager* asteroidsManager_;
	SoundEffect* crash_sfx_, *destroy_sfx_;
};
