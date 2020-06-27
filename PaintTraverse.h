#pragma once
#include "HookIncludes.h"
#include "Interfaces.h"
#include <ctime>
#include "ESP.h"
#include "MovementRecorder.h"

typedef void(__thiscall* paint_traverse_t)(PVOID, unsigned int, bool, bool);


bool once = true;
bool once1 = false;
int width1 = 0;
int height1 = 0;
std::string sPanel = ("FocusOverlayPanel");
void __fastcall hkPaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	static auto ofunc = hooks::panel.get_original<paint_traverse_t>(41);
	C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	if (local != nullptr && local->IsAlive() && g_Options.Visuals.noscopeborder && !strcmp("HudZoom", g_Panel->GetName(vguiPanel)))
	{
		return;
	}
	ofunc(pPanels, vguiPanel, forceRepaint, allowForce);
	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;
	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)g_Panel->GetName(vguiPanel);
		if (strstr(szPanelName, "FocusOverlayPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{

		if (g_Engine->IsConnected() && g_Engine->IsInGame())
		{
			static auto linegoesthrusmoke = U::FindPattern("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");
			static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
			if (g_Options.Visuals.NoSmoke) *(int*)(smokecout) = 0;

			visuals::instance().OnPaintTraverse(local);

			auto m_flFlashDuration = NetVarManager->GetOffset("DT_CSPlayer", "m_flFlashDuration");
			auto m_flFlashMaxAlpha = NetVarManager->GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
			if (local != nullptr)
			{
				CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
				if (g_Options.Visuals.NoFlash)
				{
					*MakePtr(float*, local, m_flFlashDuration) = 0.f;
					*MakePtr(float*, local, m_flFlashMaxAlpha) = 0.f;
				}
				if (local && local->IsScoped() && g_Options.Visuals.noscopeborder && MiscFunctions::IsSniper(pWeapon))
				{
					int width = 0;
					int height = 0;
					g_Engine->GetScreenSize(width, height);
					int centerX = static_cast<int>(width * 0.5f);
					int centerY = static_cast<int>(height * 0.5f);
					g_Render->Line(0, centerY, width, centerY, Color(0, 0, 0, 255));
					g_Render->Line(centerX, 0, centerX, height, Color(0, 0, 0, 255));

				}
			}
		}

		if (!g_Engine->IsInGame()) {	
			g_Options.Misc.NameChangerFix = false;
			g_Options.Misc.NameSpammer = 0;
			g_Options.Misc.NoName = false;
			g_Options.Misc.ChatSpamMode = false;
		}
		if (g_Options.Ragebot.MainSwitch && !once)
		{
			g_Options.Legitbot.MainSwitch = false;
			once = !once;
		}
		if (g_Options.Legitbot.MainSwitch && once)
		{
			g_Options.Ragebot.MainSwitch = false;
			once = !once;
		}

		if (g_Options.Visuals.minecraft)
		{
			static auto minecraft = g_CVar->FindVar("mat_showlowresimage");
			minecraft->SetValue(1);
		}
		else
		{
			static auto minecraft = g_CVar->FindVar("mat_showlowresimage");
			minecraft->SetValue(0);
		}

		if (g_Options.Visuals.grey)
		{
			static auto grey = g_CVar->FindVar("mat_drawgray");
			grey->SetValue(1);
		}
		else
		{
			static auto grey = g_CVar->FindVar("mat_drawgray");
			grey->SetValue(0);
		}

		if (g_Options.Visuals.NoPP)
		{
			static bool* s_bOverridePostProcessingDisable = *(bool**)(U::pattern_scan(GetModuleHandleW(L"client_panorama.dll"), "80 3D ? ? ? ? ? 53 56 57 0F 85") + 0x2);
			g_Options.Visuals.NoPP ? *s_bOverridePostProcessingDisable = true : *s_bOverridePostProcessingDisable = false;
		}

		if (g_Options.Visuals.floatingRagdolls)
		{

			static auto floatingRagdolls = g_CVar->FindVar("cl_ragdoll_gravity");
			floatingRagdolls->SetValue(-1);
		}
		else
		{
			static auto floatingRagdolls = g_CVar->FindVar("cl_ragdoll_gravity");
			floatingRagdolls->SetValue(600);
		}


		if (g_Options.Visuals.Time)
		{
			int width = 0;
			int height = 0;
			g_Engine->GetScreenSize(width, height);

			std::time_t result = std::time(nullptr);

			if (g_Engine->IsInGame() && g_Engine->IsConnected())
			{
				if (local->IsAlive())
				{
					if (g_Options.Visuals.Time)
					{
						if (local->GetEyeAngles()->y - local->GetLowerBodyYaw() > 35)
						{
							g_Render->Textf(8, height - 85, Color(182, 221, 37, 255), g_Render->font.LBY, "LBY");
							/*--------------------------------------------------------------
							JUNK START
							----------------------------------------------------------------*/
							bool llyJPVhmnZd;
							bool SpkjS = false;
							int tkoxzECWNvDI = 7838917193;
							int rBqvJQZYhyGUHjjguXhZYpLwIiREs = tkoxzECWNvDI + 67813979;
							bool GqccslR = true;
							int qHvdehnjljKwOmiPKSJEqxfwh = tkoxzECWNvDI + 3;
							int yfoRVESpPHluejHKgOTyZuDUnghUu = tkoxzECWNvDI + 417648627;
							int TNpkgGiWpZyOKLMaEKfxpQQxMYNtpIotQc = tkoxzECWNvDI + 31146;
							int eysAqNRYWntTRYdLuZyFHU = 284346839822;
							bool LAteobSbRyMvTN;
							const char* wRjMhUYSHkrCxjAb = "LkdiKAyVr";
							char jfkwOWycnEOVTchrkVGEEWqieBOS;
							int feKxqhsqCuEoHWPUDvUKJAvFepisIIFkk = 6486;
							/*--------------------------------------------------------------
							JUNK END
							----------------------------------------------------------------*/
						}
						else
						{
							g_Render->Textf(8, height - 85, Color(188, 32, 32, 255), g_Render->font.LBY, "LBY");
							/*--------------------------------------------------------------
							JUNK START
							----------------------------------------------------------------*/
							const char* UUzSfCFMaSBbsmthYhLOxnwuEEosIUUP;
							bool XkYFrLLoMgvpsnEvMidmrPBHEpwGd = true;
							float xcdPTWoKIIOxHOpMTRb;
							const char* DiqhohveTjEdBjpAhgzTeZSBcvLGnEFhpw = "WuINN";
							int LgxtUqwpmYQxCaCIIcdPrElAZhPXA = 141327128215;
							bool GGZYrfX;
							int bZqCQVHS = LgxtUqwpmYQxCaCIIcdPrElAZhPXA + 15562;
							float nLpyQkXUVerkQaLNYvZZ;
							int yfyUAZwrIoQVQWly = 5512727339536182764;
							int vZmjRCUIVDUhq = 4584596436359161;
							char VLLhYnRmo;
							char EvKmcWpn = 16275;
							bool chtXrpRxzNXiOSBRB = false;
							char DoKNxFTxzlmMvVntDzqfPviYrxgnCvodQ = 264938559;
							bool wxeTiUkPSzRswAJghzoGRNNTIGGchnj;
							int NtXpRODMSe = 763572749824;
							int cZFKeyQjTWn = NtXpRODMSe + 842129;
							/*--------------------------------------------------------------
							JUNK END
							----------------------------------------------------------------*/
						}
					}
				}
			}
		}

	}



	const char* pszPanelName = g_Panel->GetName(vguiPanel);

	if (!strstr(pszPanelName, sPanel.data()))
		return;


	g_Panel->SetMouseInputEnabled(vguiPanel, G::menuon);
}


