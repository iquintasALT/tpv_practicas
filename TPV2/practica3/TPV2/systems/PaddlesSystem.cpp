#include "PaddlesSystem.h"

#include "../ecs/Manager.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

#include "../components/PaddleCtrlKeys.h"
#include "../components/Transform.h"
#include "../components/Image.h"

#include "NetworkSystem.h"

PaddlesSystem::PaddlesSystem() :
		leftFighter_(nullptr), //
		rightFighter_(nullptr) {
}

PaddlesSystem::~PaddlesSystem() {
}

void PaddlesSystem::init() {
	leftFighter_ = manager_->addEntity();
	manager_->addComponent<Transform>(leftFighter_, //
			Vector2D(10.0, sdlutils().height() / 2.0f - 25.0f), //
			Vector2D(), //
			50.0f, 50.0f, 0.0f);
	manager_->addComponent<Image>(leftFighter_, //
			&sdlutils().images().at("fighter1"));
	manager_->addComponent<PaddleCtrlKeys>(leftFighter_, //
			SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_SPACE, 5.0f);
	manager_->setHandler<LeftFighter>(leftFighter_);

	rightFighter_ = manager_->addEntity();
	manager_->addComponent<Transform>(
			rightFighter_, //
			Vector2D(sdlutils().width() - 50.0f - 10.0f,
					sdlutils().height() / 2.0f - 25.0f), //
			Vector2D(), //
			50.0f, 50.0f, 0.0f);
	manager_->addComponent<Image>(rightFighter_, //
		&sdlutils().images().at("fighter2"));
	manager_->addComponent<PaddleCtrlKeys>(rightFighter_, //
			SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_SPACE, 5.0f);
	manager_->setHandler<RightFighter>(rightFighter_);
}

void PaddlesSystem::update() {

	// move you paddle only
	Uint8 myId = manager_->getSystem<NetworkSystem>()->getId();

	if (myId == 0)
		movePaddle(leftFighter_);
	else
		movePaddle(rightFighter_);
}

void PaddlesSystem::movePaddle(Entity *e) {

	auto tr_ = manager_->getComponent<Transform>(e);
	auto keys = manager_->getComponent<PaddleCtrlKeys>(e);

	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_UP)) {
			tr_->vel_.setY(-keys->speed_);
		} else if (ih().isKeyDown(SDL_SCANCODE_DOWN)) {
			tr_->vel_.setY(keys->speed_);
		} else if (ih().isKeyDown(SDL_SCANCODE_STOP)) {
			tr_->vel_.setY(0.0f);
		}
	}

	tr_->pos_ = tr_->pos_ + tr_->vel_;

	if (tr_->pos_.getY() < 0) {
		tr_->pos_.setY(0);
		tr_->vel_.setY(0);
	} else if (tr_->pos_.getY() + tr_->height_ > sdlutils().height()) {
		tr_->pos_.setY(sdlutils().height() - tr_->height_);
		tr_->vel_.setY(0);
	}

	manager_->getSystem<NetworkSystem>()->sendPaddlePosition(tr_->pos_);

}

void PaddlesSystem::setPaddlePosition(Uint8 id, Vector2D pos) {
	Entity *e = nullptr;
	if (id == 0) {
		e = leftFighter_;
	} else {
		e = rightFighter_;
	}
	auto tr_ = manager_->getComponent<Transform>(e);
	tr_->pos_ = pos;
}
