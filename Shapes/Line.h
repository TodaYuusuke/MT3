#pragma once
#include "Vector3Func.h"

// 直線
struct Line {
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};
// 半直線
struct Ray {
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};
// 線分
struct Segment {
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};


// 正射影ベクトルを求める
static Vector3 Project(const Vector3& v1, const Vector3& v2) {
	Vector3 unitVector = Normalize(v2);
	return Multiply(Dot(v1, unitVector),unitVector);
}
// 最近接点を求める
static Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 proj = Project(Subtract(point, segment.origin), segment.diff);
	return Add(segment.origin, proj);
}