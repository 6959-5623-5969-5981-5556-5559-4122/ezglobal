#pragma once
#include "HookIncludes.h"
typedef void(__thiscall* dme_t)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);

float flColor[3];
float flVColor[3];

IMaterial* GetCurrentMaterial(int mati)
{
	static IMaterial* mat;

	switch (mati)
	{
	case 3:
		mat = g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", nullptr);
		break;
	case 4:
		mat = g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gold", nullptr);
		break;
	case 5:
		mat = g_MaterialSystem->FindMaterial("models/player/ct_fbi/ct_fbi_glass", nullptr);
		break;
	case 6:
		mat = g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", nullptr);
		break;
	case 7:
		mat = g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", nullptr);
		break;
	default:
		mat = g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", nullptr);
		return mat;
		break;
	}
	return mat;
}

std::vector<const char*> vistasmoke_mats =
{
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
};


IMaterial* GetCurrentZMaterial(int mati)
{
	static IMaterial* zmat;

	switch (mati)
	{
	case 3:
		zmat = g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 4:
		zmat = g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gold", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 5:
		zmat = g_MaterialSystem->FindMaterial("models/player/ct_fbi/ct_fbi_glass", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 6:
		zmat = g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	case 7:
		zmat = g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.XQZ);
		return zmat;
		break;
	default:
		zmat = g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", nullptr);
		return zmat;
		break;
	}
}

