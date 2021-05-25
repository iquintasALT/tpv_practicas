#include "CollisionSystem.h"

#include <cassert>

#include "../ecs/Manager.h"

#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"

#include "../components/Transform.h"

#include "GameManagerSystem.h"
#include "NetworkSystem.h"

CollisionSystem::CollisionSystem() :
	leftFighterTr_(nullptr), //
	rightFighterTr_(nullptr) {}

CollisionSystem::~CollisionSystem() {}

void CollisionSystem::init() {
	leftFighterTr_ = manager_->getComponent<Transform>(manager_->getHandler<LeftFighter>());
	assert(leftFighterTr_ != nullptr);

	rightFighterTr_ = manager_->getComponent<Transform>(manager_->getHandler<RightFighter>());
	assert(rightFighterTr_ != nullptr);
}

void CollisionSystem::update() {
	if (!manager_->getSystem<NetworkSystem>()->isMaster() ||
		manager_->getSystem<GameManagerSystem>()->getState() != GameManagerSystem::RUNNING)
		return;

	auto entities = manager_->getEntities();
	for (int i = 0; i < manager_->getEntities().size(); i++) {
		auto entity = entities[i];
		auto bullet_tr = manager_->getComponent<Transform>(entity);
		if (manager_->hasGroup<LeftBullet>(entity)) { // si el derecho ha sido disparado
			if (Collisions::collides(rightFighterTr_->pos_, rightFighterTr_->width_, rightFighterTr_->height_,
				bullet_tr->pos_, bullet_tr->width_, bullet_tr->height_)) {
				manager_->getSystem<GameManagerSystem>()->onCollision(GameManagerSystem::LEFT_FIGHTER); // left fighter adds score
			}
		}
		else if (manager_->hasGroup<RightBullet>(entity)) { // si el izquierdo ha sido disparado
			if (Collisions::collides(leftFighterTr_->pos_, leftFighterTr_->width_, leftFighterTr_->height_,
				bullet_tr->pos_, bullet_tr->width_, bullet_tr->height_)) {
				manager_->getSystem<GameManagerSystem>()->onCollision(GameManagerSystem::RIGHT_FIGHTER); // right fighter adds score
			}
		}
	}
}
