#include "Render.h"



 void DrawRadars()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 oldPadding = style.WindowPadding;
	float oldAlpha = style.Colors[ImGuiCol_WindowBg].w;
	style.WindowPadding = ImVec2(0, 0);
	style.Colors[ImGuiCol_WindowBg].w = (float)g_Options.Radar.radralpha;
	style.Colors[ImGuiCol_TitleBg] = ImColor(21, 21, 21, 255);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(21, 21, 21, 255);
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(21, 21, 21, 255);
	style.Colors[ImGuiCol_CloseButton] = ImColor(0, 0, 0, 0);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImColor(0, 0, 0, 0);
	style.Colors[ImGuiCol_CloseButtonActive] = ImColor(0, 0, 0, 0);
	style.Alpha = 1.f;
	ImGui::SetNextWindowSize(ImVec2(g_Options.Radar.radrsize, g_Options.Radar.radrsize));

	if (ImGui::Begin(("Radar"), &g_Options.Radar.radarwindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize))
	{
		ImVec2 siz = ImGui::GetWindowSize();
		ImVec2 pos = ImGui::GetWindowPos();

		ImGui::GetWindowDrawList()->AddRect(ImVec2(pos.x - 6, pos.y - 6), ImVec2(pos.x + siz.x + 6, pos.y + siz.y + 6), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), 0.0F, -1, 1.5f);
		ImDrawList* windowDrawList = ImGui::GetWindowDrawList();
		windowDrawList->AddLine(ImVec2(pos.x + (siz.x / 2), pos.y + 0), ImVec2(pos.x + (siz.x / 2), pos.y + siz.y), ImGui::GetColorU32(ImVec4(50, 50, 150, 100)), 1.f);
		windowDrawList->AddLine(ImVec2(pos.x + 0, pos.y + (siz.y / 2)), ImVec2(pos.x + siz.x, pos.y + (siz.y / 2)), ImGui::GetColorU32(ImVec4(50, 50, 150, 100)), 1.f);

		C_BaseEntity *pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());


		if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
		{

			Vector LocalPos = pLocal->GetEyePosition();
			Vector ang;
			Interfaces::Engine()->GetViewAngles(ang);

			for (int i = 0; i < Interfaces::Engine()->GetMaxClients(); i++) {
				C_BaseEntity* pBaseEntity = Interfaces::EntityList()->GetClientEntity(i);


				if (!pBaseEntity)
					continue;
				if (pBaseEntity->IsDormant())
					continue;
				if (!pBaseEntity->GetHealth() > 0)
					continue;

				if (pBaseEntity == pLocal)
					continue;


				if (pLocal->GetTeamNum() == pBaseEntity->GetTeamNum())
					continue;

				bool viewCheck = false;
				Vector EntityPos = misc::RotatePoint(pBaseEntity->GetOrigin(), LocalPos, pos.x, pos.y, siz.x, siz.y, ang.y, g_Options.Radar.radrzoom, &viewCheck);


				ImU32 clr = ImGui::GetColorU32(ImVec4(255, 0, 0, 255));

				int s = 3;

				windowDrawList->AddCircleFilled(ImVec2(EntityPos.x, EntityPos.y), s, clr);

			}

		}

	}
	ImGui::End();
	style.WindowPadding = oldPadding;
	style.Colors[ImGuiCol_WindowBg].w = oldAlpha;

}
