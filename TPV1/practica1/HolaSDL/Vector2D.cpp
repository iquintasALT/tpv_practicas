#include "Vector2D.h"

//suma de vectores (resultado en primer operando)
void Vector2D::operator+=(Vector2D& const auxVec)  {
	xCoor += auxVec.getXCoor();
	yCoor += auxVec.getYCoor();
}

//resta de vectores (resultado en primer operando)
void Vector2D::operator-=(Vector2D& const auxVec) {
	xCoor -= auxVec.getXCoor();
	yCoor -= auxVec.getYCoor();
}

//suma de vectores (resultado en otro vector)
Vector2D Vector2D::operator+(Vector2D& const auxVec) const {
	int x = xCoor + auxVec.getXCoor();
	int y = yCoor + auxVec.getYCoor();
	return Vector2D(x, y);
}

//resta de vectores
Vector2D Vector2D::operator-(Vector2D& const auxVec) const {
	int x = xCoor - auxVec.getXCoor();
	int y = yCoor - auxVec.getYCoor();
	return Vector2D(x, y);
}

//multiplicacion de vectores
int Vector2D::operator*(Vector2D& const auxVec) const {
	return xCoor * auxVec.getXCoor() + yCoor * auxVec.getYCoor();
}

//multiplicacion de vector por auxiliar
Vector2D Vector2D::operator*(int const value) const {
	int x = xCoor * value;
	int y = yCoor * value;
	return Vector2D(x, y);
}

//igualdad de vectores
bool Vector2D::operator==(Vector2D& const auxVec) const {
	return ((xCoor == auxVec.getXCoor()) && (yCoor == auxVec.getYCoor()));
}

