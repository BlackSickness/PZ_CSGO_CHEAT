#pragma once
#include "ImGui/imgui.h"
#include "SDK.h"



void DrawingSpectatorList()
{
	int specs = 0;
	int modes = 0;
	std::string spect = "";
	std::string mode = "";
	if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
	{
		int localIndex = Interfaces::Engine()->GetLocalPlayer();
		C_BaseEntity* pLocalEntity = Interfaces::EntityList()->GetClientEntity(localIndex);
		if (pLocalEntity)
		{
			for (int i = 0; i < Interfaces::EntityList()->GetHighestEntityIndex(); i++)
			{
				C_BaseEntity*  pBaseEntity = Interfaces::EntityList()->GetClientEntity(i);

				if (!pBaseEntity)
					continue;
				if (pBaseEntity->IsAlive())
					continue;
				if (pBaseEntity == nullptr)
					continue;
				if (pBaseEntity == pLocalEntity)
					continue;
				if (pBaseEntity->IsDormant())
					continue;
				//	if (pBaseEntity->m_hObserverTarget() != pLocalEntity)
				//	continue;


				PlayerInfo pInfo;
				if (Interfaces::Engine()->GetPlayerInfo(i, &pInfo) && pBaseEntity->IsDead())
				{
					if (pInfo.m_bIsHLTV)
						continue;

					spect += pInfo.m_szPlayerName;
					spect += "\n";
					specs++;

					switch (*pBaseEntity->GetObserverMode())
					{
					case ObserverMode_t::OBS_MODE_IN_EYE:
						mode += "Perspective";
						break;
					case ObserverMode_t::OBS_MODE_CHASE:
						mode += "3rd Person";
						break;
					case ObserverMode_t::OBS_MODE_ROAMING:
						mode += "No Clip";
						break;
					case ObserverMode_t::OBS_MODE_DEATHCAM:
						mode += "Deathcam";
						break;
					case ObserverMode_t::OBS_MODE_FREEZECAM:
						mode += "Freezecam";
						break;
					case ObserverMode_t::OBS_MODE_FIXED:
						mode += "Fixed";
						break;
					default:
						break;
					}

					mode += "\n";
					modes++;
				}
			}
		}
	}
	if (ImGui::Begin(XorStr("Spectator List"), &g_Options.Misc.SpecList, ImVec2(0, 0), 0.4F, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
	{
		if (specs > 0) spect += "\n";
		if (modes > 0) mode += "\n";
		ImVec2 size = ImGui::CalcTextSize(spect.c_str());
		ImGui::SetWindowSize(ImVec2(200, 50 + size.y));
		ImGui::Columns(2);
		ImGui::Separator();

		ImGui::Text("Name");
		ImGui::NextColumn();

		ImGui::Text("Mode");
		ImGui::NextColumn();
		ImGui::Separator();

		ImGui::Text(spect.c_str());
		ImGui::NextColumn();

		ImGui::Text(mode.c_str());
		ImGui::Columns(1);
		ImGui::Separator();
	}
	ImGui::End();
}