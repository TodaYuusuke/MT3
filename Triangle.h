#pragma once
#include "Vector3Func.h"
#include "Matrix4x4Func.h"

struct Triangle {
	Vector3 vertices[3]; // !< 頂点
};

// 三角形を描画
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 points[3];
	points[0] = TransformCoord(TransformCoord(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	points[1] = TransformCoord(TransformCoord(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	points[2] = TransformCoord(TransformCoord(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);

	Novice::DrawTriangle((int)points[0].x, (int)points[0].y, (int)points[1].x, (int)points[1].y, (int)points[2].x, (int)points[2].y, color, kFillModeWireFrame);
}