void __fastcall hkDrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	static bool DontDraw = false;
	static auto ofunc = hooks::modelrender.get_original<dme_t>(21);

	IMaterial* zmat = GetCurrentZMaterial(g_Options.Visuals.matierial);
	IMaterial* mat = GetCurrentMaterial(g_Options.Visuals.matierial);

	IMaterial* hmat = GetCurrentMaterial(g_Options.Visuals.matierial);
	IMaterial* wmat = GetCurrentMaterial(g_Options.Visuals.matierial);

	static IMaterial* norm = CreateMaterial(false, true, false);
	static IMaterial* flat = CreateMaterial(false, false, false);
	static IMaterial* wire = CreateMaterial(false, false, true);

	static IMaterial* znorm = CreateMaterial(true, true, false);
	static IMaterial* zflat = CreateMaterial(true, false, false);
	static IMaterial* zwire = CreateMaterial(true, false, true);

	static IMaterial* Outline = CreateMaterial(true, false, true);
	DontDraw = false;

	const char * ModelName = g_ModelInfo->GetModelName((model_t*)pInfo.pModel);
	C_BaseEntity* pModelEntity = (C_BaseEntity*)g_EntityList->GetClientEntity(pInfo.entity_index);
	C_BaseEntity* pLocal = (C_BaseEntity*)g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	if (ModelName && strstr(ModelName, "sleeve"))
	{

		auto meme = zmat = g_MaterialSystem->FindMaterial(ModelName, TEXTURE_GROUP_MODEL);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		g_ModelRender->ForcedMaterialOverride(meme);
	}


	if (ModelName && strstr(ModelName, "models/player"))
	{
		if (pModelEntity && pLocal)
		{
			if ((g_Options.Visuals.takimchams || pModelEntity->GetTeamNum() != pLocal->GetTeamNum()) && g_Options.Visuals.Chams)
			{
				pModelEntity = g_EntityList->GetClientEntity(pInfo.entity_index);

				if (pModelEntity->GetTeamNum() != pLocal->GetTeamNum())
				{
					flColor[0] = g_Options.Colors.dusmanchamsgozuk[0];
					flColor[1] = g_Options.Colors.dusmanchamsgozuk[1];
					flColor[2] = g_Options.Colors.dusmanchamsgozuk[2];
					flVColor[0] = g_Options.Colors.dusmantakimchams[0];
					flVColor[1] = g_Options.Colors.dusmantakimchams[1];
					flVColor[2] = g_Options.Colors.dusmantakimchams[2];
				}
				if (pModelEntity->GetTeamNum() == pLocal->GetTeamNum())
				{
					flColor[0] = g_Options.Colors.takimchamsgozuk[0];
					flColor[1] = g_Options.Colors.takimchamsgozuk[1];
					flColor[2] = g_Options.Colors.takimchamsgozuk[2];
					flVColor[0] = g_Options.Colors.takimchamsgozuk[0];
					flVColor[1] = g_Options.Colors.takimchamsgozuk[1];
					flVColor[2] = g_Options.Colors.takimchamsgozuk[2];
				}
				if (pModelEntity)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
					{
						if (g_Options.Visuals.matierial == 0)
						{
							if (g_Options.Visuals.XQZ)
							{

								g_RenderView->SetColorModulation(flColor);
								g_RenderView->SetBlend(g_Options.Visuals.oyuncuboyamaalfa / 100.f);
								g_ModelRender->ForcedMaterialOverride(znorm);
								ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							g_RenderView->SetColorModulation(flVColor);
							g_RenderView->SetBlend(g_Options.Visuals.oyuncuboyamaalfa / 100.f);
							g_ModelRender->ForcedMaterialOverride(norm);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else if (g_Options.Visuals.matierial == 1)
						{
							if (g_Options.Visuals.XQZ)
							{

								g_RenderView->SetColorModulation(flColor);
								g_RenderView->SetBlend(g_Options.Visuals.oyuncuboyamaalfa / 100.f);
								g_ModelRender->ForcedMaterialOverride(zflat);
								ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							g_RenderView->SetColorModulation(flVColor);
							g_RenderView->SetBlend(g_Options.Visuals.oyuncuboyamaalfa / 100.f);
							g_ModelRender->ForcedMaterialOverride(flat);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else if (g_Options.Visuals.matierial == 2)
						{
							if (g_Options.Visuals.XQZ)
							{

								g_RenderView->SetColorModulation(flColor);
								g_RenderView->SetBlend(g_Options.Visuals.oyuncuboyamaalfa / 100.f);
								g_ModelRender->ForcedMaterialOverride(zwire);
								ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							g_RenderView->SetColorModulation(flVColor);
							g_RenderView->SetBlend(g_Options.Visuals.oyuncuboyamaalfa / 100.f);
							g_ModelRender->ForcedMaterialOverride(wire);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							// Junk
							const char* xTFHvXXYTGoKRCxAETau = "Gnd";
bool wZyiayAbJrQgepPxyJSTt = true;
bool VldvybSUXKBMfzVrtnOVI = false;
const char* PGhO = "vAFMS";
bool QIvMVubCWTfsrqTmKNepBHoWEjaZHK = false;
const char* MTpiXxXLZqySXtw;
const char* IdYAxYqlePRxIBexXYfVSDDYGthCuyYfw = "JSFyB";
int ZmBInTa = 153249218812;
bool GNGUP = true;
char xeatoWzaBSDNvQhkFCmofI;
char FQgvpmvoRPJF;
int zMpJ = ZmBInTa + 39;
int kxJ = ZmBInTa + 2897;
int FKCyZoBGHtUoxRAJvPRE = 4;
int pSW = 816758296142;
const char* uQDSCILIQEC;
const char* uILGMFCSWt;
bool xlcBhqkVC = true;
float EbKrJLPGfgRATzbSGUvzIfNh;
const char* JsjA;
int UXpannAMxJDVzUHNACzYHepwzOl = 35939547848532193;
int dFzLKTBqdx = 61528519;
int mesBjlkyerTAHNexHUqmJicbMUSq = dFzLKTBqdx + 27595;
int XptxxEuMZ = dFzLKTBqdx + 65358933;
							// JUnk
						}
						else
						{
							if (g_Options.Visuals.XQZ)
							{

								g_RenderView->SetColorModulation(flColor);
								g_RenderView->SetBlend(g_Options.Visuals.oyuncuboyamaalfa / 100.f);
								g_ModelRender->ForcedMaterialOverride(zmat);
								ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							g_RenderView->SetColorModulation(flVColor);
							g_RenderView->SetBlend(g_Options.Visuals.oyuncuboyamaalfa / 100.f);
							g_ModelRender->ForcedMaterialOverride(mat);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							// Junk
							int unQP = 799;
bool BbcjpdyjlIbItllpLxELrdKexdTquv = false;
int EyHTLQLBBBvoQOzkyNayYRbAQmyjHzxnD = 14625696597149;
int cDlpQmVJiFVgY = 583184;
int QTmRpNdeVnmtHJiTsSpaQNjrdoYJWpYat = 5524;
int pAmSvbmV = QTmRpNdeVnmtHJiTsSpaQNjrdoYJWpYat + 15795;
bool KJIgAvyXjVEqvRBAC = true;
int AiPyHbBXi = QTmRpNdeVnmtHJiTsSpaQNjrdoYJWpYat + 4;
bool whRBlIrIzXOQPT = true;
int eaAfJEQiozPYFAUuqmVh = 51449769324216;
char Lslwuh;
const char* jRR = "bwH";
int ZkHdkcrFVBeiYIpVfVPZhYuJABtQwWtB = eaAfJEQiozPYFAUuqmVh + 3;
float EmhibYXrNXsxrzLeTbqExKRqOiernAPL;
const char* PQOubvJSeBAlWnKsVLOID;
int IrwycZTWZZvEWxoTaKuDKPED = eaAfJEQiozPYFAUuqmVh + 18148381;
int NeVdcpLbKlPjMPFdCYQ = eaAfJEQiozPYFAUuqmVh + 69;
const char* TrFfvpEKkq;
char PRYjautEuvCZv = 2884353;
const char* RgDLwx = "Aig";
int UeIcuFDIaMHtKbXoTdirXXGVzqMEy = eaAfJEQiozPYFAUuqmVh + 564233686;
bool ZaU = false;
int ftgmOPSRGpuGxt = 14245224464793354;
int murWmxc = 439612862296832589;
char XssQxnjOmUyjnJuWrSYxwq = 8257819;
int jdrlTBscIEDpiwLAQiGesJSQhsjLRglMWa = 447;
							// JUnk
						}
					}
					else
					{
						ForceMaterial(Color(255, 255, 255), znorm);
					}

				}
			}

			if (pModelEntity == pLocal)
			{
				if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
				{
					CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
					if (pLocal && pLocal->IsScoped() && g_Options.Visuals.tpopacity && MiscFunctions::IsSniper(pWeapon))
					{


						g_RenderView->SetBlend(0.3f);
						g_ModelRender->ForcedMaterialOverride(nullptr);
						ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
				}
				else
				{
					ForceMaterial(Color(255, 255, 255), zmat);
				}
			}
		}
	}



 


	if (!DontDraw)
		ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	g_ModelRender->ForcedMaterialOverride(NULL);


}

