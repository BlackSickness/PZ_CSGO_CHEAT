
#pragma once

#include "SDK.h"
#include "singleton.hpp"

class ragebot
	: public singleton<ragebot>
{
public:

	ragebot();

	void OnCreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket);
	bool hit_chance(C_BaseEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon, C_BaseEntity* target);
	Vector BestPoint(C_BaseEntity *targetPlayer, Vector &final);
private:
	float FovToPlayer(Vector ViewOffSet, Vector View, C_BaseEntity* pEntity, int aHitBox);
	int GetTargetCrosshair();
	int GetTargetDistance();
	int GetTargetHealth();
	int GetTargetThreat(CInput::CUserCmd* pCmd);
	void AutoZeus(C_BaseEntity* pBaseEntity, CInput::CUserCmd* pCmd);
	bool TargetMeetsRequirements(C_BaseEntity* pEntity);
	int HitScan(C_BaseEntity* pEntity);
	void DoNoRecoil(CInput::CUserCmd* pCmd);
	bool AimAtPoint(C_BaseEntity* pLocal, Vector point, CInput::CUserCmd *pCmd);
	void DoAimbot(CInput::CUserCmd *pCmd, bool& bSendPacket);
	void DoAntiAim(CInput::CUserCmd *pCmd, bool& bSendPacket);
	void AIFakeLag(C_BaseEntity* pLocal, bool& bSendPacket);
	void aimAtPlayer(CInput::CUserCmd *pCmd);
	void DoNoSpread(CInput::CUserCmd *pCmd, CBaseCombatWeapon* weapon, C_BaseEntity* local, C_BaseEntity* target);
	void FAKELAG( bool& bSendPacket);

private:
	bool IsLocked;
	int TargetID;
	int HitBox;
	C_BaseEntity* pTarget;

};

inline int LagCompBreak(C_BaseEntity* local) {
	Vector velocity = local->GetVelocity();
	velocity.z = 0;
	float speed = velocity.Length();
	if (speed > 0.f) {
		auto distance_per_tick = speed *
			Interfaces::Globals()->interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, 14);
	}
	return 1;
}


inline bool CanAttack()
{
	auto local = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	if (local && local->IsAlive())
	{
		CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
		if (weapon)
		{
			if (weapon->ammo() < 1 && !MiscFunctions::IsKnife(weapon))
				return false;

			bool revolver_can_shoot = true;
			if (*weapon->GetItemDefinitionIndex() == 64)
			{
				float time_to_shoot = weapon->m_flPostponeFireReadyTime() - local->GetTickBase() * Interfaces::Globals()->interval_per_tick;
				revolver_can_shoot = time_to_shoot <= Interfaces::Globals()->absoluteframetime;
			}

			float time = local->GetTickBase() * Interfaces::Globals()->interval_per_tick;
			float next_attack = weapon->GetNextPrimaryAttack();
			return revolver_can_shoot && !(next_attack > time);
		}
	}

	return false;
}