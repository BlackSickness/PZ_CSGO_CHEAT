#pragma once
#include "HookIncludes.h"
#include "SkinChanger.h"
typedef bool(__thiscall *FireEventClientSideFn)(PVOID, IGameEvent*);
FireEventClientSideFn oFireEventClientSide;
bool __fastcall hkFireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event);

int Kills2 = 0;
int Kills = 0;
bool RoundInfo = false;
size_t Delay = 0;



bool __fastcall hkFireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event) {
	if (!Event)
		return oFireEventClientSide(ECX, Event);

	const char* szEventName = Event->GetName();

	if (!strcmp(szEventName, "player_death"))
	{
		int nUserID = Event->GetInt("attacker");

		if (nUserID && (Interfaces::Engine()->GetPlayerForUserID(nUserID) == Interfaces::Engine()->GetLocalPlayer()))
		{
			const char* szWeapon = Event->GetString("weapon");

			/*for (auto ReplacementIcon : killIcons) {
				// Search for a valid replacement.
				if (!strcmp(szWeapon, ReplacementIcon.first)) {
					// Replace with user defined value.
					Event->SetString("weapon", ReplacementIcon.second);
				}
			}*/
		}
	}

	return oFireEventClientSide(ECX, Event);
}

