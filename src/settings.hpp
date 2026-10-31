#pragma once

// todo:
// Singleton this later

namespace Settings {

	namespace ESP {
	    inline bool Enabled{ false };

        inline bool Box{ false };
        inline bool Name{ false };
        inline bool Bone{ false };
        inline bool State{ false }; // moving, running, scoped in and similar
        inline bool Distance{ false };
        inline bool LookingAtYou{ false };
        inline bool Radar{ false };

        inline bool IgnoreDead{ true };
        inline bool IgnoreTeam{ false };

        // Colors - 1.0f, 1.0f, 1.0f is white
        inline float BoxColor[3]        = { 1.0f, 1.0f, 1.0f };
        inline float BoneColor[3]       = { 1.0f, 1.0f, 1.0f };
        inline float NameColor[3]       = { 1.0f, 1.0f, 1.0f };
        inline float StateColor[3]      = { 1.0f, 1.0f, 1.0f };
        inline float DistanceColor[3]   = { 1.0f, 1.0f, 1.0f };
        inline float StrokeColor[3]     = { 0.0f, 0.0f, 0.0f };
	} // namespace ESP

	namespace Cvar {
        inline bool CG_Thirdperson{ false }; // self explanatory
        inline bool FX_Draw{ false }; // no smoke
        inline bool Jump_Slowdown{ false }; // disable slowdown once you land after jumping
        inline bool CompassShowEnemies{ false }; // show enemies on radar
	} // namespace Cvar

	namespace Aimbot {
        inline bool Enabled{ false };

        inline bool FOVCircle{ false };
        inline bool VisibleOnly{ false };
		inline int MaxDistance{ 50 };
		inline float FOV{ 100 };
	} // namespace Aimbot

	namespace SilentAim {
	    inline bool Enabled{ false };

		inline bool FOVCircle{ false };
		inline int MaxDistance{ 50 };
		inline float FOV { 50 };
	}

	namespace Triggerbot {
	    inline bool Enabled{ false };

		inline int Delay{ 250 };
	}

#ifdef _DEBUG
	namespace Debug {
        inline float Width{ 30.0f };
        inline float Height{ 125.0f };
        //inline Vec3 OffsetLeg{ 0.0f, 0.0f, 0.0f };
	}
#endif

} // namespace Settings
