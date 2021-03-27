#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../utils/Collisions.h"
#include "../sdlutils/SDLUtils.h"

#include "Transform.h"
#include "State.h"
#include "Heart.h"

class CollisionsManager : public Component {
public:
	CollisionsManager() : state_(nullptr) {}

	virtual ~CollisionsManager() {}

	void init() override {
		state_ = entity_->getComponent<State>();

		assert(state_ != nullptr);
	}

	void update() override {
		auto entities = entity_->getMngr()->getEntities();
		bool collidesWithPlayer = false;

		for (int i = 0; i < entities.size(); i++) {
			if (entities[i]->hasGroup<Asteroid_grp>()) {
				auto fighter = entity_->getMngr()->getHandler<Player_hdlr>();
				auto fighterTr_ = fighter->getComponent<Transform>();
				auto tr_ = entities[i]->getComponent<Transform>();

				auto& pos = tr_->getPos();
				auto w = tr_->getW();
				auto h = tr_->getH();

				if (Collisions::collides(pos, w, h, fighterTr_->getPos(), fighterTr_->getW(), fighterTr_->getH())) {
					auto health = fighter->getComponent<Heart>();
					health->eraseLife();

					state_->setState(health->getLifes() > 0 ? states::PAUSED : states::GAMEOVER);

					for (auto entity : entities)
						entity->setActive(false);

					fighter->setActive(true);
					fighterTr_->getPos().set(sdlutils().width() / 2.0f - (fighterTr_->getW() / 2),
						sdlutils().height() / 2.0f - (fighterTr_->getH() / 2));
					fighterTr_->getVel().set(Vector2D());
					fighterTr_->setRot(0);

					break;
				}
				else {
					for (int i = 0; i < entities.size(); i++) {
						if (entities[i]->hasGroup<Bullet_grp>() && entities[i]->isActive()) {
							auto bulletTr_ = entities[i]->getComponent<Transform>();
							if (Collisions::collides(pos, w, h, bulletTr_->getPos(), bulletTr_->getW(), bulletTr_->getH())) {
								//se destruye
								entities[i]->setActive(false);
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
};
