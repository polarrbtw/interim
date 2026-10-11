#include "EntityManager.hpp"
#include <common.hpp>
#include <hooks/engine.hpp>

#define SV_MAXCLIENTS 32

void EntityManager::UpdateEntities() {
	if (!globals->IsInGame) return;

	UpdateLocalPlayer();

	EntityManager::EntityList.clear();
	for (int index = 0; index <= SV_MAXCLIENTS; ++index) {
		CEntity_t cent = Mem.raw<CEntity_t>(offsets::CEntity + index * sizeof(CEntity_t));
		ClientInfo_t cinfo = Mem.raw<ClientInfo_t>(offsets::ClientInfo + index * sizeof(ClientInfo_t));
		
		Entity entity{};
		const char* name = Engine::CG_GetClientName(index);
		const char* clantag = Engine::CG_GetClientClantag(index);
		entity.SetInfo(cent, cinfo, name, clantag, index);

		if (!entity.IsValid()) continue;
		if (entity.GetIndex() == EntityManager::GetLocalPlayer()->GetIndex()) continue;

		EntityManager::EntityList.push_back(entity);
	}
}

void EntityManager::UpdateLocalPlayer() {
	int index = Mem.raw<int>(offsets::CG); // localplayer index

	CEntity_t cent = Mem.raw<CEntity_t>(offsets::CEntity + index * sizeof(CEntity_t));
	ClientInfo_t cinfo = Mem.raw<ClientInfo_t>(offsets::ClientInfo + index * sizeof(ClientInfo_t));
	float FOV = Mem.read<float>(offsets::FOV);

	const char* name = CG_GetUsernameX(index);
	const char* clantag = CG_GetClantag(index);

	LocalPlayer.SetInfo(cent, cinfo, name, clantag, index);

}

const LocalEntity *EntityManager::GetLocalPlayer() { return &LocalPlayer; }

const std::vector<Entity> &EntityManager::GetEntities() { return EntityList; }
