#pragma once
#include "HookIncludes.h"
#include <intrin.h>  
using is_connected_t = bool(__stdcall*)(void*);

bool __stdcall hkIsConnected(void* ecx, void* edx)
{
    static auto ofunc = hooks::engine.get_original<is_connected_t>(27);

    void* returna = _ReturnAddress();
    static auto v1 = (uintptr_t)U::pattern_scan(GetModuleHandleW(L"client_panorama.dll"), "75 04 B0 01 5F") - 2;

    if ((uintptr_t)returna == v1) return true;
    return ofunc;
  
}

/*
PatternScan(GetModuleHandleA("client_panorama.dll"), "75 04 B0 01 5F") - 2;
*/

