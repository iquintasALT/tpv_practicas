#include "InfoBar.h"
#include "Game.h"

//constructora de InfoBar
InfoBar::InfoBar(Game* gameP, int* const liveCounter, int* const powerUpActive) {
	game = gameP;
	liveCount = liveCounter; puntuation = 0;
	powerUpController = powerUpActive;
}

//destructora de InfoBar
InfoBar::~InfoBar() {
	game = nullptr;
	liveCount = nullptr;
	powerUpController = nullptr;
};

//metodo para la impresion de la informacion en pantalla
void InfoBar::render() const {
	SDL_Rect dstRect;//imprimimos fondo
	dstRect.w = 150; dstRect.h = winHeight;
	dstRect.x = winWidth - 150; dstRect.y = 0;
	Texture* texture = game->getTexture(TextureName::BackgroundTexture);
	texture->render(dstRect);
	//imprimimos vidas
	dstRect.w = dstRect.h = 50;
	texture = game->getTexture(TextureName::HeartTexture);
	for (int i = 0; i < (*liveCount); i++) {
		dstRect.x = (winWidth - 150) + (150 / 3 * i);
		texture->render(dstRect);
	}
	//imprimimos powerUp
	dstRect.w = dstRect.h = 70;
	if ((*powerUpController) != 0) {
		dstRect.y = 50; dstRect.x = winWidth - 157;
		texture = game->getTexture(TextureName::BatteryTexture);
		texture->render(dstRect);
	}
	//imprimimos puntuacion
	dstRect.w = dstRect.h = 60;
	dstRect.y = winHeight - dstRect.h;
	texture = game->getTexture(TextureName::PuntuationTexture);
	int puntAux = puntuation, value;
	for (int i = 2; i > -1; i--) {
		dstRect.x = winWidth - 55 - (50 * i);
		value = puntAux / (int)pow(10, i);
		puntAux %= (int)pow(10, i);
		texture->renderFrame(dstRect, 0, value);
	}
}