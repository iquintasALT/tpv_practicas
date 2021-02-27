#ifndef PACMANERROR_H_
#define PACMANERROR_H_

#include <stdexcept>
#include <string>

using namespace std;

class PacmanError : public logic_error {
protected:
	PacmanError(string const message): logic_error("Error: " + message) {};
};
#endif