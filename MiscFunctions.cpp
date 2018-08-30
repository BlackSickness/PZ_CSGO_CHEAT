

#include "MiscFunctions.h"
#include "Autowall.h"
#include "Render.h"


char shit[16];
trace_t Trace;
char shit2[16];
C_BaseEntity* entCopy;


bool MiscFunctions::IsVisible(C_BaseEntity* pLocal, C_BaseEntity* pEntity)
{
	entCopy = pEntity;
	if (!pLocal->IsValid())
		return false;

	Vector vSrcOrigin = pLocal->GetEyePosition();

	if (vSrcOrigin.IsZero() || !vSrcOrigin.IsValid())
		return false;

	BYTE bHitBoxCheckVisible[19] = {
		Head,
		LeftHand,
		RightHand,
		LeftFoot,
		RightFoot,
		Chest,
	};

	CTraceFilter filter;

	filter.pSkip = pLocal;

	for (int nHit = 0; nHit < 19; nHit++)
	{
		Vector vHitBox = GetHitboxPosition(pEntity,bHitBoxCheckVisible[nHit]);

		if (vHitBox.IsZero() || !vHitBox.IsValid())
			continue;

		trace_t tr;
		Ray_t ray;

		ray.Init(vSrcOrigin, vHitBox);

		Interfaces::EngineTrace()->TraceRay(ray, PlayerVisibleMask, &filter, &tr);

		if (tr.m_pEnt == (C_BaseEntity*)entCopy && !tr.allsolid)
			return true;
	}

	return false;

}


bool MiscFunctions::IsKnife(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity* weaponEnt = (C_BaseEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CKnife || pWeaponClass->m_ClassID == (int)ClassID::CC4 || pWeaponClass->m_ClassID == (int)ClassID::CKnifeGG)
		return true;
	else
		return false;
}

bool MiscFunctions::IsBomb(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity* weaponEnt = (C_BaseEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CC4)
		return true;
	else
		return false;
}

bool MiscFunctions::IsPistol(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity* weaponEnt = (C_BaseEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CWeaponElite || pWeaponClass->m_ClassID == (int)ClassID::CWeaponFiveSeven || pWeaponClass->m_ClassID == (int)ClassID::CWeaponGlock || pWeaponClass->m_ClassID == (int)ClassID::CWeaponHKP2000 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponP250 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponP228 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponTec9 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponUSP)
		return true;
	else
		return false;
}

bool MiscFunctions::IsRevolver(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity* weaponEnt = (C_BaseEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CDEagle)
		return true;
	else
		return false;
}

bool MiscFunctions::IsSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity* weaponEnt = (C_BaseEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CWeaponAWP || pWeaponClass->m_ClassID == (int)ClassID::CWeaponSSG08 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponSCAR20 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponG3SG1)
		return true;
	else
		return false;
}

bool MiscFunctions::IsRifle(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity* weaponEnt = (C_BaseEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CWeaponFamas || pWeaponClass->m_ClassID == (int)ClassID::CAK47 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponM4A1 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponGalil || pWeaponClass->m_ClassID == (int)ClassID::CWeaponGalilAR || pWeaponClass->m_ClassID == (int)ClassID::CWeaponAug || pWeaponClass->m_ClassID == (int)ClassID::CWeaponSG556)
		return true;
	else
		return false;
}

bool MiscFunctions::IsSmg(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity* weaponEnt = (C_BaseEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CWeaponMP7 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponMP9 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponUMP45 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponP90 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponBizon || pWeaponClass->m_ClassID == (int)ClassID::CWeaponMAC10)
		return true;
	else
		return false;
}

bool MiscFunctions::IsHeavy(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity* weaponEnt = (C_BaseEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CWeaponNegev || pWeaponClass->m_ClassID == (int)ClassID::CWeaponM249 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponXM1014 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponNOVA || pWeaponClass->m_ClassID == (int)ClassID::CWeaponMag7 || pWeaponClass->m_ClassID == (int)ClassID::CWeaponSawedoff)
		return true;
	else
		return false;
}

bool MiscFunctions::IsGrenade(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity* weaponEnt = (C_BaseEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)ClassID::CDecoyGrenade || pWeaponClass->m_ClassID == (int)ClassID::CHEGrenade || pWeaponClass->m_ClassID == (int)ClassID::CIncendiaryGrenade || pWeaponClass->m_ClassID == (int)ClassID::CMolotovGrenade || pWeaponClass->m_ClassID == (int)ClassID::CSensorGrenade || pWeaponClass->m_ClassID == (int)ClassID::CSmokeGrenade || pWeaponClass->m_ClassID == (int)ClassID::CFlashbang)
		return true;
	else
		return false;
}


void SayInChat(const char *text)
{
	char buffer[250];
	sprintf_s(buffer, "say \"%s\"", text);
	Interfaces::Engine()->ClientCmd_Unrestricted2(buffer);
}

float GenerateRandomFloat(float Min, float Max)
{
	float randomized = (float)rand() / (float)RAND_MAX;
	return Min + randomized * (Max - Min);
}





Vector GetHitboxPosition(C_BaseEntity* pEntity, int Hitbox)
{
	matrix3x4_t matrix[MAXSTUDIOBONES];
	Vector vRet, vMin, vMax;
	vRet = Vector(0, 0, 0);
	studiohdr_t* hdr = Interfaces::ModelInfo()->GetStudiomodel(pEntity->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
	mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);
	if (!hitbox || !pEntity->IsValid())
		return vRet;
	if (!pEntity->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, pEntity->GetSimulationTime()))
		return vRet;
	if (!hitbox->bone || !hitbox->bbmin.IsValid() || !hitbox->bbmax.IsValid())
		return vRet;

	VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
	VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
	vRet = (vMin + vMax) * 0.5f;
	return vRet;
}

Vector GetHitboxPositionFromMatrix(C_BaseEntity* pEntity, matrix3x4_t matrix[128], int Hitbox)
{

    studiohdr_t* hdr = Interfaces::ModelInfo()->GetStudiomodel(pEntity->GetModel());
    mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

    mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

    if (!hitbox)
        return Vector(0, 0, 0);

    Vector vMin, vMax, vCenter, sCenter;
    VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
    VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
    vCenter = (vMin + vMax) *0.5f;
    return vCenter;
}
