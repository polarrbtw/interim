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
	inline constexpr uintptr_t FOV = 0x00C7AF1AC; // iw3mp.exe+C7AF1AC
	inline constexpr uintptr_t ViewAngles = 0x00C84FD8;

	// screenshotrequest in ida - 0x64E53EE0 ; image base - 0x64D00000
	namespace engine {
		inline constexpr uintptr_t CL_ParseSnapshot = 0x00473710; // iw3mp.exe+73710
		inline constexpr uintptr_t AimTarget_GetTagPos = 0x004024B0; // iw3mp.exe+3470 0x00403470
		inline constexpr uintptr_t RegisterTag = 0x00518290; // iw3mp.exe+118290
		inline constexpr uintptr_t Com_Error = 0x004FD330; // iw3mp.exe+FD330
		inline constexpr uintptr_t SendCommandToConsole = 0x004F9AB0; // iw3mp.exe+F9AB0
		inline constexpr uintptr_t WriteToConsole = 0x004FCBC0; // iw3mp.exe+FCBC0

		inline constexpr uintptr_t ScreenshotRequest = 0x00154010; // cod4x_021.dll+154010
		inline constexpr uintptr_t CG_GetUsernameX = 0x00042800; // cod4x_021.dll+0x42800
		inline constexpr uintptr_t CG_GetClantag = 0x00042810; // cod4x_021.dll+0x42810
		inline constexpr uintptr_t Com_IsLegacyServer = 0x0006CFF0; // cod4x_021.dll+0x6CFF0
	} // namespace engine

} // namespace offset

namespace cvars {
	inline constexpr uintptr_t FX_Draw = 0xC7C5E88; // iw3mp.exe+C7C5E88 <-- mem.write<BYTE>(fx_draw, 0) to turn it on, 1 for off
	inline constexpr uintptr_t CG_Thirdperson = 0xC7B828C; // iw3mp.exe+C7B828C
	inline constexpr uintptr_t Jump_Slowdown = 0xC7BC4C0; // iw3mp.exe+C7BC4C0
	inline constexpr uintptr_t CompassShowEnemies = 0xC7BA594; // iw3mp.exe+C7BA594

	// other
	inline constexpr uintptr_t MaxFPS = 0xC7AB8F8; // iw3mp.exe+C7AB8F8
} // namespace cvars
