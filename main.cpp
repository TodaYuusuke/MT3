#include <Novice.h>
#include <ImGuiManager.h>
#include "Matrix4x4Func.h"
#include "Vector2Func.h"

#include "./Shapes/Line.h"
#include "./Shapes/Sphere.h"
#include "./Shapes/ShapeCollision.h"

const char kWindowTitle[] = "MT3_02_04_確認課題_LE2B_21_トダ_ユウスケ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// マウス座標
	Vector2 mousePosition{ 0.0f,0.0f };
	// 前フレームのマウス座標
	Vector2 preMousePosition{ 0.0f,0.0f };

	// カメラ座標
	Vector3 cameraPosition{ 0.0f,1.9f,-6.49f };
	// カメラ角度
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	// 三角形
	Triangle triangle{ .vertices{{ -1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f }} };
	// 線分
	Segment segment{ .origin{-0.450f,0.420f,-1.0f }, .diff{0.0f,0.5f,2.0f} };
	// 色
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
		
		// カメラ操作

		// 回転 //
		mousePosition = GetMousePosition();
		if (Novice::IsPressMouse(1)) {
			cameraRotate.x += (mousePosition.y - preMousePosition.y) * 0.001f;
			cameraRotate.y += (mousePosition.x - preMousePosition.x) * 0.001f;
		}
		preMousePosition = mousePosition;

		// カメラの移動法線ベクトル
		Matrix4x4 rMatrix = MakeRotateXYZMatrix(cameraRotate);
		Vector3 cameraMove = { 0.0f,0.0f,0.1f };
		cameraMove = Multiply(rMatrix, cameraMove);

		// 移動 // 
		if (keys[DIK_W]) {
			cameraPosition = Add(cameraPosition, cameraMove);
		}
		if (keys[DIK_S]) {
			cameraPosition = Subtract(cameraPosition, cameraMove);
		}
		cameraMove = { 0.0f,0.0f,0.1f };
		cameraMove = Multiply(MakeRotateXYZMatrix({0.0f,1.57f,0.0f,}), cameraMove);
		cameraMove = Multiply(rMatrix, cameraMove);
		if (keys[DIK_D]) {
			cameraPosition = Add(cameraPosition, cameraMove);
		}
		if (keys[DIK_A]) {
			cameraPosition = Subtract(cameraPosition, cameraMove);
		}

		if (keys[DIK_LCONTROL]) {
			cameraPosition.y -= 0.1f;
		}
		if (keys[DIK_SPACE]) {
			cameraPosition.y += 0.1f;
		}

		// 各行列の計算
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);;
		Matrix4x4 viewPortMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		if (IsCollision(triangle, segment)) { color = RED; }
		else { color = WHITE; }

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		// グリッドを描画
		DrawGrid(viewProjectionMatrix, viewPortMatrix);

		DrawLine(segment, viewProjectionMatrix, viewPortMatrix, color);
		DrawTriangle(triangle, viewProjectionMatrix, viewPortMatrix, WHITE);

		ImGui::Begin("Window");
		ImGui::DragFloat3("Triangle.vertices[0]", &triangle.vertices[0].x, 0.01f, {}, {}, "%.3f");
		ImGui::DragFloat3("Triangle.vertices[1]", &triangle.vertices[1].x, 0.01f, {}, {}, "%.3f");
		ImGui::DragFloat3("Triangle.vertices[2]", &triangle.vertices[2].x, 0.01f, {}, {}, "%.3f");
		ImGui::DragFloat3("segment.origin", &segment.origin.x, 0.01f, {}, {}, "%.3f");
		ImGui::DragFloat3("segment.diff", &segment.diff.x, 0.01f, {}, {}, "%.3f");
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