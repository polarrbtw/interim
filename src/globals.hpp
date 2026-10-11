#pragma once
#include <util/memory.hpp>
#include <sdk/offsets.hpp>
#include <sdk/structs.hpp>
#include <hooks/engine.hpp>

class Globals {
public:
	int WindowHeight{ 0 };
	int WindowWidth{ 0 };
	int FrameCounter{ 0 }; // for hiding esp in hk_ScreenshotRequest

	bool IsInGame{ false };
	bool Running{ true };

	Refdef_t Refdef{};

	void Update() {
		WindowHeight = Mem.raw<int>(offsets::ScreenHeight);
		WindowWidth = Mem.raw<int>(offsets::ScreenWidth);
		IsInGame = Mem.raw<bool>(offsets::IsInGame);
		Refdef = Mem.raw<Refdef_t>(offsets::Refdef);
	}
};

inline Globals* globals = new Globals();
inline Bones_t bones;