#pragma once
/*
#include "HookIncludes.h"
#include <intrin.h>  
using is_connected_t = bool(__stdcall*)(void*);

bool __stdcall hkIsConnected()
{
    static auto ofunc = hooks::engine.get_original<is_connected_t>(27);

    void* returna = _ReturnAddress();
    static auto v1 = (uintptr_t)CSX::Memory::NewPatternScan(GetModuleHandleW(L"client_panorama.dll"), "84 C0 75 04 B0 01 5F");

    if ((uintptr_t)returna == v1) return false;
    return ofunc;
}
*/