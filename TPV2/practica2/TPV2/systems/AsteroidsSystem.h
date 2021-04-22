#pragma once

#include "../ecs/Entity.h"
#include "../ecs/System.h"

class AsteroidsSystem : public System {
public:
	// - a�ade n asteroides al juego como en la pr�ctica 1.
	void addAsteroids(int n);

	// - desactivar el asteroide �a� y crear 2 asteroides como en la pr�ctica 1.
	// - pasamos una referencia a la bala aunque no se usa de momento (en el futuro
	// se puede usar para tener comportamientos distintos depende del tipo de
	// bala, etc.)
	// - si no hay m�s asteroides avisar al GameCtrlSystem
	void onCollisionWithBullet(Entity* a, Entity* b);

	// - si el juego est� parado no hacer nada.
	// - mover los asteroides como en la pr�ctica 1.
	void update() override;

private:
	std::size_t numOfAsteroids_;
};