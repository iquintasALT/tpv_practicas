#ifndef PRIZE_H_
#define PRIZE_H_

#include "GameCharacter.h"

//clase SDLApplication para evitar dependecias circulares
class SDLApplication;
//clase PlayState para evitar dependecias circulares
class PlayState;

enum class PrizeType{ExtraLive, NextLevel};

class Prize : public GameCharacter
{
private:
	PrizeType myType;

public:
	//mejor como gameCharacter? aunque no use iterador ni tenga update
	Prize(Point2D const iniPos, SDLApplication* appPtr, Point2D const framePos, Point2D const size, PrizeType const Ptype, PlayState* playState);
	virtual ~Prize() {};
	//Getter
	PrizeType getPrizeType() const { return myType; };
	virtual void saveToFile(ofstream& savelfile) {};
};

#endif
