#include "AutoWall.h"
//#include "R.h"



void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, C_BaseEntity* ignore, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilter filter;
	filter.pSkip = ignore;

	Interfaces::EngineTrace()->TraceRay(ray, mask, &filter, ptr);
}
float DistanceToRay(const Vector &pos, const Vector &rayStart, const Vector &rayEnd, float *along = NULL, Vector *pointOnRay = NULL)
{
	Vector to = pos - rayStart;
	Vector dir = rayEnd - rayStart;
	float length = dir.NormalizeInPlace();

	float rangeAlong = dir.Dot(to);
	if (along)
	{
		*along = rangeAlong;
	}

	float range;

	if (rangeAlong < 0.0f)
	{
		// off start point
		range = -(pos - rayStart).Length();

		if (pointOnRay)
		{
			*pointOnRay = rayStart;
		}
	}
	else if (rangeAlong > length)
	{
		// off end point
		range = -(pos - rayEnd).Length();

		if (pointOnRay)
		{
			*pointOnRay = rayEnd;
		}
	}
	else // within ray bounds
	{
		Vector onRay = rayStart + (dir.operator*(rangeAlong));
		range = (pos - onRay).Length();

		if (pointOnRay)
		{
			*pointOnRay = onRay;
		}
	}

	return range;
}

void UTIL_ClipTraceToPlayers(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	trace_t playerTrace;
	Ray_t ray;
	float smallestFraction = tr->fraction;
	const float maxRange = 60.0f;

	ray.Init(vecAbsStart, vecAbsEnd);

	for (int k = 1; k <= Interfaces::Globals()->maxClients; ++k)
	{
		C_BaseEntity *player = (C_BaseEntity*)Interfaces::EntityList()->GetClientEntity(k);

		if (!player || !player->IsAlive())
			continue;

		if (player->IsDormant())
			continue;


		if (filter && filter->ShouldHitEntity((C_BaseEntity*)player, mask) == false)
			continue;

		float range = DistanceToRay(player->WorldSpaceCenter(), vecAbsStart, vecAbsEnd);
		if (range < 0.0f || range > maxRange)
			continue;

		Interfaces::EngineTrace()->ClipRayToEntity(ray, mask | CONTENTS_HITBOX, (C_BaseEntity*)player, &playerTrace);
		if (playerTrace.fraction < smallestFraction)
		{
			// we shortened the ray - save off the trace
			*tr = playerTrace;
			smallestFraction = playerTrace.fraction;
		}
	}

}

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	static DWORD TraceToExit = CSX::Memory::FindPattern(CLIENT_DLL, "\x55\x8B\xEC\x83\xEC\x30\xF3\x0F\x10\x75", "xxxxxxxxxx",0);

	if (!TraceToExit)
		return false;

	float start_y = start.y, start_z = start.z, start_x = start.x, dir_y = vEnd.y, dir_x = vEnd.x, dir_z = vEnd.z;

	_asm
	{
		push trace
		push dir_z
		push dir_y
		push dir_x
		push start_z
		push start_y
		push start_x
		mov edx, tr
		mov ecx, end
		call TraceToExit
		add esp, 0x1C
	}
}
bool TraceToExit1(Vector &end, trace_t *enter_trace, Vector start, Vector dir, trace_t *exit_trace)
{
	float distance = 0.0f;

	while (distance <= 90.0f)
	{
		distance += 4.0f;
		end = start + dir * distance;

		auto point_contents = Interfaces::EngineTrace()->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);

		if (point_contents & MASK_SHOT_HULL && !(point_contents & CONTENTS_HITBOX))
			continue;

		auto new_end = end - (dir * 4.0f);

		Ray_t ray;
		ray.Init(end, new_end);
		Interfaces::EngineTrace()->TraceRay(ray, MASK_SHOT, 0, exit_trace);

		if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX)
		{
			ray.Init(end, start);

			CTraceFilter filter;
			filter.pSkip = exit_trace->m_pEnt;

			Interfaces::EngineTrace()->TraceRay(ray, 0x600400B, &filter, exit_trace);

			if ((exit_trace->fraction < 1.0f || exit_trace->allsolid) && !exit_trace->startsolid)
			{
				end = exit_trace->endpos;
				return true;
			}

			continue;
		}

		if (!(exit_trace->fraction < 1.0 || exit_trace->allsolid || exit_trace->startsolid) || exit_trace->startsolid)
		{
			if (exit_trace->m_pEnt)
			{
				if (enter_trace->m_pEnt && enter_trace->m_pEnt == Interfaces::EntityList()->GetClientEntity(0))
					return true;
			}

			continue;
		}

		if (exit_trace->surface.flags >> 7 & 1 && !(enter_trace->surface.flags >> 7 & 1))
			continue;

		if (exit_trace->plane.normal.Dot(dir) <= 1.0f)
		{
			auto fraction = exit_trace->fraction * 4.0f;
			end = end - (dir * fraction);

			return true;
		}
	}

	return false;
}


bool IsArmored(C_BaseEntity* Entity, int ArmorValue, int Hitgroup)
{
	bool result = false;

	if (ArmorValue > 0)
	{
		switch (Hitgroup)
		{
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			result = true;
			break;
		case HITGROUP_HEAD:
			result = Entity->HasHelmet(); // DT_CSPlayer -> m_bHasHelmet
			break;
		}
	}

	return result;
}

