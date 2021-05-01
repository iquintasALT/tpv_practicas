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
	void onFighterDeath();

	// - a este método se le va a llamar cuando no haya más asteroides.
	// - desactivar todas las balas, etc.
	void onAsteroidsExtinction();

	// - devuelve el estado del juego.
	inline GameState getGameState() { return gameState; };

	inline void setGameState(GameState const gs) { gameState = gs; };

	// - inicializar el estado del juego si es necesario, etc.
	inline void init() override {};

	// - si el juego está parado y el jugador pulsa SDLK_SPACE cambia el estado como
	// en la práctica 1, etc.
	void update() override;

private:
	GameState gameState = GameState::NEWGAME;
};