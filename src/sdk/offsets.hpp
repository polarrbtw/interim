#pragma once
#include <cstdint>

namespace offsets {
	inline constexpr uintptr_t CG = 0x0074E338; // ClientGame
	inline constexpr uintptr_t CGS = 0x0074A908; // ClientGameServer
	inline constexpr uintptr_t Refdef = 0x00797600;

	inline constexpr uintptr_t ClientInfo = 0x00839270;
	inline constexpr uintptr_t CEntity = 0x0084F2D8;

	// other
	inline constexpr uintptr_t IsInGame = 0x0074E35C;
	inline constexpr uintptr_t ScreenWidth = 0x00797608;
	inline constexpr uintptr_t ScreenHeight = 0x0079760C;
	inline constexpr uintptr_t FOV = 0xC7AF1AC; // iw3mp.exe+C7AF1AC

	// screenshotrequest in ida - 0x64E53EE0 ; image base - 0x64D00000
	namespace fn {
		inline constexpr uintptr_t CL_ParseSnapshot = 0x473710; // in iw3mp.exe
		inline constexpr uintptr_t ScreenshotRequest = 0x154010; // cod4x_021.dll+154010
		inline constexpr uintptr_t CG_GetUsernameX = 0x42800; // cod4x_021.dll+0x42800
		inline constexpr uintptr_t CG_GetClantag = 0x42810; // cod4x_021.dll+0x42810
		inline constexpr uintptr_t Com_IsLegacyServer = 0x6CFF0; // cod4x_021.dll+0x6CFF0
	}

}

namespace cvars {
	inline constexpr uintptr_t fx_draw = 0xC7C5E88; // iw3mp.exe+C7C5E88 <-- mem.write<BYTE>(fx_draw, 0) to turn it on, 1 for off
	inline constexpr uintptr_t cg_thirdperson = 0xC7B828C; // iw3mp.exe+C7B828C
	inline constexpr uintptr_t jump_slowdown = 0xC7BC4C0; // iw3mp.exe+C7BC4C0
}