#include "esp.hpp"
#include <common.hpp>
#include <sdk/EntityManager.hpp>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>
#include <imgui_internal.h>

#include <thread>
#include <chrono>

void ESP::DrawPlayers() {
	if (!Settings::ESP::Enabled) return;

	std::vector<Entity> entities = EntityManager::GetEntities();
	if (entities.empty()) { return; }

	auto* localPlayer = EntityManager::GetLocalPlayer();
	if (!localPlayer) return;

	for (const auto& entity : entities) {
		if (entity.GetIndex() == localPlayer->GetIndex()) continue;
		if (!entity.IsValid()) continue; 

		if (Settings::ESP::IgnoreDead && !entity.IsAlive()) continue;
		if (Settings::ESP::IgnoreTeam && entity.GetTeam() == localPlayer->GetTeam()) continue;

		ImDrawList* draw = ImGui::GetBackgroundDrawList();

		Vec2 position{};
		if (!WorldToScreen(entity.GetPosition(), position)) continue;

		// scaling
		float distance = localPlayer->GetPosition().Distance(entity.GetPosition());
		float scale = 500.0f / distance;
		float width = 30.0f * scale;
		float height = 125.0f * scale;

		// prevent it from making the entity too big
		// if you're too close
		if (distance < 1.0f) distance = 1.0f;

		if (Settings::ESP::Roccat) {
			float radius = 6.0f * scale;
			draw->AddCircleFilled({position.x, position.y - height / 2}, radius, ORANGE, 0); // height / 2 for waist level
			
		}

		if (Settings::ESP::Box) {
			// p_min - top left ; p_max - bottom right
			draw->AddRect({ position.x - width, position.y - height}, { position.x + width, position.y }, MAGENTA, 0.0f, 0, 2.5f * scale);
		}

	}
}