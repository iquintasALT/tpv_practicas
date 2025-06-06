#include "FighterSystem.h"

#include "../ecs/Manager.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

#include "../components/Transform.h"
#include "../components/Image.h"
#include "../components/FighterCtrlKeys.h"

#include "NetworkSystem.h"
#include "GameManagerSystem.h"
#include "BulletsSystem.h"

FighterSystem::FighterSystem() : leftFighter_(nullptr), rightFighter_(nullptr) {}

FighterSystem::~FighterSystem() {}

void FighterSystem::init() {
	//caza izquierdo
	leftFighter_ = manager_->addEntity();
	manager_->addComponent<Transform>(leftFighter_, Vector2D(10.0, sdlutils().height() / 2.0f - 25.0f), Vector2D(),
		50.0f, 50.0f, 90.0f);
	manager_->addComponent<Image>(leftFighter_, &sdlutils().images().at("fighter1"));
	manager_->addComponent<FighterCtrlKeys>(leftFighter_, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_SPACE, 5.0f);
	manager_->setHandler<LeftFighter>(leftFighter_);

	//caza derecho
	rightFighter_ = manager_->addEntity();
	manager_->addComponent<Transform>(rightFighter_, Vector2D(sdlutils().width() - 50.0f - 10.0f, sdlutils().height() / 2.0f - 25.0f),
		Vector2D(), 50.0f, 50.0f, 270.0f);
	manager_->addComponent<Image>(rightFighter_, &sdlutils().images().at("fighter2"));
	manager_->addComponent<FighterCtrlKeys>(rightFighter_, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_SPACE, 5.0f);
	manager_->setHandler<RightFighter>(rightFighter_);
}

void FighterSystem::update() {
	// elegimos el caza dependiendo del id de cada usuario
	Uint8 myId = manager_->getSystem<NetworkSystem>()->getId();
	moveFighter((myId == 0 ? leftFighter_ : rightFighter_));
}

void FighterSystem::moveFighter(Entity* e) {
	auto player_tr_ = manager_->getComponent<Transform>(e);
	auto keys = manager_->getComponent<FighterCtrlKeys>(e);

	if (manager_->getSystem<GameManagerSystem>()->getState() == GameManagerSystem::RUNNING) {
		if (ih().keyDownEvent()) {
			if (ih().isKeyDown(SDL_SCANCODE_UP)) { // impulso
				auto& vel = player_tr_->vel_;
				auto newVel = vel + (Vector2D(0, -1).rotate(player_tr_->rotation_)).normalize() * thrust;
				// si se pasa del limite de veocidad, establecemos este como nueva velocidad
				vel.set((newVel.magnitude() > speedLimit) ?
					(Vector2D(0, -1).rotate(player_tr_->rotation_)).normalize() * speedLimit : newVel);

				sdlutils().soundEffects().at("thrust").play(); // sonido de impulso
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

		fighterToroidalMove(e);
		// se manda un mensaje para actualizar la posicion del jugador en la otra pantalla
		manager_->getSystem<NetworkSystem>()->sendFighterMovement(player_tr_->pos_, player_tr_->rotation_);

		// fighter gun
		// este mensaje se envia despues del mensaje de actualizar el caza porque como hacemos el metodo cogiendo el transform
		// del caza utilizando su id, si en la otra pantalla no se ha actualizado aun, la bala sale en una posicion incorrecta
		if (ih().keyDownEvent()) {
			if (ih().isKeyDown(SDLK_s) && sdlutils().currRealTime() - msToNextBullet > nextBullet) { // shoot
				manager_->getSystem<BulletsSystem>()->shoot(manager_->getSystem<NetworkSystem>()->getId());
				manager_->getSystem<NetworkSystem>()->sendShoot();
				msToNextBullet = sdlutils().currRealTime();
			}
		}
	}

}

void FighterSystem::fighterToroidalMove(Entity* e)
{
	auto player_tr_ = manager_->getComponent<Transform>(e);
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

// se actualiza la posicion del caza cuando se mueve (llamado tras recibir un mensaje del otro usuario)
void FighterSystem::setPositionFighter(Uint8 id, Vector2D pos, float rot) {
	Entity* e = (id == 0) ? leftFighter_ : rightFighter_;

	auto tr_ = manager_->getComponent<Transform>(e);
	tr_->pos_ = pos;
	tr_->rotation_ = rot;
	tr_->vel_ = Vector2D();
}

// se resetea el caza tras colision de bala
void FighterSystem::resetFighterPosition() {
	Uint8 id = manager_->getSystem<NetworkSystem>()->getId();
	Entity* e = (id == 0) ? leftFighter_ : rightFighter_;

	auto tr_ = manager_->getComponent<Transform>(e);
	tr_->pos_ = (id == 0) ? Vector2D(10.0, sdlutils().height() / 2.0f - 25.0f) :
		Vector2D(sdlutils().width() - 50.0f - 10.0f, sdlutils().height() / 2.0f - 25.0f);
	tr_->rotation_ = (id == 0) ? 90.0f : 270.0f;
	tr_->vel_ = Vector2D();

	// se envia un mensaje para resetear la posicion en la otra ventana
	manager_->getSystem<NetworkSystem>()->sendFighterMovement(tr_->pos_, tr_->rotation_);
}