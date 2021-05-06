// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../ecs/ecs.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"

#include "../systems/AsteroidsSystem.h"
#include "../systems/BulletsSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/FighterSystem.h"
#include "../systems/GameCtrlSystem.h"

Game::Game() {
	mngr_.reset(new Manager());
}

Game::~Game() {}

void Game::init() {

	SDLUtils::init("Asteroid", 800, 600,
		"resources/config/resources.json");

	// FigtherCtrl y Gun se añaden en GameCtrl.h, para controlar que solo lo tenga
	// durante el estado de juego (ver implementacion del update de GameCtrl.h [41-42],
	// update de CollisionManager.h [61-62] y onCollision de AsteroidsManager.cpp [84-85])
	systems.push_back(mngr_->addSystem<AsteroidsSystem>());
	systems.push_back(mngr_->addSystem<BulletsSystem>());
	systems.push_back(mngr_->addSystem<CollisionSystem>());
	systems.push_back(mngr_->addSystem<FighterSystem>());
	systems.push_back(mngr_->addSystem<GameCtrlSystem>());
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;
	SDL_Event event;

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		ih().clearState();
		while (SDL_PollEvent(&event))
			ih().update(event);

		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		for (System* s : systems) s->update();

		mngr_->flushMsgsQueue();
		mngr_->refresh();

		sdlutils().clearRenderer();
		for (System* s : systems) s->render();

		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}