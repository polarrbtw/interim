#pragma once

namespace fn {
	void Setup();

	const char* CG_GetClientName(int index);
	const char* CG_GetClientClantag(int index);
}

using oCG_GetUsernameX = char* (*)(int clnum);
using oCG_GetClantag = char* (*)(int clnum);
using oCom_IsLegacyServer = int(*)();

extern oCG_GetUsernameX CG_GetUsernameX;
extern oCG_GetClantag CG_GetClantag;
extern oCom_IsLegacyServer Com_IsLegacyServer;