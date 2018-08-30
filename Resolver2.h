#pragma once
#include "HookIncludes.h"
#include "singleton.hpp"

class ResolverSetup
	: public singleton<ResolverSetup>

{
public:
	bool didhit;
	void FSN( ClientFrameStage_t stage); //used to get the fresh THINGS and the resolve
	void Auto();
	void CM(C_BaseEntity* pEntity); //cause this is slower than FSN so we are going to get older info and not updated at all ez to compare between etc.
	void OverrideResolver(C_BaseEntity * pEntity);
	void Resolve(C_BaseEntity* pEntity); //resolve
	void StoreThings(C_BaseEntity* pEntity);
	void StoreThings2(C_BaseEntity* pEntity);
	std::map<int, QAngle>StoredAngles; //y and x lol (stored)
	std::map<int, QAngle>NewANgles; //y and x lol (new)
	std::map<int, float>storedlby;
	std::map<int, float>newlby;
	std::map<int, float>storeddelta;
	std::map<int, float>newdelta;
	std::map<int, float>finaldelta;
	std::map<int, float>storedlbydelta;
	std::map<int, float>newlbydelta;
	std::map<int, float>finallbydelta;
	float newsimtime;
	float storedsimtime;
	bool lbyupdated;
	float storedlbyFGE;
	float storedanglesFGE;
	float storedsimtimeFGE;
	bool didhitHS;
	void StoreFGE(C_BaseEntity* pEntity);

};
namespace Resolver
{
	extern bool didhitHS;
}
namespace Globals
{
	extern CUserCmd* UserCmd;
	extern C_BaseEntity* Target;
	extern int Shots;
	extern bool change;
	extern int TargetID;
	extern bool didhitHS;
	extern int missedshots;
	extern std::map<int, QAngle> storedshit;
}