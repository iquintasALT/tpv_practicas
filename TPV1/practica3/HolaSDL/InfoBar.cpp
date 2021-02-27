#include "InfoBar.h"
#include "PlayState.h"
#include "SDLApplication.h"


//constructora de InfoBar
InfoBar::InfoBar(SDLApplication* appPtr, int* const liveCounter, int* const powerUpActive, int const points, Point2D pos, Point2D size) 
	: GameObject(pos, size.getYCoor(), size.getXCoor(), appPtr){
	liveCount = liveCounter; puntuation = points;
	powerUpController = powerUpActive;
}

//destructora de InfoBar
InfoBar::~InfoBar() {
	liveCount = nullptr;
	powerUpController = nullptr;
};

//metodo para la impresion de la informacion en pantalla
void InfoBar::render() const {
	SDL_Rect dstRect = getDestRect();//imprimimos fondo
	Texture* texture = app->getTexture(TextureName::InfobarBgTexture);
	texture->render(dstRect);
	//imprimimos vidas
	dstRect.w = dstRect.h = 50;
	texture = app->getTexture(TextureName::HeartTexture);
	for (int i = 0; i < (*liveCount); i++) {
		dstRect.x = (winWidth - infoBarWidth) + (infoBarWidth / 3 * i);
		texture->render(dstRect);
	}
	//imprimimos powerUp
	dstRect.w = dstRect.h = 70;
	if ((*powerUpController) != 0) {
		dstRect.y = 50; dstRect.x = winWidth - 157;
		texture = app->getTexture(TextureName::BatteryTexture);
		texture->render(dstRect);
	}
	//imprimimos puntuacion
	dstRect.w = dstRect.h = 60;
	dstRect.y = winHeight - dstRect.h;
	texture = app->getTexture(TextureName::PuntuationTexture);
	int puntAux = puntuation, value;
	for (int i = 2; i > -1; i--) {
		dstRect.x = winWidth - 55 - (50 * i);
		value = puntAux / (int)pow(10, i);
		puntAux %= (int)pow(10, i);
		texture->renderFrame(dstRect, 0, value);
	}
}