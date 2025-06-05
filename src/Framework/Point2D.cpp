#include "Point2D.h"

#include <iostream>

Point2D::Point2D()
{
	setX(0);
	setY(0);
}

Point2D::Point2D(float x, float y)
{
	setX(x);
	setY(y);
}

void Point2D::setX(float x)
{
	if (x >= 0) // Coordinates start at (0,0) in the upper left corner
		this->x = x;
	else
		this->x = 0;
}

void Point2D::setY(float y)
{
	if (y >= 0)
		this->y = y;
	else
		this->y = 0;
}

float Point2D::getX() const
{
	return x;
}

float Point2D::getY() const
{
	return y;
}
