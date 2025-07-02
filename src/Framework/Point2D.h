#pragma once

#include <string>

struct Point2D
{
public:
	Point2D();
	Point2D(float x, float y);
	void setX(float x);
	void setY(float y);
	float getX() const;
	float getY() const;
private:
	float x;
	float y;
};

