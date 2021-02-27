#ifndef FILEFORMATERROR_H_
#define FILEFORMATERROR_H_

#include "PacmanError.h"

class FileFormatError : public PacmanError {
public:
	FileFormatError(string const message) : PacmanError(message) {};
};

#endif