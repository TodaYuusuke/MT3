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

// 線の描画
void DrawLine(const Line& line, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 points[2];
	points[0] = Add(line.origin, Multiply(-2.0f, line.diff));
	points[1] = Add(line.origin, Multiply(2.0f, line.diff));

	points[0] = TransformCoord(TransformCoord(points[0], viewProjectionMatrix), viewportMatrix);
	points[1] = TransformCoord(TransformCoord(points[1], viewProjectionMatrix), viewportMatrix);

	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[1].x, (int)points[1].y, color);
}
void DrawLine(const Ray& ray, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 points[2];
	points[0] = TransformCoord(TransformCoord(ray.origin, viewProjectionMatrix), viewportMatrix);
	points[1] = TransformCoord(TransformCoord(Add(ray.origin, Multiply(10.0f, ray.diff)), viewProjectionMatrix), viewportMatrix);
	
	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[1].x, (int)points[1].y, color);
}
void DrawLine(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 points[2];
	points[0] = TransformCoord(TransformCoord(segment.origin, viewProjectionMatrix), viewportMatrix);
	points[1] = TransformCoord(TransformCoord(Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);

	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[1].x, (int)points[1].y, color);
}
