#pragma once
#include "Vector2.h"
#include <Novice.h>

/// <summary>
/// マウス座標を取得する
/// </summary>
/// <returns>マウス座標</returns>
Vector2 GetMousePosition() {
	int x{}, y{};
	Novice::GetMousePosition(&x, &y);
	return { (float)x,(float)y };
}
