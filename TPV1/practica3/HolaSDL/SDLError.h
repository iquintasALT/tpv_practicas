#ifndef SDLERROR_H_
#define SDLERROR_H_

#include "PacmanError.h"

class SDLError : public PacmanError {
public:
	SDLError(string const error) : PacmanError("SDL stopped working at some point during runtime.\nDetailed error --> " + error) {};
};

#endif