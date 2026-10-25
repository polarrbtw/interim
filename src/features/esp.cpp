#include "esp.hpp"
#include "settings.hpp"
#include <common.hpp>
#include <sdk/EntityManager.hpp>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>
#include <imgui_internal.h>

#include <thread>
#include <chrono>

namespace {
	void ConnectBone(ImDrawList* draw, Vec2 bone1, Vec2 bone2) {
	    if (bone1.IsZero() || bone2.IsZero()) return;

		draw->AddLine(bone1, bone2, ToU32(Settings::ESP::BoneColor), 1.0f);
	}
}

void ESP::DrawPlayers() {
	if (!globals->Running) return;
	if (!Settings::ESP::Enabled) return;

	const auto& EntityList = EntityManager::GetEntities();
	if (EntityList.empty()) { return; }

	auto* LocalPlayer = EntityManager::GetLocalPlayer();
	if (!LocalPlayer || !LocalPlayer->CEntity) return;

	for (const auto& Entity : EntityList) {
		if (Entity.Index == LocalPlayer->Index) continue;
		if (!Entity.IsValid()) continue;

		Vec2 Position{};
		if (!WorldToScreen(Entity.CEntity->Position, Position)) continue;

		if (Settings::ESP::IgnoreDead && !Entity.IsAlive()) continue;
		if (Settings::ESP::IgnoreTeam && Entity.ClientInfo->Team == LocalPlayer->ClientInfo->Team) continue;

		ImDrawList* draw = ImGui::GetBackgroundDrawList();

		float Distance = LocalPlayer->CEntity->Position.Distance(Entity.CEntity->Position);

		// prevent boxes from getting too big
		// if you're too close
		if (Distance < 1.0f) Distance = 1.0f;

		// scaling
		float Scale = 500.0f / Distance;
		float Width = 30.0f * Scale;
		float Height = 125.0f * Scale;

		if (Settings::ESP::Box) {
			// p_min - top left ; p_max - bottom right
			Vec2 Head = Engine::GetPlayerTag(bones.list[HEAD], Entity.CEntity, { 0.0f, 0.0f, 11.0f });
			Vec2 Foot = Engine::GetPlayerTag(bones.list[LEFTFOOT], Entity.CEntity, { 0.0f, 0.0f, -8.0f });

			if (Head.IsZero() || Foot.IsZero()) continue;

			// bounding box
			float BoxHeight = abs(Head.y - Foot.y);
			float BoxWidth = BoxHeight / 2.0f;

			Vec2 TopLeft = { Head.x - (BoxWidth / 2.0f), Head.y };
			Vec2 BottomRight = { Head.x + (BoxWidth / 2.0f), Foot.y };

			// stroke
			draw->AddRect({ TopLeft.x - 1.0f, TopLeft.y - 1.0f }, { BottomRight.x + 1.0f, BottomRight.y + 1.0f }, ToU32(Settings::ESP::StrokeColor), 0.0f, 0, 2.0f);
			// box
			draw->AddRect(TopLeft, BottomRight, ToU32(Settings::ESP::BoxColor), 0.0f, 0, 1.5f);
		}

		if (Settings::ESP::Bone) {
			if (!Entity.CEntity || !Entity.CEntity->IsValidEntity) {
				continue;
			}

			// upper
			Vec2 Head = Engine::GetPlayerTag(bones.list[HEAD], Entity.CEntity);
			Vec2 Neck = Engine::GetPlayerTag(bones.list[NECK], Entity.CEntity);
			Vec2 Spine = Engine::GetPlayerTag(bones.list[SPINE], Entity.CEntity);

			// arms
			Vec2 LeftClavicle = Engine::GetPlayerTag(bones.list[LEFTCLAVICLE], Entity.CEntity);
			Vec2 LeftElbow = Engine::GetPlayerTag(bones.list[LEFTELBOW], Entity.CEntity);
			Vec2 LeftWrist = Engine::GetPlayerTag(bones.list[LEFTWRIST], Entity.CEntity);
			Vec2 RightClavicle = Engine::GetPlayerTag(bones.list[RIGHTCLAVICLE], Entity.CEntity);
			Vec2 RightElbow = Engine::GetPlayerTag(bones.list[RIGHTELBOW], Entity.CEntity);
			Vec2 RightWrist = Engine::GetPlayerTag(bones.list[RIGHTWRIST], Entity.CEntity);

			// lower
			Vec2 Pelvis = Engine::GetPlayerTag(bones.list[PELVIS], Entity.CEntity);
			Vec2 LeftFoot = Engine::GetPlayerTag(bones.list[LEFTFOOT], Entity.CEntity);
			Vec2 LeftKnee = Engine::GetPlayerTag(bones.list[LEFTKNEE], Entity.CEntity);
			Vec2 RightFoot = Engine::GetPlayerTag(bones.list[RIGHTFOOT], Entity.CEntity);
			Vec2 RightKnee = Engine::GetPlayerTag(bones.list[RIGHTKNEE], Entity.CEntity);

			// draw bones
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
			const char* Name = Entity.Name;
			Vec2 LeftFoot = Engine::GetPlayerTag(bones.list[LEFTFOOT], Entity.CEntity);
			Vec2 RightFoot = Engine::GetPlayerTag(bones.list[RIGHTFOOT], Entity.CEntity);

			Vec2 Midpoint = LeftFoot.Midpoint(RightFoot);

			ImVec2 TextSize = ImGui::CalcTextSize(Name);
			Vec2 CenteredText = { Midpoint.x - (TextSize.x * 0.5f), Midpoint.y }; // without * 0.5f it goes too much to the left

			// stroke
			draw->AddText({ CenteredText.x + 1.0f, CenteredText.y - 1.0f}, ToU32(Settings::ESP::StrokeColor), Name);
			draw->AddText({ CenteredText.x - 1.0f, CenteredText.y + 1.0f }, ToU32(Settings::ESP::StrokeColor), Name);

			// name
			draw->AddText(CenteredText, ToU32(Settings::ESP::NameColor), Name);
		}

		if (Settings::ESP::Distance) {
		    // convert float to string
			char PlayerDistance[64]{};
			int _ = snprintf(PlayerDistance, sizeof(PlayerDistance), "%.0fm", Distance / 40.0f); // meter suffix; should be divided by ~38 but its rounded to 40

			Vec2 LeftFoot = Engine::GetPlayerTag(bones.list[LEFTFOOT], Entity.CEntity);
			Vec2 RightFoot = Engine::GetPlayerTag(bones.list[RIGHTFOOT], Entity.CEntity);

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
			draw->AddText({ CenteredText.x + 1.0f, CenteredText.y - 1.0f }, ToU32(Settings::ESP::StrokeColor), PlayerDistance);
			draw->AddText({ CenteredText.x - 1.0f, CenteredText.y + 1.0f }, ToU32(Settings::ESP::StrokeColor), PlayerDistance);

			// distance
			draw->AddText(CenteredText, ToU32(Settings::ESP::DistanceColor), PlayerDistance);
		}
	}
}
