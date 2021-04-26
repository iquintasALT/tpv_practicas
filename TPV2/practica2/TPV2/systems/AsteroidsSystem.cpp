#include "AsteroidsSystem.h"

void AsteroidsSystem::addAsteroid()
{
	auto asteroid = manager_->addEntity();

	int lives = sdlutils().rand().nextInt(1, 4); //vidas aleatorias

	//marco de aparicion
	Vector2D c = Vector2D(sdlutils().width() / 2, sdlutils().height() / 2) +
		Vector2D(sdlutils().rand().nextInt(-100, 101), sdlutils().rand().nextInt(-100, 101));
	//se escogem la anchura o la altura aleatoriamente
	int selection = sdlutils().rand().nextInt(0, 2);
	//tras escoger una de ellas, la segunda componente del vector sera la que no se ha escogido en primer lugar
	//(quitamos el ancho/alto del asteroide para que spawneen siempre dentro 
	int first = (selection == 0 ? sdlutils().rand().nextInt(0, sdlutils().width() - (20 + 10 * lives)) :
		sdlutils().rand().nextInt(0, 2) * (sdlutils().width() - (20 + 10 * lives)));
	int second = (selection == 0 ? sdlutils().rand().nextInt(0, 2) * (sdlutils().height() - (20 + 10 * lives)) :
		sdlutils().rand().nextInt(0, sdlutils().height() - (20 + 10 * lives)));

	//creamos el vector pos y vel
	Vector2D pos = Vector2D(first, second);
	Vector2D vel = (c - pos).normalize() * (sdlutils().rand().nextInt(1, 10) / 10.0);

	// 20 + 10 * lives para el tama�o dependiente de las generaciones que le quedan
	manager_->addComponent<Transform>(asteroid, pos, vel, 20 + 10 * lives, 20 + 10 * lives, 0.0f);
	manager_->addComponent<Generations>(asteroid, lives);

	// 70% TIPO A, 30% TIPO B
	if (sdlutils().rand().nextInt() % 100 < 30) {
		asteroid->addComponent<Follow>();
		asteroid->addComponent<FramedImage>(&sdlutils().images().at("asteroid_gold"), 5, 6);
	}
	else asteroid->addComponent<FramedImage>(&sdlutils().images().at("asteroid"), 5, 6);

	//se añade al grupo para comprobar colisiones mas tarde
	asteroid->setGroup<Asteroid_grp>(asteroid);
	numAsteroids++;
}

void AsteroidsSystem::asteroidFollow(Entity* asteroid)
{
	Transform* tr = manager_->getComponent<Transform>(asteroid);
	//toroidal en el eje X (fluido)
	if (tr->pos_.getX() > sdlutils().width())
		tr->pos_.setX(0 - tr->width_);
	else if (tr->pos_.getX() + tr->width_ < 0)
		tr->pos_.setX(sdlutils().width());

	//toroidal en el eje Y (fluido)
	if (tr->pos_.getY() > sdlutils().height())
		tr->pos_.setY(0 - tr->height_);
	else if (tr->pos_.getY() + tr->height_ < 0)
		tr->pos_.setY(sdlutils().height());
}
}

void AsteroidsSystem::update()
{
	for (Entity* asteroid : manager_->getEntities()) {


}
