

#include "ESP.h"
#include "Interfaces.h"
#include "Render.h"
#include <ctime>
#include <iostream>
#include <algorithm>
#include "GrenadePrediction.h"
#include "LagComp.h"
#include "Autowall.h"
#include "flashlight.h"


auto LoadNamedSky = reinterpret_cast< void(__fastcall*)(const char*) >(CSX::Memory::FindPatternV2(XorStr("engine.dll"), XorStr("55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45")));

visuals::visuals()
{
	BombCarrier = nullptr;
}
namespace ColorAlpha
{
	VisualsStruct ESP_ctx;
	RECT bbox;
}

int width = 0;
int height = 0;
bool done = false;

float damage;
char bombdamagestringdead[24];
char bombdamagestringalive[24];

bool PrecacheModel(const char* szModelName)
{
	auto m_pModelPrecacheTable = Interfaces::g_ClientStringTableContainer->FindTable("modelprecache");

	if (m_pModelPrecacheTable)
	{
	Interfaces::ModelInfo()->FindOrLoadModel(szModelName);
		int idx = m_pModelPrecacheTable->AddString(false, szModelName);
		if (idx == INVALID_STRING_INDEX)
			return false;
	}
	return true;
}
void visuals::OnPaintTraverse()
{
	C_BaseEntity *local = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle(local->GetActiveWeaponHandle());

	NightMode();
	NoSky();
	ASUSWALL();
	Clear();
	if (g_Options.Misc.misc_SkyName) { LoadNamedSky(g_Options.Misc.misc_SkyName); }
	FlashlightRun(local);
	CCSGrenadeHint::instance().Paint();

	renderBeams();

	if (g_Options.Misc.Crosshair)
		Crosshair(local);
	if (g_Options.Misc.Crosshair2)
		Crosshair2();
	if (g_Options.Misc.RecoilCrosshair)
		RecoilCrosshair(local);
	if (g_Options.Misc.Hitmarker)
		Hitmarker();

	if (g_Options.Visuals.angleLines && local->IsAlive())
		DrawAngles();

	if (g_Options.Visuals.DrawAwall && local->IsAlive())
		DrawAwall();

	static float rainbow;
	rainbow += 0.005f;

	if (rainbow > 1.f) rainbow = 0.f;

	if (g_Options.Visuals.CrosshairOn)
	{
		Interfaces::Engine()->ClientCmd_Unrestricted2("crosshair 0");
		DrawCrosshair(local,pWeapon, g_Options.Visuals.CrosshairRainbow ? Color::FromHSB(rainbow, 1.f, 1.f) : Color::Red());
	}
	else
	{
		Interfaces::Engine()->ClientCmd_Unrestricted2("crosshair 1");

	}

	if (g_Options.Visuals.AWallCrosshair)
	{
		RECT View = g_Render->GetViewport();
		int xs = View.right / 2;
		int ys = View.bottom / 2;

		int damage;

		if (MiscFunctions::IsKnife(pWeapon) || MiscFunctions::IsGrenade(pWeapon) || MiscFunctions::IsBomb(pWeapon))
		{
			g_Render->DrawFilledCircle2(xs, xs, 5, 50, Color(101, 101, 101, 170));
		}
		else if (MiscFunctions::IsSniper(pWeapon) && g_Options.Misc.noscopeborder && local->IsScoped())
		{

		}
		else if (pWeapon->AWall())
		{
			g_Render->DrawFilledCircle2(xs, xs, 5, 50, CanWallbang(damage) ? Color(130, 241, 13) : Color(255, 102, 102));
		}
	}

	int W, H, cW, cH;
	Interfaces::Engine()->GetScreenSize(W, H);
	cW = W / 2;
	cH = H / 2;

	if (g_Options.Visuals.SniperCrosshair && g_Options.Visuals.Enabled)
	{
		Color color(int(g_Options.Color.color_sniper[0] * 255.f), int(g_Options.Color.color_sniper[1] * 255.f), int(g_Options.Color.color_sniper[2] * 255.f), 150);
		int damage;
		RECT View = g_Render->GetViewport();
		int pXs = View.right / 2;
		int pYs = View.bottom / 2;
		if (MiscFunctions::IsSniper(pWeapon))
		{
			switch (g_Options.Visuals.SniperCrosshairType)
			{

			case 1:
				Interfaces::Surface()->DrawSetColor(color);
				Interfaces::Surface()->DrawFilledRect(cW - 6, cH - 1, cW - 3 + 9, cH - 1 + 2);
				Interfaces::Surface()->DrawFilledRect(cW - 1, cH - 6, cW - 1 + 2, cH - 3 + 9);
				break;
			case 2:
				if (CanWallbang(damage))
				{
					Interfaces::Surface()->DrawSetColor(0, 255, 0, 150);
					Interfaces::Surface()->DrawFilledRect(cW - 6, cH - 1, cW - 3 + 9, cH - 1 + 2);
					Interfaces::Surface()->DrawFilledRect(cW - 1, cH - 6, cW - 1 + 2, cH - 3 + 9);
				}
				else
				{
					Interfaces::Surface()->DrawSetColor(255, 0, 0, 150);
					Interfaces::Surface()->DrawFilledRect(cW - 6, cH - 1, cW - 3 + 9, cH - 1 + 2);
					Interfaces::Surface()->DrawFilledRect(cW - 1, cH - 6, cW - 1 + 2, cH - 3 + 9);
				}
				break;
			case 3:
				g_Render->Line(pXs - 10, pYs, pXs + 10, pYs, color);
				g_Render->Line(pXs, pYs - 10, pXs, pYs + 10, color);
				break;
			case 4:
				if (CanWallbang(damage))
				{
					g_Render->Line(pXs - 10, pYs, pXs + 10, pYs, Color(0, 255, 0));
					g_Render->Line(pXs, pYs - 10, pXs, pYs + 10, Color(0, 255, 0));
				}
				else
				{
					g_Render->Line(pXs - 10, pYs, pXs + 10, pYs, Color(255, 0, 0));
					g_Render->Line(pXs, pYs - 10, pXs, pYs + 10, Color(255, 0, 0));
				}
				break;
			}

		}
	}

	for (int i = 0; i < Interfaces::EntityList()->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity *entity = Interfaces::EntityList()->GetClientEntity(i);
		if (entity == nullptr)
			continue;
		if (entity == local)
			continue;
		if (entity->IsDormant())
			continue;



		PlayerInfo pinfo;
		Vector pos, pos3D;
		pos3D = entity->GetOrigin();

		int owner = 0;
		ClientClass* cClass = (ClientClass*)entity->GetClientClass();

		if (!g_Render->WorldToScreen(pos3D, pos))
			continue;

		if (g_Options.Visuals.OffscreenIndicator && entity->IsAlive())
		{
			auto pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

			if (pLocal->GetTeamNum() != entity->GetTeamNum())
			{
				Vector eyeangles, poopvec;
				int screen_w, screen_h;
				Interfaces::Engine()->GetScreenSize(screen_w, screen_h);
				Interfaces::Engine()->GetViewAngles(eyeangles);
				Vector newangle = CalcAngle(Vector(pLocal->GetAbsOrigin3().x, pLocal->GetAbsOrigin3().y, 0), Vector(entity->GetAbsOrigin3().x, entity->GetAbsOrigin3().y, 0));
				AngleVectors(Vector(0, 270, 0) - newangle + Vector(0, eyeangles.y, 0), &poopvec);
				auto circlevec = Vector(screen_w / 2, screen_h / 2, 0) + (poopvec * 250.f);

				g_Render->DrawFilledCircle(Vector2D(circlevec.x, circlevec.y), Color(255, 0, 0, 100), g_Options.Visuals.OffscreenIndicatorSize, 50);
			}
		}

		if (g_Options.Visuals.DroppedGuns && g_Options.Visuals.Enabled &&  cClass->m_ClassID != (int)ClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)ClassID::CDEagle || cClass->m_ClassID == (int)ClassID::CAK47)))
		{
			Color clr = Color(int(g_Options.Color.droppedguns[0] * 255.f), int(g_Options.Color.droppedguns[1] * 255.f), int(g_Options.Color.droppedguns[2] * 255.f));

			RenderWeapon(entity, clr);
		}

		if (g_Options.Visuals.C4World && g_Options.Visuals.Enabled)
		{
			if (cClass->m_ClassID == (int)ClassID::CPlantedC4)
				DrawBombPlanted(entity, local);
		}
		if (cClass->m_ClassID == (int)ClassID::CC4)
		{
			DrawBomb(entity, cClass);
		}
		if (g_Options.Visuals.Hostage)
		{
			Color clr = Color(255, 255, 255);
			if (cClass->m_ClassID == (int)ClassID::CHostage || cClass->m_ClassID == (int)ClassID::CHostageCarriableProp)
			{
				
					ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetOrigin(), clr);
			
			}
		}
		if (g_Options.Visuals.Chicken)
		{
			Color clr = Color(255, 255, 255);
			if (cClass->m_ClassID == (int)ClassID::CChicken)
			{
			
					ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetOrigin(), clr);
				
			}
		}
		if (g_Options.Visuals.Grenades && g_Options.Visuals.Enabled)
		{
			if (!g_Render->WorldToScreen(pos3D, pos))
				continue;

			Color GrenadeColor = Color(0, 0, 0, 0);
			char* szModelName = "Unknown Projectile";
			char* szModelIcon = "";
			if (strstr(cClass->m_pNetworkName, XorStr("Projectile")))
			{
				const model_t* pModel = entity->GetModel();
				if (!pModel)
					return;


				const studiohdr_t* pHdr = Interfaces::ModelInfo()->GetStudiomodel(pModel);

				if (!pHdr)
					return;

				if (!strstr(pHdr->name, XorStr("thrown")) && !strstr(pHdr->name, XorStr("dropped")))
					return;


				IMaterial* mats[32];
				Interfaces::ModelInfo()->GetModelMaterials(pModel, pHdr->numtextures, mats);

				for (int i = 0; i < pHdr->numtextures; i++)
				{
					IMaterial* mat = mats[i];
					if (!mat)
						continue;

					if (strstr(mat->GetName(), "flashbang"))
					{
						GrenadeColor = Color(255, 255, 0, 255);
						szModelName = "Flashbang";
						szModelIcon = "i";
					}
					else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
					{
						szModelName = "Grenade";
						szModelIcon = "j";
						GrenadeColor = Color(255, 0, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "smoke"))
					{
						szModelName = "Smoke";
						szModelIcon = "k";
						GrenadeColor = Color(0, 255, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "decoy"))
					{
						szModelName = "Decoy";
						szModelIcon = "m";
						GrenadeColor = Color(0, 255, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "incendiary"))
					{
						szModelName = "Incendiary";
						szModelIcon = "n";
						GrenadeColor = Color(255, 0, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "molotov"))
					{
						szModelName = "Molotov";
						szModelIcon = "l";
						GrenadeColor = Color(255, 0, 0, 255);
						break;
					}
				}
				switch (g_Options.Visuals.Grenades)
				{
				case 0:
					if (g_Options.Visuals.GrenadeBox)
					{
						ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetOrigin(), GrenadeColor);
					}
					break;
				case 1:
					if (!g_Options.Visuals.GrenadeBox)
					{
						g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, GrenadeColor, FONT_CENTER, "%s", szModelName);
					}
					else if (g_Options.Visuals.GrenadeBox)
					{
						//						g_Render->DrawOutlinedRect((int)pos.x - 10, (int)pos.y - 10, 20, 20, GrenadeColor);
						g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y + 15, GrenadeColor, FONT_CENTER, "%s", szModelName);
						ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetOrigin(), GrenadeColor);
					}
					break;
				case 2:
					if (!g_Options.Visuals.GrenadeBox)
					{
						g_Render->DrawString2(g_Render->font.Icon, (int)pos.x, (int)pos.y, GrenadeColor, FONT_CENTER, "%s", szModelIcon);
					}

					else if (g_Options.Visuals.GrenadeBox)
					{
						//						g_Render->DrawOutlinedRect((int)pos.x - 10, (int)pos.y - 10, 20, 20, GrenadeColor);
						g_Render->DrawString2(g_Render->font.Icon, (int)pos.x, (int)pos.y + 15, GrenadeColor, FONT_CENTER, "%s", szModelIcon);
						ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetOrigin(), GrenadeColor);
					}
					break;
				}
			}
		}


		if (Interfaces::Engine()->GetPlayerInfo(i, &pinfo) && entity->IsAlive())
		{
			if (g_Options.Legitbot.backtrack)
			{
				if (local->IsAlive())
				{
					for (int t = 0; t < g_Options.Legitbot.backtrackTicks; ++t)
					{
						Vector screenbacktrack[64][12];

						if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimulationTime())
						{
							if (g_Render->WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
							{
								g_Render->DrawFilledCircle2(screenbacktrack[i][t].x, screenbacktrack[i][t].y, g_Options.Legitbot.backtrack_size, g_Options.Legitbot.backtrack_size, Color(g_Options.Color.backtrack[0] * 255, g_Options.Color.backtrack[1] * 255, g_Options.Color.backtrack[2] * 255, 255));

							}
						}
					}
				}
				else
				{
					memset(&headPositions[0][0], 0, sizeof(headPositions));
				}
			}
			if (g_Options.Ragebot.PosAdjust)
			{
				if (local->IsAlive())
				{
					for (int t = 0; t < 12; ++t)
					{
						Vector screenbacktrack[64][12];

						if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimulationTime())
						{
							if (g_Render->WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
							{

								Interfaces::Surface()->DrawSetColor(Color::Green());
								Interfaces::Surface()->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);

							}
						}
					}
				}
				else
				{
					memset(&headPositions[0][0], 0, sizeof(headPositions));
				}
			}
			if (g_Options.Ragebot.FakeLagFix)
			{
				if (local->IsAlive())
				{
					Vector screenbacktrack[64];

					if (backtracking->records[i].tick_count + 12 > Interfaces::Globals()->tickcount)
					{
						if (g_Render->WorldToScreen(backtracking->records[i].headPosition, screenbacktrack[i]))
						{

							Interfaces::Surface()->DrawSetColor(Color::Black());
							Interfaces::Surface()->DrawLine(screenbacktrack[i].x, screenbacktrack[i].y, screenbacktrack[i].x + 2, screenbacktrack[i].y + 2);

						}
					}
				}
				else
				{
					memset(&backtracking->records[0], 0, sizeof(backtracking->records));
				}
			}
			custommodels(entity);

			if (!g_Render->WorldToScreen(pos3D, pos))
				continue;


			Color EspPlayerColor = GetPlayerColor(entity);
			Color EspVisibleColor = GetPlayerVisibleColor(local,entity);

			if (entity->GetTeamNum() == local->GetTeamNum() && !g_Options.Visuals.TeamESP)
				continue;
			if (!entity->IsAlive())
				continue;

			bool PVS = false;
			RECT rect = DynamicBox(entity, PVS, local);
			DrawInfo(rect, entity, local);
			DrawFov();

			if (g_Options.Visuals.Box)
			{
				switch (g_Options.Visuals.BoxType)
				{
				case 0:
					DrawBox(rect, EspVisibleColor);
					break;
				case 1:
					DrawCorners(rect, EspVisibleColor);
					break;
				case 2:
					ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetOrigin(), EspVisibleColor);
					break;
				}
			}
			if (g_Options.Visuals.skeletonenbl && g_Options.Visuals.Enabled)
			{
				switch (g_Options.Visuals.skeletonopts)
				{
				case 0:
					Skeleton(entity, Color(int(g_Options.Color.color_skeleton[0] * 255.f), int(g_Options.Color.color_skeleton[1] * 255.f), int(g_Options.Color.color_skeleton[2] * 255.f)));
					break;
				case 1:
					FingerSkeleton(entity, Color(int(g_Options.Color.color_skeleton[0] * 255.f), int(g_Options.Color.color_skeleton[1] * 255.f), int(g_Options.Color.color_skeleton[2] * 255.f)));
					break;
				case 2:
					HealthSkeleton(entity);
					break;
				}
			}

			if (g_Options.Visuals.health)
			{
				switch (g_Options.Visuals.healthtype)
				{
				case 0: break;
				case 1:
					DrawHealth(rect, entity);
					break;
				case 2:
					edgyHealthBar(rect, entity);
					break;
				}
			}
			if (g_Options.Visuals.armor)
				armorbar(rect, entity);
			if (g_Options.Visuals.fill)
		    	FillBox(rect, local, entity);
			if (g_Options.Visuals.AimLine)
				DrawSnapLine(Vector(rect.left + ((rect.right - rect.left) / 2), rect.bottom, 0), EspVisibleColor);
			if (g_Options.Visuals.barrel)
				BulletTrace(entity, EspVisibleColor);
			if (g_Options.Visuals.DLight)
				DLight(entity);

		}


	}
}

