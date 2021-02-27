#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include <fstream>
#include <cmath>

using namespace std;

class Vector2D
{
private:
	//coordenadas 'x' e 'y' --> (x, y)
	int xCoor, yCoor;

public:
	Vector2D() : xCoor(0), yCoor(0) {}; //constructora por defecto
	//constructora con parametros
	Vector2D(int xPoint, int yPoint) : xCoor(xPoint), yCoor(yPoint) {};
	//inversa del vector
	Vector2D reverse() const { return Vector2D(-xCoor, -yCoor); };
	void operator+=(Vector2D& const auxVec);
	void operator-=(Vector2D& const auxVec);
	Vector2D operator+(Vector2D& const auxVec) const;
	Vector2D operator-(Vector2D& const auxVec) const;
	int operator*(Vector2D& const auxVec) const;
	Vector2D operator*(int const value) const;
	bool operator==(Vector2D& const auxVec) const;
	//modulo del vector
	float modulo() { return sqrt(pow(xCoor, 2) + pow(yCoor, 2)); };
	friend ostream& operator<<(ostream& out, Vector2D& const auxVec);
	friend istream& operator>>(istream& in, Vector2D& auxVec);
#pragma region Getters&Setters
	int getXCoor() const { return xCoor; };
	int getYCoor() const { return yCoor; };
	void setXCoor(int const x) { xCoor = x; };
	void setYCoor(int const y) { yCoor = y; };
#pragma endregion
};

//alias Point2D
using Point2D = Vector2D;

#endif