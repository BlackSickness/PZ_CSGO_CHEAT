#pragma once
#include "HookIncludes.h"
#include "Misc.h"
#include "MovementRecorder.h"
#include "RageBot.h"
#include "LegitBot.h"
#include "GrenadePrediction.h"
#include "LagComp.h"
#include "ESP.h"
#include "LocalInfo.h"

QAngle qLastTickAngles = QAngle(0.0f, 0.0f, 0.0f);
void __stdcall CHLCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
{
	hooks::create_move(Interfaces::Client(), sequence_number, input_sample_frametime, active);
	C_BaseEntity *local = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
	CInput::CUserCmd *cmd = Interfaces::Input()->GetUserCmd(0, sequence_number);
	LocalInfo* localinfo1;

	if (!cmd)
		return;

	static float next_time = 0;
	if (!Globals::ySwitch && local->IsAlive() && local->GetVelocity().Length2D() == 0)
	{

		float TickStuff = TICKS_TO_TIME(local->GetTickBase());
		Globals::shouldflip = false;
		bool moveFlip;
		bool checkmove;
		Globals::NextTime = next_time;

		if (next_time - TICKS_TO_TIME(local->GetTickBase()) > 1.1) next_time = 0;

		if (local->GetVelocity().Length2D() == 0)
		{
			if (TickStuff > next_time + 1.1f) // 1.1
			{
				next_time = TickStuff + TICKS_TO_TIME(1);
				Globals::shouldflip = true;
			}
		}

	}


	if (!cmd->command_number)
		return;

	CInput::CVerifiedUserCmd *verifiedCommands = *(CInput::CVerifiedUserCmd **)(reinterpret_cast< uint32_t >(Interfaces::Input()) + 0xF0);
	CInput::CVerifiedUserCmd *verified = &verifiedCommands[sequence_number % 150];
	if (!verified)
		return;

	if (!local)
		return;
	QAngle oldAngle = cmd->viewangles;
	float oldForward = cmd->forwardmove;
	float oldSideMove = cmd->sidemove;

	backtracking->legitBackTrack(cmd, local);
	backtracking->RageBackTrack(cmd, local);


	misc::instance().OnCreateMove(cmd, local , bSendPacket);
	if (GetAsyncKeyState(g_Options.Misc.iFreeCamKey) && g_Options.Misc.bFreeCam)
	{
		cmd->forwardmove = 0;
		cmd->sidemove = 0;
	}
	if (g_Options.Misc.AutoStrafe)
	misc::AutoStrafe(cmd, local, cmd->viewangles);
	misc::SlowMove(cmd, bSendPacket, local);
	misc::FakeWalk(cmd);
	misc::moonwalk(cmd);
	misc::Knifebot(cmd);
	legitbot::instance().OnCreateMove(cmd, local , bSendPacket);
	ragebot::instance().OnCreateMove(cmd, bSendPacket);
	CCSGrenadeHint::instance().Tick(cmd->buttons);

	if (!bSendPacket && g_Options.Ragebot.YawFake) qLastTickAngles = cmd->viewangles;
	else if (bSendPacket && !g_Options.Ragebot.YawFake) qLastTickAngles = cmd->viewangles;

	if (g_Options.Misc.antiuntrusted)
		if (!sanitize_angles(cmd->viewangles))
			return;

	movementfix(oldAngle, cmd);
	MovementRecorder(cmd);

	if (g_Options.Misc.antiuntrusted)
		clampMovement(cmd);

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();


	if (Globals::ySwitch) {
		Globals::FakeAngle = cmd->viewangles.y;
	}
	else {
		Globals::RealAngle = cmd->viewangles.y;
	}
}
#pragma warning(disable : 4409)
__declspec(naked) void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		PUSH	EBP
		MOV		EBP, ESP
		PUSH	EBX
		LEA		ECX, [ESP]
		PUSH	ECX
		PUSH	active
		PUSH	input_sample_frametime
		PUSH	sequence_number
		CALL	CHLCreateMove
		POP		EBX
		POP		EBP
		RETN	0xC
	}
}