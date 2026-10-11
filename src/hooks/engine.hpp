#pragma once
#include <sdk/Entity.hpp>

typedef unsigned short WORD;

namespace Engine {
	void Setup();

	const char* CG_GetClientName(int index);
	const char* CG_GetClientClantag(int index);

	Vec2 GetPlayerTag(WORD TagName, CEntity_t* Entity);

	void RegisterTags();
} // namespace Engine

using oCG_GetUsernameX = char* (*)(int clnum);
using oCG_GetClantag = char* (*)(int clnum);
using oCom_IsLegacyServer = int (*)();
using oRegisterTag = int (*)(const char* name, int type, size_t length);

extern oCG_GetUsernameX CG_GetUsernameX;
extern oCG_GetClantag CG_GetClantag;
extern oCom_IsLegacyServer Com_IsLegacyServer;
extern oRegisterTag RegisterTag;