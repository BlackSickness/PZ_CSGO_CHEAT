#pragma once
#include "Configuration.hpp"
#include "dropboxes.h"
#include "Variables.h"
#include <cctype>
#include "Logging.h"
#include "Listener.h"
#include "EnginePrediction.h"
#include "SkinParser.h"
#include "SL.h"
#include "Radar.h"
#include <fstream>
#include <Windows.h>
#include <thread>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "ImGui\imgui.h"
#include "ImGui\examples\directx9_example\imgui_impl_dx9.h"
#include "ImGUI\imgui_internal.h"
#include "Gui/Gui.h"

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define BUILDSTAMP ( __DATE__ )


string BaseDir = "";
string LogFile = "";
string GuiFile = "";
string IniFile = "";
vector<string> ConfigList;



void ReadConfigs(LPCTSTR lpszFileName)
{
	if (!strstr(lpszFileName, "gui.ini"))
	{
		ConfigList.push_back(lpszFileName);
	}
}

void RefreshConfigs()
{
	ConfigList.clear();
	string ConfigDir = "C:/DARKHOOK/*.ini";
	SearchFiles(ConfigDir.c_str(), ReadConfigs, FALSE);
}


void RenderDrow()
{
	if (g_Options.Radar.radarwindow)
	{
		DrawRadars();
	}
	if (g_Options.Misc.SpecList)
	{
		DrawingSpectatorList();
	}
	if (g_Options.Misc.Weaponslog)
	{
		purchase.on_fire_weapons(nullptr, true);
	}
	if (g_Options.Misc.DMGlog)
	{
		purchase.on_fire_DMG(nullptr, true);
	}
}


int ParseSkins()
{
	valve_parser::Document doc;
	auto r = doc.Load(".\\csgo\\scripts\\items\\items_game.txt", valve_parser::ENCODING::UTF8);
	if (!r)
		return 1;

	valve_parser::Document english;
	r = english.Load(".\\csgo\\resource\\csgo_english.txt", valve_parser::ENCODING::UTF16_LE);
	if (!r)
		return 2;

	auto weaponSkinCombo = doc.BreadthFirstSearch("weapon_icons");
	if (!weaponSkinCombo || !weaponSkinCombo->ToObject())
		return 3;

	auto skinDataVec = doc.BreadthFirstSearchMultiple("paint_kits");
	if (!skinDataVec.size())
		return 4;

	auto PaintKitNames = english.BreadthFirstSearch("Tokens");
	if (!PaintKitNames || !PaintKitNames->ToObject())
		return 5;

	//std::unordered_map<std::string, std::set<std::string>> G::weaponSkins;
	//std::unordered_map<std::string, skinInfo> G::skinMap;
	//std::unordered_map<std::string, std::string> G::skinNames;

	std::vector<std::string> weaponNames = {
		"deagle",
		"elite",
		"fiveseven",
		"glock",
		"ak47",
		"aug",
		"awp",
		"famas",
		"g3sg1",
		"galilar",
		"m249",
		"m4a1_silencer", //needs to be before m4a1 else silencer doesnt get filtered out :D
		"m4a1",
		"mac10",
		"p90",
		"mp5sd",
		"ump45",
		"xm1014",
		"bizon",
		"mag7",
		"negev",
		"sawedoff",
		"tec9",
		"hkp2000",
		"mp7",
		"mp9",
		"nova",
		"p250",
		"scar20",
		"sg556",
		"ssg08",
		"usp_silencer",
		"cz75a",
		"revolver",
		"knife_m9_bayonet", //needs to be before bayonet else knife_m9_bayonet doesnt get filtered out :D
		"bayonet",
		"knife_flip",
		"knife_gut",
		"knife_karambit",
		"knife_tactical",
		"knife_falchion",
		"knife_survival_bowie",
		"knife_butterfly",
		"knife_push",
		"studded_bloodhound_gloves",
		"sporty_gloves",
		"slick_gloves",
		"leather_handwraps",
		"motorcycle_gloves",
		"specialist_gloves"

	};

	//populate G::weaponSkins
	for (auto child : weaponSkinCombo->children)
	{
		if (child->ToObject())
		{
			for (auto weapon : weaponNames)
			{
				auto skinName = child->ToObject()->GetKeyByName("icon_path")->ToKeyValue()->Value.toString();
				auto pos = skinName.find(weapon);
				//filter out the skinname
				if (pos != std::string::npos)
				{
					auto pos2 = skinName.find_last_of('_');
					g_Options.Skinchanger.weaponSkins[weapon].insert(
						skinName.substr(pos + weapon.length() + 1,
							pos2 - pos - weapon.length() - 1)
					);
					break;
				}
			}
		}
	}

	//populate skinData
	for (auto skinData : skinDataVec)
	{
		if (skinData->ToObject())
		{
			for (auto skin : skinData->children)
			{
				if (skin->ToObject())
				{
					skinInfo si;
					si.paintkit = skin->ToObject()->name.toInt();

					auto skinName = skin->ToObject()->GetKeyByName("name")->ToKeyValue()->Value.toString();
					auto tagNode = skin->ToObject()->GetKeyByName("description_tag");
					if (tagNode)
					{
						std::string tag = tagNode->ToKeyValue()->Value.toString();
						tag = tag.substr(1, std::string::npos); //skip #
						std::transform(tag.begin(), tag.end(), tag.begin(), towlower);
						si.tagName = tag;
					}

					auto keyVal = skin->ToObject()->GetKeyByName("seed");
					if (keyVal != nullptr)
						si.seed = keyVal->ToKeyValue()->Value.toInt();

					g_Options.Skinchanger.skinMap[skinName] = si;
				}
			}
		}
	}

	//populate G::skinNames
	for (auto child : PaintKitNames->children)
	{
		if (child->ToKeyValue())
		{
			std::string key = child->ToKeyValue()->Key.toString();
			std::transform(key.begin(), key.end(), key.begin(), towlower);
			if (key.find("paintkit") != std::string::npos &&
				key.find("tag") != std::string::npos)
			{
				g_Options.Skinchanger.skinNames[key] = child->ToKeyValue()->Value.toString();
			}
		}
	}

	return 0;
}



void initializeskins()
{
	static bool once = false;

	if (!once)
	{
		ParseSkins();
		once = true;
	}
}
void FullUpdate()
{
	CSkin::instance().SetKillIconCfg();

	typedef void(*ForceUpdate) (void);
	ForceUpdate FullUpdate = (ForceUpdate)CSX::Memory::FindPatternV2(ENGINE_DLL, "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
	FullUpdate();
}
void dankestSetClanTag(const char* tag, const char* name)
{
	static auto ClanTagOffset = CSX::Memory::FindPattern(ENGINE_DLL, "\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "xxxxxxxxx",0);
	if (ClanTagOffset)
	{
		if (strlen(name) > 0) {
			auto name_ = std::string(name);
			auto newline = name_.find("\\n");
			auto tab = name_.find("\\t");
			if (newline != std::string::npos) {
				name_.replace(newline, newline + 2, "\n");
			}
			if (tab != std::string::npos) {
				name_.replace(tab, tab + 2, "\t");
			}
		}
		auto tag_ = std::string(tag);
		if (strlen(tag) > 0) {
			auto newline = tag_.find("\\n");
			auto tab = tag_.find("\\t");
			if (newline != std::string::npos) {
				tag_.replace(newline, newline + 2, "\n");
			}
			if (tab != std::string::npos) {
				tag_.replace(tab, tab + 2, "\t");
			}
		}
		static auto dankesttSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(ClanTagOffset);
		dankesttSetClanTag(tag_.data(), tag_.data());
	}
}
static const char* rare[] =
{
	"None",
	"1 - Consumer grade (white)",
	"2 - Industrial grade (light blue)",
	"3 - Mil-Spec (darker blue)",
	"4 - Restricted (purple)",
	"5 - Classified (pinkish purple)",
	"6 - Covert (red)",
	"7 - Exceedingly Rare (gold)",
};
const char* weaponnames(int id)
{
	switch (id)
	{
	case 1:
		return "deagle";
	case 2:
		return "elite";
	case 3:
		return "fiveseven";
	case 4:
		return "glock";
	case 7:
		return "ak47";
	case 8:
		return "aug";
	case 9:
		return "awp";
	case 10:
		return "famas";
	case 11:
		return "g3sg1";
	case 13:
		return "galilar";
	case 14:
		return "m249";
	case 60:
		return "m4a1_silencer";
	case 16:
		return "m4a1";
	case 17:
		return "mac10";
	case 19:
		return "p90";
	case 23:
		return "mp5sd";
	case 24:
		return "ump45";
	case 25:
		return "xm1014";
	case 26:
		return "bizon";
	case 27:
		return "mag7";
	case 28:
		return "negev";
	case 29:
		return "sawedoff";
	case 30:
		return "tec9";
	case 32:
		return "hkp2000";
	case 33:
		return "mp7";
	case 34:
		return "mp9";
	case 35:
		return "nova";
	case 36:
		return "p250";
	case 38:
		return "scar20";
	case 39:
		return "sg556";
	case 40:
		return "ssg08";
	case 61:
		return "usp_silencer";
	case 63:
		return "cz75a";
	case 64:
		return "revolver";
	case 508:
		return "knife_m9_bayonet";
	case 500:
		return "bayonet";
	case 505:
		return "knife_flip";
	case 506:
		return "knife_gut";
	case 507:
		return "knife_karambit";
	case 509:
		return "knife_tactical";
	case 512:
		return "knife_falchion";
	case 514:
		return "knife_survival_bowie";
	case 515:
		return "knife_butterfly";
	case 516:
		return "knife_push";

	default:
		return "";
	}
}

bool IsUtility(ItemDefinitionIndexx index)
{
	switch (index)
	{
	case ItemDefinitionIndexx::ITEM_NONE:
	case ItemDefinitionIndexx::WEAPON_C4:
	case ItemDefinitionIndexx::WEAPON_FLASH:
	case ItemDefinitionIndexx::WEAPON_HE:
	case ItemDefinitionIndexx::WEAPON_INC:
	case ItemDefinitionIndexx::WEAPON_MOLOTOV:
	case ItemDefinitionIndexx::WEAPON_SMOKE:
	case ItemDefinitionIndexx::WEAPON_DECOY:
	case ItemDefinitionIndexx::WEAPON_KNIFE_T:
	case ItemDefinitionIndexx::WEAPON_KNIFE_CT:
	case ItemDefinitionIndexx::GLOVE_T_SIDE:
	case ItemDefinitionIndexx::GLOVE_CT_SIDE:
	case ItemDefinitionIndexx::GLOVE_SPORTY:
	case ItemDefinitionIndexx::GLOVE_SLICK:
	case ItemDefinitionIndexx::GLOVE_LEATHER_WRAP:
	case ItemDefinitionIndexx::GLOVE_STUDDED_BLOODHOUND:
	case ItemDefinitionIndexx::GLOVE_MOTORCYCLE:
	case ItemDefinitionIndexx::GLOVE_SPECIALIST:
	case ItemDefinitionIndexx::GLOVE_HYDRA:
		return true;
	default:
		return false;
	}
}

bool Contains(const std::string &word, const std::string &sentence) {
	if (word == "" || sentence == "")
		return true;

	return sentence.find(word) != std::string::npos;
}

std::string ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), (int(*)(int))std::tolower);

	return str;
}
//--------------------------------------------


