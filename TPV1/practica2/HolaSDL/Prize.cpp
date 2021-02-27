#include "Prize.h"
#include "Game.h"

Prize::Prize(Point2D const iniPos, Game* gameP, Point2D const framePos, Point2D const size, PrizeType const Ptype)
	: GameCharacter(iniPos, gameP, framePos, size) {
	myType = Ptype;
	if (myType == PrizeType::ExtraLive) texture = game->getTexture(TextureName::HeartTexture);
	else texture = game->getTexture(TextureName::BatteryTexture);
}