void visuals::DrawBox(RECT rect, Color Col)
{
	g_Render->DrawOutlinedRect(rect.left - 1, rect.top - 1, rect.right - rect.left + 2, rect.bottom - rect.top + 2, Color(0, 0, 0, 150));
	g_Render->DrawOutlinedRect(rect.left + 1, rect.top + 1, rect.right - rect.left - 2, rect.bottom - rect.top - 2, Color(0, 0, 0, 125));
	g_Render->DrawOutlinedRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, Col);
}

void visuals::DrawCorners(RECT rect, Color Col)
{
	int x1, y1, x2, y2, w, h;
	x1 = rect.left;
	y1 = rect.top;
	x2 = rect.right;
	y2 = rect.bottom;

	w = x2;
	h = y2;

	int Line_Size = (y1 - h) / 6;
	int Line_Size2 = (y1 - h) / 6;

	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
	uint8_t alpha = 0;
	Col.GetColor(red, green, blue, alpha);
	Interfaces::Surface()->DrawSetColor(red, green, blue, alpha);

	//top inwards
	Interfaces::Surface()->DrawLine(w, y1, w + Line_Size, y1);
	Interfaces::Surface()->DrawLine(x1, y1, x1 - Line_Size, y1);

	//top downwards
	Interfaces::Surface()->DrawLine(x1, y1, x1, y1 - Line_Size);
	Interfaces::Surface()->DrawLine(w, y1, w, y1 - Line_Size);

	//bottom inwards
	Interfaces::Surface()->DrawLine(x1, h, x1 - Line_Size, h);
	Interfaces::Surface()->DrawLine(w, h, w + Line_Size, h);

	//bottom upwards
	Interfaces::Surface()->DrawLine(x1, h, x1, h + Line_Size);
	Interfaces::Surface()->DrawLine(w, h, w, h + Line_Size);

	//outlines

	Interfaces::Surface()->DrawSetColor(0, 0, 0, 200);

	//top inwards
	Interfaces::Surface()->DrawLine(w, y1 - 1, w + Line_Size, y1 - 1);
	Interfaces::Surface()->DrawLine(x1, y1 - 1, x1 - Line_Size, y1 - 1);
	//inlines
	Interfaces::Surface()->DrawLine(w - 1, y1 + 1, w + Line_Size, y1 + 1);
	Interfaces::Surface()->DrawLine(x1 + 1, y1 + 1, x1 - Line_Size, y1 + 1);

	// top downwards
	Interfaces::Surface()->DrawLine(x1 - 1, y1 - 1, x1 - 1, y1 - Line_Size);
	Interfaces::Surface()->DrawLine(w + 1, y1 - 1, w + 1, y1 - Line_Size);
	//inlines
	Interfaces::Surface()->DrawLine(x1 + 1, y1, x1 + 1, y1 - Line_Size);
	Interfaces::Surface()->DrawLine(w - 1, y1, w - 1, y1 - Line_Size);

	//bottom inwards
	Interfaces::Surface()->DrawLine(x1, h + 1, x1 - Line_Size, h + 1);
	Interfaces::Surface()->DrawLine(w, h + 1, w + Line_Size, h + 1);
	//inlines
	Interfaces::Surface()->DrawLine(x1 + 1, h - 1, x1 - Line_Size, h - 1);
	Interfaces::Surface()->DrawLine(w - 1, h - 1, w + Line_Size, h - 1);

	//bottom upwards
	Interfaces::Surface()->DrawLine(x1 - 1, h + 1, x1 - 1, h + Line_Size);
	Interfaces::Surface()->DrawLine(w + 1, h + 1, w + 1, h + Line_Size);
	//inlines
	Interfaces::Surface()->DrawLine(x1 + 1, h, x1 + 1, h + Line_Size);
	Interfaces::Surface()->DrawLine(w - 1, h, w - 1, h + Line_Size);
}

