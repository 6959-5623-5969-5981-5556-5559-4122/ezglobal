#pragma once
#include "Configuration.hpp"
#include "dropboxes.h"
#include "Variables.h"
#include "Themes.h"
#include <cctype>
#include "Logging.h"
#include "Listener.h"
#include "locale.h"


#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define BUILDSTAMP ( __DATE__ )
typedef void(*CL_FullUpdate_t) (void);
CL_FullUpdate_t CL_FullUpdate = nullptr;


void FullUpdate()
{

	static auto CL_FullUpdate = reinterpret_cast<CL_FullUpdate_t>(U::FindPattern("engine.dll", reinterpret_cast<PBYTE>("\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85"), "x????x????xxxxxxx"));
	CL_FullUpdate();

}


void  setName(const char* name)
{
	static ConVar* namevar = g_CVar->FindVar("name");
	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(&namevar->fnChangeCallback) + 0xC) = NULL;
	namevar->SetValue(name);
}



const char* weaponnames(int id)
{
	switch (id)
	{
	case 1:
		return "deagle";
	case 2:
		return "elite";
	case 3:
		return "fiveseven";
	case 4:
		return "glock";
	case 7:
		return "ak47";
	case 8:
		return "aug";
	case 9:
		return "awp";
	case 10:
		return "famas";
	case 11:
		return "g3sg1";
	case 13:
		return "galilar";
	case 14:
		return "m249";
	case 60:
		return "m4a1_silencer";
	case 16:
		return "m4a1";
	case 17:
		return "mac10";
	case 19:
		return "p90";
	case 24:
		return "ump45";
	case 25:
		return "xm1014";
	case 26:
		return "bizon";
	case 27:
		return "mag7";
	case 28:
		return "negev";
	case 29:
		return "sawedoff";
	case 30:
		return "tec9";
	case 32:
		return "hkp2000";
	case 33:
		return "mp7";
	case 34:
		return "mp9";
	case 35:
		return "nova";
	case 36:
		return "p250";
	case 38:
		return "scar20";
	case 39:
		return "sg556";
	case 40:
		return "ssg08";
	case 61:
		return "usp_silencer";
	case 63:
		return "cz75a";
	case 64:
		return "revolver";
	case 508:
		return "knife_m9_bayonet";
	case 500:
		return "bayonet";
	case 505:
		return "knife_flip";
	case 506:
		return "knife_gut";
	case 507:
		return "knife_karambit";
	case 509:
		return "knife_tactical";
	case 512:
		return "knife_falchion";
	case 514:
		return "knife_survival_bowie";
	case 515:
		return "knife_butterfly";
	case 516:
		return "knife_push";

	default:
		return "";
	}
}

bool IsUtility(ItemDefinitionIndexx index)
{
	switch (index)
	{
	case ItemDefinitionIndexx::WEAPON_NONE:
	case ItemDefinitionIndexx::WEAPON_C4:
	case ItemDefinitionIndexx::WEAPON_FLASHBANG:
	case ItemDefinitionIndexx::WEAPON_HEGRENADE:
	case ItemDefinitionIndexx::WEAPON_INCGRENADE:
	case ItemDefinitionIndexx::WEAPON_MOLOTOV:
	case ItemDefinitionIndexx::WEAPON_SMOKEGRENADE:
	case ItemDefinitionIndexx::WEAPON_DECOY:
	case ItemDefinitionIndexx::WEAPON_KNIFE_T:
	case ItemDefinitionIndexx::WEAPON_KNIFE:
		return true;
	default:
		return false;
	}
}

bool Contains(const std::string &word, const std::string &sentence) {
	if (word == "" || sentence == "")
		return true;

	return sentence.find(word) != std::string::npos;
}

std::string ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), (int(*)(int))std::tolower);

	return str;
}

bool defaultk;
bool bayonet;
bool flip;
bool gut;
bool karambit;
bool m9bayonet;
bool huntsman;
bool falchion;
bool bowie;
bool butterfly;
bool daggers;

void doknives(bool* disknife)
{
	defaultk = false;
	bayonet = false;
	flip = false;
	gut = false;
	karambit = false;
	m9bayonet = false;
	huntsman = false;
	falchion = false;
	bowie = false;
	butterfly = false;
	daggers = false;

	*disknife = true;
}

