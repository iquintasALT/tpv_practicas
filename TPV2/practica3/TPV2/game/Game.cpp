#include "Game.h"

#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"

#include "../systems/CollisionSystem.h"
#include "../systems/GameManagerSystem.h"
#include "../systems/NetworkSystem.h"
#include "../systems/FighterSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/BulletsSystem.h"

Game::Game() {
	mngr_.reset(new Manager());
	networkSys_ = nullptr;
	fighterSys_ = nullptr;
	collisionSys_ = nullptr;
	gameMngrSys_ = nullptr;
	renderSys_ = nullptr;
	bulletSys_ = nullptr;
}

Game::~Game() {}

void Game::init(const char *host, Uint16 port) {

	// ask the player for a name
	std::string playerName;
	std::cout << "Enter you name: ";
	std::cin >> playerName;

	SDLUtils::init("Fighterz", 800, 600,
			"./resources/config/resources.json");

	networkSys_ = mngr_->addSystem<NetworkSystem>(host, port, playerName);
	fighterSys_ = mngr_->addSystem<FighterSystem>();
	collisionSys_ = mngr_->addSystem<CollisionSystem>();
	gameMngrSys_ = mngr_->addSystem<GameManagerSystem>();
	renderSys_ = mngr_->addSystem<RenderSystem>();
	bulletSys_ = mngr_->addSystem<BulletsSystem>();
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

		mngr_->refresh();

		fighterSys_->update();
		collisionSys_->update();
		gameMngrSys_->update();
		bulletSys_->update();
		networkSys_->update();

		sdlutils().clearRenderer();
		renderSys_->update();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}

