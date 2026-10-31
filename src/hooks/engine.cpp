#include "engine.hpp"
#include "sdk/offsets.hpp"
#include <globals.hpp>
#include <common.hpp>
#include <sdk/EntityManager.hpp>

oCG_GetUsernameX CG_GetUsernameX{ nullptr };
oCG_GetClantag CG_GetClantag{ nullptr };
oCom_IsLegacyServer Com_IsLegacyServer{ nullptr };
oRegisterTag RegisterTag{ nullptr };
oSendCommand SendCommand{ nullptr };
oWriteToConsole WriteToConsole{ nullptr };

// no need to have these functions in other files
namespace {
	void RemoveColorCodes(char* name) {
		for (size_t i = 0; name[i] != 0; ++i) {
			if (name[i] == '^') {
				name[i] = ' '; // remove ^
				name[i + 1] = ' '; // remove color code
			}
		}
	}

	void RegisterTags() {
		const char* BoneNames[] = {
		"j_helmet",
		"j_head",
		"neck",
		"j_spine4",
		"pelvis",
		"j_clavicle_le", "j_clavicle_ri",
		"j_shoulder_le", "j_shoulder_ri",
		"j_elbow_le", "j_elbow_ri",
		"j_wrist_le", "j_wrist_ri",
		"j_knee_le", "j_knee_ri",
		"j_ankle_le", "j_ankle_ri"
		};

		bool initialized{ true };

		for (size_t i = 0; i < BONE::COUNT; ++i) {
			bones.list[i] = RegisterTag(BoneNames[i], 1, (strlen(BoneNames[i]) + 1));
			if (!bones.list[i]) {
				initialized = false;
				info("bone: %d failed to initialize", bones.list[i]);
			}
		}

		if (initialized) {
			info("bone tags initialized");
		}
		else {
			error("bone tags failed to initialize");
		}
	}
} // namespace

void Engine::Setup() {
    // cod4x
	CG_GetUsernameX = reinterpret_cast<oCG_GetUsernameX>(Mem.Cod4xBase + offsets::engine::CG_GetUsernameX);
	CG_GetClantag = reinterpret_cast<oCG_GetClantag>(Mem.Cod4xBase + offsets::engine::CG_GetClantag);
	Com_IsLegacyServer = reinterpret_cast<oCom_IsLegacyServer>(Mem.Cod4xBase + offsets::engine::Com_IsLegacyServer);

	// game
	RegisterTag = reinterpret_cast<oRegisterTag>(offsets::engine::RegisterTag);
	SendCommand = reinterpret_cast<oSendCommand>(offsets::engine::SendCommandToConsole);
	WriteToConsole = reinterpret_cast<oWriteToConsole>(offsets::engine::WriteToConsole);

	info("engine functions initialized");

	RegisterTags();
}

void Engine::TestingCommands() {
    PrintConsole("Ran all test commands");
    SendCmd("jump_slowdownEnable 0");
    SendCmd("bg_fallDamageMinHeight 10000");
    SendCmd("player_sustainAmmo 1");
}

const char* Engine::CG_GetClientName(int index) {
	unsigned long valid = *reinterpret_cast<unsigned long*>(0x4CC * index + 0x839270); // if ( *(_DWORD *)(0x4CC * i + 0x839270)
	if (!valid) return "invalid";

	if (Com_IsLegacyServer()) {
		char* name = reinterpret_cast<char*>(0x4CC * index + 0x83927C); //name = (char *)(0x4CC * i + 0x83927C);
		RemoveColorCodes(name);
		return name;
	}
	else {
		char* name = CG_GetUsernameX(index);
		RemoveColorCodes(name);
		return name;
	}
}

const char* Engine::CG_GetClientClantag(int index) {
	if (Com_IsLegacyServer()) {
		return "";  // legacy servers dont have clan tags
	}
	else {
		char* clantag = CG_GetClantag(index);
		RemoveColorCodes(clantag);
		return clantag;
	}
}

// from moonbase
Vec3 Engine::GetTagPosition(WORD TagName, CEntity_t* CEntity, Vec3 Offset) {
   	// offset param is optional, need it for Box ESP
	int LocalClientNum{ 0 };
	float* Position;
	float _[3]{ 0.0f, 0.0f, 0.0f };
	Position = _;

	__asm
	{
		push	Position
		movzx	esi, TagName
		mov		ecx, CEntity
		movzx	eax, byte ptr[ecx + 4]
		mov		edi, 0x004024B0
		call	edi
		add		esp, 4
		mov		LocalClientNum, eax
	}

	return { Position[0] + Offset.x, Position[1] + Offset.y, Position[2] + Offset.z };
}

Vec2 Engine::GetPlayerTag(WORD TagName, CEntity_t* CEntity, Vec3 Offset) {
	Vec2 Screen{};
	Vec3 Position = GetTagPosition(TagName, CEntity, Offset);

	if (!WorldToScreen(Position, Screen)) return { 0.0f, 0.0f };

	return Screen;
}

bool Engine::GetClosestEnemy(Entity& out) {
	std::vector<Entity> EntityList = EntityManager::GetEntities();
	if (EntityList.empty()) return false;

	auto LocalPlayer = EntityManager::GetLocalPlayer();
	if (!LocalPlayer || !LocalPlayer->CEntity || !LocalPlayer->IsValid()) return false;

	Entity Closest{};
	float ClosestDistance = FLT_MAX;

	for (const auto& Entity : EntityList) {
		if (!Entity.IsValid() || !Entity.IsAlive()) continue;
		if (Entity.ClientInfo->Team == LocalPlayer->ClientInfo->Team) continue;

		float Distance = LocalPlayer->CEntity->Position.Distance(Entity.CEntity->Position);

		if (Distance < ClosestDistance) {
			ClosestDistance = Distance;
			Closest = Entity;
		}
	}

	if (!Closest.CEntity) return false;
	out = Closest;
	return true;
}

bool Engine::GetClosestEnemyFOV(Entity& out, int MaxDistance) {
    std::vector<Entity> EntityList = EntityManager::GetEntities();
	if (EntityList.empty()) return false;

	auto LocalPlayer = EntityManager::GetLocalPlayer();
	if (!LocalPlayer || !LocalPlayer->CEntity || !LocalPlayer->IsValid()) return false;

	Entity Closest{};
	float ClosestDistance = FLT_MAX;

	Vec2 ScreenCenter = GetScreenCenter();

	for (const auto& Entity : EntityList) {
		if (!Entity.IsValid() || !Entity.IsAlive()) continue;
		if (Entity.ClientInfo->Team == LocalPlayer->ClientInfo->Team) continue;

		CEntity_t* CEntity = Entity.CEntity;
		Vec2 HeadPosition = Engine::GetPlayerTag(bones.list[HEAD], CEntity);
		if (HeadPosition.IsZero()) continue;

		float Distance = ScreenCenter.Distance(HeadPosition);
		if (Distance > Settings::Aimbot::FOV) continue;
		if (Distance > MaxDistance) continue;

		if (Distance < ClosestDistance) {
			ClosestDistance = Distance;
			Closest = Entity;
		}
	}

	if (!Closest.CEntity) return false;
	out = Closest;
	return true;
}

int Engine::SendCmd(const char* command) {
    return SendCommand(0, 0, command);
}

int Engine::PrintConsole(const char *Format, ...) {
    char buffer[0x1000u];
    va_list va;
    va_start(va, Format);
    vsnprintf(buffer, sizeof(buffer), Format, va);
    va_end(va);

    return WriteToConsole(0, "%s", buffer);
}