const char* HitmarkSound[] =
{
	"Disabled",
	"Default",
	"Metallic"
};



int tab = 2;





void RenderInterface() {


	auto& style = ImGui::GetStyle();
	bool is_renderer_active = renderer->IsActive();
	if (&g_Options.Menu.Opened && style.Alpha < 1.00f)
	{
		g_Options.Menu.Opened = true;
		if (style.Alpha > 1.f)
			style.Alpha = 1.f;
		else if (style.Alpha != 1.f)
			style.Alpha += 0.f;
	}
	else if (style.Alpha > 0.f)
	{
		if (style.Alpha < 0.f)
			style.Alpha = 1.f;
		else if (style.Alpha != 0.f)
			style.Alpha -= 0.03f;
		if (style.Alpha == 0.f)
			g_Options.Menu.Opened = false;
	}

	ImGui::Begin("TenseHook Advanced for Counter-Strike: Global Offensive", &g_Options.Menu.Opened, ImVec2(890, 600), 1.f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
	style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 1.00f, 1.00f, 0.01f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.01f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.01f);
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	if (ImGui::Button("Rage", ImVec2(120, 65)))
	{
		tab = 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("Anti-Aim", ImVec2(120, 65)))

	{
		tab = 4;
	}
	ImGui::SameLine();
	if (ImGui::Button("Legit", ImVec2(120, 65)))
	{
		tab = 6;
	}
	ImGui::SameLine();
	if (ImGui::Button("Visuals", ImVec2(120, 65)))
	{
		tab = 2;
	}
	ImGui::SameLine();
	if (ImGui::Button("Miscellaneous", ImVec2(120, 65)))
	{
		tab = 3;
	}
	
	ImGui::SameLine();
	if (ImGui::Button("Skinchanger", ImVec2(120, 65)))
	{
		tab = 5;
	}
	
	style.Colors[ImGuiCol_Button] = ImVec4(0.87f, 0.89f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.81f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.98f, 0.06f, 1.00f);
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	
	if (tab == 1) 
	{
		ImGui::Text("Rage settings");
		{
			ImGui::PushItemWidth(200);
			ImGui::Checkbox(XorStr("Master function switch."), &g_Options.Ragebot.Enabled);
			ImGui::Checkbox(XorStr("Auto fire"), &g_Options.Ragebot.AutoFire);
			ImGui::NewLine();
			ImGui::SameLine();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			ImGui::SliderFloat(XorStr("FOV"), &g_Options.Ragebot.FOV, 1.f, 180.f, "%.0f");
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			ImGui::Checkbox(XorStr("Silent aim"), &g_Options.Ragebot.Silent);
			ImGui::Checkbox(XorStr("Auto pistol"), &g_Options.Ragebot.AutoPistol);
			ImGui::Text("Target hitbox");
			ImGui::NewLine();
			ImGui::SameLine();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			ImGui::Combo(XorStr("##hitbox"), &g_Options.Ragebot.Hitbox, aimBones, ARRAYSIZE(aimBones));
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			ImGui::Text("Target hitscan");
			ImGui::NewLine();
			ImGui::SameLine();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			ImGui::Combo(XorStr("##hitscan"), &g_Options.Ragebot.Hitscan, hitscan, ARRAYSIZE(hitscan));
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			ImGui::Checkbox(XorStr("Minimun damage"), &g_Options.Ragebot.mindmg);
			if (g_Options.Ragebot.mindmg)
			{
				style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
				ImGui::NewLine();
				ImGui::SameLine();
				ImGui::SliderFloat(("Sniper##sniper"), &g_Options.Ragebot.MinimumDamageSniper, 1.f, 100.f, "%.2f");
				ImGui::NewLine();
				ImGui::SameLine();
				ImGui::SliderFloat(("Rifle##rifle"), &g_Options.Ragebot.MinimumDamageRifle, 1.f, 100.f, "%.2f");
				ImGui::NewLine();
				ImGui::SameLine();
				ImGui::SliderFloat(("Pistol##pistol"), &g_Options.Ragebot.MinimumDamagePistol, 1.f, 100.f, "%.2f");
				ImGui::NewLine();
				ImGui::SameLine();
				ImGui::SliderFloat(("Heavy##heavy"), &g_Options.Ragebot.MinimumDamageHeavy, 1.f, 100.f, "%.2f");
				ImGui::NewLine();
				ImGui::SameLine();
				ImGui::SliderFloat(("SMG##smg"), &g_Options.Ragebot.MinimumDamageSmg, 1.f, 100.f, "%.2f");
				ImGui::NewLine();
				ImGui::SameLine();
				ImGui::SliderFloat(("Deagle & Revolver##deagandrevol"), &g_Options.Ragebot.MinimumDamageRevolver, 1.f, 100.f, "%.2f");
				style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			}
				ImGui::Checkbox(XorStr("Hitchance"), &g_Options.Ragebot.Hitchance);
				if (g_Options.Ragebot.Hitchance)
				{
					style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
					ImGui::NewLine();
					ImGui::SameLine();
					ImGui::SliderFloat(XorStr("Sniper"), &g_Options.Ragebot.HitchanceSniper, 0.f, 100.f, "%.1f");
					ImGui::NewLine();
					ImGui::SameLine();
					ImGui::SliderFloat(XorStr("Rifle"), &g_Options.Ragebot.HitchanceRifle, 0.f, 100.f, "%.1f");
					ImGui::NewLine();
					ImGui::SameLine();
					ImGui::SliderFloat(XorStr("Pistol"), &g_Options.Ragebot.HitchancePistol, 0.f, 100.f, "%.1f");
					ImGui::NewLine();
					ImGui::SameLine();
					ImGui::SliderFloat(XorStr("SMG"), &g_Options.Ragebot.HitchanceSmgs, 0.f, 100.f, "%.1f");
					ImGui::NewLine();
					ImGui::SameLine();
					ImGui::SliderFloat(XorStr("Heavy"), &g_Options.Ragebot.HitchanceHeavy, 0.f, 100.f, "%.1f");
					ImGui::NewLine();
					ImGui::SameLine();
					ImGui::SliderFloat(XorStr("Deagle & Revolver"), &g_Options.Ragebot.HitchanceRevolver, 0.f, 100.f, "%.1f");
					style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				}
			ImGui::NextColumn();
			ImGui::Text("Miscellaneous");
			{
				ImGui::Checkbox(XorStr("Resolver"), &g_Options.Ragebot.Resolver);
				ImGui::SameLine();
				ImGui::Checkbox(XorStr("Anti Recoil"), &g_Options.Ragebot.AntiRecoil);
				ImGui::Checkbox(XorStr("Position Adjustment    "), &g_Options.Ragebot.PosAdjust);
				ImGui::SameLine();
				ImGui::Checkbox(XorStr("Auto stop"), &g_Options.Ragebot.otodurma);
				ImGui::Checkbox(XorStr("Auto crouch"), &g_Options.Ragebot.ototur);
				ImGui::SameLine();
				ImGui::Checkbox(XorStr("Auto zoom"), &g_Options.Ragebot.otozoom);
			}


		}
	}

	if (tab == 2)
	{
		ImGui::Columns(2, false);
		ImGui::PushItemWidth(200);
		ImGui::Checkbox(XorStr("Team ESP"), &g_Options.Visuals.takimesp);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##ESP", g_Options.Colors.takimesp, 1 << 7);
		ImGui::Checkbox(XorStr("Box"), &g_Options.Visuals.Box);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##EdddSP", g_Options.Colors.dusmanesp, 1 << 7);
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		if (g_Options.Visuals.Box)
		{
			ImGui::NewLine();
			ImGui::SameLine();
			ImGui::Combo("##boxtype", &g_Options.Visuals.BoxType, "Square\0\rAngular\0\r3d\0\0", -1);
		}
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		ImGui::Checkbox(XorStr("Name"), &g_Options.Visuals.Name);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##NAMECLR", g_Options.Colors.nameclr, 1 << 7);
		ImGui::Checkbox(XorStr("Health"), &g_Options.Visuals.health);
		ImGui::Checkbox(XorStr("Armor"), &g_Options.Visuals.armor);
		ImGui::Checkbox(XorStr("Ammo"), &g_Options.Visuals.ammo);
		ImGui::Checkbox(XorStr("Draw auto wall damage"), &g_Options.Visuals.DrawAwall);
		ImGui::Checkbox(XorStr("Money"), &g_Options.Visuals.Money);
		ImGui::Checkbox(XorStr("Callout"), &g_Options.Visuals.Callout);
		ImGui::Checkbox(XorStr("Distance"), &g_Options.Visuals.Distance);
		ImGui::Checkbox(XorStr("Aim lines"), &g_Options.Visuals.AimLine);
		ImGui::NewLine();
		ImGui::SameLine();
		
		ImGui::NextColumn();
		ImGui::Text("Miscellaneous");
		ImGui::PushItemWidth(200);
		ImGui::PushItemWidth(200);
		ImGui::Checkbox(XorStr("Resolve mode"), &g_Options.Visuals.resolveMode);
		ImGui::Checkbox(XorStr("Dropped weapons"), &g_Options.Visuals.Droppedguns);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##DROPWPNS", g_Options.Colors.DroppedWeapons, 1 << 7);
		ImGui::Checkbox(XorStr("Anti-Aim lines"), &g_Options.Visuals.angleLines);
		ImGui::Checkbox(XorStr("LBY Indicator"), &g_Options.Visuals.Time);
		ImGui::Checkbox(XorStr("Weapon glow"), &g_Options.Visuals.weaponglow);
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		ImGui::SliderInt(XorStr(" ##wga"), &g_Options.Visuals.weaponglowalpha, 0, 255);
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		ImGui::Text("Chams");
		ImGui::PushItemWidth(200);
		ImGui::Checkbox(XorStr("Enable chams"), &g_Options.Visuals.Chams);
		ImGui::SameLine();

		ImGui::MyColorEdit3("##45ESP", g_Options.Colors.dusmantakimchams, 1 << 7);
		ImGui::Checkbox(XorStr("Team Chams"), &g_Options.Visuals.takimchams);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##E772SP", g_Options.Colors.takimchamsgozuk, 1 << 7);
		ImGui::Checkbox(XorStr("XQZ"), &g_Options.Visuals.XQZ);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##43ESP", g_Options.Colors.dusmanchamsgozuk, 1 << 7);
		ImGui::PushItemWidth(200);
		ImGui::NewLine();
		ImGui::SameLine();
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		ImGui::SliderInt(XorStr(" ##pca"), &g_Options.Visuals.oyuncuboyamaalfa, 0, 100);
		ImGui::NewLine();
		ImGui::SameLine();
		
		ImGui::Combo("##sdgl", &g_Options.Visuals.matierial, "Normal\0\rColor\0\rFlat\0\rMetallic\0\rGolden\0\rPlatinum\0\rBallon\0\rCrystal\0\0", -1);
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

	}

	if (tab == 3)
	{
		ImGui::PushItemWidth(-1);
		ImGui::Checkbox(XorStr("Auto hop  "), &g_Options.Misc.Bhop);
		ImGui::SameLine();
		ImGui::Checkbox(XorStr("Spectator list"), &g_Options.Misc.SpecList);
		ImGui::SameLine();
		ImGui::Checkbox(XorStr("Scope opacity"), &g_Options.Visuals.tpopacity);
		ImGui::SameLine();
		ImGui::Checkbox(XorStr("Remove smoke"), &g_Options.Visuals.NoSmoke);
		ImGui::SameLine();
		ImGui::Checkbox(XorStr("Remove flash"), &g_Options.Visuals.NoFlash);
		ImGui::Checkbox(XorStr("Remove scope border"), &g_Options.Visuals.noscopeborder);
		ImGui::SameLine();
		ImGui::Checkbox(XorStr("Nightmode        "), &g_Options.Misc.nightMode);
		ImGui::NewLine();
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		ImGui::PushItemWidth(200);
		ImGui::SliderFloat(XorStr("FOV"), &g_Options.Visuals.FOVChanger, 0, 60, "%.0f");
		ImGui::SliderFloat(XorStr("Viewmodel FOV"), &g_Options.Visuals.viewmodelChanger, 68, 140, "%.0f");
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		ImGui::Checkbox(XorStr("Flying ragdolls"), &g_Options.Visuals.floatingRagdolls);
		ImGui::Checkbox(XorStr("Hitmarker"), &g_Options.Visuals.Hitmarker);
		ImGui::Text(XorStr("Hitsound:"));
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		ImGui::Combo(XorStr("##asad4asds"), &g_Options.Visuals.Hitsound, HitmarkSound, ARRAYSIZE(HitmarkSound));
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		ImGui::PushItemWidth(-1);
		ImGui::NewLine();
		ImGui::Checkbox("Clantag", &g_Options.Misc.klantagxddx);
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		if (ImGui::Button("Name spam"))
			setName(u8"www.TenseHook.tk");
			setName(u8"www.TenseHook.tk");
			setName(u8"www.TenseHook.tk");
			setName(u8"www.TenseHook.tk");
			setName(u8"www.TenseHook.tk");
			setName(u8"www.TenseHook.tk");
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::Text("Settings");
		ImGui::PushItemWidth(200);
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		ImGui::Combo((""), &g_Options.Menu.ConfigFile, configFiles, ARRAYSIZE(configFiles));
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		if (ImGui::Button("Save")) Config->Save();

		if (ImGui::Button("Load")) Config->Load();
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		
	}

	if (tab == 4) {
		ImGui::Text("Anti-Aim");
		{
			ImGui::PushItemWidth(200);
			ImGui::Checkbox(XorStr("Master function switch"), &g_Options.Ragebot.EnabledAntiAim);
			ImGui::Text("Pitch");
			ImGui::NewLine();
			ImGui::SameLine();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			ImGui::Combo(XorStr("##pitch"), &g_Options.Ragebot.Pitch, antiaimpitch, ARRAYSIZE(antiaimpitch));
			ImGui::NewLine();
			ImGui::SameLine();		
			ImGui::SliderFloat(XorStr("Pitch adder: "), &g_Options.Ragebot.PitchAdder, -180, 180, "%1.f");
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			ImGui::Text("Yaw");
			ImGui::NewLine();
			ImGui::SameLine();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			ImGui::Combo(XorStr("##yaw"), &g_Options.Ragebot.YawTrue, antiaimyawtrue, ARRAYSIZE(antiaimyawtrue));
			ImGui::NewLine();
			ImGui::SameLine();			
			ImGui::SliderFloat(XorStr("Yaw true adder: "), &g_Options.Ragebot.YawTrueAdder, -180, 180, "%1.f");
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			ImGui::Text("Fake");
			ImGui::NewLine();
			ImGui::SameLine();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			ImGui::Combo(XorStr("##fake"), &g_Options.Ragebot.YawFake, antiaimyawfake, ARRAYSIZE(antiaimyawfake));
			ImGui::NewLine();
			ImGui::SameLine();
			ImGui::SliderFloat(XorStr("Yaw fake adder: "), &g_Options.Ragebot.YawFakeAdder, -180, 180, "%1.f");
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			ImGui::NewLine();
			ImGui::SameLine();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			ImGui::SliderInt(XorStr("LBY Delta: "), &g_Options.Ragebot.LBYDeltaReal, -180, 180, "%1.f");
			ImGui::Checkbox(XorStr("Edge"), &g_Options.Ragebot.Edge);
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		}
	}	

	if (tab == 5)
	{
		ImGui::BeginChild("Skinchanger", ImVec2(620, 270), true);
		{
			ImGui::Checkbox(XorStr("Master function switch"), &g_Options.Skinchanger.Enabled);
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			if (ImGui::Button(XorStr("Force update"), ImVec2(-1, 20))) FullUpdate();
			ImGui::BeginChild("Weapons", ImVec2(190, 200), true);
			{
				static char filterWeapons[32];
				static char filterSkins[32];
				ImGui::PushItemWidth(-1);
				ImGui::InputText(XorStr("##FILTERWEAPONS"), filterWeapons, IM_ARRAYSIZE(filterWeapons));
				ImGui::PopItemWidth();
				ImGui::ListBoxHeader(XorStr("##GUNS"), ImVec2(-1, -1));
				for (auto it : ItemDefinitionIndexMap)
				{
					bool isDefault = (int)it.first < 0;
					if (!isDefault && !Contains(ToLower(std::string(filterWeapons)), ToLower(ItemDefinitionIndexMap.at(it.first).entityName)))
						continue;

					if (IsUtility(it.first))
						continue;

					const bool item_selected = ((int)it.first == (int)g_Options.Menu.currentWeapon);
					//ImGui::PushID((int)it.first);
					std::string formattedName;

					formattedName = ItemDefinitionIndexMap.at(it.first).killIcon;
					if (ImGui::Selectable(formattedName.c_str(), item_selected))
					{
						g_Options.Menu.currentWeapon = (int)it.first;
					}
					//ImGui::PopID();
				}
				ImGui::ListBoxFooter();
			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild("Skins", ImVec2(190, 200), true);
			{
				static char filterWeapons[32];
				static char filterSkins[32];
				std::string skinName = weaponnames(g_Options.Menu.currentWeapon);
				{
					static char filterWeapons[32];
					static char filterSkins[32];
					ImGui::PushItemWidth(-1);
					ImGui::InputText(XorStr("##FILTERSKINS"), filterSkins, IM_ARRAYSIZE(filterSkins));
					ImGui::InputInt(XorStr("##custom ID"), &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin);
					ImGui::PopItemWidth();
					ImGui::ListBoxHeader(XorStr("##SKINS"), ImVec2(-1, -1));
					std::string skinStr = "";
					int curItem = -1;
					int s = 0;
					for (auto skin : g_Options.Skinchanger.weaponSkins[skinName])
					{
						int pk = g_Options.Skinchanger.skinMap[skin].paintkit;
						if (pk == g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin)
							curItem = s;
						bool selected = g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin == pk;

						skinStr = g_Options.Skinchanger.skinNames[g_Options.Skinchanger.skinMap[skin].tagName].c_str();
						skinStr += " | ";
						skinStr += std::to_string(g_Options.Skinchanger.skinMap[skin].paintkit);


						if (!Contains(ToLower(std::string(filterSkins)), ToLower(skinStr)))
							continue;
						if (ImGui::Selectable(skinStr.c_str(), selected))
						{
							pk = g_Options.Skinchanger.skinMap[skin].paintkit;
							g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin = pk;
							//      FullUpdate();
							break;
						}
						s++;
					}

					ImGui::ListBoxFooter();
				}
			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild("Model", ImVec2(-1, 200), true);
			{
				ImGui::ListBoxHeader("##KNIVES", ImVec2(-1, -1));
				{
					if (ImGui::RadioButton(XorStr("Default"), &g_Options.Skinchanger.knifemodel, 0))
					{
					}
					if (ImGui::RadioButton(XorStr("Bayonet"), &g_Options.Skinchanger.knifemodel, 1))
					{
					}
					if (ImGui::RadioButton(XorStr("Flip Knife"), &g_Options.Skinchanger.knifemodel, 2))
					{
					}
					if (ImGui::RadioButton(XorStr("Gut Knife"), &g_Options.Skinchanger.knifemodel, 3))
					{
					}
					if (ImGui::RadioButton(XorStr("Karambit"), &g_Options.Skinchanger.knifemodel, 4))
					{
					}
					if (ImGui::RadioButton(XorStr("M9 Bayonet"), &g_Options.Skinchanger.knifemodel, 5))
					{
					}
					if (ImGui::RadioButton(XorStr("Huntsman"), &g_Options.Skinchanger.knifemodel, 6))
					{
					}
					if (ImGui::RadioButton(XorStr("Falchion"), &g_Options.Skinchanger.knifemodel, 7))
					{
					}
					if (ImGui::RadioButton(XorStr("Bowie"), &g_Options.Skinchanger.knifemodel, 8))
					{
					}
					if (ImGui::RadioButton(XorStr("Butterfly Knife"), &g_Options.Skinchanger.knifemodel, 9))
					{
					}
					if (ImGui::RadioButton(XorStr("Shadow Daggers"), &g_Options.Skinchanger.knifemodel, 10))
					{
					}
					
				}
				ImGui::ListBoxFooter();
			}ImGui::EndChild();


		}
		ImGui::EndChild();
	}

	if (tab == 6)
	{
		ImGui::BeginChild("Legitbot", ImVec2(620, 505), true);
		{
			ImGui::Checkbox(XorStr("Master function switch"), &g_Options.Legitbot.MainSwitch);
			ImGui::BeginChild("Legit", ImVec2(195, 200), true);

			{
				ImGui::Text("Global");
				ImGui::PushItemWidth(-1);
				style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
				ImGui::Hotkey(XorStr("Key##0"), &g_Options.Legitbot.MainKey);
				ImGui::SliderFloat(XorStr("Smooth##0"), &g_Options.Legitbot.MainSmooth, 1.00f, 100.00f, "%.2f");
				ImGui::SliderFloat(XorStr("FOV##0"), &g_Options.Legitbot.Mainfov, 0.00f, 30.00f, "%.2f");
				ImGui::SliderFloat(XorStr("Minimum recoil##0"), &g_Options.Legitbot.main_recoil_min, 1.00f, 100.00f, "%.2f");
				ImGui::SliderFloat(XorStr("Maximum recoil##0"), &g_Options.Legitbot.main_recoil_max, 1.00f, 100.00f, "%.2f");
				style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				ImGui::Separator();
				ImGui::PushItemWidth(-1);
				ImGui::Checkbox(XorStr("Position adjustment"), &g_Options.Legitbot.backtrack);
				ImGui::SameLine();
				style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
				ImGui::SliderFloat(XorStr("Tick: "), &g_Options.Legitbot.backtrackTicks, 0, 12, "%1.f");
				style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild("Pistol", ImVec2(195, 200), true);
			{
				ImGui::Text("Pistol");
				ImGui::PushItemWidth(-1);
				style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
				ImGui::Hotkey(XorStr("Key##1"), &g_Options.Legitbot.PistolKey);
				ImGui::SliderFloat(XorStr("Smooth##1"), &g_Options.Legitbot.PistolSmooth, 1.00f, 100.00f, "%.2f");
				ImGui::SliderFloat(XorStr("FOV##1"), &g_Options.Legitbot.Pistolfov, 0.00f, 30.00f, "%.2f");
				ImGui::SliderFloat(XorStr("Minimum recoil##1"), &g_Options.Legitbot.pistol_recoil_min, 1.00f, 100.00f, "%.2f");
				ImGui::SliderFloat(XorStr("Maximum recoil##1"), &g_Options.Legitbot.pistol_recoil_max, 1.00f, 100.00f, "%.2f");
				style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			}ImGui::EndChild();
			ImGui::SameLine();

			ImGui::BeginChild("Sniper", ImVec2(195, 200), true);
			{
				ImGui::Text("Sniper");
				ImGui::PushItemWidth(-1);
				style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
				ImGui::Hotkey(XorStr("Key##2"), &g_Options.Legitbot.SniperKey);
				ImGui::SliderFloat(XorStr("Smooth##2"), &g_Options.Legitbot.SniperSmooth, 1.00f, 100.00f, "%.2f");
				ImGui::SliderFloat(XorStr("FOV##2"), &g_Options.Legitbot.Sniperfov, 0.00f, 30.00f, "%.2f");
				ImGui::SliderFloat(XorStr("Minimum recoil##2"), &g_Options.Legitbot.sniper_recoil_min, 1.00f, 100.00f, "%.2f");
				ImGui::SliderFloat(XorStr("Maximum recoil##2"), &g_Options.Legitbot.sniper_recoil_max, 1.00f, 100.00f, "%.2f");				
				style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			}ImGui::EndChild();

			ImGui::BeginChild("Triggerbot", ImVec2(195, 225), true);
			{
				ImGui::Text("Triggerbot");
				ImGui::PushItemWidth(-1);
				style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
				ImGui::Hotkey("Key##3", &g_Options.Legitbot.Triggerbot.Key);
				ImGui::SliderFloat(("Delay"), &g_Options.Legitbot.Triggerbot.Delay, 1.f, 150.f, "%.0f");
				ImGui::SliderFloat(("Hitchance"), &g_Options.Legitbot.Triggerbot.hitchance, 1.f, 100.f, "%.0f");
				style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				ImGui::Text(("Hitboxes"));
				ImGui::Separator();
				ImGui::Selectable((" Head"), &g_Options.Legitbot.Triggerbot.Filter.Head);
				ImGui::Selectable((" Arms"), &g_Options.Legitbot.Triggerbot.Filter.Arms);
				ImGui::Selectable((" Chest"), &g_Options.Legitbot.Triggerbot.Filter.Chest);
				ImGui::Selectable((" Stomach"), &g_Options.Legitbot.Triggerbot.Filter.Stomach);
				ImGui::Selectable((" Legs"), &g_Options.Legitbot.Triggerbot.Filter.Legs);


			}ImGui::EndChild();

		}ImGui::EndChild();
	}

	ImGui::End();
}