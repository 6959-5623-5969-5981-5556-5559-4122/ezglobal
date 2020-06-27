#pragma once
#include "HookIncludes.h"

#define MP5_WEAPON_MODEL_PATH "models/weapons/v_smg_mp5.mdl"

typedef MDLHandle_t(__thiscall* tFindMDL)(void*, const char*);
tFindMDL oFindMDL;

MDLHandle_t __fastcall hkFindMDL(void* ecx, void* edx, const char* pMDLRelativePath)
{
	if (&g_Options.Visuals.Box)
	{
		// Junk
		// [JUNK_CODE]
		// JUnk
		return oFindMDL(ecx, pMDLRelativePath);
	}
}

