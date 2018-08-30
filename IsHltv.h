#pragma once
/*
#include "HookIncludes.h"
#include <intrin.h>  
using is_hltv_t = bool(__thiscall*)(void*);

bool __fastcall hkIsHltv(void* ecx, void* edx)
{
    static auto ofunc = hooks::engine.get_original<is_hltv_t>(93);
    uintptr_t pvs_entity;
    __asm
    {
        mov pvs_entity, edi
    }


    void* returna = _ReturnAddress();
    static auto v1 = (DWORD)CSX::Memory::NewPatternScan(GetModuleHandleW(L"client_panorama.dll"), "84 C0 0F 85 ? ? ? ? A1 ? ? ? ? 8B B7");
    if (reinterpret_cast<DWORD>(returna) == (v1))
    {
        if (pvs_entity)
        {
            *(int*)(pvs_entity + 0xA24) = -1;
            *(int*)(pvs_entity + 0xA2C) = *(int *)(pvs_entity + 0xA28);
            *(int*)(pvs_entity + 0xA28) = 0;
            return true;
        }
    }
    return ofunc(ecx);
}
*/