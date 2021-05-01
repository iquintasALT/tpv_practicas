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
	// - a este m�todo se le va a llamar cuando muere el caza.
	// - desactivar los asteroides y las balas, actualizar el estado del juego, etc.
	void onFighterDeath();

	// - a este m�todo se le va a llamar cuando no haya m�s asteroides.
	// - desactivar todas las balas, etc.
	void onAsteroidsExtinction();

	// - devuelve el estado del juego.
	inline GameState getGameState() { return gameState; };

	inline void setGameState(GameState const gs) { gameState = gs; };

	// - inicializar el estado del juego si es necesario, etc.
	inline void init() override {};

	// - si el juego est� parado y el jugador pulsa SDLK_SPACE cambia el estado como
	// en la pr�ctica 1, etc.
	void update() override;

private:
	GameState gameState = GameState::NEWGAME;
};