#pragma once
#include "HookIncludes.h"
#include <intrin.h>  




auto dwCAM_Think = CSX::Memory::NewPatternScan(GetModuleHandleW(L"client_panorama.dll"), "85 C0 75 30 38 86");

bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx)
{
	if (!hooks::original_svc_get_bool)
		return false;

	if (reinterpret_cast<DWORD>(_ReturnAddress()) == reinterpret_cast<DWORD>(dwCAM_Think))
		return true;
	return hooks::original_svc_get_bool(pConVar);
}