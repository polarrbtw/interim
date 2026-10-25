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

	// this function sleeps every time for 20ms
	void Update() {
		WindowHeight = Mem.Raw<int>(offsets::ScreenHeight);
		WindowWidth = Mem.Raw<int>(offsets::ScreenWidth);
		IsInGame = Mem.Raw<bool>(offsets::IsInGame);
		// removed refdef
	}
};

inline Globals* globals = new Globals();
inline Bones_t bones;
