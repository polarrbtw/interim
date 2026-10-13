#include "engine.hpp"
#include <globals.hpp>
#include <common.hpp>

oCG_GetUsernameX CG_GetUsernameX{ nullptr };
oCG_GetClantag CG_GetClantag{ nullptr };
oCom_IsLegacyServer Com_IsLegacyServer{ nullptr };
oRegisterTag RegisterTag{ nullptr };

namespace {
	void RemoveColorCodes(char* name) {
		for (size_t i = 0; name[i] != 0; ++i) {
			if (name[i] == '^') {
				name[i] = ' '; // remove ^
				name[i + 1] = ' '; // remove color code
			}
		}
	}
} // namespace 

void Engine::Setup() {
	CG_GetUsernameX = (oCG_GetUsernameX)(Mem.Cod4xBase + offsets::engine::CG_GetUsernameX);
	CG_GetClantag = (oCG_GetClantag)(Mem.Cod4xBase + offsets::engine::CG_GetClantag);
	Com_IsLegacyServer = (oCom_IsLegacyServer)(Mem.Cod4xBase + offsets::engine::Com_IsLegacyServer);

	RegisterTag = (oRegisterTag)(offsets::engine::RegisterTag);

	info("engine functions initialized");

	RegisterTags();
}

const char* Engine::CG_GetClientName(int index) {
	int valid = *(unsigned long*)(0x4CC * index + 0x839270); // if ( *(_DWORD *)(0x4CC * i + 0x839270)
	if (!valid) return "invalid";

	if (Com_IsLegacyServer()) {
		char* name = (char*)(0x4CC * index + 0x83927C); //name = (char *)(0x4CC * i + 0x83927C);
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
Vec2 Engine::GetPlayerTag(WORD TagName, CEntity_t* Entity, Vec3 Offset) {
	// Offset param is optional, need it for Box ESP
	int LocalClientNum{ 0 };
	float* Position;
	float _[3]{ 0.0f, 0.0f, 0.0f };
	Position = _;

	__asm
	{
		push	Position
		movzx	esi, TagName
		mov		ecx, Entity
		movzx	eax, byte ptr[ecx + 4]
		mov		edi, 0x004024B0
		call	edi
		add		esp, 4
		mov		LocalClientNum, eax
	}

	Vec2 screen{};
	Vec3 tmp{ Position[0] + Offset.x, Position[1] + Offset.y, Position[2] + Offset.z };
	if (!WorldToScreen(tmp, screen)) return { 0.0f, 0.0f };
	
	return screen;
}

void Engine::RegisterTags() {
	const char* boneNames[] = {
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
		bones.list[i] = RegisterTag(boneNames[i], 1, (strlen(boneNames[i]) + 1));
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