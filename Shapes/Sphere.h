#pragma once
#include "Vector3Func.h"
#include "Matrix4x4Func.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdint>

struct Sphere {
	Vector3 center;	//!< 中心点
	float radius;	//!< 半径
};

// 球を描画
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 12;									// 分割数
	const float kLonEvery = (float)(2.0f * M_PI) / (float)kSubdivision;	// 経度分割1つ分の角度
	const float kLatEvery = (float)M_PI / (float)kSubdivision;			// 緯度分割1つ分の角度

	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = (float)(-M_PI / 2.0f + kLatEvery * latIndex); // 現在の緯度
		// 緯度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 現在の経度
			// world座標系でのa,b,cを求める
			Vector3 a, b, c;
			a = { cosf(lat) * cosf(lon) * sphere.radius, sinf(lat) * sphere.radius, cosf(lat) * sinf(lon) * sphere.radius };
			a = Add(a, sphere.center);
			b = { cosf(lat + kLatEvery) * cosf(lon) * sphere.radius, sinf(lat + kLatEvery) * sphere.radius, cosf(lat + kLatEvery) * sinf(lon) * sphere.radius };
			b = Add(b, sphere.center);
			c = { cosf(lat) * cosf(lon + kLonEvery) * sphere.radius, sinf(lat) * sphere.radius, cosf(lat) * sinf(lon + kLonEvery) * sphere.radius };
			c = Add(c, sphere.center);
			// a,b,cをScreen座標系まで変換
			Matrix4x4 aWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, a);
			Matrix4x4 aWorldViewProjectionMatrix = Multiply(aWorldMatrix, viewProjectionMatrix);
			Vector3 aScreenVertices = TransformCoord(TransformCoord({ 0.0f,0.0f,0.0f }, aWorldViewProjectionMatrix), viewportMatrix);
			Matrix4x4 bWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, b);
			Matrix4x4 bWorldViewProjectionMatrix = Multiply(bWorldMatrix, viewProjectionMatrix);
			Vector3 bScreenVertices = TransformCoord(TransformCoord({ 0.0f,0.0f,0.0f }, bWorldViewProjectionMatrix), viewportMatrix);
			Matrix4x4 cWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, c);
			Matrix4x4 cWorldViewProjectionMatrix = Multiply(cWorldMatrix, viewProjectionMatrix);
			Vector3 cScreenVertices = TransformCoord(TransformCoord({ 0.0f,0.0f,0.0f }, cWorldViewProjectionMatrix), viewportMatrix);
			// ab,bcで線を引く
			Novice::DrawLine((int)aScreenVertices.x, (int)aScreenVertices.y, (int)bScreenVertices.x, (int)bScreenVertices.y, color);
			Novice::DrawLine((int)aScreenVertices.x, (int)aScreenVertices.y, (int)cScreenVertices.x, (int)cScreenVertices.y, color);
		}
	}
}