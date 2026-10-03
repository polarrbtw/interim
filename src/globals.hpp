#pragma once
#include <util/memory.hpp>
#include <sdk/offsets.hpp>
#include <sdk/structs.hpp>

class Globals {
public:
	int WindowHeight{ 0 };
	int WindowWidth{ 0 };
	int FrameCounter{ 0 }; // for hiding esp in hk_SnapshotRequest

	bool IsInGame{ false };
	bool Running{ true };

	Refdef_t refdef{};

	void Update() {
		WindowHeight = mem.raw<int>(offsets::ScreenHeight);
		WindowWidth = mem.raw<int>(offsets::ScreenWidth);
		IsInGame = mem.raw<bool>(offsets::IsInGame);
		refdef = mem.raw<Refdef_t>(offsets::Refdef);
	}
};

inline Globals* globals = new Globals();