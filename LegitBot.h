


#pragma once

#include "SDK.h"
#include "singleton.hpp"

class legitbot
	: public singleton<legitbot>
{
public:
	legitbot();
	void OnCreateMove(CInput::CUserCmd *pCmd, C_BaseEntity* local, bool& bSendPacket);
private:
	// Targetting

	void weapon_settings(CBaseCombatWeapon* weapon);
	bool hit_chance(C_BaseEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon, C_BaseEntity* target);
	void LegitAA(C_BaseEntity* local, CBaseCombatWeapon* weapon, CInput::CUserCmd *pCmd, bool& bSendPacket);
	void FakeLag(C_BaseEntity* pLocal, CInput::CUserCmd *pCmd, bool& bSendPacket);
	void do_aimbot(C_BaseEntity *local, CBaseCombatWeapon *weapon, CInput::CUserCmd *cmd);
	void triggerbot(CInput::CUserCmd* cmd, C_BaseEntity* local, CBaseCombatWeapon* weapon);
	void AutoPistol(CInput::CUserCmd *cmd, C_BaseEntity* local, CBaseCombatWeapon* weapon);
	void FastZoom(CInput::CUserCmd *cmd, C_BaseEntity* local, CBaseCombatWeapon* weapon);
	void AimSniper(C_BaseEntity *local);
	// Functionality
	bool get_hitbox(C_BaseEntity *local, C_BaseEntity *entity, Vector &destination);
	int get_target(C_BaseEntity *local, CBaseCombatWeapon *weapon, CInput::CUserCmd *cmd, Vector &destination);
	void RCS(C_BaseEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon);
	// Aimbot
	int best_target;
	Vector vSmoothAimAngle;
	QAngle view_angle;
	QAngle aim_angle;
	QAngle delta_angle;

	Vector hitbox_position;

	int aim_key;
	int TriggerbotKey;
	int aim_rcstype;
	float TriggerbotDelay;
	float aim_smooth;
	float aim_fov;
	float aim_rcs;
	float Triggerbot_hitchance;
	bool Triggerbot_AutoFire;
	bool Triggerbot;
	bool AutoLock;
	bool Silent;
	bool shoot;
	bool fakelag;
	bool draw_fov;
	float nfakelag;
	int fakelag_mode;
	int hitboxpon;
	struct
	{
		bool Head = false;
		bool Arms = false;
		bool Chest = false;
		bool Stomach = false;
		bool Legs = false;
	} Triggerbot_Filter;

};