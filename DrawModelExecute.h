#pragma once
#include "HookIncludes.h"

float flColor[3];
float flVColor[3];
float wlColor[3];
float hlColor[3];
float wwlColor[3];
IMaterial* GetCurrentMaterial(int mati)
{
	static IMaterial* mat;

	switch (mati)
	{
	case 3:
		mat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", nullptr); // Crysta
		break;
	case 4:
		mat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gold", nullptr);  // Gold
		break;
	case 5:
		mat = Interfaces::MaterialSystem()->FindMaterial("models/player/ct_fbi/ct_fbi_glass", nullptr); //glass
		break;
	case 6:
		mat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gloss", nullptr); // Plastic Glass
		break;
	case 7:
		mat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", nullptr);  // Crystal Blue
		break;
	case 8:
		mat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", nullptr);  // Glass
		break;
	case 9:
		mat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/velvet", nullptr);  // Velvet
		break;
	case 10:
		mat = Interfaces::MaterialSystem()->FindMaterial("models/gibs/glass/glass", nullptr); // Dark Chrome
		break;
	case 11:
		mat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail", nullptr); // Detailed Gold
		break;
	case 12:
		mat = Interfaces::MaterialSystem()->FindMaterial("vgui/achievements/glow", nullptr); //glow
		break;
	default:
		mat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", nullptr);
		return mat;
		break;
	}
	return mat;
}

