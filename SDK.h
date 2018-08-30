#pragma once
#include "Entities.h"
#include "CSX.h"
#include "BaseClient.h"
#include "EngineClient.h"
#include "MiscClasses.h"
#include "Surface.h"
#include "Materials.h"
#include "NetVarManager.h"
#include "Vector.hpp"
#include "Vector2D.h"
#include "bspflags.h"
#include "Interfaces.h"
#include "MiscFunctions.h"
#include "MathFunctions.h"
#include "Variables.h"
#include "Global.h"


#pragma warning(disable : 4020)
#define M_PI 3.14159265358979323846
inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}

namespace Globals
{
	extern int TargetID;
	extern int Shots;
	extern int missedshots;
	extern float RealAngle;
	extern float FakeAngle;
	extern Vector AimPoint;
	extern bool shouldflip;
	extern bool ySwitch;
	extern float NextTime;
	extern int resolvemode;
	extern float fakeAngle;
	extern float OldSimulationTime[65];
	extern bool error;
}

inline void RandomSeed(int seed)
{
	static auto fn = (decltype(&RandomSeed))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed"));

	return fn(seed);
}

#define XM_2PI              6.283185307f
#include <deque>

