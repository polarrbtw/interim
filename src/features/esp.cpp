#include "esp.hpp"
#include <common.hpp>
#include <sdk/EntityManager.hpp>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>
#include <imgui_internal.h>

#include <thread>
#include <chrono>

namespace {
	void ConnectBone(ImDrawList* draw, ImVec2 bone1, ImVec2 bone2) {
		draw->AddLine(bone1, bone2, WHITE, 1.0f);
	}
}

void ESP::DrawPlayers() {
	if (!globals->Running) return;
	if (!Settings::ESP::Enabled) return;

	std::vector<Entity> EntityList = EntityManager::GetEntities();
	if (EntityList.empty()) { return; }

	auto* LocalPlayer = EntityManager::GetLocalPlayer();
	if (!LocalPlayer) return;

	for (const auto& Entity : EntityList) {
		if (Entity.GetIndex() == LocalPlayer->GetIndex()) continue;
		if (!Entity.IsValid()) continue; 

		if (Settings::ESP::IgnoreDead && !Entity.IsAlive()) continue;
		if (Settings::ESP::IgnoreTeam && Entity.GetTeam() == LocalPlayer->GetTeam()) continue;

		Vec2 Position{};
		if (!WorldToScreen(Entity.GetPosition(), Position)) continue;

		ImDrawList* draw = ImGui::GetBackgroundDrawList();

		CEntity_t* cent = reinterpret_cast<CEntity_t*>(offsets::CEntity + (Entity.GetIndex() * sizeof(CEntity_t)));

		// scaling
		float Distance = LocalPlayer->GetPosition().Distance(Entity.GetPosition());

		// prevent it from making the Entity too big
		// if you're too close
		if (Distance < 1.0f) Distance = 1.0f;

		float Scale = 500.0f / Distance;
		float Width = 30.0f * Scale;
		float Height = 125.0f * Scale;

		if (Settings::ESP::Roccat) {
			float Radius = 6.0f * Scale;
			draw->AddCircleFilled(Engine::GetPlayerTag(bones.list[PELVIS], cent), Radius, ORANGE, 0);
		}

		// fix: when scoped in / ads'd, box becomes smaller
		// todo: replace with bones
		if (Settings::ESP::Box) {
			// p_min - top left ; p_max - bottom right
			draw->AddRect({ Position.x - Width, Position.y - Height }, { Position.x + Width, Position.y }, MAGENTA, 0.0f, 0, 2.5f * Scale);
		}

		if (Settings::ESP::Bone) {
			if (!cent || !cent->IsValidEntity) {
				continue;
			}

			// upper
			Vec2 Head = Engine::GetPlayerTag(bones.list[HEAD], cent);
			Vec2 Neck = Engine::GetPlayerTag(bones.list[NECK], cent);
			Vec2 Spine = Engine::GetPlayerTag(bones.list[SPINE], cent);

			// arms
			Vec2 LeftClavicle = Engine::GetPlayerTag(bones.list[LEFTCLAVICLE], cent);
			Vec2 LeftElbow = Engine::GetPlayerTag(bones.list[LEFTELBOW], cent);
			Vec2 LeftWrist = Engine::GetPlayerTag(bones.list[LEFTWRIST], cent);
			Vec2 RightClavicle = Engine::GetPlayerTag(bones.list[RIGHTCLAVICLE], cent);
			Vec2 RightElbow = Engine::GetPlayerTag(bones.list[RIGHTELBOW], cent);
			Vec2 RightWrist = Engine::GetPlayerTag(bones.list[RIGHTWRIST], cent);
			
			// lower
			Vec2 Pelvis = Engine::GetPlayerTag(bones.list[PELVIS], cent);
			Vec2 LeftAnkle = Engine::GetPlayerTag(bones.list[LEFTANKLE], cent);
			Vec2 LeftKnee = Engine::GetPlayerTag(bones.list[LEFTKNEE], cent);
			Vec2 RightAnkle = Engine::GetPlayerTag(bones.list[RIGHTANKLE], cent);
			Vec2 RightKnee = Engine::GetPlayerTag(bones.list[RIGHTKNEE], cent);

			ConnectBone(draw, Head, Neck);
			ConnectBone(draw, Neck, Spine);
			ConnectBone(draw, Spine, LeftClavicle);
			ConnectBone(draw, LeftClavicle, LeftElbow);
			ConnectBone(draw, LeftElbow, LeftWrist);
			ConnectBone(draw, Spine, RightClavicle);
			ConnectBone(draw, RightClavicle, RightElbow);
			ConnectBone(draw, RightElbow, RightWrist);
			ConnectBone(draw, Spine, Pelvis);

			ConnectBone(draw, Pelvis, LeftKnee);
			ConnectBone(draw, LeftKnee, LeftAnkle);
			ConnectBone(draw, Pelvis, RightKnee);
			ConnectBone(draw, RightKnee, RightAnkle);
		}

		if (Settings::ESP::Name) {
			const char* Name = Entity.GetName();
			Vec2 LeftAnkle = Engine::GetPlayerTag(bones.list[LEFTANKLE], cent);
			Vec2 RightAnkle = Engine::GetPlayerTag(bones.list[RIGHTANKLE], cent);

			Vec2 Midpoint = LeftAnkle.Midpoint(RightAnkle);

			ImVec2 TextSize = ImGui::CalcTextSize(Name);
			Vec2 CenteredText = { Midpoint.x - (TextSize.x * 0.5f), Midpoint.y }; // without * 0.5f it goes too much to the left

			draw->AddText(CenteredText, WHITE, Name);
		}
	}
}