#pragma once
#include "HookIncludes.h"
#include "Interfaces.h"
#include <ctime>
#include "ESP.h"
#include "MovementRecorder.h"



bool once = true;
bool once1 = false;
int width1 = 0;
int height1 = 0;
void __fastcall hkPaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	C_BaseEntity* local = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
	if (local != nullptr && local->IsAlive() && g_Options.Misc.noscopeborder && !strcmp("HudZoom", Interfaces::Panel()->GetName(vguiPanel)))
	{
		return;
	}
	hooks::original_paint_traverse(pPanels, vguiPanel, forceRepaint, allowForce);
	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;
	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)Interfaces::Panel()->GetName(vguiPanel);
		if (strstr(szPanelName, "FocusOverlayPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{
		if (Interfaces::Engine()->IsConnected() && Interfaces::Engine()->IsInGame())
		{
			static auto linegoesthrusmoke = CSX::Memory::FindPattern(CLIENT_DLL, "\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx",0);
			static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
			if (g_Options.Misc.NoSmoke) *(int*)(smokecout) = 0;

			visuals::instance().OnPaintTraverse();

			auto m_flFlashDuration = NetVarManager->GetOffset("DT_CSPlayer", "m_flFlashDuration");
			auto m_flFlashMaxAlpha = NetVarManager->GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
			if (local != nullptr)
			{
				CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
				if (g_Options.Misc.NoFlash)
				{
					*MakePtr(float*, local, m_flFlashDuration) = 0.f;
					*MakePtr(float*, local, m_flFlashMaxAlpha) = 0.f;
				}
				if (local && local->IsScoped() && g_Options.Misc.noscopeborder && MiscFunctions::IsSniper(pWeapon))
				{
					int width = 0;
					int height = 0;
					Interfaces::Engine()->GetScreenSize(width, height);

					int centerX = static_cast<int>(width * 0.5f);
					int centerY = static_cast<int>(height * 0.5f);
					g_Render->Line(0, centerY, width, centerY, Color(g_Options.Color.Scope[0] * 255, g_Options.Color.Scope[1] * 255, g_Options.Color.Scope[2] * 255, 255));
					g_Render->Line(centerX, 0, centerX, height, Color(g_Options.Color.Scope[0] * 255, g_Options.Color.Scope[1] * 255, g_Options.Color.Scope[2] * 255, 255));
				}
			}
		}
		if (!Interfaces::Engine()->IsInGame()) {
			g_Options.Misc.namespam = false;
			g_Options.Misc.NameSpammer = 0;
			g_Options.Misc.hidename = false;
			g_Options.Misc.ChatSpamMode = false;
			g_Options.Misc.silentstealer = false;
		}
		if (g_Options.Ragebot.MainSwitch && !once)
		{
			g_Options.Legitbot.MainSwitch = false;
			once = !once;
		}
		if (g_Options.Legitbot.MainSwitch && once)
		{
			g_Options.Ragebot.MainSwitch = false;
			once = !once;
		}


		if (g_Options.Ragebot.RageDraw)
		{
			int width = 0;
			int height = 0;
		Interfaces::Engine()->GetScreenSize(width, height);

			if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
			{
				if (local->IsAlive())
				{
					g_Render->Textf(1, 50, Color(255, 0, 0, 255), g_Render->font.ESP, "Shots Fired: %i", Globals::Shots);
					g_Render->Textf(1, 70, Color(255, 0, 0, 255), g_Render->font.ESP, "Missed Shots: %i", Globals::missedshots);
					g_Render->Textf(1, 90, Color(255, 0, 0, 255), g_Render->font.ESP, "Tick Base: %f", Globals::NextTime);
					if (g_Options.Visuals.LBYIndicator)
					{
						if (local->GetEyeAngles()->y - local->GetLowerBodyYaw() > 35)
						{
							g_Render->Textf(10, height / 2, Color(0, 255, 0, 255), g_Render->font.LBY, "LBY");
						}
						else
						{
							g_Render->Textf(10, height / 2, Color(255, 0, 0, 255), g_Render->font.LBY, "LBY");
						}
					}
				}
			}
		}


		DrawMovementRecorder();
	}
}
