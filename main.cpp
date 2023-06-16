#include <Novice.h>
#include <ImGuiManager.h>
#include "Matrix4x4Func.h"
#include "Vector2.h"

#include "./Shapes/Line.h"
#include "./Shapes/Sphere.h"

const char kWindowTitle[] = "MT3_02_01_確認課題_LE2B_21_トダ_ユウスケ";

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
	
	Sphere sphere[2] = {
		{ {0.0f,0.0f,0.0f}, 0.5f },
		{ {1.0f,0.0f,-1.0f}, 0.2f },
	};

	unsigned int color = WHITE;

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

		if (IsCollision(sphere[0], sphere[1])) { color = RED; }
		else { color = WHITE; }

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		// グリッドを描画
		DrawGrid(viewProjectionMatrix, viewPortMatrix);

		DrawSphere(sphere[0], viewProjectionMatrix, viewPortMatrix, color);
		DrawSphere(sphere[1], viewProjectionMatrix, viewPortMatrix, WHITE);

		ImGui::Begin("Window");
		ImGui::DragFloat3("Sphere[0].center", &sphere[0].center.x, 0.01f, -2.0f, 2.0f, "%.3f");
		ImGui::DragFloat("Sphere[0].radius", &sphere[0].radius, 0.01f, 0.01f, 2.0f, "%.3f");
		ImGui::DragFloat3("Sphere[1].center", &sphere[1].center.x, 0.01f, -2.0f, 2.0f, "%.3f");
		ImGui::DragFloat("Sphere[1].radius", &sphere[1].radius, 0.01f, 0.01f, 2.0f, "%.3f");
		ImGui::End();

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