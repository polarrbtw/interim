#pragma once

// todo:
// Singleton this later

namespace Settings {

	namespace ESP {
		inline bool Enabled{ false };
		inline bool Box{ false };
		inline bool Roccat{ false };
		inline bool IgnoreDead{ true };
		inline bool IgnoreTeam{ false };
	}

	namespace CVAR {
		inline bool ThirdPerson{ false };
		inline bool NoSmoke{ false };
		inline bool NoJumpCooldown{ false };
	}

	namespace Aimbot {
		inline bool Enabled{ false };
	}

	/*
	namespace Debug {
		inline float width{ 30.0f };
		inline float height{ 125.0f };
	}
	*/

}