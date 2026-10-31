#include "esp.hpp"
#include "settings.hpp"
#include "util/math.hpp"
#include <common.hpp>
#include <ratio>
#include <sdk/EntityManager.hpp>
#include <render/menu.hpp>

#include <imgui.h>

#include <chrono>
#include <unordered_map>

#define concat(first, second) first second

namespace {
	void ConnectBone(ImDrawList* draw, Vec2 bone1, Vec2 bone2) {
	    if (bone1.IsZero() || bone2.IsZero()) return;

		draw->AddLine(bone1, bone2, ToU32(Settings::ESP::BoneColor), 1.0f);
	}

	// Spherical to Cartesian
	Vector3 GetForwardVector(Vector2 ViewAngles) {
    float PitchRad = ViewAngles.x * DEGTORAD;
    float YawRad = ViewAngles.y * DEGTORAD;

    float SinPitch = sin(PitchRad);
    float CosPitch = cos(PitchRad);
    float SinYaw = sin(YawRad);
    float CosYaw = cos(YawRad);

    // build the direction vector based on the angles
    Vector3 ForwardVec;
    ForwardVec.x = CosPitch * CosYaw;
    ForwardVec.y = CosPitch * SinYaw;
    ForwardVec.z = -SinPitch;

    return ForwardVec;
	}
}

void ESP::DrawPlayers() {
	if (!globals->Running || !globals->IsInGame) return;
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

		// gimmick feature
		if (Settings::ESP::LookingAtYou) {
            Vector3 enemyToMe = LocalPlayer->CEntity->Position - Entity.CEntity->Position;
            enemyToMe.Normalize();

            float Dot = enemyToMe.Dot(GetForwardVector(Entity.CEntity->ViewAngles));
            bool FacingMe = Dot > 0.95f;

            // todo: add IsVisible check with CG_Trace

            if (FacingMe) {
                // prevention of spamming notifications
                // but also be able to get notis for other enemies looking at you
                auto now = std::chrono::steady_clock::now();
                static std::unordered_map<int, std::chrono::time_point<std::chrono::steady_clock>> u{}; // only initialize once

                auto Iterator = u.find(Entity.Index);

                bool ShouldNotify = false;
                // if index isnt stored in map then notify
                if (Iterator == u.end()) {
                    ShouldNotify = true;
                }

                // if 3.5s havent passed, don't notify (to stop spamming notifications)
                else {
                    auto IteratorTime = Iterator->second;

                    if (now - IteratorTime > std::chrono::milliseconds(3500)) {
                        ShouldNotify = true;
                    }
                }

                if (ShouldNotify) {
                    u[Entity.Index] = now; // store index with the time the notification ran

                    // probably the worst way to do this
                    char msg[48]; // 32 for name + 15 + null terminator (1) = 48
                    strcpy_s(msg, sizeof(msg), Entity.Name);
                    strcat_s(msg, sizeof(msg), " is facing you!");
                    Widgets::Notify(WARNING, msg);
                }
            }
		}
	}
}
