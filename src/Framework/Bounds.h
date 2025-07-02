#pragma once

#include "Point2D.h"

namespace PositionalCache
{
class Bounds
{
public:
	Bounds(const Point2D& pointA, const Point2D& pointB);
	void normalize();
	void setPointA(const Point2D& newPoint);
	void setPointB(const Point2D& newPoint);
	Point2D getPointA() const;
	Point2D getPointB() const;
	bool containsPosition(const Point2D& coordinate) const;
	bool intersects(const Bounds& other) const;
private:
	Point2D pointA; // Top left corner
	Point2D pointB; // Bottom right corner
};
}