void visuals::ThreeDBox(Vector minin, Vector maxin, Vector pos, Color Col)
{
	Vector min = minin + pos;
	Vector max = maxin + pos;

	Vector corners[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(min.x, min.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(max.x, max.y, max.z),
		Vector(max.x, min.y, max.z) };


	int edges[12][2] = { { 0, 1 },{ 1, 2 },{ 2, 3 },{ 3, 0 },{ 4, 5 },{ 5, 6 },{ 6, 7 },{ 7, 4 },{ 0, 4 },{ 1, 5 },{ 2, 6 },{ 3, 7 }, };

	for (const auto edge : edges)
	{
		Vector p1, p2;
		if (!g_Render->WorldToScreen(corners[edge[0]], p1) || !g_Render->WorldToScreen(corners[edge[1]], p2))
			return;
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;
		uint8_t alpha = 0;
		Col.GetColor(red, green, blue, alpha);
		Interfaces::Surface()->DrawSetColor(red, green, blue, alpha);
		Interfaces::Surface()->DrawLine(p1.x, p1.y, p2.x, p2.y);
	}
}

void visuals::FillBox(RECT rect,C_BaseEntity* local, C_BaseEntity* entity)
{
	if (entity->GetTeamNum() == local->GetTeamNum())
	{
	Interfaces::Surface()->DrawSetColor(Color(int(g_Options.Color.fill_color_team[0] * 255.f), int(g_Options.Color.fill_color_team[1] * 255.f), int(g_Options.Color.fill_color_team[2] * 255.f), g_Options.Visuals.esp_fill_amount));
		ColorAlpha::ESP_ctx.clr_fill.SetAlpha(g_Options.Visuals.esp_fill_amount * 255.f);
		RenderFill(rect);
	}
	else if (entity->GetTeamNum() != local->GetTeamNum())
	{
		ColorAlpha::ESP_ctx.clr_fill.SetAlpha(g_Options.Visuals.esp_fill_amount * 255.f);
		Interfaces::Surface()->DrawSetColor(Color(int(g_Options.Color.fill_color_enemy[0] * 255.f), int(g_Options.Color.fill_color_enemy[1] * 255.f), int(g_Options.Color.fill_color_enemy[2] * 255.f), g_Options.Visuals.esp_fill_amount));

		RenderFill(rect);
	}
}

void visuals::DrawAwall()
{
	int MidX;
	int MidY;
	Interfaces::Engine()->GetScreenSize(MidX, MidY);

	int damage;
	if (CanWallbang(damage))
	{
		g_Render->OutlineCircle(MidX / 2, MidY / 2, 10, 10, Color(0, 255, 0));
		g_Render->Textf(MidX / 2, MidY / 2 + 6, Color(255, 255, 255, 255), g_Render->font.ESP, "DMG: %1i", damage);
	}
	else
	{
		g_Render->OutlineCircle(MidX / 2, MidY / 2, 10, 10, Color(255, 0, 0));
		g_Render->Textf(MidX / 2, MidY / 2 + 6, Color(255, 255, 255, 255), g_Render->font.ESP, "DMG: 0");
	}
}

void visuals::DrawAngles()
{
	C_BaseEntity *pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = pLocal;

	/*AngleVectors(QAngle(0, pLocal->GetLowerBodyYaw(), 0), &forward);
	src3D = pLocal->GetOrigin();
	dst3D = src3D + (forward * 35.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::EngineTrace()->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
	return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(255, 165, 0, 255));
	g_Render->Text(dst.x, dst.y, Color(255, 165.f, 0, 255), g_Render->font.ESP, "lby");*/

	AngleVectors(QAngle(0, Globals::RealAngle, 0), &forward);
	src3D = pLocal->GetOrigin();
	dst3D = src3D + (forward * 45.f);

	ray.Init(src3D, dst3D);

	Interfaces::EngineTrace()->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(0, 255, 0, 255));
	g_Render->Text(dst.x, dst.y, Color(0, 255, 0, 255), g_Render->font.ESP, "REAL");

	AngleVectors(QAngle(0, Globals::FakeAngle, 0), &forward);
	dst3D = src3D + (forward * 45.f);

	ray.Init(src3D, dst3D);

	Interfaces::EngineTrace()->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(255, 0, 0, 255));
	g_Render->Text(dst.x, dst.y, Color(255, 0, 0, 255), g_Render->font.ESP, "FAKE");
}

RECT visuals::DynamicBox(C_BaseEntity* pPlayer, bool& PVS, C_BaseEntity* local)
{
	Vector trans = pPlayer->GetOrigin();

	Vector min;
	Vector max;

	min = pPlayer->GetCollideable()->OBBMins();
	max = pPlayer->GetCollideable()->OBBMaxs();

	Vector pointList[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector Distance = pointList[0] - pointList[1];
	int dst = Distance.Length();
	dst /= 1.3f;
	Vector angs;
	CalcAngle(trans, local->GetEyePosition(), angs);

	Vector all[8];
	angs.y += 45;
	for (int i = 0; i < 4; i++)
	{
		AngleVectors(angs, &all[i]);
		all[i] *= dst;
		all[i + 4] = all[i];
		all[i].z = max.z;
		all[i + 4].z = min.z;
		VectorAdd(all[i], trans, all[i]);
		VectorAdd(all[i + 4], trans, all[i + 4]);
		angs.y += 90;
	}

	Vector flb, brt, blb, frt, frb, brb, blt, flt;
	PVS = true;

	if (!Interfaces::DebugOverlay()->ScreenPosition(all[3], flb))
		PVS = false;
	if (!Interfaces::DebugOverlay()->ScreenPosition(all[0], blb))
		PVS = false;
	if (!Interfaces::DebugOverlay()->ScreenPosition(all[2], frb))
		PVS = false;
	if (!Interfaces::DebugOverlay()->ScreenPosition(all[6], blt))
		PVS = false;
	if (!Interfaces::DebugOverlay()->ScreenPosition(all[5], brt))
		PVS = false;
	if (!Interfaces::DebugOverlay()->ScreenPosition(all[4], frt))
		PVS = false;
	if (!Interfaces::DebugOverlay()->ScreenPosition(all[1], brb))
		PVS = false;
	if (!Interfaces::DebugOverlay()->ScreenPosition(all[7], flt))
		PVS = false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 0; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
	}
	RECT rect;
	rect.left = left;
	rect.bottom = bottom;
	rect.right = right;
	rect.top = top;
	return rect;
}

void visuals::BulletTrace(C_BaseEntity* pEntity, Color color)
{
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	Vector eyes = *pEntity->GetEyeAngles();

	AngleVectors(eyes, &forward);
	filter.pSkip = pEntity;
	src3D = pEntity->GetBonePos(6) - Vector(0, 0, 0);
	dst3D = src3D + (forward * g_Options.Visuals.barrelL);

	ray.Init(src3D, dst3D);

	Interfaces::EngineTrace()->TraceRay(ray, MASK_SHOT, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;


	g_Render->Line(src.x, src.y, dst.x, dst.y, color);
	g_Render->DrawOutlinedRect(dst.x - 3, dst.y - 3, 6, 6, color);
};


void visuals::DrawSnapLine(Vector to, Color clr)
{
	int width = 0;
	int height = 0;
	Interfaces::Engine()->GetScreenSize(width, height);
	Vector From((width / 2), height - 1, 0);
	g_Render->Line(From.x, From.y, to.x, to.y, clr);
}

void visuals::DrawHealth(RECT rect, C_BaseEntity* pPlayer)
{
	float HealthValue = pPlayer->GetHealth();

	float HealthValue2 = pPlayer->GetHealth();
	if (HealthValue2 > 100)
		HealthValue2 = 100;

	//	char hp[256];
	//	sprintf(hp, "%.0f", HealthValue);
	int Red = 255 - (HealthValue2 * 2.55);
	int Green = HealthValue2 * 2.55;
	float height = (rect.bottom - rect.top) * (HealthValue2 / 100);
	g_Render->GradientH(rect.left - 9, rect.top - 1, 6, rect.bottom - rect.top + 2, Color(0, 0, 0, 150), Color(0, 0, 0, 150));
	g_Render->GradientH(rect.left - 8, rect.bottom - height, 4, height, Color(Red, Green, 0, 255), Color(Red, Green, 0, 255));
	g_Render->DrawOutlinedRect(rect.left - 9, rect.top - 1, 6, rect.bottom - rect.top + 2, Color(0, 0, 0, 255));
	//	g_Render->DrawString2(g_Render->font.ESP, rect.left - 6, rect.bottom - height + 1, Color(255, 255, 255, 255), FONT_CENTER, hp);
}

void visuals::edgyHealthBar(RECT rect, C_BaseEntity* pEntity)
{

	float top = 1.4;
	float right = 0;
	float left = 0;

	float HealthValue = pEntity->GetHealth();
	int iHealthValue = HealthValue;
	int Red = 255 - (HealthValue * 2.00);
	int Green = HealthValue * 2.00;
	float flBoxes = std::ceil(pEntity->GetHealth() / 10.f);

	float height = (rect.bottom - rect.top) * (HealthValue / 100);
	float height2 = (rect.bottom - rect.top) * (100 / 100);
	float flHeight = height2 / 10.f;


	g_Render->DrawRect(rect.left - 5, rect.top - 1, rect.left - 1, rect.bottom + 1, Color(0, 0, 0, 150));
	g_Render->DrawRect(rect.left - 4, rect.bottom - height, rect.left - 2, rect.bottom, Color(Red, Green, 0, 255));

	for (int i = 0; i < 10; i++)
		g_Render->Line(rect.left - 5, rect.top + i * flHeight, rect.left - 2, rect.top + i * flHeight, Color(0, 0, 0, 255));


}

void visuals::armorbar(RECT rect, C_BaseEntity* pEntity)
{
	float ArmorValue = pEntity->ArmorValue();
	int iArmorValue = ArmorValue;
	int red = 255 - (ArmorValue * 2.0);
	int blue = ArmorValue * 2.0;

	float height = (rect.right - rect.left) * (ArmorValue / 100);

	g_Render->DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, Color(10, 10, 10, 0));
	g_Render->DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, Color(red, blue, blue, 0));

	if (g_Options.Visuals.armor && g_Options.Visuals.Enabled)
	{
		g_Render->DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, Color(10, 10, 10, 165));
		g_Render->DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, Color(red, blue, blue, 255));
	}

	int Armor = pEntity->ArmorValue();

}