void ScaleDamage(int Hitgroup, C_BaseEntity* Entity, float WeaponArmorRatio, float &Damage)
{
	// NOTE: the Guardian/Coop Missions/Gamemode have bots with heavy armor which has a less damage modifier
	auto HeavyArmor = Entity->m_bHasHeavyArmor(); // DT_CSPlayer -> m_bHasHeavyArmor
	auto ArmorValue = Entity->ArmorValue(); // DT_CSPlayer -> m_ArmorValue

	switch (Hitgroup)
	{
	case HITGROUP_HEAD:
		if (HeavyArmor)
			Damage = (Damage * 4.f) * 0.5f;
		else
			Damage *= 4.f;
		break;
	case HITGROUP_STOMACH:
		Damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		Damage *= 0.75f;
		break;
	}

	if (IsArmored(Entity, ArmorValue, Hitgroup))
	{
		float v47 = 1.f, ArmorBonusRatio = 0.5f, ArmorRatio = WeaponArmorRatio * 0.5f;

		if (HeavyArmor)
		{
			ArmorBonusRatio = 0.33f;
			ArmorRatio = (WeaponArmorRatio * 0.5f) * 0.5f;
			v47 = 0.33f;
		}

		auto NewDamage = Damage * ArmorRatio;

		if (HeavyArmor)
			NewDamage *= 0.85f;

		if (((Damage - (Damage * ArmorRatio)) * (v47 * ArmorBonusRatio)) > ArmorValue)
			NewDamage = Damage - (ArmorValue / ArmorBonusRatio);

		Damage = NewDamage;
	}
}


bool SimulateFireBullet( CBaseCombatWeapon *weapon, FireBulletData &data)
{
	C_BaseEntity *local = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	auto *wpn_data = weapon->GetCSWpnData();

	data.current_damage = static_cast<float>(wpn_data->damage);

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = wpn_data->range - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		UTIL_TraceLine(data.src, end, MASK_SHOT, local,  &data.enter_trace);
		UTIL_ClipTraceToPlayers( data.src, end + data.direction * 40.f, MASK_SHOT, &data.filter, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if ((data.enter_trace.hitgroup <= HITGROUP_RIGHTLEG)
			&& (data.enter_trace.hitgroup > HITGROUP_GENERIC)
			&& (local->GetTeamNum() != data.enter_trace.m_pEnt->GetTeamNum()
				|| g_Options.Ragebot.FriendlyFire))
		{
			data.trace_length += (float)(data.enter_trace.fraction * data.trace_length_remaining);
			data.current_damage *= (float)(pow(wpn_data->range_modifier, data.trace_length * 0.002));
			ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, wpn_data->armor_ratio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(wpn_data, data))
			break;
	}

	return false;
}

bool HandleBulletPenetration(CSWeaponInfo* weaponInfo, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = Interfaces::PhysProps()->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.penetrationmodifier;

	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= powf(weaponInfo->range_modifier, data.trace_length * 0.002f);

	if (data.trace_length > 3000.f || enter_surf_penetration_mod < 0.1f)
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	trace_t trace_exit;

	if (!TraceToExit1(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	surfacedata_t *exit_surface_data = Interfaces::PhysProps()->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.penetrationmodifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if ((data.enter_trace.contents & CONTENTS_GRATE) != 0 || enter_material == 89 || enter_material == 71)
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / weaponInfo->penetration) * 1.25f);
	float thickness = (trace_exit.endpos - data.enter_trace.endpos).Length();

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}

float GetDamage(const Vector &point, float *damage_given)
{
	C_BaseEntity *local = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
	CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle((HANDLE)local->GetActiveWeaponHandle());

	*damage_given = 0.f;
	Vector dst = point;
	FireBulletData data(local->GetEyePosition());
	data.filter.pSkip = local;

	QAngle angles = CalcAngle(data.src, dst);
	AngleVectors(angles, &data.direction);

	data.direction.NormalizeInPlace();

	if (!weapon)
		return 0.f;

	if (SimulateFireBullet( weapon, data)) {
		*damage_given = data.current_damage;
	}

	return *damage_given;
}

bool CanDamage( const Vector &point, float *damage_given)
{
	return GetDamage(point, damage_given) > .0f;
}

bool CanWallbang(int &dmg)
{

	auto *local = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
	CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle((HANDLE)local->GetActiveWeaponHandle());
	if (!local)
		return false;
	FireBulletData data = FireBulletData(local->GetEyePosition());
	data.filter = CTraceFilter();
	data.filter.pSkip = local;
	auto flRange = weapon->GetCSWpnData()->range;
	Vector EyeAng;
	Interfaces::Engine()->GetViewAngles(EyeAng);

	Vector dst, forward;

	AngleVectors(EyeAng, &forward);
	dst = data.src + (forward * 8000.f);

	Vector angles;
	CalcAngle(data.src, dst, angles);
	AngleVectors(angles, &data.direction);
	sanitize_angles(data.direction);

	if (!weapon)
		return false;

	data.penetrate_count = 1;

	CSWeaponInfo *weaponData = weapon->GetCSWpnData();

	if (!weaponData)
		return false;

	data.current_damage = (int)weaponData->damage;

	data.trace_length_remaining = weaponData->range;

	Vector end = data.src + data.direction * data.trace_length_remaining;
	UTIL_TraceLine(data.src, end, MASK_SHOT | CONTENTS_GRATE, local, &data.enter_trace);

	if (data.enter_trace.fraction == 1.f)
		return false;

	if (HandleBulletPenetration(weaponData, data))
	{
		dmg = data.current_damage;
		return true;
	}

	return false;
}