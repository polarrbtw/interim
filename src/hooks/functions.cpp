#include "functions.hpp"
#include <globals.hpp>

oCG_GetUsernameX CG_GetUsernameX{ nullptr };
oCG_GetClantag CG_GetClantag{ nullptr };
oCom_IsLegacyServer Com_IsLegacyServer{ nullptr };

void fn::Setup() {
	CG_GetUsernameX = (oCG_GetUsernameX)(mem.cod4xBase + offsets::fn::CG_GetUsernameX);
	CG_GetClantag = (oCG_GetClantag)(mem.cod4xBase + offsets::fn::CG_GetClantag);
	Com_IsLegacyServer = (oCom_IsLegacyServer)(mem.cod4xBase + offsets::fn::Com_IsLegacyServer);
}

const char* fn::CG_GetClientName(int index) {
	int valid = *(unsigned long*)(0x4CC * index + 0x839270); // if ( *(_DWORD *)(0x4CC * i + 0x839270)
	if (!valid) return "unknown";

	if (Com_IsLegacyServer()) {
		return (const char*)(0x4CC * index + 0x83927C); //name = (char *)(0x4CC * i + 0x83927C);
	}
	else {
		return CG_GetUsernameX(index);
	}
}

const char* fn::CG_GetClientClantag(int index) {
	if (Com_IsLegacyServer()) { return ""; } // they dont have clan tags 
	else { return CG_GetClantag(index); }
}

// todo: remove colors from names like ^3 and allat for example