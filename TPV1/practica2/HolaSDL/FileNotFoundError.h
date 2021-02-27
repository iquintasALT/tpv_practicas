#ifndef FILENOTFOUNDERROR_H_
#define FILENOTFOUNDERROR_H_

#include "PacmanError.h"

class FileNotFoundError : public PacmanError {
public:
	FileNotFoundError(string const file) : PacmanError("File " + file + " not found.") {};
};

#endif