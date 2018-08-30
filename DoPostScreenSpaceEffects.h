#pragma once
#include "HookIncludes.h"



bool _fastcall hkDoPostScreenSpaceEffects(void* ecx, void* edx, CViewSetup* pSetup)
{
	IMaterial *pMatGlowColor = Interfaces::MaterialSystem()->FindMaterial("dev/glow_color", TEXTURE_GROUP_OTHER, true);

	if (Interfaces::g_GlowObjManager && Interfaces::Engine()->IsConnected())
	{
		auto local = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
		if (local)
		{

			Interfaces::ModelRender()->ForcedMaterialOverride(pMatGlowColor);

			for (int i = 0; i < Interfaces::g_GlowObjManager->m_GlowObjectDefinitions.Count(); ++i)
			{
				if (Interfaces::g_GlowObjManager->m_GlowObjectDefinitions[i].IsUnused() || !Interfaces::g_GlowObjManager->m_GlowObjectDefinitions[i].getEnt())
					continue;

				CGlowObjectManager::GlowObjectDefinition_t* glowEnt = &Interfaces::g_GlowObjManager->m_GlowObjectDefinitions[i];

				switch (glowEnt->getEnt()->GetClientClass()->m_ClassID)
				{
				default:
					if (g_Options.Visuals.WeaponsDroppedChams)
					{
						if (strstr(glowEnt->getEnt()->GetClientClass()->m_pNetworkName, ("CWeapon")));
						glowEnt->set(0, 0, 0, 0);
					}
					if (strstr(glowEnt->getEnt()->GetClientClass()->m_pNetworkName, ("CWeapon")) && g_Options.Visuals.weaponglow)
						glowEnt->set(g_Options.Color.WeaponGlow[0], g_Options.Color.WeaponGlow[1], g_Options.Color.WeaponGlow[2], g_Options.Color.weaponglowalpha / 255.f);
					break;
				case 1:
					if (g_Options.Visuals.weaponglow)
						glowEnt->set(g_Options.Color.WeaponGlow[0], g_Options.Color.WeaponGlow[1], g_Options.Color.WeaponGlow[2], g_Options.Color.weaponglowalpha / 255.f);
					break;
				case 35:
					if (g_Options.Visuals.glowplayer)
					{

						if (!g_Options.Visuals.teamglow && glowEnt->getEnt()->GetTeamNum() == local->GetTeamNum())
							break;

						if (glowEnt->getEnt()->GetTeamNum() == local->GetTeamNum())
							glowEnt->set(g_Options.Color.TeamGlow[0], g_Options.Color.TeamGlow[1], g_Options.Color.TeamGlow[2], g_Options.Color.playerglowalpha / 255.f);
						else if (glowEnt->getEnt()->GetTeamNum() != local->GetTeamNum())
							glowEnt->set(g_Options.Color.EnemyGlow[0], g_Options.Color.EnemyGlow[1], g_Options.Color.EnemyGlow[2], g_Options.Color.playerglowalpha / 255.f);
					}
					break;
				case 39:
					if (g_Options.Visuals.weaponglow)
						glowEnt->set(g_Options.Color.WeaponGlow[0], g_Options.Color.WeaponGlow[1], g_Options.Color.WeaponGlow[2], g_Options.Visuals.weaponglow / 255.f);
					break;
				case 29:
					if (g_Options.Visuals.bombglow)
						glowEnt->set(g_Options.Color.C4glow[0], g_Options.Color.C4glow[1], g_Options.Color.C4glow[2], g_Options.Color.bombglowalpha / 255.f);
					break;
				case 105:
					if (g_Options.Visuals.bombglow)
						glowEnt->set(g_Options.Color.C4glow[0], g_Options.Color.C4glow[1], g_Options.Color.C4glow[2], g_Options.Color.bombglowalpha / 255.f);
					break;
				}
			}
		}
	}
	return hooks::original_do_post_screen_space_effects(ecx, pSetup);
}