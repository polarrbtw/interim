#pragma once
#include <util/math.hpp>
#include <sdk/EntityManager.hpp>

namespace Aimbot {
	inline int TargetIndex = -1;
	inline Entity TargetEntity{};

	void RunAimbot();
	void DrawFOVCircle();
	Vector2 CalculateAngles(Vector3 from, Vector3 to);
}