void visuals::Hitmarker()
{
	if (G::hitmarkeralpha < 0.f)
		G::hitmarkeralpha = 0.f;
	else if (G::hitmarkeralpha > 0.f)
		G::hitmarkeralpha -= 0.01f;

	int W, H;
	Interfaces::Engine()->GetScreenSize(W, H);

	if (G::hitmarkeralpha > 0.f)
	{
		g_Render->Line(W / 2 - 5, H / 2 - 5, W / 2 - 3, H / 2 - 3, Color(int(g_Options.Color.hitmarker_color[0] * 255.f), int(g_Options.Color.hitmarker_color[1] * 255.f), int(g_Options.Color.hitmarker_color[2] * 255.f), (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 - 5, H / 2 + 5, W / 2 - 3, H / 2 + 3, Color(int(g_Options.Color.hitmarker_color[0] * 255.f), int(g_Options.Color.hitmarker_color[1] * 255.f), int(g_Options.Color.hitmarker_color[2] * 255.f), (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 + 5, H / 2 - 5, W / 2 + 3, H / 2 - 3, Color(int(g_Options.Color.hitmarker_color[0] * 255.f), int(g_Options.Color.hitmarker_color[1] * 255.f), int(g_Options.Color.hitmarker_color[2] * 255.f), (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 + 5, H / 2 + 5, W / 2 + 3, H / 2 + 3, Color(int(g_Options.Color.hitmarker_color[0] * 255.f), int(g_Options.Color.hitmarker_color[1] * 255.f), int(g_Options.Color.hitmarker_color[2] * 255.f), (G::hitmarkeralpha * 255.f)));

	}
}
void visuals::Crosshair2()
{

	int screenW, screenH;
	Interfaces::Engine()->GetScreenSize(screenW, screenH);

	float centreX = screenW / 2, centreY = screenH / 2;
	Interfaces::Surface()->DrawSetColor(Color(g_Options.Color.Crosshair[0] * 255, g_Options.Color.Crosshair[1] * 255, g_Options.Color.Crosshair[2] * 255, 255));
	Interfaces::Surface()->DrawLine(centreX - 10, centreY, centreX + 10, centreY);
	Interfaces::Surface()->DrawLine(centreX, centreY - 10, centreX, centreY + 10);

}
void visuals::Crosshair(C_BaseEntity* local)
{
	Interfaces::Engine()->GetScreenSize(width, height);
	if (local  && local->IsAlive())
	{
		static Vector ViewAngles;
		Interfaces::Engine()->GetViewAngles(ViewAngles);
		ViewAngles += local->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

		static Vector fowardVec;
		AngleVectors(ViewAngles, &fowardVec);
		fowardVec *= 10000;

		// Get ray start / end
		Vector start = local->GetOrigin() + local->GetViewOffset();
		Vector end = start + fowardVec, endScreen;

		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
		float cone = pWeapon->GetSpread() + pWeapon->GetInaccuracy();
		if (cone > 0.0f)
		{
			if (cone < 0.01f) cone = 0.01f;
			float size = (cone * height) * 0.7f;
			Color color(g_Options.Color.CrsSpread[0] * 255, g_Options.Color.CrsSpread[1] * 255, g_Options.Color.CrsSpread[2] * 255, 255);

			if (g_Render->WorldToScreen(end, endScreen))
			{
				g_Render->OutlineCircle(endScreen.x, endScreen.y, (int)size, 100, color);
			}
		}
	}
}


void visuals::DLight( C_BaseEntity* entity)
{
	if (entity->GetTeamNum() == TEAM_CT)
	{
		Vector getorig = entity->GetOrigin();
		Vector getheadorig = entity->GetEyePosition();
		dlight_t* pElight = Interfaces::Dlight()->CL_AllocElight(entity->EntIndex());
		pElight->color.r = float(g_Options.Color.Light_CT[0] * 255.f);;
		pElight->color.g = float(g_Options.Color.Light_CT[1] * 255.f);;
		pElight->color.b = float(g_Options.Color.Light_CT[2] * 255.f);;
		pElight->color.exponent = 8.f;
		pElight->m_Direction = getheadorig;
		pElight->origin = getheadorig;
		pElight->radius = 200.0f;
		pElight->die = Interfaces::Globals()->curtime + 0.1f;
		pElight->decay = 25.0f;
		pElight->key = entity->EntIndex();
		dlight_t* pDlight = Interfaces::Dlight()->CL_AllocDlight(entity->EntIndex());
		pDlight->color.r = float(g_Options.Color.Light_CT[0] * 255.f);;
		pDlight->color.g = float(g_Options.Color.Light_CT[1] * 255.f);;
		pDlight->color.b = float(g_Options.Color.Light_CT[2] * 255.f);;
		pDlight->color.exponent = 8.f;
		pDlight->m_Direction = getorig;
		pDlight->origin = getorig;
		pDlight->radius = 100.f;
		pDlight->die = Interfaces::Globals()->curtime + 0.1f;
		pDlight->decay = pDlight->radius / 5.f;
		pDlight->key = entity->EntIndex();
	}
	else if (entity->GetTeamNum() == TEAM_TT)
	{
		Vector getorig = entity->GetOrigin();
		Vector getheadorig = entity->GetEyePosition();
		dlight_t* pElight = Interfaces::Dlight()->CL_AllocElight(entity->EntIndex());
		pElight->color.r = float(g_Options.Color.Light_TT[0] * 255.f);;
		pElight->color.g = float(g_Options.Color.Light_TT[1] * 255.f);;
		pElight->color.b = float(g_Options.Color.Light_TT[2] * 255.f);;
		pElight->color.exponent = 8.f;
		pElight->m_Direction = getheadorig;
		pElight->origin = getheadorig;
		pElight->radius = 200.0f;
		pElight->die = Interfaces::Globals()->curtime + 0.1f;
		pElight->decay = 25.0f;
		pElight->key = entity->EntIndex();
		dlight_t* pDlight = Interfaces::Dlight()->CL_AllocDlight(entity->EntIndex());
		pDlight->color.r = float(g_Options.Color.Light_TT[0] * 255.f);;
		pDlight->color.g = float(g_Options.Color.Light_TT[1] * 255.f);;
		pDlight->color.b = float(g_Options.Color.Light_TT[2] * 255.f);;
		pDlight->color.exponent = 8.f;
		pDlight->m_Direction = getorig;
		pDlight->origin = getorig;
		pDlight->radius = 100.f;
		pDlight->die = Interfaces::Globals()->curtime + 0.1f;
		pDlight->decay = pDlight->radius / 5.f;
		pDlight->key = entity->EntIndex();
	}
}

void visuals::NightMode()
{
	static bool freakware = false;
	if (g_Options.Misc.NightMode)
	{
		if (!freakware)
		{
			ConVar* staticdrop = Interfaces::CVar()->FindVar("r_DrawSpecificStaticProp");
			staticdrop->SetValue(0);
			LoadNamedSky("sky_csgo_night02");

			{
				for (MaterialHandle_t i = Interfaces::MaterialSystem()->FirstMaterial(); i != Interfaces::MaterialSystem()->InvalidMaterial(); i = Interfaces::MaterialSystem()->NextMaterial(i))
				{
					IMaterial *pMaterial = Interfaces::MaterialSystem()->GetMaterial(i);

					if (!pMaterial)
						continue;

					if (!strcmp(pMaterial->GetTextureGroupName(), "World textures"))  // walls
					{

						pMaterial->ColorModulate(g_Options.Color.worldmode[0], g_Options.Color.worldmode[1], g_Options.Color.worldmode[2]);

					}
					if (!strcmp(pMaterial->GetTextureGroupName(), "StaticProp textures"))
					{
						pMaterial->ColorModulate(g_Options.Color.worldmode[0], g_Options.Color.worldmode[1], g_Options.Color.worldmode[3]);   // values of boxes n barrels n shit
					}
					if (strstr(pMaterial->GetName(), "models/props/de_dust/palace_bigdome"))
					{
						pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
					}
					if (strstr(pMaterial->GetName(), "models/props/de_dust/palace_pillars"))
					{

						pMaterial->ColorModulate(g_Options.Color.worldmode[0], g_Options.Color.worldmode[1], g_Options.Color.worldmode[3]);
					}
					if (strstr(pMaterial->GetTextureGroupName(), "Particle textures"))
					{
						pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
					}
				}
			}
		}
		freakware = true; // Update for anti fps suicide (?)
						  //Menu::Window.VisualsTab.colmodupdate.SetState(false);
	}
	else
	{
		freakware = false;
	}
}

void visuals::NoSky()
{
	static bool freakware = false;
	if (g_Options.Misc.NoSky)
	{
		if (!freakware)
		{
			ConVar* staticdrop = Interfaces::CVar()->FindVar("r_DrawSpecificStaticProp");
			staticdrop->SetValue(0);

			{
				for (MaterialHandle_t i = Interfaces::MaterialSystem()->FirstMaterial(); i != Interfaces::MaterialSystem()->InvalidMaterial(); i = Interfaces::MaterialSystem()->NextMaterial(i))
				{
					IMaterial *pMaterial = Interfaces::MaterialSystem()->GetMaterial(i);

					if (!pMaterial)
						continue;

					LoadNamedSky("sky_l4d_rural02_ldr");

					if (!strcmp(pMaterial->GetTextureGroupName(), "SkyBox textures"))  // walls
					{
						pMaterial->ColorModulate(0.f, 0.f, 0.f);

					}

					if (!strcmp(pMaterial->GetTextureGroupName(), "StaticProp textures"))
					{
						pMaterial->ColorModulate(0.f, 0.f, 0.f);   // values of boxes n barrels n shit
					}




				}
			}
		}
		freakware = true; // Update for anti fps suicide (?)
	}			  //Menu::Window.VisualsTab.colmodupdate.SetState(false);
	else
	{
		freakware = false;
	}
}

void visuals::ASUSWALL()
{
	static bool bPerformed = false, bLastSetting;

	C_BaseEntity *pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());


	if (!pLocal || !Interfaces::Engine()->IsConnected() || !Interfaces::Engine()->IsInGame())
		return;

	if (!bPerformed)
	{
		for (MaterialHandle_t i = Interfaces::MaterialSystem()->FirstMaterial(); i != Interfaces::MaterialSystem()->InvalidMaterial(); i = Interfaces::MaterialSystem()->NextMaterial(i))
		{
			IMaterial *pMaterial = Interfaces::MaterialSystem()->GetMaterial(i);

			if (!pMaterial)
				continue;


			const char* group = pMaterial->GetTextureGroupName();
			const char* name = pMaterial->GetName();



			if (strstr(group, "World textures"))
			{
				if (bLastSetting)
				{
					pMaterial->AlphaModulate(0.6);
				}
				else
				{
					pMaterial->AlphaModulate(1.00);
				}

			}


		}
		bPerformed = true;
	}
	if (bLastSetting != g_Options.Misc.ASUSWALL)
	{
		bLastSetting = g_Options.Misc.ASUSWALL;
		bPerformed = false;
	}

}
void visuals::Clear()
{
	static bool freakware = false;
	if (!g_Options.Misc.ASUSWALL && !g_Options.Misc.NoSky && !g_Options.Misc.NightMode)
	{
		if (!freakware)
		{

			{
				for (MaterialHandle_t i = Interfaces::MaterialSystem()->FirstMaterial(); i != Interfaces::MaterialSystem()->InvalidMaterial(); i = Interfaces::MaterialSystem()->NextMaterial(i))
				{
					IMaterial *pMaterial = Interfaces::MaterialSystem()->GetMaterial(i);

					if (!pMaterial)
						continue;



					if (!strcmp(pMaterial->GetTextureGroupName(), "World textures"))  // walls
					{
						pMaterial->SetMaterialVarFlag(MaterialVarFlags_t::MATERIAL_VAR_TRANSLUCENT, false);
						pMaterial->AlphaModulate(1.0f);
						pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);

					}

					if (!strcmp(pMaterial->GetTextureGroupName(), "SkyBox textures"))  // walls
					{
						pMaterial->ColorModulate(1.f, 1.f, 1.f);

					}

					if (!strcmp(pMaterial->GetTextureGroupName(), "StaticProp textures"))
					{
						pMaterial->ColorModulate(1.f, 1.f, 1.f);   // values of boxes n barrels n shit
					}

					if (!strcmp(pMaterial->GetTextureGroupName(), "StaticProp textures"))
					{
						pMaterial->ColorModulate(1.f, 1.f, 1.f);   // values of boxes n barrels n shit
					}

				}
			}
		}
		freakware = true; // Update for anti fps suicide (?)
	}			  //Menu::Window.VisualsTab.colmodupdate.SetState(false);
	else
	{
		freakware = false;
	}
}

Color visuals::GetPlayerColor(C_BaseEntity* pPlayer)
{
	Color CT_Color(int(g_Options.Color.esp_Color_CT[0] * 255.f), int(g_Options.Color.esp_Color_CT[1] * 255.f), int(g_Options.Color.esp_Color_CT[2] * 255.f));
	Color TT_Color(int(g_Options.Color.esp_Color_TT[0] * 255.f), int(g_Options.Color.esp_Color_TT[1] * 255.f), int(g_Options.Color.esp_Color_TT[2] * 255.f));

	if (pPlayer->GetTeamNum() == TEAM_CT)
	{
		return CT_Color;
	}
	else if (pPlayer->GetTeamNum() == TEAM_TT)
	{
		return TT_Color;
	}

	return Color::White();
}

Color visuals::GetPlayerVisibleColor(C_BaseEntity* local,C_BaseEntity* pPlayer)
{
	Color PlayerColor = GetPlayerColor(pPlayer);

	bool SetColor = false;

	// Подсвечивать зелёным если игрок виден:

	if (MiscFunctions::IsVisible(local,pPlayer))
	{
		if (g_Options.Visuals.esp_Visible == 0 && pPlayer->GetTeamNum() != local->GetTeamNum()) // Противников
		{
			SetColor = true;
		}
		else if (g_Options.Visuals.esp_Visible == 1 && pPlayer->GetTeamNum() == local->GetTeamNum()) // Своих
		{
			SetColor = true;
		}
		else if (g_Options.Visuals.esp_Visible == 2) // Всех
		{
			SetColor = true;
		}

		Color VCT_Color(int(g_Options.Color.esp_Color_VCT[0] * 255.f), int(g_Options.Color.esp_Color_VCT[1] * 255.f), int(g_Options.Color.esp_Color_VCT[2] * 255.f));
		Color VTT_Color(int(g_Options.Color.esp_Color_VTT[0] * 255.f), int(g_Options.Color.esp_Color_VTT[1] * 255.f), int(g_Options.Color.esp_Color_VTT[2] * 255.f));

		if (SetColor)
		{
			if (pPlayer->GetTeamNum() == TEAM_CT)
			{
				PlayerColor = VCT_Color;
			}
			else if (pPlayer->GetTeamNum() == TEAM_TT)
			{
				PlayerColor = VTT_Color;
			}
		}
	}

	return PlayerColor;
}
void visuals::DrawFov()
{


		//(jmp_label31)
		int xs;
		C_BaseEntity *pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

		int ys;
		float FoV;
		if (!pWeapon)
			return;

		else if (MiscFunctions::IsSniper(pWeapon))
		{
			if (g_Options.Legitbot.Sniper_drawfov)
			{ 
			FoV = g_Options.Legitbot.Sniperfov;
			Interfaces::Engine()->GetScreenSize(xs, ys);
			Interfaces::Engine()->GetScreenSize(xs, ys);
			xs /= 2; ys /= 2;
			g_Render->DrawCircle(xs, ys, FoV * 14.25, FoV * 14.25, Color(255, 255, 255, 255));
			}
		}
		else if (MiscFunctions::IsPistol(pWeapon))
		{
			if (g_Options.Legitbot.Pistol_drawfov)
			{
				FoV = g_Options.Legitbot.Pistolfov;
				Interfaces::Engine()->GetScreenSize(xs, ys);
				Interfaces::Engine()->GetScreenSize(xs, ys);
				xs /= 2; ys /= 2;
				g_Render->DrawCircle(xs, ys, FoV * 14.25, FoV * 14.25, Color(255, 255, 255, 255));
			}
		}
		else if (MiscFunctions::IsSmg(pWeapon))
		{
			if (g_Options.Legitbot.smg_drawfov)
			{
				FoV = g_Options.Legitbot.smgfov;
				Interfaces::Engine()->GetScreenSize(xs, ys);
				Interfaces::Engine()->GetScreenSize(xs, ys);
				xs /= 2; ys /= 2;
				g_Render->DrawCircle(xs, ys, FoV * 14.25, FoV * 14.25, Color(255, 255, 255, 255));
			}
		}
		else if (MiscFunctions::IsRifle(pWeapon))
		{
			if (g_Options.Legitbot.main_drawfov)
			{
				FoV = g_Options.Legitbot.Mainfov;
				Interfaces::Engine()->GetScreenSize(xs, ys);
				Interfaces::Engine()->GetScreenSize(xs, ys);
				xs /= 2; ys /= 2;
				g_Render->DrawCircle(xs, ys, FoV * 14.25, FoV * 14.25, Color(255, 255, 255, 255));
			}
		}
		else if (MiscFunctions::IsHeavy(pWeapon))
		{
			if (g_Options.Legitbot.heavy_drawfov)
			{
				FoV = g_Options.Legitbot.heavyfov;
				Interfaces::Engine()->GetScreenSize(xs, ys);
				Interfaces::Engine()->GetScreenSize(xs, ys);
				xs /= 2; ys /= 2;
				g_Render->DrawCircle(xs, ys, FoV * 14.25, FoV * 14.25, Color(255, 255, 255, 255));
			}
		}
		else if (MiscFunctions::IsRevolver(pWeapon))
		{
			if (g_Options.Legitbot.Revolver_drawfov)
			{
				FoV = g_Options.Legitbot.Revolverfov;
				Interfaces::Engine()->GetScreenSize(xs, ys);
				Interfaces::Engine()->GetScreenSize(xs, ys);
				xs /= 2; ys /= 2;
				g_Render->DrawCircle(xs, ys, FoV * 14.25, FoV * 14.25, Color(255, 255, 255, 255));
			}
		}
	}
void visuals::RecoilCrosshair(C_BaseEntity* local)
{
	Interfaces::Engine()->GetScreenSize(width, height);
	if (local && local->IsAlive())
	{
		static Vector ViewAngles;
		Interfaces::Engine()->GetViewAngles(ViewAngles);
		ViewAngles += local->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

		Vector fowardVec;
		AngleVectors(ViewAngles, &fowardVec);
		fowardVec *= 10000;

		Vector start = local->GetOrigin() + local->GetViewOffset();
		Vector end = start + fowardVec, endScreen;

		if (g_Render->WorldToScreen(end, endScreen) && local->IsAlive())
		{
			g_Render->Line(endScreen.x - 4, endScreen.y, endScreen.x + 4, endScreen.y, Color(int(g_Options.Color.color_recoil[0] * 255.f), int(g_Options.Color.color_recoil[1] * 255.f), int(g_Options.Color.color_recoil[2] * 255.f)));
			g_Render->Line(endScreen.x, endScreen.y - 4, endScreen.x, endScreen.y + 4, Color(int(g_Options.Color.color_recoil[0] * 255.f), int(g_Options.Color.color_recoil[1] * 255.f), int(g_Options.Color.color_recoil[2] * 255.f)));
		}
	}
}
void visuals::RenderFill(RECT rect)
{
Interfaces::Surface()->DrawFilledRect(rect.left + 2, rect.top + 2, rect.right - 2, rect.bottom - 2);

}

void visuals::renderBeams()
{

	if (g_Options.Visuals.bulletshow )
		return;

	auto local = static_cast<C_BaseEntity*>(Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer()));
	if (!local)
		return;

	for (size_t i = 0; i < logs.size(); i++)
	{

		auto current = logs.at(i);

		current.color = Color(int(g_Options.Color.flTracers[0] * 255.f), int(g_Options.Color.flTracers[1] * 255.f), int(g_Options.Color.flTracers[2] * 255.f));

		if (g_Options.Visuals.bulletshow)

		Interfaces::DebugOverlay()->AddLineOverlay(current.src, current.dst, current.color.r(), current.color.g(), current.color.g(), true, -1.f);


		Interfaces::DebugOverlay()->AddBoxOverlay(current.dst, Vector(-2, -2, -2), Vector(2, 2, 2), Vector(0, 0, 0), current.color.r(), current.color.g(), current.color.g(), 127, -1.f);

		if (fabs(Interfaces::Globals()->curtime - current.time) > 5.f)
			logs.erase(logs.begin() + i);
	}
}


void visuals::WorldModes()
{
	static auto postprocess_enable = Interfaces::CVar()->FindVar("mat_postprocess_enable");
	static auto postprocess_enable_s = new SpoofedConvar(postprocess_enable);


	if (g_Options.Misc.postprocess && g_Options.Visuals.Enabled)
	{
		postprocess_enable_s->SetInt(0);
	}
	else
	{
		postprocess_enable_s->SetInt(1);
	}


	static auto snowmode = Interfaces::CVar()->FindVar("mat_drawgray");
	static auto snowmode_s = new SpoofedConvar(snowmode);

	if (g_Options.Misc.Snow && g_Options.Visuals.Enabled)
	{
		snowmode_s->SetInt(1);
	}
	else
	{
		snowmode_s->SetInt(0);
	}


	static auto lsdmode_enabled = Interfaces::CVar()->FindVar("mat_showmiplevels");
	static auto  lsdmode_enabled_s = new SpoofedConvar(lsdmode_enabled);


	if (g_Options.Misc.lsdmode && g_Options.Visuals.Enabled)
	{

		lsdmode_enabled_s->SetInt(1);
	}
	else
	{
		lsdmode_enabled_s->SetInt(0);
	}


	static auto chrome_enabled = Interfaces::CVar()->FindVar("r_showenvcubemap");
	static auto  chrome_enabled_s = new SpoofedConvar(chrome_enabled);


	if (g_Options.Misc.chromemode && g_Options.Visuals.Enabled)
	{
		chrome_enabled_s->SetInt(1);
	}
	else
	{
		chrome_enabled_s->SetInt(0);
	}


	static auto minecraftmode = Interfaces::CVar()->FindVar("mat_showlowresimage");
	static auto  minecraftmode_s = new SpoofedConvar(minecraftmode);


	if (g_Options.Misc.minecraftmode && g_Options.Visuals.Enabled)
	{
		minecraftmode_s->SetInt(1);
	}
	else
	{
		minecraftmode_s->SetInt(0);
	}


	if (g_Options.Misc.ambientlight && g_Options.Visuals.Enabled)
	{



		float AmbientRedAmount = g_Options.Color.esp_Ambient[0];
		float AmbientGreenAmount = g_Options.Color.esp_Ambient[1];
		float AmbientBlueAmount = g_Options.Color.esp_Ambient[2];



		ConVar* AmbientRedCvar = Interfaces::CVar()->FindVar("mat_ambient_light_r");
		*(float*)((DWORD)&AmbientRedCvar->fnChangeCallback + 0xC) = NULL;
		AmbientRedCvar->SetValue(AmbientRedAmount);

		ConVar* AmbientGreenCvar = Interfaces::CVar()->FindVar("mat_ambient_light_g");
		*(float*)((DWORD)&AmbientGreenCvar->fnChangeCallback + 0xC) = NULL;
		AmbientGreenCvar->SetValue(AmbientGreenAmount);

		ConVar* AmbientBlueCvar = Interfaces::CVar()->FindVar("mat_ambient_light_b");
		*(float*)((DWORD)&AmbientBlueCvar->fnChangeCallback + 0xC) = NULL;
		AmbientBlueCvar->SetValue(AmbientBlueAmount);
	}

}

void visuals::custommodels(C_BaseEntity* entity)
{
	if (entity->GetTeamNum() == TEAM_CT)
	{
		switch (g_Options.Misc.customodelsct)
		{
		case 0: 
		{

		}
		break;
		case 1:
			PrecacheModel("models/player/custom_player/maoling/neptunia/vert/normal/vert.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/vert/normal/vert.mdl"));
			break;

		case 2:
			PrecacheModel("models/player/custom_player/maoling/neptunia/uzume/normal/uzume.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/uzume/normal/uzume.mdl"));
			break;

		case 3:
			PrecacheModel("models/player/custom_player/maoling/neptunia/uni/normal/uni.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/uni/normal/uni.mdl"));
			break;

		case 4:
			PrecacheModel("models/player/custom_player/maoling/neptunia/s-sha/normal/s-sha.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/s-sha/normal/s-sha.mdl"));
			break;
		case 5:
			PrecacheModel("models/player/custom_player/maoling/neptunia/pururut/normal/pururut.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/pururut/normal/pururut.mdl"));
			break;
		case 6:
			PrecacheModel("models/player/custom_player/maoling/neptunia/noire/normal/noire.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/noire/normal/noire.mdl"));
			break;
		case 7:
			PrecacheModel("models/player/custom_player/maoling/neptunia/neptune/swimsuit/neptune.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/neptune/swimsuit/neptune.mdl"));
			break;
		case 8:
			PrecacheModel("models/player/custom_player/maoling/neptunia/neptune/swimwear/neptune.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/neptune/swimwear/neptune.mdl"));
			break;
		case 9:
			PrecacheModel("models/player/custom_player/maoling/neptunia/histoire/normal/histoire.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/histoire/normal/histoire.mdl"));
			break;
		case 10:
			PrecacheModel("models/player/custom_player/maoling/neptunia/hatsumi_sega/normal/sega_girl.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/hatsumi_sega/normal/sega_girl.mdl"));
			break;
		case 11:
			PrecacheModel("models/player/custom_player/maoling/neptunia/blanc/normal/blanc.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/blanc/normal/blanc.mdl"));
			break;
		case 12:
			PrecacheModel("models/player/custom_player/maoling/neptunia/adult_neptune/normal/neptune.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/adult_neptune/normal/neptune.mdl"));
			break;
		}
	}
	else if (entity->GetTeamNum() == TEAM_TT)
	{
		switch (g_Options.Misc.customodelst)
		{
		case 0: 
		{

		}
		break;
		case 1:
			PrecacheModel("models/player/custom_player/maoling/neptunia/vert/normal/vert.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/vert/normal/vert.mdl"));
			break;

		case 2:
			PrecacheModel("models/player/custom_player/maoling/neptunia/uzume/normal/uzume.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/uzume/normal/uzume.mdl"));
			break;

		case 3:
			PrecacheModel("models/player/custom_player/maoling/neptunia/uni/normal/uni.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/uni/normal/uni.mdl"));
			break;

		case 4:
			PrecacheModel("models/player/custom_player/maoling/neptunia/s-sha/normal/s-sha.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/s-sha/normal/s-sha.mdl"));
			break;
		case 5:
			PrecacheModel("models/player/custom_player/maoling/neptunia/pururut/normal/pururut.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/pururut/normal/pururut.mdl"));
			break;
		case 6:
			PrecacheModel("models/player/custom_player/maoling/neptunia/noire/normal/noire.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/noire/normal/noire.mdl"));
			break;
		case 7:
			PrecacheModel("models/player/custom_player/maoling/neptunia/neptune/swimsuit/neptune.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/neptune/swimsuit/neptune.mdl"));
			break;
		case 8:
			PrecacheModel("models/player/custom_player/maoling/neptunia/neptune/swimwear/neptune.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/neptune/swimwear/neptune.mdl"));
			break;
		case 9:
			PrecacheModel("models/player/custom_player/maoling/neptunia/neptune/swimwear/neptune.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/neptune/swimwear/neptune.mdl"));
			break;
		case 10:
			PrecacheModel("models/player/custom_player/maoling/neptunia/histoire/normal/histoire.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/histoire/normal/histoire.mdl"));
			break;
		case 11:
			PrecacheModel("models/player/custom_player/maoling/neptunia/blanc/normal/blanc.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/blanc/normal/blanc.mdl"));
			break;
		case 12:
			PrecacheModel("models/player/custom_player/maoling/neptunia/adult_neptune/normal/neptune.mdl");
			entity->SetModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/player/custom_player/maoling/neptunia/adult_neptune/normal/neptune.mdl"));
			break;

		}

	}
}

