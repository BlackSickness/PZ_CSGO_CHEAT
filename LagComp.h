#pragma once
#include "SDK.h"

struct lbyRecords
{
	int tick_count;
	float lby;
	Vector headPosition;
};
struct backtrackData
{
	float simtime;
	Vector hitboxPos;
};

class BackTrack
{
	int latest_tick;
	bool IsTickValid(int tick);
	void UpdateRecord(int i);
public:
	lbyRecords records[64];
	bool RunLBYBackTrack(int i, CInput::CUserCmd* cmd, Vector& aimPoint);
	void Update(int tick_count);
	void legitBackTrack(CInput::CUserCmd* cmd, C_BaseEntity* pLocal);
	void RageBackTrack(CInput::CUserCmd* cmd, C_BaseEntity* pLocal);
};

extern backtrackData headPositions[64][12];

extern BackTrack* backtracking;






