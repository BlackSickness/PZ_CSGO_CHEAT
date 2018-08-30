#pragma once
#include "HookIncludes.h"




float __fastcall hkGetViewModelFOV(void* ecx, void* edx)
{
	float viewmodelFOV = hooks::original_get_fov(ecx);
	return g_Options.Misc.viewmodelChanger;
}

void __fastcall hkOverrideView(void* _this, void* _edx, CViewSetup* setup)
{
	C_BaseEntity *pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
	if (pLocal && Interfaces::Engine()->IsInGame())
	{
		if (!pLocal->IsScoped())
			setup->fov += g_Options.Misc.FOVChanger;

		static bool enabledtp = false;
		static bool check = false;
		if (GetAsyncKeyState(g_Options.Misc.TPKey))
		{
			if (!check)
				enabledtp = !enabledtp;
			check = true;
		}
		else
			check = false;

		if (g_Options.Misc.ThirdPerson && enabledtp && pLocal->IsAlive())
		{
			if (!Interfaces::Input()->m_fCameraInThirdPerson)
			{
				Interfaces::Input()->m_fCameraInThirdPerson = true;
			}
			//Interfaces::Input()->m_vecCameraOffset.z = g_Options.Visuals.tpdist;
		}
		else
		{
			Interfaces::Input()->m_fCameraInThirdPerson = false;
		}

		if (g_Options.Misc.bFreeCam)
		{
			static Vector newOrigin;

			int freecamkey = g_Options.Misc.iFreeCamKey;
			float freecamspeed = g_Options.Misc.flFreeCamSpeed;


			if (!GetAsyncKeyState(freecamkey))
			{
				newOrigin = setup->origin;
			}
			if (GetAsyncKeyState(freecamkey))
			{

				unsigned int fSpeed = freecamspeed; //5.f;
				if (GetAsyncKeyState(VK_CONTROL))
					fSpeed = fSpeed * 0.45;
				if (GetAsyncKeyState(VK_SHIFT))
					fSpeed = fSpeed * 1.65;
				if (GetAsyncKeyState(0x57)) // W		
				{
					newOrigin += setup->angles.Forward() * fSpeed;
				}
				if (GetAsyncKeyState(0x41)) // A		
				{
					newOrigin += setup->angles.Right() * fSpeed;
				}
				if (GetAsyncKeyState(0x44)) // D		
				{
					newOrigin -= setup->angles.Right() * fSpeed;
				}
				if (GetAsyncKeyState(0x53)) // S		
				{
					newOrigin -= setup->angles.Forward() * fSpeed;

				}
				if (GetAsyncKeyState(VK_SPACE)) // Space		
				{
					newOrigin += setup->angles.Up() * fSpeed;
				}
				setup->origin = newOrigin;
			}
		}
	}
	CCSGrenadeHint::instance().View(setup);
	hooks::original_override_view(_this, setup);
}