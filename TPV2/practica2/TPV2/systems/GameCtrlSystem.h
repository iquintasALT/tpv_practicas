#pragma once

#include "../ecs/System.h"

class GameCtrlSystem : public System {
public:
	// - a este método se le va a llamar cuando muere el caza.
	// - desactivar los asteroides y las balas, actualizar el estado del juego, etc.
	void onFighterDeath();

	// - a este método se le va a llamar cuando no haya más asteroides.
	// - desactivar todas las balas, etc.
	void onAsteroidsExtinction();

	// - devuelve el estado del juego.
	GameState getGameState();

	// - inicializar el estado del juego si es necesario, etc.
	void init() override;

	// - si el juego está parado y el jugador pulsa SDLK_SPACE cambia el estado como
	// en la práctica 1, etc.
	void update() override;
};