#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <vector>
#include <Novice.h>

/// <summary>
/// 3次元ベクトルの加算
/// </summary>
/// <param name="v1">... ベクトル1</param>
/// <param name="v2">... ベクトル2</param>
/// <returns></returns>
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	return v;
}

/// <summary>
/// 3次元ベクトルの減算
/// </summary>
/// <param name="v1">... ベクトル1</param>
/// <param name="v2">... ベクトル2</param>
/// <returns></returns>
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	return v;
}

/// <summary>
/// 3次元ベクトルのスカラー倍
/// </summary>
/// <param name="scalar">... スカラー</param>
/// <param name="v">... ベクトル</param>
/// <returns></returns>
Vector3 Multiply(const float& scalar, const Vector3& v) {
	Vector3 result;
	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;
	return result;
}

/// <summary>
/// 3次元ベクトルの行列倍
/// </summary>
/// <param name="matrix">... 行列/param>
/// <param name="v">... ベクトル</param>
/// <returns></returns>
Vector3 Multiply(const Matrix4x4& matrix, const Vector3& v) {
	Vector3 result{};
	result.x = v.x * matrix.m[0][0] + v.y * matrix.m[1][0] + v.z * matrix.m[2][0] + matrix.m[3][0];
	result.y = v.x * matrix.m[0][1] + v.y * matrix.m[1][1] + v.z * matrix.m[2][1] + matrix.m[3][1];
	result.z = v.x * matrix.m[0][2] + v.y * matrix.m[1][2] + v.z * matrix.m[2][2] + matrix.m[3][2];
	return result;
}

/// <summary>
/// 3次元ベクトルの内積
/// </summary>
/// <param name="v1">... ベクトル1</param>
/// <param name="v2">... ベクトル2</param>
/// <returns></returns>
float Dot(const Vector3& v1, const Vector3& v2) {
	float result;
	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return result;
}

/// <summary>
/// 3次元ベクトルの長さ
/// </summary>
/// <param name="v">... ベクトル</param>
/// <returns></returns>
float Length(const Vector3& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

/// <summary>
/// 3次元ベクトルの正規化
/// </summary>
/// <param name="v">... ベクトル</param>
/// <returns></returns>
Vector3 Normalize(const Vector3& v) {
	Vector3 norm = v;
	float length = Length(v);
	if (length != 0.0f) {
		norm.x /= length;
		norm.y /= length;
		norm.z /= length;
	}
	return norm;
}

/// <summary>
/// クロス積を求める関数
/// </summary>
/// <param name="v1">... ベクトル１</param>
/// <param name="v2">... ベクトル１</param>
/// <returns></returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
}



/// <summary>
/// 3次元ベクトルの描画
/// </summary>
/// <param name="x">... 描画する座標X</param>
/// <param name="y">... 描画する座標Y</param>
/// <param name="vector">... ベクトル</param>
/// <param name="label">... ラベル</param>
/// <returns></returns>
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	// 描画用の幅
	const int kColumnWidth = 60;

	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}