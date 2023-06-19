#pragma once
#include "Line.h"
#include "Plane.h"
#include "Sphere.h"


// Sphere * Sphere
bool IsCollision(const Sphere& s1, const Sphere& s2) {
	float distance = Length(Subtract(s1.center, s2.center));
	return distance <= s1.radius + s2.radius;
}

// Sphere * Plane
bool IsCollision(const Sphere& s, const Plane& p) {
	float distance = Dot(p.normal, s.center) - p.distance;
	return sqrtf(distance * distance) <= s.radius;
}