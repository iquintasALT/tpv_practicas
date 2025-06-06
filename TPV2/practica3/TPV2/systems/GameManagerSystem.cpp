#include "GameManagerSystem.h"

#include "../ecs/Manager.h"

#include "../sdlutils/InputHandler.h"

#include "../components/Transform.h"

#include "NetworkSystem.h"
#include "BulletsSystem.h"
#include "FighterSystem.h"

GameManagerSystem::GameManagerSystem() : score_(), state_(NEWGAME), maxScore_(3) {}

GameManagerSystem::~GameManagerSystem() {}

void GameManagerSystem::init() {}

// maquina de estados
void GameManagerSystem::update() {
	if (state_ != RUNNING) {
		if (ih().isKeyDown(SDL_SCANCODE_SPACE)) {
			switch (state_) {
			case NEWGAME:
				startGame();
				break;
			case PAUSED:
				startGame();
				break;
			case GAMEOVER:
				resetGame();
				manager_->getSystem<NetworkSystem>()->sendStateChanged(state_, score_[0], score_[1]);
				break;
			default:
				break;
			}
		}
		// cambio de lugar 
		else if (ih().isKeyDown(SDL_SCANCODE_P)) {
			manager_->getSystem<NetworkSystem>()->switchId();
		}
	}
}

void GameManagerSystem::startGame() {
	if (state_ == RUNNING || !manager_->getSystem<NetworkSystem>()->isGameReady())
		return;

	auto isMaster = manager_->getSystem<NetworkSystem>()->isMaster();

	if (isMaster) {
		state_ = RUNNING;
		manager_->getSystem<NetworkSystem>()->sendStateChanged(state_, score_[0], score_[1]);
	}
	else manager_->getSystem<NetworkSystem>()->sendStartGameRequest();
}

void GameManagerSystem::changeState(Uint8 state, Uint8 left_score, Uint8 right_score) {
	state_ = state;
	score_[0] = left_score;
	score_[1] = right_score;
}

void GameManagerSystem::resetGame() {
	state_ = NEWGAME;
	score_[0] = score_[1] = 0;
}

// metodo llamado por el master cuando se produce una colision
void GameManagerSystem::onCollision(Uint8 id) {
	score_[id]++;
	// se actualiza la puntuacion
	changeState((score_[LEFT_FIGHTER] > 2 || score_[RIGHT_FIGHTER] > 2) ? GAMEOVER : PAUSED,
		score_[LEFT_FIGHTER], score_[RIGHT_FIGHTER]);

	// mensaje para gestionar la colision en la otra ventana
	manager_->getSystem<NetworkSystem>()->sendCollision(state_, score_[LEFT_FIGHTER], score_[RIGHT_FIGHTER]);

	// se resetea posiciones y balas
	manager_->getSystem<BulletsSystem>()->resetBullets();
	manager_->getSystem<FighterSystem>()->resetFighterPosition();
	sdlutils().soundEffects().at("explosion").play(); //sonido
}