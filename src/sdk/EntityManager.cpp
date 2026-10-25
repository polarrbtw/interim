#include "EntityManager.hpp"
#include <common.hpp>
#include <hooks/engine.hpp>

#define SV_MAXCLIENTS 32

void EntityManager::UpdateEntities() {
	if (!globals->IsInGame) return;

	UpdateLocalPlayer();

	EntityManager::EntityList.clear();
	for (int index = 0; index <= SV_MAXCLIENTS; ++index) {
		CEntity_t* cent = Mem.Ptr<CEntity_t*>(offsets::CEntity + index * sizeof(CEntity_t));
		ClientInfo_t* cinfo = Mem.Ptr<ClientInfo_t*>(offsets::ClientInfo + index * sizeof(ClientInfo_t));

		Entity entity{};
		entity.CEntity = cent;
		entity.ClientInfo = cinfo;
		entity.Index = index;
		entity.Name = Engine::CG_GetClientName(index);
		entity.Clantag = Engine::CG_GetClientClantag(index);

		if (!entity.IsValid()) continue;
		if (entity.Index == EntityManager::GetLocalPlayer()->Index) continue;

		EntityManager::EntityList.push_back(entity);
	}
}

void EntityManager::UpdateLocalPlayer() {
	int index = Mem.Raw<int>(offsets::CG); // localplayer index

	CEntity_t* cent = Mem.Ptr<CEntity_t*>(offsets::CEntity + index * sizeof(CEntity_t));
	ClientInfo_t* cinfo = Mem.Ptr<ClientInfo_t*>(offsets::ClientInfo + index * sizeof(ClientInfo_t));
	float FOV = Mem.Read<float>(offsets::FOV);

	const char* name = CG_GetUsernameX(index);
	const char* clantag = CG_GetClantag(index);

	LocalPlayer.CEntity = cent;
	LocalPlayer.ClientInfo = cinfo;
	LocalPlayer.Index = index;
	LocalPlayer.Name = name;
	LocalPlayer.Clantag = clantag;
	LocalPlayer.FOV = FOV;
}

LocalEntity *EntityManager::GetLocalPlayer() { return &LocalPlayer; }

const std::vector<Entity> &EntityManager::GetEntities() { return EntityList; }
