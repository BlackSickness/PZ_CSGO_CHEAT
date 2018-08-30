#pragma once
#include "HookIncludes.h"

void __stdcall hkPlaySound(const char* szFileName)
{
	//Call original PlaySound
    hooks::original_play_sound(Interfaces::Surface(), szFileName);
	if (g_Options.Misc.AutoAccept)
	{ 
	if (Interfaces::Engine()->IsInGame()) return;

	if (strstr(szFileName, "UI/competitive_accept_beep.wav")) 
	{

        static auto CServerConfirmedReservationCheckCallback__IsReady
            = (void(__cdecl*)(void))CSX::Memory::NewPatternScan(
                GetModuleHandleW(L"client_panorama.dll"),
                "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE"
            );
        CServerConfirmedReservationCheckCallback__IsReady();
	}
	}
}
