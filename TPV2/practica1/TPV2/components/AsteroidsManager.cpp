#include "AsteroidsManager.h"

void AsteroidsManager::generateAsteroid()
{
	auto asteroid = entity_->getMngr()->addEntity();

	//creacion de un asteroide desde 0
	//marco de aparicion
	Vector2D c = Vector2D(sdlutils().width() / 2, sdlutils().height() / 2) +
		Vector2D(sdlutils().rand().nextInt(-100, 101), sdlutils().rand().nextInt(-100, 101));
	//escogemos la anchura o la altura aleatoriamente
	int selection = sdlutils().rand().nextInt(0, 2);
	//tras escoger una de ellas, la segunda componente del vector sera la contraria
	int first = (selection == 0 ? sdlutils().rand().nextInt(0, sdlutils().width()) :
		sdlutils().rand().nextInt(0, 2) * sdlutils().width());
	int second = (selection == 0 ? sdlutils().rand().nextInt(0, 2) * sdlutils().height() :
		sdlutils().rand().nextInt(0, sdlutils().height()));

	//creamos el vector pos y vel
	Vector2D pos = Vector2D(first, second);
	Vector2D vel = (c - pos).normalize() * (sdlutils().rand().nextInt(1, 10) / 10.0);

	int lives = sdlutils().rand().nextInt(1, 4);
	// 20 + 10 * lives para el tamaño dependiente de las generaciones que le quedan
	asteroid->addComponent<Transform>(pos, vel, 20 + 10 * lives, 20 + 10 * lives, 0.0f);
	asteroid->addComponent<ShowAtOppositeSide>();
	asteroid->addComponent<Generations>(lives);

	// 70% TIPO A, 30% TIPO B
	if (sdlutils().rand().nextInt() % 100 < 30) {
		asteroid->addComponent<Follow>();
		asteroid->addComponent<FramedImage>(&sdlutils().images().at("asteroid_gold"), 5, 6);
	}
	else asteroid->addComponent<FramedImage>(&sdlutils().images().at("asteroid"), 5, 6);

	asteroid->setGroup<Asteroid_grp>(asteroid);
	numAsteroids++;
}

void AsteroidsManager::generateAsteroid(int lives, Entity* prev_asteroid)
{
	auto asteroid = entity_->getMngr()->addEntity();

	int rand = sdlutils().rand().nextInt(0, 360);
	//creacion de un asteroide a partir de otro
	Transform* t = prev_asteroid->getComponent<Transform>();

	Vector2D pos = t->getPos() + t->getVel().rotate(rand) * 2 * t->getW();
	Vector2D vel = t->getVel().rotate(rand) * 1.1f;

	// 20 + 10 * lives para el tamaño dependiente de las generaciones que le quedan
	asteroid->addComponent<Transform>(pos,
		vel, 20 + 10 * lives, 20 + 10 * lives, 0.0f);
	asteroid->addComponent<ShowAtOppositeSide>();
	asteroid->addComponent<Generations>(lives);

	// 70% TIPO A, 30% TIPO B
	if (sdlutils().rand().nextInt() % 100 < 30) {
		asteroid->addComponent<Follow>();
		asteroid->addComponent<FramedImage>(&sdlutils().images().at("asteroid_gold"), 5, 6);
	}
	else asteroid->addComponent<FramedImage>(&sdlutils().images().at("asteroid"), 5, 6);

	asteroid->setGroup<Asteroid_grp>(asteroid);
	numAsteroids++;
}

void AsteroidsManager::onCollision(Entity* hit_asteroid) {
	int lives = --hit_asteroid->getComponent<Generations>()->getLives();
	hit_asteroid->setActive(false);

	if (lives > 0) {
		generateAsteroid(lives, hit_asteroid);
		generateAsteroid(lives, hit_asteroid);
	}

	numAsteroids--;
	if (numAsteroids <= 0) state_->setState(states::WIN);
}