#pragma once
#include "Vector3Func.h"
#include "Matrix4x4Func.h"

struct Plane {
	Vector3 normal;	//!< 法線
	float distance;	//!< 距離
};

// 法線ベクトルと垂直なベクトルを求める
Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f,-vector.z, vector.y };
}

// 平面を描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = Multiply(plane.distance, plane.normal); // 1
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal)); // 2
	perpendiculars[2] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z }; // 3
	perpendiculars[1] = Cross(plane.normal,perpendiculars[0]); // 4
	perpendiculars[3] = { -perpendiculars[1].x,-perpendiculars[1].y,-perpendiculars[1].z }; // 5
	// 6
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = TransformCoord(TransformCoord(point, viewProjectionMatrix), viewportMatrix);
	}

	// 描画
	for (int32_t index = 0; index < 4; ++index) {
		int32_t next = index + 1;
		if (next >= 4) { next = 0; }
		Novice::DrawLine((int)points[index].x, (int)points[index].y, (int)points[next].x, (int)points[next].y, color);
	}
}