void visuals::Hitbox(int index)
{
	if (g_Options.Visuals.HitboxDuration == 0.f || !g_Options.Visuals.Hitbox)
		return;

	C_BaseEntity* pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	float duration = g_Options.Visuals.HitboxDuration;

	if (index < 0)
		return;

	Color color = Color(int(g_Options.Color.hitbox[0] * 255.f), int(g_Options.Color.hitbox[1] * 255.f), int(g_Options.Color.hitbox[2] * 255.f));

	auto entity = reinterpret_cast<C_BaseEntity*>(Interfaces::EntityList()->GetClientEntity(index));

	if (!entity)
		return;

	studiohdr_t* pStudioModel = Interfaces::ModelInfo()->GetStudiomodel(entity->GetModel());

	if (!pStudioModel)
		return;

	static matrix3x4_t pBoneToWorldOut[128];

	if (!entity->SetupBones(pBoneToWorldOut, MAXSTUDIOBONES, 256, Interfaces::Globals()->curtime))
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->GetHitboxSet(0);
	if (!pHitboxSet)
		return;

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox(i);
		if (!pHitbox)
			continue;

		Vector vMin, vMax;
		VectorTransform(pHitbox->bbmin, pBoneToWorldOut[pHitbox->bone], vMin); //nullptr???
		VectorTransform(pHitbox->bbmax, pBoneToWorldOut[pHitbox->bone], vMax);

		if (pHitbox->m_flRadius > -1)
		{
			Interfaces::DebugOverlay()->AddCapsuleOverlay(vMin, vMax, pHitbox->m_flRadius, color.r(), color.g(), color.b(), 100, duration);
		}
	}
}


