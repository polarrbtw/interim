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
	  inline bool Roccat{ false };

	  inline bool IgnoreDead{ true };
	  inline bool IgnoreTeam{ false };
	} // namespace ESP

	namespace Cvar {
	  inline bool ThirdPerson{ false };
	  inline bool NoSmoke{ false };
	  inline bool NoJumpCooldown{ false };
	} // namespace Cvar

	namespace Aimbot {
	  inline bool Enabled{ false };
	} // namespace Aimbot

#ifdef _DEBUG
	namespace Debug {
	  inline float Width{ 30.0f };
	  inline float Height{ 125.0f };
	}
#endif

} // namespace Settings