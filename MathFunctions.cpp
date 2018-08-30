

#include "Vector.hpp"
#include "MathFunctions.h"
#include "CommonIncludes.h"
#include "CSX.h"
#include "CODEX.h"

enum MathThings {
	PITCH = 0,	// up / down
	YAW,		// left / right
	ROLL		// fall over
};

void AngleNormalize(QAngle& angle)
{
	while (angle.x > 89.0f) {
		angle.x -= 180.f;
	}
	while (angle.x < -89.0f) {
		angle.x += 180.f;
	}
	while (angle.y > 180.f) {
		angle.y -= 360.f;
	}
	while (angle.y < -180.f) {
		angle.y += 360.f;
	}
}

void normalize_angles(QAngle& angles)
{
	for (auto i = 0; i < 3; i++) {
		while (angles[i] < -180.0f) angles[i] += 360.0f;
		while (angles[i] >  180.0f) angles[i] -= 360.0f;
	}
}


void ClampAngles(QAngle &angles) {
	if (angles.y > 180.0f)
		angles.y = 180.0f;
	else if (angles.y < -180.0f)
		angles.y = -180.0f;

	if (angles.x > 89.0f)
		angles.x = 89.0f;
	else if (angles.x < -89.0f)
		angles.x = -89.0f;

	angles.z = 0;
}


void SinCos(float radians, float *sine, float *cosine) {
	*sine = sin(radians);
	*cosine = cos(radians);
}

Vector AngleVector(QAngle meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(3.141592654f));
	auto cy = cos(meme.y / 180.f * static_cast<float>(3.141592654f));

	auto sp = sin(meme.x / 180.f * static_cast<float>(3.141592654f));
	auto cp = cos(meme.x / 180.f* static_cast<float>(3.141592654f));

	return Vector(cp*cy, cp*sy, -sp);
}

void AngleVectors(const QAngle &angles, Vector *forward) {
	float sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[MathThings::YAW]), &sy, &cy);
	SinCos(DEG2RAD(angles[MathThings::PITCH]), &sp, &cp);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

void VectorAngles1(const Vector& forward, QAngle &angles) {
	float	tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}
void VectorAngles(const Vector& forward, QAngle &angles) {
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
		angles[1] = 0.0f;
	}
	else
	{
		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
		angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

		if (angles[1] > 90) angles[1] -= 180;
		else if (angles[1] < 90) angles[1] += 180;
		else if (angles[1] == 90) angles[1] = 0;
	}

	angles[2] = 0.0f;
}

void AngleVectors(const QAngle &angles, Vector *forward, Vector *right, Vector *up) {
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[MathThings::YAW]), &sy, &cy);
	SinCos(DEG2RAD(angles[MathThings::PITCH]), &sp, &cp);
	SinCos(DEG2RAD(angles[MathThings::ROLL]), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr * -sy);
		up->y = (cr*sp*sy + -sr * cy);
		up->z = cr * cp;
	}
}


void Normalize(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}


void CalcAngle(Vector src, Vector dst, Vector &angles)
{
	Vector delta = src - dst;
	double hyp = delta.Length2D();
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (vec_t)(atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.00;

	if (delta.x >= 0.0)
		angles.y += 180.0f;
}



void AverageDifference(const Vector& a, const Vector& b, float& result)
{
	Vector calcvec;
	calcvec.x = abs(a.x - b.x);
	calcvec.y = abs(a.y - b.y);
	calcvec.z = abs(a.y - b.y);

	result = (calcvec.x + calcvec.y + calcvec.z) / 3.f;
}

QAngle CalcAngle(Vector src, Vector dst) {
	QAngle angles;
	Vector delta = src - dst;
	VectorAngles(delta, angles);
	AngleNormalize(delta);
	return angles;
}

bool LineGoesThroughSmoke(Vector vStartPos, Vector vEndPos)
{
	typedef bool(__cdecl* _LineGoesThroughSmoke) (Vector, Vector);

	static _LineGoesThroughSmoke LineGoesThroughSmokeFn = 0;
	static bool SearchFunction = false;

	if (!SearchFunction)
	{
		DWORD dwFunctionAddress = CSX::Memory::FindPattern(CLIENT_DLL, SMOK_PATTERN, "xxxxxxxx????xxx", 0);

		if (dwFunctionAddress)
		{
			LineGoesThroughSmokeFn = (_LineGoesThroughSmoke)dwFunctionAddress;
			SearchFunction = true;
		}
	}

	if (LineGoesThroughSmokeFn && SearchFunction)
	{
		return LineGoesThroughSmokeFn(vStartPos, vEndPos);
	}

	return false;
}
float DistancePointToLine(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

bool sanitize_angles(QAngle &angles)
{
	QAngle temp = angles;
	normalize_angles(temp);
	ClampAngles(temp);

	if (!isfinite(temp.x) ||
		!isfinite(temp.y) ||
		!isfinite(temp.z))
		return false;

	angles = temp;

	return true;
}

void SmoothAngles(Vector MyViewAngles, Vector AimAngles, Vector &OutAngles, float Smoothing)
{
	OutAngles = AimAngles - MyViewAngles;

	AngleNormalize(OutAngles);

	OutAngles.x = OutAngles.x / Smoothing + MyViewAngles.x;
	OutAngles.y = OutAngles.y / Smoothing + MyViewAngles.y;

	AngleNormalize(OutAngles);
}




bool CTimer::delay(DWORD dwMsec)
{
	if (!bEnable)
		return true;

	if (!dwTime)
		dwTime = GetTickCount64();

	if (dwTime + dwMsec < GetTickCount64())
	{
		//dwTime = 0;
		bEnable = false;
		return true;
	}
	else
	{
		return false;
	}
}

void CTimer::reset()
{
	dwTime = 0;
	bEnable = true;
}

void CTimer::stop()
{
	bEnable = false;
}