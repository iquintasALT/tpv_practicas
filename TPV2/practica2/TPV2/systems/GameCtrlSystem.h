#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../components/Health.h"

#include "../sdlutils/InputHandler.h"

#include "AsteroidsSystem.h"

enum class GameState { NEWGAME, PAUSED, RUNNING, GAMEOVER, WIN };

class GameCtrlSystem : public System {
public:
	// - a este método se le va a llamar cuando muere el caza.
	// - desactivar los asteroides y las balas, actualizar el estado del juego, etc.
	void onFighterDeath() {
		gameState = manager_->getComponent<Health>(manager_->getHandler<Player_hdlr>())->lifes_ > 0 ?
			GameState::PAUSED : GameState::GAMEOVER;

		//desactivamos todas las entidades para su posterior eliminacion
		for (auto entity : manager_->getEntities())
			if (manager_->hasGroup<Asteroid_grp>(entity) || manager_->hasGroup<Bullet_grp>(entity))
				manager_->setActive(entity, false);

		manager_->getSystem<AsteroidsSystem>()->resetAsteroids();
	};

	// - a este método se le va a llamar cuando no haya más asteroides.
	// - desactivar todas las balas, etc.
	void onAsteroidsExtinction() {
		//desactivamos todas las entidades para su posterior eliminacion
		for (auto entity : manager_->getEntities())
			if (manager_->hasGroup<Bullet_grp>(entity))
				manager_->setActive(entity, false);
	};

	// - devuelve el estado del juego.
	GameState getGameState() { return gameState; };

	void setGameState(GameState const gs) { gameState = gs; };

	// - inicializar el estado del juego si es necesario, etc.
	void init() override {};

	// - si el juego está parado y el jugador pulsa SDLK_SPACE cambia el estado como
	// en la práctica 1, etc.
	void update() override {
		if (ih().keyDownEvent()) {
			// si, estando fuera de juego, le da a espacio
			if (ih().isKeyDown(SDL_SCANCODE_SPACE) && gameState != GameState::RUNNING) {
				// cambiamos a estado RUNNING o NEWGAME dependiendo de si estamos en
				// un estado final (WIN, GAMEOVER) o no (NEWGAME, PAUSED)
				gameState = gameState > GameState::RUNNING ? GameState::NEWGAME : GameState::RUNNING);

				// si pasamos a RUNNING, generamos los 10 asteroides y reseteamos el timer de spawn de los asteroides
				if (gameState == GameState::RUNNING) 
					manager_->getSystem<AsteroidsSystem>()->addAsteroid();
				// si pasamos a NEWGAME, reseteamos las vidas del caza
				else if (gameState == GameState::NEWGAME) {
					auto health = manager_->getComponent<Health>(manager_->getHandler<Player_hdlr>());
					health->lifes_ = health->maxLifes_;
				}
			}
		}
	};

private:
	GameState gameState = GameState::NEWGAME;
};