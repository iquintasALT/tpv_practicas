#include "GameCtrlSystem.h"

void GameCtrlSystem::onFighterDeath() {
	gameState = manager_->getComponent<Health>(manager_->getHandler<Player_hdlr>())->lifes_ > 0 ?
		GameState::PAUSED : GameState::GAMEOVER;

	//desactivamos todas las entidades para su posterior eliminacion
	for (auto entity : manager_->getEntities())
		if (manager_->hasGroup<Asteroid_grp>(entity) || manager_->hasGroup<Bullet_grp>(entity))
			manager_->setActive(entity, false);

	manager_->getSystem<AsteroidsSystem>()->resetAsteroids();
};

void GameCtrlSystem::onAsteroidsExtinction() {
	//desactivamos todas las entidades para su posterior eliminacion
	for (auto entity : manager_->getEntities())
		if (manager_->hasGroup<Bullet_grp>(entity))
			manager_->setActive(entity, false);
};

void GameCtrlSystem::update() {
	if (ih().keyDownEvent()) {
		// si, estando fuera de juego, le da a espacio
		if (ih().isKeyDown(SDL_SCANCODE_SPACE) && gameState != GameState::RUNNING) {
			// cambiamos a estado RUNNING o NEWGAME dependiendo de si estamos en
			// un estado final (WIN, GAMEOVER) o no (NEWGAME, PAUSED)
			gameState = gameState > GameState::RUNNING ? GameState::NEWGAME : GameState::RUNNING;

			// si pasamos a RUNNING, generamos los 10 asteroides y reseteamos el timer de spawn de los asteroides
			if (gameState == GameState::RUNNING)
				manager_->getSystem<AsteroidsSystem>()->addAsteroids(10);
			// si pasamos a NEWGAME, reseteamos las vidas del caza
			else if (gameState == GameState::NEWGAME) {
				auto health = manager_->getComponent<Health>(manager_->getHandler<Player_hdlr>());
				health->lifes_ = health->maxLifes_;
			}
		}
	}
};