void visuals::DrawCrosshair(C_BaseEntity* local, CBaseCombatWeapon* Weapon,Color color)
{
	int screenW, screenH;
    Interfaces::Engine()->GetScreenSize(screenW, screenH);

	int crX = screenW / 2, crY = screenH / 2;
	int dy = screenH / G::FOV;
	int dx = screenW / G::FOV;
	int drX;
	int drY;

	if (g_Options.Visuals.CrosshairType)
	{
		if (g_Options.Misc.RemovalsVisualRecoil)
		{
			drX = crX - (int)(dx * (local->localPlayerExclusive()->GetAimPunchAngle().y * 2));
			drY = crY + (int)(dy * (local->localPlayerExclusive()->GetAimPunchAngle().x * 2));
		}
		else
		{
			drX = crX - (int)(dx * (((local->localPlayerExclusive()->GetAimPunchAngle().y * 2.f) * 0.45f) + local->localPlayerExclusive()->GetViewPunchAngle().y));
			drY = crY + (int)(dy * (((local->localPlayerExclusive()->GetAimPunchAngle().x * 2.f) * 0.45f) + local->localPlayerExclusive()->GetViewPunchAngle().x));
		}
	}
	else
	{
		drX = crX;
		drY = crY;
	}

	if (g_Options.Visuals.CrosshairStyle == 0)
	{
		//D::DrawCircle( drX, drY, 5, 30, color );
		if (local->IsAlive() && Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
		{
			auto accuracy = 0.f;

			if (g_Options.Visuals.CrosshairSpread)
				accuracy = (Weapon->GetInaccuracy() + Weapon->GetSpread()) * 500.f;
			else
				accuracy = 0.f;

		g_Render->Line(crX + accuracy, crY, crX + 5 + accuracy, crY, color);
		g_Render->Line(crX - 5 - accuracy, crY, crX - accuracy, crY, color);
		g_Render->Line(crX, crY + accuracy, crX, crY + 5 + accuracy, color);
		g_Render->Line(crX, crY - 5 - accuracy, crX, crY - accuracy, color);

			if (g_Options.Visuals.CrosshairType)
				g_Render->DrawBox(drX - 1, drY - 1, 3, 3, color);
		}
	}
	if (g_Options.Visuals.CrosshairStyle == 1)
	{
		//D::DrawCircle( drX, drY, 5, 30, color );
		if (local->IsAlive() && Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
		{

			auto accuracy = (Weapon->GetInaccuracy() + Weapon->GetSpread()) * 500.f;

			if (Weapon->IsValidWeapon())
			g_Render->DrawCircle(drX, drY, g_Options.Visuals.CrosshairSpread ? accuracy + 3 : 6, 30, color);
			else
			g_Render->DrawCircle(drX, drY, 6, 30, color);
		}
		else
		{
			g_Render->DrawCircle(drX, drY, 6, 30, color);
		}
	}

	if (g_Options.Visuals.CrosshairStyle == 2)
	{
		//D::DrawOutlinedRect( drX - 5, drY - 5, 10, 30, color );


		POINT Middle; Middle.x = drX; Middle.y = drY;
		int a = (int)(drY / 30);
		float gamma = atan(a / a);

		static int rotationdegree = 0;

		if (rotationdegree > 89) { rotationdegree = 0; }

		for (int i = 0; i < 4; i++)
		{
			std::vector <int> p;
			p.push_back(a * sin(DEG2RAD(rotationdegree + (i * 90))));									//p[0]		p0_A.x
			p.push_back(a * cos(DEG2RAD(rotationdegree + (i * 90))));									//p[1]		p0_A.y
			p.push_back((a / cos(gamma)) * sin(DEG2RAD(rotationdegree + (i * 90) + RAD2DEG(gamma))));	//p[2]		p0_B.x
			p.push_back((a / cos(gamma)) * cos(DEG2RAD(rotationdegree + (i * 90) + RAD2DEG(gamma))));	//p[3]		p0_B.y

			g_Render->Line(Middle.x, Middle.y, Middle.x + p[0], Middle.y - p[1], color);
			//D::DrawLine(Middle.x + p[0], Middle.y - p[1], Middle.x + p[2], Middle.y - p[3], Color(255, 255, 0, 255));
		}
		rotationdegree++;
	}
}
void visuals::DrawInfo(RECT rect, C_BaseEntity* entity, C_BaseEntity* local)
{
	PlayerInfo info;
	static class Text
	{
	public:
		std::string text;
		int side;
		int Font;
		Color color;

		Text(std::string text, int side, int Font, Color color) : text(text), side(side), Font(Font), color(color)
		{
		}
	};
	std::vector< Text > texts;
	if (Interfaces::Engine()->GetPlayerInfo(entity->EntIndex(), &info))
	{
		if (entity == BombCarrier && g_Options.Visuals.BombCarrier &&  g_Options.Visuals.Enabled)
		{
			texts.push_back(Text("Bomb Carrier", 1, g_Render->font.ESP, Color(255, 220, 220, 255)));
		}

		if (g_Options.Visuals.Flashed && entity->IsFlashed() && g_Options.Visuals.Enabled)
		{
			texts.push_back(Text("Flashed", 1, g_Render->font.ESP, Color(255, 255, 183, 255)));
		}

		if (g_Options.Visuals.IsHasDefuser && entity->hasDefuser() && g_Options.Visuals.Enabled)
		{
			texts.push_back(Text("r", 1, g_Render->font.Icon, Color(255, 255, 183, 255))); //b1g defuser icon
		}

		if (g_Options.Visuals.IsDefusing &&  entity->IsDefusing() && g_Options.Visuals.Enabled)
		{
			texts.push_back(Text("Defusing", 1, g_Render->font.ESP, Color(255, 255, 183, 255)));
		}

		if (g_Options.Visuals.Name && g_Options.Visuals.Enabled)
		{
			texts.push_back(Text(info.m_szPlayerName, 0, g_Render->font.ESP, Color(255, 255, 255, 255)));
		}

		switch (g_Options.Visuals.Armor2)
		{
		case 0:
			break;
		case 1:
			texts.push_back(Text(entity->GetArmorName(), 1, g_Render->font.ESP, Color(255, 255, 255, 255)));
			break;
		case 2:
			texts.push_back(Text(entity->GetArmorIcon(), 1, g_Render->font.Icon, Color(255, 255, 255, 255)));
			break;
		default:
			break;
		}

		if (g_Options.Visuals.Distance && g_Options.Visuals.Enabled)
		{
			texts.push_back(Text(std::to_string(flGetDistance(local->GetOrigin(), entity->GetOrigin())) + std::string("FT"), 2, g_Render->font.ESP, Color(255, 255, 255, 255)));
		}
		if (g_Options.Visuals.Callout && g_Options.Visuals.Enabled)
			texts.push_back(Text(entity->GetCallout(), 1, g_Render->font.ESP, Color(255, 220, 220, 255)));

		if (g_Options.Visuals.resolveMode && g_Options.Visuals.Enabled)
		{
			if (Globals::resolvemode == 1)
			{
				texts.push_back(Text(std::string("Resolver: Bruteforce"), 0, g_Render->font.ESP, Color(255, 255, 183, 255)));
			}
			else if (Globals::resolvemode == 2)
			{
				texts.push_back(Text(std::string("Resolver: Predicting LBY"), 0, g_Render->font.ESP, Color(255, 255, 183, 255)));
			}
			else if (Globals::resolvemode == 3)
			{
				texts.push_back(Text(std::string("Resolver: LBY Update"), 0, g_Render->font.ESP, Color(255, 255, 183, 255)));
			}
			else if (Globals::resolvemode == 4)
			{
				texts.push_back(Text(std::string("Resolver: Legit Player"), 0, g_Render->font.ESP, Color(255, 255, 183, 255)));
			}
		}

		CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle(entity->GetActiveWeaponHandle());
		if (weapon)
		{
			switch (g_Options.Visuals.Weapon)
			{
			case 0:
				break;
			case 1:
				if (MiscFunctions::IsKnife(weapon) || MiscFunctions::IsGrenade(weapon))
				{
					texts.push_back(Text(weapon->GetGunName(), 2, g_Render->font.ESP, Color(255, 255, 255, 255)));
				}

				else
					texts.push_back(Text(weapon->GetGunName() + std::string("[") + std::to_string(weapon->ammo()) + std::string("|") + std::to_string(weapon->ammo2()) + std::string("]"), 2, g_Render->font.ESP, Color(255, 255, 255, 255)));
				break;
			case 2:
				texts.push_back(Text(weapon->GetGunIcon(), 2, g_Render->font.Icon, Color(255, 255, 255, 255)));
				break;
			default:
				break;
			}
		}

		if (g_Options.Visuals.Scoped && entity->IsScoped() && g_Options.Visuals.Enabled)
		{
			texts.push_back(Text("SCOPE", 1, g_Render->font.ESPan, Color(0, 202, 247, 255)));
		}

		if (g_Options.Visuals.IsReloading && g_Options.Visuals.Enabled && weapon->IsInReload())
			texts.push_back(Text("Reloading", 2, g_Render->font.ESP, Color(121, 252, 250, 255)));

		if (g_Options.Visuals.Money && g_Options.Visuals.Enabled)
		{
			texts.push_back(Text(std::string("$") + std::to_string(entity->iAccount()), 1, g_Render->font.ESP, Color(71, 132, 60, 255)));
		}

	}


	int middle = ((rect.right - rect.left) / 2) + rect.left;
	int Top[3] = { rect.top,rect.top, rect.bottom };
	for (auto text : texts)
	{
		RECT nameSize = g_Render->GetTextSize(text.Font, (char*)text.text.c_str());
		switch (text.side)
		{
		case 0:
			Top[0] -= nameSize.bottom + 1;
			g_Render->DrawString2(text.Font, middle, Top[0] + 8, text.color, FONT_CENTER, (char*)text.text.c_str());
			break;
		case 1:
			g_Render->DrawString2(text.Font, rect.right + 2, Top[1] + 8, text.color, FONT_LEFT, (char*)text.text.c_str());
			Top[1] += nameSize.bottom - 4;
			break;
		case 2:
			g_Render->DrawString2(text.Font, middle, Top[2] + 8, text.color, FONT_CENTER, (char*)text.text.c_str());
			Top[2] += nameSize.bottom - 4;
			break;
		}
	}

}
void visuals::Skeleton(C_BaseEntity* pEntity, Color Col)
{
	studiohdr_t* pStudioHdr = Interfaces::ModelInfo()->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && pBone->flags & BONE_USED_BY_HITBOX && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			int iChestBone = 6;  // Parameter of relevant Bone number
			Vector vBreastBone; // New reference Point for connecting many bones
			Vector vUpperDirection = pEntity->GetBonePos(iChestBone + 1) - pEntity->GetBonePos(iChestBone); // direction vector from chest to neck
			vBreastBone = pEntity->GetBonePos(iChestBone) + vUpperDirection / 2;
			Vector vDeltaChild = vChild - vBreastBone; // Used to determine close bones to the reference point
			Vector vDeltaParent = vParent - vBreastBone;

			// Eliminating / Converting all disturbing bone positions in three steps.
			if ((vDeltaParent.Length() < 9 && vDeltaChild.Length() < 9))
				vParent = vBreastBone;

			if (j == iChestBone - 1)
				vChild = vBreastBone;

			if (abs(vDeltaChild.z) < 5 && (vDeltaParent.Length() < 5 && vDeltaChild.Length() < 5) || j == iChestBone)
				continue;

			Interfaces::DebugOverlay()->ScreenPosition(vParent, sParent);
			Interfaces::DebugOverlay()->ScreenPosition(vChild, sChild);

			Interfaces::Surface()->DrawSetColor(Color(int(g_Options.Color.color_skeleton[0] * 255.f), int(g_Options.Color.color_skeleton[1] * 255.f), int(g_Options.Color.color_skeleton[2] * 255.f)));
			Interfaces::Surface()->DrawLine(sParent[0], sParent[1], sChild[0], sChild[1]);
		}
	}
}

