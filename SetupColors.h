#pragma once
#include "Global.h"

//Colors
void SetupColors()
{
	//ESP
	Global::ColorsForPicker1.push_back(ColorP("Vis CT - Box", g_Options.Color.esp_Color_VCT));
	Global::ColorsForPicker1.push_back(ColorP("Vis T - Box", g_Options.Color.esp_Color_VTT));

	Global::ColorsForPicker1.push_back(ColorP("InVis CT - Box", g_Options.Color.esp_Color_CT));
	Global::ColorsForPicker1.push_back(ColorP("InVis T - Box", g_Options.Color.esp_Color_TT));

	Global::ColorsForPicker1.push_back(ColorP("Enemy - Fill-Box", g_Options.Color.fill_color_enemy));
	Global::ColorsForPicker1.push_back(ColorP("Team - Fill-Box", g_Options.Color.fill_color_team));
	
	Global::ColorsForPicker1.push_back(ColorP("Skeleton", g_Options.Color.color_skeleton));
	Global::ColorsForPicker1.push_back(ColorP("Hitbox Backtrack", g_Options.Color.hitbox));

	Global::ColorsForPicker1.push_back(ColorP("Light T", g_Options.Color.Light_TT));
	Global::ColorsForPicker1.push_back(ColorP("Light CT", g_Options.Color.Light_CT));

	Global::ColorsForPicker1.push_back(ColorP("Grenade Prediction Box", g_Options.Color.GrenadeCollision));
	Global::ColorsForPicker1.push_back(ColorP("Grenade Prediction Circle", g_Options.Color.color_grenadeprediction_circle));

	Global::ColorsForPicker1.push_back(ColorP("Grenade Prediction Line", g_Options.Color.color_grenadeprediction));
	Global::ColorsForPicker1.push_back(ColorP("Dropped Guns", g_Options.Color.droppedguns));

	//chams
	Global::ColorsForPicker1.push_back(ColorP("Players Chams", g_Options.Color.EnemyChamsVis));
	Global::ColorsForPicker1.push_back(ColorP("Team Chams", g_Options.Color.TeamChamsVis));

	Global::ColorsForPicker1.push_back(ColorP("Fake Angle", g_Options.Color.esp_player_fakechams_color));
	Global::ColorsForPicker1.push_back(ColorP("Hands", g_Options.Color.HandCham));

	Global::ColorsForPicker1.push_back(ColorP("Weapon Chams", g_Options.Color.WeaponHandCham));
	Global::ColorsForPicker1.push_back(ColorP("Weapons Dropped Chams", g_Options.Color.WeaponDropped));

	//glow
	Global::ColorsForPicker1.push_back(ColorP("Player Glow", g_Options.Color.EnemyGlow));
	Global::ColorsForPicker1.push_back(ColorP("Team Glow", g_Options.Color.TeamGlow));

	Global::ColorsForPicker1.push_back(ColorP("Weapons Glow", g_Options.Color.WeaponGlow));
	Global::ColorsForPicker1.push_back(ColorP("Bomb Glow", g_Options.Color.C4glow));

	// crosshair
	Global::ColorsForPicker1.push_back(ColorP("Crosshair", g_Options.Color.Crosshair));
	Global::ColorsForPicker1.push_back(ColorP("Crosshair Spread", g_Options.Color.CrsSpread));

	Global::ColorsForPicker1.push_back(ColorP("Sniper Crosshair", g_Options.Color.color_sniper));
	Global::ColorsForPicker1.push_back(ColorP("Scoped", g_Options.Color.Scope));
	// colrinvisible

	Global::ColorsForPicker1.push_back(ColorP("InVis CT", g_Options.Color.esp_Color_CT));
	Global::ColorsForPicker1.push_back(ColorP("InVis T", g_Options.Color.esp_Color_TT));

	Global::ColorsForPicker1.push_back(ColorP("InVis Chams Players", g_Options.Color.EnemyChamsNVis));
	Global::ColorsForPicker1.push_back(ColorP("InVis Chams Team", g_Options.Color.TeamChamsNVis));

	// other

	Global::ColorsForPicker1.push_back(ColorP("backtrack", g_Options.Color.backtrack));
	Global::ColorsForPicker1.push_back(ColorP("Ambient Light", g_Options.Color.esp_Ambient));

	Global::ColorsForPicker1.push_back(ColorP("Bullet Impacts", g_Options.Color.flTracers));
	Global::ColorsForPicker1.push_back(ColorP("World Mode", g_Options.Color.worldmode));

}