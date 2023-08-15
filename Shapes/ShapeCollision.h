#pragma once
#include "Line.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "AABB.h"

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
// Sphere * AABB
bool IsCollision(const Sphere& s, const AABB& aabb) {
	// 最近接点を求める
	Vector3 closestPoint{};
	closestPoint.x = std::clamp(s.center.x, aabb.min.x, aabb.max.x);
	closestPoint.y = std::clamp(s.center.y, aabb.min.y, aabb.max.y);
	closestPoint.z = std::clamp(s.center.z, aabb.min.z, aabb.max.z);
	// 最近接点と球の中心にとの距離を求める
	float distance = Length(Subtract(closestPoint, s.center));
	// 距離が半径よりも小さければ衝突
	if (distance <= s.radius) {
		return true;
	}

	return false;
}
bool IsCollision(const AABB& aabb, const Sphere& s) { return IsCollision(s, aabb); }

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


// Line * Triangle
bool IsCollision(const Line& line, const Triangle& triangle) {
	// 三角形から平面を求める
	Plane plane = TriangleToPlane(triangle);
	// 垂直の場合はヒットしていない
	if (Dot(plane.normal, line.diff) == 0.0f) { return false; }
	
	// 媒介変数tを求める
	float t = (plane.distance - Dot(line.origin, plane.normal)) / Dot(line.diff, plane.normal);
	// 衝突点を求める
	Vector3 hitPosition = Add(line.origin, Multiply(t, line.diff));

	// 各辺を結んだベクトルと頂点と衝突点pを結んだベクトルのクロス積を取る
	Vector3 cross01 = Cross(Subtract(triangle.vertices[0], triangle.vertices[1]), Subtract(triangle.vertices[1], hitPosition));
	Vector3 cross12 = Cross(Subtract(triangle.vertices[1], triangle.vertices[2]), Subtract(triangle.vertices[2], hitPosition));
	Vector3 cross20 = Cross(Subtract(triangle.vertices[2], triangle.vertices[0]), Subtract(triangle.vertices[0], hitPosition));

	// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
	if (Dot(cross01, plane.normal) >= 0.0f && Dot(cross12, plane.normal) >= 0.0f && Dot(cross20, plane.normal) >= 0.0f) {
		return true;
	}

	return false;
}
bool IsCollision(const Triangle& t, const Line& l) { return IsCollision(l, t); }

// Ray * Triangle
bool IsCollision(const Ray& ray, const Triangle& triangle) {
	// 三角形から平面を求める
	Plane plane = TriangleToPlane(triangle);
	// 垂直の場合はヒットしていない
	if (Dot(plane.normal, ray.diff) == 0.0f) { return false; }

	// 媒介変数tを求める
	float t = (plane.distance - Dot(ray.origin, plane.normal)) / Dot(ray.diff, plane.normal);
	if (!(t >= 0)) { return false; }
	// 衝突点を求める
	Vector3 hitPosition = Add(ray.origin, Multiply(t, ray.diff));

	// 各辺を結んだベクトルと頂点と衝突点pを結んだベクトルのクロス積を取る
	Vector3 cross01 = Cross(Subtract(triangle.vertices[0], triangle.vertices[1]), Subtract(triangle.vertices[1], hitPosition));
	Vector3 cross12 = Cross(Subtract(triangle.vertices[1], triangle.vertices[2]), Subtract(triangle.vertices[2], hitPosition));
	Vector3 cross20 = Cross(Subtract(triangle.vertices[2], triangle.vertices[0]), Subtract(triangle.vertices[0], hitPosition));

	// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
	if (Dot(cross01, plane.normal) >= 0.0f && Dot(cross12, plane.normal) >= 0.0f && Dot(cross20, plane.normal) >= 0.0f) {
		return true;
	}

	return false;
}
bool IsCollision(const Triangle& t, const Ray& r) { return IsCollision(r, t); }

// Segment * Triangle
bool IsCollision(const Segment& segment, const Triangle& triangle) {
	// 三角形から平面を求める
	Plane plane = TriangleToPlane(triangle);
	// 垂直の場合はヒットしていない
	if (Dot(plane.normal, segment.diff) == 0.0f) { return false; }

	// 媒介変数tを求める
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / Dot(segment.diff, plane.normal);
	if (!(t >= 0 && t <= 1)) { return false; }
	// 衝突点を求める
	Vector3 hitPosition = Add(segment.origin, Multiply(t, segment.diff));

	// 各辺を結んだベクトルと頂点と衝突点pを結んだベクトルのクロス積を取る
	Vector3 cross01 = Cross(Subtract(triangle.vertices[0], triangle.vertices[1]), Subtract(triangle.vertices[1], hitPosition));
	Vector3 cross12 = Cross(Subtract(triangle.vertices[1], triangle.vertices[2]), Subtract(triangle.vertices[2], hitPosition));
	Vector3 cross20 = Cross(Subtract(triangle.vertices[2], triangle.vertices[0]), Subtract(triangle.vertices[0], hitPosition));

	// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
	if (Dot(cross01, plane.normal) >= 0.0f && Dot(cross12, plane.normal) >= 0.0f && Dot(cross20, plane.normal) >= 0.0f) {
		return true;
	}

	return false;
}
bool IsCollision(const Triangle& t, const Segment& s) { return IsCollision(s, t); }


// AABB * AABB
bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	// x軸方向で重なっていない
	if (aabb1.min.x > aabb2.max.x || aabb1.max.x < aabb2.min.x) {
		return false;
	}
	// y軸方向で重なっていない
	if (aabb1.min.y > aabb2.max.y || aabb1.max.y < aabb2.min.y) {
		return false;
	}
	// z軸方向で重なっていない
	if (aabb1.min.z > aabb2.max.z || aabb1.max.z < aabb2.min.z) {
		return false;
	}
	// すべての軸方向で重なっている
	return true;
}