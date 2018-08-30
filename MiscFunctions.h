
#pragma once

#include "SDK.h"

namespace MiscFunctions
{
	bool IsVisible(C_BaseEntity* pLocal, C_BaseEntity* pEntity);
	bool IsKnife(void* weapon);
	bool IsPistol(void* weapon);
	bool IsSniper(void* weapon);
	bool IsGrenade(void* weapon);
	bool IsRifle(void* weapon);
	bool IsSmg(void* weapon);
	bool IsHeavy(void* weapon);
	bool IsRevolver(void* weapon);
	bool IsBomb(void * weapon);

};


void SayInChat(const char *text);

Vector GetHitboxPosition(C_BaseEntity* pEntity, int Hitbox);
Vector GetHitboxPositionFromMatrix(C_BaseEntity* pEntity, matrix3x4_t matrix[128], int Hitbox);

void ForceUpdate();


inline void clampMovement(CInput::CUserCmd* cmd)
{
	if (cmd->forwardmove >= 450) cmd->forwardmove = 450;
	if (cmd->sidemove >= 450) cmd->sidemove = 450;
	if (cmd->upmove >= 450) cmd->upmove = 450;
	if (cmd->forwardmove <= -450) cmd->forwardmove = -450;
	if (cmd->sidemove <= -450) cmd->sidemove = -450;
	if (cmd->upmove <= -450) cmd->upmove = -450;
}
#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define RADPI 57.295779513082f

inline float sseSqrt(float x)
{
	float root = 0.0f;

	__asm
	{
		sqrtss xmm0, x
		movss root, xmm0
	}

	return root;
}
#define square( x ) ( x * x )
inline void vectorAnglesVec(Vector forward, Vector& angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
		angles[1] = 0.0f;
	}
	else
	{
		float len2d = sseSqrt(square(forward[0]) + square(forward[1]));

		angles[0] = RAD2DEG(atan2f(-forward[2], len2d));
		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));

		if (angles[0] < 0.0f)
			angles[0] += 360.0f;
		if (angles[1] < 0.0f)
			angles[1] += 360.0f;
	}
}

inline static Vector Normalizekek(Vector& angs)
{
	while (angs.y < -180.0f)
		angs.y += 360.0f;
	while (angs.y > 180.0f)
		angs.y -= 360.0f;
	if (angs.x > 89.0f)
		angs.x = 89.0f;
	if (angs.x < -89.0f)
		angs.x = -89.0f;
	angs.z = 0;
	return angs;
}

inline void movementfix(QAngle realvec, CInput::CUserCmd* cmd)
{
	Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
	Vector vMove2;
	vectorAnglesVec(vMove, vMove2);
	Normalizekek(vMove2);
	flYaw = DEG2RAD(cmd->viewangles.y - realvec.y + vMove2.y);
	cmd->forwardmove = cos(flYaw) * flSpeed;
	cmd->sidemove = sin(flYaw) * flSpeed;
	if (90 < abs(cmd->viewangles.x) && 180 > abs(cmd->viewangles.x))
		cmd->forwardmove *= -1;
}