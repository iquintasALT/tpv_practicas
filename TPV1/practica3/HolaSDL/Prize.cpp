#include "Prize.h"
#include "SDLApplication.h"

//constructor de prize
Prize::Prize(Point2D const iniPos, SDLApplication* appPtr, Point2D const framePos, Point2D const size, PrizeType const Ptype, PlayState* const playStatePtr)
	: GameCharacter(iniPos, appPtr, framePos, size, playStatePtr) {
	//guardamos el tipo de recompensa
	myType = Ptype;
	//segun el tipo, asignamos la textura
	if (myType == PrizeType::ExtraLive) texture = app->getTexture(TextureName::HeartTexture);
	else texture = app->getTexture(TextureName::BatteryTexture);
}