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
bool IsCollision(const Plane& p, const Sphere& s) { return IsCollision(s, p); }

// Line * Plane
bool IsCollision(const Line& l, const Plane& p) {
	// 垂直の場合はヒットしていない
	if (Dot(p.normal, l.diff) == 0.0f) { return false; }
	return true;
}
bool IsCollision(const Plane& p, const Line& l) { return IsCollision(l, p); }

// Ray * Plane
bool IsCollision(const Ray& r, const Plane& p) {
	// 垂直の場合はヒットしていない
	if (Dot(p.normal, r.diff) == 0.0f) { return false; }

	float t = (p.distance - Dot(r.origin, p.normal)) / Dot(r.diff, p.normal);
	if (t >= 0) { return true; }
	return false;
}
bool IsCollision(const Plane& p, const Ray& r) { return IsCollision(r, p); }

// Segment * Plane
bool IsCollision(const Segment& s, const Plane& p) {
	// 垂直の場合はヒットしていない
	if (Dot(p.normal, s.diff) == 0.0f) { return false; }
	
	float t = (p.distance - Dot(s.origin, p.normal)) / Dot(s.diff, p.normal);
	if (t >= 0 && t <= 1) { return true; }
	return false;
}
bool IsCollision(const Plane& p, const Segment& s) { return IsCollision(s, p); }