#pragma once
#include <sdk/Entity.hpp>

typedef unsigned short WORD;

namespace Engine {
	void Setup();
	void TestingCommands();

	const char* CG_GetClientName(int index);
	const char* CG_GetClientClantag(int index);

	// offset parameters are optional
	Vec3 GetTagPosition(WORD TagName, CEntity_t* CEntity, Vec3 Offset = {});
	// calls GetTagPosition and returns whatever WorldToScreen returns
	Vec2 GetPlayerTag(WORD TagName, CEntity_t* CEntity, Vec3 Offset = {});

	bool GetClosestEnemy(Entity& out);
	bool GetClosestEnemyFOV(Entity& out, int MaxDistance);

	int SendCmd(const char* command);
	int PrintConsole(const char* Format, ...);
} // namespace Engine

using oCG_GetUsernameX = char* (*)(int clnum);
using oCG_GetClantag = char* (*)(int clnum);
using oCom_IsLegacyServer = int (*)();
using oRegisterTag = int (*)(const char* name, int type, size_t length);
using oSendCommand = int (*)(int a1, int a2, const char* command);
using oWriteToConsole = int (*)(int a1, const char* Format, ...);

extern oCG_GetUsernameX CG_GetUsernameX;
extern oCG_GetClantag CG_GetClantag;
extern oCom_IsLegacyServer Com_IsLegacyServer;
extern oRegisterTag RegisterTag;
extern oSendCommand SendCommand;
extern oWriteToConsole WriteToConsole;
