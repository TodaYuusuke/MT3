#pragma once
#include "../Vector3Func.h"
#include "Line.h"

struct AABB {
	Vector3 min; //!< 最小点
	Vector3 max; //!< 最大点
};

// 線の描画
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	// ⑤ーーーーー⑥
	// ｜＼　　　　｜＼
	// ｜　④ーーー＋ー⑦
	// ｜　｜　　　｜　｜
	// ①ー＋ーーー②　｜
	// 　＼｜　　　　＼｜
	// 　　⓪ーーーーー③
	Vector3 points[8];
	points[0] = aabb.min;
	points[1] = { aabb.min.x, aabb.min.y, aabb.max.z };
	points[2] = { aabb.max.x, aabb.min.y, aabb.max.z };
	points[3] = { aabb.max.x, aabb.min.y, aabb.min.z };
	points[4] = { aabb.min.x, aabb.max.y, aabb.min.z };
	points[5] = { aabb.min.x, aabb.max.y, aabb.max.z };
	points[6] = aabb.max;
	points[7] = { aabb.max.x, aabb.max.y, aabb.min.z };

	for (int i = 0; i < 8; i++) {
		points[i] = TransformCoord(TransformCoord(points[i], viewProjectionMatrix), viewportMatrix);
	}

	Segment segments[12];
	segments[0] = { points[0],points[1] };
	segments[1] = { points[2],points[2] };
	segments[2] = { points[2],points[3] };
	segments[3] = { points[3],points[0] };

	segments[4] = { points[0],points[4] };
	segments[5] = { points[1],points[5] };
	segments[6] = { points[2],points[6] };
	segments[7] = { points[3],points[7] };

	segments[8] = { points[4],points[5] };
	segments[9] = { points[5],points[6] };
	segments[10] = { points[6],points[7] };
	segments[11] = { points[7],points[4] };


	// 描画
	for (int i = 0; i < 12; i++) {
		Novice::DrawLine((int)segments[i].origin.x, (int)segments[i].origin.y, (int)segments[i].diff.x, (int)segments[i].diff.y, color);
	}
}