void visuals::FingerSkeleton(C_BaseEntity* pEntity, Color Col)
{
	studiohdr_t* pStudioHdr = Interfaces::ModelInfo()->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && pBone->flags & BONE_USED_BY_HITBOX | BONE_USED_BY_ATTACHMENT && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			int iChestBone = 6;  // Parameter of relevant Bone number
			Vector vBreastBone; // New reference Point for connecting many bones
			Vector vUpperDirection = pEntity->GetBonePos(iChestBone + 1) - pEntity->GetBonePos(iChestBone); // direction vector from chest to neck
			vBreastBone = pEntity->GetBonePos(iChestBone) + vUpperDirection / 2;
			Vector vDeltaChild = vChild - vBreastBone; // Used to determine close bones to the reference point
			Vector vDeltaParent = vParent - vBreastBone;

			// Eliminating / Converting all disturbing bone positions in three steps.
			if ((vDeltaParent.Length() < 9 && vDeltaChild.Length() < 9))
				vParent = vBreastBone;

			if (j == iChestBone - 1)
				vChild = vBreastBone;

			if (abs(vDeltaChild.z) < 5 && (vDeltaParent.Length() < 5 && vDeltaChild.Length() < 5) || j == iChestBone)
				continue;

			if ((pBone->flags & BONE_USED_BY_HITBOX ^ BONE_USED_BY_HITBOX) && (vDeltaParent.Length() < 19 && vDeltaChild.Length() < 19))
				continue;

			Interfaces::DebugOverlay()->ScreenPosition(vParent, sParent);
			Interfaces::DebugOverlay()->ScreenPosition(vChild, sChild);

			Interfaces::Surface()->DrawSetColor(Color(int(g_Options.Color.color_skeleton[0] * 255.f), int(g_Options.Color.color_skeleton[1] * 255.f), int(g_Options.Color.color_skeleton[2] * 255.f)));
			Interfaces::Surface()->DrawLine(sParent[0], sParent[1], sChild[0], sChild[1]);
		}
	}
}

