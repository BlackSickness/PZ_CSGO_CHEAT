#pragma once
#include "HookIncludes.h"
#include "Render.h"
#include "Variables.h"
struct Recording_t
{
	QAngle m_ViewAngles;
	Vector m_RecordedWaypoints;
	float m_ForwardMove;
	float m_SideMove;
	float m_UpMove;
	int m_Buttons;
	bool m_bShouldJump = false;
	bool m_IsInitialized = false;
};

int m_iRecordFrame = 0;
int m_iReplayFrame = 0;

Recording_t m_RecordWaypoints[100000];
#define VMD_PI 3.14159265358979323846
#define DEGTORAD(a)     (a*VMD_PI/180.0)
bool m_bIsRecording;
bool m_bIsReplaying;
int m_iOldTickCount;

void RotateMovement(CInput::CUserCmd* pCmd, float rotation)
{
	rotation = (float)(DEGTORAD(rotation));

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
	float new_sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);

	pCmd->forwardmove = new_forwardmove;
	pCmd->sidemove = new_sidemove;
}

static void MovementRecorder(CInput::CUserCmd *cmd)
{
	C_BaseEntity *pLocalPlayer = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	if (g_Options.Misc.isRecording && !m_bIsRecording && !(g_Options.Misc.isReplaying))
	{
		m_bIsRecording = true;

		memset(m_RecordWaypoints, 0, sizeof(m_RecordWaypoints));
	}
	else if (!(g_Options.Misc.isRecording && m_bIsRecording))
	{
		m_bIsRecording = false;

		m_iRecordFrame = 0;
	}

	if (g_Options.Misc.isReplaying && !m_bIsReplaying && !g_Options.Misc.isRecording)
	{
		m_bIsReplaying = true;
	}
	else if (!g_Options.Misc.isReplaying)
	{
		m_bIsReplaying = false;

		m_iReplayFrame = 0;
	}

	if (m_bIsRecording)
	{


		m_RecordWaypoints[m_iRecordFrame].m_RecordedWaypoints = pLocalPlayer->GetOrigin();
		m_RecordWaypoints[m_iRecordFrame].m_ForwardMove = cmd->forwardmove;
		m_RecordWaypoints[m_iRecordFrame].m_SideMove = cmd->sidemove;
		m_RecordWaypoints[m_iRecordFrame].m_UpMove = cmd->upmove;
		m_RecordWaypoints[m_iRecordFrame].m_ViewAngles = cmd->viewangles;
		m_RecordWaypoints[m_iRecordFrame].m_Buttons = cmd->buttons;
		m_RecordWaypoints[m_iRecordFrame].m_IsInitialized = true;

		//if (cmd->buttons & IN_JUMP)
		//m_RecordWaypoints[m_iRecordFrame].m_bShouldJump = true;

		m_iRecordFrame++;

		m_iOldTickCount = cmd->tick_count;
	}


	if (m_bIsReplaying)
	{
		if (m_iReplayFrame == 0)
		{
			float Diff;
			AverageDifference(pLocalPlayer->GetOrigin(), m_RecordWaypoints[0].m_RecordedWaypoints, Diff);

			if (Diff > 1.f)
			{
				Vector Dest = CalcAngle(pLocalPlayer->GetEyePosition(), m_RecordWaypoints[0].m_RecordedWaypoints);


				if (Diff < 30.f)
					cmd->forwardmove = Diff;
				else
					cmd->forwardmove = 450.f;

				float flYawDelta = cmd->viewangles.y - Dest.y;
				RotateMovement(cmd, flYawDelta);
			}
			else
			{
				m_iReplayFrame++;
			}
		}
		else
		{
			//test
			Interfaces::Engine()->SetViewAngles(m_RecordWaypoints[m_iReplayFrame].m_ViewAngles);
			cmd->sidemove = m_RecordWaypoints[m_iReplayFrame].m_SideMove;
			cmd->forwardmove = m_RecordWaypoints[m_iReplayFrame].m_ForwardMove;
			cmd->upmove = m_RecordWaypoints[m_iReplayFrame].m_UpMove;
			cmd->buttons = m_RecordWaypoints[m_iReplayFrame].m_Buttons;


			m_iReplayFrame++;

			if (!m_RecordWaypoints[m_iReplayFrame].m_IsInitialized)
				m_iReplayFrame = 0;
		}

		m_iOldTickCount = cmd->tick_count;
	}
}

void DrawMovementRecorder()
{
	for (int i = 0; i < 99999; i++)
	{
		if (!m_RecordWaypoints[i].m_IsInitialized)
			break;

		if (i - 1 <= 0)
			continue;
		//test
		Color col = i > m_iReplayFrame ? Color(255, 0, 0) : Color(0, 255, 0);

		Vector m_Point, m_Point1;
		int sex = i % 4;
		switch (sex)
		{
			case 1:
			{
				if (g_Render->WorldToScreen(Vector(m_RecordWaypoints[i].m_RecordedWaypoints.x, m_RecordWaypoints[i].m_RecordedWaypoints.y, m_RecordWaypoints[i].m_RecordedWaypoints.z), m_Point))
				{
					if (g_Render->WorldToScreen(Vector(m_RecordWaypoints[i].m_RecordedWaypoints.x, m_RecordWaypoints[i].m_RecordedWaypoints.y, m_RecordWaypoints[i].m_RecordedWaypoints.z + 4), m_Point1))
					{
						g_Render->Line(m_Point.x, m_Point.y, m_Point1.x, m_Point1.y, Color(255, 255, 255));
					}
	
				}
				break;
			}
			default:
			{
				break;
			}
		}
	}
}