void Ragebot()
{
	ImGui::Checkbox(XorStr("Active"), &g_Options.Ragebot.MainSwitch);
	ImGui::Separator();
	
		if (ImGui::Button(XorStr("Aimbot##aimbots"), ImVec2(200, 0)))
		{
			g_Options.Menu.iRageSubTab = 0;
		}

		ImGui::SameLine();

		if (ImGui::Button(XorStr("HVH##hh"), ImVec2(200, 0)))
		{
			g_Options.Menu.iRageSubTab = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button(XorStr("Anti Aim##antiaim"), ImVec2(200, 0)))
		{
			g_Options.Menu.iRageSubTab = 2;
		}

		ImGui::Separator();

		

		switch (g_Options.Menu.iRageSubTab)
		{
		case 0:
		{
			ImGui::BeginChild(XorStr("first child"), ImVec2(300, 190), true);
			{
				ImGui::Text(XorStr("Genral"));
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Aim Lock"), &g_Options.Ragebot.Enabled);
				ImGui::Checkbox(XorStr("Auto Fire"), &g_Options.Ragebot.AutoFire);
				ImGui::Checkbox(XorStr("Silent Aim"), &g_Options.Ragebot.Silent);
				ImGui::Checkbox(XorStr("AutoPistol"), &g_Options.Ragebot.AutoPistol);
				ImGui::PushItemWidth(150);
				ImGui::SliderFloat(XorStr("FOV"), &g_Options.Ragebot.FOV, 1.f, 360.f, "%.0f");
				ImGui::Combo(XorStr("Selection"), &g_Options.Ragebot.TargetSelection, "Crosshair\0\rDistance\0\rHealth\0\rThreat\0\0", -1);
			}ImGui::EndChild();
			ImGui::SameLine();

			ImGui::BeginChild(XorStr("second child"), ImVec2(300, 190), true);
			{

				ImGui::Text(XorStr("Accuracy"));
				ImGui::Separator();
				ImGui::Checkbox(XorStr("No Recoil"), &g_Options.Ragebot.AntiRecoil);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Auto Stop"), &g_Options.Ragebot.AutoStop);
				ImGui::Checkbox(XorStr("Friendly Fire"), &g_Options.Ragebot.FriendlyFire);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Auto Scope"), &g_Options.Ragebot.AutoScope);
				ImGui::Checkbox(XorStr("Aim Step"), &g_Options.Ragebot.AimbotAimStep);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Auto Crouch"), &g_Options.Ragebot.AutoCrouch);
				ImGui::Checkbox(XorStr("No Spread"), &g_Options.Ragebot.AntiSpread);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Rage Info"), &g_Options.Ragebot.RageDraw);
				ImGui::Checkbox(XorStr("Auto Zeus "), &g_Options.Ragebot.Zeus);


			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("3 child"), ImVec2(200, 190), true);
			{
				ImGui::PushItemWidth(150);
				ImGui::Checkbox(XorStr("Fakelag"), &g_Options.Ragebot.FakeLag);
				ImGui::Combo(XorStr("Type"), &g_Options.Ragebot.FakeLagMode, fakelag, ARRAYSIZE(fakelag));
				ImGui::SliderInt(XorStr("##Fakelag"), &g_Options.Ragebot.FakeLagAmt, 1.f, 15.f);

			}ImGui::EndChild();

			ImGui::BeginChild(XorStr("fourthly child"), ImVec2(300, 190), true);
			{
				ImGui::Text(XorStr("Min Dmg Wall"));
				ImGui::Separator();
				ImGui::PushItemWidth(184);
				ImGui::Checkbox(XorStr("Auto Wall"), &g_Options.Ragebot.autowall);
				ImGui::SliderFloat(XorStr("Snipers"), &g_Options.Ragebot.MinimumDamageSniper, 1.f, 100.f, "%.2f");
				ImGui::SliderFloat(XorStr("Rifles"), &g_Options.Ragebot.MinimumDamageRifle, 1.f, 100.f, "%.2f");
				ImGui::SliderFloat(XorStr("Pistols"), &g_Options.Ragebot.MinimumDamagePistol, 1.f, 100.f, "%.2f");
				ImGui::SliderFloat(XorStr("Heavies"), &g_Options.Ragebot.MinimumDamageHeavy, 1.f, 100.f, "%.2f");
				ImGui::SliderFloat(XorStr("SMGs"), &g_Options.Ragebot.MinimumDamageSmg, 1.f, 100.f, "%.2f");
				ImGui::SliderFloat(XorStr("Revolver"), &g_Options.Ragebot.MinimumDamageRevolver, 1.f, 100.f, "%.2f");
			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("fifthly child"), ImVec2(300, 190), true);
			{
				ImGui::Text(XorStr("Hitchance"));
				ImGui::Separator();
				ImGui::PushItemWidth(184);
				ImGui::Checkbox(XorStr("Hitchance Enabled"), &g_Options.Ragebot.Hitchance);
				ImGui::SliderFloat(XorStr("Snipers"), &g_Options.Ragebot.HitchanceSniper, 0.f, 100.f, "%.1f");
				ImGui::SliderFloat(XorStr("Rifles"), &g_Options.Ragebot.HitchanceRifle, 0.f, 100.f, "%.1f");
				ImGui::SliderFloat(XorStr("Pistols"), &g_Options.Ragebot.HitchancePistol, 0.f, 100.f, "%.1f");
				ImGui::SliderFloat(XorStr("SMGs"), &g_Options.Ragebot.HitchanceSmgs, 0.f, 100.f, "%.1f");
				ImGui::SliderFloat(XorStr("Heavies"), &g_Options.Ragebot.HitchanceHeavy, 0.f, 100.f, "%.1f");
				ImGui::SliderFloat(XorStr("Revolver"), &g_Options.Ragebot.HitchanceRevolver, 0.f, 100.f, "%.1f");

			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("Seventh child"), ImVec2(200, 190), true);
			{
				ImGui::Text(XorStr("Hit Box"));
				ImGui::Separator();
				ImGui::PushItemWidth(150);
				ImGui::Combo(XorStr("##hb"), &g_Options.Ragebot.Hitbox, aimBones, ARRAYSIZE(aimBones));
				ImGui::Text(XorStr("Hit Scan Mode"));
				ImGui::Combo(XorStr("##hsm"), &g_Options.Ragebot.Hitscan, hitscan, ARRAYSIZE(hitscan));
				ImGui::Checkbox(XorStr("Hitscan InVisible"), &g_Options.Ragebot.hitscaninvis);
			}ImGui::EndChild();
			break;
		}
		case 1:
		{
			ImGui::BeginChild(XorStr("6 child"), ImVec2(300, 180), true);
			{
				ImGui::Text(XorStr("Resolver 1"));
				ImGui::Separator();
				ImGui::Checkbox(XorStr("LBY Correction"), &g_Options.Ragebot.LBYCorrection2);
				ImGui::PushItemWidth(150);
				ImGui::Combo(XorStr("Resolver"), &g_Options.Ragebot.Resolver3, resolvers, ARRAYSIZE(resolvers));

			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("7 child"), ImVec2(300, 180), true);
			{
				ImGui::Text(XorStr("Resolver 2"));
				ImGui::Separator();

				ImGui::Checkbox(XorStr("Resolver"), &g_Options.Ragebot.Resolver);
				ImGui::Text(XorStr("Bruteforce after X bullets:"));
				ImGui::PushItemWidth(150);
				ImGui::SliderFloat(XorStr("Shots: "), &g_Options.Ragebot.bruteAfterX, 0, 10, "%1.f");


			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("9 child"), ImVec2(200, 180), true);
			{
				ImGui::Checkbox(XorStr("Position Adjustment"), &g_Options.Ragebot.PosAdjust);
				ImGui::Checkbox("Fake Lag Fix", &g_Options.Ragebot.FakeLagFix);
			}ImGui::EndChild();
			ImGui::BeginChild(XorStr("8 child"), ImVec2(300, 180), true);
			{
				ImGui::Text(XorStr("Resolver Spread"));
				ImGui::Separator();
				ImGui::Checkbox(XorStr("LBY Correction"), &g_Options.Ragebot.LBYCorrection);
				ImGui::PushItemWidth(150);
				ImGui::Combo(XorStr("Resolver2"), &g_Options.Ragebot.AimbotResolver, "Off\0\rSpread\0\rNoSpreed\0\0", -1);


			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("5 child"), ImVec2(300, 180), true);
			{
				ImGui::Text(XorStr("Multipoint"));
				ImGui::Separator();

				ImGui::Checkbox("Enable", &g_Options.Ragebot.Multipoint);
				ImGui::PushItemWidth(150);
				ImGui::SliderFloat(XorStr("Pointscale X"), &g_Options.Ragebot.Pointscalex, 0.0f, 10.0f);
				ImGui::SliderFloat(XorStr("Pointscale Y"), &g_Options.Ragebot.Pointscaley, 0.0f, 10.0f);
			}ImGui::EndChild();
			
			break;
		}
		case 2:
		{
			ImGui::BeginChild(XorStr("third child"), ImVec2(300, 250), true);
			{
				ImGui::Text("Anti-Aim");
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Anti-Aim Enabled"), &g_Options.Ragebot.EnabledAntiAim);
				ImGui::Checkbox(XorStr("Enable"), &g_Options.Ragebot.PreAAs);
				ImGui::PushItemWidth(150);
				ImGui::Combo(XorStr("Pitch"), &g_Options.Ragebot.Pitch, antiaimpitch, ARRAYSIZE(antiaimpitch));
				ImGui::SliderFloat(XorStr("Pitch Adder "), &g_Options.Ragebot.PitchAdder, -180, 180, "%1.f");
				ImGui::Combo(XorStr("Yaw"), &g_Options.Ragebot.YawTrue, antiaimyawtrue, ARRAYSIZE(antiaimyawtrue));
				ImGui::SliderFloat(XorStr("Real Adder "), &g_Options.Ragebot.YawTrueAdder, -180, 180, "%1.f");
				ImGui::Combo(XorStr("Fake-Yaw"), &g_Options.Ragebot.YawFake, antiaimyawfake, ARRAYSIZE(antiaimyawfake));
				ImGui::SliderFloat(XorStr("Fake Adder "), &g_Options.Ragebot.YawFakeAdder, -180, 180, "%1.f");

			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("7 child"), ImVec2(300, 250), true);
			{
				ImGui::Text("Builder Anti-Aim");
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Enable   "), &g_Options.Ragebot.BuilderAAs);
				ImGui::PushItemWidth(150);
				ImGui::SliderFloat(XorStr("Pitch Angle "), &g_Options.Ragebot.BuilderPitch, -89, 89, "%1.f");
				ImGui::SliderFloat(XorStr("Real Angle "), &g_Options.Ragebot.BuilderReal, -180, 180, "%1.f");
				ImGui::SliderFloat(XorStr("Fake Angle "), &g_Options.Ragebot.BuilderFake, -180, 180, "%1.f");

				ImGui::Checkbox(XorStr("Enable Real Jitter"), &g_Options.Ragebot.Jitter);
				ImGui::SliderFloat(XorStr("Jitter Range "), &g_Options.Ragebot.JitterRange, -90, 90, "%1.f");
				ImGui::Checkbox(XorStr("Enable Fake Jitter"), &g_Options.Ragebot.FJitter);
				ImGui::SliderFloat(XorStr("FJitter Range "), &g_Options.Ragebot.FJitterRange, -90, 90, "%1.f");
			}ImGui::EndChild();
			ImGui::BeginChild(XorStr("8 child"), ImVec2(300, 170), true);
			{
				ImGui::Text("Options Anti-Aim");
				ImGui::Separator();
				ImGui::Checkbox(XorStr("LBY Breaker"), &g_Options.Ragebot.LBYBreaker);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("At Players"), &g_Options.Ragebot.AtTarget);
				ImGui::Checkbox(XorStr("Edge"), &g_Options.Ragebot.Edge);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("LBY Update"), &g_Options.Ragebot.NextLBYUpdate);
				ImGui::Checkbox(XorStr("Moving LBY Update"), &g_Options.Ragebot.NextMovingLBYUpdate);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("AntiAim knife"), &g_Options.Ragebot.KnifeAA);
			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("VI child"), ImVec2(300, 170), true);
			{
				ImGui::Text(XorStr("Fake Lag Anti-Aim"));
				ImGui::Separator();
				ImGui::PushItemWidth(184);
				ImGui::Checkbox(XorStr("FakeLag Anti-Aim"), &g_Options.Ragebot.FakeLagAA);
				ImGui::Combo(XorStr("Fake Lag Type"), &g_Options.Ragebot.FakeLagType, Fakelagtypelist, ARRAYSIZE(Fakelagtypelist));
				ImGui::SliderInt(XorStr("Choke Factor"), &g_Options.Ragebot.FakeLagAmtAA, 0, 18);
			}ImGui::EndChild();
			break;
		}
			break;
		}
	
}
void Legitbot()
{
	ImGui::Checkbox(XorStr("Active"), &g_Options.Legitbot.MainSwitch);
	ImGui::Separator();

	if (ImGui::Button(XorStr("Aimbot"), ImVec2(200, 0)))
	{
		g_Options.Menu.iLegitSubTab = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button(XorStr("Options"), ImVec2(200, 0)))
	{
		g_Options.Menu.iLegitSubTab = 1;
	}

	ImGui::Separator();
	switch (g_Options.Menu.iLegitSubTab)
	{
	case 0:
	{
		ImGui::BeginChild("first child", ImVec2(260, 255), true);
		{
			ImGui::Text(XorStr("Pistols"));
			ImGui::PushItemWidth(184);
			ImGui::Hotkey(XorStr("Key##1"), &g_Options.Legitbot.PistolKey);
			ImGui::SliderFloat(XorStr("Smooth##1"), &g_Options.Legitbot.Pistol_Smooth, 1.00f, 300.00f, "%.2f");
			ImGui::SliderFloat(XorStr("FOV##1"), &g_Options.Legitbot.Pistolfov, 1.00f, 30.00f, "%.2f");
			ImGui::SliderFloat(XorStr("RCS##1"), &g_Options.Legitbot.Pistol_rcs, 1.00f, 100.00f, "%.2f");
			ImGui::Combo(XorStr("##rcs"), &g_Options.Legitbot.Pistol_RcsType, RCSType, ARRAYSIZE(RCSType));
			ImGui::Combo(XorStr("##hbp"), &g_Options.Legitbot.hitboxpistol, HitBoxesToScan, ARRAYSIZE(HitBoxesToScan));
			ImGui::Combo(XorStr("Lag##lag1"), &g_Options.Legitbot.Pistol_fakelag_mode, "Normal\0\rWhen Shooting\0\0");
			ImGui::SliderFloat(XorStr("##Fake Lage"), &g_Options.Legitbot.Pistol_fakelag_value, 0.f, 20.f, "Fake Lag Amount: %0.f");
			ImGui::Checkbox(XorStr("AutoLock"), &g_Options.Legitbot.Pistol_AutoLock);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Silent Aim"), &g_Options.Legitbot.Pistol_Silent);
			ImGui::Checkbox(XorStr("Fake Lage"), &g_Options.Legitbot.Pistol_fakelag);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Draw Fov"), &g_Options.Legitbot.Pistol_drawfov);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("2 child", ImVec2(260, 255), true);
		{
			ImGui::Text(XorStr("Heavy"));
			ImGui::PushItemWidth(184);
			ImGui::Hotkey(XorStr("Key##0"), &g_Options.Legitbot.heavyKey);
			ImGui::SliderFloat(XorStr("Smooth##0"), &g_Options.Legitbot.heavy_Smooth, 1.00f, 300.00f, "%.2f");
			ImGui::SliderFloat(XorStr("FOV##0"), &g_Options.Legitbot.heavyfov, 1.00f, 30.00f, "%.2f");
			ImGui::SliderFloat(XorStr("RCS##0"), &g_Options.Legitbot.heavy_rcs, 1.00f, 100.00f, "%.2f");
			ImGui::Combo(XorStr("##rcs"), &g_Options.Legitbot.heavy_RcsType, RCSType, ARRAYSIZE(RCSType));
			ImGui::Combo(XorStr("##hbh"), &g_Options.Legitbot.hitboxheavy, HitBoxesToScan, ARRAYSIZE(HitBoxesToScan));
			ImGui::Combo(XorStr("Lag##lag2"), &g_Options.Legitbot.heavy_fakelag_mode, "Normal\0\rWhen Shooting\0\0");
			ImGui::SliderFloat(XorStr("##Fake Lage"), &g_Options.Legitbot.heavy_fakelag_value, 0.f, 20.f, "Fake Lag Amount: %0.f");
			ImGui::Checkbox(XorStr("AutoLock"), &g_Options.Legitbot.heavy_AutoLock);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Silent Aim"), &g_Options.Legitbot.heavy_Silent);
			ImGui::Checkbox(XorStr("Fake Lage"), &g_Options.Legitbot.heavy_fakelag);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Draw Fov"), &g_Options.Legitbot.heavy_drawfov);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("3 child", ImVec2(260, 255), true);
		{
			ImGui::Text(XorStr("Smg"));
			ImGui::PushItemWidth(184);
			ImGui::Hotkey(XorStr("Key##0"), &g_Options.Legitbot.smgKey);
			ImGui::SliderFloat(XorStr("Smooth##0"), &g_Options.Legitbot.smg_Smooth, 1.00f, 300.00f, "%.2f");
			ImGui::SliderFloat(XorStr("FOV##0"), &g_Options.Legitbot.smgfov, 1.00f, 30.00f, "%.2f");
			ImGui::SliderFloat(XorStr("RCS##0"), &g_Options.Legitbot.smg_rcs, 1.00f, 100.00f, "%.2f");
			ImGui::Combo(XorStr("##rcs"), &g_Options.Legitbot.smg_RcsType, RCSType, ARRAYSIZE(RCSType));
			ImGui::Combo(XorStr("##hbs"), &g_Options.Legitbot.hitboxsmg, HitBoxesToScan, ARRAYSIZE(HitBoxesToScan));
			ImGui::Combo(XorStr("Lag##lag3"), &g_Options.Legitbot.smg_fakelag_mode, "Normal\0\rWhen Shooting\0\0");
			ImGui::SliderFloat(XorStr("##Fake Lage"), &g_Options.Legitbot.smg_fakelag_value, 0.f, 20.f, "Fake Lag Amount: %0.f");
			ImGui::Checkbox(XorStr("AutoLock"), &g_Options.Legitbot.smg_AutoLock);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Silent Aim"), &g_Options.Legitbot.smg_Silent);
			ImGui::Checkbox(XorStr("Fake Lage"), &g_Options.Legitbot.smg_fakelag);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Draw Fov"), &g_Options.Legitbot.smg_drawfov);

		}ImGui::EndChild();

		ImGui::BeginChild("4 child", ImVec2(260, 255), true);
		{
			ImGui::Text(XorStr("Rifles"));
			ImGui::PushItemWidth(184);
			ImGui::Hotkey(XorStr("Key##0"), &g_Options.Legitbot.MainKey);
			ImGui::SliderFloat(XorStr("Smooth##0"), &g_Options.Legitbot.Main_Smooth, 1.00f, 300.00f, "%.2f");
			ImGui::SliderFloat(XorStr("FOV##0"), &g_Options.Legitbot.Mainfov, 1.00f, 30.00f, "%.2f");
			ImGui::SliderFloat(XorStr("RCS##0"), &g_Options.Legitbot.main_rcs, 1.00f, 100.00f, "%.2f");
			ImGui::Combo(XorStr("##rcs"), &g_Options.Legitbot.main_RcsType, RCSType, ARRAYSIZE(RCSType));
			ImGui::Combo(XorStr("##hbr"), &g_Options.Legitbot.hitboxrifle, HitBoxesToScan, ARRAYSIZE(HitBoxesToScan));
			ImGui::Combo(XorStr("Lag##lag4"), &g_Options.Legitbot.main_fakelag_mode, "Normal\0\rWhen Shooting\0\0");
			ImGui::SliderFloat(XorStr("##Fake Lage"), &g_Options.Legitbot.main_fakelag_value, 0.f, 20.f, "Fake Lag Amount: %0.f");
			ImGui::Checkbox(XorStr("AutoLock"), &g_Options.Legitbot.main_AutoLock);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Silent Aim"), &g_Options.Legitbot.main_Silent);
			ImGui::Checkbox(XorStr("Fake Lage"), &g_Options.Legitbot.main_fakelag);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Draw Fov"), &g_Options.Legitbot.main_drawfov);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("5 child", ImVec2(260, 255), true);
		{
			ImGui::Text(XorStr("Snipers"));
			ImGui::PushItemWidth(184);
			ImGui::Hotkey(XorStr("Key##2"), &g_Options.Legitbot.SniperKey);
			ImGui::SliderFloat(XorStr("Smooth##2"), &g_Options.Legitbot.Sniper_Smooth, 1.00f, 300.00f, "%.2f");
			ImGui::SliderFloat(XorStr("FOV##2"), &g_Options.Legitbot.Sniperfov, 1.00f, 30.00f, "%.2f");
			ImGui::SliderFloat(XorStr("RCS##2"), &g_Options.Legitbot.sniper_rcs, 1.00f, 100.00f, "%.2f");
			ImGui::Combo(XorStr("##rcs"), &g_Options.Legitbot.sniper_RcsType, RCSType, ARRAYSIZE(RCSType));
			ImGui::Combo(XorStr("##hbs"), &g_Options.Legitbot.hitboxsniper, HitBoxesToScan, ARRAYSIZE(HitBoxesToScan));
			ImGui::Combo(XorStr("Lage##lag5"), &g_Options.Legitbot.Sniper_fakelag_mode, "Normal\0\rWhen Shooting\0\0");
			ImGui::SliderFloat(XorStr("##Fake Lage"), &g_Options.Legitbot.Sniper_fakelag_value, 0.f, 20.f, "Fake Lag Amount: %0.f");
			ImGui::Checkbox(XorStr("AutoLock"), &g_Options.Legitbot.sniper_AutoLock);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Silent Aim"), &g_Options.Legitbot.sniper_Silent);
			ImGui::Checkbox(XorStr("Fake Lage"), &g_Options.Legitbot.Sniper_fakelag);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Draw Fov"), &g_Options.Legitbot.Sniper_drawfov);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("6 child", ImVec2(260, 255), true);
		{
			ImGui::Text(XorStr("R8/Deag"));
			ImGui::PushItemWidth(184);
			ImGui::Hotkey(XorStr("Key##2"), &g_Options.Legitbot.RevolverKey);
			ImGui::SliderFloat(XorStr("Smooth##2"), &g_Options.Legitbot.Revolver_Smooth, 1.00f, 300.00f, "%.2f");
			ImGui::SliderFloat(XorStr("FOV##2"), &g_Options.Legitbot.Revolverfov, 1.00f, 30.00f, "%.2f");
			ImGui::SliderFloat(XorStr("RCS##2"), &g_Options.Legitbot.Revolver_rcs, 1.00f, 100.00f, "%.2f");
			ImGui::Combo(XorStr("##rcs"), &g_Options.Legitbot.Revolver_RcsType, RCSType, ARRAYSIZE(RCSType));
			ImGui::Combo(XorStr("##hbr"), &g_Options.Legitbot.hitboxRevolver, HitBoxesToScan, ARRAYSIZE(HitBoxesToScan));
			ImGui::Combo(XorStr("Lage##lag6"), &g_Options.Legitbot.Revolver_fakelag_mode, "Normal\0\rWhen Shooting\0\0");
			ImGui::SliderFloat(XorStr("##Fake Lage"), &g_Options.Legitbot.Revolver_fakelag_value, 0.f, 20.f, "Fake Lag Amount: %0.f");
			ImGui::Checkbox(XorStr("AutoLock"), &g_Options.Legitbot.Revolver_AutoLock);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Silent Aim"), &g_Options.Legitbot.Revolver_Silent);
			ImGui::Checkbox(XorStr("Fake Lage"), &g_Options.Legitbot.Revolver_fakelag);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Draw Fov"), &g_Options.Legitbot.Revolver_drawfov);

		}ImGui::EndChild();
		break;
	}
	case 1:
	{
		ImGui::BeginChild("1 child", ImVec2(240, 255), true);
		{
			ImGui::Checkbox(XorStr("Backtrack"), &g_Options.Legitbot.backtrack);
			ImGui::Checkbox(XorStr("A Legit AA"), &g_Options.Legitbot.Aimbot.ALegitAA);
			ImGui::Checkbox(XorStr("Legit AA"), &g_Options.Legitbot.Aimbot.EnableLegitAA);
			ImGui::Checkbox(XorStr("Aim AWP"), &g_Options.Legitbot.Aimbot.AimSniper);
			ImGui::Checkbox(XorStr("Fast Zoom"), &g_Options.Legitbot.Aimbot.fastzoom);
			ImGui::Checkbox(XorStr("Fast Switch"), &g_Options.Legitbot.Aimbot.fastzoomswitch);
			ImGui::Checkbox(XorStr("AutoPistol"), &g_Options.Legitbot.Aimbot.AutoPistol);
			ImGui::Checkbox(XorStr("Smoke"), &g_Options.Legitbot.checksmoke);
		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("2 child", ImVec2(300, 255), true);
		{
			ImGui::PushItemWidth(180);
			ImGui::SliderFloat(XorStr("Backtrack Size"), &g_Options.Legitbot.backtrack_size, 1.00f, 10.00f, "%.2f");
			ImGui::SliderFloat(XorStr("Backtrack Ticks"), &g_Options.Legitbot.backtrackTicks, 0, 12, "%1.f");
			ImGui::SliderFloat(XorStr("AA Angle"), &g_Options.Legitbot.Aimbot.AAAngle, -180, 180, "%1.f");

		}ImGui::EndChild();
		break;
	}
		break;
	}
}

void Trigeerbot()
{
	ImGui::Checkbox(XorStr("Active"), &g_Options.Legitbot.TriggerbotSwitch);
	ImGui::Separator();
	ImGui::BeginChild("1 child", ImVec2(240, 255), true);
	{
		ImGui::Text(XorStr("Pistols"));
		ImGui::PushItemWidth(184);
		ImGui::Checkbox(XorStr("Triggerbot"), &g_Options.Legitbot.Pistol_Triggerbot);
		ImGui::Checkbox(XorStr("Auto Fire"), &g_Options.Legitbot.Pistol_Triggerbot_AutoFire);
		ImGui::Hotkey(XorStr("Key##3"), &g_Options.Legitbot.Pistol_TriggerbotKey);
		ImGui::SliderFloat(XorStr("Hitchance"), &g_Options.Legitbot.Pistol_Triggerbot_hitchance, 1.00f, 100.00f, "%.2f");

		ImGui::Text(XorStr("Filter"));

		ImGui::BeginChild(XorStr("Filter Pistols Triggerbot"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 21 * 5));
		{
			ImGui::Selectable(XorStr(" Head"), &g_Options.Legitbot.Pistol_TriggerbotFilter.Head);
			ImGui::Selectable(XorStr(" Arms"), &g_Options.Legitbot.Pistol_TriggerbotFilter.Arms);
			ImGui::Selectable(XorStr(" Chest"), &g_Options.Legitbot.Pistol_TriggerbotFilter.Chest);
			ImGui::Selectable(XorStr(" Stomach"), &g_Options.Legitbot.Pistol_TriggerbotFilter.Stomach);
			ImGui::Selectable(XorStr(" Legs"), &g_Options.Legitbot.Pistol_TriggerbotFilter.Legs);
		}ImGui::EndChild();
	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("2 child", ImVec2(240, 255), true);
	{
		ImGui::Text(XorStr("Heavy"));
		ImGui::PushItemWidth(184);
		ImGui::Checkbox(XorStr("Triggerbot"), &g_Options.Legitbot.heavy_Triggerbot);
		ImGui::Checkbox(XorStr("Auto Fire"), &g_Options.Legitbot.heavy_Triggerbot_AutoFire);
		ImGui::Hotkey(XorStr("Key##3"), &g_Options.Legitbot.heavy_TriggerbotKey);
		ImGui::SliderFloat(XorStr("Hitchance"), &g_Options.Legitbot.heavy_Triggerbot_hitchance, 1.00f, 100.00f, "%.2f");

		ImGui::Text(XorStr("Filter"));

		ImGui::BeginChild(XorStr("Filter Heavy Triggerbot"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 21 * 5));
		{
			ImGui::Selectable(XorStr(" Head"), &g_Options.Legitbot.heavy_TriggerbotFilter.Head);
			ImGui::Selectable(XorStr(" Arms"), &g_Options.Legitbot.heavy_TriggerbotFilter.Arms);
			ImGui::Selectable(XorStr(" Chest"), &g_Options.Legitbot.heavy_TriggerbotFilter.Chest);
			ImGui::Selectable(XorStr(" Stomach"), &g_Options.Legitbot.heavy_TriggerbotFilter.Stomach);
			ImGui::Selectable(XorStr(" Legs"), &g_Options.Legitbot.heavy_TriggerbotFilter.Legs);
		}ImGui::EndChild();

	}ImGui::EndChild();
	ImGui::SameLine();

	ImGui::BeginChild("3 child", ImVec2(240, 255), true);
	{
		ImGui::Text(XorStr("Smg"));
		ImGui::PushItemWidth(184);
		ImGui::Checkbox(XorStr("Triggerbot"), &g_Options.Legitbot.smg_Triggerbot);
		ImGui::Checkbox(XorStr("Auto Fire"), &g_Options.Legitbot.smg_Triggerbot_AutoFire);
		ImGui::Hotkey(XorStr("Key##3"), &g_Options.Legitbot.smg_TriggerbotKey);
		ImGui::SliderFloat(XorStr("Hitchance"), &g_Options.Legitbot.smg_Triggerbot_hitchance, 1.00f, 100.00f, "%.2f");

		ImGui::Text(XorStr("Filter"));

		ImGui::BeginChild(XorStr("Filter Smg Triggerbot"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 21 * 5));
		{
			ImGui::Selectable(XorStr(" Head"), &g_Options.Legitbot.smg_TriggerbotFilter.Head);
			ImGui::Selectable(XorStr(" Arms"), &g_Options.Legitbot.smg_TriggerbotFilter.Arms);
			ImGui::Selectable(XorStr(" Chest"), &g_Options.Legitbot.smg_TriggerbotFilter.Chest);
			ImGui::Selectable(XorStr(" Stomach"), &g_Options.Legitbot.smg_TriggerbotFilter.Stomach);
			ImGui::Selectable(XorStr(" Legs"), &g_Options.Legitbot.smg_TriggerbotFilter.Legs);
		}ImGui::EndChild();
	}ImGui::EndChild();
	ImGui::BeginChild("4 child", ImVec2(240, 255), true);
	{
		ImGui::Text(XorStr("Rifles"));
		ImGui::PushItemWidth(184);
		ImGui::Checkbox(XorStr("Triggerbot"), &g_Options.Legitbot.main_Triggerbot);
		ImGui::Checkbox(XorStr("Auto Fire"), &g_Options.Legitbot.main_Triggerbot_AutoFire);
		ImGui::Hotkey(XorStr("Key##3"), &g_Options.Legitbot.main_TriggerbotKey);
		ImGui::SliderFloat(XorStr("Hitchance"), &g_Options.Legitbot.main_Triggerbot_hitchance, 1.00f, 100.00f, "%.2f");

		ImGui::Text(XorStr("Filter"));

		ImGui::BeginChild(XorStr("Filter Rifles Triggerbot"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 21 * 5));
		{
			ImGui::Selectable(XorStr(" Head"), &g_Options.Legitbot.main_TriggerbotFilter.Head);
			ImGui::Selectable(XorStr(" Arms"), &g_Options.Legitbot.main_TriggerbotFilter.Arms);
			ImGui::Selectable(XorStr(" Chest"), &g_Options.Legitbot.main_TriggerbotFilter.Chest);
			ImGui::Selectable(XorStr(" Stomach"), &g_Options.Legitbot.main_TriggerbotFilter.Stomach);
			ImGui::Selectable(XorStr(" Legs"), &g_Options.Legitbot.main_TriggerbotFilter.Legs);
		}ImGui::EndChild();

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("5 child", ImVec2(240, 255), true);
	{
		ImGui::Text(XorStr("Snipers"));
		ImGui::PushItemWidth(184);
		ImGui::Checkbox(XorStr("Triggerbot"), &g_Options.Legitbot.sniper_Triggerbot);
		ImGui::Checkbox(XorStr("Auto Fire"), &g_Options.Legitbot.sniper_Triggerbot_AutoFire);
		ImGui::Hotkey(XorStr("Key##3"), &g_Options.Legitbot.sniper_TriggerbotKey);
		ImGui::SliderFloat(XorStr("Hitchance"), &g_Options.Legitbot.sniper_Triggerbot_hitchance, 1.00f, 100.00f, "%.2f");

		ImGui::Text(XorStr("Filter"));

		ImGui::BeginChild(XorStr("Filter Snipers Triggerbot"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 21 * 5));
		{
			ImGui::Selectable(XorStr(" Head"), &g_Options.Legitbot.sniper_TriggerbotFilter.Head);
			ImGui::Selectable(XorStr(" Arms"), &g_Options.Legitbot.sniper_TriggerbotFilter.Arms);
			ImGui::Selectable(XorStr(" Chest"), &g_Options.Legitbot.sniper_TriggerbotFilter.Chest);
			ImGui::Selectable(XorStr(" Stomach"), &g_Options.Legitbot.sniper_TriggerbotFilter.Stomach);
			ImGui::Selectable(XorStr(" Legs"), &g_Options.Legitbot.sniper_TriggerbotFilter.Legs);
		}ImGui::EndChild();

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("6 child", ImVec2(240, 255), true);
	{
		ImGui::Text(XorStr("R8/Deag")); ImGui::NextColumn();
		ImGui::PushItemWidth(184);
		ImGui::Checkbox(XorStr("Triggerbot"), &g_Options.Legitbot.Revolver_Triggerbot);
		ImGui::Checkbox(XorStr("Auto Fire"), &g_Options.Legitbot.Revolver_Triggerbot_AutoFire);
		ImGui::Hotkey(XorStr("Key##3"), &g_Options.Legitbot.Revolver_TriggerbotKey);
		ImGui::SliderFloat(XorStr("Hitchance"), &g_Options.Legitbot.Revolver_Triggerbot_hitchance, 1.00f, 100.00f, "%.2f");

		ImGui::Text(XorStr("Filter"));

		ImGui::BeginChild(XorStr("Filter R8/Deag Triggerbot"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 21 * 5));
		{
			ImGui::Selectable(XorStr(" Head"), &g_Options.Legitbot.Revolver_TriggerbotFilter.Head);
			ImGui::Selectable(XorStr(" Arms"), &g_Options.Legitbot.Revolver_TriggerbotFilter.Arms);
			ImGui::Selectable(XorStr(" Chest"), &g_Options.Legitbot.Revolver_TriggerbotFilter.Chest);
			ImGui::Selectable(XorStr(" Stomach"), &g_Options.Legitbot.Revolver_TriggerbotFilter.Stomach);
			ImGui::Selectable(XorStr(" Legs"), &g_Options.Legitbot.Revolver_TriggerbotFilter.Legs);
		}ImGui::EndChild();
	}ImGui::EndChild();
}
void Visuals()
{
	ImGui::Checkbox(XorStr("Active"), &g_Options.Visuals.Enabled);
	ImGui::Separator();

	if (ImGui::Button(XorStr("ESP"), ImVec2(200, 0)))
	{
		g_Options.Menu.iVisualsSubTab = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button(XorStr("Chams"), ImVec2(200, 0)))
	{
		g_Options.Menu.iVisualsSubTab = 1;
	}
	ImGui::SameLine();
	if (ImGui::Button(XorStr("Glow"), ImVec2(200, 0)))
	{
		g_Options.Menu.iVisualsSubTab = 2;
	}
	ImGui::Separator();
	switch (g_Options.Menu.iVisualsSubTab)
	{
	case 0:
	{
		ImGui::BeginChild(XorStr("first child"), ImVec2(270, 250), true);
		{
			ImGui::Text(XorStr("ESP"));
			ImGui::Separator();

			ImGui::Checkbox(XorStr("Box"), &g_Options.Visuals.Box);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Name"), &g_Options.Visuals.Name);
			ImGui::Checkbox(XorStr("HP"), &g_Options.Visuals.health);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Aimlines"), &g_Options.Visuals.AimLine);
			ImGui::Checkbox(XorStr("Skeleton"), &g_Options.Visuals.skeletonenbl);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Bullet Trace"), &g_Options.Visuals.barrel);
			ImGui::Checkbox(XorStr("Screen Indicator"), &g_Options.Visuals.OffscreenIndicator);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Fill-Box"), &g_Options.Visuals.fill);
			ImGui::Checkbox(XorStr("Armor"), &g_Options.Visuals.armor);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Grenades Box"), &g_Options.Visuals.GrenadeBox);
			ImGui::Checkbox(XorStr("Hostage"), &g_Options.Visuals.Hostage);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Chicken"), &g_Options.Visuals.Chicken);
			ImGui::Checkbox(XorStr("Dropped Guns"), &g_Options.Visuals.DroppedGuns);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Bomb ESP"), &g_Options.Visuals.C4World);
			ImGui::Checkbox(XorStr("DLight"), &g_Options.Visuals.DLight);

		//	ImGui::Checkbox(XorStr("Lag Hitboxes"), &g_Options.Visuals.visuals_lagcomp);


		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("second child"), ImVec2(270, 250), true);
		{
			ImGui::Text(XorStr("ESP Options"));
			ImGui::Separator();
			ImGui::Combo(XorStr("Box"), &g_Options.Visuals.BoxType, "Full\0\rCorners\0\r3D\0\0", -1);
			ImGui::Combo(XorStr("Health"), &g_Options.Visuals.healthtype, "OFF\0\rNormal\0\rEdgy\0\0", -1);
			ImGui::Combo(XorStr("Armor"), &g_Options.Visuals.Armor2, armor, ARRAYSIZE(armor));
			ImGui::Combo(XorStr("Weapon"), &g_Options.Visuals.Weapon, WeaponType, ARRAYSIZE(WeaponType));
			ImGui::Combo(XorStr("skelopts"), &g_Options.Visuals.skeletonopts, skeletonopts, ARRAYSIZE(skeletonopts));
			ImGui::Combo(XorStr("Grenades"), &g_Options.Visuals.Grenades, grenades, ARRAYSIZE(grenades));
			ImGui::Combo(XorStr("Dropped"), &g_Options.Visuals.DroppedGunsType, droppedWeapons, ARRAYSIZE(droppedWeapons));
			ImGui::SliderFloat(XorStr("Alpha fill Box"), &g_Options.Visuals.esp_fill_amount, 0.f, 255.f);
			ImGui::SliderInt(XorStr("Screen Indicator"), &g_Options.Visuals.OffscreenIndicatorSize, 0, 25, "Size: %.2f");
			ImGui::SliderInt(XorStr("barrelL "), &g_Options.Visuals.barrelL, 0, 1500, "%.0f");

			
		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("3 child"), ImVec2(270, 250), true);
		{
			ImGui::Text(XorStr("ESP Filter"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("Team ESP"), &g_Options.Visuals.TeamESP);
			ImGui::Combo("Visible", &g_Options.Visuals.esp_Visible, "Enemy\0\rTeam\0\rAll\0\rOnly Visible\0\0", -1);

		}ImGui::EndChild();
		ImGui::BeginChild(XorStr("4 child"), ImVec2(270, 250), true);
		{
			ImGui::Text(XorStr("ESP Player Info"));
			ImGui::Separator();

			ImGui::Checkbox(XorStr("Defuser"), &g_Options.Visuals.IsHasDefuser);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Scoped"), &g_Options.Visuals.Scoped);
			ImGui::Checkbox(XorStr("Bomb-Carrier"), &g_Options.Visuals.BombCarrier);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Flashed"), &g_Options.Visuals.Flashed);
			ImGui::Checkbox(XorStr("Distance"), &g_Options.Visuals.Distance);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Money"), &g_Options.Visuals.Money);
			ImGui::Checkbox(XorStr("Reloading"), &g_Options.Visuals.IsReloading);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Callout"), &g_Options.Visuals.Callout);
		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("5 child"), ImVec2(270, 250), true);
		{
			ImGui::Text(XorStr("Other ESP"));
			ImGui::Separator();

			ImGui::Checkbox(XorStr("Grenade Prediction"), &g_Options.Visuals.GrenadePrediction);
			ImGui::Checkbox(XorStr("LBY Indicator"), &g_Options.Visuals.LBYIndicator);
			ImGui::Checkbox(XorStr("Angle Lines"), &g_Options.Visuals.angleLines);
			ImGui::Checkbox(XorStr("Bullet Impacts"), &g_Options.Visuals.bulletshow);
			ImGui::SliderFloat(XorStr("Duration"), &g_Options.Visuals.flTracersDuration, 0.f, 10.f, "%.2f");
			ImGui::SliderFloat(XorStr("Width"), &g_Options.Visuals.flTracersWidth, 0.f, 10.f, "%.2f");
			ImGui::Checkbox(XorStr("Backtrack Hitbox"), &g_Options.Visuals.Hitbox);
			ImGui::SliderFloat(XorStr("##duration"), &g_Options.Visuals.HitboxDuration, 0.f, 10.f, "Duration: %.2f");


		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("6 child"), ImVec2(270, 250), true);
		{
			ImGui::Text(XorStr("Crosshair Options"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("Auto Wall Crosshair"), &g_Options.Visuals.DrawAwall);
			ImGui::Checkbox(XorStr("Auto-Wall Crosshair"), &g_Options.Visuals.AWallCrosshair);
			ImGui::Checkbox(XorStr("Sniper Crosshair"), &g_Options.Visuals.SniperCrosshair);
			ImGui::Combo(XorStr(""), &g_Options.Visuals.SniperCrosshairType, snipercrosshair, ARRAYSIZE(snipercrosshair));
			ImGui::Checkbox(XorStr("Crosshair Enabled"), &g_Options.Visuals.CrosshairOn);
			ImGui::Checkbox(XorStr("Show Spread"), &g_Options.Visuals.CrosshairSpread);
			//ImGui::Checkbox(XorStr("Show FOV"), &g_Options.Visuals.CrosshairFOV);
			ImGui::Checkbox(XorStr("Show Recoil"), &g_Options.Visuals.CrosshairType);
			ImGui::Checkbox(XorStr("Rainbow Color"), &g_Options.Visuals.CrosshairRainbow);
			ImGui::Text(XorStr("Crosshair Style"));
			ImGui::SameLine();
			ImGui::Combo(XorStr("##CRSHRStyle"), &g_Options.Visuals.CrosshairStyle, XorStr("Plus\0\rCircle\0\rBox\0\0"), -1);
		}ImGui::EndChild();
		break;
	}
	case 1:
	{
		ImGui::BeginChild(XorStr("2 child"), ImVec2(268, 350), true);
		{
			ImGui::Text(XorStr("Chams"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("Cham Players"), &g_Options.Visuals.Chams);
			ImGui::Checkbox(XorStr("Dropped Chams"), &g_Options.Visuals.WeaponsDroppedChams);
			ImGui::Checkbox(XorStr("Weapons Chams"), &g_Options.Misc.WeaponsHandMat);
			ImGui::Checkbox(XorStr("Hand Chams"), &g_Options.Misc.HandMat);
			ImGui::Checkbox(XorStr("Deads Players"), &g_Options.Visuals.corpses);
			ImGui::Checkbox(XorStr("Chams Heath"), &g_Options.Visuals.chamsHp);
			ImGui::Checkbox("Fake Angle Chams", &g_Options.Visuals.esp_player_chams_fakeangle);


		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("3 child"), ImVec2(268, 350), true);
		{
			ImGui::Text(XorStr("Chams Options"));
			ImGui::Separator();
			ImGui::Combo(XorStr("Players"), &g_Options.Visuals.Playermatierial, "Normal\0\rFlat\0\rWireframe\0\rGlass\0\rGold\0\rPlatinum\0\rPlastic\0\rCrystal\0\rglass2\0\rVelvet\0\rDark Chrome\0\rDetailed Gold\0\rglow\0\0", -1);
			ImGui::Combo(XorStr("Dropped"), &g_Options.Visuals.Weaponsdroppedmatierial, "Normal\0\rFlat\0\rWireframe\0\rGlass\0\rGold\0\rPlatinum\0\rPlastic\0\rCrystal\0\rglass2\0\rVelvet\0\rDark Chrome\0\rDetailed Gold\0\rglow\0\0", -1);
			ImGui::Combo(XorStr("Hand "), &g_Options.Misc.handmat, "Normal\0\rFlat\0\rWireframe\0\rGlass\0\rGold\0\rPlatinum\0\rPlastic\0\rCrystal\0\rglass2\0\rVelvet\0\rDark Chrome\0\rDetailed Gold\0\rglow\0\0", -1);
			ImGui::Combo(XorStr("Weapon "), &g_Options.Misc.weaponhandmat, "Normal\0\rFlat\0\rWireframe\0\rGlass\0\rGold\0\rPlatinum\0\rPlastic\0\rCrystal\0\rglass2\0\rVelvet\0\rDark Chrome\0\rDetailed Gold\0\rglow\0\0", -1);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("4 child"), ImVec2(268, 350), true);
		{
			ImGui::Text(XorStr("Filter"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("Team Chams"), &g_Options.Visuals.Teamchams);
			ImGui::Checkbox(XorStr("XQZ"), &g_Options.Visuals.XQZ);

		}ImGui::EndChild();
		ImGui::BeginChild(XorStr("5 child"), ImVec2(820, 150), true);
		{
			ImGui::Text(XorStr("Alpha"));
			ImGui::Separator();
			ImGui::PushItemWidth(600);
			ImGui::SliderInt(XorStr("Chams Player Alpha"), &g_Options.Color.champlayeralpha, 0, 100);
			ImGui::SliderInt(XorStr("Chams Hands Alpha"), &g_Options.Color.chamhandalpha, 0, 100);
			ImGui::SliderInt(XorStr("Chams Weapon Alpha"), &g_Options.Color.weaponhandalpha, 0, 100);
			ImGui::SliderInt(XorStr("Chams Weapons Dropped Alpha"), &g_Options.Color.weaponsworldalph, 0, 100);
		}ImGui::EndChild();
		break;
	}
	case 2:
	{
		ImGui::BeginChild(XorStr("1 child"), ImVec2(406, 350), true);
		{
			ImGui::Text(XorStr("Glow Options"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("Players Glow"), &g_Options.Visuals.glowplayer);
			ImGui::Checkbox(XorStr("Weapon Glow"), &g_Options.Visuals.weaponglow);
			ImGui::Checkbox(XorStr("Bomb glow"), &g_Options.Visuals.bombglow);
		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("third child"), ImVec2(406, 350), true);
		{
			ImGui::Text(XorStr("Filter"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("Team Glow"), &g_Options.Visuals.teamglow);
		}ImGui::EndChild();
		ImGui::BeginChild(XorStr("4 child"), ImVec2(820, 150), true);
		{
			ImGui::Text(XorStr("Alpha"));
			ImGui::Separator();
			ImGui::PushItemWidth(600);
			ImGui::SliderInt(XorStr("Glow Player Alpha"), &g_Options.Color.playerglowalpha, 0, 255);
			ImGui::SliderInt(XorStr("Glow Weapon Alpha"), &g_Options.Color.weaponglowalpha, 0, 255);
			ImGui::SliderInt(XorStr("Glow Bomb Alpha"), &g_Options.Color.bombglowalpha, 0, 255);
		}ImGui::EndChild();
		break;
	}

	}
}
void Misc()
{
	if (ImGui::Button(XorStr("Spammer"), ImVec2(200, 0)))
	{
		g_Options.Menu.iMiscSubTab = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button(XorStr("Misc Options"), ImVec2(200, 0)))
	{
		g_Options.Menu.iMiscSubTab = 1;
	}
	ImGui::SameLine();
	if (ImGui::Button(XorStr("Other Misc"), ImVec2(200, 0)))
	{
		g_Options.Menu.iMiscSubTab = 2;
	}
	ImGui::Separator();
	switch (g_Options.Menu.iMiscSubTab)
	{
	case 0:
	{
		ImGui::BeginChild(XorStr("first child"), ImVec2(330, 220), true);
		{
			ImGui::Text(XorStr("Spamer"));
			ImGui::Separator();

			ImGui::Checkbox(XorStr("Name Stealer"), &g_Options.Misc.silentstealer);

			if (g_Options.Misc.silentstealer)
			{
				change_name("\nญญญ");
			}

			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Hide Name"), &g_Options.Misc.hidename);
			ImGui::Checkbox(XorStr("Name Spammer"), &g_Options.Misc.namespam);

			ImGui::Text(XorStr("Custom Name"));
			ImGui::PushItemWidth(180);
			static char buf1[128] = ""; ImGui::InputText(XorStr("##Name"), buf1, 128, ImGuiInputTextFlags_AllowTabInput);
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			if (ImGui::Button(XorStr("Change Name")))
			{
				ConVar* name = Interfaces::CVar()->FindVar("name");

				if (name)
				{
					*(int*)((DWORD)&name->fnChangeCallback + 0xC) = NULL;
					change_name(buf1);
				}
			}
			ImGui::Text(XorStr("Chat Spammer"));
			ImGui::PushItemWidth(180);
			ImGui::Combo(XorStr("##cs"), &g_Options.Misc.spammer, spammers, ARRAYSIZE(spammers));

			ImGui::Text(XorStr("Speed Name"));
			ImGui::PushItemWidth(180);
			ImGui::SliderInt(XorStr("Name Speed : "), &g_Options.Misc.namespeed, 100, 5000, "%.0f");
		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("2 child"), ImVec2(330, 220), true);
		{
			ImGui::Text(XorStr("ClanTag"));
			ImGui::Separator();
			ImGui::Text(XorStr("Choose ClanTag"));
			ImGui::PushItemWidth(180);
			ImGui::Combo(XorStr("##cc"), &g_Options.Misc.syncclantag, "OFF\0\rDARK HOOK\0\rPROJECT Z\0\rRank\0\rVALVE\0\rVALVE NN\0\rDARK HOOK NA\0\rAdmin CM\0\rTime\0\0");

			ImGui::Text(XorStr("Custom ClanTag"));
			ImGui::PushItemWidth(180);
			static char buf2[128] = ""; ImGui::InputText(XorStr("##Clantag"), buf2, 16, ImGuiInputTextFlags_AllowTabInput);
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			if (ImGui::Button(XorStr("Change Clantag")))
			{
				dankestSetClanTag(buf2, buf2);
			}

			ImGui::Text(XorStr("Animated ClanTag"));
			ImGui::PushItemWidth(180);
			ImGui::InputText(XorStr("##AnimatedClantag"), G::AnimatedClantag, 16, ImGuiInputTextFlags_AllowTabInput);
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			if (ImGui::Button(XorStr("Start")))
			{
				g_Options.Misc.animatedclantag = true;
			}

			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			if (ImGui::Button(XorStr("Stop")))
			{
				g_Options.Misc.animatedclantag = false;
			}

		}ImGui::EndChild();
		break;
	}
	case 1:
	{
		ImGui::BeginChild(XorStr("second child"), ImVec2(300, 200), true);
		{
			ImGui::Text(XorStr("Misc Options"));
			ImGui::Separator();

			ImGui::Checkbox(XorStr("Auto Accept"), &g_Options.Misc.AutoAccept);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("SpecList"), &g_Options.Misc.SpecList);
			ImGui::Checkbox(XorStr("No Flash"), &g_Options.Misc.NoFlash);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("No Smoke"), &g_Options.Misc.NoSmoke);
			ImGui::Checkbox(XorStr("DMG Log"), &g_Options.Misc.DMGlog);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("No Scope"), &g_Options.Misc.noscopeborder);
			ImGui::Checkbox(XorStr("Crosshair2"), &g_Options.Misc.Crosshair2);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Time"), &g_Options.Misc.Time);
			ImGui::Checkbox(XorStr("Third Person"), &g_Options.Misc.ThirdPerson);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Hitmarker"), &g_Options.Misc.Hitmarker);
			ImGui::Checkbox(XorStr("Crosshair"), &g_Options.Misc.Crosshair);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Show Ranks"), &g_Options.Misc.Ranks);
			ImGui::Checkbox(XorStr("Recoil Crosshair"), &g_Options.Misc.RecoilCrosshair);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Weapons Log"), &g_Options.Misc.Weaponslog);

			if (ImGui::Checkbox(("AfkBot"), &g_Options.Misc.afkbot))
			{
				if (g_Options.Misc.afkbot)
				{

					Interfaces::Engine()->ClientCmd_Unrestricted2("+forward;+moveleft;+left");

				}
				else if (!g_Options.Misc.afkbot)
				{

					Interfaces::Engine()->ClientCmd_Unrestricted2("-forward;-moveleft;-left");
				}
			}

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("2 child"), ImVec2(200, 200), true);
		{
			ImGui::Text(XorStr("RCS PRO"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("RCS"), &g_Options.Misc.RCS);
			ImGui::SliderFloat(XorStr("Spread : "), &g_Options.Misc.NRCS, 1.0f, 2.0f);
			ImGui::Checkbox(XorStr("Distortion"), &g_Options.Misc.RCS_Distortion);
			ImGui::SliderFloat(XorStr("Distortion : "), &g_Options.Misc.RCS_NDistortion, 1.0f, 5.0f);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("4 child"), ImVec2(200, 200), true);
		{

			ImGui::Text(XorStr(" Move Recorde"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("Replaying"), &g_Options.Misc.isReplaying);
			ImGui::Checkbox(XorStr("Recording"), &g_Options.Misc.isRecording);

		}ImGui::EndChild();
		ImGui::BeginChild(XorStr("6 child"), ImVec2(300, 200), true);
		{
			ImGui::Text(XorStr("World Options"));
			ImGui::Separator();

			ImGui::Text(XorStr("SkyBox"));
			ImGui::PushItemWidth(180);
			ImGui::Combo(XorStr("##sb"), &g_Options.Misc.SkyShit, skybox_items, ARRAYSIZE(skybox_items));
			ImGui::SameLine();
			if (ImGui::Button(XorStr("Chanage")))
			{
				g_Options.Misc.misc_SkyName = skybox_items[g_Options.Misc.SkyShit];
			}
			ImGui::Checkbox(XorStr("NightMode"), &g_Options.Misc.NightMode);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("AsusWalls"), &g_Options.Misc.ASUSWALL);
			ImGui::Checkbox(XorStr("PostProcessing"), &g_Options.Misc.postprocess);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("SnowMode"), &g_Options.Misc.Snow);
			ImGui::Checkbox(XorStr("LSDMode"), &g_Options.Misc.lsdmode);
			ImGui::SameLine(150);
			ImGui::Checkbox(("Chrome"), &g_Options.Misc.chromemode);
			ImGui::Checkbox(XorStr("Minecraft"), &g_Options.Misc.minecraftmode);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("AmbientLight"), &g_Options.Misc.ambientlight);
			ImGui::Checkbox(XorStr("No Sky"), &g_Options.Misc.NoSky);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("5 child"), ImVec2(200, 200), true);
		{
			ImGui::Text(XorStr("Movement Options"));
			ImGui::Separator();

			ImGui::Checkbox(XorStr("Auto Strafe"), &g_Options.Misc.AutoStrafe);
			ImGui::Checkbox(XorStr("Bunny Hop"), &g_Options.Misc.Bhop);
			ImGui::Checkbox(XorStr("Fake Walk"), &g_Options.Misc.fakewalk);
			ImGui::Checkbox(XorStr("Moon Walk"), &g_Options.Misc.moonwalk);
			ImGui::Checkbox(XorStr("Edge jump"), &g_Options.Misc.Edgjump);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("7 child"), ImVec2(200, 200), true);
		{
			ImGui::Text(XorStr("Cmn Options"));
			ImGui::Separator();

			ImGui::Checkbox("Freecam", &g_Options.Misc.bFreeCam);
			ImGui::PushItemWidth(180);
			ImGui::Hotkey("##freecamkey", &g_Options.Misc.iFreeCamKey, ImVec2(150, 25));
			ImGui::SameLine();
			ImGui::Text("Key");
			ImGui::SliderFloat("Speed", &g_Options.Misc.flFreeCamSpeed, 1, 25, "%.1f", (1.0F));

		}ImGui::EndChild();

		break;
	}

	case 2:
	{
		ImGui::BeginChild(XorStr("third child"), ImVec2(200, 200), true);
		{
			ImGui::Text(XorStr("FOV"));
			ImGui::SliderFloat(XorStr("##fov"), &g_Options.Misc.FOVChanger, 0.f, 60.f, "%.0f");
			ImGui::Text(XorStr("Viewmodel FOV"));
			ImGui::SliderFloat(XorStr("##vf"), &g_Options.Misc.viewmodelChanger, 0.f, 120.f, "%.0f");
			ImGui::Text(XorStr("HitSound"));
			ImGui::Combo(XorStr("##asad4asds"), &g_Options.Misc.Hitsound, HitmarkSound, ARRAYSIZE(HitmarkSound));

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("2 child"), ImVec2(200, 200), true);
		{
			ImGui::Text("Radar");

			ImGui::Separator();

			ImGui::Checkbox(("Engine Radar"), &g_Options.Radar.radaringame);

			if (g_Options.Radar.radaringame == true)
			{
				g_Options.Radar.radarwindow = false;
			}
			if (g_Options.Radar.radarwindow == true)
			{
				g_Options.Radar.radaringame = false;
			}

			ImGui::Checkbox("Radar Window", &g_Options.Radar.radarwindow);

			ImGui::SliderFloat("##radrzom", &g_Options.Radar.radrzoom, 0.f, 4.f, "zoom: %.2f");
			ImGui::SliderFloat("##radralpha", &g_Options.Radar.radralpha, 0.f, 1.f, "Alpha: %.2f");
			ImGui::SliderInt(("##Size"), &g_Options.Radar.radrsize, 100, 500, "Size: %.0f");

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("3 child"), ImVec2(200, 200), true);
		{
			ImGui::Checkbox("BlockBot", &g_Options.Misc.bBlockbot);
			ImGui::PushItemWidth(180);
			ImGui::Hotkey("##Blockbotkey", &g_Options.Misc.iBlockbotKey, ImVec2(150, 25));
			ImGui::SameLine();
			ImGui::Text("BlockBot-Key");

		}ImGui::EndChild();
		ImGui::BeginChild(XorStr("4 child"), ImVec2(200, 200), true);
		{

			ImGui::Text("CustomModels");

			ImGui::Separator();
			ImGui::PushItemWidth(180);
			ImGui::Combo(("CT-MODELS"), &g_Options.Misc.customodelsct, customModelsct, ARRAYSIZE(customModelsct));
			ImGui::Combo(("T-MODELS"), &g_Options.Misc.customodelst, customModelst, ARRAYSIZE(customModelst));


			ImGui::Separator();


			if (ImGui::Button("Disable", ImVec2(240.0f, 20.0f)))
			{
				g_Options.Misc.customodelsct = 0;
				g_Options.Misc.customodelst = 0;
				FullUpdate();
			}



		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("5 child"), ImVec2(200, 200), true);
		{
			ImGui::Text("Knifebot Options");
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Checkbox("Active", &g_Options.Knifebot.knf_Active);
			ImGui::Checkbox("Friendly Fire", &g_Options.Knifebot.knf_Team);

			ImGui::Separator();

			string attack_1 = "Trigger";
			string attack_2 = "Backstab Only";
			string attack_3 = "Auto";

			const char* items[] = { attack_1.c_str() , attack_2.c_str() , attack_3.c_str() };
			ImGui::Combo("Type", &g_Options.Knifebot.knf_Attack, items, IM_ARRAYSIZE(items));

			ImGui::Separator();

			ImGui::SliderInt(" trigger", &g_Options.Knifebot.knf_DistAttack, 1, 100);
			ImGui::SliderInt(" backstab", &g_Options.Knifebot.knf_DistAttack2, 1, 100);
		}ImGui::EndChild();
		break;
	}

	}

}
void Changer()
{
	static char filterWeapons[32];
	static char filterSkins[32];
	const char* gstr;
	ImGui::Checkbox(XorStr("Active"), &g_Options.Skinchanger.Enabled);
	ImGui::Separator();
	if (ImGui::Button(XorStr("Weapons Skins"), ImVec2(200, 0)))
	{
		g_Options.Menu.skinstap = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button(XorStr("Sticker"), ImVec2(200, 0)))
	{
		g_Options.Menu.skinstap = 1;
	}
	ImGui::SameLine();
	if (ImGui::Button(XorStr("Weapons Options"), ImVec2(200, 0)))
	{
		g_Options.Menu.skinstap = 2;
	}
	ImGui::Separator();

	switch (g_Options.Menu.skinstap)
		{
		case 0:
		{

			ImGui::BeginChild(XorStr("first child"), ImVec2(200, 480), true);
			{

				ImGui::PushItemWidth(-1);
				ImGui::InputText(XorStr("##FILTERWEAPONS"), filterWeapons, IM_ARRAYSIZE(filterWeapons));
				ImGui::PopItemWidth();
				ImGui::ListBoxHeader(XorStr("##GUNS"), ImVec2(-1, -1));
				for (auto it : ItemDefinitionIndexMap)
				{
					bool isDefault = (int)it.first < 0;
					if (!isDefault && !Contains(ToLower(std::string(filterWeapons)), ToLower(ItemDefinitionIndexMap.at(it.first).entityName)))
						continue;

					if (IsUtility(it.first))
						continue;

					const bool item_selected = ((int)it.first == (int)g_Options.Menu.currentWeapon);
					//ImGui::PushID((int)it.first);
					std::string formattedName;

					formattedName = ItemDefinitionIndexMap.at(it.first).entityName;
					if (ImGui::Selectable(formattedName.c_str(), item_selected))
					{
						g_Options.Menu.currentWeapon = (int)it.first;
					}
					//ImGui::PopID();
				}
				ImGui::ListBoxFooter();
			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("second child"), ImVec2(200, 480), true);
			{
				std::string skinName = weaponnames(g_Options.Menu.currentWeapon);
				{
					ImGui::PushItemWidth(-1);
					ImGui::InputText(XorStr("##FILTERSKINS"), filterSkins, IM_ARRAYSIZE(filterSkins));
					ImGui::InputInt(XorStr("##custom ID"), &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].nFallbackPaintKit);
					ImGui::PopItemWidth();
					ImGui::ListBoxHeader(XorStr("##SKINS"), ImVec2(-1, -1));
					std::string skinStr = "";
					int curItem = -1;
					int s = 0;
					for (auto skin : g_Options.Skinchanger.weaponSkins[skinName])
					{
						int pk = g_Options.Skinchanger.skinMap[skin].paintkit;
						if (pk == g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].nFallbackPaintKit)
							curItem = s;
						bool selected = g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].nFallbackPaintKit == pk;

						skinStr = g_Options.Skinchanger.skinNames[g_Options.Skinchanger.skinMap[skin].tagName].c_str();
						skinStr += " | ";
						skinStr += std::to_string(g_Options.Skinchanger.skinMap[skin].paintkit);
						if (!Contains(ToLower(std::string(filterSkins)), ToLower(skinStr)))
							continue;
						if (ImGui::Selectable(skinStr.c_str(), selected))
						{
							pk = g_Options.Skinchanger.skinMap[skin].paintkit;
							g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].nFallbackPaintKit = pk;
							FullUpdate();
							break;
						}
						s++;
					}

					ImGui::ListBoxFooter();
				}
			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("third child"), ImVec2(200, 480), true);
			{
				ImGui::ListBoxHeader("##KNIVES", ImVec2(-1, -1));
				{
					if (ImGui::RadioButton(XorStr("Default"), &g_Options.Skinchanger.knifemodel, 0))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Bayonet"), &g_Options.Skinchanger.knifemodel, 1))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Flip Knife"), &g_Options.Skinchanger.knifemodel, 2))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Gut Knife"), &g_Options.Skinchanger.knifemodel, 3))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Karambit"), &g_Options.Skinchanger.knifemodel, 4))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("M9 Bayonet"), &g_Options.Skinchanger.knifemodel, 5))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Huntsman"), &g_Options.Skinchanger.knifemodel, 6))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Falchion"), &g_Options.Skinchanger.knifemodel, 7))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Bowie"), &g_Options.Skinchanger.knifemodel, 8))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Butterfly Knife"), &g_Options.Skinchanger.knifemodel, 9))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Shadow Daggers"), &g_Options.Skinchanger.knifemodel, 10))
					{
						FullUpdate();
					}
				}
				ImGui::ListBoxFooter();
			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("fourth child"), ImVec2(200, 480), true);
			{
				
				if (g_Options.Skinchanger.glove == 1)
				{
					gstr = "Charred\0\rSnakebite\0\rBronzed\0\rGuerilla\0\0";
				}
				else if (g_Options.Skinchanger.glove == 2)
				{
					gstr = "Hedge Maze\0\rPandoras Box\0\rSuperconductor\0\rArid\0\rVice\0\rOmega\0\rBronze Morph\0\rAmphibious\0\0";
				}
				else if (g_Options.Skinchanger.glove == 3)
				{
					gstr = "Lunar Weave\0\rConvoy\0\rCrimson Weave\0\rDiamondback\0\rRacing Green\0\rOvertake\0\rImperial Plaid\0\rKing Snake\0\0";
				}
				else if (g_Options.Skinchanger.glove == 4)
				{
					gstr = "Leather\0\rSpruce DDPAT\0\rSlaughter\0\rBadlands\0\rArboreal\0\rDuct Tape\0\rOverprint\0\rCobalt Skulls\0\0";
				}
				else if (g_Options.Skinchanger.glove == 5)
				{
					gstr = "Eclipse\0\rSpearmint\0\rBoom!\0\rCool Mint\0\rPolygon\0\rTransport\0\rTurtle\0\rPOW!\0\0";
				}
				else if (g_Options.Skinchanger.glove == 6)
				{
					gstr = "Forest DDPAT\0\rCrimson Kimono\0\rEmerald Web\0\rFoundation\0\rMogul\0\rFade\0\rBuckshot\0\rCrimson Web\0\0";
				}
				else if (g_Options.Skinchanger.glove == 7)
				{
					gstr = "Case Hardened\0\rRattler\0\rMangrove\0\rEmerald\0\0";
				}
				else
				{
					gstr = "";
				}
				ImGui::ListBoxHeader("##GLOVES", ImVec2(-1, -1));
				{
					if (ImGui::RadioButton(XorStr("Bloodhound"), &g_Options.Skinchanger.glove, 1))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Sport"), &g_Options.Skinchanger.glove, 2))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Driver"), &g_Options.Skinchanger.glove, 3))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Hand Wraps"), &g_Options.Skinchanger.glove, 4))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Motorcycle"), &g_Options.Skinchanger.glove, 5))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Specialst"), &g_Options.Skinchanger.glove, 6))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(XorStr("Hydra"), &g_Options.Skinchanger.glove, 7))
					{
						FullUpdate();
					}
					ImGui::PushItemWidth(150);
					if (ImGui::Combo(XorStr("##2"), &g_Options.Skinchanger.gloveskin, gstr, -1))
					{
						FullUpdate();

					}
				}
			ImGui::ListBoxFooter();
			}ImGui::EndChild();
			break;
		}
		case 1 :
		{
			ImGui::BeginChild(XorStr("1 child"), ImVec2(300, 480), true);
			{
				ImGui::Checkbox("Enable Sticker", &g_Options.Skinchanger.EnabledSticker);

				ImGui::Combo("ID##0", &g_Options.Sticker.Weapons[g_Options.Menu.currentWeapon].Sticker[0].ID, [](void* data, int idx, const char** out_text)
				{
					*out_text = k_stickers.at(idx).name.c_str();
					return true;
				}, nullptr, k_stickers.size());
				ImGui::Combo("ID##1", &g_Options.Sticker.Weapons[g_Options.Menu.currentWeapon].Sticker[1].ID, [](void* data, int idx, const char** out_text)
				{
					*out_text = k_stickers.at(idx).name.c_str();
					return true;
				}, nullptr, k_stickers.size());

				ImGui::Combo("ID##2", &g_Options.Sticker.Weapons[g_Options.Menu.currentWeapon].Sticker[2].ID, [](void* data, int idx, const char** out_text)
				{
					*out_text = k_stickers.at(idx).name.c_str();
					return true;
				}, nullptr, k_stickers.size());
				ImGui::Combo("ID##3", &g_Options.Sticker.Weapons[g_Options.Menu.currentWeapon].Sticker[3].ID, [](void* data, int idx, const char** out_text)
				{
					*out_text = k_stickers.at(idx).name.c_str();
					return true;
				}, nullptr, k_stickers.size());

				ImGui::Combo("ID##4", &g_Options.Sticker.Weapons[g_Options.Menu.currentWeapon].Sticker[4].ID, [](void* data, int idx, const char** out_text)
				{
					*out_text = k_stickers.at(idx).name.c_str();
					return true;
				}, nullptr, k_stickers.size());

			}ImGui::EndChild();
			break;
		}
		case 2 :
		{
			ImGui::BeginChild(XorStr("Fifthly child"), ImVec2(300, 480), true);
			{
				ImGui::Text(XorStr("Weapon Quality"));
				ImGui::Combo("##Quality", &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].iEntityQuality, quality_items, IM_ARRAYSIZE(quality_items));
				ImGui::Text(XorStr("Weapon Wear"));
				ImGui::SliderFloat("##Wear", &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].flFallbackWear, 0.f, 1.f);
				ImGui::Text(XorStr("Weapon StatTrak"));
				ImGui::InputInt("##StatTrak", &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].nFallbackStatTrak, 1, 100, ImGuiInputTextFlags_CharsDecimal);
				ImGui::Text(XorStr("Weapon Name"));
				ImGui::InputText(XorStr("##Name"), g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerName, 32, ImGuiInputTextFlags_AllowTabInput);
				ImGui::Text(XorStr("Weapon Seed"));
				ImGui::InputInt("Seed", &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].flFallbackSeed);

			}ImGui::EndChild();
		}
			break;
		}

	if (ImGui::Button(XorStr("Update"), ImVec2(-1, 20)))
		{
			FullUpdate();
		}

}
void Colors()
{
	switch (g_Options.Menu.iColorsSubTab)
	{
	case 0:
	{
		auto& style = ImGui::GetStyle();

		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);

		static int selectedItem = 0;
		ImVec2 lastCursor = ImGui::GetCursorPos();

		ImGui::ListBoxHeader("##0", ImVec2(240, 540));
		for (int i = 0; i < Global::ColorsForPicker1.size(); i++)
		{
			bool selected = i == selectedItem;

			if (ImGui::Selectable(Global::ColorsForPicker1[i].Name, selected))
				selectedItem = i;
		}
		ImGui::ListBoxFooter();


		ImGui::SameLine(0.0f, 15.0f);

		ImGui::BeginChild("#generalcolors", ImVec2(580, 540), false, ImGuiWindowFlags_NoScrollbar);
		{
			style.Colors[ImGuiCol_ChildWindowBg] = ImColor(10, 10, 10, 255);

			ImGui::BeginChild("#ColorsChild", ImVec2(570, 130), true);
			{
				style.Colors[ImGuiCol_ChildWindowBg] = ImColor(10, 10, 10, 255);

				ImGui::Spacing();
				ImGui::Spacing();
				ColorP col = Global::ColorsForPicker1[selectedItem];
				int r = (col.Ccolor[0] * 255.f);
				int g = (col.Ccolor[1] * 255.f);
				int b = (col.Ccolor[2] * 255.f);
				ImGui::PushItemWidth(400);
				ImGui::SliderInt("##red", &r, 0, 255, "%.0f"); ImGui::SameLine(); ImGui::Text("Red Amount");
				ImGui::Spacing();
				ImGui::SliderInt("##green", &g, 0, 255, "%.0f"); ImGui::SameLine(); ImGui::Text("Green Amount");
				ImGui::Spacing();
				ImGui::SliderInt("##blue", &b, 0, 255, "%.0f"); ImGui::SameLine(); ImGui::Text("Blue Amount");
				ImGui::PopItemWidth();
				col.Ccolor[0] = r / 255.0f;
				col.Ccolor[1] = g / 255.0f;
				col.Ccolor[2] = b / 255.0f;

			}ImGui::EndChild();


			ImGui::BeginChild("#colorpreview", ImVec2(570, 540), true);
			{
				style.Colors[ImGuiCol_ChildWindowBg] = ImColor(10, 10, 10, 255);

				ColorP col = Global::ColorsForPicker1[selectedItem];
				ImGui::ColorPicker("##COLOR_PICKER", col.Ccolor);

			}ImGui::EndChild();

		}ImGui::EndChild();

		break;
	}
	}

}
void Settings()
{
	static int iConfigSelect = 0;
	static int iMenuSheme = 1;
	static char ConfigName[64] = { 0 };


		ImGui::BeginChild(XorStr("first child"), ImVec2(400, 200), true);
		{
			ImGui::ComboBoxArray(XorStr("Select Config"), &iConfigSelect, ConfigList);

			ImGui::Separator();

			if (ImGui::Button(XorStr("Load Config")))
			{
				LoadSettings("C:/DARKHOOK/" + ConfigList[iConfigSelect]);
				FullUpdate();
			}
			ImGui::SameLine();
			if (ImGui::Button(XorStr("Save Config")))
			{
				SaveSettings("C:/DARKHOOK/" + ConfigList[iConfigSelect]);
			}
			ImGui::SameLine();
			if (ImGui::Button(XorStr("Refresh Config List")))
			{
				RefreshConfigs();
			}

			ImGui::Separator();

			ImGui::InputText(XorStr("Config Name"), ConfigName, 64);

			if (ImGui::Button(XorStr("Create & Save new Config")))
			{
				string ConfigFileName = ConfigName;

				if (ConfigFileName.size() < 1)
				{
					ConfigFileName = "settings";
				}

				SaveSettings("C:/DARKHOOK/" + ConfigFileName + ".ini");
				RefreshConfigs();
			}

			ImGui::Separator();
		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("2 child"), ImVec2(180, 50), true);
		{
			const char* ThemesList[] = { "purple" , "DefaultSheme1" , "RedSheme" , "darkblue" , "MidNightSheme" , "NightSheme" , "DunnoSheme" , "BlueSheme"  , "MidNight2" , "BlackSheme2" , "green" , "pink" , "blue" , "yellow" , "BlackSheme" };

			ImGui::Combo(XorStr("Themes"), &g_Options.Menu.Theme, ThemesList, IM_ARRAYSIZE(ThemesList), -1);
		}ImGui::EndChild();


}
void Hotkeys()
{
	ImGui::BeginChild(XorStr("second child"), ImVec2(200, 400), true);
	{
		ImGui::Text(XorStr("Ragebot Key"));
		ImGui::PushItemWidth(184);
		ImGui::Hotkey(XorStr("Key##999"), &g_Options.Ragebot.KeyPress);
		ImGui::Text(XorStr("Ragebot BAIM Key"));
		ImGui::Hotkey(XorStr("Key##998"), &g_Options.Ragebot.BAIMkey);
		ImGui::Text(XorStr("AirStuck Key"));
		ImGui::Hotkey(XorStr("##as"), &g_Options.Misc.AirlockKey);
		ImGui::Text(XorStr("Third Person Key"));
		ImGui::Hotkey(XorStr("##TP"), &g_Options.Misc.TPKey);
		ImGui::Text(XorStr("Flash Light"));
		ImGui::Hotkey(XorStr("##flashlightkey"), &g_Options.Misc.flashlight);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(XorStr("third child"), ImVec2(200, 200), true);
	{

		ImGui::Text(XorStr("Edge Jump Key"));
		ImGui::Hotkey(XorStr("##ej"), &g_Options.Misc.Edgekey);

		ImGui::Text(XorStr("Flip AA"));
		ImGui::Hotkey(XorStr("##faa"), &g_Options.Ragebot.flipkey);

		ImGui::Text(XorStr("Slow Move"));
		ImGui::Hotkey(XorStr("##sm"), &g_Options.Misc.slowmovekey);

		ImGui::Text(XorStr("Fake Walk"));
		ImGui::Hotkey(XorStr("##fw"), &g_Options.Misc.fakewalkkey);

		ImGui::Text(XorStr("Moon Walk"));
		ImGui::Hotkey(XorStr("##mw"), &g_Options.Misc.moonwalkkey);

	}ImGui::EndChild();
}

static const char* sidebar_tabs[] = {
	"Ragebot",
	"Legitbot",
	"Triggerbot",
	"Visual",
	"Misc",
	"Changer",
	"Colors",
	"Settings",
	"HotKeys",
	"",
	"",
};

void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
{
	ImDrawList* windowDrawList = ImGui::GetWindowDrawList();

	Color colColor(0, 0, 0, 255);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		windowDrawList->AddRectFilled(ImVec2(x + i, y), ImVec2(width, height), colRainbow.GetU32());
	}
}