void visuals::HealthSkeleton(C_BaseEntity* pEntity)
{

	studiohdr_t* pStudioHdr = Interfaces::ModelInfo()->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);


		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			int iChestBone = 6;  // Parameter of relevant Bone number
			Vector vBreastBone; // New reference Point for connecting many bones
			Vector vUpperDirection = pEntity->GetBonePos(iChestBone + 1) - pEntity->GetBonePos(iChestBone); // direction vector from chest to neck
			vBreastBone = pEntity->GetBonePos(iChestBone) + vUpperDirection / 2;
			Vector vDeltaChild = vChild - vBreastBone; // Used to determine close bones to the reference point
			Vector vDeltaParent = vParent - vBreastBone;

			// Eliminating / Converting all disturbing bone positions in three steps.
			if ((vDeltaParent.Length() < 9 && vDeltaChild.Length() < 9))
				vParent = vBreastBone;

			if (j == iChestBone - 1)
				vChild = vBreastBone;

			if (abs(vDeltaChild.z) < 5 && (vDeltaParent.Length() < 5 && vDeltaChild.Length() < 5) || j == iChestBone)
				continue;

			if (pEntity->GetHealth() == 100)
			{
				Interfaces::DebugOverlay()->ScreenPosition(vParent, sParent);
				Interfaces::DebugOverlay()->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(0, 255, 0));
			}

			if (pEntity->GetHealth() < 100 & pEntity->GetHealth() > 85 || pEntity->GetHealth() == 85)
			{
				Interfaces::DebugOverlay()->ScreenPosition(vParent, sParent);
				Interfaces::DebugOverlay()->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(114, 255, 00));

			}
			else if (pEntity->GetHealth() < 85 & pEntity->GetHealth() > 60 || pEntity->GetHealth() == 60)
			{
				Interfaces::DebugOverlay()->ScreenPosition(vParent, sParent);
				Interfaces::DebugOverlay()->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(178, 255, 0));
			}
			else if (pEntity->GetHealth() < 60 & pEntity->GetHealth() > 45 || pEntity->GetHealth() == 45)
			{
				Interfaces::DebugOverlay()->ScreenPosition(vParent, sParent);
				Interfaces::DebugOverlay()->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 229, 0));
			}
			else if (pEntity->GetHealth() < 45 & pEntity->GetHealth() > 30 || pEntity->GetHealth() == 30)
			{
				Interfaces::DebugOverlay()->ScreenPosition(vParent, sParent);
				Interfaces::DebugOverlay()->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 127, 0));
			}
			else  if (pEntity->GetHealth() < 30 & pEntity->GetHealth() > 15 || pEntity->GetHealth() == 15)
			{
				Interfaces::DebugOverlay()->ScreenPosition(vParent, sParent);
				Interfaces::DebugOverlay()->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 55, 0));
			}
			else  if (pEntity->GetHealth() < 15 & pEntity->GetHealth() > 0)
			{
				Interfaces::DebugOverlay()->ScreenPosition(vParent, sParent);
				Interfaces::DebugOverlay()->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(191, 0, 0));
			}
		}
	}
}
void visuals::DrawBomb(C_BaseEntity* entity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)entity;
	Vector vOrig; Vector vScreen;
	vOrig = entity->GetOrigin();
	bool adopted = true;
	auto parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		C_BaseEntity* pParentEnt = (Interfaces::EntityList()->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}
	if (g_Options.Visuals.C4World)
	{
		if (adopted)
		{
			if (g_Render->WorldToScreen(vOrig, vScreen))
			{
				g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(255, 20, 20, 255), g_Render->font.ESP, "Bomb");
			}
		}

	}


}
void visuals::DrawBombPlanted(C_BaseEntity* entity, C_BaseEntity* local)
{
	BombCarrier = nullptr;

	float damage;
	char bombdamagestringdead[24];
	char bombdamagestringalive[24];

	Vector vOrig; Vector vScreen;
	vOrig = entity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)entity;
	float flBlow = Bomb->GetC4BlowTime();
	float lifetime = flBlow - (Interfaces::Globals()->interval_per_tick * local->GetTickBase());
	if (g_Render->WorldToScreen(vOrig, vScreen))
	{
		if (local->IsAlive())
		{
			float flDistance = local->GetEyePosition().DistTo(entity->GetEyePosition());
			float a = 450.7f;
			float b = 75.68f;
			float c = 789.2f;
			float d = ((flDistance - b) / c);
			float flDamage = a * exp(-d * d);

			damage = float((std::max)((int)ceilf(CSGO_Armor(flDamage, local->ArmorValue())), 0));

			sprintf_s(bombdamagestringdead, sizeof(bombdamagestringdead) - 1, "Health Left: 0");
			sprintf_s(bombdamagestringalive, sizeof(bombdamagestringalive) - 1, "Health Left: %.0f", local->GetHealth() - damage);
			if (lifetime > -2.f)
			{
				if (damage >= local->GetHealth())
				{
					g_Render->Text((int)vScreen.x, int(vScreen.y + 10), Color(250, 42, 42, 255), g_Render->font.ESP, bombdamagestringdead);
				}
				else if (local->GetHealth() > damage)
				{
					g_Render->Text((int)vScreen.x, int(vScreen.y + 10), Color(0, 255, 0, 255), g_Render->font.ESP, bombdamagestringalive);
				}
			}
		}
		char buffer[64];
		if (lifetime > 0.01f && !Bomb->IsBombDefused())
		{
			sprintf_s(buffer, "Bomb: %.1f", lifetime);
			g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(250, 42, 42, 255), g_Render->font.ESP, buffer);
		}


	}

	Interfaces::Engine()->GetScreenSize(width, height);
	int halfX = width / 2;
	int halfY = height / 2;


	if (Bomb->GetBombDefuser() > 0)
	{
		float countdown = Bomb->GetC4DefuseCountDown() - (local->GetTickBase() * Interfaces::Globals()->interval_per_tick);
		if (countdown > 0.01f)
		{
			if (lifetime > countdown)
			{
				char defuseTimeString[24];
				sprintf_s(defuseTimeString, sizeof(defuseTimeString) - 1, "Defusing: %.1f", countdown);
				g_Render->Text(halfX - 50, halfY + 200, Color(0, 255, 0, 255), g_Render->font.ESP, defuseTimeString);
			}
			else
			{
				g_Render->Text(halfX - 50, halfY + 200, Color(255, 0, 0, 255), g_Render->font.ESP, "RIP "  __DATE__ " :(");
			}
		}

	}
}
void visuals::RenderWeapon(C_BaseEntity* entity, Color color)
{

	if (entity)
	{
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)entity;

		auto owner = pWeapon->GetOwnerHandle();

		if (owner > -1)
			return;

		Vector pos3D = entity->GetAbsOrigin2();

		if (pos3D.x == 0.0f && pos3D.y == 0.0f && pos3D.z == 0.0f)
			return;

		Vector pos;


		if (!g_Render->WorldToScreen(pos3D, pos))
			return;

		int weaponID = *pWeapon->GetItemDefinitionIndex();
		auto weaponName = ItemDefinitionIndexToString(weaponID);
		auto weaponIcon = ItemDefinitionIndexToString2(weaponID);

		switch (g_Options.Visuals.DroppedGunsType)
		{
		case 0:
			break;
		case 1:
			g_Render->Text(pos.x, pos.y, color, g_Render->font.Guns, weaponName);
			break;
		case 2:
			g_Render->Text(pos.x, pos.y, color, g_Render->font.Icon, weaponIcon);
			break;
		default:
			break;
		}

	}
}
