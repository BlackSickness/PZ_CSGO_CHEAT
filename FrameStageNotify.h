#pragma once
#include "SkinChanger.h"
#include "Resolver.h"
#include "Resolver2.h"
#include "Resolver3.h"
#include "HookIncludes.h"
#include "GloveChanger.h"
#include "LagComp.h"
#include "LBYBreaking.h"



void  __stdcall hkFrameStageNotify(ClientFrameStage_t curStage)
{
	if (Interfaces::Engine()->IsConnected() && Interfaces::Engine()->IsInGame())
	{
		if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
			if (g_Options.Legitbot.Aimbot.ALegitAA)
			{
				Resolverr1();
			}
			if (g_Options.Ragebot.Resolver)
			{
				Resolverr1();
			}
			
			//ResolverSetup::instance().FSN(stage);
			if (g_Options.Ragebot.LBYCorrection2)
			{
				LbyCorrect();
			}
			Resolver3();

			if (g_Options.Skinchanger.Enabled)
			{
				GloveChanger();
				SkinChanger();
				//ApplyStickers();

			}
			if (g_Options.Ragebot.NextLBYUpdate)
			{
				LBYMod::NextLBYUpdate();
			}
			if (g_Options.Ragebot.NextMovingLBYUpdate)
			{
				LBYMod::NextMovingLBYUpdate();
			}
			backtracking->Update(Interfaces::Globals()->tickcount);
		}

		if (curStage == FRAME_RENDER_START)
		{
			auto pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
			auto dwDeadFlag = NetVarManager->GetOffset("DT_CSPlayer", "deadflag"); // int
			if (pLocal)
			{
				*(int*)((uintptr_t)pLocal + 0xA30) = Interfaces::Globals()->framecount; //we'll skip occlusion checks now
				*(int*)((uintptr_t)pLocal + 0xA28) = 0; //clear occlusion flags

				if (pLocal->IsAlive() && Interfaces::Input()->m_fCameraInThirdPerson) { *reinterpret_cast<Vector*>(reinterpret_cast<DWORD>(pLocal) + dwDeadFlag + 4) = qLastTickAngles; }
			}
		}

	}
	hooks::fsn(curStage);
}
