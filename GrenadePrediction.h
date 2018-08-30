#pragma once
#include "HookIncludes.h"
#include "singleton.hpp"


enum ACT
{
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP,
};

class CCSGrenadeHint
	: public singleton<CCSGrenadeHint>
{
private:
	std::vector<Vector> path;
	std::vector<std::pair<Vector, QAngle>> OtherCollisions;

	int type = 0;
	int act = 0;

	void Setup(Vector & vecSrc, Vector & vecThrow, Vector viewangles);

	void Simulate(CViewSetup * setup);

	int Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval);
	bool CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval);

	void TraceHull(Vector& src, Vector& end, trace_t& tr);
	void AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground);
	void PushEntity(Vector& src, const Vector& move, trace_t& tr);
	void ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval);
	int PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce);

	Color TracerColor = Color(int(g_Options.Color.color_grenadeprediction[0] * 255.f), int(g_Options.Color.color_grenadeprediction[1] * 255.f), int(g_Options.Color.color_grenadeprediction[2] * 255.f));
	Color boxColor = Color(int(g_Options.Color.GrenadeCollision[0] * 255.f), int(g_Options.Color.GrenadeCollision[1] * 255.f), int(g_Options.Color.GrenadeCollision[2] * 255.f));
public:
	void Tick(int buttons);
	void View(CViewSetup* setup);
	void Paint();
};

