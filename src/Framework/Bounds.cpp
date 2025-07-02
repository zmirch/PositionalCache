#include "Bounds.h"

namespace PositionalCache
{
Bounds::Bounds(const Point2D& pointA, const Point2D& pointB) : pointA(pointA), pointB(pointB)
{
	normalize();
}

void Bounds::normalize()
{
	// Ensures pointA is the top left corner, and pointB is the bottom right corner

	double lesserX = std::min(pointA.getX(), pointB.getX());
	double greaterX = std::max(pointA.getX(), pointB.getX());

	double lesserY = std::min(pointA.getY(), pointB.getY());
	double greaterY = std::max(pointA.getY(), pointB.getY());

	pointA = Point2D(lesserX, lesserY);
	pointB = Point2D(greaterX, greaterY);
}

void Bounds::setPointA(const Point2D& newPoint)
{
	pointA = newPoint;
	normalize();
}

void Bounds::setPointB(const Point2D& newPoint)
{
	pointB = newPoint;
	normalize();
}

Point2D Bounds::getPointA() const
{
	return pointA;
}

Point2D Bounds::getPointB() const
{
	return pointB;
}

bool Bounds::containsPosition(const Point2D& coordinate) const
{
	return !(coordinate.getX() > pointB.getX() ||
		coordinate.getX() < pointA.getX() ||
		coordinate.getY() > pointB.getY() ||
		coordinate.getY() < pointA.getY());
}

bool Bounds::intersects(const Bounds& other) const
{
	// If one rectangle is completely to the left, right, above, or below the other, they do not intersect
	return !(pointB.getX() < other.pointA.getX() ||
			 pointA.getX() > other.pointB.getX() ||
			 pointB.getY() < other.pointA.getY() ||
			 pointA.getY() > other.pointB.getY());
}

}
