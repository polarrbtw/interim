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

		Vec2 Position{};
		if (!WorldToScreen(Entity.GetPosition(), Position)) continue;

		if (Settings::ESP::IgnoreDead && !Entity.IsAlive()) continue;
		if (Settings::ESP::IgnoreTeam && Entity.GetTeam() == LocalPlayer->GetTeam()) continue;

		ImDrawList* draw = ImGui::GetBackgroundDrawList();

		CEntity_t* cent = reinterpret_cast<CEntity_t*>(offsets::CEntity + (Entity.GetIndex() * sizeof(CEntity_t)));

		float Distance = LocalPlayer->GetPosition().Distance(Entity.GetPosition());

		// prevent it from getting too big
		// if you're too close
		if (Distance < 1.0f) Distance = 1.0f;

		// scaling
		float Scale = 500.0f / Distance;
		float Width = 30.0f * Scale;
		float Height = 125.0f * Scale;

		if (Settings::ESP::Box) {
			// p_min - top left ; p_max - bottom right
			Vec2 Head = Engine::GetPlayerTag(bones.list[HEAD], cent, { 0.0f, 0.0f, 11.0f });
			Vec2 Foot = Engine::GetPlayerTag(bones.list[LEFTFOOT], cent, { 0.0f, 0.0f, -8.0f });

			// bounding box
			float BoxHeight = abs(Head.y - Foot.y);
			float BoxWidth = BoxHeight / 2.0f;

			Vec2 TopLeft = { Head.x - (BoxWidth / 2.0f), Head.y };
			Vec2 BottomRight = { Head.x + (BoxWidth / 2.0f), Foot.y };

			// stroke
			draw->AddRect({ TopLeft.x - 1.0f, TopLeft.y - 1.0f }, { BottomRight.x + 1.0f, BottomRight.y + 1.0f }, BLACK, 0.0f, 0, 2.0f);
			// box
			draw->AddRect(TopLeft, BottomRight, WHITE, 0.0f, 0, 1.5f);
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
			Vec2 LeftFoot = Engine::GetPlayerTag(bones.list[LEFTFOOT], cent);
			Vec2 LeftKnee = Engine::GetPlayerTag(bones.list[LEFTKNEE], cent);
			Vec2 RightFoot = Engine::GetPlayerTag(bones.list[RIGHTFOOT], cent);
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
			ConnectBone(draw, LeftKnee, LeftFoot);
			ConnectBone(draw, Pelvis, RightKnee);
			ConnectBone(draw, RightKnee, RightFoot);
		}

		if (Settings::ESP::Name) {
			const char* Name = Entity.GetName();
			Vec2 LeftFoot = Engine::GetPlayerTag(bones.list[LEFTFOOT], cent);
			Vec2 RightFoot = Engine::GetPlayerTag(bones.list[RIGHTFOOT], cent);

			Vec2 Midpoint = LeftFoot.Midpoint(RightFoot);

			ImVec2 TextSize = ImGui::CalcTextSize(Name);
			Vec2 CenteredText = { Midpoint.x - (TextSize.x * 0.5f), Midpoint.y }; // without * 0.5f it goes too much to the left

			// stroke
			draw->AddText({ CenteredText.x + 1.0f, CenteredText.y - 1.0f}, BLACK, Name);
			draw->AddText({ CenteredText.x - 1.0f, CenteredText.y + 1.0f }, BLACK, Name);

			// name
			draw->AddText(CenteredText, WHITE, Name);
		}

		if (Settings::ESP::Distance) {
			char PlayerDistance[64]{};
			int _ = snprintf(PlayerDistance, sizeof(PlayerDistance), "%.0fm", Distance / 40.0f);

			Vec2 LeftFoot = Engine::GetPlayerTag(bones.list[LEFTFOOT], cent);
			Vec2 RightFoot = Engine::GetPlayerTag(bones.list[RIGHTFOOT], cent);

			Vec2 Midpoint = LeftFoot.Midpoint(RightFoot);

			ImVec2 TextSize = ImGui::CalcTextSize(PlayerDistance);
			Vec2 CenteredText{};
			if (Settings::ESP::Name) {
				CenteredText = { Midpoint.x - (TextSize.x * 0.5f), Midpoint.y + 10.0f };
			}
			else {
				CenteredText = { Midpoint.x - (TextSize.x * 0.5f), Midpoint.y };
			}
			
			// stroke
			draw->AddText({ CenteredText.x + 1.0f, CenteredText.y - 1.0f }, BLACK, PlayerDistance);
			draw->AddText({ CenteredText.x - 1.0f, CenteredText.y + 1.0f }, BLACK, PlayerDistance);

			// distance
			draw->AddText(CenteredText, WHITE, PlayerDistance);
		}
	}
}