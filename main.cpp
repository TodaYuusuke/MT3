#include <Novice.h>
#include "Matrix4x4Func.h"
#include "Vector2.h"

#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "MT3_01_02_確認課題_LE2B_21_トダ_ユウスケ";

struct Sphere {
	Vector3 center;	//!< 中心点
	float radius;	//!< 半径
};

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	
	// カメラ座標
	Vector3 cameraPosition{ 0.0f,1.9f,-6.49f };
	// カメラ角度
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	// 球
	Sphere sphere = { {0.0f,1.0f,0.0f},0.5f };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		// 各行列の計算
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);;
		Matrix4x4 viewPortMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewPortMatrix);
		DrawSphere(sphere, viewProjectionMatrix, viewPortMatrix, 0x000000FF);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}



void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;										// Gridの半分の幅
	const uint32_t kSubdivision = 10;										// 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);	// 1つ分の長さ
	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		// 上の情報を使ってワールド座標上の始点と終点を求める
		Vector3 startPosition = { kGridEvery * ((kSubdivision / 2.0f) - xIndex), 0.0f, kGridEvery * (kSubdivision / 2) };
		Vector3 endPosition = { kGridEvery * ((kSubdivision / 2.0f) - xIndex), 0.0f, -kGridEvery * (kSubdivision / 2) };
		// スクリーン座標系まで変換をかける
		Matrix4x4 startWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, startPosition);
		Matrix4x4 endWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, endPosition);
		Matrix4x4 startWorldViewProjectionMatrix = Multiply(startWorldMatrix, viewProjectionMatrix);
		Matrix4x4 endWorldViewProjectionMatrix = Multiply(endWorldMatrix, viewProjectionMatrix);
		Vector3 startScreenVertices = TransformCoord(TransformCoord({ 0.0f,0.0f,0.0f }, startWorldViewProjectionMatrix), viewportMatrix);
		Vector3 endScreenVertices = TransformCoord(TransformCoord({ 0.0f,0.0f,0.0f }, endWorldViewProjectionMatrix), viewportMatrix);
		// 変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF)、原点は黒ぐらいが良いが、何でも良い
		unsigned int color;
		if (xIndex == kSubdivision / 2.0f) {
			color = 0x000000FF;
		}
		else {
			color = 0xAAAAAAFF;
		}
		Novice::DrawLine((int)startScreenVertices.x, (int)startScreenVertices.y, (int)endScreenVertices.x, (int)endScreenVertices.y, color);
	}
	// 左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// 奥から手前が左右に代わるだけ
		// 上の情報を使ってワールド座標上の始点と終点を求める
		Vector3 startPosition = { kGridEvery * (kSubdivision / 2), 0.0f, kGridEvery * ((kSubdivision / 2.0f) - zIndex) };
		Vector3 endPosition = { -kGridEvery * (kSubdivision / 2), 0.0f, kGridEvery * ((kSubdivision / 2.0f) - zIndex) };
		// スクリーン座標系まで変換をかける
		Matrix4x4 startWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, startPosition);
		Matrix4x4 endWorldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, endPosition);
		Matrix4x4 startWorldViewProjectionMatrix = Multiply(startWorldMatrix, viewProjectionMatrix);
		Matrix4x4 endWorldViewProjectionMatrix = Multiply(endWorldMatrix, viewProjectionMatrix);
		Vector3 startScreenVertices = TransformCoord(TransformCoord({ 0.0f,0.0f,0.0f }, startWorldViewProjectionMatrix), viewportMatrix);
		Vector3 endScreenVertices = TransformCoord(TransformCoord({ 0.0f,0.0f,0.0f }, endWorldViewProjectionMatrix), viewportMatrix);
		// 変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF)、原点は黒ぐらいが良いが、何でも良い
		unsigned int color;
		if (zIndex == kSubdivision / 2.0f) {
			color = 0x000000FF;
		}
		else {
			color = 0xAAAAAAFF;
		}
		Novice::DrawLine((int)startScreenVertices.x, (int)startScreenVertices.y, (int)endScreenVertices.x, (int)endScreenVertices.y, color);
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 12;									// 分割数
	const float kLonEvery = (float)(2.0f * M_PI) / (float)kSubdivision;	// 経度分割1つ分の角度
	const float kLatEvery = (float)M_PI / (float)kSubdivision;			// 緯度分割1つ分の角度

	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = (float)(- M_PI / 2.0f + kLatEvery * latIndex); // 現在の緯度
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