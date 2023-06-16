#include <Novice.h>
#include <ImGuiManager.h>
#include "Matrix4x4Func.h"
#include "Vector2.h"

#include "Line.h"


const char kWindowTitle[] = "MT3_02_00_確認課題_LE2B_21_トダ_ユウスケ";

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
	
	// 線分
	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
	// 点
	Vector3 point{ -1.5f,0.6f,0.6f };
	Sphere pointSphere{ point, 0.01f }; // 1cmの球

	// 正射影ベクトル
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	// 最近接点
	Vector3 closestPoint = ClosestPoint(point, segment);
	Sphere closestPointSphere{ closestPoint, 0.01f }; // 1cmの球


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

		Vector3 start = TransformCoord(TransformCoord(segment.origin, viewProjectionMatrix), viewPortMatrix);
		Vector3 end = TransformCoord(TransformCoord(Add(segment.origin, segment.diff), viewProjectionMatrix), viewPortMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		// グリッドを描画
		DrawGrid(viewProjectionMatrix, viewPortMatrix);

		DrawSphere(pointSphere, viewProjectionMatrix, viewPortMatrix, RED);
		DrawSphere(closestPointSphere, viewProjectionMatrix, viewPortMatrix, BLACK);

		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		ImGui::Begin("Window");
		ImGui::InputFloat3("Point", &point.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Segment origin", &segment.origin.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Segment diff", &segment.diff.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
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