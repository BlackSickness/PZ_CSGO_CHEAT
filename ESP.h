
#pragma once

#include "SDK.h"
#include "singleton.hpp"
#include "Color2.h"
#include "bulletshow.h"

class visuals
	:public singleton<visuals>
{
public:
	visuals();
	static void Hitbox(int index);
	void OnPaintTraverse();
	void renderBeams();
	void custommodels(C_BaseEntity* entity);
	std::vector<cbullet_tracer_info>logs;

private:

	C_BaseEntity *BombCarrier;
	void RenderWeapon(C_BaseEntity * entity, Color col);
	void DrawBombPlanted(C_BaseEntity * entity, C_BaseEntity * local);
	void DrawBomb(C_BaseEntity * entity, ClientClass * cClass);
	Color GetPlayerColor(C_BaseEntity* pPlayer);
	Color GetPlayerVisibleColor(C_BaseEntity* local,C_BaseEntity* pPlayer);
	void DrawBox(RECT rect, Color Col);
	void DrawCorners(RECT rect, Color Col);
	void ThreeDBox(Vector minin, Vector maxin, Vector pos, Color Col);
	void FillBox(RECT rect, C_BaseEntity* local, C_BaseEntity* entity);
	void BulletTrace(C_BaseEntity* pEntity, Color color);
	void DrawSnapLine(Vector to, Color clr);
	void DrawHealth(RECT rect, C_BaseEntity* pPlayer);
	void edgyHealthBar(RECT rect, C_BaseEntity* pEntity);
	void armorbar(RECT rect, C_BaseEntity* pEntity);
	void DrawInfo(RECT rect, C_BaseEntity * entity, C_BaseEntity * local);
	void DrawAngles();
	void DrawAwall();
	void DrawFov();
	RECT DynamicBox(C_BaseEntity* pPlayer, bool& PVS, C_BaseEntity* local);
	void RenderFill(RECT rect);
	void WorldModes();
	void DLight(C_BaseEntity* entity);
	void Crosshair(C_BaseEntity* local);
	void RecoilCrosshair(C_BaseEntity* local);
	void Crosshair2();
	void NightMode();
	void Hitmarker();
	void NoSky();
	void ASUSWALL();
	void Clear();
	void DrawCrosshair(C_BaseEntity* local,  CBaseCombatWeapon* Weapon,Color color);
	void Skeleton(C_BaseEntity* pEntity, Color Color);
	void FingerSkeleton(C_BaseEntity * pEntity, Color Col);
	void HealthSkeleton(C_BaseEntity * pEntity);

};

namespace ColorAlpha
{
	struct VisualsStruct
	{
		Color2 clr_fill;
		C_BaseEntity* player;
	};

	extern VisualsStruct ESP_ctx;
	extern Color2 clr_fill;
}

inline int flGetDistance(Vector from, Vector to)
{
	Vector angle;
	angle.x = to.x - from.x;	angle.y = to.y - from.y;	angle.z = to.z - from.z;

	return floor(sqrt(angle.x*angle.x + angle.y*angle.y + angle.z*angle.z) * 0.0254f);
}

inline float CSGO_Armor(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue)) {
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}

char* const itemIcons[] =
{
	"3", //0 - default
	"A", //1
	"B", //2
	"C", //3
	"D", //4
	"none", //5
	"none", //6
	"W", //7
	"U", //8
	"Z", //9

	"R", //10
	"X", //11
	"none", //12
	"Q", //13
	"g", //14
	"none", //15
	"S", //16
	"K", //17
	"none", //18
	"P", //19

	"none", //20
	"none",
	"none",
	"none",
	"L",
	"b",
	"M",
	"d",
	"f",
	"c",

	"H", //30
	"h",
	"E",
	"N",
	"O",
	"e",
	"F",
	"none",
	"Y",
	"V",

	"a", //40
	"1",
	"1",
	"i",
	"j",
	"k",
	"l",
	"m",
	"n",
	"o",

	"none", //50
	"none",
	"none",
	"none",
	"none",
	"none",
	"none",
	"none",
	"none",
	"knife",

	"T", //60
	"G",
	"none",
	"I",
	"J"
};

inline const char* ItemDefinitionIndexToString2(int index)
{
	if (index < 0 || index > 64)
		index = 0;

	return itemIcons[index];
}


char* const itemNames[] =
{

	"KNIFE", //0 - default
	"DEAGLE",
	"ELITE",
	"FIVESEVEN",
	"GLOCK",
	"none",
	"none",
	"AK-47",
	"AUG",
	"AWP",

	"FAMAS", //10
	"G3SG1",
	"none",
	"GALIL",
	"M249",
	"none",
	"M4A4",
	"MAC-10",
	"none",
	"P90",

	"none", //20
	"none",
	"none",
	"none",
	"UMP-45",
	"XM1014",
	"BIZON",
	"MAG-7",
	"NEGEV",
	"SAWED-OFF",

	"TEC-9", //30
	"Zeus-x27",
	"P2000",
	"MP-7",
	"MP-9",
	"NOVA",
	"P250",
	"none",
	"SCAR-20",
	"SG-556",

	"SSG-08", //40
	"KNIFE",
	"KNIFE",
	"FLASH",
	"NADE",
	"SMOKE",
	"MOLOTOV",
	"DECOY",
	"INCENDIARY",
	"C4",

	"none", //50
	"none",
	"none",
	"none",
	"none",
	"none",
	"none",
	"none",
	"none",
	"KNIFE",

	"M4A1", //60
	"USP",
	"none",
	"CZ-75",
	"REVOLVER"
};




inline const char* ItemDefinitionIndexToString(int index)
{
	if (index < 0 || index > 64)
		index = 0;

	return itemNames[index];
}