template<size_t N>
void render_tabs(const char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
	bool values[N] = { false };

	values[activetab] = true;

	for (auto i = 0u; i < N; ++i) {
		if (ImGui::Button(names[i], ImVec2{ w, h })) {
			activetab = i;
		}
		if (sameline && i < N - 1)
			ImGui::SameLine();
	}
}

int get_fps()
{
	using namespace std::chrono;
	static int count = 0;
	static auto last = high_resolution_clock::now();
	auto now = high_resolution_clock::now();
	static int fps = 0;

	count++;

	if (duration_cast<milliseconds>(now - last).count() > 1000) {
		fps = count;
		count = 0;
		last = now;
	}

	return fps;
}
constexpr static float get_sidebar_item_width() { return 150.0f; }
constexpr static float get_sidebar_item_height() { return  50.0f; }




ImVec2 get_sidebar_size()
{
	constexpr float padding = 10.0f;
	constexpr auto size_w = padding * 2.0f + get_sidebar_item_width();
	constexpr auto size_h = padding * 2.0f + (sizeof(sidebar_tabs) / sizeof(char*)) * get_sidebar_item_height();

	return ImVec2{ size_w, size_h };
}


void RenderInterface() {



	auto& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = GuiFile.c_str();

	if (bIsGuiVisible)
		if (style.Alpha > 1.f)
			style.Alpha = 1.f;
		else if (style.Alpha != 1.f)
			style.Alpha += 0.01f;
		else if (!bIsGuiVisible)
			if (style.Alpha < 0.f)
				style.Alpha = 0.f;
			else if (style.Alpha != 0.f)
				style.Alpha -= 0.01f;


		if (Globals::error)
		{
			ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
			ImGui::SetNextWindowSize(ImVec2(200, 50));
			if (ImGui::Begin(XorStr("Warning!"), &bIsGuiVisible, ImVec2(400, 200), 1.f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
			{
				ImGui::Text(XorStr("Please dont enable both Builder and Pre-set AAs!"));
				if (ImGui::Button(XorStr("OK!")))
				{
					g_Options.Ragebot.PreAAs = false;
					g_Options.Ragebot.BuilderAAs = false;
					Globals::error = false;
				}
			}ImGui::End();
		}

		ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2{ 1040, 650 }, ImGuiSetCond_Once);
		const auto sidebar_size = get_sidebar_size();
		static int active_sidebar_tab = 0;
		if (ImGui::Begin(" PROJECT Z V 2.0", &bIsGuiVisible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize))
		{


			static float flRainbow;
			float flSpeed = 0.0003f;
			int curWidth = 1;
			ImVec2 curPos = ImGui::GetCursorPos();
			ImVec2 curWindowPos = ImGui::GetWindowPos();
			curPos.x += curWindowPos.x;
			curPos.y += curWindowPos.y;
			int size1;
			int y;
		Interfaces::Engine()->GetScreenSize(y, size1);
			DrawRectRainbow(curPos.x - 10, curPos.y - 5, ImGui::GetWindowSize().x + size1, curPos.y + -4, flSpeed, flRainbow);



			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
			{
				ImGui::BeginGroupBox("##sidebar", sidebar_size);
				{
					ImGui::GetCurrentWindow()->Flags &= ~ImGuiWindowFlags_ShowBorders;

					render_tabs(sidebar_tabs, active_sidebar_tab, get_sidebar_item_width(), get_sidebar_item_height(), false);
				}
				ImGui::EndGroupBox();
			}
			ImGui::PopStyleVar();
			ImGui::SameLine();


			auto size = ImVec2{ 0.0f, sidebar_size.y };

			ImGui::BeginGroupBox("##body", size);
			if (active_sidebar_tab == 0) {
				Ragebot();
			}
			else if (active_sidebar_tab == 1) {
				Legitbot();
			}
			else if (active_sidebar_tab == 2) {
				Trigeerbot();
			}
			else if (active_sidebar_tab == 3) {
				Visuals();
			}
			else if (active_sidebar_tab == 4) {
				Misc();
			}
			else if (active_sidebar_tab == 5) {
				Changer();
			}
			else if (active_sidebar_tab == 6) {
				Colors();
			}
			else if (active_sidebar_tab == 7) {
				Settings();
			}
			else if (active_sidebar_tab == 8) {
				Hotkeys();
			}
			ImGui::EndGroupBox();



			ImGui::TextColored(ImVec4{ 0.0f, 0.5f, 0.0f, 1.0f }, "FPS: %03d", get_fps());
			ImGui::End();
		}

	

	
}
