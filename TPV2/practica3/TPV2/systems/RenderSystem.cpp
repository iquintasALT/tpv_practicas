#include "RenderSystem.h"

#include "../ecs/Manager.h"

#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"

#include "../components/Transform.h"
#include "../components/Image.h"

#include "GameManagerSystem.h"
#include "NetworkSystem.h"

RenderSystem::RenderSystem() :
	leftFighterTr_(nullptr),
	leftFighterImg_(nullptr),
	rightFighterTr_(nullptr),
	rightFighterImg_(nullptr) {}

RenderSystem::~RenderSystem() {}

void RenderSystem::init() {
	leftFighterTr_ = manager_->getComponent<Transform>(manager_->getHandler<LeftFighter>());
	assert(leftFighterTr_ != nullptr);

	leftFighterImg_ = manager_->getComponent<Image>(manager_->getHandler<LeftFighter>());
	assert(leftFighterImg_ != nullptr);

	rightFighterTr_ = manager_->getComponent<Transform>(manager_->getHandler<RightFighter>());
	assert(rightFighterTr_ != nullptr);

	rightFighterImg_ = manager_->getComponent<Image>(manager_->getHandler<RightFighter>());
	assert(rightFighterImg_ != nullptr);
}

void RenderSystem::update() {
	drawFighter(leftFighterTr_, leftFighterImg_);
	drawFighter(rightFighterTr_, rightFighterImg_);

	drawScore(); // puntuacion
	drawHand(); // icono de que caza controla el usuario
	drawBullets(); // balas
	drawMsgs(); // mensajes de cada estado
	drawNames(); // nombre de cada usuario
}

void RenderSystem::drawFighter(Transform* tr, Image* img) {
	SDL_Rect dest = build_sdlrect(tr->pos_, tr->width_, tr->height_);
	img->tex_->render(dest, tr->rotation_);
}

void RenderSystem::drawBullets() {
	auto state_ = manager_->getSystem<GameManagerSystem>()->getState();
	if (state_ == GameManagerSystem::RUNNING) {
		for (Entity* e : manager_->getEntities()) {
			if (manager_->hasGroup<LeftBullet>(e) || manager_->hasGroup<RightBullet>(e)) {
				Image* image = manager_->getComponent<Image>(e);
				Transform* tr_ = manager_->getComponent<Transform>(e);

				SDL_Rect dest = build_sdlrect(tr_->pos_, tr_->width_, tr_->height_);
				image->tex_->render(dest, tr_->rotation_);
			}
		}
	}
}

void RenderSystem::drawHand() {
	auto state_ = manager_->getSystem<GameManagerSystem>()->getState();
	if (state_ != GameManagerSystem::RUNNING) {
		auto myId = manager_->getSystem<NetworkSystem>()->getId();

		auto& t = sdlutils().images().at("hand");
		t.render((myId == 0 ? leftFighterTr_->pos_.getX() : rightFighterTr_->pos_.getX()),
			(myId == 0 ? leftFighterTr_->pos_.getY() - 65 : rightFighterTr_->pos_.getY() - 65));
	}
}

void RenderSystem::drawScore() {
	auto& score_ = manager_->getSystem<GameManagerSystem>()->getScore();

	// puntuacion
	Texture scoreMsg(sdlutils().renderer(), std::to_string(score_[0]) + " - " + std::to_string(score_[1]),
		sdlutils().fonts().at("ARIAL16"), build_sdlcolor(0xffffffff));
	scoreMsg.render((sdlutils().width() - scoreMsg.width()) / 2, 10);
}

void RenderSystem::drawMsgs() {
	auto state_ = manager_->getSystem<GameManagerSystem>()->getState();
	// mensajes solo aparecen cuando no se esta jugando
	if (state_ != GameManagerSystem::RUNNING) {
		// mensaje de fin de juego
		if (state_ == GameManagerSystem::GAMEOVER) {
			auto& t = sdlutils().msgs().at("gameover");
			t.render((sdlutils().width() - t.width()) / 2,
				(sdlutils().height() - t.height()) / 2 - 50);
		}

		// mensaje de nueva partida
		if (state_ == GameManagerSystem::NEWGAME) {
			auto& t = sdlutils().msgs().at("start");
			t.render((sdlutils().width() - t.width()) / 2,
				sdlutils().height() / 2 + t.height() * 2);
		}
		// mensaje de continuar la partida
		else {
			auto& t = sdlutils().msgs().at("continue");
			t.render((sdlutils().width() - t.width()) / 2,
				sdlutils().height() / 2 + t.height() * 2);
		}

		auto myId = manager_->getSystem<NetworkSystem>()->getId();

		// mensaje de en que lado esta jugando el usuario
		Texture side(sdlutils().renderer(), (myId == 0 ? "You are playing left" : "You are playing right"),
			sdlutils().fonts().at("ARIAL16"), build_sdlcolor(0xffffffff));
		side.render((sdlutils().width() - side.width()) / 2,
			sdlutils().height() - side.height() - 2 * side.height() - 10);

		// mensaje que indica con que tecla se cambia de lado
		auto isMaster = manager_->getSystem<NetworkSystem>()->isMaster();
		auto isGameReady = manager_->getSystem<NetworkSystem>()->isGameReady();

		if (isMaster && !isGameReady) {
			Texture switch_key(sdlutils().renderer(), "(press P to switch side)", sdlutils().fonts().at("ARIAL16"),
				build_sdlcolor(0xffffffff));
			switch_key.render((sdlutils().width() - switch_key.width()) / 2,
				sdlutils().height() - switch_key.height() - 10);
		}
	}
}

void RenderSystem::drawNames() {
	auto& names_ = manager_->getSystem<NetworkSystem>()->getNames();

	// nombre del jugador 0
	Texture name_0(sdlutils().renderer(), names_[0], sdlutils().fonts().at("ARIAL16"), build_sdlcolor(0xffffffff));
	name_0.render(10, 10);

	// nombre del jugador 1
	Texture name_1(sdlutils().renderer(), names_[1], sdlutils().fonts().at("ARIAL16"), build_sdlcolor(0xffffffff));
	name_1.render(sdlutils().width() - name_1.width() - 10, 10);

	// se dibuja una "estrella" al lado del nombre del master
	auto isMaster = manager_->getSystem<NetworkSystem>()->isMaster();
	auto myId = manager_->getSystem<NetworkSystem>()->getId();

	Texture isMasterMsg(sdlutils().renderer(), "*", sdlutils().fonts().at("ARIAL24"), build_sdlcolor(0xff0000ff));
	isMasterMsg.render((isMaster && myId == 1) || (!isMaster && myId == 0) ?
		sdlutils().width() - isMasterMsg.width() - 1 : 1, 10);

}
