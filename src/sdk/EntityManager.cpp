#include "EntityManager.hpp"
#include <common.hpp>
#include <hooks/functions.hpp>

#define SV_MAXCLIENTS 32

void EntityManager::UpdateEntities() {
	if (!globals->IsInGame) return;

	UpdateLocalPlayer();

	EntityManager::entities.clear();
	for (int i = 0; i <= SV_MAXCLIENTS; ++i) {
		CEntity_t cent = mem.raw<CEntity_t>(offsets::CEntity + i * sizeof(CEntity_t));
		ClientInfo_t cinfo = mem.raw<ClientInfo_t>(offsets::ClientInfo + i * sizeof(ClientInfo_t));
		
		Entity entity{};
		//entity.SetInfo(cent, cinfo, i);
		const char* name = fn::CG_GetClientName(entity.GetIndex());
		const char* clantag = fn::CG_GetClientClantag(entity.GetIndex());
		entity.SetInfo(cent, cinfo, name, clantag, i);

		//printf("name: %s\n", name);

		if (!entity.IsValid()) continue;
		if (entity.GetIndex() == EntityManager::GetLocalPlayer()->GetIndex()) continue;

		EntityManager::entities.push_back(entity);
	}
}

void EntityManager::UpdateLocalPlayer() {
	int i = mem.raw<int>(offsets::CG); // localplayer index

	CEntity_t cent = mem.raw<CEntity_t>(offsets::CEntity + i * sizeof(CEntity_t));
	ClientInfo_t cinfo = mem.raw<ClientInfo_t>(offsets::ClientInfo + i * sizeof(ClientInfo_t));
	float FOV = mem.read<float>(offsets::FOV);

	const char* name = CG_GetUsernameX(i);
	const char* clantag = CG_GetClantag(i);

	localPlayer.SetInfo(cent, cinfo, name, clantag, i);

	//printf("localplayer: %s\n clantag: %s\n", name, clantag);
}

const LocalPlayer *EntityManager::GetLocalPlayer() { return &localPlayer; }

const std::vector<Entity> &EntityManager::GetEntities() { return entities; }