IMaterial* GetCurrentZMaterial(int mati)
{
	static IMaterial* zmat;

	switch (mati)
	{
	case 3:
		zmat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 4:
		zmat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gold", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 5:
		zmat = Interfaces::MaterialSystem()->FindMaterial("models/player/ct_fbi/ct_fbi_glass", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 6:
		zmat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gloss", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 7:
		zmat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 8:
		zmat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 9:
		zmat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/velvet", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 10:
		zmat = Interfaces::MaterialSystem()->FindMaterial("models/gibs/glass/glass", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 11:
		zmat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 12:
		zmat = Interfaces::MaterialSystem()->FindMaterial("vgui/achievements/glow", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	default:
		zmat = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", nullptr);
		return zmat;
		break;
	}
}

void __fastcall hkDrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	static bool DontDraw = false;

	IMaterial* zmat = GetCurrentZMaterial(g_Options.Visuals.Playermatierial);
	IMaterial* mat = GetCurrentMaterial(g_Options.Visuals.Playermatierial);

	IMaterial* hmat = GetCurrentMaterial(g_Options.Misc.handmat);
	IMaterial* wmat = GetCurrentMaterial(g_Options.Misc.weaponhandmat);
	IMaterial* wwmat = GetCurrentMaterial(g_Options.Visuals.Weaponsdroppedmatierial);

	static IMaterial* norm = CreateMaterial(false, true, false);
	static IMaterial* flat = CreateMaterial(false, false, false);
	static IMaterial* wire = CreateMaterial(false, false, true);

	static IMaterial* znorm = CreateMaterial(true, true, false);
	static IMaterial* zflat = CreateMaterial(true, false, false);
	static IMaterial* zwire = CreateMaterial(true, false, true);

	static IMaterial* Outline = CreateMaterial(true, false, true);
	DontDraw = false;

	const char * ModelName = Interfaces::ModelInfo()->GetModelName((model_t*)pInfo.pModel);
	C_BaseEntity* pModelEntity = (C_BaseEntity*)Interfaces::EntityList()->GetClientEntity(pInfo.entity_index);
	C_BaseEntity* pLocal = (C_BaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	if (ModelName && strstr(ModelName, "models/player"))
	{
		if (pModelEntity && pLocal)
		{
			if ((g_Options.Visuals.Teamchams || pModelEntity->GetTeamNum() != pLocal->GetTeamNum()) && g_Options.Visuals.Chams)
			{

				pModelEntity = Interfaces::EntityList()->GetClientEntity(pInfo.entity_index);
				if (pModelEntity->GetTeamNum() != pLocal->GetTeamNum())
				{
					flColor[0] = g_Options.Color.EnemyChamsNVis[0];
					flColor[1] = g_Options.Color.EnemyChamsNVis[1];
					flColor[2] = g_Options.Color.EnemyChamsNVis[2];
					flVColor[0] = g_Options.Color.EnemyChamsVis[0];
					flVColor[1] = g_Options.Color.EnemyChamsVis[1];
					flVColor[2] = g_Options.Color.EnemyChamsVis[2];
				}
				if (pModelEntity->GetTeamNum() == pLocal->GetTeamNum())
				{
					flColor[0] = g_Options.Color.TeamChamsNVis[0];
					flColor[1] = g_Options.Color.TeamChamsNVis[1];
					flColor[2] = g_Options.Color.TeamChamsNVis[2];
					flVColor[0] = g_Options.Color.TeamChamsVis[0];
					flVColor[1] = g_Options.Color.TeamChamsVis[1];
					flVColor[2] = g_Options.Color.TeamChamsVis[2];
				}
				if (pModelEntity)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
					{
						if (g_Options.Visuals.Playermatierial == 0)
						{
							if (g_Options.Visuals.XQZ)
							{

								Interfaces::RenderView()->SetColorModulation(flColor);
								Interfaces::RenderView()->SetBlend(g_Options.Color.champlayeralpha / 100.f);
								Interfaces::ModelRender()->ForcedMaterialOverride(znorm);
								hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							Interfaces::RenderView()->SetColorModulation(flVColor);
							Interfaces::RenderView()->SetBlend(g_Options.Color.champlayeralpha / 100.f);
							Interfaces::ModelRender()->ForcedMaterialOverride(norm);
							hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else if (g_Options.Visuals.Playermatierial == 1)
						{
							if (g_Options.Visuals.XQZ)
							{

								Interfaces::RenderView()->SetColorModulation(flColor);
								Interfaces::RenderView()->SetBlend(g_Options.Color.champlayeralpha / 100.f);
								Interfaces::ModelRender()->ForcedMaterialOverride(zflat);
								hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							Interfaces::RenderView()->SetColorModulation(flVColor);
							Interfaces::RenderView()->SetBlend(g_Options.Color.champlayeralpha / 100.f);
							Interfaces::ModelRender()->ForcedMaterialOverride(flat);
							hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else if (g_Options.Visuals.Playermatierial == 2)
						{
							if (g_Options.Visuals.XQZ)
							{

								Interfaces::RenderView()->SetColorModulation(flColor);
								Interfaces::RenderView()->SetBlend(g_Options.Color.champlayeralpha / 100.f);
								Interfaces::ModelRender()->ForcedMaterialOverride(zwire);
								hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							Interfaces::RenderView()->SetColorModulation(flVColor);
							Interfaces::RenderView()->SetBlend(g_Options.Color.champlayeralpha / 100.f);
							Interfaces::ModelRender()->ForcedMaterialOverride(wire);
							hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else
						{
							if (g_Options.Visuals.XQZ)
							{

								Interfaces::RenderView()->SetColorModulation(flColor);
								Interfaces::RenderView()->SetBlend(g_Options.Color.champlayeralpha / 100.f);
								Interfaces::ModelRender()->ForcedMaterialOverride(zmat);
								hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							Interfaces::RenderView()->SetColorModulation(flVColor);
							Interfaces::RenderView()->SetBlend(g_Options.Color.champlayeralpha / 100.f);
							Interfaces::ModelRender()->ForcedMaterialOverride(mat);
							hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
					}
					else
					{
						if (g_Options.Visuals.corpses)
						{
							ForceMaterial(Color(255, 255, 255), znorm);

						}
						
					}

				}

				if (g_Options.Visuals.chamsHp && g_Options.Visuals.Chams && g_Options.Visuals.Enabled)
				{
					float color1[3] = { 0.f, 1.f, 0.f };
					float color2[3] = { 1.f, 1.f, 0.f };
					float color3[3] = { 1.f, 0.f, 0.f };

					float alpha = 1.f;
					if (pModelEntity->HasGunGameImmunity())
						alpha = 0, 1.f;

					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 && pModelEntity->GetTeamNum() != pLocal->GetTeamNum() && pModelEntity != pLocal)
					{

						if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 50)
						{
							Interfaces::RenderView()->SetColorModulation(color1);
							Interfaces::RenderView()->SetBlend(alpha);
							Interfaces::ModelRender()->ForcedMaterialOverride(norm);
							hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else if (pModelEntity->IsAlive() && pModelEntity->GetHealth() < 50 && pModelEntity->GetHealth() > 25)
						{
							Interfaces::RenderView()->SetColorModulation(color2);
							Interfaces::RenderView()->SetBlend(alpha);
							Interfaces::ModelRender()->ForcedMaterialOverride(norm);
							hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else if (pModelEntity->IsAlive() && pModelEntity->GetHealth() < 25)
						{
							Interfaces::RenderView()->SetColorModulation(color3);
							Interfaces::RenderView()->SetBlend(alpha);
							Interfaces::ModelRender()->ForcedMaterialOverride(norm);
							hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}

					}
					else if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 && g_Options.Visuals.Teamchams && pModelEntity != pLocal)
					{
						if (pModelEntity->GetTeamNum() == pLocal->GetTeamNum())
						{
							if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 50)
							{
								Interfaces::RenderView()->SetColorModulation(color1);
								Interfaces::RenderView()->SetBlend(alpha);
								Interfaces::ModelRender()->ForcedMaterialOverride(norm);
								hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							else if (pModelEntity->IsAlive() && pModelEntity->GetHealth() < 50 && pModelEntity->GetHealth() > 25)
							{
								Interfaces::RenderView()->SetColorModulation(color2);
								Interfaces::RenderView()->SetBlend(alpha);
								Interfaces::ModelRender()->ForcedMaterialOverride(norm);
								hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							else if (pModelEntity->IsAlive() && pModelEntity->GetHealth() < 25)
							{
								Interfaces::RenderView()->SetColorModulation(color3);
								Interfaces::RenderView()->SetBlend(alpha);
								Interfaces::ModelRender()->ForcedMaterialOverride(norm);
								hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
						}
					}
				}
			}

			if (pModelEntity == pLocal)
			{
				if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
				{

					Interfaces::RenderView()->SetBlend(0.3f);
					Interfaces::ModelRender()->ForcedMaterialOverride(nullptr);
					hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

				}
				else
				{
					if (g_Options.Visuals.corpses)
					{
						ForceMaterial(Color(255, 255, 255), zmat);
					}
				}
			}
		}

	}

	else if (g_Options.Misc.HandMat &&g_Options.Misc.handmat && strstr(ModelName, "arms"))
	{
		hlColor[0] = g_Options.Color.HandCham[0];
		hlColor[1] = g_Options.Color.HandCham[1];
		hlColor[2] = g_Options.Color.HandCham[2];

		if (g_Options.Misc.handmat == 0)
		{
			Interfaces::RenderView()->SetColorModulation(hlColor);
			Interfaces::RenderView()->SetBlend(g_Options.Color.chamhandalpha / 100.f);
			Interfaces::ModelRender()->ForcedMaterialOverride(norm);
			hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		if (g_Options.Misc.handmat == 1)
		{
			Interfaces::RenderView()->SetColorModulation(hlColor);
			Interfaces::RenderView()->SetBlend(g_Options.Color.chamhandalpha / 100.f);
			Interfaces::ModelRender()->ForcedMaterialOverride(flat);
			hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		if (g_Options.Misc.handmat == 2)
		{
			Interfaces::RenderView()->SetColorModulation(hlColor);
			Interfaces::RenderView()->SetBlend(g_Options.Color.chamhandalpha / 100.f);
			Interfaces::ModelRender()->ForcedMaterialOverride(wire);
			hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		else
		{
			Interfaces::RenderView()->SetColorModulation(hlColor);
			Interfaces::RenderView()->SetBlend(g_Options.Color.chamhandalpha / 100.f);
			Interfaces::ModelRender()->ForcedMaterialOverride(hmat);
			hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
	}


	else if (g_Options.Misc.WeaponsHandMat && g_Options.Misc.weaponhandmat && strstr(ModelName, "weapons/v"))
	{
		wlColor[0] = g_Options.Color.WeaponHandCham[0];
		wlColor[1] = g_Options.Color.WeaponHandCham[1];
		wlColor[2] = g_Options.Color.WeaponHandCham[2];

		if (g_Options.Misc.weaponhandmat == 0)
		{
			Interfaces::RenderView()->SetColorModulation(wlColor);
			Interfaces::RenderView()->SetBlend(g_Options.Color.weaponhandalpha / 100.f);
			Interfaces::ModelRender()->ForcedMaterialOverride(norm);
			hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		if (g_Options.Misc.weaponhandmat == 1)
		{
			Interfaces::RenderView()->SetColorModulation(wlColor);
			Interfaces::RenderView()->SetBlend(g_Options.Color.weaponhandalpha / 100.f);
			Interfaces::ModelRender()->ForcedMaterialOverride(flat);
			hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		if (g_Options.Misc.weaponhandmat == 2)
		{
			Interfaces::RenderView()->SetColorModulation(wlColor);
			Interfaces::RenderView()->SetBlend(g_Options.Color.weaponhandalpha / 100.f);
			Interfaces::ModelRender()->ForcedMaterialOverride(wire);
			hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		else
		{
			Interfaces::RenderView()->SetColorModulation(wlColor);
			Interfaces::RenderView()->SetBlend(g_Options.Color.weaponhandalpha / 100.f);
			Interfaces::ModelRender()->ForcedMaterialOverride(wmat);
			hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
	}

	else if (g_Options.Visuals.Enabled && g_Options.Visuals.WeaponsDroppedChams && strstr(ModelName, "_dropped.mdl"))
	{
		wwlColor[0] = g_Options.Color.WeaponDropped[0];
		wwlColor[1] = g_Options.Color.WeaponDropped[1];
		wwlColor[2] = g_Options.Color.WeaponDropped[2];

		if (g_Options.Visuals.Weaponsdroppedmatierial == 0)
		{
			Interfaces::RenderView()->SetColorModulation(wwlColor);
			Interfaces::RenderView()->SetBlend(g_Options.Color.weaponsworldalph / 100.f);
			Interfaces::ModelRender()->ForcedMaterialOverride(norm);
			hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		if (g_Options.Visuals.Weaponsdroppedmatierial == 1)
		{
			Interfaces::RenderView()->SetColorModulation(wwlColor);
			Interfaces::RenderView()->SetBlend(g_Options.Color.weaponsworldalph / 100.f);
			Interfaces::ModelRender()->ForcedMaterialOverride(flat);
			hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		if (g_Options.Visuals.Weaponsdroppedmatierial == 2)
		{
			Interfaces::RenderView()->SetColorModulation(wwlColor);
			Interfaces::RenderView()->SetBlend(g_Options.Color.weaponsworldalph / 100.f);
			Interfaces::ModelRender()->ForcedMaterialOverride(wire);
			hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		else
		{
			Interfaces::RenderView()->SetColorModulation(wwlColor);
			Interfaces::RenderView()->SetBlend(g_Options.Color.weaponsworldalph / 100.f);
			Interfaces::ModelRender()->ForcedMaterialOverride(wwmat);
			hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
	}


	if (!DontDraw)
		hooks::original_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces::ModelRender()->ForcedMaterialOverride(NULL);


}