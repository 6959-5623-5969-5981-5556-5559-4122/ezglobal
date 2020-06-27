
#include "ESP.h"
#include "Interfaces.h"
#include "Render.h"
#include <ctime>
#include <iostream>
#include <algorithm>
#include "GrenadePrediction.h"
#include "LagComp.h"
#include "Autowall.h"
#include "EngineClient.h"
#include "Entities.h"

visuals::visuals()
{
	BombCarrier = nullptr;
}

int width = 0;
int height = 0;
bool done = false;

float damage;
char bombdamagestringdead[24];
char bombdamagestringalive[24];

void visuals::OnPaintTraverse(C_BaseEntity* local)
{


	NightMode();
	if (g_Options.Misc.SpecList) SpecList(local);
	grenade_prediction::instance().Paint();
	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());


	if (g_Options.Visuals.RecoilCrosshair)
		Crosshair();

	if (g_Options.Visuals.Hitmarker)
		Hitmarker();

	if (g_Options.Visuals.angleLines && pLocal->IsAlive())
		DrawAngles();

	if (g_Options.Visuals.DrawAwall && pLocal->IsAlive())
		DrawAwall();

	 

	for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
	{

		C_BaseEntity *entity = g_EntityList->GetClientEntity(i);
		if (entity == nullptr)
			continue;
		if (entity == local && !g_Input->m_fCameraInThirdPerson == true)
			continue;

		if (entity == local)
			continue;
		if (entity->IsDormant())
			continue;
	 

		player_info_t pinfo;
		Vector pos, pos3D;
		pos3D = entity->GetOrigin();
		CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)(entity);


		 auto owner = entity->GetOwnerHandle();
		 ClientClass* cClass = (ClientClass*)entity->GetClientClass();

	
		if (!g_Render->WorldToScreen(pos3D, pos))
			continue;

		if (g_Options.Visuals.Droppedguns)
		{
			if (strstr(entity->GetClientClass()->m_pNetworkName, XorStr("CWeapon")))
			{
				int ClassID = cClass->m_ClassID;
				int owner = entity->GetOwnerHandle();

				
				if (owner = -1)
				{
				
					//backtrackcllr
					auto meme = Color(g_Options.Colors.DroppedWeapons[0] * 255, g_Options.Colors.DroppedWeapons[1] * 255, g_Options.Colors.DroppedWeapons[2] * 255, 255);
					CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)(entity);
					Vector vOrig; Vector vScreen;
					vOrig = entity->GetOrigin();
					g_Render->DrawString2(g_Render->font.ESPMini, (int)pos.x, (int)pos.y - 10, meme, FONT_CENTER, "%i FT", flGetDistance(local->GetOrigin(), vOrig));
					g_Render->DrawString2(g_Render->font.ESPMini, (int)pos.x, (int)pos.y, meme, FONT_CENTER, "%s" " " "(" "%i" "/" "%i" ")", std::string(entity->GetClientClass()->m_pNetworkName).substr(7).c_str(), weapon->ammo(), weapon->ammo2());

					if (ClassID == 1)
					{
						g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, Color(255, 255, 255, 255), FONT_RIGHT, "%s", XorStr("Desert Eagle"));
					}
					if (ClassID == 39)
					{
						g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, Color(255, 255, 255, 255), FONT_RIGHT, "%s", XorStr("AK-47"));
					}

					if (ClassID == 29)
					{
						g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, Color(255, 255, 255, 255), FONT_RIGHT, "%s", XorStr("Dropped C4"));
					}
					if (ClassID == 2)
					{
						g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, Color(255, 255, 255, 255), FONT_RIGHT, "%s", XorStr("Defuse Kit"));
					}
				}
			}
		}

		

		if (g_Options.Visuals.Bomb && cClass->m_ClassID == (int)ClassID::ClassId_CPlantedC4)
		{

		}



		if (g_Options.Visuals.Grenades)
		{

			if (!g_Render->WorldToScreen(pos3D, pos))
				continue;

			Color GrenadeColor	= Color(0, 0, 0, 0);
			char* szModelName	= "";
			char* szIconName	= "";
			if (strstr(cClass->m_pNetworkName, XorStr("Projectile")))
			{
				const model_t* pModel	= entity->GetModel();
				if (!pModel)
					return;


				const studiohdr_t* pHdr = g_ModelInfo->GetStudiomodel(pModel);
				if (!pHdr)
					return;

				if (!strstr(pHdr->name, XorStr("thrown")) && !strstr(pHdr->name, XorStr("dropped")))
					return;



				IMaterial* mats[32];
				g_ModelInfo->GetModelMaterials(pModel, pHdr->numtextures, mats);

				for (int i = 0; i < pHdr->numtextures; i++)
				{
					IMaterial* mat = mats[i];
					if (!mat)
						continue;

					if (strstr(mat->GetName(), "flashbang"))
					{
						szModelName		= "flashbang";
						szIconName		= "G";
						GrenadeColor	= Color(255, 255, 0, 255);
					}
					else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
					{
						szModelName		= "Frag";
						szIconName		= "H";
						GrenadeColor	= Color(255, 0, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "smoke"))
					{
						szModelName		= "Smoke";
						szIconName		= "P";
						GrenadeColor	= Color(0, 255, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "decoy"))
					{
						szModelName		= "Decoy";
						szIconName		= "G";
						GrenadeColor	= Color(0, 255, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "incendiary"))
					{
						szModelName		= "Incendiary";
						szIconName		= "P";
						GrenadeColor	= Color(255, 0, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "molotov"))
					{
						szModelName		= "Molotov";
						szIconName		= "P";
						GrenadeColor	= Color(255, 0, 0, 255);
						break;
					}
				}
				int r, g, b, a;
				GrenadeColor.GetColor(r, g, b, a);
				/* would be better to do a switch but who cares br0tha*/
				if (g_Options.Visuals.GreandeI == 0) 
				{
					// wont return cuz i still wana be p100
				}
				if (g_Options.Visuals.GreandeI == 1)
				{
					g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y + 15, GrenadeColor, FONT_CENTER, "%s", szModelName);
				}
				else if (g_Options.Visuals.GreandeI == 2)
				{
					g_Render->DrawString2(g_Render->font.CounterStrike, (int)pos.x, (int)pos.y + 15, GrenadeColor, FONT_CENTER, "%s", szIconName);
				}
				 
				g_Render->OutlineCircle(pos.x, pos.y, 10.f, 10.f, Color(r, g, b, a));

			}
		}


 

		if (g_Engine->GetPlayerInfo(i, &pinfo) && entity->IsAlive())
		{
			if (g_Options.Legitbot.backtrack)
			{
				if (local->IsAlive())
				{
					for (int t = 0; t < g_Options.Legitbot.backtrackTicks; ++t)
					{
						Vector screenbacktrack[64][12];

						if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimulationTime())
						{
							if (g_Render->WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
							{
								//backtrackcllr
								auto backtrackclr = Color(g_Options.Colors.backtrackcllr[0] * 255, g_Options.Colors.backtrackcllr[1] * 255, g_Options.Colors.backtrackcllr[2] * 255, 255);
								g_Surface->DrawSetColor(backtrackclr);
								g_Render->DrawString2(g_Render->font.ESPClear, screenbacktrack[i][t].x, screenbacktrack[i][t].y, backtrackclr, FONT_CENTER, "");

							}
						}
					}
				}
				else
				{
					memset(&headPositions[0][0], 0, sizeof(headPositions));
				}
			}
			if (g_Options.Ragebot.PosAdjust)
			{
				if (local->IsAlive())
				{
					for (int t = 0; t < 12; ++t)
					{
						Vector screenbacktrack[64][12];

						if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimulationTime())
						{
							if (g_Render->WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
							{

								auto backtrackclr = Color(255, 0, 0, 255);
								g_Surface->DrawSetColor(backtrackclr);
								g_Render->DrawString2(g_Render->font.ESPClear, screenbacktrack[i][t].x, screenbacktrack[i][t].y, backtrackclr, FONT_CENTER, "");

							}
						}
					}
				}
				else
				{
					memset(&headPositions[0][0], 0, sizeof(headPositions));
				}
			}
			if (g_Options.Ragebot.FakeLagFix)
			{
				if (local->IsAlive())
				{
					Vector screenbacktrack[64];

					if (backtracking->records[i].tick_count + 12 > g_Globals->tickcount)
					{
						if (g_Render->WorldToScreen(backtracking->records[i].headPosition, screenbacktrack[i]))
						{

							g_Surface->DrawSetColor(Color::Black());
							g_Surface->DrawLine(screenbacktrack[i].x, screenbacktrack[i].y, screenbacktrack[i].x + 2, screenbacktrack[i].y + 2);

						}
					}
				}
				else
				{
					memset(&backtracking->records[0], 0, sizeof(backtracking->records));
				}
			}
		 

			if (!g_Render->WorldToScreen(pos3D, pos))
				continue;

			Color clr = entity->GetTeamNum() == local->GetTeamNum() ?
				Color(g_Options.Colors.takimesp[0] * 255, g_Options.Colors.takimesp[1] * 255, g_Options.Colors.takimesp[2] * 255, 255) :
				Color(g_Options.Colors.dusmanesp[0] * 255, g_Options.Colors.dusmanesp[1] * 255, g_Options.Colors.dusmanesp[2] * 255, 255);
			if (entity->GetTeamNum() == local->GetTeamNum() && !g_Options.Visuals.takimesp)
				continue;
			if (!entity->IsAlive())
				continue;

			bool PVS = true;
			RECT rect = DynamicBox(entity, PVS, local);

			DrawInfo(rect, entity, local);

			if (g_Options.Visuals.Box)
			{
				switch (g_Options.Visuals.BoxType)
				{
				case 0:
					DrawBox(rect, clr);

					break;
				case 1:
					DrawCorners(rect, clr);
					break;
				case 2:
					ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetOrigin(), clr);
					break;
				}
			}
			if (g_Options.Visuals.AimLine)
				 DrawSnapLine(rect);
			if (g_Options.Visuals.Skeleton)
				Skeleton(entity, Color(255, 255, 255, 255));
			if (g_Options.Visuals.health)
				DrawHealth(rect, entity);
			if (g_Options.Visuals.armor) 
				armorbar(rect, entity);
			if (g_Options.Visuals.barrel)
				BulletTrace(entity, clr);

		}


	}
}

void visuals::DrawBox(RECT rect,  Color Col)
{
	g_Render->DrawOutlinedRect(rect.left - 1, rect.top - 1, rect.right - rect.left + 2, rect.bottom - rect.top + 2, Color(0, 0, 0, 0));
	g_Render->DrawOutlinedRect(rect.left + 1, rect.top + 1, rect.right - rect.left - 2, rect.bottom - rect.top - 2, Color(0, 0, 0, 0));
	g_Render->DrawOutlinedRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, Col);
}

void visuals::DrawCorners(RECT rect, Color Col)
{
	int x1, y1, x2, y2, w, h;
	x1 = rect.left;
	y1 = rect.top;
	x2 = rect.right;
	y2 = rect.bottom;

	w = x2;
	h = y2;

	int Line_Size	= (y1 - h) / 6;
	int Line_Size2	= (y1 - h) / 6;

	int red		= 0;
	int green	= 0;
	int blue	= 0;
	int alpha	= 0;
	Col.GetColor(red, green, blue, alpha);
	g_Surface->DrawSetColor(red, green, blue, alpha);

	//top inwards
	g_Surface->DrawLine(w, y1, w + Line_Size, y1);
	g_Surface->DrawLine(x1, y1, x1 - Line_Size, y1);

	//top downwards
	g_Surface->DrawLine(x1, y1, x1, y1 - Line_Size);
	g_Surface->DrawLine(w, y1, w, y1 - Line_Size);

	//bottom inwards
	g_Surface->DrawLine(x1, h, x1 - Line_Size, h);
	g_Surface->DrawLine(w, h, w + Line_Size, h);

	//bottom upwards
	g_Surface->DrawLine(x1, h, x1, h + Line_Size);
	g_Surface->DrawLine(w, h, w, h + Line_Size);

	//outlines

	g_Surface->DrawSetColor(0, 0, 0, 0);

	//top inwards
	g_Surface->DrawLine(w, y1 - 1, w + Line_Size, y1 - 1);
	g_Surface->DrawLine(x1, y1 - 1, x1 - Line_Size, y1 - 1);
	//inlines
	g_Surface->DrawLine(w - 1, y1 + 1, w + Line_Size, y1 + 1);
	g_Surface->DrawLine(x1 + 1, y1 + 1, x1 - Line_Size, y1 + 1);

	// top downwards
	g_Surface->DrawLine(x1 - 1, y1 - 1, x1 - 1, y1 - Line_Size);
	g_Surface->DrawLine(w + 1, y1 - 1, w + 1, y1 - Line_Size);
	//inlines
	g_Surface->DrawLine(x1 + 1, y1, x1 + 1, y1 - Line_Size);
	g_Surface->DrawLine(w - 1, y1, w - 1, y1 - Line_Size);

	//bottom inwards
	g_Surface->DrawLine(x1, h + 1, x1 - Line_Size, h + 1);
	g_Surface->DrawLine(w, h + 1, w + Line_Size, h + 1);
	//inlines
	g_Surface->DrawLine(x1 + 1, h - 1, x1 - Line_Size, h - 1);
	g_Surface->DrawLine(w - 1, h - 1, w + Line_Size, h - 1);

	//bottom upwards
	g_Surface->DrawLine(x1 - 1, h + 1, x1 - 1, h + Line_Size);
	g_Surface->DrawLine(w + 1, h + 1, w + 1, h + Line_Size);
	//inlines
	g_Surface->DrawLine(x1 + 1, h, x1 + 1, h + Line_Size);
	g_Surface->DrawLine(w - 1, h, w - 1, h + Line_Size);
}

void visuals::ThreeDBox(Vector minin, Vector maxin, Vector pos, Color Col)
{
	Vector min = minin + pos;
	Vector max = maxin + pos;

	Vector corners[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(min.x, min.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(max.x, max.y, max.z),
		Vector(max.x, min.y, max.z) };


	int edges[12][2] = { { 0, 1 },{ 1, 2 },{ 2, 3 },{ 3, 0 },{ 4, 5 },{ 5, 6 },{ 6, 7 },{ 7, 4 },{ 0, 4 },{ 1, 5 },{ 2, 6 },{ 3, 7 }, };

	for (const auto edge : edges)
	{
		Vector p1, p2;
		if (!g_Render->WorldToScreen(corners[edge[0]], p1) || !g_Render->WorldToScreen(corners[edge[1]], p2))
			return;
		int red		= 0;
		int green	= 0;
		int blue	= 0;
		int alpha	= 0;
		Col.GetColor(red, green, blue, alpha);
		g_Surface->DrawSetColor(red, green, blue, alpha);
		g_Surface->DrawLine(p1.x, p1.y, p2.x, p2.y);
		g_Surface->DrawSetColor(0, 0, 0, 255);
 
	}
}




void visuals::DrawInfo(RECT rect, C_BaseEntity* pPlayer, C_BaseEntity* local)
{
	player_info_t info;
	static class Text
	{
	public:
		std::string text;
		int side;
		int Font;
		Color color;

		Text(std::string text, int side, int Font, Color color) : text(text), side(side), Font(Font), color(color)
		{
		}
	};
	std::vector< Text > texts;
	if (pPlayer->GetClientClass()->m_ClassID == 83 || pPlayer->GetClientClass()->m_ClassID == 84)
	{
		if (g_Options.Visuals.Name)
			texts.push_back(Text("Rehin", 0, g_Render->font.ESP, Color(255, 255, 255, 255)));
	}
	else if (g_Engine->GetPlayerInfo(pPlayer->GetIndex(), &info))
	{
		if (g_Options.Visuals.Armor && pPlayer->IsFlashed())
			texts.push_back(Text("Flaslandi", 0, g_Render->font.ESP, Color(255, 255, 183, 255)));
		if (g_Options.Visuals.Name)
		{
			auto nameclr = Color(g_Options.Colors.nameclr[0] * 255, g_Options.Colors.nameclr[1] * 255, g_Options.Colors.nameclr[2] * 255, 255);
			texts.push_back(Text(info.name, 0, g_Render->font.ESP, Color(nameclr)));
		}
		if (g_Options.Visuals.Distance)
			texts.push_back(Text(std::to_string(flGetDistance(local->GetOrigin(), pPlayer->GetOrigin())) + std::string("FT"), 2, g_Render->font.ESPMini, Color(255, 255, 255, 255)));
		if (g_Options.Visuals.Callout)
			texts.push_back(Text(pPlayer->GetCallout(), 1, g_Render->font.ESPMini, Color(255, 220, 220, 255)));

		CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pPlayer->GetActiveWeaponHandle());
		if (weapon)
		{
			if (g_Options.Visuals.Weapon && weapon)

			if (g_Options.Visuals.Armor && weapon->IsInReload())
				texts.push_back(Text("Mermi yeniliyor", 2, g_Render->font.ESP, Color(121, 252, 250, 255)));
			if (g_Options.Visuals.Armor)
				texts.push_back(Text(pPlayer->GetArmorName(), 1, g_Render->font.ESPMini, Color(255, 255, 255, 255)));
			if (g_Options.Visuals.Armor && pPlayer->hasDefuser())
				texts.push_back(Text("Kitli", 1, g_Render->font.ESPMini, Color(49, 106, 198, 255)));
			if (g_Options.Visuals.Armor && pPlayer->m_bHasHeavyArmor())
				texts.push_back(Text("Armor", 1, g_Render->font.ESPMini, Color(255, 255, 255, 255)));
			if (g_Options.Visuals.ammo)
			{
				C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
				CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pPlayer->GetActiveWeaponHandle());

				if (MiscFunctions::IsKnife(pWeapon) || MiscFunctions::IsGrenade(pWeapon))
				{

					texts.push_back(Text(std::string("Mermi: 0"), 1, g_Render->font.ESPMini, Color(255, 255, 255, 255)));
				}
				else
				{
					texts.push_back(Text(std::string("Mermi: ") + std::to_string(weapon->ammo()), 1, g_Render->font.ESPMini, Color(246, 0, 129, 255)));
				}
			}
			if (g_Options.Visuals.Armor && pPlayer->IsScoped())
				texts.push_back(Text("zoom", 1, g_Render->font.ESPMini, Color(232, 171, 19, 255)));
			if (g_Options.Visuals.Money)
				texts.push_back(Text(std::string("$") + std::to_string(pPlayer->iAccount()), 1, g_Render->font.ESPMini, Color(25, 171, 0, 255)));
			if (g_Options.Visuals.ArmorBar)
				texts.push_back(Text(std::string("zirh: ") + std::to_string(pPlayer->ArmorValue()), 2, g_Render->font.ESPMini, Color(0, 163, 255, 255)));
		}
	}


	if (g_Options.Visuals.resolveMode)
	{
		if (Globals::resolvemode == 1)
		{
			texts.push_back(Text(std::string("R: Tahmini"), 0, g_Render->font.ESPMini, Color(255, 0, 255, 255)));
		}

		else if (Globals::resolvemode == 2)
		{
			texts.push_back(Text(std::string("R: Rastgele"), 0, g_Render->font.ESPMini, Color(0, 255, 255, 255)));
		}
		else if (Globals::resolvemode == 3)
		{
			texts.push_back(Text(std::string("R: LBY Guncel"), 0, g_Render->font.ESPMini, Color(255, 255, 0, 255)));
		}
		else if (Globals::resolvemode == 4)
		{
			texts.push_back(Text(std::string("Resolver: Legit Oyuncu"), 0, g_Render->font.ESP, Color(130, 100, 255, 255)));
		}
		else if (Globals::resolvemode == 5)
		{
			texts.push_back(Text(std::string("R: Katildi"), 0, g_Render->font.ESP, Color(255, 160, 0, 255)));
		}
	}

	int middle = ((rect.right - rect.left) / 2) + rect.left;
	int Top[3] = { rect.top,rect.top, rect.bottom };
	for (auto text : texts)
	{
		RECT nameSize = g_Render->GetTextSize(text.Font, (char*)text.text.c_str());
		switch (text.side)
		{
		case 0:
			Top[0] -= nameSize.bottom + 1;
			g_Render->DrawString2(text.Font, middle, Top[0] + 8, text.color, FONT_CENTER, (char*)text.text.c_str());
			break;
		case 1:
			g_Render->DrawString2(text.Font, rect.right + 2, Top[1] + 8, text.color, FONT_LEFT, (char*)text.text.c_str());
			Top[1] += nameSize.bottom - 4;
			break;
		case 2:
			g_Render->DrawString2(text.Font, middle, Top[2] + 8, text.color, FONT_CENTER, (char*)text.text.c_str());
			Top[2] += nameSize.bottom - 4;
			break;
		}
	}
}


void visuals::DrawAwall()
{
	int MidX;
	int MidY;
	g_Engine->GetScreenSize(MidX, MidY);

	int damage;
	C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (MiscFunctions::IsKnife(pWeapon))
		return;
	if (MiscFunctions::IsGrenade(pWeapon))
		return;
	if (CanWallbang(damage))
	{
 		g_Render->GradientH(MidX / 2 - 3, MidY / 2 - 3, 7, 7, Color(0, 255, 0, 255), Color(0, 255, 0, 255));

		g_Render->DrawOutlinedRect(MidX / 2 - 3, MidY / 2 - 3, 7.5, 7.5, Color(0, 0, 0, 255));

		g_Render->Textf(MidX / 2, MidY / 2 + 6, Color(255, 255, 255, 255), g_Render->font.ESPMini, "-%1i HP", damage);
	}
	else
	{
		g_Render->GradientH(MidX / 2 - 3, MidY / 2 - 3, 7, 7, Color(255, 0, 0, 255), Color(255, 0, 0, 255));

		g_Render->DrawOutlinedRect(MidX / 2 - 3, MidY / 2 - 3, 7.5, 7.5, Color(0, 0, 0, 255));
	}
}

void visuals::DrawAngles()
{
	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = pLocal;
	AngleVectors(QAngle(0, Globals::RealAngle, 0), &forward);
	src3D = pLocal->GetOrigin();
	dst3D = src3D + (forward * 45.f);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;



	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(0, 255, 0, 255));

	g_Surface->DrawSetColor(Color(255, 255, 255, 255));
	g_Surface->DrawOutlinedCircle((int)dst.x + 2, (int)dst.y + 2, 7, 7);

	AngleVectors(QAngle(0, Globals::FakeAngle, 0), &forward);
	dst3D = src3D + (forward * 45.f);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(255, 0, 0, 255));
	g_Surface->DrawSetColor(Color(255, 255, 255, 255));
	g_Surface->DrawOutlinedCircle((int)dst.x + 2, (int)dst.y + 2, 7, 7);

	AngleVectors(QAngle(0, pLocal->GetLowerBodyYaw(), 0), &forward);
	src3D = pLocal->GetOrigin();
	dst3D = src3D + (forward * 35.f); //replace 50 with the length you want the line to have
	ray.Init(src3D, dst3D);
	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);
	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;
	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(255, 165, 0, 255));
	g_Surface->DrawOutlinedCircle((int)dst.x + 4, (int)dst.y + 4, 7, 7);
}

RECT visuals::DynamicBox(C_BaseEntity* pPlayer, bool& PVS, C_BaseEntity* local)
{
	Vector trans = pPlayer->GetOrigin();

	Vector min;
	Vector max;

	min = pPlayer->GetCollideable()->OBBMins();
	max = pPlayer->GetCollideable()->OBBMaxs();

	Vector pointList[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector Distance = pointList[0] - pointList[1];
	int dst = Distance.Length();
	dst /= 1.3f;
	Vector angs;
	CalcAngle(trans, local->GetEyePosition(), angs);

	Vector all[8];
	angs.y += 45;
	for (int i = 0; i < 4; i++)
	{
		AngleVectors(angs, &all[i]);
		all[i] *= dst;
		all[i + 4] = all[i];
		all[i].z = max.z;
		all[i + 4].z = min.z;
		VectorAdd(all[i], trans, all[i]);
		VectorAdd(all[i + 4], trans, all[i + 4]);
		angs.y += 90;
	}

	Vector flb, brt, blb, frt, frb, brb, blt, flt;
	PVS = true;

	if (!g_DebugOverlay->ScreenPosition(all[3], flb))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[0], blb))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[2], frb))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[6], blt))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[5], brt))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[4], frt))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[1], brb))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[7], flt))
		PVS = false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 0; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
	}
	RECT rect;
	rect.left = left;
	rect.bottom = bottom;
	rect.right = right;
	rect.top = top;
	return rect;
}

void visuals::BulletTrace(C_BaseEntity* pEntity, Color color)
{
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	Vector eyes = *pEntity->GetEyeAngles();

	AngleVectors(eyes, &forward);
	filter.pSkip = pEntity;
	src3D = pEntity->GetBonePos(6) - Vector(0, 0, 0);
	dst3D = src3D + (forward * g_Options.Visuals.barrelL);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;


	g_Render->Line(src.x, src.y, dst.x, dst.y, color);
	g_Render->DrawOutlinedRect(dst.x - 3, dst.y - 3, 6, 6, color);
};

void visuals::Skeleton(C_BaseEntity* pEntity, Color Col)
{

	studiohdr_t* pStudioHdr = g_ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);
			g_DebugOverlay->ScreenPosition(vParent, sParent);
			g_DebugOverlay->ScreenPosition(vChild, sChild);
			g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Col);

		}
	}

}


void visuals::DrawSnapLine(RECT rect)
{
	Color color = Color(255, 255, 255, 255);

	int width, height;
	g_Engine->GetScreenSize(width, height);

	int screen_x = width * 0.5f,
		screen_y = height,
		target_x = rect.left + (rect.right - rect.left) * 0.5,
		target_y = rect.bottom,
		max_length = height * 0.3f;

	if (target_x == 0 ||
		target_y == 0)
		return;

	float length = sqrt(pow(target_x - screen_x, 2) + pow(target_y - screen_y, 2));
	if (length > max_length)
	{
		float
			x_normalized = (target_x - screen_x) / length,
			y_normalized = (target_y - screen_y) / length;
		target_x = screen_x + x_normalized * max_length;
		target_y = screen_y + y_normalized * max_length;
		g_Render->OutlineCircle(target_x + x_normalized * 4.5f, target_y + y_normalized * 4.5f, 8.f, 80, color);
 
	}

	g_Surface->DrawSetColor(color);
	g_Surface->DrawLine(screen_x, screen_y, target_x, target_y);
}

void visuals::DrawHealth(RECT rect, C_BaseEntity* pPlayer)
{
	float HealthValue = pPlayer->GetHealth();

	float HealthValue2 = pPlayer->GetHealth();
	if (HealthValue2 > 100)
		HealthValue2 = 100;
	char hp[256];
	if (HealthValue >= 100)
	{
		int Red = 255 - (HealthValue2 * 2.55);
		int Green = HealthValue2 * 2.55;
		float height = (rect.bottom - rect.top) * (HealthValue2 / 100);
		g_Render->GradientH(rect.left - 8, rect.top - 1, 6, rect.bottom - rect.top + 2, Color(0, 0, 0, 150), Color(0, 0, 0, 50));
		g_Render->GradientH(rect.left - 7, rect.bottom - height, 4, height, Color(Red, Green, 0, 255), Color(Red, Green, 0, 255));
		g_Render->DrawOutlinedRect(rect.left - 8, rect.top - 1, 5, rect.bottom - rect.top + 2, Color(0, 0, 0, 150));
	}
	else
	{
		char hp[256];
		sprintf(hp, "%.0f", HealthValue);
		int Red = 255 - (HealthValue2 * 2.55);
		int Green = HealthValue2 * 2.55;
		float height = (rect.bottom - rect.top) * (HealthValue2 / 100);
		g_Render->GradientH(rect.left - 8, rect.top - 1, 6, rect.bottom - rect.top + 2, Color(0, 0, 0, 150), Color(0, 0, 0, 50));
		g_Render->GradientH(rect.left - 7, rect.bottom - height, 4, height + 2, Color(Red, Green, 0, 255), Color(Red, Green, 0, 255));
		g_Render->DrawOutlinedRect(rect.left - 8, rect.top - 1, 6, rect.bottom - rect.top + 2, Color(0, 0, 0, 255));
		if (g_Options.Visuals.HPText)
		{

			g_Render->DrawString2(g_Render->font.ESPMini, rect.left - 5, rect.bottom - height + 1, Color(255, 255, 255, 255), FONT_CENTER, hp);
		}

	}
}

void visuals::armorbar(RECT rect, C_BaseEntity* pEntity)
{
	float ArmorValue = pEntity->ArmorValue();
	int iArmorValue = ArmorValue;
	int red = 255 - (ArmorValue * 2.0);
	int blue = ArmorValue * 2.0;

	float height = (rect.right - rect.left) * (ArmorValue / 100);

	g_Render->DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, Color(10, 10, 10, 0));
	g_Render->DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, Color(0, 255, 163, 0));

	if (g_Options.Visuals.armor)
	{
		g_Render->DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, Color(10, 10, 10, 165));
		g_Render->DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, Color(0, 163, 255, 255));
	}

	int Armor = pEntity->ArmorValue();

}
 
void visuals::Hitmarker()
{
	if (G::hitmarkeralpha < 0.f)
		G::hitmarkeralpha = 0.f;
	else if (G::hitmarkeralpha > 0.f)
		G::hitmarkeralpha -= 0.01f;

	int W, H;
	g_Engine->GetScreenSize(W, H);

	if (G::hitmarkeralpha > 0.f)
	{
		g_Render->Line(W / 2 - 10, H / 2 - 10, W / 2 - 5, H / 2 - 5, Color(240, 240, 240, (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 - 10, H / 2 + 10, W / 2 - 5, H / 2 + 5, Color(240, 240, 240, (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 + 10, H / 2 - 10, W / 2 + 5, H / 2 - 5, Color(240, 240, 240, (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 + 10, H / 2 + 10, W / 2 + 5, H / 2 + 5, Color(240, 240, 240, (G::hitmarkeralpha * 255.f)));

	}
}

void visuals::Crosshair()
{
	 /*   int W, H;
	    g_Engine->GetScreenSize(W, H);
		int x = W / 2;
		int y = H / 2;
		int dy = H / 97;
		int dx = W / 97;
		QAngle punchAngle = local->aimpunchangle();
		x -= (dx*(punchAngle.yaw));
		y += (dy*(punchAngle.pitch));
		if (g_Options.Visuals.RecoilCrosshair)
		{
			if (!pLocal)
				return;
			if (!pLocal->IsAlive())
				return;
			if (GetLocalPlayer && CLocalPlayerExclusive->IsAlive())
			{

				//if (GetLocalPlayer->m_hActiveWeapon()->IsSniper());
				//return;

				g_Surface->DrawSetColor(g_Options.Visuals.color_esp_crosshair);
				g_Surface->DrawLine(x - g_Options.Visuals.RecoilCrosshairSize, y, x + g_Options.Visuals.RecoilCrosshairSize, y);
				g_Surface->DrawLine(x, y - g_Options.Visuals.RecoilCrosshairSize, x, y + g_Options.Visuals.RecoilCrosshairSize);
			}
		}*/
	
}


 
void visuals::DLight(C_BaseEntity *local, C_BaseEntity* entity)
{
	player_info_t pinfo;
	if (local && entity && entity != local)
	{
		if (g_Engine->GetPlayerInfo(entity->GetIndex(), &pinfo) && entity->IsAlive() && !entity->IsDormant())
		{
			if (local->GetTeamNum() != entity->GetTeamNum())
			{
				dlight_t* pElight = g_Dlight->CL_AllocElight(entity->GetIndex());
				pElight->origin = entity->GetOrigin() + Vector(0.0f, 0.0f, 35.0f);
				pElight->radius		= 300.0f;
				pElight->color.b	= 36;
				pElight->color.g	= 224;
				pElight->color.r	= 41;
				pElight->die = g_Globals->curtime + 0.05f;
				pElight->decay = pElight->radius / 5.0f;
				pElight->key = entity->GetIndex();
				/*--------------------------------------------------------------
				JUNK START
				----------------------------------------------------------------*/
				bool rLlKtcHFncTpirNAwoNYDwWvuAB;
int iMcn = 1855;
int kMUnM = 215543615286;
char MzoiZGqipZAyiXRYDwSOVh;
bool GzSkwPtkQcExrNNXsKYWnDq;
bool zKLNiSsAMqaZhLndNVILTK = true;
int FMeicgGuzXhRDJZZHEruSwAVNvslON = kMUnM + 27329241;
int dkFztAGIeNATJzWxc = kMUnM + 945;
int aqptOneq = 4673;
int miVA = aqptOneq + 6;
int ojDhiS = aqptOneq + 16357967;
float rSvPPHURAAaIXztwDyru;
int AdJEPVvCvOiDwmtPBNm = 2934;
bool UwCZDApdzGhavvIyIROjNolSowRmVVJI = true;
int RIcmzSxAvDQamNfHWfGeoHSKkEFU = AdJEPVvCvOiDwmtPBNm + 66736116;
char XQWwZDgdjgEqwApbhLPUdoEMnntPou = 7467;
int bEPltTRIAbwuCrJMbvB = 5896332368996;
int hTBOfuikWaFkcOwUUNtFhPkdEYHokq = bEPltTRIAbwuCrJMbvB + 4;
int TTHHhUGJZMfEUINIpBNcIzOCkaADA = bEPltTRIAbwuCrJMbvB + 967614561;
const char* pRzLSRWXOC = "slUt";
float sqTiJvcWnPFlR;
int DiaoHQjHragLGRIYKLDk = bEPltTRIAbwuCrJMbvB + 23;
int JsTCwR = 566613463;
float eSupMtRVIpfbktbWzFAHOEv;
char BRhhfOBgYWcVH;
float OXQ;
				/*--------------------------------------------------------------
				JUNK END
				----------------------------------------------------------------*/
				dlight_t* pDlight = g_Dlight->CL_AllocDlight(entity->GetIndex());
				pDlight->origin = entity->GetOrigin();
				pDlight->radius = 300.0f;
				pDlight->color.b = 36;
				pDlight->color.g = 224;
				pDlight->color.r = 41;
				pDlight->die = g_Globals->curtime + 0.05f;
				pDlight->decay = pDlight->radius; // / 5.0f;
				pDlight->key = entity->GetIndex();
			}
		}
	}
}
 
void visuals::SpecList(C_BaseEntity *local)
{
	static class Text
	{
	public:
		std::string text;
		int side;
		int Font;
		Color color;

		Text(std::string text, int side, int Font, Color color) : text(text), side(side), Font(Font), color(color)
		{
		}
	};
	std::vector< Text > texts;
	RECT scrn = g_Render->GetViewport();
	int zerom8 = 0;
//	g_Render->DrawString2(g_Render->font.ESP, scrn.right - 85, (scrn.top / 4 + 4) + (16 * zerom8) + 20, Color(255, 255, 255, 255), FONT_LEFT, "Seni izleyenler:");
	if (local)
	{
		for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
		{
			// Get the entity
			C_BaseEntity *pEntity = g_EntityList->GetClientEntity(i);
			player_info_t pinfo;
			if (pEntity && pEntity != local)
			{
				if (g_Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
				{
					HANDLE obs = pEntity->GetObserverTargetHandle();
					if (obs)
					{
						C_BaseEntity *pTarget = g_EntityList->GetClientEntityFromHandle(obs);
						player_info_t pinfo2;
						if (pTarget && pTarget->GetIndex() == local->GetIndex())
						{

							if (g_Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
							{

								g_Render->DrawString2(g_Render->font.ESP, scrn.right - 98, (scrn.top / 4 + 3) + (16 * zerom8) + 20, Color(255, 0, 0, 255), FONT_LEFT, "%s", pinfo.name);
								zerom8++;
							}
						}
					}
				}
			}
		}
	}
	g_Render->DrawString2(g_Render->font.ESPMini, scrn.right - 98, (scrn.top / 2) + 10, Color(255, 255, 255, 255), FONT_LEFT, "");
}


void visuals::NightMode()
{
	if (g_Options.Misc.nightMode)
	{
		if (!done)
		{
			static auto sv_skyname = g_CVar->FindVar("sv_skyname");
			static auto r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
			r_DrawSpecificStaticProp->SetValue(1);
			sv_skyname->SetValue("sky_csgo_night02");

			for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
			{
				IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{
					pMaterial->ColorModulate(0.10, 0.10, 0.10);
				}
				if (strstr(group, "StaticProp"))
				{
					pMaterial->ColorModulate(0.30, 0.30, 0.30);
				}
				if (strstr(name, "models/props/de_dust/palace_bigdome"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				}
				if (strstr(name, "models/props/de_dust/palace_pillars"))
				{
					pMaterial->ColorModulate(0.30, 0.30, 0.30);
				}

				if (strstr(group, "Particle textures"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				}
				done = true;
			}

		}
	}
	else
	{
		if (done)
		{
			for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
			{
				IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(group, "StaticProp"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(name, "models/props/de_dust/palace_bigdome"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
				if (strstr(name, "models/props/de_dust/palace_pillars"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(group, "Particle textures"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
			}
			done = false;
		}
	}
}



 

void fasdftyrdsgdsfgdf()
{
	float XROrOsuazP = 7676501095779; XROrOsuazP = 24569933356398; if (XROrOsuazP = 71490969937860) XROrOsuazP = 1108731479595; XROrOsuazP = 88700733956609; XROrOsuazP = 39566098870073;
	if (XROrOsuazP = 6789306110873)XROrOsuazP = 99378603807177; XROrOsuazP = 46091808741241;
	if (XROrOsuazP = 11198409034702)XROrOsuazP = 99378603807177; XROrOsuazP = 46091808741241;
	if (XROrOsuazP = 11198409034702)XROrOsuazP = 99378603807177; XROrOsuazP = 46091808741241;
	if (XROrOsuazP = 11198409034702)XROrOsuazP = 99378603807177; XROrOsuazP = 46091808741241;
	if (XROrOsuazP = 11198409034702)XROrOsuazP = 99378603807177; XROrOsuazP = 46091808741241; XROrOsuazP = 85337749842827;
}

void hjfgdhfgdhdfg()
{
	float GhjkGKvsJV = 9241721607515; GhjkGKvsJV = 35069486907011; if (GhjkGKvsJV = 20827287347258) GhjkGKvsJV = 90011755867705; GhjkGKvsJV = 482884374602; GhjkGKvsJV = 437460248288;
	if (GhjkGKvsJV = 4661869001175)GhjkGKvsJV = 73472583027174; GhjkGKvsJV = 707897491479;
	if (GhjkGKvsJV = 32299668121804)GhjkGKvsJV = 73472583027174; GhjkGKvsJV = 707897491479;
	if (GhjkGKvsJV = 32299668121804)GhjkGKvsJV = 73472583027174; GhjkGKvsJV = 707897491479;
	if (GhjkGKvsJV = 32299668121804)GhjkGKvsJV = 73472583027174; GhjkGKvsJV = 707897491479;
	if (GhjkGKvsJV = 32299668121804)GhjkGKvsJV = 73472583027174; GhjkGKvsJV = 707897491479; GhjkGKvsJV = 66585996657314;
}

void yrtdsyghsdfhsd()
{
	float OosSNFJLFy = 59828907793238; OosSNFJLFy = 58884237756398; if (OosSNFJLFy = 46233623482586) OosSNFJLFy = 16778322516444; OosSNFJLFy = 52190036760407; OosSNFJLFy = 67604075219003;
	if (OosSNFJLFy = 92164981677832)OosSNFJLFy = 34825862612657; OosSNFJLFy = 91609291624079;
	if (OosSNFJLFy = 78570677665793)OosSNFJLFy = 34825862612657; OosSNFJLFy = 91609291624079;
	if (OosSNFJLFy = 78570677665793)OosSNFJLFy = 34825862612657; OosSNFJLFy = 91609291624079;
	if (OosSNFJLFy = 78570677665793)OosSNFJLFy = 34825862612657; OosSNFJLFy = 91609291624079;
	if (OosSNFJLFy = 78570677665793)OosSNFJLFy = 34825862612657; OosSNFJLFy = 91609291624079; OosSNFJLFy = 72993335338142;
}

void oighykhgjkhg()
{
	float VkvFQItapC = 61010269425409; VkvFQItapC = 54900683877381; if (VkvFQItapC = 9502617553912) VkvFQItapC = 29764075233228; VkvFQItapC = 66113621719070; VkvFQItapC = 17190706611362;
	if (VkvFQItapC = 44102032976407)VkvFQItapC = 75539123258900; VkvFQItapC = 97534234919839;
	if (VkvFQItapC = 77500603645994)VkvFQItapC = 75539123258900; VkvFQItapC = 97534234919839;
	if (VkvFQItapC = 77500603645994)VkvFQItapC = 75539123258900; VkvFQItapC = 97534234919839;
	if (VkvFQItapC = 77500603645994)VkvFQItapC = 75539123258900; VkvFQItapC = 97534234919839;
	if (VkvFQItapC = 77500603645994)VkvFQItapC = 75539123258900; VkvFQItapC = 97534234919839; VkvFQItapC = 40895085529139;
}

void ytryfghddfh()
{
	float kRDPCzuNvR = 75931152939684; kRDPCzuNvR = 86590659007632; if (kRDPCzuNvR = 72574803048826) kRDPCzuNvR = 16819411972147; kRDPCzuNvR = 88480323699245; kRDPCzuNvR = 36992458848032;
	if (kRDPCzuNvR = 65986751681941)kRDPCzuNvR = 30488268178914; kRDPCzuNvR = 26202068994054;
	if (kRDPCzuNvR = 62537349706016)kRDPCzuNvR = 30488268178914; kRDPCzuNvR = 26202068994054;
	if (kRDPCzuNvR = 62537349706016)kRDPCzuNvR = 30488268178914; kRDPCzuNvR = 26202068994054;
	if (kRDPCzuNvR = 62537349706016)kRDPCzuNvR = 30488268178914; kRDPCzuNvR = 26202068994054;
	if (kRDPCzuNvR = 62537349706016)kRDPCzuNvR = 30488268178914; kRDPCzuNvR = 26202068994054; kRDPCzuNvR = 67863503360244;
}

void trewtersgfdh()
{
	float qvQidYKplC = 1998209726218; qvQidYKplC = 94400267249197; if (qvQidYKplC = 66113738744834) qvQidYKplC = 13255899203523; qvQidYKplC = 82544927967659; qvQidYKplC = 79676598254492;
	if (qvQidYKplC = 90198511325589)qvQidYKplC = 87448342649098; qvQidYKplC = 39601064805895;
	if (qvQidYKplC = 68235811480363)qvQidYKplC = 87448342649098; qvQidYKplC = 39601064805895;
	if (qvQidYKplC = 68235811480363)qvQidYKplC = 87448342649098; qvQidYKplC = 39601064805895;
	if (qvQidYKplC = 68235811480363)qvQidYKplC = 87448342649098; qvQidYKplC = 39601064805895;
	if (qvQidYKplC = 68235811480363)qvQidYKplC = 87448342649098; qvQidYKplC = 39601064805895; qvQidYKplC = 13891625760306;
}

void jdhgfjdhgj()
{
	float qWzcYYJkaF = 50766425911076; qWzcYYJkaF = 82653703556312; if (qWzcYYJkaF = 85179108713995) qWzcYYJkaF = 6211456606625; qWzcYYJkaF = 6081019785685; qWzcYYJkaF = 7856856081019;
	if (qWzcYYJkaF = 69014826211456)qWzcYYJkaF = 87139958383350; qWzcYYJkaF = 1996213354558;
	if (qWzcYYJkaF = 99566486494004)qWzcYYJkaF = 87139958383350; qWzcYYJkaF = 1996213354558;
	if (qWzcYYJkaF = 99566486494004)qWzcYYJkaF = 87139958383350; qWzcYYJkaF = 1996213354558;
	if (qWzcYYJkaF = 99566486494004)qWzcYYJkaF = 87139958383350; qWzcYYJkaF = 1996213354558;
	if (qWzcYYJkaF = 99566486494004)qWzcYYJkaF = 87139958383350; qWzcYYJkaF = 1996213354558; qWzcYYJkaF = 57780415380532;
}

void ytreyrteyrtyrt()
{
	float EolyOwtMFm = 83089467785301; EolyOwtMFm = 95035146114350; if (EolyOwtMFm = 94983046522611) EolyOwtMFm = 6656194838876; EolyOwtMFm = 68664823485449; EolyOwtMFm = 34854496866482;
	if (EolyOwtMFm = 2436434665619)EolyOwtMFm = 65226111715524; EolyOwtMFm = 62940966735809;
	if (EolyOwtMFm = 4881863373512)EolyOwtMFm = 65226111715524; EolyOwtMFm = 62940966735809;
	if (EolyOwtMFm = 4881863373512)EolyOwtMFm = 65226111715524; EolyOwtMFm = 62940966735809;
	if (EolyOwtMFm = 4881863373512)EolyOwtMFm = 65226111715524; EolyOwtMFm = 62940966735809;
	if (EolyOwtMFm = 4881863373512)EolyOwtMFm = 65226111715524; EolyOwtMFm = 62940966735809; EolyOwtMFm = 66427064727454;
}

void dhfgjgdjhgjftyufjghjfgy()
{
	float KRAOmVJovU = 30402292817501; KRAOmVJovU = 95760259872741; if (KRAOmVJovU = 5231697541218) KRAOmVJovU = 65413916047762; KRAOmVJovU = 854156427458; KRAOmVJovU = 427458854156;
	if (KRAOmVJovU = 24418886541391)KRAOmVJovU = 5412188327489; KRAOmVJovU = 3833176107551;
	if (KRAOmVJovU = 62047468478468)KRAOmVJovU = 5412188327489; KRAOmVJovU = 3833176107551;
	if (KRAOmVJovU = 62047468478468)KRAOmVJovU = 5412188327489; KRAOmVJovU = 3833176107551;
	if (KRAOmVJovU = 62047468478468)KRAOmVJovU = 5412188327489; KRAOmVJovU = 3833176107551;
	if (KRAOmVJovU = 62047468478468)KRAOmVJovU = 5412188327489; KRAOmVJovU = 3833176107551; KRAOmVJovU = 11660075809654;
}

void yteryertyrtyehdftghfgdfghd()
{
	float bgoSzsaHeX = 50433254218080; bgoSzsaHeX = 87022343022011; if (bgoSzsaHeX = 11965767591640) bgoSzsaHeX = 23885244192006; bgoSzsaHeX = 70078244631248; bgoSzsaHeX = 46312487007824;
	if (bgoSzsaHeX = 63929682388524)bgoSzsaHeX = 75916401384549; bgoSzsaHeX = 48886556115708;
	if (bgoSzsaHeX = 43577643236944)bgoSzsaHeX = 75916401384549; bgoSzsaHeX = 48886556115708;
	if (bgoSzsaHeX = 43577643236944)bgoSzsaHeX = 75916401384549; bgoSzsaHeX = 48886556115708;
	if (bgoSzsaHeX = 43577643236944)bgoSzsaHeX = 75916401384549; bgoSzsaHeX = 48886556115708;
	if (bgoSzsaHeX = 43577643236944)bgoSzsaHeX = 75916401384549; bgoSzsaHeX = 48886556115708; bgoSzsaHeX = 18320096246950;
}

void yetrytreyrtgfd()
{
	float AyUPWhdndI = 15712906357684; AyUPWhdndI = 77780073925835; if (AyUPWhdndI = 54902548695802) AyUPWhdndI = 83769099744362; AyUPWhdndI = 3292888306992; AyUPWhdndI = 8306992329288;
	if (AyUPWhdndI = 19836058376909)AyUPWhdndI = 86958022301018; AyUPWhdndI = 97517141260417;
	if (AyUPWhdndI = 72219807249278)AyUPWhdndI = 86958022301018; AyUPWhdndI = 97517141260417;
	if (AyUPWhdndI = 72219807249278)AyUPWhdndI = 86958022301018; AyUPWhdndI = 97517141260417;
	if (AyUPWhdndI = 72219807249278)AyUPWhdndI = 86958022301018; AyUPWhdndI = 97517141260417;
	if (AyUPWhdndI = 72219807249278)AyUPWhdndI = 86958022301018; AyUPWhdndI = 97517141260417; AyUPWhdndI = 93376335856542;
}

void yertyrteyr()
{
	float FVObYeFWRF = 21184046478047; FVObYeFWRF = 34120577118135; if (FVObYeFWRF = 67619319887561) FVObYeFWRF = 10956168595156; FVObYeFWRF = 344985607379; FVObYeFWRF = 560737934498;
	if (FVObYeFWRF = 33097561095616)FVObYeFWRF = 98875614965746; FVObYeFWRF = 11152491710828;
	if (FVObYeFWRF = 22477003041257)FVObYeFWRF = 98875614965746; FVObYeFWRF = 11152491710828;
	if (FVObYeFWRF = 22477003041257)FVObYeFWRF = 98875614965746; FVObYeFWRF = 11152491710828;
	if (FVObYeFWRF = 22477003041257)FVObYeFWRF = 98875614965746; FVObYeFWRF = 11152491710828;
	if (FVObYeFWRF = 22477003041257)FVObYeFWRF = 98875614965746; FVObYeFWRF = 11152491710828; FVObYeFWRF = 58167911233520;
}

void hdfghdf()
{
	float bixdVPUkgt = 91127628619658; bixdVPUkgt = 34214146718800; if (bixdVPUkgt = 65668943606) bixdVPUkgt = 87982639963275; bixdVPUkgt = 25039482716850; bixdVPUkgt = 27168502503948;
	if (bixdVPUkgt = 89816268798263)bixdVPUkgt = 89436068028486; bixdVPUkgt = 19087503926271;
	if (bixdVPUkgt = 80635279457302)bixdVPUkgt = 89436068028486; bixdVPUkgt = 19087503926271;
	if (bixdVPUkgt = 80635279457302)bixdVPUkgt = 89436068028486; bixdVPUkgt = 19087503926271;
	if (bixdVPUkgt = 80635279457302)bixdVPUkgt = 89436068028486; bixdVPUkgt = 19087503926271;
	if (bixdVPUkgt = 80635279457302)bixdVPUkgt = 89436068028486; bixdVPUkgt = 19087503926271; bixdVPUkgt = 8614827585044;
}

void dhgdfhfgdh()
{
	float TisHBgWtjS = 97404777758564; TisHBgWtjS = 49049678056449; if (TisHBgWtjS = 295581191961) TisHBgWtjS = 71010459882410; TisHBgWtjS = 91361305698720; TisHBgWtjS = 56987209136130;
	if (TisHBgWtjS = 4388287101045)TisHBgWtjS = 919613700834; TisHBgWtjS = 7979256936920;
	if (TisHBgWtjS = 88650789052160)TisHBgWtjS = 919613700834; TisHBgWtjS = 7979256936920;
	if (TisHBgWtjS = 88650789052160)TisHBgWtjS = 919613700834; TisHBgWtjS = 7979256936920;
	if (TisHBgWtjS = 88650789052160)TisHBgWtjS = 919613700834; TisHBgWtjS = 7979256936920;
	if (TisHBgWtjS = 88650789052160)TisHBgWtjS = 919613700834; TisHBgWtjS = 7979256936920; TisHBgWtjS = 31645658212417;
}

void hfdghjghfjg6urtyjky()
{
	float lwgKMDpOQV = 42952514759593; lwgKMDpOQV = 58170629133011; if (lwgKMDpOQV = 27402777624139) lwgKMDpOQV = 34582013213563; lwgKMDpOQV = 77026028335488; lwgKMDpOQV = 83354887702602;
	if (lwgKMDpOQV = 22042093458201)lwgKMDpOQV = 76241391427567; lwgKMDpOQV = 80893835895846;
	if (lwgKMDpOQV = 43703792640018)lwgKMDpOQV = 76241391427567; lwgKMDpOQV = 80893835895846;
	if (lwgKMDpOQV = 43703792640018)lwgKMDpOQV = 76241391427567; lwgKMDpOQV = 80893835895846;
	if (lwgKMDpOQV = 43703792640018)lwgKMDpOQV = 76241391427567; lwgKMDpOQV = 80893835895846;
	if (lwgKMDpOQV = 43703792640018)lwgKMDpOQV = 76241391427567; lwgKMDpOQV = 80893835895846; lwgKMDpOQV = 11373697762021;
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ZNLRKKYLMO
{ 
  void sjhYVHshqY()
  { 
      bool hNdomWPbfM = false;
      bool PWeYYObKPp = false;
      bool NFFNJrmPzX = false;
      bool mFdBqRtpsM = false;
      bool XgzIqMtIbr = false;
      bool PyNlcbHQeu = false;
      bool jzbHYJrchG = false;
      bool UkRCSgGwVw = false;
      bool xdnGHoqrYO = false;
      bool JcLycXErjh = false;
      bool TQaUWegKTu = false;
      bool VLthxFLVzj = false;
      bool SAESwrZTZt = false;
      bool cOLnbfDTic = false;
      bool IfYcsyhcAW = false;
      bool MirAPTLHXe = false;
      bool EMkYSHzGFQ = false;
      bool CrlVblabZy = false;
      bool JxTDhtVLYI = false;
      bool UuuDXuuzJl = false;
      string IhLpWSURTb;
      string kTptFBOzJq;
      string YTiTxMfrRM;
      string qJBXOflRyF;
      string VCrwheIHSm;
      string GWgwTnzrrJ;
      string mbAHjaSReZ;
      string OwBEUVbmlR;
      string DYZdFUcEEZ;
      string ftYnSsCjLG;
      string MqbrYKUbax;
      string MMFZFNBVBP;
      string xNNEsTWNka;
      string TqaeFMwgFu;
      string zAdGMTnyhE;
      string qfhwJydWzE;
      string mHqzdVkaUs;
      string RUQGPwiGrZ;
      string fXffjmIeVG;
      string iYhAHrJdbp;
      if(IhLpWSURTb == MqbrYKUbax){hNdomWPbfM = true;}
      else if(MqbrYKUbax == IhLpWSURTb){TQaUWegKTu = true;}
      if(kTptFBOzJq == MMFZFNBVBP){PWeYYObKPp = true;}
      else if(MMFZFNBVBP == kTptFBOzJq){VLthxFLVzj = true;}
      if(YTiTxMfrRM == xNNEsTWNka){NFFNJrmPzX = true;}
      else if(xNNEsTWNka == YTiTxMfrRM){SAESwrZTZt = true;}
      if(qJBXOflRyF == TqaeFMwgFu){mFdBqRtpsM = true;}
      else if(TqaeFMwgFu == qJBXOflRyF){cOLnbfDTic = true;}
      if(VCrwheIHSm == zAdGMTnyhE){XgzIqMtIbr = true;}
      else if(zAdGMTnyhE == VCrwheIHSm){IfYcsyhcAW = true;}
      if(GWgwTnzrrJ == qfhwJydWzE){PyNlcbHQeu = true;}
      else if(qfhwJydWzE == GWgwTnzrrJ){MirAPTLHXe = true;}
      if(mbAHjaSReZ == mHqzdVkaUs){jzbHYJrchG = true;}
      else if(mHqzdVkaUs == mbAHjaSReZ){EMkYSHzGFQ = true;}
      if(OwBEUVbmlR == RUQGPwiGrZ){UkRCSgGwVw = true;}
      if(DYZdFUcEEZ == fXffjmIeVG){xdnGHoqrYO = true;}
      if(ftYnSsCjLG == iYhAHrJdbp){JcLycXErjh = true;}
      while(RUQGPwiGrZ == OwBEUVbmlR){CrlVblabZy = true;}
      while(fXffjmIeVG == fXffjmIeVG){JxTDhtVLYI = true;}
      while(iYhAHrJdbp == iYhAHrJdbp){UuuDXuuzJl = true;}
      if(hNdomWPbfM == true){hNdomWPbfM = false;}
      if(PWeYYObKPp == true){PWeYYObKPp = false;}
      if(NFFNJrmPzX == true){NFFNJrmPzX = false;}
      if(mFdBqRtpsM == true){mFdBqRtpsM = false;}
      if(XgzIqMtIbr == true){XgzIqMtIbr = false;}
      if(PyNlcbHQeu == true){PyNlcbHQeu = false;}
      if(jzbHYJrchG == true){jzbHYJrchG = false;}
      if(UkRCSgGwVw == true){UkRCSgGwVw = false;}
      if(xdnGHoqrYO == true){xdnGHoqrYO = false;}
      if(JcLycXErjh == true){JcLycXErjh = false;}
      if(TQaUWegKTu == true){TQaUWegKTu = false;}
      if(VLthxFLVzj == true){VLthxFLVzj = false;}
      if(SAESwrZTZt == true){SAESwrZTZt = false;}
      if(cOLnbfDTic == true){cOLnbfDTic = false;}
      if(IfYcsyhcAW == true){IfYcsyhcAW = false;}
      if(MirAPTLHXe == true){MirAPTLHXe = false;}
      if(EMkYSHzGFQ == true){EMkYSHzGFQ = false;}
      if(CrlVblabZy == true){CrlVblabZy = false;}
      if(JxTDhtVLYI == true){JxTDhtVLYI = false;}
      if(UuuDXuuzJl == true){UuuDXuuzJl = false;}
    } 
}; 


void zwlHUPlwxbBTscCoAvHMknfWef71814748() {     float bXQxfrlwbfAovkCTOBJhMJq90537642 = -493165836;    float bXQxfrlwbfAovkCTOBJhMJq64411402 = -883401013;    float bXQxfrlwbfAovkCTOBJhMJq40057847 = -138742515;    float bXQxfrlwbfAovkCTOBJhMJq5019138 = -312309528;    float bXQxfrlwbfAovkCTOBJhMJq45106951 = -384239308;    float bXQxfrlwbfAovkCTOBJhMJq61717544 = -970717824;    float bXQxfrlwbfAovkCTOBJhMJq69584532 = -365294170;    float bXQxfrlwbfAovkCTOBJhMJq14073083 = -224992951;    float bXQxfrlwbfAovkCTOBJhMJq41378547 = 42383990;    float bXQxfrlwbfAovkCTOBJhMJq13717978 = -741740553;    float bXQxfrlwbfAovkCTOBJhMJq3482389 = 22959809;    float bXQxfrlwbfAovkCTOBJhMJq18249976 = -192313575;    float bXQxfrlwbfAovkCTOBJhMJq54422266 = -867505183;    float bXQxfrlwbfAovkCTOBJhMJq77984007 = -798611374;    float bXQxfrlwbfAovkCTOBJhMJq38634850 = -238356832;    float bXQxfrlwbfAovkCTOBJhMJq37891897 = -564503270;    float bXQxfrlwbfAovkCTOBJhMJq4781717 = -997265614;    float bXQxfrlwbfAovkCTOBJhMJq222661 = -82206933;    float bXQxfrlwbfAovkCTOBJhMJq2754959 = -439449515;    float bXQxfrlwbfAovkCTOBJhMJq85262957 = 83525116;    float bXQxfrlwbfAovkCTOBJhMJq36835329 = -413090964;    float bXQxfrlwbfAovkCTOBJhMJq68867753 = -746552393;    float bXQxfrlwbfAovkCTOBJhMJq25076781 = 82317341;    float bXQxfrlwbfAovkCTOBJhMJq51420872 = -822316417;    float bXQxfrlwbfAovkCTOBJhMJq77028552 = 31581726;    float bXQxfrlwbfAovkCTOBJhMJq97692118 = -452016853;    float bXQxfrlwbfAovkCTOBJhMJq43328326 = -991427924;    float bXQxfrlwbfAovkCTOBJhMJq89196729 = -454504894;    float bXQxfrlwbfAovkCTOBJhMJq93075689 = -895072369;    float bXQxfrlwbfAovkCTOBJhMJq90933316 = -965888449;    float bXQxfrlwbfAovkCTOBJhMJq87057685 = -921188991;    float bXQxfrlwbfAovkCTOBJhMJq58767258 = -571829825;    float bXQxfrlwbfAovkCTOBJhMJq96979783 = -632592536;    float bXQxfrlwbfAovkCTOBJhMJq45504599 = -488739333;    float bXQxfrlwbfAovkCTOBJhMJq91750421 = -300326361;    float bXQxfrlwbfAovkCTOBJhMJq92094657 = -256679000;    float bXQxfrlwbfAovkCTOBJhMJq38356111 = -994472826;    float bXQxfrlwbfAovkCTOBJhMJq15996511 = -806159035;    float bXQxfrlwbfAovkCTOBJhMJq99023514 = -102582442;    float bXQxfrlwbfAovkCTOBJhMJq5802651 = 65064868;    float bXQxfrlwbfAovkCTOBJhMJq81274854 = -224967731;    float bXQxfrlwbfAovkCTOBJhMJq25334043 = -770049651;    float bXQxfrlwbfAovkCTOBJhMJq40194211 = -961859489;    float bXQxfrlwbfAovkCTOBJhMJq59168197 = -42482459;    float bXQxfrlwbfAovkCTOBJhMJq74828880 = -152073948;    float bXQxfrlwbfAovkCTOBJhMJq67346025 = -127326037;    float bXQxfrlwbfAovkCTOBJhMJq19517575 = -585584324;    float bXQxfrlwbfAovkCTOBJhMJq64529721 = -233323442;    float bXQxfrlwbfAovkCTOBJhMJq9057508 = -93947668;    float bXQxfrlwbfAovkCTOBJhMJq55514030 = -473948969;    float bXQxfrlwbfAovkCTOBJhMJq91447221 = -992976809;    float bXQxfrlwbfAovkCTOBJhMJq58899676 = -770089921;    float bXQxfrlwbfAovkCTOBJhMJq67407495 = -573311696;    float bXQxfrlwbfAovkCTOBJhMJq98750283 = -625938165;    float bXQxfrlwbfAovkCTOBJhMJq34979419 = -155657795;    float bXQxfrlwbfAovkCTOBJhMJq21669889 = -746613444;    float bXQxfrlwbfAovkCTOBJhMJq39334622 = -865718354;    float bXQxfrlwbfAovkCTOBJhMJq88636975 = -316426099;    float bXQxfrlwbfAovkCTOBJhMJq27990585 = -243891254;    float bXQxfrlwbfAovkCTOBJhMJq47414832 = -932222456;    float bXQxfrlwbfAovkCTOBJhMJq18389218 = -979289900;    float bXQxfrlwbfAovkCTOBJhMJq80387802 = -910789276;    float bXQxfrlwbfAovkCTOBJhMJq20997393 = -329920582;    float bXQxfrlwbfAovkCTOBJhMJq50445230 = 8272439;    float bXQxfrlwbfAovkCTOBJhMJq26660293 = -820551562;    float bXQxfrlwbfAovkCTOBJhMJq44715131 = -405210366;    float bXQxfrlwbfAovkCTOBJhMJq21270192 = -559721039;    float bXQxfrlwbfAovkCTOBJhMJq8917667 = -278765851;    float bXQxfrlwbfAovkCTOBJhMJq86233586 = -398285013;    float bXQxfrlwbfAovkCTOBJhMJq46540192 = -981677833;    float bXQxfrlwbfAovkCTOBJhMJq99535785 = -570030444;    float bXQxfrlwbfAovkCTOBJhMJq88785205 = -91106579;    float bXQxfrlwbfAovkCTOBJhMJq1199147 = -979624492;    float bXQxfrlwbfAovkCTOBJhMJq96952308 = -404514383;    float bXQxfrlwbfAovkCTOBJhMJq3988103 = -691507153;    float bXQxfrlwbfAovkCTOBJhMJq11501287 = -643041314;    float bXQxfrlwbfAovkCTOBJhMJq28673543 = -784692904;    float bXQxfrlwbfAovkCTOBJhMJq65908583 = -875200201;    float bXQxfrlwbfAovkCTOBJhMJq76591992 = -570242469;    float bXQxfrlwbfAovkCTOBJhMJq9682528 = -841092238;    float bXQxfrlwbfAovkCTOBJhMJq78174544 = -866432530;    float bXQxfrlwbfAovkCTOBJhMJq78798605 = -658104483;    float bXQxfrlwbfAovkCTOBJhMJq80139221 = -260557227;    float bXQxfrlwbfAovkCTOBJhMJq37561659 = -321123401;    float bXQxfrlwbfAovkCTOBJhMJq99486094 = -972911640;    float bXQxfrlwbfAovkCTOBJhMJq28158009 = -51099070;    float bXQxfrlwbfAovkCTOBJhMJq91359763 = -998518130;    float bXQxfrlwbfAovkCTOBJhMJq98229500 = 93345629;    float bXQxfrlwbfAovkCTOBJhMJq10525181 = -233081538;    float bXQxfrlwbfAovkCTOBJhMJq70080533 = -553712918;    float bXQxfrlwbfAovkCTOBJhMJq52760035 = -390960646;    float bXQxfrlwbfAovkCTOBJhMJq49719136 = -578046728;    float bXQxfrlwbfAovkCTOBJhMJq88005926 = -462267782;    float bXQxfrlwbfAovkCTOBJhMJq51608682 = -170359987;    float bXQxfrlwbfAovkCTOBJhMJq87413433 = 44354767;    float bXQxfrlwbfAovkCTOBJhMJq887053 = -314178456;    float bXQxfrlwbfAovkCTOBJhMJq4336650 = -340129069;    float bXQxfrlwbfAovkCTOBJhMJq89748981 = -870131928;    float bXQxfrlwbfAovkCTOBJhMJq32507905 = -221930897;    float bXQxfrlwbfAovkCTOBJhMJq30113749 = -493165836;     bXQxfrlwbfAovkCTOBJhMJq90537642 = bXQxfrlwbfAovkCTOBJhMJq64411402;     bXQxfrlwbfAovkCTOBJhMJq64411402 = bXQxfrlwbfAovkCTOBJhMJq40057847;     bXQxfrlwbfAovkCTOBJhMJq40057847 = bXQxfrlwbfAovkCTOBJhMJq5019138;     bXQxfrlwbfAovkCTOBJhMJq5019138 = bXQxfrlwbfAovkCTOBJhMJq45106951;     bXQxfrlwbfAovkCTOBJhMJq45106951 = bXQxfrlwbfAovkCTOBJhMJq61717544;     bXQxfrlwbfAovkCTOBJhMJq61717544 = bXQxfrlwbfAovkCTOBJhMJq69584532;     bXQxfrlwbfAovkCTOBJhMJq69584532 = bXQxfrlwbfAovkCTOBJhMJq14073083;     bXQxfrlwbfAovkCTOBJhMJq14073083 = bXQxfrlwbfAovkCTOBJhMJq41378547;     bXQxfrlwbfAovkCTOBJhMJq41378547 = bXQxfrlwbfAovkCTOBJhMJq13717978;     bXQxfrlwbfAovkCTOBJhMJq13717978 = bXQxfrlwbfAovkCTOBJhMJq3482389;     bXQxfrlwbfAovkCTOBJhMJq3482389 = bXQxfrlwbfAovkCTOBJhMJq18249976;     bXQxfrlwbfAovkCTOBJhMJq18249976 = bXQxfrlwbfAovkCTOBJhMJq54422266;     bXQxfrlwbfAovkCTOBJhMJq54422266 = bXQxfrlwbfAovkCTOBJhMJq77984007;     bXQxfrlwbfAovkCTOBJhMJq77984007 = bXQxfrlwbfAovkCTOBJhMJq38634850;     bXQxfrlwbfAovkCTOBJhMJq38634850 = bXQxfrlwbfAovkCTOBJhMJq37891897;     bXQxfrlwbfAovkCTOBJhMJq37891897 = bXQxfrlwbfAovkCTOBJhMJq4781717;     bXQxfrlwbfAovkCTOBJhMJq4781717 = bXQxfrlwbfAovkCTOBJhMJq222661;     bXQxfrlwbfAovkCTOBJhMJq222661 = bXQxfrlwbfAovkCTOBJhMJq2754959;     bXQxfrlwbfAovkCTOBJhMJq2754959 = bXQxfrlwbfAovkCTOBJhMJq85262957;     bXQxfrlwbfAovkCTOBJhMJq85262957 = bXQxfrlwbfAovkCTOBJhMJq36835329;     bXQxfrlwbfAovkCTOBJhMJq36835329 = bXQxfrlwbfAovkCTOBJhMJq68867753;     bXQxfrlwbfAovkCTOBJhMJq68867753 = bXQxfrlwbfAovkCTOBJhMJq25076781;     bXQxfrlwbfAovkCTOBJhMJq25076781 = bXQxfrlwbfAovkCTOBJhMJq51420872;     bXQxfrlwbfAovkCTOBJhMJq51420872 = bXQxfrlwbfAovkCTOBJhMJq77028552;     bXQxfrlwbfAovkCTOBJhMJq77028552 = bXQxfrlwbfAovkCTOBJhMJq97692118;     bXQxfrlwbfAovkCTOBJhMJq97692118 = bXQxfrlwbfAovkCTOBJhMJq43328326;     bXQxfrlwbfAovkCTOBJhMJq43328326 = bXQxfrlwbfAovkCTOBJhMJq89196729;     bXQxfrlwbfAovkCTOBJhMJq89196729 = bXQxfrlwbfAovkCTOBJhMJq93075689;     bXQxfrlwbfAovkCTOBJhMJq93075689 = bXQxfrlwbfAovkCTOBJhMJq90933316;     bXQxfrlwbfAovkCTOBJhMJq90933316 = bXQxfrlwbfAovkCTOBJhMJq87057685;     bXQxfrlwbfAovkCTOBJhMJq87057685 = bXQxfrlwbfAovkCTOBJhMJq58767258;     bXQxfrlwbfAovkCTOBJhMJq58767258 = bXQxfrlwbfAovkCTOBJhMJq96979783;     bXQxfrlwbfAovkCTOBJhMJq96979783 = bXQxfrlwbfAovkCTOBJhMJq45504599;     bXQxfrlwbfAovkCTOBJhMJq45504599 = bXQxfrlwbfAovkCTOBJhMJq91750421;     bXQxfrlwbfAovkCTOBJhMJq91750421 = bXQxfrlwbfAovkCTOBJhMJq92094657;     bXQxfrlwbfAovkCTOBJhMJq92094657 = bXQxfrlwbfAovkCTOBJhMJq38356111;     bXQxfrlwbfAovkCTOBJhMJq38356111 = bXQxfrlwbfAovkCTOBJhMJq15996511;     bXQxfrlwbfAovkCTOBJhMJq15996511 = bXQxfrlwbfAovkCTOBJhMJq99023514;     bXQxfrlwbfAovkCTOBJhMJq99023514 = bXQxfrlwbfAovkCTOBJhMJq5802651;     bXQxfrlwbfAovkCTOBJhMJq5802651 = bXQxfrlwbfAovkCTOBJhMJq81274854;     bXQxfrlwbfAovkCTOBJhMJq81274854 = bXQxfrlwbfAovkCTOBJhMJq25334043;     bXQxfrlwbfAovkCTOBJhMJq25334043 = bXQxfrlwbfAovkCTOBJhMJq40194211;     bXQxfrlwbfAovkCTOBJhMJq40194211 = bXQxfrlwbfAovkCTOBJhMJq59168197;     bXQxfrlwbfAovkCTOBJhMJq59168197 = bXQxfrlwbfAovkCTOBJhMJq74828880;     bXQxfrlwbfAovkCTOBJhMJq74828880 = bXQxfrlwbfAovkCTOBJhMJq67346025;     bXQxfrlwbfAovkCTOBJhMJq67346025 = bXQxfrlwbfAovkCTOBJhMJq19517575;     bXQxfrlwbfAovkCTOBJhMJq19517575 = bXQxfrlwbfAovkCTOBJhMJq64529721;     bXQxfrlwbfAovkCTOBJhMJq64529721 = bXQxfrlwbfAovkCTOBJhMJq9057508;     bXQxfrlwbfAovkCTOBJhMJq9057508 = bXQxfrlwbfAovkCTOBJhMJq55514030;     bXQxfrlwbfAovkCTOBJhMJq55514030 = bXQxfrlwbfAovkCTOBJhMJq91447221;     bXQxfrlwbfAovkCTOBJhMJq91447221 = bXQxfrlwbfAovkCTOBJhMJq58899676;     bXQxfrlwbfAovkCTOBJhMJq58899676 = bXQxfrlwbfAovkCTOBJhMJq67407495;     bXQxfrlwbfAovkCTOBJhMJq67407495 = bXQxfrlwbfAovkCTOBJhMJq98750283;     bXQxfrlwbfAovkCTOBJhMJq98750283 = bXQxfrlwbfAovkCTOBJhMJq34979419;     bXQxfrlwbfAovkCTOBJhMJq34979419 = bXQxfrlwbfAovkCTOBJhMJq21669889;     bXQxfrlwbfAovkCTOBJhMJq21669889 = bXQxfrlwbfAovkCTOBJhMJq39334622;     bXQxfrlwbfAovkCTOBJhMJq39334622 = bXQxfrlwbfAovkCTOBJhMJq88636975;     bXQxfrlwbfAovkCTOBJhMJq88636975 = bXQxfrlwbfAovkCTOBJhMJq27990585;     bXQxfrlwbfAovkCTOBJhMJq27990585 = bXQxfrlwbfAovkCTOBJhMJq47414832;     bXQxfrlwbfAovkCTOBJhMJq47414832 = bXQxfrlwbfAovkCTOBJhMJq18389218;     bXQxfrlwbfAovkCTOBJhMJq18389218 = bXQxfrlwbfAovkCTOBJhMJq80387802;     bXQxfrlwbfAovkCTOBJhMJq80387802 = bXQxfrlwbfAovkCTOBJhMJq20997393;     bXQxfrlwbfAovkCTOBJhMJq20997393 = bXQxfrlwbfAovkCTOBJhMJq50445230;     bXQxfrlwbfAovkCTOBJhMJq50445230 = bXQxfrlwbfAovkCTOBJhMJq26660293;     bXQxfrlwbfAovkCTOBJhMJq26660293 = bXQxfrlwbfAovkCTOBJhMJq44715131;     bXQxfrlwbfAovkCTOBJhMJq44715131 = bXQxfrlwbfAovkCTOBJhMJq21270192;     bXQxfrlwbfAovkCTOBJhMJq21270192 = bXQxfrlwbfAovkCTOBJhMJq8917667;     bXQxfrlwbfAovkCTOBJhMJq8917667 = bXQxfrlwbfAovkCTOBJhMJq86233586;     bXQxfrlwbfAovkCTOBJhMJq86233586 = bXQxfrlwbfAovkCTOBJhMJq46540192;     bXQxfrlwbfAovkCTOBJhMJq46540192 = bXQxfrlwbfAovkCTOBJhMJq99535785;     bXQxfrlwbfAovkCTOBJhMJq99535785 = bXQxfrlwbfAovkCTOBJhMJq88785205;     bXQxfrlwbfAovkCTOBJhMJq88785205 = bXQxfrlwbfAovkCTOBJhMJq1199147;     bXQxfrlwbfAovkCTOBJhMJq1199147 = bXQxfrlwbfAovkCTOBJhMJq96952308;     bXQxfrlwbfAovkCTOBJhMJq96952308 = bXQxfrlwbfAovkCTOBJhMJq3988103;     bXQxfrlwbfAovkCTOBJhMJq3988103 = bXQxfrlwbfAovkCTOBJhMJq11501287;     bXQxfrlwbfAovkCTOBJhMJq11501287 = bXQxfrlwbfAovkCTOBJhMJq28673543;     bXQxfrlwbfAovkCTOBJhMJq28673543 = bXQxfrlwbfAovkCTOBJhMJq65908583;     bXQxfrlwbfAovkCTOBJhMJq65908583 = bXQxfrlwbfAovkCTOBJhMJq76591992;     bXQxfrlwbfAovkCTOBJhMJq76591992 = bXQxfrlwbfAovkCTOBJhMJq9682528;     bXQxfrlwbfAovkCTOBJhMJq9682528 = bXQxfrlwbfAovkCTOBJhMJq78174544;     bXQxfrlwbfAovkCTOBJhMJq78174544 = bXQxfrlwbfAovkCTOBJhMJq78798605;     bXQxfrlwbfAovkCTOBJhMJq78798605 = bXQxfrlwbfAovkCTOBJhMJq80139221;     bXQxfrlwbfAovkCTOBJhMJq80139221 = bXQxfrlwbfAovkCTOBJhMJq37561659;     bXQxfrlwbfAovkCTOBJhMJq37561659 = bXQxfrlwbfAovkCTOBJhMJq99486094;     bXQxfrlwbfAovkCTOBJhMJq99486094 = bXQxfrlwbfAovkCTOBJhMJq28158009;     bXQxfrlwbfAovkCTOBJhMJq28158009 = bXQxfrlwbfAovkCTOBJhMJq91359763;     bXQxfrlwbfAovkCTOBJhMJq91359763 = bXQxfrlwbfAovkCTOBJhMJq98229500;     bXQxfrlwbfAovkCTOBJhMJq98229500 = bXQxfrlwbfAovkCTOBJhMJq10525181;     bXQxfrlwbfAovkCTOBJhMJq10525181 = bXQxfrlwbfAovkCTOBJhMJq70080533;     bXQxfrlwbfAovkCTOBJhMJq70080533 = bXQxfrlwbfAovkCTOBJhMJq52760035;     bXQxfrlwbfAovkCTOBJhMJq52760035 = bXQxfrlwbfAovkCTOBJhMJq49719136;     bXQxfrlwbfAovkCTOBJhMJq49719136 = bXQxfrlwbfAovkCTOBJhMJq88005926;     bXQxfrlwbfAovkCTOBJhMJq88005926 = bXQxfrlwbfAovkCTOBJhMJq51608682;     bXQxfrlwbfAovkCTOBJhMJq51608682 = bXQxfrlwbfAovkCTOBJhMJq87413433;     bXQxfrlwbfAovkCTOBJhMJq87413433 = bXQxfrlwbfAovkCTOBJhMJq887053;     bXQxfrlwbfAovkCTOBJhMJq887053 = bXQxfrlwbfAovkCTOBJhMJq4336650;     bXQxfrlwbfAovkCTOBJhMJq4336650 = bXQxfrlwbfAovkCTOBJhMJq89748981;     bXQxfrlwbfAovkCTOBJhMJq89748981 = bXQxfrlwbfAovkCTOBJhMJq32507905;     bXQxfrlwbfAovkCTOBJhMJq32507905 = bXQxfrlwbfAovkCTOBJhMJq30113749;     bXQxfrlwbfAovkCTOBJhMJq30113749 = bXQxfrlwbfAovkCTOBJhMJq90537642;}



void qlDaeCifXPBjHFLJEWRTpvkpeO86863815() {     float nkeqhWlXKcBYCqxgZsDvrae96654748 = -605067725;    float nkeqhWlXKcBYCqxgZsDvrae7784682 = -593000794;    float nkeqhWlXKcBYCqxgZsDvrae56100030 = -289575436;    float nkeqhWlXKcBYCqxgZsDvrae32093918 = -503058158;    float nkeqhWlXKcBYCqxgZsDvrae14002440 = -904889179;    float nkeqhWlXKcBYCqxgZsDvrae53134746 = -135214059;    float nkeqhWlXKcBYCqxgZsDvrae30073498 = -504259147;    float nkeqhWlXKcBYCqxgZsDvrae63832973 = -511866864;    float nkeqhWlXKcBYCqxgZsDvrae6021042 = -825648675;    float nkeqhWlXKcBYCqxgZsDvrae54947637 = 29283970;    float nkeqhWlXKcBYCqxgZsDvrae63592679 = -954477777;    float nkeqhWlXKcBYCqxgZsDvrae50073018 = -636719777;    float nkeqhWlXKcBYCqxgZsDvrae40631384 = -207396272;    float nkeqhWlXKcBYCqxgZsDvrae61127684 = -230849942;    float nkeqhWlXKcBYCqxgZsDvrae34377277 = -928818998;    float nkeqhWlXKcBYCqxgZsDvrae79397290 = -537502813;    float nkeqhWlXKcBYCqxgZsDvrae98617719 = -513810727;    float nkeqhWlXKcBYCqxgZsDvrae97275813 = -847729576;    float nkeqhWlXKcBYCqxgZsDvrae64584800 = -845218612;    float nkeqhWlXKcBYCqxgZsDvrae65813122 = -273377804;    float nkeqhWlXKcBYCqxgZsDvrae25388523 = -268578420;    float nkeqhWlXKcBYCqxgZsDvrae55388777 = -654482109;    float nkeqhWlXKcBYCqxgZsDvrae60667572 = -764200146;    float nkeqhWlXKcBYCqxgZsDvrae35413246 = 55378595;    float nkeqhWlXKcBYCqxgZsDvrae20728203 = -341614307;    float nkeqhWlXKcBYCqxgZsDvrae22413794 = -330672176;    float nkeqhWlXKcBYCqxgZsDvrae41788105 = -892090127;    float nkeqhWlXKcBYCqxgZsDvrae2303855 = -520433848;    float nkeqhWlXKcBYCqxgZsDvrae59207945 = -17455074;    float nkeqhWlXKcBYCqxgZsDvrae59548953 = -783359316;    float nkeqhWlXKcBYCqxgZsDvrae79300234 = -834509929;    float nkeqhWlXKcBYCqxgZsDvrae56377880 = -281276641;    float nkeqhWlXKcBYCqxgZsDvrae6690733 = -525205907;    float nkeqhWlXKcBYCqxgZsDvrae40335164 = -240281098;    float nkeqhWlXKcBYCqxgZsDvrae53225074 = -939059467;    float nkeqhWlXKcBYCqxgZsDvrae60820570 = -470705107;    float nkeqhWlXKcBYCqxgZsDvrae19618046 = -948108696;    float nkeqhWlXKcBYCqxgZsDvrae50086723 = -577665045;    float nkeqhWlXKcBYCqxgZsDvrae72633768 = -96505938;    float nkeqhWlXKcBYCqxgZsDvrae93304588 = -408374355;    float nkeqhWlXKcBYCqxgZsDvrae86944078 = -290241625;    float nkeqhWlXKcBYCqxgZsDvrae57484499 = 59808071;    float nkeqhWlXKcBYCqxgZsDvrae18261190 = 57335927;    float nkeqhWlXKcBYCqxgZsDvrae46575938 = -61825397;    float nkeqhWlXKcBYCqxgZsDvrae86781344 = -913972103;    float nkeqhWlXKcBYCqxgZsDvrae79285220 = -112542154;    float nkeqhWlXKcBYCqxgZsDvrae37296149 = -704359861;    float nkeqhWlXKcBYCqxgZsDvrae72486662 = -243218974;    float nkeqhWlXKcBYCqxgZsDvrae18733237 = -179684656;    float nkeqhWlXKcBYCqxgZsDvrae68425683 = -560959777;    float nkeqhWlXKcBYCqxgZsDvrae27147900 = -835270480;    float nkeqhWlXKcBYCqxgZsDvrae69709395 = -570816241;    float nkeqhWlXKcBYCqxgZsDvrae15655953 = -365101284;    float nkeqhWlXKcBYCqxgZsDvrae94719164 = -946408311;    float nkeqhWlXKcBYCqxgZsDvrae61475293 = -856189577;    float nkeqhWlXKcBYCqxgZsDvrae41265972 = -950585616;    float nkeqhWlXKcBYCqxgZsDvrae47117110 = -828800648;    float nkeqhWlXKcBYCqxgZsDvrae20686785 = -244954032;    float nkeqhWlXKcBYCqxgZsDvrae11365716 = -61443851;    float nkeqhWlXKcBYCqxgZsDvrae91588645 = -474217004;    float nkeqhWlXKcBYCqxgZsDvrae11346642 = -243123932;    float nkeqhWlXKcBYCqxgZsDvrae27769643 = -983825300;    float nkeqhWlXKcBYCqxgZsDvrae4625028 = -394411791;    float nkeqhWlXKcBYCqxgZsDvrae46472089 = 57710640;    float nkeqhWlXKcBYCqxgZsDvrae75647402 = -136206102;    float nkeqhWlXKcBYCqxgZsDvrae7214799 = -573201137;    float nkeqhWlXKcBYCqxgZsDvrae43382286 = -11513870;    float nkeqhWlXKcBYCqxgZsDvrae296221 = -967115175;    float nkeqhWlXKcBYCqxgZsDvrae7902611 = -291790476;    float nkeqhWlXKcBYCqxgZsDvrae73556706 = -358113892;    float nkeqhWlXKcBYCqxgZsDvrae59779245 = -589394117;    float nkeqhWlXKcBYCqxgZsDvrae48530996 = -936145682;    float nkeqhWlXKcBYCqxgZsDvrae24642046 = -651223638;    float nkeqhWlXKcBYCqxgZsDvrae71280212 = -336844257;    float nkeqhWlXKcBYCqxgZsDvrae78869043 = -983136179;    float nkeqhWlXKcBYCqxgZsDvrae67904023 = -228386491;    float nkeqhWlXKcBYCqxgZsDvrae37127588 = -611818037;    float nkeqhWlXKcBYCqxgZsDvrae14091635 = -602374749;    float nkeqhWlXKcBYCqxgZsDvrae48631901 = -30649303;    float nkeqhWlXKcBYCqxgZsDvrae41442983 = -129072154;    float nkeqhWlXKcBYCqxgZsDvrae85117645 = -626312316;    float nkeqhWlXKcBYCqxgZsDvrae69301442 = -548871154;    float nkeqhWlXKcBYCqxgZsDvrae83570618 = -240749193;    float nkeqhWlXKcBYCqxgZsDvrae90782262 = -456495297;    float nkeqhWlXKcBYCqxgZsDvrae32401054 = -948088836;    float nkeqhWlXKcBYCqxgZsDvrae9590839 = -163693688;    float nkeqhWlXKcBYCqxgZsDvrae40721928 = -916175358;    float nkeqhWlXKcBYCqxgZsDvrae11971569 = -578797597;    float nkeqhWlXKcBYCqxgZsDvrae78859870 = -384091521;    float nkeqhWlXKcBYCqxgZsDvrae11959102 = -988473851;    float nkeqhWlXKcBYCqxgZsDvrae13703461 = -641904459;    float nkeqhWlXKcBYCqxgZsDvrae98931260 = -603154664;    float nkeqhWlXKcBYCqxgZsDvrae38721008 = -416221194;    float nkeqhWlXKcBYCqxgZsDvrae81045122 = -622288935;    float nkeqhWlXKcBYCqxgZsDvrae81957946 = -65250423;    float nkeqhWlXKcBYCqxgZsDvrae59174436 = -306416326;    float nkeqhWlXKcBYCqxgZsDvrae52859471 = -545780139;    float nkeqhWlXKcBYCqxgZsDvrae71789100 = 99625287;    float nkeqhWlXKcBYCqxgZsDvrae70928535 = -925619296;    float nkeqhWlXKcBYCqxgZsDvrae79566546 = -605067725;     nkeqhWlXKcBYCqxgZsDvrae96654748 = nkeqhWlXKcBYCqxgZsDvrae7784682;     nkeqhWlXKcBYCqxgZsDvrae7784682 = nkeqhWlXKcBYCqxgZsDvrae56100030;     nkeqhWlXKcBYCqxgZsDvrae56100030 = nkeqhWlXKcBYCqxgZsDvrae32093918;     nkeqhWlXKcBYCqxgZsDvrae32093918 = nkeqhWlXKcBYCqxgZsDvrae14002440;     nkeqhWlXKcBYCqxgZsDvrae14002440 = nkeqhWlXKcBYCqxgZsDvrae53134746;     nkeqhWlXKcBYCqxgZsDvrae53134746 = nkeqhWlXKcBYCqxgZsDvrae30073498;     nkeqhWlXKcBYCqxgZsDvrae30073498 = nkeqhWlXKcBYCqxgZsDvrae63832973;     nkeqhWlXKcBYCqxgZsDvrae63832973 = nkeqhWlXKcBYCqxgZsDvrae6021042;     nkeqhWlXKcBYCqxgZsDvrae6021042 = nkeqhWlXKcBYCqxgZsDvrae54947637;     nkeqhWlXKcBYCqxgZsDvrae54947637 = nkeqhWlXKcBYCqxgZsDvrae63592679;     nkeqhWlXKcBYCqxgZsDvrae63592679 = nkeqhWlXKcBYCqxgZsDvrae50073018;     nkeqhWlXKcBYCqxgZsDvrae50073018 = nkeqhWlXKcBYCqxgZsDvrae40631384;     nkeqhWlXKcBYCqxgZsDvrae40631384 = nkeqhWlXKcBYCqxgZsDvrae61127684;     nkeqhWlXKcBYCqxgZsDvrae61127684 = nkeqhWlXKcBYCqxgZsDvrae34377277;     nkeqhWlXKcBYCqxgZsDvrae34377277 = nkeqhWlXKcBYCqxgZsDvrae79397290;     nkeqhWlXKcBYCqxgZsDvrae79397290 = nkeqhWlXKcBYCqxgZsDvrae98617719;     nkeqhWlXKcBYCqxgZsDvrae98617719 = nkeqhWlXKcBYCqxgZsDvrae97275813;     nkeqhWlXKcBYCqxgZsDvrae97275813 = nkeqhWlXKcBYCqxgZsDvrae64584800;     nkeqhWlXKcBYCqxgZsDvrae64584800 = nkeqhWlXKcBYCqxgZsDvrae65813122;     nkeqhWlXKcBYCqxgZsDvrae65813122 = nkeqhWlXKcBYCqxgZsDvrae25388523;     nkeqhWlXKcBYCqxgZsDvrae25388523 = nkeqhWlXKcBYCqxgZsDvrae55388777;     nkeqhWlXKcBYCqxgZsDvrae55388777 = nkeqhWlXKcBYCqxgZsDvrae60667572;     nkeqhWlXKcBYCqxgZsDvrae60667572 = nkeqhWlXKcBYCqxgZsDvrae35413246;     nkeqhWlXKcBYCqxgZsDvrae35413246 = nkeqhWlXKcBYCqxgZsDvrae20728203;     nkeqhWlXKcBYCqxgZsDvrae20728203 = nkeqhWlXKcBYCqxgZsDvrae22413794;     nkeqhWlXKcBYCqxgZsDvrae22413794 = nkeqhWlXKcBYCqxgZsDvrae41788105;     nkeqhWlXKcBYCqxgZsDvrae41788105 = nkeqhWlXKcBYCqxgZsDvrae2303855;     nkeqhWlXKcBYCqxgZsDvrae2303855 = nkeqhWlXKcBYCqxgZsDvrae59207945;     nkeqhWlXKcBYCqxgZsDvrae59207945 = nkeqhWlXKcBYCqxgZsDvrae59548953;     nkeqhWlXKcBYCqxgZsDvrae59548953 = nkeqhWlXKcBYCqxgZsDvrae79300234;     nkeqhWlXKcBYCqxgZsDvrae79300234 = nkeqhWlXKcBYCqxgZsDvrae56377880;     nkeqhWlXKcBYCqxgZsDvrae56377880 = nkeqhWlXKcBYCqxgZsDvrae6690733;     nkeqhWlXKcBYCqxgZsDvrae6690733 = nkeqhWlXKcBYCqxgZsDvrae40335164;     nkeqhWlXKcBYCqxgZsDvrae40335164 = nkeqhWlXKcBYCqxgZsDvrae53225074;     nkeqhWlXKcBYCqxgZsDvrae53225074 = nkeqhWlXKcBYCqxgZsDvrae60820570;     nkeqhWlXKcBYCqxgZsDvrae60820570 = nkeqhWlXKcBYCqxgZsDvrae19618046;     nkeqhWlXKcBYCqxgZsDvrae19618046 = nkeqhWlXKcBYCqxgZsDvrae50086723;     nkeqhWlXKcBYCqxgZsDvrae50086723 = nkeqhWlXKcBYCqxgZsDvrae72633768;     nkeqhWlXKcBYCqxgZsDvrae72633768 = nkeqhWlXKcBYCqxgZsDvrae93304588;     nkeqhWlXKcBYCqxgZsDvrae93304588 = nkeqhWlXKcBYCqxgZsDvrae86944078;     nkeqhWlXKcBYCqxgZsDvrae86944078 = nkeqhWlXKcBYCqxgZsDvrae57484499;     nkeqhWlXKcBYCqxgZsDvrae57484499 = nkeqhWlXKcBYCqxgZsDvrae18261190;     nkeqhWlXKcBYCqxgZsDvrae18261190 = nkeqhWlXKcBYCqxgZsDvrae46575938;     nkeqhWlXKcBYCqxgZsDvrae46575938 = nkeqhWlXKcBYCqxgZsDvrae86781344;     nkeqhWlXKcBYCqxgZsDvrae86781344 = nkeqhWlXKcBYCqxgZsDvrae79285220;     nkeqhWlXKcBYCqxgZsDvrae79285220 = nkeqhWlXKcBYCqxgZsDvrae37296149;     nkeqhWlXKcBYCqxgZsDvrae37296149 = nkeqhWlXKcBYCqxgZsDvrae72486662;     nkeqhWlXKcBYCqxgZsDvrae72486662 = nkeqhWlXKcBYCqxgZsDvrae18733237;     nkeqhWlXKcBYCqxgZsDvrae18733237 = nkeqhWlXKcBYCqxgZsDvrae68425683;     nkeqhWlXKcBYCqxgZsDvrae68425683 = nkeqhWlXKcBYCqxgZsDvrae27147900;     nkeqhWlXKcBYCqxgZsDvrae27147900 = nkeqhWlXKcBYCqxgZsDvrae69709395;     nkeqhWlXKcBYCqxgZsDvrae69709395 = nkeqhWlXKcBYCqxgZsDvrae15655953;     nkeqhWlXKcBYCqxgZsDvrae15655953 = nkeqhWlXKcBYCqxgZsDvrae94719164;     nkeqhWlXKcBYCqxgZsDvrae94719164 = nkeqhWlXKcBYCqxgZsDvrae61475293;     nkeqhWlXKcBYCqxgZsDvrae61475293 = nkeqhWlXKcBYCqxgZsDvrae41265972;     nkeqhWlXKcBYCqxgZsDvrae41265972 = nkeqhWlXKcBYCqxgZsDvrae47117110;     nkeqhWlXKcBYCqxgZsDvrae47117110 = nkeqhWlXKcBYCqxgZsDvrae20686785;     nkeqhWlXKcBYCqxgZsDvrae20686785 = nkeqhWlXKcBYCqxgZsDvrae11365716;     nkeqhWlXKcBYCqxgZsDvrae11365716 = nkeqhWlXKcBYCqxgZsDvrae91588645;     nkeqhWlXKcBYCqxgZsDvrae91588645 = nkeqhWlXKcBYCqxgZsDvrae11346642;     nkeqhWlXKcBYCqxgZsDvrae11346642 = nkeqhWlXKcBYCqxgZsDvrae27769643;     nkeqhWlXKcBYCqxgZsDvrae27769643 = nkeqhWlXKcBYCqxgZsDvrae4625028;     nkeqhWlXKcBYCqxgZsDvrae4625028 = nkeqhWlXKcBYCqxgZsDvrae46472089;     nkeqhWlXKcBYCqxgZsDvrae46472089 = nkeqhWlXKcBYCqxgZsDvrae75647402;     nkeqhWlXKcBYCqxgZsDvrae75647402 = nkeqhWlXKcBYCqxgZsDvrae7214799;     nkeqhWlXKcBYCqxgZsDvrae7214799 = nkeqhWlXKcBYCqxgZsDvrae43382286;     nkeqhWlXKcBYCqxgZsDvrae43382286 = nkeqhWlXKcBYCqxgZsDvrae296221;     nkeqhWlXKcBYCqxgZsDvrae296221 = nkeqhWlXKcBYCqxgZsDvrae7902611;     nkeqhWlXKcBYCqxgZsDvrae7902611 = nkeqhWlXKcBYCqxgZsDvrae73556706;     nkeqhWlXKcBYCqxgZsDvrae73556706 = nkeqhWlXKcBYCqxgZsDvrae59779245;     nkeqhWlXKcBYCqxgZsDvrae59779245 = nkeqhWlXKcBYCqxgZsDvrae48530996;     nkeqhWlXKcBYCqxgZsDvrae48530996 = nkeqhWlXKcBYCqxgZsDvrae24642046;     nkeqhWlXKcBYCqxgZsDvrae24642046 = nkeqhWlXKcBYCqxgZsDvrae71280212;     nkeqhWlXKcBYCqxgZsDvrae71280212 = nkeqhWlXKcBYCqxgZsDvrae78869043;     nkeqhWlXKcBYCqxgZsDvrae78869043 = nkeqhWlXKcBYCqxgZsDvrae67904023;     nkeqhWlXKcBYCqxgZsDvrae67904023 = nkeqhWlXKcBYCqxgZsDvrae37127588;     nkeqhWlXKcBYCqxgZsDvrae37127588 = nkeqhWlXKcBYCqxgZsDvrae14091635;     nkeqhWlXKcBYCqxgZsDvrae14091635 = nkeqhWlXKcBYCqxgZsDvrae48631901;     nkeqhWlXKcBYCqxgZsDvrae48631901 = nkeqhWlXKcBYCqxgZsDvrae41442983;     nkeqhWlXKcBYCqxgZsDvrae41442983 = nkeqhWlXKcBYCqxgZsDvrae85117645;     nkeqhWlXKcBYCqxgZsDvrae85117645 = nkeqhWlXKcBYCqxgZsDvrae69301442;     nkeqhWlXKcBYCqxgZsDvrae69301442 = nkeqhWlXKcBYCqxgZsDvrae83570618;     nkeqhWlXKcBYCqxgZsDvrae83570618 = nkeqhWlXKcBYCqxgZsDvrae90782262;     nkeqhWlXKcBYCqxgZsDvrae90782262 = nkeqhWlXKcBYCqxgZsDvrae32401054;     nkeqhWlXKcBYCqxgZsDvrae32401054 = nkeqhWlXKcBYCqxgZsDvrae9590839;     nkeqhWlXKcBYCqxgZsDvrae9590839 = nkeqhWlXKcBYCqxgZsDvrae40721928;     nkeqhWlXKcBYCqxgZsDvrae40721928 = nkeqhWlXKcBYCqxgZsDvrae11971569;     nkeqhWlXKcBYCqxgZsDvrae11971569 = nkeqhWlXKcBYCqxgZsDvrae78859870;     nkeqhWlXKcBYCqxgZsDvrae78859870 = nkeqhWlXKcBYCqxgZsDvrae11959102;     nkeqhWlXKcBYCqxgZsDvrae11959102 = nkeqhWlXKcBYCqxgZsDvrae13703461;     nkeqhWlXKcBYCqxgZsDvrae13703461 = nkeqhWlXKcBYCqxgZsDvrae98931260;     nkeqhWlXKcBYCqxgZsDvrae98931260 = nkeqhWlXKcBYCqxgZsDvrae38721008;     nkeqhWlXKcBYCqxgZsDvrae38721008 = nkeqhWlXKcBYCqxgZsDvrae81045122;     nkeqhWlXKcBYCqxgZsDvrae81045122 = nkeqhWlXKcBYCqxgZsDvrae81957946;     nkeqhWlXKcBYCqxgZsDvrae81957946 = nkeqhWlXKcBYCqxgZsDvrae59174436;     nkeqhWlXKcBYCqxgZsDvrae59174436 = nkeqhWlXKcBYCqxgZsDvrae52859471;     nkeqhWlXKcBYCqxgZsDvrae52859471 = nkeqhWlXKcBYCqxgZsDvrae71789100;     nkeqhWlXKcBYCqxgZsDvrae71789100 = nkeqhWlXKcBYCqxgZsDvrae70928535;     nkeqhWlXKcBYCqxgZsDvrae70928535 = nkeqhWlXKcBYCqxgZsDvrae79566546;     nkeqhWlXKcBYCqxgZsDvrae79566546 = nkeqhWlXKcBYCqxgZsDvrae96654748;}



void giShYRZtLtCAiJHnoDYwtWwzci2243081() {     float qvOJhCiQCVwChDcCBXSziqJ48110987 = -151966171;    float qvOJhCiQCVwChDcCBXSziqJ42732658 = -865630886;    float qvOJhCiQCVwChDcCBXSziqJ12722780 = -12152358;    float qvOJhCiQCVwChDcCBXSziqJ72813618 = -508245208;    float qvOJhCiQCVwChDcCBXSziqJ17286360 = -21459133;    float qvOJhCiQCVwChDcCBXSziqJ65182983 = 30007730;    float qvOJhCiQCVwChDcCBXSziqJ38009096 = -45867919;    float qvOJhCiQCVwChDcCBXSziqJ53631811 = -2349862;    float qvOJhCiQCVwChDcCBXSziqJ65629291 = -260474040;    float qvOJhCiQCVwChDcCBXSziqJ10184825 = -757697109;    float qvOJhCiQCVwChDcCBXSziqJ65412280 = -166030864;    float qvOJhCiQCVwChDcCBXSziqJ33423025 = -630180252;    float qvOJhCiQCVwChDcCBXSziqJ94876101 = -865405531;    float qvOJhCiQCVwChDcCBXSziqJ5149151 = -396137983;    float qvOJhCiQCVwChDcCBXSziqJ45067739 = -574872385;    float qvOJhCiQCVwChDcCBXSziqJ5699696 = -113138443;    float qvOJhCiQCVwChDcCBXSziqJ48474718 = -476370581;    float qvOJhCiQCVwChDcCBXSziqJ49732912 = -846759373;    float qvOJhCiQCVwChDcCBXSziqJ45622311 = -483158427;    float qvOJhCiQCVwChDcCBXSziqJ802495 = -896939028;    float qvOJhCiQCVwChDcCBXSziqJ59436752 = -366449098;    float qvOJhCiQCVwChDcCBXSziqJ68953477 = -905959601;    float qvOJhCiQCVwChDcCBXSziqJ96666928 = -382932054;    float qvOJhCiQCVwChDcCBXSziqJ61627359 = -348806099;    float qvOJhCiQCVwChDcCBXSziqJ97117926 = -768590741;    float qvOJhCiQCVwChDcCBXSziqJ22468599 = -113824643;    float qvOJhCiQCVwChDcCBXSziqJ8479541 = -538707764;    float qvOJhCiQCVwChDcCBXSziqJ97565505 = -87277929;    float qvOJhCiQCVwChDcCBXSziqJ98120970 = -505663218;    float qvOJhCiQCVwChDcCBXSziqJ9819051 = 14496411;    float qvOJhCiQCVwChDcCBXSziqJ53249934 = -586051576;    float qvOJhCiQCVwChDcCBXSziqJ7720750 = 28951310;    float qvOJhCiQCVwChDcCBXSziqJ34198228 = -63245879;    float qvOJhCiQCVwChDcCBXSziqJ40792672 = -188383786;    float qvOJhCiQCVwChDcCBXSziqJ62553607 = -519243237;    float qvOJhCiQCVwChDcCBXSziqJ85048131 = -516501364;    float qvOJhCiQCVwChDcCBXSziqJ38116861 = -50971323;    float qvOJhCiQCVwChDcCBXSziqJ41117376 = -470504713;    float qvOJhCiQCVwChDcCBXSziqJ30787970 = -421162250;    float qvOJhCiQCVwChDcCBXSziqJ28693024 = -832936664;    float qvOJhCiQCVwChDcCBXSziqJ24509570 = -330909260;    float qvOJhCiQCVwChDcCBXSziqJ38657959 = -896594203;    float qvOJhCiQCVwChDcCBXSziqJ65611344 = -742093593;    float qvOJhCiQCVwChDcCBXSziqJ84221177 = -164453113;    float qvOJhCiQCVwChDcCBXSziqJ22767799 = -916612061;    float qvOJhCiQCVwChDcCBXSziqJ76318781 = -941013326;    float qvOJhCiQCVwChDcCBXSziqJ66298803 = -177358637;    float qvOJhCiQCVwChDcCBXSziqJ32399875 = -884223490;    float qvOJhCiQCVwChDcCBXSziqJ4724589 = 70544684;    float qvOJhCiQCVwChDcCBXSziqJ56541096 = 5023823;    float qvOJhCiQCVwChDcCBXSziqJ994409 = -598903657;    float qvOJhCiQCVwChDcCBXSziqJ5684223 = -238622327;    float qvOJhCiQCVwChDcCBXSziqJ7624755 = -309029422;    float qvOJhCiQCVwChDcCBXSziqJ43522558 = -802911822;    float qvOJhCiQCVwChDcCBXSziqJ42806329 = -917711956;    float qvOJhCiQCVwChDcCBXSziqJ79157509 = -246006571;    float qvOJhCiQCVwChDcCBXSziqJ46065729 = -382698832;    float qvOJhCiQCVwChDcCBXSziqJ51095421 = -663346260;    float qvOJhCiQCVwChDcCBXSziqJ75695692 = -739654467;    float qvOJhCiQCVwChDcCBXSziqJ94817760 = -907634490;    float qvOJhCiQCVwChDcCBXSziqJ56703443 = -431284506;    float qvOJhCiQCVwChDcCBXSziqJ40443591 = -958589990;    float qvOJhCiQCVwChDcCBXSziqJ55510840 = -496686644;    float qvOJhCiQCVwChDcCBXSziqJ55810240 = -174970452;    float qvOJhCiQCVwChDcCBXSziqJ56934890 = -71645534;    float qvOJhCiQCVwChDcCBXSziqJ57691531 = -94982175;    float qvOJhCiQCVwChDcCBXSziqJ99224797 = -466934374;    float qvOJhCiQCVwChDcCBXSziqJ54083429 = -577021745;    float qvOJhCiQCVwChDcCBXSziqJ42595544 = -876894746;    float qvOJhCiQCVwChDcCBXSziqJ60019607 = 41628979;    float qvOJhCiQCVwChDcCBXSziqJ67582835 = 37832880;    float qvOJhCiQCVwChDcCBXSziqJ7357343 = 94134131;    float qvOJhCiQCVwChDcCBXSziqJ18944943 = -325597124;    float qvOJhCiQCVwChDcCBXSziqJ16929288 = -650221763;    float qvOJhCiQCVwChDcCBXSziqJ76292925 = -466029768;    float qvOJhCiQCVwChDcCBXSziqJ20778794 = -469854896;    float qvOJhCiQCVwChDcCBXSziqJ3342133 = -63866008;    float qvOJhCiQCVwChDcCBXSziqJ12445752 = -118478941;    float qvOJhCiQCVwChDcCBXSziqJ38859560 = -432194038;    float qvOJhCiQCVwChDcCBXSziqJ20799145 = -827577416;    float qvOJhCiQCVwChDcCBXSziqJ56169795 = -936466007;    float qvOJhCiQCVwChDcCBXSziqJ76079665 = -654484275;    float qvOJhCiQCVwChDcCBXSziqJ92840916 = -57822614;    float qvOJhCiQCVwChDcCBXSziqJ41579874 = -410687041;    float qvOJhCiQCVwChDcCBXSziqJ8824643 = -386599933;    float qvOJhCiQCVwChDcCBXSziqJ47565711 = -247429250;    float qvOJhCiQCVwChDcCBXSziqJ95995 = -662019268;    float qvOJhCiQCVwChDcCBXSziqJ90675669 = -260334057;    float qvOJhCiQCVwChDcCBXSziqJ97986343 = -270671830;    float qvOJhCiQCVwChDcCBXSziqJ83396098 = -173236667;    float qvOJhCiQCVwChDcCBXSziqJ38982402 = -33802533;    float qvOJhCiQCVwChDcCBXSziqJ87021439 = -387625064;    float qvOJhCiQCVwChDcCBXSziqJ65421684 = -730850247;    float qvOJhCiQCVwChDcCBXSziqJ35970209 = -513527760;    float qvOJhCiQCVwChDcCBXSziqJ71989580 = -301652159;    float qvOJhCiQCVwChDcCBXSziqJ84065978 = -372319271;    float qvOJhCiQCVwChDcCBXSziqJ83147118 = -299907559;    float qvOJhCiQCVwChDcCBXSziqJ9801105 = -467123141;    float qvOJhCiQCVwChDcCBXSziqJ27286287 = 7192420;    float qvOJhCiQCVwChDcCBXSziqJ65076268 = -151966171;     qvOJhCiQCVwChDcCBXSziqJ48110987 = qvOJhCiQCVwChDcCBXSziqJ42732658;     qvOJhCiQCVwChDcCBXSziqJ42732658 = qvOJhCiQCVwChDcCBXSziqJ12722780;     qvOJhCiQCVwChDcCBXSziqJ12722780 = qvOJhCiQCVwChDcCBXSziqJ72813618;     qvOJhCiQCVwChDcCBXSziqJ72813618 = qvOJhCiQCVwChDcCBXSziqJ17286360;     qvOJhCiQCVwChDcCBXSziqJ17286360 = qvOJhCiQCVwChDcCBXSziqJ65182983;     qvOJhCiQCVwChDcCBXSziqJ65182983 = qvOJhCiQCVwChDcCBXSziqJ38009096;     qvOJhCiQCVwChDcCBXSziqJ38009096 = qvOJhCiQCVwChDcCBXSziqJ53631811;     qvOJhCiQCVwChDcCBXSziqJ53631811 = qvOJhCiQCVwChDcCBXSziqJ65629291;     qvOJhCiQCVwChDcCBXSziqJ65629291 = qvOJhCiQCVwChDcCBXSziqJ10184825;     qvOJhCiQCVwChDcCBXSziqJ10184825 = qvOJhCiQCVwChDcCBXSziqJ65412280;     qvOJhCiQCVwChDcCBXSziqJ65412280 = qvOJhCiQCVwChDcCBXSziqJ33423025;     qvOJhCiQCVwChDcCBXSziqJ33423025 = qvOJhCiQCVwChDcCBXSziqJ94876101;     qvOJhCiQCVwChDcCBXSziqJ94876101 = qvOJhCiQCVwChDcCBXSziqJ5149151;     qvOJhCiQCVwChDcCBXSziqJ5149151 = qvOJhCiQCVwChDcCBXSziqJ45067739;     qvOJhCiQCVwChDcCBXSziqJ45067739 = qvOJhCiQCVwChDcCBXSziqJ5699696;     qvOJhCiQCVwChDcCBXSziqJ5699696 = qvOJhCiQCVwChDcCBXSziqJ48474718;     qvOJhCiQCVwChDcCBXSziqJ48474718 = qvOJhCiQCVwChDcCBXSziqJ49732912;     qvOJhCiQCVwChDcCBXSziqJ49732912 = qvOJhCiQCVwChDcCBXSziqJ45622311;     qvOJhCiQCVwChDcCBXSziqJ45622311 = qvOJhCiQCVwChDcCBXSziqJ802495;     qvOJhCiQCVwChDcCBXSziqJ802495 = qvOJhCiQCVwChDcCBXSziqJ59436752;     qvOJhCiQCVwChDcCBXSziqJ59436752 = qvOJhCiQCVwChDcCBXSziqJ68953477;     qvOJhCiQCVwChDcCBXSziqJ68953477 = qvOJhCiQCVwChDcCBXSziqJ96666928;     qvOJhCiQCVwChDcCBXSziqJ96666928 = qvOJhCiQCVwChDcCBXSziqJ61627359;     qvOJhCiQCVwChDcCBXSziqJ61627359 = qvOJhCiQCVwChDcCBXSziqJ97117926;     qvOJhCiQCVwChDcCBXSziqJ97117926 = qvOJhCiQCVwChDcCBXSziqJ22468599;     qvOJhCiQCVwChDcCBXSziqJ22468599 = qvOJhCiQCVwChDcCBXSziqJ8479541;     qvOJhCiQCVwChDcCBXSziqJ8479541 = qvOJhCiQCVwChDcCBXSziqJ97565505;     qvOJhCiQCVwChDcCBXSziqJ97565505 = qvOJhCiQCVwChDcCBXSziqJ98120970;     qvOJhCiQCVwChDcCBXSziqJ98120970 = qvOJhCiQCVwChDcCBXSziqJ9819051;     qvOJhCiQCVwChDcCBXSziqJ9819051 = qvOJhCiQCVwChDcCBXSziqJ53249934;     qvOJhCiQCVwChDcCBXSziqJ53249934 = qvOJhCiQCVwChDcCBXSziqJ7720750;     qvOJhCiQCVwChDcCBXSziqJ7720750 = qvOJhCiQCVwChDcCBXSziqJ34198228;     qvOJhCiQCVwChDcCBXSziqJ34198228 = qvOJhCiQCVwChDcCBXSziqJ40792672;     qvOJhCiQCVwChDcCBXSziqJ40792672 = qvOJhCiQCVwChDcCBXSziqJ62553607;     qvOJhCiQCVwChDcCBXSziqJ62553607 = qvOJhCiQCVwChDcCBXSziqJ85048131;     qvOJhCiQCVwChDcCBXSziqJ85048131 = qvOJhCiQCVwChDcCBXSziqJ38116861;     qvOJhCiQCVwChDcCBXSziqJ38116861 = qvOJhCiQCVwChDcCBXSziqJ41117376;     qvOJhCiQCVwChDcCBXSziqJ41117376 = qvOJhCiQCVwChDcCBXSziqJ30787970;     qvOJhCiQCVwChDcCBXSziqJ30787970 = qvOJhCiQCVwChDcCBXSziqJ28693024;     qvOJhCiQCVwChDcCBXSziqJ28693024 = qvOJhCiQCVwChDcCBXSziqJ24509570;     qvOJhCiQCVwChDcCBXSziqJ24509570 = qvOJhCiQCVwChDcCBXSziqJ38657959;     qvOJhCiQCVwChDcCBXSziqJ38657959 = qvOJhCiQCVwChDcCBXSziqJ65611344;     qvOJhCiQCVwChDcCBXSziqJ65611344 = qvOJhCiQCVwChDcCBXSziqJ84221177;     qvOJhCiQCVwChDcCBXSziqJ84221177 = qvOJhCiQCVwChDcCBXSziqJ22767799;     qvOJhCiQCVwChDcCBXSziqJ22767799 = qvOJhCiQCVwChDcCBXSziqJ76318781;     qvOJhCiQCVwChDcCBXSziqJ76318781 = qvOJhCiQCVwChDcCBXSziqJ66298803;     qvOJhCiQCVwChDcCBXSziqJ66298803 = qvOJhCiQCVwChDcCBXSziqJ32399875;     qvOJhCiQCVwChDcCBXSziqJ32399875 = qvOJhCiQCVwChDcCBXSziqJ4724589;     qvOJhCiQCVwChDcCBXSziqJ4724589 = qvOJhCiQCVwChDcCBXSziqJ56541096;     qvOJhCiQCVwChDcCBXSziqJ56541096 = qvOJhCiQCVwChDcCBXSziqJ994409;     qvOJhCiQCVwChDcCBXSziqJ994409 = qvOJhCiQCVwChDcCBXSziqJ5684223;     qvOJhCiQCVwChDcCBXSziqJ5684223 = qvOJhCiQCVwChDcCBXSziqJ7624755;     qvOJhCiQCVwChDcCBXSziqJ7624755 = qvOJhCiQCVwChDcCBXSziqJ43522558;     qvOJhCiQCVwChDcCBXSziqJ43522558 = qvOJhCiQCVwChDcCBXSziqJ42806329;     qvOJhCiQCVwChDcCBXSziqJ42806329 = qvOJhCiQCVwChDcCBXSziqJ79157509;     qvOJhCiQCVwChDcCBXSziqJ79157509 = qvOJhCiQCVwChDcCBXSziqJ46065729;     qvOJhCiQCVwChDcCBXSziqJ46065729 = qvOJhCiQCVwChDcCBXSziqJ51095421;     qvOJhCiQCVwChDcCBXSziqJ51095421 = qvOJhCiQCVwChDcCBXSziqJ75695692;     qvOJhCiQCVwChDcCBXSziqJ75695692 = qvOJhCiQCVwChDcCBXSziqJ94817760;     qvOJhCiQCVwChDcCBXSziqJ94817760 = qvOJhCiQCVwChDcCBXSziqJ56703443;     qvOJhCiQCVwChDcCBXSziqJ56703443 = qvOJhCiQCVwChDcCBXSziqJ40443591;     qvOJhCiQCVwChDcCBXSziqJ40443591 = qvOJhCiQCVwChDcCBXSziqJ55510840;     qvOJhCiQCVwChDcCBXSziqJ55510840 = qvOJhCiQCVwChDcCBXSziqJ55810240;     qvOJhCiQCVwChDcCBXSziqJ55810240 = qvOJhCiQCVwChDcCBXSziqJ56934890;     qvOJhCiQCVwChDcCBXSziqJ56934890 = qvOJhCiQCVwChDcCBXSziqJ57691531;     qvOJhCiQCVwChDcCBXSziqJ57691531 = qvOJhCiQCVwChDcCBXSziqJ99224797;     qvOJhCiQCVwChDcCBXSziqJ99224797 = qvOJhCiQCVwChDcCBXSziqJ54083429;     qvOJhCiQCVwChDcCBXSziqJ54083429 = qvOJhCiQCVwChDcCBXSziqJ42595544;     qvOJhCiQCVwChDcCBXSziqJ42595544 = qvOJhCiQCVwChDcCBXSziqJ60019607;     qvOJhCiQCVwChDcCBXSziqJ60019607 = qvOJhCiQCVwChDcCBXSziqJ67582835;     qvOJhCiQCVwChDcCBXSziqJ67582835 = qvOJhCiQCVwChDcCBXSziqJ7357343;     qvOJhCiQCVwChDcCBXSziqJ7357343 = qvOJhCiQCVwChDcCBXSziqJ18944943;     qvOJhCiQCVwChDcCBXSziqJ18944943 = qvOJhCiQCVwChDcCBXSziqJ16929288;     qvOJhCiQCVwChDcCBXSziqJ16929288 = qvOJhCiQCVwChDcCBXSziqJ76292925;     qvOJhCiQCVwChDcCBXSziqJ76292925 = qvOJhCiQCVwChDcCBXSziqJ20778794;     qvOJhCiQCVwChDcCBXSziqJ20778794 = qvOJhCiQCVwChDcCBXSziqJ3342133;     qvOJhCiQCVwChDcCBXSziqJ3342133 = qvOJhCiQCVwChDcCBXSziqJ12445752;     qvOJhCiQCVwChDcCBXSziqJ12445752 = qvOJhCiQCVwChDcCBXSziqJ38859560;     qvOJhCiQCVwChDcCBXSziqJ38859560 = qvOJhCiQCVwChDcCBXSziqJ20799145;     qvOJhCiQCVwChDcCBXSziqJ20799145 = qvOJhCiQCVwChDcCBXSziqJ56169795;     qvOJhCiQCVwChDcCBXSziqJ56169795 = qvOJhCiQCVwChDcCBXSziqJ76079665;     qvOJhCiQCVwChDcCBXSziqJ76079665 = qvOJhCiQCVwChDcCBXSziqJ92840916;     qvOJhCiQCVwChDcCBXSziqJ92840916 = qvOJhCiQCVwChDcCBXSziqJ41579874;     qvOJhCiQCVwChDcCBXSziqJ41579874 = qvOJhCiQCVwChDcCBXSziqJ8824643;     qvOJhCiQCVwChDcCBXSziqJ8824643 = qvOJhCiQCVwChDcCBXSziqJ47565711;     qvOJhCiQCVwChDcCBXSziqJ47565711 = qvOJhCiQCVwChDcCBXSziqJ95995;     qvOJhCiQCVwChDcCBXSziqJ95995 = qvOJhCiQCVwChDcCBXSziqJ90675669;     qvOJhCiQCVwChDcCBXSziqJ90675669 = qvOJhCiQCVwChDcCBXSziqJ97986343;     qvOJhCiQCVwChDcCBXSziqJ97986343 = qvOJhCiQCVwChDcCBXSziqJ83396098;     qvOJhCiQCVwChDcCBXSziqJ83396098 = qvOJhCiQCVwChDcCBXSziqJ38982402;     qvOJhCiQCVwChDcCBXSziqJ38982402 = qvOJhCiQCVwChDcCBXSziqJ87021439;     qvOJhCiQCVwChDcCBXSziqJ87021439 = qvOJhCiQCVwChDcCBXSziqJ65421684;     qvOJhCiQCVwChDcCBXSziqJ65421684 = qvOJhCiQCVwChDcCBXSziqJ35970209;     qvOJhCiQCVwChDcCBXSziqJ35970209 = qvOJhCiQCVwChDcCBXSziqJ71989580;     qvOJhCiQCVwChDcCBXSziqJ71989580 = qvOJhCiQCVwChDcCBXSziqJ84065978;     qvOJhCiQCVwChDcCBXSziqJ84065978 = qvOJhCiQCVwChDcCBXSziqJ83147118;     qvOJhCiQCVwChDcCBXSziqJ83147118 = qvOJhCiQCVwChDcCBXSziqJ9801105;     qvOJhCiQCVwChDcCBXSziqJ9801105 = qvOJhCiQCVwChDcCBXSziqJ27286287;     qvOJhCiQCVwChDcCBXSziqJ27286287 = qvOJhCiQCVwChDcCBXSziqJ65076268;     qvOJhCiQCVwChDcCBXSziqJ65076268 = qvOJhCiQCVwChDcCBXSziqJ48110987;}



void LoCCiROcUwqPhloRWjlAhIFifW53502849() {     float GQmPLtPLWJDeYkJNrKzDOVQ69037012 = -968864794;    float GQmPLtPLWJDeYkJNrKzDOVQ96061419 = -573490193;    float GQmPLtPLWJDeYkJNrKzDOVQ10854219 = -772075741;    float GQmPLtPLWJDeYkJNrKzDOVQ35910502 = -99871312;    float GQmPLtPLWJDeYkJNrKzDOVQ18806771 = -197212672;    float GQmPLtPLWJDeYkJNrKzDOVQ94121937 = -870397046;    float GQmPLtPLWJDeYkJNrKzDOVQ48639739 = -379648803;    float GQmPLtPLWJDeYkJNrKzDOVQ41377370 = -210596496;    float GQmPLtPLWJDeYkJNrKzDOVQ15239297 = -220079267;    float GQmPLtPLWJDeYkJNrKzDOVQ44190857 = -816062517;    float GQmPLtPLWJDeYkJNrKzDOVQ16374993 = -370706232;    float GQmPLtPLWJDeYkJNrKzDOVQ16694725 = -139073842;    float GQmPLtPLWJDeYkJNrKzDOVQ81529250 = -17357447;    float GQmPLtPLWJDeYkJNrKzDOVQ69125601 = -467423488;    float GQmPLtPLWJDeYkJNrKzDOVQ54991635 = -528331350;    float GQmPLtPLWJDeYkJNrKzDOVQ4576614 = -893767093;    float GQmPLtPLWJDeYkJNrKzDOVQ90330640 = -105801474;    float GQmPLtPLWJDeYkJNrKzDOVQ45502629 = -772275983;    float GQmPLtPLWJDeYkJNrKzDOVQ46187634 = -893807437;    float GQmPLtPLWJDeYkJNrKzDOVQ48384728 = -435030596;    float GQmPLtPLWJDeYkJNrKzDOVQ86023697 = -592915508;    float GQmPLtPLWJDeYkJNrKzDOVQ52926195 = -716742335;    float GQmPLtPLWJDeYkJNrKzDOVQ91619692 = -459499120;    float GQmPLtPLWJDeYkJNrKzDOVQ44650614 = -969304654;    float GQmPLtPLWJDeYkJNrKzDOVQ2600467 = -8193822;    float GQmPLtPLWJDeYkJNrKzDOVQ98147346 = -155720847;    float GQmPLtPLWJDeYkJNrKzDOVQ66543711 = -875276405;    float GQmPLtPLWJDeYkJNrKzDOVQ34767949 = -215613543;    float GQmPLtPLWJDeYkJNrKzDOVQ25609341 = -21264929;    float GQmPLtPLWJDeYkJNrKzDOVQ50773535 = -460228713;    float GQmPLtPLWJDeYkJNrKzDOVQ48650549 = -656146007;    float GQmPLtPLWJDeYkJNrKzDOVQ2461965 = -677214316;    float GQmPLtPLWJDeYkJNrKzDOVQ1818721 = -381007563;    float GQmPLtPLWJDeYkJNrKzDOVQ99935978 = -746919247;    float GQmPLtPLWJDeYkJNrKzDOVQ41222967 = -97301333;    float GQmPLtPLWJDeYkJNrKzDOVQ34634582 = -570924793;    float GQmPLtPLWJDeYkJNrKzDOVQ47013784 = 69203319;    float GQmPLtPLWJDeYkJNrKzDOVQ6151084 = -441737526;    float GQmPLtPLWJDeYkJNrKzDOVQ97427096 = -757063032;    float GQmPLtPLWJDeYkJNrKzDOVQ92394972 = -526672149;    float GQmPLtPLWJDeYkJNrKzDOVQ6593201 = -34377215;    float GQmPLtPLWJDeYkJNrKzDOVQ35266136 = -943444682;    float GQmPLtPLWJDeYkJNrKzDOVQ94023901 = -799542347;    float GQmPLtPLWJDeYkJNrKzDOVQ65443980 = -826912379;    float GQmPLtPLWJDeYkJNrKzDOVQ67778150 = -253060131;    float GQmPLtPLWJDeYkJNrKzDOVQ17706478 = -428804752;    float GQmPLtPLWJDeYkJNrKzDOVQ20366891 = 6269419;    float GQmPLtPLWJDeYkJNrKzDOVQ48623022 = -731324982;    float GQmPLtPLWJDeYkJNrKzDOVQ2186934 = -627224250;    float GQmPLtPLWJDeYkJNrKzDOVQ30543496 = -393248701;    float GQmPLtPLWJDeYkJNrKzDOVQ95961643 = -874263013;    float GQmPLtPLWJDeYkJNrKzDOVQ4471096 = 86755166;    float GQmPLtPLWJDeYkJNrKzDOVQ25556617 = -781053020;    float GQmPLtPLWJDeYkJNrKzDOVQ12693413 = -796029008;    float GQmPLtPLWJDeYkJNrKzDOVQ67479151 = -814568151;    float GQmPLtPLWJDeYkJNrKzDOVQ16110818 = -152122460;    float GQmPLtPLWJDeYkJNrKzDOVQ4441727 = -13991073;    float GQmPLtPLWJDeYkJNrKzDOVQ66203604 = -802771087;    float GQmPLtPLWJDeYkJNrKzDOVQ33310035 = 8322509;    float GQmPLtPLWJDeYkJNrKzDOVQ20659424 = 58508174;    float GQmPLtPLWJDeYkJNrKzDOVQ27578226 = -995120641;    float GQmPLtPLWJDeYkJNrKzDOVQ13871790 = -64035261;    float GQmPLtPLWJDeYkJNrKzDOVQ15768029 = -89331568;    float GQmPLtPLWJDeYkJNrKzDOVQ64465761 = -759850554;    float GQmPLtPLWJDeYkJNrKzDOVQ95540307 = -59916510;    float GQmPLtPLWJDeYkJNrKzDOVQ13913029 = -693491916;    float GQmPLtPLWJDeYkJNrKzDOVQ14876005 = -758066279;    float GQmPLtPLWJDeYkJNrKzDOVQ81593271 = -270438201;    float GQmPLtPLWJDeYkJNrKzDOVQ27902635 = -270122155;    float GQmPLtPLWJDeYkJNrKzDOVQ20357054 = -957406557;    float GQmPLtPLWJDeYkJNrKzDOVQ57562830 = -862970413;    float GQmPLtPLWJDeYkJNrKzDOVQ84179557 = -664063948;    float GQmPLtPLWJDeYkJNrKzDOVQ48075533 = 84787049;    float GQmPLtPLWJDeYkJNrKzDOVQ53792662 = -267135288;    float GQmPLtPLWJDeYkJNrKzDOVQ41791528 = -300653381;    float GQmPLtPLWJDeYkJNrKzDOVQ50757562 = -649470827;    float GQmPLtPLWJDeYkJNrKzDOVQ58902293 = -917199988;    float GQmPLtPLWJDeYkJNrKzDOVQ26175712 = -632586742;    float GQmPLtPLWJDeYkJNrKzDOVQ76872463 = -616244523;    float GQmPLtPLWJDeYkJNrKzDOVQ84893988 = -579389070;    float GQmPLtPLWJDeYkJNrKzDOVQ77780456 = -61990266;    float GQmPLtPLWJDeYkJNrKzDOVQ17920689 = -43951423;    float GQmPLtPLWJDeYkJNrKzDOVQ32581015 = -588389293;    float GQmPLtPLWJDeYkJNrKzDOVQ95065845 = -628016229;    float GQmPLtPLWJDeYkJNrKzDOVQ54811891 = -585965701;    float GQmPLtPLWJDeYkJNrKzDOVQ44179453 = -642901173;    float GQmPLtPLWJDeYkJNrKzDOVQ76905347 = -896161297;    float GQmPLtPLWJDeYkJNrKzDOVQ89125308 = -584978556;    float GQmPLtPLWJDeYkJNrKzDOVQ32456828 = -932351097;    float GQmPLtPLWJDeYkJNrKzDOVQ25112149 = -945178874;    float GQmPLtPLWJDeYkJNrKzDOVQ30192855 = -456933721;    float GQmPLtPLWJDeYkJNrKzDOVQ80810179 = -128025594;    float GQmPLtPLWJDeYkJNrKzDOVQ72841048 = -350060035;    float GQmPLtPLWJDeYkJNrKzDOVQ76767673 = -715571207;    float GQmPLtPLWJDeYkJNrKzDOVQ64816746 = -531551508;    float GQmPLtPLWJDeYkJNrKzDOVQ92721410 = -970341955;    float GQmPLtPLWJDeYkJNrKzDOVQ19498108 = -754113115;    float GQmPLtPLWJDeYkJNrKzDOVQ29558141 = 60308207;    float GQmPLtPLWJDeYkJNrKzDOVQ69903672 = -666995869;    float GQmPLtPLWJDeYkJNrKzDOVQ53865122 = -968864794;     GQmPLtPLWJDeYkJNrKzDOVQ69037012 = GQmPLtPLWJDeYkJNrKzDOVQ96061419;     GQmPLtPLWJDeYkJNrKzDOVQ96061419 = GQmPLtPLWJDeYkJNrKzDOVQ10854219;     GQmPLtPLWJDeYkJNrKzDOVQ10854219 = GQmPLtPLWJDeYkJNrKzDOVQ35910502;     GQmPLtPLWJDeYkJNrKzDOVQ35910502 = GQmPLtPLWJDeYkJNrKzDOVQ18806771;     GQmPLtPLWJDeYkJNrKzDOVQ18806771 = GQmPLtPLWJDeYkJNrKzDOVQ94121937;     GQmPLtPLWJDeYkJNrKzDOVQ94121937 = GQmPLtPLWJDeYkJNrKzDOVQ48639739;     GQmPLtPLWJDeYkJNrKzDOVQ48639739 = GQmPLtPLWJDeYkJNrKzDOVQ41377370;     GQmPLtPLWJDeYkJNrKzDOVQ41377370 = GQmPLtPLWJDeYkJNrKzDOVQ15239297;     GQmPLtPLWJDeYkJNrKzDOVQ15239297 = GQmPLtPLWJDeYkJNrKzDOVQ44190857;     GQmPLtPLWJDeYkJNrKzDOVQ44190857 = GQmPLtPLWJDeYkJNrKzDOVQ16374993;     GQmPLtPLWJDeYkJNrKzDOVQ16374993 = GQmPLtPLWJDeYkJNrKzDOVQ16694725;     GQmPLtPLWJDeYkJNrKzDOVQ16694725 = GQmPLtPLWJDeYkJNrKzDOVQ81529250;     GQmPLtPLWJDeYkJNrKzDOVQ81529250 = GQmPLtPLWJDeYkJNrKzDOVQ69125601;     GQmPLtPLWJDeYkJNrKzDOVQ69125601 = GQmPLtPLWJDeYkJNrKzDOVQ54991635;     GQmPLtPLWJDeYkJNrKzDOVQ54991635 = GQmPLtPLWJDeYkJNrKzDOVQ4576614;     GQmPLtPLWJDeYkJNrKzDOVQ4576614 = GQmPLtPLWJDeYkJNrKzDOVQ90330640;     GQmPLtPLWJDeYkJNrKzDOVQ90330640 = GQmPLtPLWJDeYkJNrKzDOVQ45502629;     GQmPLtPLWJDeYkJNrKzDOVQ45502629 = GQmPLtPLWJDeYkJNrKzDOVQ46187634;     GQmPLtPLWJDeYkJNrKzDOVQ46187634 = GQmPLtPLWJDeYkJNrKzDOVQ48384728;     GQmPLtPLWJDeYkJNrKzDOVQ48384728 = GQmPLtPLWJDeYkJNrKzDOVQ86023697;     GQmPLtPLWJDeYkJNrKzDOVQ86023697 = GQmPLtPLWJDeYkJNrKzDOVQ52926195;     GQmPLtPLWJDeYkJNrKzDOVQ52926195 = GQmPLtPLWJDeYkJNrKzDOVQ91619692;     GQmPLtPLWJDeYkJNrKzDOVQ91619692 = GQmPLtPLWJDeYkJNrKzDOVQ44650614;     GQmPLtPLWJDeYkJNrKzDOVQ44650614 = GQmPLtPLWJDeYkJNrKzDOVQ2600467;     GQmPLtPLWJDeYkJNrKzDOVQ2600467 = GQmPLtPLWJDeYkJNrKzDOVQ98147346;     GQmPLtPLWJDeYkJNrKzDOVQ98147346 = GQmPLtPLWJDeYkJNrKzDOVQ66543711;     GQmPLtPLWJDeYkJNrKzDOVQ66543711 = GQmPLtPLWJDeYkJNrKzDOVQ34767949;     GQmPLtPLWJDeYkJNrKzDOVQ34767949 = GQmPLtPLWJDeYkJNrKzDOVQ25609341;     GQmPLtPLWJDeYkJNrKzDOVQ25609341 = GQmPLtPLWJDeYkJNrKzDOVQ50773535;     GQmPLtPLWJDeYkJNrKzDOVQ50773535 = GQmPLtPLWJDeYkJNrKzDOVQ48650549;     GQmPLtPLWJDeYkJNrKzDOVQ48650549 = GQmPLtPLWJDeYkJNrKzDOVQ2461965;     GQmPLtPLWJDeYkJNrKzDOVQ2461965 = GQmPLtPLWJDeYkJNrKzDOVQ1818721;     GQmPLtPLWJDeYkJNrKzDOVQ1818721 = GQmPLtPLWJDeYkJNrKzDOVQ99935978;     GQmPLtPLWJDeYkJNrKzDOVQ99935978 = GQmPLtPLWJDeYkJNrKzDOVQ41222967;     GQmPLtPLWJDeYkJNrKzDOVQ41222967 = GQmPLtPLWJDeYkJNrKzDOVQ34634582;     GQmPLtPLWJDeYkJNrKzDOVQ34634582 = GQmPLtPLWJDeYkJNrKzDOVQ47013784;     GQmPLtPLWJDeYkJNrKzDOVQ47013784 = GQmPLtPLWJDeYkJNrKzDOVQ6151084;     GQmPLtPLWJDeYkJNrKzDOVQ6151084 = GQmPLtPLWJDeYkJNrKzDOVQ97427096;     GQmPLtPLWJDeYkJNrKzDOVQ97427096 = GQmPLtPLWJDeYkJNrKzDOVQ92394972;     GQmPLtPLWJDeYkJNrKzDOVQ92394972 = GQmPLtPLWJDeYkJNrKzDOVQ6593201;     GQmPLtPLWJDeYkJNrKzDOVQ6593201 = GQmPLtPLWJDeYkJNrKzDOVQ35266136;     GQmPLtPLWJDeYkJNrKzDOVQ35266136 = GQmPLtPLWJDeYkJNrKzDOVQ94023901;     GQmPLtPLWJDeYkJNrKzDOVQ94023901 = GQmPLtPLWJDeYkJNrKzDOVQ65443980;     GQmPLtPLWJDeYkJNrKzDOVQ65443980 = GQmPLtPLWJDeYkJNrKzDOVQ67778150;     GQmPLtPLWJDeYkJNrKzDOVQ67778150 = GQmPLtPLWJDeYkJNrKzDOVQ17706478;     GQmPLtPLWJDeYkJNrKzDOVQ17706478 = GQmPLtPLWJDeYkJNrKzDOVQ20366891;     GQmPLtPLWJDeYkJNrKzDOVQ20366891 = GQmPLtPLWJDeYkJNrKzDOVQ48623022;     GQmPLtPLWJDeYkJNrKzDOVQ48623022 = GQmPLtPLWJDeYkJNrKzDOVQ2186934;     GQmPLtPLWJDeYkJNrKzDOVQ2186934 = GQmPLtPLWJDeYkJNrKzDOVQ30543496;     GQmPLtPLWJDeYkJNrKzDOVQ30543496 = GQmPLtPLWJDeYkJNrKzDOVQ95961643;     GQmPLtPLWJDeYkJNrKzDOVQ95961643 = GQmPLtPLWJDeYkJNrKzDOVQ4471096;     GQmPLtPLWJDeYkJNrKzDOVQ4471096 = GQmPLtPLWJDeYkJNrKzDOVQ25556617;     GQmPLtPLWJDeYkJNrKzDOVQ25556617 = GQmPLtPLWJDeYkJNrKzDOVQ12693413;     GQmPLtPLWJDeYkJNrKzDOVQ12693413 = GQmPLtPLWJDeYkJNrKzDOVQ67479151;     GQmPLtPLWJDeYkJNrKzDOVQ67479151 = GQmPLtPLWJDeYkJNrKzDOVQ16110818;     GQmPLtPLWJDeYkJNrKzDOVQ16110818 = GQmPLtPLWJDeYkJNrKzDOVQ4441727;     GQmPLtPLWJDeYkJNrKzDOVQ4441727 = GQmPLtPLWJDeYkJNrKzDOVQ66203604;     GQmPLtPLWJDeYkJNrKzDOVQ66203604 = GQmPLtPLWJDeYkJNrKzDOVQ33310035;     GQmPLtPLWJDeYkJNrKzDOVQ33310035 = GQmPLtPLWJDeYkJNrKzDOVQ20659424;     GQmPLtPLWJDeYkJNrKzDOVQ20659424 = GQmPLtPLWJDeYkJNrKzDOVQ27578226;     GQmPLtPLWJDeYkJNrKzDOVQ27578226 = GQmPLtPLWJDeYkJNrKzDOVQ13871790;     GQmPLtPLWJDeYkJNrKzDOVQ13871790 = GQmPLtPLWJDeYkJNrKzDOVQ15768029;     GQmPLtPLWJDeYkJNrKzDOVQ15768029 = GQmPLtPLWJDeYkJNrKzDOVQ64465761;     GQmPLtPLWJDeYkJNrKzDOVQ64465761 = GQmPLtPLWJDeYkJNrKzDOVQ95540307;     GQmPLtPLWJDeYkJNrKzDOVQ95540307 = GQmPLtPLWJDeYkJNrKzDOVQ13913029;     GQmPLtPLWJDeYkJNrKzDOVQ13913029 = GQmPLtPLWJDeYkJNrKzDOVQ14876005;     GQmPLtPLWJDeYkJNrKzDOVQ14876005 = GQmPLtPLWJDeYkJNrKzDOVQ81593271;     GQmPLtPLWJDeYkJNrKzDOVQ81593271 = GQmPLtPLWJDeYkJNrKzDOVQ27902635;     GQmPLtPLWJDeYkJNrKzDOVQ27902635 = GQmPLtPLWJDeYkJNrKzDOVQ20357054;     GQmPLtPLWJDeYkJNrKzDOVQ20357054 = GQmPLtPLWJDeYkJNrKzDOVQ57562830;     GQmPLtPLWJDeYkJNrKzDOVQ57562830 = GQmPLtPLWJDeYkJNrKzDOVQ84179557;     GQmPLtPLWJDeYkJNrKzDOVQ84179557 = GQmPLtPLWJDeYkJNrKzDOVQ48075533;     GQmPLtPLWJDeYkJNrKzDOVQ48075533 = GQmPLtPLWJDeYkJNrKzDOVQ53792662;     GQmPLtPLWJDeYkJNrKzDOVQ53792662 = GQmPLtPLWJDeYkJNrKzDOVQ41791528;     GQmPLtPLWJDeYkJNrKzDOVQ41791528 = GQmPLtPLWJDeYkJNrKzDOVQ50757562;     GQmPLtPLWJDeYkJNrKzDOVQ50757562 = GQmPLtPLWJDeYkJNrKzDOVQ58902293;     GQmPLtPLWJDeYkJNrKzDOVQ58902293 = GQmPLtPLWJDeYkJNrKzDOVQ26175712;     GQmPLtPLWJDeYkJNrKzDOVQ26175712 = GQmPLtPLWJDeYkJNrKzDOVQ76872463;     GQmPLtPLWJDeYkJNrKzDOVQ76872463 = GQmPLtPLWJDeYkJNrKzDOVQ84893988;     GQmPLtPLWJDeYkJNrKzDOVQ84893988 = GQmPLtPLWJDeYkJNrKzDOVQ77780456;     GQmPLtPLWJDeYkJNrKzDOVQ77780456 = GQmPLtPLWJDeYkJNrKzDOVQ17920689;     GQmPLtPLWJDeYkJNrKzDOVQ17920689 = GQmPLtPLWJDeYkJNrKzDOVQ32581015;     GQmPLtPLWJDeYkJNrKzDOVQ32581015 = GQmPLtPLWJDeYkJNrKzDOVQ95065845;     GQmPLtPLWJDeYkJNrKzDOVQ95065845 = GQmPLtPLWJDeYkJNrKzDOVQ54811891;     GQmPLtPLWJDeYkJNrKzDOVQ54811891 = GQmPLtPLWJDeYkJNrKzDOVQ44179453;     GQmPLtPLWJDeYkJNrKzDOVQ44179453 = GQmPLtPLWJDeYkJNrKzDOVQ76905347;     GQmPLtPLWJDeYkJNrKzDOVQ76905347 = GQmPLtPLWJDeYkJNrKzDOVQ89125308;     GQmPLtPLWJDeYkJNrKzDOVQ89125308 = GQmPLtPLWJDeYkJNrKzDOVQ32456828;     GQmPLtPLWJDeYkJNrKzDOVQ32456828 = GQmPLtPLWJDeYkJNrKzDOVQ25112149;     GQmPLtPLWJDeYkJNrKzDOVQ25112149 = GQmPLtPLWJDeYkJNrKzDOVQ30192855;     GQmPLtPLWJDeYkJNrKzDOVQ30192855 = GQmPLtPLWJDeYkJNrKzDOVQ80810179;     GQmPLtPLWJDeYkJNrKzDOVQ80810179 = GQmPLtPLWJDeYkJNrKzDOVQ72841048;     GQmPLtPLWJDeYkJNrKzDOVQ72841048 = GQmPLtPLWJDeYkJNrKzDOVQ76767673;     GQmPLtPLWJDeYkJNrKzDOVQ76767673 = GQmPLtPLWJDeYkJNrKzDOVQ64816746;     GQmPLtPLWJDeYkJNrKzDOVQ64816746 = GQmPLtPLWJDeYkJNrKzDOVQ92721410;     GQmPLtPLWJDeYkJNrKzDOVQ92721410 = GQmPLtPLWJDeYkJNrKzDOVQ19498108;     GQmPLtPLWJDeYkJNrKzDOVQ19498108 = GQmPLtPLWJDeYkJNrKzDOVQ29558141;     GQmPLtPLWJDeYkJNrKzDOVQ29558141 = GQmPLtPLWJDeYkJNrKzDOVQ69903672;     GQmPLtPLWJDeYkJNrKzDOVQ69903672 = GQmPLtPLWJDeYkJNrKzDOVQ53865122;     GQmPLtPLWJDeYkJNrKzDOVQ53865122 = GQmPLtPLWJDeYkJNrKzDOVQ69037012;}



void mjcvzpqsXCdAtefmhwSeCmAfcd2685184() {     float HaTuvUqItLUUcgdRLBfRZfe83290742 = -48886684;    float HaTuvUqItLUUcgdRLBfRZfe75278730 = -941912829;    float HaTuvUqItLUUcgdRLBfRZfe57931646 = -703036970;    float HaTuvUqItLUUcgdRLBfRZfe10681272 = -518314186;    float HaTuvUqItLUUcgdRLBfRZfe131617 = -635977278;    float HaTuvUqItLUUcgdRLBfRZfe88570738 = 91908851;    float HaTuvUqItLUUcgdRLBfRZfe82825257 = -708990828;    float HaTuvUqItLUUcgdRLBfRZfe86770731 = -113287445;    float HaTuvUqItLUUcgdRLBfRZfe75457068 = -392782102;    float HaTuvUqItLUUcgdRLBfRZfe11527601 = -667719204;    float HaTuvUqItLUUcgdRLBfRZfe10120917 = -317869208;    float HaTuvUqItLUUcgdRLBfRZfe6984802 = -552779999;    float HaTuvUqItLUUcgdRLBfRZfe174672 = -266247033;    float HaTuvUqItLUUcgdRLBfRZfe43543761 = -911108887;    float HaTuvUqItLUUcgdRLBfRZfe59937460 = -146623077;    float HaTuvUqItLUUcgdRLBfRZfe3816130 = -712901724;    float HaTuvUqItLUUcgdRLBfRZfe98197128 = -597810298;    float HaTuvUqItLUUcgdRLBfRZfe16267281 = -456640744;    float HaTuvUqItLUUcgdRLBfRZfe67636302 = 90252519;    float HaTuvUqItLUUcgdRLBfRZfe9899511 = -36793169;    float HaTuvUqItLUUcgdRLBfRZfe60824492 = -38786297;    float HaTuvUqItLUUcgdRLBfRZfe83520249 = 29407621;    float HaTuvUqItLUUcgdRLBfRZfe1842152 = -31058699;    float HaTuvUqItLUUcgdRLBfRZfe88984167 = -292223446;    float HaTuvUqItLUUcgdRLBfRZfe10109743 = -173897937;    float HaTuvUqItLUUcgdRLBfRZfe75516162 = -339944138;    float HaTuvUqItLUUcgdRLBfRZfe96762915 = -629200825;    float HaTuvUqItLUUcgdRLBfRZfe35426355 = -540563499;    float HaTuvUqItLUUcgdRLBfRZfe8952138 = -418067262;    float HaTuvUqItLUUcgdRLBfRZfe95637475 = -960254237;    float HaTuvUqItLUUcgdRLBfRZfe8564057 = -556691243;    float HaTuvUqItLUUcgdRLBfRZfe83856906 = -210017961;    float HaTuvUqItLUUcgdRLBfRZfe75830424 = -266499942;    float HaTuvUqItLUUcgdRLBfRZfe76974895 = -540583122;    float HaTuvUqItLUUcgdRLBfRZfe39485466 = -933717616;    float HaTuvUqItLUUcgdRLBfRZfe79136925 = -540694099;    float HaTuvUqItLUUcgdRLBfRZfe56379265 = -574175247;    float HaTuvUqItLUUcgdRLBfRZfe82529820 = -262487597;    float HaTuvUqItLUUcgdRLBfRZfe49557890 = 48622558;    float HaTuvUqItLUUcgdRLBfRZfe3270576 = -104145853;    float HaTuvUqItLUUcgdRLBfRZfe26842581 = -21617021;    float HaTuvUqItLUUcgdRLBfRZfe49171145 = -359022146;    float HaTuvUqItLUUcgdRLBfRZfe45761646 = -93927367;    float HaTuvUqItLUUcgdRLBfRZfe69061936 = -946024562;    float HaTuvUqItLUUcgdRLBfRZfe86741503 = -145266097;    float HaTuvUqItLUUcgdRLBfRZfe17619225 = -284516190;    float HaTuvUqItLUUcgdRLBfRZfe28480428 = -772003320;    float HaTuvUqItLUUcgdRLBfRZfe31054935 = -187349905;    float HaTuvUqItLUUcgdRLBfRZfe24590154 = -479010128;    float HaTuvUqItLUUcgdRLBfRZfe80529839 = -61008012;    float HaTuvUqItLUUcgdRLBfRZfe14931749 = -528309235;    float HaTuvUqItLUUcgdRLBfRZfe93164769 = -758481198;    float HaTuvUqItLUUcgdRLBfRZfe44975960 = -264889927;    float HaTuvUqItLUUcgdRLBfRZfe38258557 = -136124522;    float HaTuvUqItLUUcgdRLBfRZfe41860692 = -195961280;    float HaTuvUqItLUUcgdRLBfRZfe99770493 = 21705694;    float HaTuvUqItLUUcgdRLBfRZfe73436579 = -810854131;    float HaTuvUqItLUUcgdRLBfRZfe68947479 = -310813525;    float HaTuvUqItLUUcgdRLBfRZfe571529 = -244416249;    float HaTuvUqItLUUcgdRLBfRZfe24615454 = -196033140;    float HaTuvUqItLUUcgdRLBfRZfe91807823 = -278890325;    float HaTuvUqItLUUcgdRLBfRZfe47398902 = -68427329;    float HaTuvUqItLUUcgdRLBfRZfe77818594 = -695220184;    float HaTuvUqItLUUcgdRLBfRZfe79819592 = -432527866;    float HaTuvUqItLUUcgdRLBfRZfe2963544 = -11027961;    float HaTuvUqItLUUcgdRLBfRZfe26264011 = -7851247;    float HaTuvUqItLUUcgdRLBfRZfe31154378 = -186280057;    float HaTuvUqItLUUcgdRLBfRZfe23199776 = -725663912;    float HaTuvUqItLUUcgdRLBfRZfe4058296 = -977391272;    float HaTuvUqItLUUcgdRLBfRZfe80800534 = -605928979;    float HaTuvUqItLUUcgdRLBfRZfe47436865 = -38726477;    float HaTuvUqItLUUcgdRLBfRZfe15667308 = -235322701;    float HaTuvUqItLUUcgdRLBfRZfe66709390 = -405263302;    float HaTuvUqItLUUcgdRLBfRZfe64365727 = -805601628;    float HaTuvUqItLUUcgdRLBfRZfe83056930 = 84823851;    float HaTuvUqItLUUcgdRLBfRZfe11653347 = -679764152;    float HaTuvUqItLUUcgdRLBfRZfe37758603 = -876665012;    float HaTuvUqItLUUcgdRLBfRZfe32780215 = -85034138;    float HaTuvUqItLUUcgdRLBfRZfe2242664 = -46957349;    float HaTuvUqItLUUcgdRLBfRZfe92490518 = -889381747;    float HaTuvUqItLUUcgdRLBfRZfe47035734 = -567940818;    float HaTuvUqItLUUcgdRLBfRZfe65707980 = -341850921;    float HaTuvUqItLUUcgdRLBfRZfe10836202 = 38446628;    float HaTuvUqItLUUcgdRLBfRZfe28422298 = -257059250;    float HaTuvUqItLUUcgdRLBfRZfe80705727 = -331945002;    float HaTuvUqItLUUcgdRLBfRZfe15399287 = -798210045;    float HaTuvUqItLUUcgdRLBfRZfe38880946 = -945128035;    float HaTuvUqItLUUcgdRLBfRZfe37571867 = -30375421;    float HaTuvUqItLUUcgdRLBfRZfe35114203 = -244621842;    float HaTuvUqItLUUcgdRLBfRZfe39714972 = -855423310;    float HaTuvUqItLUUcgdRLBfRZfe5700347 = -729839969;    float HaTuvUqItLUUcgdRLBfRZfe87431786 = -163361723;    float HaTuvUqItLUUcgdRLBfRZfe81958291 = 81928652;    float HaTuvUqItLUUcgdRLBfRZfe24942436 = -755344303;    float HaTuvUqItLUUcgdRLBfRZfe11462752 = -825255529;    float HaTuvUqItLUUcgdRLBfRZfe79443679 = -953189692;    float HaTuvUqItLUUcgdRLBfRZfe71352551 = -663801962;    float HaTuvUqItLUUcgdRLBfRZfe65942054 = -661399502;    float HaTuvUqItLUUcgdRLBfRZfe66098392 = -834996601;    float HaTuvUqItLUUcgdRLBfRZfe60477493 = -48886684;     HaTuvUqItLUUcgdRLBfRZfe83290742 = HaTuvUqItLUUcgdRLBfRZfe75278730;     HaTuvUqItLUUcgdRLBfRZfe75278730 = HaTuvUqItLUUcgdRLBfRZfe57931646;     HaTuvUqItLUUcgdRLBfRZfe57931646 = HaTuvUqItLUUcgdRLBfRZfe10681272;     HaTuvUqItLUUcgdRLBfRZfe10681272 = HaTuvUqItLUUcgdRLBfRZfe131617;     HaTuvUqItLUUcgdRLBfRZfe131617 = HaTuvUqItLUUcgdRLBfRZfe88570738;     HaTuvUqItLUUcgdRLBfRZfe88570738 = HaTuvUqItLUUcgdRLBfRZfe82825257;     HaTuvUqItLUUcgdRLBfRZfe82825257 = HaTuvUqItLUUcgdRLBfRZfe86770731;     HaTuvUqItLUUcgdRLBfRZfe86770731 = HaTuvUqItLUUcgdRLBfRZfe75457068;     HaTuvUqItLUUcgdRLBfRZfe75457068 = HaTuvUqItLUUcgdRLBfRZfe11527601;     HaTuvUqItLUUcgdRLBfRZfe11527601 = HaTuvUqItLUUcgdRLBfRZfe10120917;     HaTuvUqItLUUcgdRLBfRZfe10120917 = HaTuvUqItLUUcgdRLBfRZfe6984802;     HaTuvUqItLUUcgdRLBfRZfe6984802 = HaTuvUqItLUUcgdRLBfRZfe174672;     HaTuvUqItLUUcgdRLBfRZfe174672 = HaTuvUqItLUUcgdRLBfRZfe43543761;     HaTuvUqItLUUcgdRLBfRZfe43543761 = HaTuvUqItLUUcgdRLBfRZfe59937460;     HaTuvUqItLUUcgdRLBfRZfe59937460 = HaTuvUqItLUUcgdRLBfRZfe3816130;     HaTuvUqItLUUcgdRLBfRZfe3816130 = HaTuvUqItLUUcgdRLBfRZfe98197128;     HaTuvUqItLUUcgdRLBfRZfe98197128 = HaTuvUqItLUUcgdRLBfRZfe16267281;     HaTuvUqItLUUcgdRLBfRZfe16267281 = HaTuvUqItLUUcgdRLBfRZfe67636302;     HaTuvUqItLUUcgdRLBfRZfe67636302 = HaTuvUqItLUUcgdRLBfRZfe9899511;     HaTuvUqItLUUcgdRLBfRZfe9899511 = HaTuvUqItLUUcgdRLBfRZfe60824492;     HaTuvUqItLUUcgdRLBfRZfe60824492 = HaTuvUqItLUUcgdRLBfRZfe83520249;     HaTuvUqItLUUcgdRLBfRZfe83520249 = HaTuvUqItLUUcgdRLBfRZfe1842152;     HaTuvUqItLUUcgdRLBfRZfe1842152 = HaTuvUqItLUUcgdRLBfRZfe88984167;     HaTuvUqItLUUcgdRLBfRZfe88984167 = HaTuvUqItLUUcgdRLBfRZfe10109743;     HaTuvUqItLUUcgdRLBfRZfe10109743 = HaTuvUqItLUUcgdRLBfRZfe75516162;     HaTuvUqItLUUcgdRLBfRZfe75516162 = HaTuvUqItLUUcgdRLBfRZfe96762915;     HaTuvUqItLUUcgdRLBfRZfe96762915 = HaTuvUqItLUUcgdRLBfRZfe35426355;     HaTuvUqItLUUcgdRLBfRZfe35426355 = HaTuvUqItLUUcgdRLBfRZfe8952138;     HaTuvUqItLUUcgdRLBfRZfe8952138 = HaTuvUqItLUUcgdRLBfRZfe95637475;     HaTuvUqItLUUcgdRLBfRZfe95637475 = HaTuvUqItLUUcgdRLBfRZfe8564057;     HaTuvUqItLUUcgdRLBfRZfe8564057 = HaTuvUqItLUUcgdRLBfRZfe83856906;     HaTuvUqItLUUcgdRLBfRZfe83856906 = HaTuvUqItLUUcgdRLBfRZfe75830424;     HaTuvUqItLUUcgdRLBfRZfe75830424 = HaTuvUqItLUUcgdRLBfRZfe76974895;     HaTuvUqItLUUcgdRLBfRZfe76974895 = HaTuvUqItLUUcgdRLBfRZfe39485466;     HaTuvUqItLUUcgdRLBfRZfe39485466 = HaTuvUqItLUUcgdRLBfRZfe79136925;     HaTuvUqItLUUcgdRLBfRZfe79136925 = HaTuvUqItLUUcgdRLBfRZfe56379265;     HaTuvUqItLUUcgdRLBfRZfe56379265 = HaTuvUqItLUUcgdRLBfRZfe82529820;     HaTuvUqItLUUcgdRLBfRZfe82529820 = HaTuvUqItLUUcgdRLBfRZfe49557890;     HaTuvUqItLUUcgdRLBfRZfe49557890 = HaTuvUqItLUUcgdRLBfRZfe3270576;     HaTuvUqItLUUcgdRLBfRZfe3270576 = HaTuvUqItLUUcgdRLBfRZfe26842581;     HaTuvUqItLUUcgdRLBfRZfe26842581 = HaTuvUqItLUUcgdRLBfRZfe49171145;     HaTuvUqItLUUcgdRLBfRZfe49171145 = HaTuvUqItLUUcgdRLBfRZfe45761646;     HaTuvUqItLUUcgdRLBfRZfe45761646 = HaTuvUqItLUUcgdRLBfRZfe69061936;     HaTuvUqItLUUcgdRLBfRZfe69061936 = HaTuvUqItLUUcgdRLBfRZfe86741503;     HaTuvUqItLUUcgdRLBfRZfe86741503 = HaTuvUqItLUUcgdRLBfRZfe17619225;     HaTuvUqItLUUcgdRLBfRZfe17619225 = HaTuvUqItLUUcgdRLBfRZfe28480428;     HaTuvUqItLUUcgdRLBfRZfe28480428 = HaTuvUqItLUUcgdRLBfRZfe31054935;     HaTuvUqItLUUcgdRLBfRZfe31054935 = HaTuvUqItLUUcgdRLBfRZfe24590154;     HaTuvUqItLUUcgdRLBfRZfe24590154 = HaTuvUqItLUUcgdRLBfRZfe80529839;     HaTuvUqItLUUcgdRLBfRZfe80529839 = HaTuvUqItLUUcgdRLBfRZfe14931749;     HaTuvUqItLUUcgdRLBfRZfe14931749 = HaTuvUqItLUUcgdRLBfRZfe93164769;     HaTuvUqItLUUcgdRLBfRZfe93164769 = HaTuvUqItLUUcgdRLBfRZfe44975960;     HaTuvUqItLUUcgdRLBfRZfe44975960 = HaTuvUqItLUUcgdRLBfRZfe38258557;     HaTuvUqItLUUcgdRLBfRZfe38258557 = HaTuvUqItLUUcgdRLBfRZfe41860692;     HaTuvUqItLUUcgdRLBfRZfe41860692 = HaTuvUqItLUUcgdRLBfRZfe99770493;     HaTuvUqItLUUcgdRLBfRZfe99770493 = HaTuvUqItLUUcgdRLBfRZfe73436579;     HaTuvUqItLUUcgdRLBfRZfe73436579 = HaTuvUqItLUUcgdRLBfRZfe68947479;     HaTuvUqItLUUcgdRLBfRZfe68947479 = HaTuvUqItLUUcgdRLBfRZfe571529;     HaTuvUqItLUUcgdRLBfRZfe571529 = HaTuvUqItLUUcgdRLBfRZfe24615454;     HaTuvUqItLUUcgdRLBfRZfe24615454 = HaTuvUqItLUUcgdRLBfRZfe91807823;     HaTuvUqItLUUcgdRLBfRZfe91807823 = HaTuvUqItLUUcgdRLBfRZfe47398902;     HaTuvUqItLUUcgdRLBfRZfe47398902 = HaTuvUqItLUUcgdRLBfRZfe77818594;     HaTuvUqItLUUcgdRLBfRZfe77818594 = HaTuvUqItLUUcgdRLBfRZfe79819592;     HaTuvUqItLUUcgdRLBfRZfe79819592 = HaTuvUqItLUUcgdRLBfRZfe2963544;     HaTuvUqItLUUcgdRLBfRZfe2963544 = HaTuvUqItLUUcgdRLBfRZfe26264011;     HaTuvUqItLUUcgdRLBfRZfe26264011 = HaTuvUqItLUUcgdRLBfRZfe31154378;     HaTuvUqItLUUcgdRLBfRZfe31154378 = HaTuvUqItLUUcgdRLBfRZfe23199776;     HaTuvUqItLUUcgdRLBfRZfe23199776 = HaTuvUqItLUUcgdRLBfRZfe4058296;     HaTuvUqItLUUcgdRLBfRZfe4058296 = HaTuvUqItLUUcgdRLBfRZfe80800534;     HaTuvUqItLUUcgdRLBfRZfe80800534 = HaTuvUqItLUUcgdRLBfRZfe47436865;     HaTuvUqItLUUcgdRLBfRZfe47436865 = HaTuvUqItLUUcgdRLBfRZfe15667308;     HaTuvUqItLUUcgdRLBfRZfe15667308 = HaTuvUqItLUUcgdRLBfRZfe66709390;     HaTuvUqItLUUcgdRLBfRZfe66709390 = HaTuvUqItLUUcgdRLBfRZfe64365727;     HaTuvUqItLUUcgdRLBfRZfe64365727 = HaTuvUqItLUUcgdRLBfRZfe83056930;     HaTuvUqItLUUcgdRLBfRZfe83056930 = HaTuvUqItLUUcgdRLBfRZfe11653347;     HaTuvUqItLUUcgdRLBfRZfe11653347 = HaTuvUqItLUUcgdRLBfRZfe37758603;     HaTuvUqItLUUcgdRLBfRZfe37758603 = HaTuvUqItLUUcgdRLBfRZfe32780215;     HaTuvUqItLUUcgdRLBfRZfe32780215 = HaTuvUqItLUUcgdRLBfRZfe2242664;     HaTuvUqItLUUcgdRLBfRZfe2242664 = HaTuvUqItLUUcgdRLBfRZfe92490518;     HaTuvUqItLUUcgdRLBfRZfe92490518 = HaTuvUqItLUUcgdRLBfRZfe47035734;     HaTuvUqItLUUcgdRLBfRZfe47035734 = HaTuvUqItLUUcgdRLBfRZfe65707980;     HaTuvUqItLUUcgdRLBfRZfe65707980 = HaTuvUqItLUUcgdRLBfRZfe10836202;     HaTuvUqItLUUcgdRLBfRZfe10836202 = HaTuvUqItLUUcgdRLBfRZfe28422298;     HaTuvUqItLUUcgdRLBfRZfe28422298 = HaTuvUqItLUUcgdRLBfRZfe80705727;     HaTuvUqItLUUcgdRLBfRZfe80705727 = HaTuvUqItLUUcgdRLBfRZfe15399287;     HaTuvUqItLUUcgdRLBfRZfe15399287 = HaTuvUqItLUUcgdRLBfRZfe38880946;     HaTuvUqItLUUcgdRLBfRZfe38880946 = HaTuvUqItLUUcgdRLBfRZfe37571867;     HaTuvUqItLUUcgdRLBfRZfe37571867 = HaTuvUqItLUUcgdRLBfRZfe35114203;     HaTuvUqItLUUcgdRLBfRZfe35114203 = HaTuvUqItLUUcgdRLBfRZfe39714972;     HaTuvUqItLUUcgdRLBfRZfe39714972 = HaTuvUqItLUUcgdRLBfRZfe5700347;     HaTuvUqItLUUcgdRLBfRZfe5700347 = HaTuvUqItLUUcgdRLBfRZfe87431786;     HaTuvUqItLUUcgdRLBfRZfe87431786 = HaTuvUqItLUUcgdRLBfRZfe81958291;     HaTuvUqItLUUcgdRLBfRZfe81958291 = HaTuvUqItLUUcgdRLBfRZfe24942436;     HaTuvUqItLUUcgdRLBfRZfe24942436 = HaTuvUqItLUUcgdRLBfRZfe11462752;     HaTuvUqItLUUcgdRLBfRZfe11462752 = HaTuvUqItLUUcgdRLBfRZfe79443679;     HaTuvUqItLUUcgdRLBfRZfe79443679 = HaTuvUqItLUUcgdRLBfRZfe71352551;     HaTuvUqItLUUcgdRLBfRZfe71352551 = HaTuvUqItLUUcgdRLBfRZfe65942054;     HaTuvUqItLUUcgdRLBfRZfe65942054 = HaTuvUqItLUUcgdRLBfRZfe66098392;     HaTuvUqItLUUcgdRLBfRZfe66098392 = HaTuvUqItLUUcgdRLBfRZfe60477493;     HaTuvUqItLUUcgdRLBfRZfe60477493 = HaTuvUqItLUUcgdRLBfRZfe83290742;}



void AqSqYGEbByhYQzmTErCDtOlzpc87748021() {     float GoIOEjaMZuTyPkXdnDonzKy67014260 = -398908751;    float GoIOEjaMZuTyPkXdnDonzKy72876825 = -745564680;    float GoIOEjaMZuTyPkXdnDonzKy46517764 = -571344661;    float GoIOEjaMZuTyPkXdnDonzKy7829225 = -523196115;    float GoIOEjaMZuTyPkXdnDonzKy79692952 = 66074531;    float GoIOEjaMZuTyPkXdnDonzKy99910256 = -11411818;    float GoIOEjaMZuTyPkXdnDonzKy19705820 = -730504966;    float GoIOEjaMZuTyPkXdnDonzKy30110815 = -733742032;    float GoIOEjaMZuTyPkXdnDonzKy25676597 = 9735202;    float GoIOEjaMZuTyPkXdnDonzKy57633189 = -890760220;    float GoIOEjaMZuTyPkXdnDonzKy53009952 = -158154466;    float GoIOEjaMZuTyPkXdnDonzKy97196572 = -481919270;    float GoIOEjaMZuTyPkXdnDonzKy51228523 = -109079276;    float GoIOEjaMZuTyPkXdnDonzKy37916906 = -160791749;    float GoIOEjaMZuTyPkXdnDonzKy64116718 = -72320382;    float GoIOEjaMZuTyPkXdnDonzKy75630158 = -637029375;    float GoIOEjaMZuTyPkXdnDonzKy98062538 = -756690160;    float GoIOEjaMZuTyPkXdnDonzKy30344551 = -67492318;    float GoIOEjaMZuTyPkXdnDonzKy8612784 = -798396720;    float GoIOEjaMZuTyPkXdnDonzKy84007155 = -753086086;    float GoIOEjaMZuTyPkXdnDonzKy28164002 = -713252818;    float GoIOEjaMZuTyPkXdnDonzKy84522320 = -983747665;    float GoIOEjaMZuTyPkXdnDonzKy71018016 = -60453436;    float GoIOEjaMZuTyPkXdnDonzKy90126862 = -931456099;    float GoIOEjaMZuTyPkXdnDonzKy46711836 = -252228699;    float GoIOEjaMZuTyPkXdnDonzKy28508920 = -782911166;    float GoIOEjaMZuTyPkXdnDonzKy18354855 = 26923752;    float GoIOEjaMZuTyPkXdnDonzKy78025555 = -327004988;    float GoIOEjaMZuTyPkXdnDonzKy80870279 = -942263162;    float GoIOEjaMZuTyPkXdnDonzKy31185803 = -532860611;    float GoIOEjaMZuTyPkXdnDonzKy89928479 = -775789264;    float GoIOEjaMZuTyPkXdnDonzKy8650195 = -759215184;    float GoIOEjaMZuTyPkXdnDonzKy89955125 = -931714033;    float GoIOEjaMZuTyPkXdnDonzKy12699610 = -944679769;    float GoIOEjaMZuTyPkXdnDonzKy7088791 = -668008223;    float GoIOEjaMZuTyPkXdnDonzKy48998159 = -519090576;    float GoIOEjaMZuTyPkXdnDonzKy56142855 = -894516544;    float GoIOEjaMZuTyPkXdnDonzKy32911612 = -161630814;    float GoIOEjaMZuTyPkXdnDonzKy10173610 = -256936324;    float GoIOEjaMZuTyPkXdnDonzKy42459691 = -50792733;    float GoIOEjaMZuTyPkXdnDonzKy91610101 = -771657147;    float GoIOEjaMZuTyPkXdnDonzKy78510871 = 34952185;    float GoIOEjaMZuTyPkXdnDonzKy78561792 = -846331621;    float GoIOEjaMZuTyPkXdnDonzKy16257455 = -524968294;    float GoIOEjaMZuTyPkXdnDonzKy14728755 = -471280175;    float GoIOEjaMZuTyPkXdnDonzKy61886106 = -999547882;    float GoIOEjaMZuTyPkXdnDonzKy61659397 = -793649227;    float GoIOEjaMZuTyPkXdnDonzKy69796782 = 50528198;    float GoIOEjaMZuTyPkXdnDonzKy58464367 = -178794279;    float GoIOEjaMZuTyPkXdnDonzKy16403170 = -693023448;    float GoIOEjaMZuTyPkXdnDonzKy55022580 = -694081636;    float GoIOEjaMZuTyPkXdnDonzKy44670490 = -510533985;    float GoIOEjaMZuTyPkXdnDonzKy90358362 = -276822293;    float GoIOEjaMZuTyPkXdnDonzKy84191163 = -712833709;    float GoIOEjaMZuTyPkXdnDonzKy59584019 = -512688225;    float GoIOEjaMZuTyPkXdnDonzKy82491940 = -415161087;    float GoIOEjaMZuTyPkXdnDonzKy1858810 = -585111245;    float GoIOEjaMZuTyPkXdnDonzKy56390901 = -639888562;    float GoIOEjaMZuTyPkXdnDonzKy61117389 = -170967417;    float GoIOEjaMZuTyPkXdnDonzKy51184033 = -151014304;    float GoIOEjaMZuTyPkXdnDonzKy81555402 = 61664429;    float GoIOEjaMZuTyPkXdnDonzKy41680265 = -303499979;    float GoIOEjaMZuTyPkXdnDonzKy49240535 = -791478870;    float GoIOEjaMZuTyPkXdnDonzKy94490793 = -457404188;    float GoIOEjaMZuTyPkXdnDonzKy67704709 = -14970956;    float GoIOEjaMZuTyPkXdnDonzKy44359758 = -398939282;    float GoIOEjaMZuTyPkXdnDonzKy7241447 = -550205237;    float GoIOEjaMZuTyPkXdnDonzKy38528914 = -164399508;    float GoIOEjaMZuTyPkXdnDonzKy30828115 = -492783527;    float GoIOEjaMZuTyPkXdnDonzKy15118560 = -553229807;    float GoIOEjaMZuTyPkXdnDonzKy19487303 = -742512832;    float GoIOEjaMZuTyPkXdnDonzKy65150927 = -495059347;    float GoIOEjaMZuTyPkXdnDonzKy20170941 = -810555995;    float GoIOEjaMZuTyPkXdnDonzKy66153092 = -647603987;    float GoIOEjaMZuTyPkXdnDonzKy92397054 = -981428939;    float GoIOEjaMZuTyPkXdnDonzKy49653130 = -648205003;    float GoIOEjaMZuTyPkXdnDonzKy5960528 = -37416044;    float GoIOEjaMZuTyPkXdnDonzKy54760561 = -535485142;    float GoIOEjaMZuTyPkXdnDonzKy75398108 = -360175924;    float GoIOEjaMZuTyPkXdnDonzKy84825729 = -252680817;    float GoIOEjaMZuTyPkXdnDonzKy66849522 = -989261939;    float GoIOEjaMZuTyPkXdnDonzKy48558072 = 76395554;    float GoIOEjaMZuTyPkXdnDonzKy19561189 = -48210709;    float GoIOEjaMZuTyPkXdnDonzKy64467110 = -149239715;    float GoIOEjaMZuTyPkXdnDonzKy76163222 = -838778975;    float GoIOEjaMZuTyPkXdnDonzKy45257990 = -165255280;    float GoIOEjaMZuTyPkXdnDonzKy18291833 = -382392892;    float GoIOEjaMZuTyPkXdnDonzKy5763962 = -118880324;    float GoIOEjaMZuTyPkXdnDonzKy53115590 = -331991545;    float GoIOEjaMZuTyPkXdnDonzKy24596851 = -152847137;    float GoIOEjaMZuTyPkXdnDonzKy47139350 = -933979332;    float GoIOEjaMZuTyPkXdnDonzKy99751954 = -154627982;    float GoIOEjaMZuTyPkXdnDonzKy71794222 = -990663398;    float GoIOEjaMZuTyPkXdnDonzKy58989576 = -5922020;    float GoIOEjaMZuTyPkXdnDonzKy60904289 = -12457162;    float GoIOEjaMZuTyPkXdnDonzKy49929837 = -368157169;    float GoIOEjaMZuTyPkXdnDonzKy29270336 = -173568946;    float GoIOEjaMZuTyPkXdnDonzKy84070999 = -288927434;    float GoIOEjaMZuTyPkXdnDonzKy48552746 = -409997338;    float GoIOEjaMZuTyPkXdnDonzKy70368996 = -398908751;     GoIOEjaMZuTyPkXdnDonzKy67014260 = GoIOEjaMZuTyPkXdnDonzKy72876825;     GoIOEjaMZuTyPkXdnDonzKy72876825 = GoIOEjaMZuTyPkXdnDonzKy46517764;     GoIOEjaMZuTyPkXdnDonzKy46517764 = GoIOEjaMZuTyPkXdnDonzKy7829225;     GoIOEjaMZuTyPkXdnDonzKy7829225 = GoIOEjaMZuTyPkXdnDonzKy79692952;     GoIOEjaMZuTyPkXdnDonzKy79692952 = GoIOEjaMZuTyPkXdnDonzKy99910256;     GoIOEjaMZuTyPkXdnDonzKy99910256 = GoIOEjaMZuTyPkXdnDonzKy19705820;     GoIOEjaMZuTyPkXdnDonzKy19705820 = GoIOEjaMZuTyPkXdnDonzKy30110815;     GoIOEjaMZuTyPkXdnDonzKy30110815 = GoIOEjaMZuTyPkXdnDonzKy25676597;     GoIOEjaMZuTyPkXdnDonzKy25676597 = GoIOEjaMZuTyPkXdnDonzKy57633189;     GoIOEjaMZuTyPkXdnDonzKy57633189 = GoIOEjaMZuTyPkXdnDonzKy53009952;     GoIOEjaMZuTyPkXdnDonzKy53009952 = GoIOEjaMZuTyPkXdnDonzKy97196572;     GoIOEjaMZuTyPkXdnDonzKy97196572 = GoIOEjaMZuTyPkXdnDonzKy51228523;     GoIOEjaMZuTyPkXdnDonzKy51228523 = GoIOEjaMZuTyPkXdnDonzKy37916906;     GoIOEjaMZuTyPkXdnDonzKy37916906 = GoIOEjaMZuTyPkXdnDonzKy64116718;     GoIOEjaMZuTyPkXdnDonzKy64116718 = GoIOEjaMZuTyPkXdnDonzKy75630158;     GoIOEjaMZuTyPkXdnDonzKy75630158 = GoIOEjaMZuTyPkXdnDonzKy98062538;     GoIOEjaMZuTyPkXdnDonzKy98062538 = GoIOEjaMZuTyPkXdnDonzKy30344551;     GoIOEjaMZuTyPkXdnDonzKy30344551 = GoIOEjaMZuTyPkXdnDonzKy8612784;     GoIOEjaMZuTyPkXdnDonzKy8612784 = GoIOEjaMZuTyPkXdnDonzKy84007155;     GoIOEjaMZuTyPkXdnDonzKy84007155 = GoIOEjaMZuTyPkXdnDonzKy28164002;     GoIOEjaMZuTyPkXdnDonzKy28164002 = GoIOEjaMZuTyPkXdnDonzKy84522320;     GoIOEjaMZuTyPkXdnDonzKy84522320 = GoIOEjaMZuTyPkXdnDonzKy71018016;     GoIOEjaMZuTyPkXdnDonzKy71018016 = GoIOEjaMZuTyPkXdnDonzKy90126862;     GoIOEjaMZuTyPkXdnDonzKy90126862 = GoIOEjaMZuTyPkXdnDonzKy46711836;     GoIOEjaMZuTyPkXdnDonzKy46711836 = GoIOEjaMZuTyPkXdnDonzKy28508920;     GoIOEjaMZuTyPkXdnDonzKy28508920 = GoIOEjaMZuTyPkXdnDonzKy18354855;     GoIOEjaMZuTyPkXdnDonzKy18354855 = GoIOEjaMZuTyPkXdnDonzKy78025555;     GoIOEjaMZuTyPkXdnDonzKy78025555 = GoIOEjaMZuTyPkXdnDonzKy80870279;     GoIOEjaMZuTyPkXdnDonzKy80870279 = GoIOEjaMZuTyPkXdnDonzKy31185803;     GoIOEjaMZuTyPkXdnDonzKy31185803 = GoIOEjaMZuTyPkXdnDonzKy89928479;     GoIOEjaMZuTyPkXdnDonzKy89928479 = GoIOEjaMZuTyPkXdnDonzKy8650195;     GoIOEjaMZuTyPkXdnDonzKy8650195 = GoIOEjaMZuTyPkXdnDonzKy89955125;     GoIOEjaMZuTyPkXdnDonzKy89955125 = GoIOEjaMZuTyPkXdnDonzKy12699610;     GoIOEjaMZuTyPkXdnDonzKy12699610 = GoIOEjaMZuTyPkXdnDonzKy7088791;     GoIOEjaMZuTyPkXdnDonzKy7088791 = GoIOEjaMZuTyPkXdnDonzKy48998159;     GoIOEjaMZuTyPkXdnDonzKy48998159 = GoIOEjaMZuTyPkXdnDonzKy56142855;     GoIOEjaMZuTyPkXdnDonzKy56142855 = GoIOEjaMZuTyPkXdnDonzKy32911612;     GoIOEjaMZuTyPkXdnDonzKy32911612 = GoIOEjaMZuTyPkXdnDonzKy10173610;     GoIOEjaMZuTyPkXdnDonzKy10173610 = GoIOEjaMZuTyPkXdnDonzKy42459691;     GoIOEjaMZuTyPkXdnDonzKy42459691 = GoIOEjaMZuTyPkXdnDonzKy91610101;     GoIOEjaMZuTyPkXdnDonzKy91610101 = GoIOEjaMZuTyPkXdnDonzKy78510871;     GoIOEjaMZuTyPkXdnDonzKy78510871 = GoIOEjaMZuTyPkXdnDonzKy78561792;     GoIOEjaMZuTyPkXdnDonzKy78561792 = GoIOEjaMZuTyPkXdnDonzKy16257455;     GoIOEjaMZuTyPkXdnDonzKy16257455 = GoIOEjaMZuTyPkXdnDonzKy14728755;     GoIOEjaMZuTyPkXdnDonzKy14728755 = GoIOEjaMZuTyPkXdnDonzKy61886106;     GoIOEjaMZuTyPkXdnDonzKy61886106 = GoIOEjaMZuTyPkXdnDonzKy61659397;     GoIOEjaMZuTyPkXdnDonzKy61659397 = GoIOEjaMZuTyPkXdnDonzKy69796782;     GoIOEjaMZuTyPkXdnDonzKy69796782 = GoIOEjaMZuTyPkXdnDonzKy58464367;     GoIOEjaMZuTyPkXdnDonzKy58464367 = GoIOEjaMZuTyPkXdnDonzKy16403170;     GoIOEjaMZuTyPkXdnDonzKy16403170 = GoIOEjaMZuTyPkXdnDonzKy55022580;     GoIOEjaMZuTyPkXdnDonzKy55022580 = GoIOEjaMZuTyPkXdnDonzKy44670490;     GoIOEjaMZuTyPkXdnDonzKy44670490 = GoIOEjaMZuTyPkXdnDonzKy90358362;     GoIOEjaMZuTyPkXdnDonzKy90358362 = GoIOEjaMZuTyPkXdnDonzKy84191163;     GoIOEjaMZuTyPkXdnDonzKy84191163 = GoIOEjaMZuTyPkXdnDonzKy59584019;     GoIOEjaMZuTyPkXdnDonzKy59584019 = GoIOEjaMZuTyPkXdnDonzKy82491940;     GoIOEjaMZuTyPkXdnDonzKy82491940 = GoIOEjaMZuTyPkXdnDonzKy1858810;     GoIOEjaMZuTyPkXdnDonzKy1858810 = GoIOEjaMZuTyPkXdnDonzKy56390901;     GoIOEjaMZuTyPkXdnDonzKy56390901 = GoIOEjaMZuTyPkXdnDonzKy61117389;     GoIOEjaMZuTyPkXdnDonzKy61117389 = GoIOEjaMZuTyPkXdnDonzKy51184033;     GoIOEjaMZuTyPkXdnDonzKy51184033 = GoIOEjaMZuTyPkXdnDonzKy81555402;     GoIOEjaMZuTyPkXdnDonzKy81555402 = GoIOEjaMZuTyPkXdnDonzKy41680265;     GoIOEjaMZuTyPkXdnDonzKy41680265 = GoIOEjaMZuTyPkXdnDonzKy49240535;     GoIOEjaMZuTyPkXdnDonzKy49240535 = GoIOEjaMZuTyPkXdnDonzKy94490793;     GoIOEjaMZuTyPkXdnDonzKy94490793 = GoIOEjaMZuTyPkXdnDonzKy67704709;     GoIOEjaMZuTyPkXdnDonzKy67704709 = GoIOEjaMZuTyPkXdnDonzKy44359758;     GoIOEjaMZuTyPkXdnDonzKy44359758 = GoIOEjaMZuTyPkXdnDonzKy7241447;     GoIOEjaMZuTyPkXdnDonzKy7241447 = GoIOEjaMZuTyPkXdnDonzKy38528914;     GoIOEjaMZuTyPkXdnDonzKy38528914 = GoIOEjaMZuTyPkXdnDonzKy30828115;     GoIOEjaMZuTyPkXdnDonzKy30828115 = GoIOEjaMZuTyPkXdnDonzKy15118560;     GoIOEjaMZuTyPkXdnDonzKy15118560 = GoIOEjaMZuTyPkXdnDonzKy19487303;     GoIOEjaMZuTyPkXdnDonzKy19487303 = GoIOEjaMZuTyPkXdnDonzKy65150927;     GoIOEjaMZuTyPkXdnDonzKy65150927 = GoIOEjaMZuTyPkXdnDonzKy20170941;     GoIOEjaMZuTyPkXdnDonzKy20170941 = GoIOEjaMZuTyPkXdnDonzKy66153092;     GoIOEjaMZuTyPkXdnDonzKy66153092 = GoIOEjaMZuTyPkXdnDonzKy92397054;     GoIOEjaMZuTyPkXdnDonzKy92397054 = GoIOEjaMZuTyPkXdnDonzKy49653130;     GoIOEjaMZuTyPkXdnDonzKy49653130 = GoIOEjaMZuTyPkXdnDonzKy5960528;     GoIOEjaMZuTyPkXdnDonzKy5960528 = GoIOEjaMZuTyPkXdnDonzKy54760561;     GoIOEjaMZuTyPkXdnDonzKy54760561 = GoIOEjaMZuTyPkXdnDonzKy75398108;     GoIOEjaMZuTyPkXdnDonzKy75398108 = GoIOEjaMZuTyPkXdnDonzKy84825729;     GoIOEjaMZuTyPkXdnDonzKy84825729 = GoIOEjaMZuTyPkXdnDonzKy66849522;     GoIOEjaMZuTyPkXdnDonzKy66849522 = GoIOEjaMZuTyPkXdnDonzKy48558072;     GoIOEjaMZuTyPkXdnDonzKy48558072 = GoIOEjaMZuTyPkXdnDonzKy19561189;     GoIOEjaMZuTyPkXdnDonzKy19561189 = GoIOEjaMZuTyPkXdnDonzKy64467110;     GoIOEjaMZuTyPkXdnDonzKy64467110 = GoIOEjaMZuTyPkXdnDonzKy76163222;     GoIOEjaMZuTyPkXdnDonzKy76163222 = GoIOEjaMZuTyPkXdnDonzKy45257990;     GoIOEjaMZuTyPkXdnDonzKy45257990 = GoIOEjaMZuTyPkXdnDonzKy18291833;     GoIOEjaMZuTyPkXdnDonzKy18291833 = GoIOEjaMZuTyPkXdnDonzKy5763962;     GoIOEjaMZuTyPkXdnDonzKy5763962 = GoIOEjaMZuTyPkXdnDonzKy53115590;     GoIOEjaMZuTyPkXdnDonzKy53115590 = GoIOEjaMZuTyPkXdnDonzKy24596851;     GoIOEjaMZuTyPkXdnDonzKy24596851 = GoIOEjaMZuTyPkXdnDonzKy47139350;     GoIOEjaMZuTyPkXdnDonzKy47139350 = GoIOEjaMZuTyPkXdnDonzKy99751954;     GoIOEjaMZuTyPkXdnDonzKy99751954 = GoIOEjaMZuTyPkXdnDonzKy71794222;     GoIOEjaMZuTyPkXdnDonzKy71794222 = GoIOEjaMZuTyPkXdnDonzKy58989576;     GoIOEjaMZuTyPkXdnDonzKy58989576 = GoIOEjaMZuTyPkXdnDonzKy60904289;     GoIOEjaMZuTyPkXdnDonzKy60904289 = GoIOEjaMZuTyPkXdnDonzKy49929837;     GoIOEjaMZuTyPkXdnDonzKy49929837 = GoIOEjaMZuTyPkXdnDonzKy29270336;     GoIOEjaMZuTyPkXdnDonzKy29270336 = GoIOEjaMZuTyPkXdnDonzKy84070999;     GoIOEjaMZuTyPkXdnDonzKy84070999 = GoIOEjaMZuTyPkXdnDonzKy48552746;     GoIOEjaMZuTyPkXdnDonzKy48552746 = GoIOEjaMZuTyPkXdnDonzKy70368996;     GoIOEjaMZuTyPkXdnDonzKy70368996 = GoIOEjaMZuTyPkXdnDonzKy67014260;}



void wEAXFUpSbbLovEgIRvnSlaQlFO72810859() {     float RiXqkcwqAjvfuXfUlLBWfKR50737778 = -748930819;    float RiXqkcwqAjvfuXfUlLBWfKR70474921 = -549216531;    float RiXqkcwqAjvfuXfUlLBWfKR35103881 = -439652351;    float RiXqkcwqAjvfuXfUlLBWfKR4977178 = -528078044;    float RiXqkcwqAjvfuXfUlLBWfKR59254289 = -331873661;    float RiXqkcwqAjvfuXfUlLBWfKR11249775 = -114732487;    float RiXqkcwqAjvfuXfUlLBWfKR56586383 = -752019104;    float RiXqkcwqAjvfuXfUlLBWfKR73450897 = -254196618;    float RiXqkcwqAjvfuXfUlLBWfKR75896125 = -687747495;    float RiXqkcwqAjvfuXfUlLBWfKR3738777 = -13801235;    float RiXqkcwqAjvfuXfUlLBWfKR95898988 = 1560277;    float RiXqkcwqAjvfuXfUlLBWfKR87408342 = -411058541;    float RiXqkcwqAjvfuXfUlLBWfKR2282376 = 48088480;    float RiXqkcwqAjvfuXfUlLBWfKR32290050 = -510474612;    float RiXqkcwqAjvfuXfUlLBWfKR68295977 = 1982312;    float RiXqkcwqAjvfuXfUlLBWfKR47444187 = -561157026;    float RiXqkcwqAjvfuXfUlLBWfKR97927949 = -915570023;    float RiXqkcwqAjvfuXfUlLBWfKR44421820 = -778343892;    float RiXqkcwqAjvfuXfUlLBWfKR49589264 = -587045958;    float RiXqkcwqAjvfuXfUlLBWfKR58114800 = -369379003;    float RiXqkcwqAjvfuXfUlLBWfKR95503512 = -287719338;    float RiXqkcwqAjvfuXfUlLBWfKR85524391 = -896902951;    float RiXqkcwqAjvfuXfUlLBWfKR40193882 = -89848172;    float RiXqkcwqAjvfuXfUlLBWfKR91269557 = -470688752;    float RiXqkcwqAjvfuXfUlLBWfKR83313928 = -330559460;    float RiXqkcwqAjvfuXfUlLBWfKR81501677 = -125878194;    float RiXqkcwqAjvfuXfUlLBWfKR39946794 = -416951671;    float RiXqkcwqAjvfuXfUlLBWfKR20624756 = -113446476;    float RiXqkcwqAjvfuXfUlLBWfKR52788421 = -366459063;    float RiXqkcwqAjvfuXfUlLBWfKR66734130 = -105466986;    float RiXqkcwqAjvfuXfUlLBWfKR71292902 = -994887285;    float RiXqkcwqAjvfuXfUlLBWfKR33443483 = -208412406;    float RiXqkcwqAjvfuXfUlLBWfKR4079827 = -496928125;    float RiXqkcwqAjvfuXfUlLBWfKR48424324 = -248776417;    float RiXqkcwqAjvfuXfUlLBWfKR74692116 = -402298831;    float RiXqkcwqAjvfuXfUlLBWfKR18859393 = -497487053;    float RiXqkcwqAjvfuXfUlLBWfKR55906446 = -114857840;    float RiXqkcwqAjvfuXfUlLBWfKR83293402 = -60774031;    float RiXqkcwqAjvfuXfUlLBWfKR70789328 = -562495205;    float RiXqkcwqAjvfuXfUlLBWfKR81648807 = 2560388;    float RiXqkcwqAjvfuXfUlLBWfKR56377622 = -421697274;    float RiXqkcwqAjvfuXfUlLBWfKR7850599 = -671073484;    float RiXqkcwqAjvfuXfUlLBWfKR11361939 = -498735876;    float RiXqkcwqAjvfuXfUlLBWfKR63452974 = -103912027;    float RiXqkcwqAjvfuXfUlLBWfKR42716005 = -797294254;    float RiXqkcwqAjvfuXfUlLBWfKR6152988 = -614579574;    float RiXqkcwqAjvfuXfUlLBWfKR94838366 = -815295134;    float RiXqkcwqAjvfuXfUlLBWfKR8538630 = -811593700;    float RiXqkcwqAjvfuXfUlLBWfKR92338580 = -978578430;    float RiXqkcwqAjvfuXfUlLBWfKR52276499 = -225038883;    float RiXqkcwqAjvfuXfUlLBWfKR95113412 = -859854038;    float RiXqkcwqAjvfuXfUlLBWfKR96176209 = -262586771;    float RiXqkcwqAjvfuXfUlLBWfKR35740765 = -288754658;    float RiXqkcwqAjvfuXfUlLBWfKR30123769 = -189542897;    float RiXqkcwqAjvfuXfUlLBWfKR77307347 = -829415170;    float RiXqkcwqAjvfuXfUlLBWfKR65213387 = -852027868;    float RiXqkcwqAjvfuXfUlLBWfKR30281040 = -359368360;    float RiXqkcwqAjvfuXfUlLBWfKR43834323 = -968963600;    float RiXqkcwqAjvfuXfUlLBWfKR21663249 = -97518584;    float RiXqkcwqAjvfuXfUlLBWfKR77752611 = -105995468;    float RiXqkcwqAjvfuXfUlLBWfKR71302980 = -697780817;    float RiXqkcwqAjvfuXfUlLBWfKR35961628 = -538572629;    float RiXqkcwqAjvfuXfUlLBWfKR20662477 = -887737555;    float RiXqkcwqAjvfuXfUlLBWfKR9161995 = -482280510;    float RiXqkcwqAjvfuXfUlLBWfKR32445875 = -18913951;    float RiXqkcwqAjvfuXfUlLBWfKR62455505 = -790027317;    float RiXqkcwqAjvfuXfUlLBWfKR83328516 = -914130417;    float RiXqkcwqAjvfuXfUlLBWfKR53858052 = -703135103;    float RiXqkcwqAjvfuXfUlLBWfKR57597934 = -8175782;    float RiXqkcwqAjvfuXfUlLBWfKR49436584 = -500530635;    float RiXqkcwqAjvfuXfUlLBWfKR91537741 = -346299187;    float RiXqkcwqAjvfuXfUlLBWfKR14634548 = -754795993;    float RiXqkcwqAjvfuXfUlLBWfKR73632491 = -115848688;    float RiXqkcwqAjvfuXfUlLBWfKR67940457 = -489606346;    float RiXqkcwqAjvfuXfUlLBWfKR1737178 = -947681730;    float RiXqkcwqAjvfuXfUlLBWfKR87652914 = -616645855;    float RiXqkcwqAjvfuXfUlLBWfKR74162452 = -298167076;    float RiXqkcwqAjvfuXfUlLBWfKR76740907 = -985936146;    float RiXqkcwqAjvfuXfUlLBWfKR48553552 = -673394499;    float RiXqkcwqAjvfuXfUlLBWfKR77160941 = -715979887;    float RiXqkcwqAjvfuXfUlLBWfKR86663310 = -310583060;    float RiXqkcwqAjvfuXfUlLBWfKR31408165 = -605357971;    float RiXqkcwqAjvfuXfUlLBWfKR28286175 = -134868046;    float RiXqkcwqAjvfuXfUlLBWfKR511922 = -41420180;    float RiXqkcwqAjvfuXfUlLBWfKR71620718 = -245612948;    float RiXqkcwqAjvfuXfUlLBWfKR75116693 = -632300514;    float RiXqkcwqAjvfuXfUlLBWfKR97702718 = -919657749;    float RiXqkcwqAjvfuXfUlLBWfKR73956057 = -207385228;    float RiXqkcwqAjvfuXfUlLBWfKR71116976 = -419361247;    float RiXqkcwqAjvfuXfUlLBWfKR9478730 = -550270963;    float RiXqkcwqAjvfuXfUlLBWfKR88578353 = -38118694;    float RiXqkcwqAjvfuXfUlLBWfKR12072123 = -145894241;    float RiXqkcwqAjvfuXfUlLBWfKR61630153 = -963255447;    float RiXqkcwqAjvfuXfUlLBWfKR93036716 = -356499738;    float RiXqkcwqAjvfuXfUlLBWfKR10345827 = -299658796;    float RiXqkcwqAjvfuXfUlLBWfKR20415995 = -883124646;    float RiXqkcwqAjvfuXfUlLBWfKR87188121 = -783335929;    float RiXqkcwqAjvfuXfUlLBWfKR2199945 = 83544633;    float RiXqkcwqAjvfuXfUlLBWfKR31007100 = 15001924;    float RiXqkcwqAjvfuXfUlLBWfKR80260499 = -748930819;     RiXqkcwqAjvfuXfUlLBWfKR50737778 = RiXqkcwqAjvfuXfUlLBWfKR70474921;     RiXqkcwqAjvfuXfUlLBWfKR70474921 = RiXqkcwqAjvfuXfUlLBWfKR35103881;     RiXqkcwqAjvfuXfUlLBWfKR35103881 = RiXqkcwqAjvfuXfUlLBWfKR4977178;     RiXqkcwqAjvfuXfUlLBWfKR4977178 = RiXqkcwqAjvfuXfUlLBWfKR59254289;     RiXqkcwqAjvfuXfUlLBWfKR59254289 = RiXqkcwqAjvfuXfUlLBWfKR11249775;     RiXqkcwqAjvfuXfUlLBWfKR11249775 = RiXqkcwqAjvfuXfUlLBWfKR56586383;     RiXqkcwqAjvfuXfUlLBWfKR56586383 = RiXqkcwqAjvfuXfUlLBWfKR73450897;     RiXqkcwqAjvfuXfUlLBWfKR73450897 = RiXqkcwqAjvfuXfUlLBWfKR75896125;     RiXqkcwqAjvfuXfUlLBWfKR75896125 = RiXqkcwqAjvfuXfUlLBWfKR3738777;     RiXqkcwqAjvfuXfUlLBWfKR3738777 = RiXqkcwqAjvfuXfUlLBWfKR95898988;     RiXqkcwqAjvfuXfUlLBWfKR95898988 = RiXqkcwqAjvfuXfUlLBWfKR87408342;     RiXqkcwqAjvfuXfUlLBWfKR87408342 = RiXqkcwqAjvfuXfUlLBWfKR2282376;     RiXqkcwqAjvfuXfUlLBWfKR2282376 = RiXqkcwqAjvfuXfUlLBWfKR32290050;     RiXqkcwqAjvfuXfUlLBWfKR32290050 = RiXqkcwqAjvfuXfUlLBWfKR68295977;     RiXqkcwqAjvfuXfUlLBWfKR68295977 = RiXqkcwqAjvfuXfUlLBWfKR47444187;     RiXqkcwqAjvfuXfUlLBWfKR47444187 = RiXqkcwqAjvfuXfUlLBWfKR97927949;     RiXqkcwqAjvfuXfUlLBWfKR97927949 = RiXqkcwqAjvfuXfUlLBWfKR44421820;     RiXqkcwqAjvfuXfUlLBWfKR44421820 = RiXqkcwqAjvfuXfUlLBWfKR49589264;     RiXqkcwqAjvfuXfUlLBWfKR49589264 = RiXqkcwqAjvfuXfUlLBWfKR58114800;     RiXqkcwqAjvfuXfUlLBWfKR58114800 = RiXqkcwqAjvfuXfUlLBWfKR95503512;     RiXqkcwqAjvfuXfUlLBWfKR95503512 = RiXqkcwqAjvfuXfUlLBWfKR85524391;     RiXqkcwqAjvfuXfUlLBWfKR85524391 = RiXqkcwqAjvfuXfUlLBWfKR40193882;     RiXqkcwqAjvfuXfUlLBWfKR40193882 = RiXqkcwqAjvfuXfUlLBWfKR91269557;     RiXqkcwqAjvfuXfUlLBWfKR91269557 = RiXqkcwqAjvfuXfUlLBWfKR83313928;     RiXqkcwqAjvfuXfUlLBWfKR83313928 = RiXqkcwqAjvfuXfUlLBWfKR81501677;     RiXqkcwqAjvfuXfUlLBWfKR81501677 = RiXqkcwqAjvfuXfUlLBWfKR39946794;     RiXqkcwqAjvfuXfUlLBWfKR39946794 = RiXqkcwqAjvfuXfUlLBWfKR20624756;     RiXqkcwqAjvfuXfUlLBWfKR20624756 = RiXqkcwqAjvfuXfUlLBWfKR52788421;     RiXqkcwqAjvfuXfUlLBWfKR52788421 = RiXqkcwqAjvfuXfUlLBWfKR66734130;     RiXqkcwqAjvfuXfUlLBWfKR66734130 = RiXqkcwqAjvfuXfUlLBWfKR71292902;     RiXqkcwqAjvfuXfUlLBWfKR71292902 = RiXqkcwqAjvfuXfUlLBWfKR33443483;     RiXqkcwqAjvfuXfUlLBWfKR33443483 = RiXqkcwqAjvfuXfUlLBWfKR4079827;     RiXqkcwqAjvfuXfUlLBWfKR4079827 = RiXqkcwqAjvfuXfUlLBWfKR48424324;     RiXqkcwqAjvfuXfUlLBWfKR48424324 = RiXqkcwqAjvfuXfUlLBWfKR74692116;     RiXqkcwqAjvfuXfUlLBWfKR74692116 = RiXqkcwqAjvfuXfUlLBWfKR18859393;     RiXqkcwqAjvfuXfUlLBWfKR18859393 = RiXqkcwqAjvfuXfUlLBWfKR55906446;     RiXqkcwqAjvfuXfUlLBWfKR55906446 = RiXqkcwqAjvfuXfUlLBWfKR83293402;     RiXqkcwqAjvfuXfUlLBWfKR83293402 = RiXqkcwqAjvfuXfUlLBWfKR70789328;     RiXqkcwqAjvfuXfUlLBWfKR70789328 = RiXqkcwqAjvfuXfUlLBWfKR81648807;     RiXqkcwqAjvfuXfUlLBWfKR81648807 = RiXqkcwqAjvfuXfUlLBWfKR56377622;     RiXqkcwqAjvfuXfUlLBWfKR56377622 = RiXqkcwqAjvfuXfUlLBWfKR7850599;     RiXqkcwqAjvfuXfUlLBWfKR7850599 = RiXqkcwqAjvfuXfUlLBWfKR11361939;     RiXqkcwqAjvfuXfUlLBWfKR11361939 = RiXqkcwqAjvfuXfUlLBWfKR63452974;     RiXqkcwqAjvfuXfUlLBWfKR63452974 = RiXqkcwqAjvfuXfUlLBWfKR42716005;     RiXqkcwqAjvfuXfUlLBWfKR42716005 = RiXqkcwqAjvfuXfUlLBWfKR6152988;     RiXqkcwqAjvfuXfUlLBWfKR6152988 = RiXqkcwqAjvfuXfUlLBWfKR94838366;     RiXqkcwqAjvfuXfUlLBWfKR94838366 = RiXqkcwqAjvfuXfUlLBWfKR8538630;     RiXqkcwqAjvfuXfUlLBWfKR8538630 = RiXqkcwqAjvfuXfUlLBWfKR92338580;     RiXqkcwqAjvfuXfUlLBWfKR92338580 = RiXqkcwqAjvfuXfUlLBWfKR52276499;     RiXqkcwqAjvfuXfUlLBWfKR52276499 = RiXqkcwqAjvfuXfUlLBWfKR95113412;     RiXqkcwqAjvfuXfUlLBWfKR95113412 = RiXqkcwqAjvfuXfUlLBWfKR96176209;     RiXqkcwqAjvfuXfUlLBWfKR96176209 = RiXqkcwqAjvfuXfUlLBWfKR35740765;     RiXqkcwqAjvfuXfUlLBWfKR35740765 = RiXqkcwqAjvfuXfUlLBWfKR30123769;     RiXqkcwqAjvfuXfUlLBWfKR30123769 = RiXqkcwqAjvfuXfUlLBWfKR77307347;     RiXqkcwqAjvfuXfUlLBWfKR77307347 = RiXqkcwqAjvfuXfUlLBWfKR65213387;     RiXqkcwqAjvfuXfUlLBWfKR65213387 = RiXqkcwqAjvfuXfUlLBWfKR30281040;     RiXqkcwqAjvfuXfUlLBWfKR30281040 = RiXqkcwqAjvfuXfUlLBWfKR43834323;     RiXqkcwqAjvfuXfUlLBWfKR43834323 = RiXqkcwqAjvfuXfUlLBWfKR21663249;     RiXqkcwqAjvfuXfUlLBWfKR21663249 = RiXqkcwqAjvfuXfUlLBWfKR77752611;     RiXqkcwqAjvfuXfUlLBWfKR77752611 = RiXqkcwqAjvfuXfUlLBWfKR71302980;     RiXqkcwqAjvfuXfUlLBWfKR71302980 = RiXqkcwqAjvfuXfUlLBWfKR35961628;     RiXqkcwqAjvfuXfUlLBWfKR35961628 = RiXqkcwqAjvfuXfUlLBWfKR20662477;     RiXqkcwqAjvfuXfUlLBWfKR20662477 = RiXqkcwqAjvfuXfUlLBWfKR9161995;     RiXqkcwqAjvfuXfUlLBWfKR9161995 = RiXqkcwqAjvfuXfUlLBWfKR32445875;     RiXqkcwqAjvfuXfUlLBWfKR32445875 = RiXqkcwqAjvfuXfUlLBWfKR62455505;     RiXqkcwqAjvfuXfUlLBWfKR62455505 = RiXqkcwqAjvfuXfUlLBWfKR83328516;     RiXqkcwqAjvfuXfUlLBWfKR83328516 = RiXqkcwqAjvfuXfUlLBWfKR53858052;     RiXqkcwqAjvfuXfUlLBWfKR53858052 = RiXqkcwqAjvfuXfUlLBWfKR57597934;     RiXqkcwqAjvfuXfUlLBWfKR57597934 = RiXqkcwqAjvfuXfUlLBWfKR49436584;     RiXqkcwqAjvfuXfUlLBWfKR49436584 = RiXqkcwqAjvfuXfUlLBWfKR91537741;     RiXqkcwqAjvfuXfUlLBWfKR91537741 = RiXqkcwqAjvfuXfUlLBWfKR14634548;     RiXqkcwqAjvfuXfUlLBWfKR14634548 = RiXqkcwqAjvfuXfUlLBWfKR73632491;     RiXqkcwqAjvfuXfUlLBWfKR73632491 = RiXqkcwqAjvfuXfUlLBWfKR67940457;     RiXqkcwqAjvfuXfUlLBWfKR67940457 = RiXqkcwqAjvfuXfUlLBWfKR1737178;     RiXqkcwqAjvfuXfUlLBWfKR1737178 = RiXqkcwqAjvfuXfUlLBWfKR87652914;     RiXqkcwqAjvfuXfUlLBWfKR87652914 = RiXqkcwqAjvfuXfUlLBWfKR74162452;     RiXqkcwqAjvfuXfUlLBWfKR74162452 = RiXqkcwqAjvfuXfUlLBWfKR76740907;     RiXqkcwqAjvfuXfUlLBWfKR76740907 = RiXqkcwqAjvfuXfUlLBWfKR48553552;     RiXqkcwqAjvfuXfUlLBWfKR48553552 = RiXqkcwqAjvfuXfUlLBWfKR77160941;     RiXqkcwqAjvfuXfUlLBWfKR77160941 = RiXqkcwqAjvfuXfUlLBWfKR86663310;     RiXqkcwqAjvfuXfUlLBWfKR86663310 = RiXqkcwqAjvfuXfUlLBWfKR31408165;     RiXqkcwqAjvfuXfUlLBWfKR31408165 = RiXqkcwqAjvfuXfUlLBWfKR28286175;     RiXqkcwqAjvfuXfUlLBWfKR28286175 = RiXqkcwqAjvfuXfUlLBWfKR511922;     RiXqkcwqAjvfuXfUlLBWfKR511922 = RiXqkcwqAjvfuXfUlLBWfKR71620718;     RiXqkcwqAjvfuXfUlLBWfKR71620718 = RiXqkcwqAjvfuXfUlLBWfKR75116693;     RiXqkcwqAjvfuXfUlLBWfKR75116693 = RiXqkcwqAjvfuXfUlLBWfKR97702718;     RiXqkcwqAjvfuXfUlLBWfKR97702718 = RiXqkcwqAjvfuXfUlLBWfKR73956057;     RiXqkcwqAjvfuXfUlLBWfKR73956057 = RiXqkcwqAjvfuXfUlLBWfKR71116976;     RiXqkcwqAjvfuXfUlLBWfKR71116976 = RiXqkcwqAjvfuXfUlLBWfKR9478730;     RiXqkcwqAjvfuXfUlLBWfKR9478730 = RiXqkcwqAjvfuXfUlLBWfKR88578353;     RiXqkcwqAjvfuXfUlLBWfKR88578353 = RiXqkcwqAjvfuXfUlLBWfKR12072123;     RiXqkcwqAjvfuXfUlLBWfKR12072123 = RiXqkcwqAjvfuXfUlLBWfKR61630153;     RiXqkcwqAjvfuXfUlLBWfKR61630153 = RiXqkcwqAjvfuXfUlLBWfKR93036716;     RiXqkcwqAjvfuXfUlLBWfKR93036716 = RiXqkcwqAjvfuXfUlLBWfKR10345827;     RiXqkcwqAjvfuXfUlLBWfKR10345827 = RiXqkcwqAjvfuXfUlLBWfKR20415995;     RiXqkcwqAjvfuXfUlLBWfKR20415995 = RiXqkcwqAjvfuXfUlLBWfKR87188121;     RiXqkcwqAjvfuXfUlLBWfKR87188121 = RiXqkcwqAjvfuXfUlLBWfKR2199945;     RiXqkcwqAjvfuXfUlLBWfKR2199945 = RiXqkcwqAjvfuXfUlLBWfKR31007100;     RiXqkcwqAjvfuXfUlLBWfKR31007100 = RiXqkcwqAjvfuXfUlLBWfKR80260499;     RiXqkcwqAjvfuXfUlLBWfKR80260499 = RiXqkcwqAjvfuXfUlLBWfKR50737778;}



void DDGrhxuqEElhjmlUUPCPyUYWZa57873697() {     float oVTEEJQHMjsIUZocaaoGcwm34461297 = 1047114;    float oVTEEJQHMjsIUZocaaoGcwm68073016 = -352868383;    float oVTEEJQHMjsIUZocaaoGcwm23689998 = -307960042;    float oVTEEJQHMjsIUZocaaoGcwm2125131 = -532959973;    float oVTEEJQHMjsIUZocaaoGcwm38815626 = -729821852;    float oVTEEJQHMjsIUZocaaoGcwm22589292 = -218053156;    float oVTEEJQHMjsIUZocaaoGcwm93466946 = -773533242;    float oVTEEJQHMjsIUZocaaoGcwm16790981 = -874651204;    float oVTEEJQHMjsIUZocaaoGcwm26115654 = -285230191;    float oVTEEJQHMjsIUZocaaoGcwm49844365 = -236842251;    float oVTEEJQHMjsIUZocaaoGcwm38788024 = -938724981;    float oVTEEJQHMjsIUZocaaoGcwm77620113 = -340197812;    float oVTEEJQHMjsIUZocaaoGcwm53336227 = -894743764;    float oVTEEJQHMjsIUZocaaoGcwm26663195 = -860157474;    float oVTEEJQHMjsIUZocaaoGcwm72475236 = 76285007;    float oVTEEJQHMjsIUZocaaoGcwm19258216 = -485284678;    float oVTEEJQHMjsIUZocaaoGcwm97793360 = 25550114;    float oVTEEJQHMjsIUZocaaoGcwm58499089 = -389195466;    float oVTEEJQHMjsIUZocaaoGcwm90565744 = -375695196;    float oVTEEJQHMjsIUZocaaoGcwm32222444 = 14328080;    float oVTEEJQHMjsIUZocaaoGcwm62843022 = -962185859;    float oVTEEJQHMjsIUZocaaoGcwm86526462 = -810058237;    float oVTEEJQHMjsIUZocaaoGcwm9369747 = -119242909;    float oVTEEJQHMjsIUZocaaoGcwm92412252 = -9921405;    float oVTEEJQHMjsIUZocaaoGcwm19916022 = -408890222;    float oVTEEJQHMjsIUZocaaoGcwm34494435 = -568845221;    float oVTEEJQHMjsIUZocaaoGcwm61538733 = -860827094;    float oVTEEJQHMjsIUZocaaoGcwm63223955 = -999887965;    float oVTEEJQHMjsIUZocaaoGcwm24706563 = -890654963;    float oVTEEJQHMjsIUZocaaoGcwm2282458 = -778073360;    float oVTEEJQHMjsIUZocaaoGcwm52657325 = -113985305;    float oVTEEJQHMjsIUZocaaoGcwm58236771 = -757609629;    float oVTEEJQHMjsIUZocaaoGcwm18204528 = -62142216;    float oVTEEJQHMjsIUZocaaoGcwm84149037 = -652873065;    float oVTEEJQHMjsIUZocaaoGcwm42295442 = -136589438;    float oVTEEJQHMjsIUZocaaoGcwm88720626 = -475883531;    float oVTEEJQHMjsIUZocaaoGcwm55670036 = -435199136;    float oVTEEJQHMjsIUZocaaoGcwm33675194 = 40082753;    float oVTEEJQHMjsIUZocaaoGcwm31405048 = -868054086;    float oVTEEJQHMjsIUZocaaoGcwm20837923 = 55913508;    float oVTEEJQHMjsIUZocaaoGcwm21145143 = -71737400;    float oVTEEJQHMjsIUZocaaoGcwm37190325 = -277099154;    float oVTEEJQHMjsIUZocaaoGcwm44162085 = -151140130;    float oVTEEJQHMjsIUZocaaoGcwm10648494 = -782855759;    float oVTEEJQHMjsIUZocaaoGcwm70703255 = -23308332;    float oVTEEJQHMjsIUZocaaoGcwm50419869 = -229611265;    float oVTEEJQHMjsIUZocaaoGcwm28017337 = -836941041;    float oVTEEJQHMjsIUZocaaoGcwm47280477 = -573715598;    float oVTEEJQHMjsIUZocaaoGcwm26212794 = -678362581;    float oVTEEJQHMjsIUZocaaoGcwm88149829 = -857054319;    float oVTEEJQHMjsIUZocaaoGcwm35204244 = 74373560;    float oVTEEJQHMjsIUZocaaoGcwm47681929 = -14639557;    float oVTEEJQHMjsIUZocaaoGcwm81123167 = -300687024;    float oVTEEJQHMjsIUZocaaoGcwm76056375 = -766252085;    float oVTEEJQHMjsIUZocaaoGcwm95030674 = -46142115;    float oVTEEJQHMjsIUZocaaoGcwm47934834 = -188894649;    float oVTEEJQHMjsIUZocaaoGcwm58703270 = -133625474;    float oVTEEJQHMjsIUZocaaoGcwm31277746 = -198038638;    float oVTEEJQHMjsIUZocaaoGcwm82209109 = -24069751;    float oVTEEJQHMjsIUZocaaoGcwm4321191 = -60976631;    float oVTEEJQHMjsIUZocaaoGcwm61050559 = -357226063;    float oVTEEJQHMjsIUZocaaoGcwm30242991 = -773645278;    float oVTEEJQHMjsIUZocaaoGcwm92084417 = -983996241;    float oVTEEJQHMjsIUZocaaoGcwm23833196 = -507156831;    float oVTEEJQHMjsIUZocaaoGcwm97187039 = -22856946;    float oVTEEJQHMjsIUZocaaoGcwm80551253 = -81115353;    float oVTEEJQHMjsIUZocaaoGcwm59415586 = -178055596;    float oVTEEJQHMjsIUZocaaoGcwm69187189 = -141870699;    float oVTEEJQHMjsIUZocaaoGcwm84367753 = -623568036;    float oVTEEJQHMjsIUZocaaoGcwm83754609 = -447831462;    float oVTEEJQHMjsIUZocaaoGcwm63588179 = 49914458;    float oVTEEJQHMjsIUZocaaoGcwm64118167 = 85467361;    float oVTEEJQHMjsIUZocaaoGcwm27094042 = -521141380;    float oVTEEJQHMjsIUZocaaoGcwm69727822 = -331608705;    float oVTEEJQHMjsIUZocaaoGcwm11077302 = -913934520;    float oVTEEJQHMjsIUZocaaoGcwm25652698 = -585086706;    float oVTEEJQHMjsIUZocaaoGcwm42364378 = -558918108;    float oVTEEJQHMjsIUZocaaoGcwm98721252 = -336387151;    float oVTEEJQHMjsIUZocaaoGcwm21708997 = -986613074;    float oVTEEJQHMjsIUZocaaoGcwm69496152 = -79278957;    float oVTEEJQHMjsIUZocaaoGcwm6477099 = -731904181;    float oVTEEJQHMjsIUZocaaoGcwm14258257 = -187111496;    float oVTEEJQHMjsIUZocaaoGcwm37011162 = -221525384;    float oVTEEJQHMjsIUZocaaoGcwm36556733 = 66399355;    float oVTEEJQHMjsIUZocaaoGcwm67078214 = -752446921;    float oVTEEJQHMjsIUZocaaoGcwm4975397 = 654252;    float oVTEEJQHMjsIUZocaaoGcwm77113604 = -356922605;    float oVTEEJQHMjsIUZocaaoGcwm42148153 = -295890132;    float oVTEEJQHMjsIUZocaaoGcwm89118362 = -506730950;    float oVTEEJQHMjsIUZocaaoGcwm94360608 = -947694790;    float oVTEEJQHMjsIUZocaaoGcwm30017357 = -242258057;    float oVTEEJQHMjsIUZocaaoGcwm24392291 = -137160499;    float oVTEEJQHMjsIUZocaaoGcwm51466084 = -935847497;    float oVTEEJQHMjsIUZocaaoGcwm27083857 = -707077456;    float oVTEEJQHMjsIUZocaaoGcwm59787364 = -586860430;    float oVTEEJQHMjsIUZocaaoGcwm90902152 = -298092123;    float oVTEEJQHMjsIUZocaaoGcwm45105907 = -293102913;    float oVTEEJQHMjsIUZocaaoGcwm20328889 = -643983299;    float oVTEEJQHMjsIUZocaaoGcwm13461455 = -659998814;    float oVTEEJQHMjsIUZocaaoGcwm90152002 = 1047114;     oVTEEJQHMjsIUZocaaoGcwm34461297 = oVTEEJQHMjsIUZocaaoGcwm68073016;     oVTEEJQHMjsIUZocaaoGcwm68073016 = oVTEEJQHMjsIUZocaaoGcwm23689998;     oVTEEJQHMjsIUZocaaoGcwm23689998 = oVTEEJQHMjsIUZocaaoGcwm2125131;     oVTEEJQHMjsIUZocaaoGcwm2125131 = oVTEEJQHMjsIUZocaaoGcwm38815626;     oVTEEJQHMjsIUZocaaoGcwm38815626 = oVTEEJQHMjsIUZocaaoGcwm22589292;     oVTEEJQHMjsIUZocaaoGcwm22589292 = oVTEEJQHMjsIUZocaaoGcwm93466946;     oVTEEJQHMjsIUZocaaoGcwm93466946 = oVTEEJQHMjsIUZocaaoGcwm16790981;     oVTEEJQHMjsIUZocaaoGcwm16790981 = oVTEEJQHMjsIUZocaaoGcwm26115654;     oVTEEJQHMjsIUZocaaoGcwm26115654 = oVTEEJQHMjsIUZocaaoGcwm49844365;     oVTEEJQHMjsIUZocaaoGcwm49844365 = oVTEEJQHMjsIUZocaaoGcwm38788024;     oVTEEJQHMjsIUZocaaoGcwm38788024 = oVTEEJQHMjsIUZocaaoGcwm77620113;     oVTEEJQHMjsIUZocaaoGcwm77620113 = oVTEEJQHMjsIUZocaaoGcwm53336227;     oVTEEJQHMjsIUZocaaoGcwm53336227 = oVTEEJQHMjsIUZocaaoGcwm26663195;     oVTEEJQHMjsIUZocaaoGcwm26663195 = oVTEEJQHMjsIUZocaaoGcwm72475236;     oVTEEJQHMjsIUZocaaoGcwm72475236 = oVTEEJQHMjsIUZocaaoGcwm19258216;     oVTEEJQHMjsIUZocaaoGcwm19258216 = oVTEEJQHMjsIUZocaaoGcwm97793360;     oVTEEJQHMjsIUZocaaoGcwm97793360 = oVTEEJQHMjsIUZocaaoGcwm58499089;     oVTEEJQHMjsIUZocaaoGcwm58499089 = oVTEEJQHMjsIUZocaaoGcwm90565744;     oVTEEJQHMjsIUZocaaoGcwm90565744 = oVTEEJQHMjsIUZocaaoGcwm32222444;     oVTEEJQHMjsIUZocaaoGcwm32222444 = oVTEEJQHMjsIUZocaaoGcwm62843022;     oVTEEJQHMjsIUZocaaoGcwm62843022 = oVTEEJQHMjsIUZocaaoGcwm86526462;     oVTEEJQHMjsIUZocaaoGcwm86526462 = oVTEEJQHMjsIUZocaaoGcwm9369747;     oVTEEJQHMjsIUZocaaoGcwm9369747 = oVTEEJQHMjsIUZocaaoGcwm92412252;     oVTEEJQHMjsIUZocaaoGcwm92412252 = oVTEEJQHMjsIUZocaaoGcwm19916022;     oVTEEJQHMjsIUZocaaoGcwm19916022 = oVTEEJQHMjsIUZocaaoGcwm34494435;     oVTEEJQHMjsIUZocaaoGcwm34494435 = oVTEEJQHMjsIUZocaaoGcwm61538733;     oVTEEJQHMjsIUZocaaoGcwm61538733 = oVTEEJQHMjsIUZocaaoGcwm63223955;     oVTEEJQHMjsIUZocaaoGcwm63223955 = oVTEEJQHMjsIUZocaaoGcwm24706563;     oVTEEJQHMjsIUZocaaoGcwm24706563 = oVTEEJQHMjsIUZocaaoGcwm2282458;     oVTEEJQHMjsIUZocaaoGcwm2282458 = oVTEEJQHMjsIUZocaaoGcwm52657325;     oVTEEJQHMjsIUZocaaoGcwm52657325 = oVTEEJQHMjsIUZocaaoGcwm58236771;     oVTEEJQHMjsIUZocaaoGcwm58236771 = oVTEEJQHMjsIUZocaaoGcwm18204528;     oVTEEJQHMjsIUZocaaoGcwm18204528 = oVTEEJQHMjsIUZocaaoGcwm84149037;     oVTEEJQHMjsIUZocaaoGcwm84149037 = oVTEEJQHMjsIUZocaaoGcwm42295442;     oVTEEJQHMjsIUZocaaoGcwm42295442 = oVTEEJQHMjsIUZocaaoGcwm88720626;     oVTEEJQHMjsIUZocaaoGcwm88720626 = oVTEEJQHMjsIUZocaaoGcwm55670036;     oVTEEJQHMjsIUZocaaoGcwm55670036 = oVTEEJQHMjsIUZocaaoGcwm33675194;     oVTEEJQHMjsIUZocaaoGcwm33675194 = oVTEEJQHMjsIUZocaaoGcwm31405048;     oVTEEJQHMjsIUZocaaoGcwm31405048 = oVTEEJQHMjsIUZocaaoGcwm20837923;     oVTEEJQHMjsIUZocaaoGcwm20837923 = oVTEEJQHMjsIUZocaaoGcwm21145143;     oVTEEJQHMjsIUZocaaoGcwm21145143 = oVTEEJQHMjsIUZocaaoGcwm37190325;     oVTEEJQHMjsIUZocaaoGcwm37190325 = oVTEEJQHMjsIUZocaaoGcwm44162085;     oVTEEJQHMjsIUZocaaoGcwm44162085 = oVTEEJQHMjsIUZocaaoGcwm10648494;     oVTEEJQHMjsIUZocaaoGcwm10648494 = oVTEEJQHMjsIUZocaaoGcwm70703255;     oVTEEJQHMjsIUZocaaoGcwm70703255 = oVTEEJQHMjsIUZocaaoGcwm50419869;     oVTEEJQHMjsIUZocaaoGcwm50419869 = oVTEEJQHMjsIUZocaaoGcwm28017337;     oVTEEJQHMjsIUZocaaoGcwm28017337 = oVTEEJQHMjsIUZocaaoGcwm47280477;     oVTEEJQHMjsIUZocaaoGcwm47280477 = oVTEEJQHMjsIUZocaaoGcwm26212794;     oVTEEJQHMjsIUZocaaoGcwm26212794 = oVTEEJQHMjsIUZocaaoGcwm88149829;     oVTEEJQHMjsIUZocaaoGcwm88149829 = oVTEEJQHMjsIUZocaaoGcwm35204244;     oVTEEJQHMjsIUZocaaoGcwm35204244 = oVTEEJQHMjsIUZocaaoGcwm47681929;     oVTEEJQHMjsIUZocaaoGcwm47681929 = oVTEEJQHMjsIUZocaaoGcwm81123167;     oVTEEJQHMjsIUZocaaoGcwm81123167 = oVTEEJQHMjsIUZocaaoGcwm76056375;     oVTEEJQHMjsIUZocaaoGcwm76056375 = oVTEEJQHMjsIUZocaaoGcwm95030674;     oVTEEJQHMjsIUZocaaoGcwm95030674 = oVTEEJQHMjsIUZocaaoGcwm47934834;     oVTEEJQHMjsIUZocaaoGcwm47934834 = oVTEEJQHMjsIUZocaaoGcwm58703270;     oVTEEJQHMjsIUZocaaoGcwm58703270 = oVTEEJQHMjsIUZocaaoGcwm31277746;     oVTEEJQHMjsIUZocaaoGcwm31277746 = oVTEEJQHMjsIUZocaaoGcwm82209109;     oVTEEJQHMjsIUZocaaoGcwm82209109 = oVTEEJQHMjsIUZocaaoGcwm4321191;     oVTEEJQHMjsIUZocaaoGcwm4321191 = oVTEEJQHMjsIUZocaaoGcwm61050559;     oVTEEJQHMjsIUZocaaoGcwm61050559 = oVTEEJQHMjsIUZocaaoGcwm30242991;     oVTEEJQHMjsIUZocaaoGcwm30242991 = oVTEEJQHMjsIUZocaaoGcwm92084417;     oVTEEJQHMjsIUZocaaoGcwm92084417 = oVTEEJQHMjsIUZocaaoGcwm23833196;     oVTEEJQHMjsIUZocaaoGcwm23833196 = oVTEEJQHMjsIUZocaaoGcwm97187039;     oVTEEJQHMjsIUZocaaoGcwm97187039 = oVTEEJQHMjsIUZocaaoGcwm80551253;     oVTEEJQHMjsIUZocaaoGcwm80551253 = oVTEEJQHMjsIUZocaaoGcwm59415586;     oVTEEJQHMjsIUZocaaoGcwm59415586 = oVTEEJQHMjsIUZocaaoGcwm69187189;     oVTEEJQHMjsIUZocaaoGcwm69187189 = oVTEEJQHMjsIUZocaaoGcwm84367753;     oVTEEJQHMjsIUZocaaoGcwm84367753 = oVTEEJQHMjsIUZocaaoGcwm83754609;     oVTEEJQHMjsIUZocaaoGcwm83754609 = oVTEEJQHMjsIUZocaaoGcwm63588179;     oVTEEJQHMjsIUZocaaoGcwm63588179 = oVTEEJQHMjsIUZocaaoGcwm64118167;     oVTEEJQHMjsIUZocaaoGcwm64118167 = oVTEEJQHMjsIUZocaaoGcwm27094042;     oVTEEJQHMjsIUZocaaoGcwm27094042 = oVTEEJQHMjsIUZocaaoGcwm69727822;     oVTEEJQHMjsIUZocaaoGcwm69727822 = oVTEEJQHMjsIUZocaaoGcwm11077302;     oVTEEJQHMjsIUZocaaoGcwm11077302 = oVTEEJQHMjsIUZocaaoGcwm25652698;     oVTEEJQHMjsIUZocaaoGcwm25652698 = oVTEEJQHMjsIUZocaaoGcwm42364378;     oVTEEJQHMjsIUZocaaoGcwm42364378 = oVTEEJQHMjsIUZocaaoGcwm98721252;     oVTEEJQHMjsIUZocaaoGcwm98721252 = oVTEEJQHMjsIUZocaaoGcwm21708997;     oVTEEJQHMjsIUZocaaoGcwm21708997 = oVTEEJQHMjsIUZocaaoGcwm69496152;     oVTEEJQHMjsIUZocaaoGcwm69496152 = oVTEEJQHMjsIUZocaaoGcwm6477099;     oVTEEJQHMjsIUZocaaoGcwm6477099 = oVTEEJQHMjsIUZocaaoGcwm14258257;     oVTEEJQHMjsIUZocaaoGcwm14258257 = oVTEEJQHMjsIUZocaaoGcwm37011162;     oVTEEJQHMjsIUZocaaoGcwm37011162 = oVTEEJQHMjsIUZocaaoGcwm36556733;     oVTEEJQHMjsIUZocaaoGcwm36556733 = oVTEEJQHMjsIUZocaaoGcwm67078214;     oVTEEJQHMjsIUZocaaoGcwm67078214 = oVTEEJQHMjsIUZocaaoGcwm4975397;     oVTEEJQHMjsIUZocaaoGcwm4975397 = oVTEEJQHMjsIUZocaaoGcwm77113604;     oVTEEJQHMjsIUZocaaoGcwm77113604 = oVTEEJQHMjsIUZocaaoGcwm42148153;     oVTEEJQHMjsIUZocaaoGcwm42148153 = oVTEEJQHMjsIUZocaaoGcwm89118362;     oVTEEJQHMjsIUZocaaoGcwm89118362 = oVTEEJQHMjsIUZocaaoGcwm94360608;     oVTEEJQHMjsIUZocaaoGcwm94360608 = oVTEEJQHMjsIUZocaaoGcwm30017357;     oVTEEJQHMjsIUZocaaoGcwm30017357 = oVTEEJQHMjsIUZocaaoGcwm24392291;     oVTEEJQHMjsIUZocaaoGcwm24392291 = oVTEEJQHMjsIUZocaaoGcwm51466084;     oVTEEJQHMjsIUZocaaoGcwm51466084 = oVTEEJQHMjsIUZocaaoGcwm27083857;     oVTEEJQHMjsIUZocaaoGcwm27083857 = oVTEEJQHMjsIUZocaaoGcwm59787364;     oVTEEJQHMjsIUZocaaoGcwm59787364 = oVTEEJQHMjsIUZocaaoGcwm90902152;     oVTEEJQHMjsIUZocaaoGcwm90902152 = oVTEEJQHMjsIUZocaaoGcwm45105907;     oVTEEJQHMjsIUZocaaoGcwm45105907 = oVTEEJQHMjsIUZocaaoGcwm20328889;     oVTEEJQHMjsIUZocaaoGcwm20328889 = oVTEEJQHMjsIUZocaaoGcwm13461455;     oVTEEJQHMjsIUZocaaoGcwm13461455 = oVTEEJQHMjsIUZocaaoGcwm90152002;     oVTEEJQHMjsIUZocaaoGcwm90152002 = oVTEEJQHMjsIUZocaaoGcwm34461297;}



void IrQInjBfIKqzTSeVTCFtsRwPBN42936535() {     float hBoSJTwGnKihxbAQkFIWNaj18184815 = -348974953;    float hBoSJTwGnKihxbAQkFIWNaj65671112 = -156520234;    float hBoSJTwGnKihxbAQkFIWNaj12276115 = -176267733;    float hBoSJTwGnKihxbAQkFIWNaj99273083 = -537841901;    float hBoSJTwGnKihxbAQkFIWNaj18376963 = -27770044;    float hBoSJTwGnKihxbAQkFIWNaj33928810 = -321373825;    float hBoSJTwGnKihxbAQkFIWNaj30347509 = -795047380;    float hBoSJTwGnKihxbAQkFIWNaj60131063 = -395105790;    float hBoSJTwGnKihxbAQkFIWNaj76335182 = -982712888;    float hBoSJTwGnKihxbAQkFIWNaj95949953 = -459883266;    float hBoSJTwGnKihxbAQkFIWNaj81677060 = -779010239;    float hBoSJTwGnKihxbAQkFIWNaj67831884 = -269337083;    float hBoSJTwGnKihxbAQkFIWNaj4390079 = -737576007;    float hBoSJTwGnKihxbAQkFIWNaj21036340 = -109840337;    float hBoSJTwGnKihxbAQkFIWNaj76654494 = -949412298;    float hBoSJTwGnKihxbAQkFIWNaj91072244 = -409412329;    float hBoSJTwGnKihxbAQkFIWNaj97658771 = -133329749;    float hBoSJTwGnKihxbAQkFIWNaj72576359 = -47040;    float hBoSJTwGnKihxbAQkFIWNaj31542226 = -164344434;    float hBoSJTwGnKihxbAQkFIWNaj6330089 = -701964837;    float hBoSJTwGnKihxbAQkFIWNaj30182533 = -536652380;    float hBoSJTwGnKihxbAQkFIWNaj87528534 = -723213524;    float hBoSJTwGnKihxbAQkFIWNaj78545612 = -148637646;    float hBoSJTwGnKihxbAQkFIWNaj93554947 = -649154058;    float hBoSJTwGnKihxbAQkFIWNaj56518114 = -487220983;    float hBoSJTwGnKihxbAQkFIWNaj87487193 = 88187751;    float hBoSJTwGnKihxbAQkFIWNaj83130672 = -204702517;    float hBoSJTwGnKihxbAQkFIWNaj5823156 = -786329453;    float hBoSJTwGnKihxbAQkFIWNaj96624704 = -314850863;    float hBoSJTwGnKihxbAQkFIWNaj37830785 = -350679735;    float hBoSJTwGnKihxbAQkFIWNaj34021749 = -333083326;    float hBoSJTwGnKihxbAQkFIWNaj83030059 = -206806852;    float hBoSJTwGnKihxbAQkFIWNaj32329229 = -727356307;    float hBoSJTwGnKihxbAQkFIWNaj19873752 = 43030288;    float hBoSJTwGnKihxbAQkFIWNaj9898768 = -970880046;    float hBoSJTwGnKihxbAQkFIWNaj58581860 = -454280008;    float hBoSJTwGnKihxbAQkFIWNaj55433626 = -755540433;    float hBoSJTwGnKihxbAQkFIWNaj84056985 = -959060464;    float hBoSJTwGnKihxbAQkFIWNaj92020766 = -73612968;    float hBoSJTwGnKihxbAQkFIWNaj60027039 = -990733371;    float hBoSJTwGnKihxbAQkFIWNaj85912663 = -821777527;    float hBoSJTwGnKihxbAQkFIWNaj66530051 = -983124823;    float hBoSJTwGnKihxbAQkFIWNaj76962231 = -903544384;    float hBoSJTwGnKihxbAQkFIWNaj57844013 = -361799492;    float hBoSJTwGnKihxbAQkFIWNaj98690506 = -349322410;    float hBoSJTwGnKihxbAQkFIWNaj94686750 = -944642957;    float hBoSJTwGnKihxbAQkFIWNaj61196306 = -858586948;    float hBoSJTwGnKihxbAQkFIWNaj86022323 = -335837496;    float hBoSJTwGnKihxbAQkFIWNaj60087007 = -378146732;    float hBoSJTwGnKihxbAQkFIWNaj24023160 = -389069754;    float hBoSJTwGnKihxbAQkFIWNaj75295075 = -91398841;    float hBoSJTwGnKihxbAQkFIWNaj99187648 = -866692344;    float hBoSJTwGnKihxbAQkFIWNaj26505570 = -312619390;    float hBoSJTwGnKihxbAQkFIWNaj21988981 = -242961272;    float hBoSJTwGnKihxbAQkFIWNaj12754003 = -362869060;    float hBoSJTwGnKihxbAQkFIWNaj30656281 = -625761430;    float hBoSJTwGnKihxbAQkFIWNaj87125500 = 92117412;    float hBoSJTwGnKihxbAQkFIWNaj18721168 = -527113675;    float hBoSJTwGnKihxbAQkFIWNaj42754970 = 49379082;    float hBoSJTwGnKihxbAQkFIWNaj30889769 = -15957795;    float hBoSJTwGnKihxbAQkFIWNaj50798137 = -16671308;    float hBoSJTwGnKihxbAQkFIWNaj24524354 = 91282072;    float hBoSJTwGnKihxbAQkFIWNaj63506359 = 19745073;    float hBoSJTwGnKihxbAQkFIWNaj38504397 = -532033153;    float hBoSJTwGnKihxbAQkFIWNaj61928205 = -26799941;    float hBoSJTwGnKihxbAQkFIWNaj98647000 = -472203388;    float hBoSJTwGnKihxbAQkFIWNaj35502655 = -541980776;    float hBoSJTwGnKihxbAQkFIWNaj84516327 = -680606295;    float hBoSJTwGnKihxbAQkFIWNaj11137573 = -138960291;    float hBoSJTwGnKihxbAQkFIWNaj18072635 = -395132290;    float hBoSJTwGnKihxbAQkFIWNaj35638618 = -653871897;    float hBoSJTwGnKihxbAQkFIWNaj13601787 = -174269285;    float hBoSJTwGnKihxbAQkFIWNaj80555592 = -926434073;    float hBoSJTwGnKihxbAQkFIWNaj71515187 = -173611064;    float hBoSJTwGnKihxbAQkFIWNaj20417425 = -880187311;    float hBoSJTwGnKihxbAQkFIWNaj63652481 = -553527557;    float hBoSJTwGnKihxbAQkFIWNaj10566303 = -819669140;    float hBoSJTwGnKihxbAQkFIWNaj20701599 = -786838155;    float hBoSJTwGnKihxbAQkFIWNaj94864440 = -199831649;    float hBoSJTwGnKihxbAQkFIWNaj61831364 = -542578027;    float hBoSJTwGnKihxbAQkFIWNaj26290887 = -53225301;    float hBoSJTwGnKihxbAQkFIWNaj97108348 = -868865022;    float hBoSJTwGnKihxbAQkFIWNaj45736149 = -308182721;    float hBoSJTwGnKihxbAQkFIWNaj72601544 = -925781110;    float hBoSJTwGnKihxbAQkFIWNaj62535709 = -159280894;    float hBoSJTwGnKihxbAQkFIWNaj34834100 = -466390983;    float hBoSJTwGnKihxbAQkFIWNaj56524490 = -894187462;    float hBoSJTwGnKihxbAQkFIWNaj10340248 = -384395036;    float hBoSJTwGnKihxbAQkFIWNaj7119750 = -594100653;    float hBoSJTwGnKihxbAQkFIWNaj79242486 = -245118617;    float hBoSJTwGnKihxbAQkFIWNaj71456360 = -446397420;    float hBoSJTwGnKihxbAQkFIWNaj36712459 = -128426758;    float hBoSJTwGnKihxbAQkFIWNaj41302015 = -908439546;    float hBoSJTwGnKihxbAQkFIWNaj61130997 = 42344827;    float hBoSJTwGnKihxbAQkFIWNaj9228902 = -874062063;    float hBoSJTwGnKihxbAQkFIWNaj61388310 = -813059600;    float hBoSJTwGnKihxbAQkFIWNaj3023693 = -902869897;    float hBoSJTwGnKihxbAQkFIWNaj38457834 = -271511231;    float hBoSJTwGnKihxbAQkFIWNaj95915808 = -234999551;    float hBoSJTwGnKihxbAQkFIWNaj43506 = -348974953;     hBoSJTwGnKihxbAQkFIWNaj18184815 = hBoSJTwGnKihxbAQkFIWNaj65671112;     hBoSJTwGnKihxbAQkFIWNaj65671112 = hBoSJTwGnKihxbAQkFIWNaj12276115;     hBoSJTwGnKihxbAQkFIWNaj12276115 = hBoSJTwGnKihxbAQkFIWNaj99273083;     hBoSJTwGnKihxbAQkFIWNaj99273083 = hBoSJTwGnKihxbAQkFIWNaj18376963;     hBoSJTwGnKihxbAQkFIWNaj18376963 = hBoSJTwGnKihxbAQkFIWNaj33928810;     hBoSJTwGnKihxbAQkFIWNaj33928810 = hBoSJTwGnKihxbAQkFIWNaj30347509;     hBoSJTwGnKihxbAQkFIWNaj30347509 = hBoSJTwGnKihxbAQkFIWNaj60131063;     hBoSJTwGnKihxbAQkFIWNaj60131063 = hBoSJTwGnKihxbAQkFIWNaj76335182;     hBoSJTwGnKihxbAQkFIWNaj76335182 = hBoSJTwGnKihxbAQkFIWNaj95949953;     hBoSJTwGnKihxbAQkFIWNaj95949953 = hBoSJTwGnKihxbAQkFIWNaj81677060;     hBoSJTwGnKihxbAQkFIWNaj81677060 = hBoSJTwGnKihxbAQkFIWNaj67831884;     hBoSJTwGnKihxbAQkFIWNaj67831884 = hBoSJTwGnKihxbAQkFIWNaj4390079;     hBoSJTwGnKihxbAQkFIWNaj4390079 = hBoSJTwGnKihxbAQkFIWNaj21036340;     hBoSJTwGnKihxbAQkFIWNaj21036340 = hBoSJTwGnKihxbAQkFIWNaj76654494;     hBoSJTwGnKihxbAQkFIWNaj76654494 = hBoSJTwGnKihxbAQkFIWNaj91072244;     hBoSJTwGnKihxbAQkFIWNaj91072244 = hBoSJTwGnKihxbAQkFIWNaj97658771;     hBoSJTwGnKihxbAQkFIWNaj97658771 = hBoSJTwGnKihxbAQkFIWNaj72576359;     hBoSJTwGnKihxbAQkFIWNaj72576359 = hBoSJTwGnKihxbAQkFIWNaj31542226;     hBoSJTwGnKihxbAQkFIWNaj31542226 = hBoSJTwGnKihxbAQkFIWNaj6330089;     hBoSJTwGnKihxbAQkFIWNaj6330089 = hBoSJTwGnKihxbAQkFIWNaj30182533;     hBoSJTwGnKihxbAQkFIWNaj30182533 = hBoSJTwGnKihxbAQkFIWNaj87528534;     hBoSJTwGnKihxbAQkFIWNaj87528534 = hBoSJTwGnKihxbAQkFIWNaj78545612;     hBoSJTwGnKihxbAQkFIWNaj78545612 = hBoSJTwGnKihxbAQkFIWNaj93554947;     hBoSJTwGnKihxbAQkFIWNaj93554947 = hBoSJTwGnKihxbAQkFIWNaj56518114;     hBoSJTwGnKihxbAQkFIWNaj56518114 = hBoSJTwGnKihxbAQkFIWNaj87487193;     hBoSJTwGnKihxbAQkFIWNaj87487193 = hBoSJTwGnKihxbAQkFIWNaj83130672;     hBoSJTwGnKihxbAQkFIWNaj83130672 = hBoSJTwGnKihxbAQkFIWNaj5823156;     hBoSJTwGnKihxbAQkFIWNaj5823156 = hBoSJTwGnKihxbAQkFIWNaj96624704;     hBoSJTwGnKihxbAQkFIWNaj96624704 = hBoSJTwGnKihxbAQkFIWNaj37830785;     hBoSJTwGnKihxbAQkFIWNaj37830785 = hBoSJTwGnKihxbAQkFIWNaj34021749;     hBoSJTwGnKihxbAQkFIWNaj34021749 = hBoSJTwGnKihxbAQkFIWNaj83030059;     hBoSJTwGnKihxbAQkFIWNaj83030059 = hBoSJTwGnKihxbAQkFIWNaj32329229;     hBoSJTwGnKihxbAQkFIWNaj32329229 = hBoSJTwGnKihxbAQkFIWNaj19873752;     hBoSJTwGnKihxbAQkFIWNaj19873752 = hBoSJTwGnKihxbAQkFIWNaj9898768;     hBoSJTwGnKihxbAQkFIWNaj9898768 = hBoSJTwGnKihxbAQkFIWNaj58581860;     hBoSJTwGnKihxbAQkFIWNaj58581860 = hBoSJTwGnKihxbAQkFIWNaj55433626;     hBoSJTwGnKihxbAQkFIWNaj55433626 = hBoSJTwGnKihxbAQkFIWNaj84056985;     hBoSJTwGnKihxbAQkFIWNaj84056985 = hBoSJTwGnKihxbAQkFIWNaj92020766;     hBoSJTwGnKihxbAQkFIWNaj92020766 = hBoSJTwGnKihxbAQkFIWNaj60027039;     hBoSJTwGnKihxbAQkFIWNaj60027039 = hBoSJTwGnKihxbAQkFIWNaj85912663;     hBoSJTwGnKihxbAQkFIWNaj85912663 = hBoSJTwGnKihxbAQkFIWNaj66530051;     hBoSJTwGnKihxbAQkFIWNaj66530051 = hBoSJTwGnKihxbAQkFIWNaj76962231;     hBoSJTwGnKihxbAQkFIWNaj76962231 = hBoSJTwGnKihxbAQkFIWNaj57844013;     hBoSJTwGnKihxbAQkFIWNaj57844013 = hBoSJTwGnKihxbAQkFIWNaj98690506;     hBoSJTwGnKihxbAQkFIWNaj98690506 = hBoSJTwGnKihxbAQkFIWNaj94686750;     hBoSJTwGnKihxbAQkFIWNaj94686750 = hBoSJTwGnKihxbAQkFIWNaj61196306;     hBoSJTwGnKihxbAQkFIWNaj61196306 = hBoSJTwGnKihxbAQkFIWNaj86022323;     hBoSJTwGnKihxbAQkFIWNaj86022323 = hBoSJTwGnKihxbAQkFIWNaj60087007;     hBoSJTwGnKihxbAQkFIWNaj60087007 = hBoSJTwGnKihxbAQkFIWNaj24023160;     hBoSJTwGnKihxbAQkFIWNaj24023160 = hBoSJTwGnKihxbAQkFIWNaj75295075;     hBoSJTwGnKihxbAQkFIWNaj75295075 = hBoSJTwGnKihxbAQkFIWNaj99187648;     hBoSJTwGnKihxbAQkFIWNaj99187648 = hBoSJTwGnKihxbAQkFIWNaj26505570;     hBoSJTwGnKihxbAQkFIWNaj26505570 = hBoSJTwGnKihxbAQkFIWNaj21988981;     hBoSJTwGnKihxbAQkFIWNaj21988981 = hBoSJTwGnKihxbAQkFIWNaj12754003;     hBoSJTwGnKihxbAQkFIWNaj12754003 = hBoSJTwGnKihxbAQkFIWNaj30656281;     hBoSJTwGnKihxbAQkFIWNaj30656281 = hBoSJTwGnKihxbAQkFIWNaj87125500;     hBoSJTwGnKihxbAQkFIWNaj87125500 = hBoSJTwGnKihxbAQkFIWNaj18721168;     hBoSJTwGnKihxbAQkFIWNaj18721168 = hBoSJTwGnKihxbAQkFIWNaj42754970;     hBoSJTwGnKihxbAQkFIWNaj42754970 = hBoSJTwGnKihxbAQkFIWNaj30889769;     hBoSJTwGnKihxbAQkFIWNaj30889769 = hBoSJTwGnKihxbAQkFIWNaj50798137;     hBoSJTwGnKihxbAQkFIWNaj50798137 = hBoSJTwGnKihxbAQkFIWNaj24524354;     hBoSJTwGnKihxbAQkFIWNaj24524354 = hBoSJTwGnKihxbAQkFIWNaj63506359;     hBoSJTwGnKihxbAQkFIWNaj63506359 = hBoSJTwGnKihxbAQkFIWNaj38504397;     hBoSJTwGnKihxbAQkFIWNaj38504397 = hBoSJTwGnKihxbAQkFIWNaj61928205;     hBoSJTwGnKihxbAQkFIWNaj61928205 = hBoSJTwGnKihxbAQkFIWNaj98647000;     hBoSJTwGnKihxbAQkFIWNaj98647000 = hBoSJTwGnKihxbAQkFIWNaj35502655;     hBoSJTwGnKihxbAQkFIWNaj35502655 = hBoSJTwGnKihxbAQkFIWNaj84516327;     hBoSJTwGnKihxbAQkFIWNaj84516327 = hBoSJTwGnKihxbAQkFIWNaj11137573;     hBoSJTwGnKihxbAQkFIWNaj11137573 = hBoSJTwGnKihxbAQkFIWNaj18072635;     hBoSJTwGnKihxbAQkFIWNaj18072635 = hBoSJTwGnKihxbAQkFIWNaj35638618;     hBoSJTwGnKihxbAQkFIWNaj35638618 = hBoSJTwGnKihxbAQkFIWNaj13601787;     hBoSJTwGnKihxbAQkFIWNaj13601787 = hBoSJTwGnKihxbAQkFIWNaj80555592;     hBoSJTwGnKihxbAQkFIWNaj80555592 = hBoSJTwGnKihxbAQkFIWNaj71515187;     hBoSJTwGnKihxbAQkFIWNaj71515187 = hBoSJTwGnKihxbAQkFIWNaj20417425;     hBoSJTwGnKihxbAQkFIWNaj20417425 = hBoSJTwGnKihxbAQkFIWNaj63652481;     hBoSJTwGnKihxbAQkFIWNaj63652481 = hBoSJTwGnKihxbAQkFIWNaj10566303;     hBoSJTwGnKihxbAQkFIWNaj10566303 = hBoSJTwGnKihxbAQkFIWNaj20701599;     hBoSJTwGnKihxbAQkFIWNaj20701599 = hBoSJTwGnKihxbAQkFIWNaj94864440;     hBoSJTwGnKihxbAQkFIWNaj94864440 = hBoSJTwGnKihxbAQkFIWNaj61831364;     hBoSJTwGnKihxbAQkFIWNaj61831364 = hBoSJTwGnKihxbAQkFIWNaj26290887;     hBoSJTwGnKihxbAQkFIWNaj26290887 = hBoSJTwGnKihxbAQkFIWNaj97108348;     hBoSJTwGnKihxbAQkFIWNaj97108348 = hBoSJTwGnKihxbAQkFIWNaj45736149;     hBoSJTwGnKihxbAQkFIWNaj45736149 = hBoSJTwGnKihxbAQkFIWNaj72601544;     hBoSJTwGnKihxbAQkFIWNaj72601544 = hBoSJTwGnKihxbAQkFIWNaj62535709;     hBoSJTwGnKihxbAQkFIWNaj62535709 = hBoSJTwGnKihxbAQkFIWNaj34834100;     hBoSJTwGnKihxbAQkFIWNaj34834100 = hBoSJTwGnKihxbAQkFIWNaj56524490;     hBoSJTwGnKihxbAQkFIWNaj56524490 = hBoSJTwGnKihxbAQkFIWNaj10340248;     hBoSJTwGnKihxbAQkFIWNaj10340248 = hBoSJTwGnKihxbAQkFIWNaj7119750;     hBoSJTwGnKihxbAQkFIWNaj7119750 = hBoSJTwGnKihxbAQkFIWNaj79242486;     hBoSJTwGnKihxbAQkFIWNaj79242486 = hBoSJTwGnKihxbAQkFIWNaj71456360;     hBoSJTwGnKihxbAQkFIWNaj71456360 = hBoSJTwGnKihxbAQkFIWNaj36712459;     hBoSJTwGnKihxbAQkFIWNaj36712459 = hBoSJTwGnKihxbAQkFIWNaj41302015;     hBoSJTwGnKihxbAQkFIWNaj41302015 = hBoSJTwGnKihxbAQkFIWNaj61130997;     hBoSJTwGnKihxbAQkFIWNaj61130997 = hBoSJTwGnKihxbAQkFIWNaj9228902;     hBoSJTwGnKihxbAQkFIWNaj9228902 = hBoSJTwGnKihxbAQkFIWNaj61388310;     hBoSJTwGnKihxbAQkFIWNaj61388310 = hBoSJTwGnKihxbAQkFIWNaj3023693;     hBoSJTwGnKihxbAQkFIWNaj3023693 = hBoSJTwGnKihxbAQkFIWNaj38457834;     hBoSJTwGnKihxbAQkFIWNaj38457834 = hBoSJTwGnKihxbAQkFIWNaj95915808;     hBoSJTwGnKihxbAQkFIWNaj95915808 = hBoSJTwGnKihxbAQkFIWNaj43506;     hBoSJTwGnKihxbAQkFIWNaj43506 = hBoSJTwGnKihxbAQkFIWNaj18184815;}



void nppzwvqxhzXmsmGDUHigJTFAGa27999373() {     float uKXqEmzvLVuTiRsccJAieCI1908333 = -698997020;    float uKXqEmzvLVuTiRsccJAieCI63269208 = 39827915;    float uKXqEmzvLVuTiRsccJAieCI862232 = -44575423;    float uKXqEmzvLVuTiRsccJAieCI96421036 = -542723830;    float uKXqEmzvLVuTiRsccJAieCI97938298 = -425718235;    float uKXqEmzvLVuTiRsccJAieCI45268327 = -424694494;    float uKXqEmzvLVuTiRsccJAieCI67228072 = -816561518;    float uKXqEmzvLVuTiRsccJAieCI3471146 = 84439624;    float uKXqEmzvLVuTiRsccJAieCI26554711 = -580195584;    float uKXqEmzvLVuTiRsccJAieCI42055542 = -682924282;    float uKXqEmzvLVuTiRsccJAieCI24566096 = -619295497;    float uKXqEmzvLVuTiRsccJAieCI58043655 = -198476354;    float uKXqEmzvLVuTiRsccJAieCI55443931 = -580408251;    float uKXqEmzvLVuTiRsccJAieCI15409484 = -459523199;    float uKXqEmzvLVuTiRsccJAieCI80833753 = -875109603;    float uKXqEmzvLVuTiRsccJAieCI62886273 = -333539981;    float uKXqEmzvLVuTiRsccJAieCI97524182 = -292209611;    float uKXqEmzvLVuTiRsccJAieCI86653628 = -710898614;    float uKXqEmzvLVuTiRsccJAieCI72518706 = 47006327;    float uKXqEmzvLVuTiRsccJAieCI80437733 = -318257754;    float uKXqEmzvLVuTiRsccJAieCI97522042 = -111118900;    float uKXqEmzvLVuTiRsccJAieCI88530605 = -636368810;    float uKXqEmzvLVuTiRsccJAieCI47721477 = -178032383;    float uKXqEmzvLVuTiRsccJAieCI94697642 = -188386711;    float uKXqEmzvLVuTiRsccJAieCI93120207 = -565551745;    float uKXqEmzvLVuTiRsccJAieCI40479951 = -354779277;    float uKXqEmzvLVuTiRsccJAieCI4722612 = -648577940;    float uKXqEmzvLVuTiRsccJAieCI48422356 = -572770941;    float uKXqEmzvLVuTiRsccJAieCI68542845 = -839046764;    float uKXqEmzvLVuTiRsccJAieCI73379112 = 76713891;    float uKXqEmzvLVuTiRsccJAieCI15386172 = -552181346;    float uKXqEmzvLVuTiRsccJAieCI7823348 = -756004074;    float uKXqEmzvLVuTiRsccJAieCI46453930 = -292570399;    float uKXqEmzvLVuTiRsccJAieCI55598466 = -361066360;    float uKXqEmzvLVuTiRsccJAieCI77502092 = -705170654;    float uKXqEmzvLVuTiRsccJAieCI28443094 = -432676485;    float uKXqEmzvLVuTiRsccJAieCI55197216 = 24118271;    float uKXqEmzvLVuTiRsccJAieCI34438776 = -858203681;    float uKXqEmzvLVuTiRsccJAieCI52636486 = -379171849;    float uKXqEmzvLVuTiRsccJAieCI99216154 = -937380251;    float uKXqEmzvLVuTiRsccJAieCI50680184 = -471817653;    float uKXqEmzvLVuTiRsccJAieCI95869778 = -589150492;    float uKXqEmzvLVuTiRsccJAieCI9762378 = -555948639;    float uKXqEmzvLVuTiRsccJAieCI5039533 = 59256776;    float uKXqEmzvLVuTiRsccJAieCI26677757 = -675336488;    float uKXqEmzvLVuTiRsccJAieCI38953632 = -559674649;    float uKXqEmzvLVuTiRsccJAieCI94375275 = -880232855;    float uKXqEmzvLVuTiRsccJAieCI24764171 = -97959393;    float uKXqEmzvLVuTiRsccJAieCI93961220 = -77930883;    float uKXqEmzvLVuTiRsccJAieCI59896489 = 78914810;    float uKXqEmzvLVuTiRsccJAieCI15385907 = -257171243;    float uKXqEmzvLVuTiRsccJAieCI50693369 = -618745130;    float uKXqEmzvLVuTiRsccJAieCI71887972 = -324551756;    float uKXqEmzvLVuTiRsccJAieCI67921587 = -819670460;    float uKXqEmzvLVuTiRsccJAieCI30477330 = -679596005;    float uKXqEmzvLVuTiRsccJAieCI13377728 = 37371790;    float uKXqEmzvLVuTiRsccJAieCI15547731 = -782139703;    float uKXqEmzvLVuTiRsccJAieCI6164590 = -856188713;    float uKXqEmzvLVuTiRsccJAieCI3300830 = -977172086;    float uKXqEmzvLVuTiRsccJAieCI57458348 = 29061041;    float uKXqEmzvLVuTiRsccJAieCI40545715 = -776116554;    float uKXqEmzvLVuTiRsccJAieCI18805717 = -143790577;    float uKXqEmzvLVuTiRsccJAieCI34928300 = -76513613;    float uKXqEmzvLVuTiRsccJAieCI53175599 = -556909475;    float uKXqEmzvLVuTiRsccJAieCI26669371 = -30742936;    float uKXqEmzvLVuTiRsccJAieCI16742749 = -863291423;    float uKXqEmzvLVuTiRsccJAieCI11589725 = -905905956;    float uKXqEmzvLVuTiRsccJAieCI99845464 = -119341891;    float uKXqEmzvLVuTiRsccJAieCI37907392 = -754352546;    float uKXqEmzvLVuTiRsccJAieCI52390659 = -342433118;    float uKXqEmzvLVuTiRsccJAieCI7689057 = -257658252;    float uKXqEmzvLVuTiRsccJAieCI63085406 = -434005931;    float uKXqEmzvLVuTiRsccJAieCI34017143 = -231726765;    float uKXqEmzvLVuTiRsccJAieCI73302551 = -15613423;    float uKXqEmzvLVuTiRsccJAieCI29757549 = -846440101;    float uKXqEmzvLVuTiRsccJAieCI1652265 = -521968409;    float uKXqEmzvLVuTiRsccJAieCI78768227 = 19579828;    float uKXqEmzvLVuTiRsccJAieCI42681944 = -137289159;    float uKXqEmzvLVuTiRsccJAieCI68019885 = -513050224;    float uKXqEmzvLVuTiRsccJAieCI54166575 = 94122903;    float uKXqEmzvLVuTiRsccJAieCI46104676 = -474546422;    float uKXqEmzvLVuTiRsccJAieCI79958440 = -450618547;    float uKXqEmzvLVuTiRsccJAieCI54461135 = -394840058;    float uKXqEmzvLVuTiRsccJAieCI8646356 = -817961575;    float uKXqEmzvLVuTiRsccJAieCI57993205 = -666114867;    float uKXqEmzvLVuTiRsccJAieCI64692803 = -933436217;    float uKXqEmzvLVuTiRsccJAieCI35935376 = -331452319;    float uKXqEmzvLVuTiRsccJAieCI78532343 = -472899939;    float uKXqEmzvLVuTiRsccJAieCI25121136 = -681470356;    float uKXqEmzvLVuTiRsccJAieCI64124365 = -642542444;    float uKXqEmzvLVuTiRsccJAieCI12895364 = -650536783;    float uKXqEmzvLVuTiRsccJAieCI49032627 = -119693017;    float uKXqEmzvLVuTiRsccJAieCI31137946 = -881031595;    float uKXqEmzvLVuTiRsccJAieCI95178137 = -308232891;    float uKXqEmzvLVuTiRsccJAieCI58670439 = -61263697;    float uKXqEmzvLVuTiRsccJAieCI31874468 = -228027077;    float uKXqEmzvLVuTiRsccJAieCI60941478 = -412636880;    float uKXqEmzvLVuTiRsccJAieCI56586779 = -999039164;    float uKXqEmzvLVuTiRsccJAieCI78370162 = -910000289;    float uKXqEmzvLVuTiRsccJAieCI9935009 = -698997020;     uKXqEmzvLVuTiRsccJAieCI1908333 = uKXqEmzvLVuTiRsccJAieCI63269208;     uKXqEmzvLVuTiRsccJAieCI63269208 = uKXqEmzvLVuTiRsccJAieCI862232;     uKXqEmzvLVuTiRsccJAieCI862232 = uKXqEmzvLVuTiRsccJAieCI96421036;     uKXqEmzvLVuTiRsccJAieCI96421036 = uKXqEmzvLVuTiRsccJAieCI97938298;     uKXqEmzvLVuTiRsccJAieCI97938298 = uKXqEmzvLVuTiRsccJAieCI45268327;     uKXqEmzvLVuTiRsccJAieCI45268327 = uKXqEmzvLVuTiRsccJAieCI67228072;     uKXqEmzvLVuTiRsccJAieCI67228072 = uKXqEmzvLVuTiRsccJAieCI3471146;     uKXqEmzvLVuTiRsccJAieCI3471146 = uKXqEmzvLVuTiRsccJAieCI26554711;     uKXqEmzvLVuTiRsccJAieCI26554711 = uKXqEmzvLVuTiRsccJAieCI42055542;     uKXqEmzvLVuTiRsccJAieCI42055542 = uKXqEmzvLVuTiRsccJAieCI24566096;     uKXqEmzvLVuTiRsccJAieCI24566096 = uKXqEmzvLVuTiRsccJAieCI58043655;     uKXqEmzvLVuTiRsccJAieCI58043655 = uKXqEmzvLVuTiRsccJAieCI55443931;     uKXqEmzvLVuTiRsccJAieCI55443931 = uKXqEmzvLVuTiRsccJAieCI15409484;     uKXqEmzvLVuTiRsccJAieCI15409484 = uKXqEmzvLVuTiRsccJAieCI80833753;     uKXqEmzvLVuTiRsccJAieCI80833753 = uKXqEmzvLVuTiRsccJAieCI62886273;     uKXqEmzvLVuTiRsccJAieCI62886273 = uKXqEmzvLVuTiRsccJAieCI97524182;     uKXqEmzvLVuTiRsccJAieCI97524182 = uKXqEmzvLVuTiRsccJAieCI86653628;     uKXqEmzvLVuTiRsccJAieCI86653628 = uKXqEmzvLVuTiRsccJAieCI72518706;     uKXqEmzvLVuTiRsccJAieCI72518706 = uKXqEmzvLVuTiRsccJAieCI80437733;     uKXqEmzvLVuTiRsccJAieCI80437733 = uKXqEmzvLVuTiRsccJAieCI97522042;     uKXqEmzvLVuTiRsccJAieCI97522042 = uKXqEmzvLVuTiRsccJAieCI88530605;     uKXqEmzvLVuTiRsccJAieCI88530605 = uKXqEmzvLVuTiRsccJAieCI47721477;     uKXqEmzvLVuTiRsccJAieCI47721477 = uKXqEmzvLVuTiRsccJAieCI94697642;     uKXqEmzvLVuTiRsccJAieCI94697642 = uKXqEmzvLVuTiRsccJAieCI93120207;     uKXqEmzvLVuTiRsccJAieCI93120207 = uKXqEmzvLVuTiRsccJAieCI40479951;     uKXqEmzvLVuTiRsccJAieCI40479951 = uKXqEmzvLVuTiRsccJAieCI4722612;     uKXqEmzvLVuTiRsccJAieCI4722612 = uKXqEmzvLVuTiRsccJAieCI48422356;     uKXqEmzvLVuTiRsccJAieCI48422356 = uKXqEmzvLVuTiRsccJAieCI68542845;     uKXqEmzvLVuTiRsccJAieCI68542845 = uKXqEmzvLVuTiRsccJAieCI73379112;     uKXqEmzvLVuTiRsccJAieCI73379112 = uKXqEmzvLVuTiRsccJAieCI15386172;     uKXqEmzvLVuTiRsccJAieCI15386172 = uKXqEmzvLVuTiRsccJAieCI7823348;     uKXqEmzvLVuTiRsccJAieCI7823348 = uKXqEmzvLVuTiRsccJAieCI46453930;     uKXqEmzvLVuTiRsccJAieCI46453930 = uKXqEmzvLVuTiRsccJAieCI55598466;     uKXqEmzvLVuTiRsccJAieCI55598466 = uKXqEmzvLVuTiRsccJAieCI77502092;     uKXqEmzvLVuTiRsccJAieCI77502092 = uKXqEmzvLVuTiRsccJAieCI28443094;     uKXqEmzvLVuTiRsccJAieCI28443094 = uKXqEmzvLVuTiRsccJAieCI55197216;     uKXqEmzvLVuTiRsccJAieCI55197216 = uKXqEmzvLVuTiRsccJAieCI34438776;     uKXqEmzvLVuTiRsccJAieCI34438776 = uKXqEmzvLVuTiRsccJAieCI52636486;     uKXqEmzvLVuTiRsccJAieCI52636486 = uKXqEmzvLVuTiRsccJAieCI99216154;     uKXqEmzvLVuTiRsccJAieCI99216154 = uKXqEmzvLVuTiRsccJAieCI50680184;     uKXqEmzvLVuTiRsccJAieCI50680184 = uKXqEmzvLVuTiRsccJAieCI95869778;     uKXqEmzvLVuTiRsccJAieCI95869778 = uKXqEmzvLVuTiRsccJAieCI9762378;     uKXqEmzvLVuTiRsccJAieCI9762378 = uKXqEmzvLVuTiRsccJAieCI5039533;     uKXqEmzvLVuTiRsccJAieCI5039533 = uKXqEmzvLVuTiRsccJAieCI26677757;     uKXqEmzvLVuTiRsccJAieCI26677757 = uKXqEmzvLVuTiRsccJAieCI38953632;     uKXqEmzvLVuTiRsccJAieCI38953632 = uKXqEmzvLVuTiRsccJAieCI94375275;     uKXqEmzvLVuTiRsccJAieCI94375275 = uKXqEmzvLVuTiRsccJAieCI24764171;     uKXqEmzvLVuTiRsccJAieCI24764171 = uKXqEmzvLVuTiRsccJAieCI93961220;     uKXqEmzvLVuTiRsccJAieCI93961220 = uKXqEmzvLVuTiRsccJAieCI59896489;     uKXqEmzvLVuTiRsccJAieCI59896489 = uKXqEmzvLVuTiRsccJAieCI15385907;     uKXqEmzvLVuTiRsccJAieCI15385907 = uKXqEmzvLVuTiRsccJAieCI50693369;     uKXqEmzvLVuTiRsccJAieCI50693369 = uKXqEmzvLVuTiRsccJAieCI71887972;     uKXqEmzvLVuTiRsccJAieCI71887972 = uKXqEmzvLVuTiRsccJAieCI67921587;     uKXqEmzvLVuTiRsccJAieCI67921587 = uKXqEmzvLVuTiRsccJAieCI30477330;     uKXqEmzvLVuTiRsccJAieCI30477330 = uKXqEmzvLVuTiRsccJAieCI13377728;     uKXqEmzvLVuTiRsccJAieCI13377728 = uKXqEmzvLVuTiRsccJAieCI15547731;     uKXqEmzvLVuTiRsccJAieCI15547731 = uKXqEmzvLVuTiRsccJAieCI6164590;     uKXqEmzvLVuTiRsccJAieCI6164590 = uKXqEmzvLVuTiRsccJAieCI3300830;     uKXqEmzvLVuTiRsccJAieCI3300830 = uKXqEmzvLVuTiRsccJAieCI57458348;     uKXqEmzvLVuTiRsccJAieCI57458348 = uKXqEmzvLVuTiRsccJAieCI40545715;     uKXqEmzvLVuTiRsccJAieCI40545715 = uKXqEmzvLVuTiRsccJAieCI18805717;     uKXqEmzvLVuTiRsccJAieCI18805717 = uKXqEmzvLVuTiRsccJAieCI34928300;     uKXqEmzvLVuTiRsccJAieCI34928300 = uKXqEmzvLVuTiRsccJAieCI53175599;     uKXqEmzvLVuTiRsccJAieCI53175599 = uKXqEmzvLVuTiRsccJAieCI26669371;     uKXqEmzvLVuTiRsccJAieCI26669371 = uKXqEmzvLVuTiRsccJAieCI16742749;     uKXqEmzvLVuTiRsccJAieCI16742749 = uKXqEmzvLVuTiRsccJAieCI11589725;     uKXqEmzvLVuTiRsccJAieCI11589725 = uKXqEmzvLVuTiRsccJAieCI99845464;     uKXqEmzvLVuTiRsccJAieCI99845464 = uKXqEmzvLVuTiRsccJAieCI37907392;     uKXqEmzvLVuTiRsccJAieCI37907392 = uKXqEmzvLVuTiRsccJAieCI52390659;     uKXqEmzvLVuTiRsccJAieCI52390659 = uKXqEmzvLVuTiRsccJAieCI7689057;     uKXqEmzvLVuTiRsccJAieCI7689057 = uKXqEmzvLVuTiRsccJAieCI63085406;     uKXqEmzvLVuTiRsccJAieCI63085406 = uKXqEmzvLVuTiRsccJAieCI34017143;     uKXqEmzvLVuTiRsccJAieCI34017143 = uKXqEmzvLVuTiRsccJAieCI73302551;     uKXqEmzvLVuTiRsccJAieCI73302551 = uKXqEmzvLVuTiRsccJAieCI29757549;     uKXqEmzvLVuTiRsccJAieCI29757549 = uKXqEmzvLVuTiRsccJAieCI1652265;     uKXqEmzvLVuTiRsccJAieCI1652265 = uKXqEmzvLVuTiRsccJAieCI78768227;     uKXqEmzvLVuTiRsccJAieCI78768227 = uKXqEmzvLVuTiRsccJAieCI42681944;     uKXqEmzvLVuTiRsccJAieCI42681944 = uKXqEmzvLVuTiRsccJAieCI68019885;     uKXqEmzvLVuTiRsccJAieCI68019885 = uKXqEmzvLVuTiRsccJAieCI54166575;     uKXqEmzvLVuTiRsccJAieCI54166575 = uKXqEmzvLVuTiRsccJAieCI46104676;     uKXqEmzvLVuTiRsccJAieCI46104676 = uKXqEmzvLVuTiRsccJAieCI79958440;     uKXqEmzvLVuTiRsccJAieCI79958440 = uKXqEmzvLVuTiRsccJAieCI54461135;     uKXqEmzvLVuTiRsccJAieCI54461135 = uKXqEmzvLVuTiRsccJAieCI8646356;     uKXqEmzvLVuTiRsccJAieCI8646356 = uKXqEmzvLVuTiRsccJAieCI57993205;     uKXqEmzvLVuTiRsccJAieCI57993205 = uKXqEmzvLVuTiRsccJAieCI64692803;     uKXqEmzvLVuTiRsccJAieCI64692803 = uKXqEmzvLVuTiRsccJAieCI35935376;     uKXqEmzvLVuTiRsccJAieCI35935376 = uKXqEmzvLVuTiRsccJAieCI78532343;     uKXqEmzvLVuTiRsccJAieCI78532343 = uKXqEmzvLVuTiRsccJAieCI25121136;     uKXqEmzvLVuTiRsccJAieCI25121136 = uKXqEmzvLVuTiRsccJAieCI64124365;     uKXqEmzvLVuTiRsccJAieCI64124365 = uKXqEmzvLVuTiRsccJAieCI12895364;     uKXqEmzvLVuTiRsccJAieCI12895364 = uKXqEmzvLVuTiRsccJAieCI49032627;     uKXqEmzvLVuTiRsccJAieCI49032627 = uKXqEmzvLVuTiRsccJAieCI31137946;     uKXqEmzvLVuTiRsccJAieCI31137946 = uKXqEmzvLVuTiRsccJAieCI95178137;     uKXqEmzvLVuTiRsccJAieCI95178137 = uKXqEmzvLVuTiRsccJAieCI58670439;     uKXqEmzvLVuTiRsccJAieCI58670439 = uKXqEmzvLVuTiRsccJAieCI31874468;     uKXqEmzvLVuTiRsccJAieCI31874468 = uKXqEmzvLVuTiRsccJAieCI60941478;     uKXqEmzvLVuTiRsccJAieCI60941478 = uKXqEmzvLVuTiRsccJAieCI56586779;     uKXqEmzvLVuTiRsccJAieCI56586779 = uKXqEmzvLVuTiRsccJAieCI78370162;     uKXqEmzvLVuTiRsccJAieCI78370162 = uKXqEmzvLVuTiRsccJAieCI9935009;     uKXqEmzvLVuTiRsccJAieCI9935009 = uKXqEmzvLVuTiRsccJAieCI1908333;}



void NifwZAWKRHtTzndMqEykcgdRNJ13062211() {     float loLuSgEXPQDkVtpqoCCeEFH85631850 = 50980913;    float loLuSgEXPQDkVtpqoCCeEFH60867303 = -863823936;    float loLuSgEXPQDkVtpqoCCeEFH89448349 = 87116886;    float loLuSgEXPQDkVtpqoCCeEFH93568990 = -547605759;    float loLuSgEXPQDkVtpqoCCeEFH77499635 = -823666426;    float loLuSgEXPQDkVtpqoCCeEFH56607845 = -528015163;    float loLuSgEXPQDkVtpqoCCeEFH4108636 = -838075656;    float loLuSgEXPQDkVtpqoCCeEFH46811229 = -536014962;    float loLuSgEXPQDkVtpqoCCeEFH76774239 = -177678281;    float loLuSgEXPQDkVtpqoCCeEFH88161130 = -905965298;    float loLuSgEXPQDkVtpqoCCeEFH67455132 = -459580755;    float loLuSgEXPQDkVtpqoCCeEFH48255425 = -127615625;    float loLuSgEXPQDkVtpqoCCeEFH6497783 = -423240494;    float loLuSgEXPQDkVtpqoCCeEFH9782629 = -809206062;    float loLuSgEXPQDkVtpqoCCeEFH85013012 = -800806908;    float loLuSgEXPQDkVtpqoCCeEFH34700301 = -257667632;    float loLuSgEXPQDkVtpqoCCeEFH97389593 = -451089474;    float loLuSgEXPQDkVtpqoCCeEFH730899 = -321750188;    float loLuSgEXPQDkVtpqoCCeEFH13495187 = -841642911;    float loLuSgEXPQDkVtpqoCCeEFH54545378 = 65449329;    float loLuSgEXPQDkVtpqoCCeEFH64861553 = -785585421;    float loLuSgEXPQDkVtpqoCCeEFH89532676 = -549524096;    float loLuSgEXPQDkVtpqoCCeEFH16897343 = -207427120;    float loLuSgEXPQDkVtpqoCCeEFH95840337 = -827619364;    float loLuSgEXPQDkVtpqoCCeEFH29722300 = -643882507;    float loLuSgEXPQDkVtpqoCCeEFH93472708 = -797746304;    float loLuSgEXPQDkVtpqoCCeEFH26314551 = 7546637;    float loLuSgEXPQDkVtpqoCCeEFH91021555 = -359212430;    float loLuSgEXPQDkVtpqoCCeEFH40460987 = -263242664;    float loLuSgEXPQDkVtpqoCCeEFH8927440 = -595892484;    float loLuSgEXPQDkVtpqoCCeEFH96750594 = -771279367;    float loLuSgEXPQDkVtpqoCCeEFH32616636 = -205201297;    float loLuSgEXPQDkVtpqoCCeEFH60578631 = -957784490;    float loLuSgEXPQDkVtpqoCCeEFH91323180 = -765163008;    float loLuSgEXPQDkVtpqoCCeEFH45105418 = -439461261;    float loLuSgEXPQDkVtpqoCCeEFH98304327 = -411072963;    float loLuSgEXPQDkVtpqoCCeEFH54960807 = -296223025;    float loLuSgEXPQDkVtpqoCCeEFH84820567 = -757346897;    float loLuSgEXPQDkVtpqoCCeEFH13252205 = -684730730;    float loLuSgEXPQDkVtpqoCCeEFH38405270 = -884027130;    float loLuSgEXPQDkVtpqoCCeEFH15447705 = -121857780;    float loLuSgEXPQDkVtpqoCCeEFH25209505 = -195176162;    float loLuSgEXPQDkVtpqoCCeEFH42562524 = -208352893;    float loLuSgEXPQDkVtpqoCCeEFH52235052 = -619686957;    float loLuSgEXPQDkVtpqoCCeEFH54665008 = 98649434;    float loLuSgEXPQDkVtpqoCCeEFH83220513 = -174706341;    float loLuSgEXPQDkVtpqoCCeEFH27554245 = -901878762;    float loLuSgEXPQDkVtpqoCCeEFH63506018 = -960081291;    float loLuSgEXPQDkVtpqoCCeEFH27835434 = -877715035;    float loLuSgEXPQDkVtpqoCCeEFH95769819 = -553100625;    float loLuSgEXPQDkVtpqoCCeEFH55476738 = -422943644;    float loLuSgEXPQDkVtpqoCCeEFH2199089 = -370797916;    float loLuSgEXPQDkVtpqoCCeEFH17270375 = -336484122;    float loLuSgEXPQDkVtpqoCCeEFH13854193 = -296379648;    float loLuSgEXPQDkVtpqoCCeEFH48200658 = -996322950;    float loLuSgEXPQDkVtpqoCCeEFH96099174 = -399494991;    float loLuSgEXPQDkVtpqoCCeEFH43969961 = -556396817;    float loLuSgEXPQDkVtpqoCCeEFH93608011 = -85263750;    float loLuSgEXPQDkVtpqoCCeEFH63846690 = -903723253;    float loLuSgEXPQDkVtpqoCCeEFH84026926 = 74079877;    float loLuSgEXPQDkVtpqoCCeEFH30293294 = -435561800;    float loLuSgEXPQDkVtpqoCCeEFH13087080 = -378863227;    float loLuSgEXPQDkVtpqoCCeEFH6350242 = -172772298;    float loLuSgEXPQDkVtpqoCCeEFH67846800 = -581785797;    float loLuSgEXPQDkVtpqoCCeEFH91410535 = -34685931;    float loLuSgEXPQDkVtpqoCCeEFH34838496 = -154379458;    float loLuSgEXPQDkVtpqoCCeEFH87676793 = -169831136;    float loLuSgEXPQDkVtpqoCCeEFH15174603 = -658077487;    float loLuSgEXPQDkVtpqoCCeEFH64677210 = -269744801;    float loLuSgEXPQDkVtpqoCCeEFH86708684 = -289733946;    float loLuSgEXPQDkVtpqoCCeEFH79739494 = -961444607;    float loLuSgEXPQDkVtpqoCCeEFH12569026 = -693742577;    float loLuSgEXPQDkVtpqoCCeEFH87478693 = -637019458;    float loLuSgEXPQDkVtpqoCCeEFH75089916 = -957615781;    float loLuSgEXPQDkVtpqoCCeEFH39097673 = -812692892;    float loLuSgEXPQDkVtpqoCCeEFH39652048 = -490409260;    float loLuSgEXPQDkVtpqoCCeEFH46970152 = -241171204;    float loLuSgEXPQDkVtpqoCCeEFH64662290 = -587740163;    float loLuSgEXPQDkVtpqoCCeEFH41175329 = -826268798;    float loLuSgEXPQDkVtpqoCCeEFH46501787 = -369176166;    float loLuSgEXPQDkVtpqoCCeEFH65918464 = -895867543;    float loLuSgEXPQDkVtpqoCCeEFH62808533 = -32372072;    float loLuSgEXPQDkVtpqoCCeEFH63186122 = -481497396;    float loLuSgEXPQDkVtpqoCCeEFH44691168 = -710142039;    float loLuSgEXPQDkVtpqoCCeEFH53450701 = -72948840;    float loLuSgEXPQDkVtpqoCCeEFH94551505 = -300481451;    float loLuSgEXPQDkVtpqoCCeEFH15346262 = -868717176;    float loLuSgEXPQDkVtpqoCCeEFH46724439 = -561404843;    float loLuSgEXPQDkVtpqoCCeEFH43122522 = -768840058;    float loLuSgEXPQDkVtpqoCCeEFH49006244 = 60033729;    float loLuSgEXPQDkVtpqoCCeEFH54334367 = -854676146;    float loLuSgEXPQDkVtpqoCCeEFH61352795 = -110959276;    float loLuSgEXPQDkVtpqoCCeEFH20973877 = -853623645;    float loLuSgEXPQDkVtpqoCCeEFH29225278 = -658810608;    float loLuSgEXPQDkVtpqoCCeEFH8111977 = -348465331;    float loLuSgEXPQDkVtpqoCCeEFH2360626 = -742994554;    float loLuSgEXPQDkVtpqoCCeEFH18859264 = 77596136;    float loLuSgEXPQDkVtpqoCCeEFH74715724 = -626567096;    float loLuSgEXPQDkVtpqoCCeEFH60824516 = -485001026;    float loLuSgEXPQDkVtpqoCCeEFH19826512 = 50980913;     loLuSgEXPQDkVtpqoCCeEFH85631850 = loLuSgEXPQDkVtpqoCCeEFH60867303;     loLuSgEXPQDkVtpqoCCeEFH60867303 = loLuSgEXPQDkVtpqoCCeEFH89448349;     loLuSgEXPQDkVtpqoCCeEFH89448349 = loLuSgEXPQDkVtpqoCCeEFH93568990;     loLuSgEXPQDkVtpqoCCeEFH93568990 = loLuSgEXPQDkVtpqoCCeEFH77499635;     loLuSgEXPQDkVtpqoCCeEFH77499635 = loLuSgEXPQDkVtpqoCCeEFH56607845;     loLuSgEXPQDkVtpqoCCeEFH56607845 = loLuSgEXPQDkVtpqoCCeEFH4108636;     loLuSgEXPQDkVtpqoCCeEFH4108636 = loLuSgEXPQDkVtpqoCCeEFH46811229;     loLuSgEXPQDkVtpqoCCeEFH46811229 = loLuSgEXPQDkVtpqoCCeEFH76774239;     loLuSgEXPQDkVtpqoCCeEFH76774239 = loLuSgEXPQDkVtpqoCCeEFH88161130;     loLuSgEXPQDkVtpqoCCeEFH88161130 = loLuSgEXPQDkVtpqoCCeEFH67455132;     loLuSgEXPQDkVtpqoCCeEFH67455132 = loLuSgEXPQDkVtpqoCCeEFH48255425;     loLuSgEXPQDkVtpqoCCeEFH48255425 = loLuSgEXPQDkVtpqoCCeEFH6497783;     loLuSgEXPQDkVtpqoCCeEFH6497783 = loLuSgEXPQDkVtpqoCCeEFH9782629;     loLuSgEXPQDkVtpqoCCeEFH9782629 = loLuSgEXPQDkVtpqoCCeEFH85013012;     loLuSgEXPQDkVtpqoCCeEFH85013012 = loLuSgEXPQDkVtpqoCCeEFH34700301;     loLuSgEXPQDkVtpqoCCeEFH34700301 = loLuSgEXPQDkVtpqoCCeEFH97389593;     loLuSgEXPQDkVtpqoCCeEFH97389593 = loLuSgEXPQDkVtpqoCCeEFH730899;     loLuSgEXPQDkVtpqoCCeEFH730899 = loLuSgEXPQDkVtpqoCCeEFH13495187;     loLuSgEXPQDkVtpqoCCeEFH13495187 = loLuSgEXPQDkVtpqoCCeEFH54545378;     loLuSgEXPQDkVtpqoCCeEFH54545378 = loLuSgEXPQDkVtpqoCCeEFH64861553;     loLuSgEXPQDkVtpqoCCeEFH64861553 = loLuSgEXPQDkVtpqoCCeEFH89532676;     loLuSgEXPQDkVtpqoCCeEFH89532676 = loLuSgEXPQDkVtpqoCCeEFH16897343;     loLuSgEXPQDkVtpqoCCeEFH16897343 = loLuSgEXPQDkVtpqoCCeEFH95840337;     loLuSgEXPQDkVtpqoCCeEFH95840337 = loLuSgEXPQDkVtpqoCCeEFH29722300;     loLuSgEXPQDkVtpqoCCeEFH29722300 = loLuSgEXPQDkVtpqoCCeEFH93472708;     loLuSgEXPQDkVtpqoCCeEFH93472708 = loLuSgEXPQDkVtpqoCCeEFH26314551;     loLuSgEXPQDkVtpqoCCeEFH26314551 = loLuSgEXPQDkVtpqoCCeEFH91021555;     loLuSgEXPQDkVtpqoCCeEFH91021555 = loLuSgEXPQDkVtpqoCCeEFH40460987;     loLuSgEXPQDkVtpqoCCeEFH40460987 = loLuSgEXPQDkVtpqoCCeEFH8927440;     loLuSgEXPQDkVtpqoCCeEFH8927440 = loLuSgEXPQDkVtpqoCCeEFH96750594;     loLuSgEXPQDkVtpqoCCeEFH96750594 = loLuSgEXPQDkVtpqoCCeEFH32616636;     loLuSgEXPQDkVtpqoCCeEFH32616636 = loLuSgEXPQDkVtpqoCCeEFH60578631;     loLuSgEXPQDkVtpqoCCeEFH60578631 = loLuSgEXPQDkVtpqoCCeEFH91323180;     loLuSgEXPQDkVtpqoCCeEFH91323180 = loLuSgEXPQDkVtpqoCCeEFH45105418;     loLuSgEXPQDkVtpqoCCeEFH45105418 = loLuSgEXPQDkVtpqoCCeEFH98304327;     loLuSgEXPQDkVtpqoCCeEFH98304327 = loLuSgEXPQDkVtpqoCCeEFH54960807;     loLuSgEXPQDkVtpqoCCeEFH54960807 = loLuSgEXPQDkVtpqoCCeEFH84820567;     loLuSgEXPQDkVtpqoCCeEFH84820567 = loLuSgEXPQDkVtpqoCCeEFH13252205;     loLuSgEXPQDkVtpqoCCeEFH13252205 = loLuSgEXPQDkVtpqoCCeEFH38405270;     loLuSgEXPQDkVtpqoCCeEFH38405270 = loLuSgEXPQDkVtpqoCCeEFH15447705;     loLuSgEXPQDkVtpqoCCeEFH15447705 = loLuSgEXPQDkVtpqoCCeEFH25209505;     loLuSgEXPQDkVtpqoCCeEFH25209505 = loLuSgEXPQDkVtpqoCCeEFH42562524;     loLuSgEXPQDkVtpqoCCeEFH42562524 = loLuSgEXPQDkVtpqoCCeEFH52235052;     loLuSgEXPQDkVtpqoCCeEFH52235052 = loLuSgEXPQDkVtpqoCCeEFH54665008;     loLuSgEXPQDkVtpqoCCeEFH54665008 = loLuSgEXPQDkVtpqoCCeEFH83220513;     loLuSgEXPQDkVtpqoCCeEFH83220513 = loLuSgEXPQDkVtpqoCCeEFH27554245;     loLuSgEXPQDkVtpqoCCeEFH27554245 = loLuSgEXPQDkVtpqoCCeEFH63506018;     loLuSgEXPQDkVtpqoCCeEFH63506018 = loLuSgEXPQDkVtpqoCCeEFH27835434;     loLuSgEXPQDkVtpqoCCeEFH27835434 = loLuSgEXPQDkVtpqoCCeEFH95769819;     loLuSgEXPQDkVtpqoCCeEFH95769819 = loLuSgEXPQDkVtpqoCCeEFH55476738;     loLuSgEXPQDkVtpqoCCeEFH55476738 = loLuSgEXPQDkVtpqoCCeEFH2199089;     loLuSgEXPQDkVtpqoCCeEFH2199089 = loLuSgEXPQDkVtpqoCCeEFH17270375;     loLuSgEXPQDkVtpqoCCeEFH17270375 = loLuSgEXPQDkVtpqoCCeEFH13854193;     loLuSgEXPQDkVtpqoCCeEFH13854193 = loLuSgEXPQDkVtpqoCCeEFH48200658;     loLuSgEXPQDkVtpqoCCeEFH48200658 = loLuSgEXPQDkVtpqoCCeEFH96099174;     loLuSgEXPQDkVtpqoCCeEFH96099174 = loLuSgEXPQDkVtpqoCCeEFH43969961;     loLuSgEXPQDkVtpqoCCeEFH43969961 = loLuSgEXPQDkVtpqoCCeEFH93608011;     loLuSgEXPQDkVtpqoCCeEFH93608011 = loLuSgEXPQDkVtpqoCCeEFH63846690;     loLuSgEXPQDkVtpqoCCeEFH63846690 = loLuSgEXPQDkVtpqoCCeEFH84026926;     loLuSgEXPQDkVtpqoCCeEFH84026926 = loLuSgEXPQDkVtpqoCCeEFH30293294;     loLuSgEXPQDkVtpqoCCeEFH30293294 = loLuSgEXPQDkVtpqoCCeEFH13087080;     loLuSgEXPQDkVtpqoCCeEFH13087080 = loLuSgEXPQDkVtpqoCCeEFH6350242;     loLuSgEXPQDkVtpqoCCeEFH6350242 = loLuSgEXPQDkVtpqoCCeEFH67846800;     loLuSgEXPQDkVtpqoCCeEFH67846800 = loLuSgEXPQDkVtpqoCCeEFH91410535;     loLuSgEXPQDkVtpqoCCeEFH91410535 = loLuSgEXPQDkVtpqoCCeEFH34838496;     loLuSgEXPQDkVtpqoCCeEFH34838496 = loLuSgEXPQDkVtpqoCCeEFH87676793;     loLuSgEXPQDkVtpqoCCeEFH87676793 = loLuSgEXPQDkVtpqoCCeEFH15174603;     loLuSgEXPQDkVtpqoCCeEFH15174603 = loLuSgEXPQDkVtpqoCCeEFH64677210;     loLuSgEXPQDkVtpqoCCeEFH64677210 = loLuSgEXPQDkVtpqoCCeEFH86708684;     loLuSgEXPQDkVtpqoCCeEFH86708684 = loLuSgEXPQDkVtpqoCCeEFH79739494;     loLuSgEXPQDkVtpqoCCeEFH79739494 = loLuSgEXPQDkVtpqoCCeEFH12569026;     loLuSgEXPQDkVtpqoCCeEFH12569026 = loLuSgEXPQDkVtpqoCCeEFH87478693;     loLuSgEXPQDkVtpqoCCeEFH87478693 = loLuSgEXPQDkVtpqoCCeEFH75089916;     loLuSgEXPQDkVtpqoCCeEFH75089916 = loLuSgEXPQDkVtpqoCCeEFH39097673;     loLuSgEXPQDkVtpqoCCeEFH39097673 = loLuSgEXPQDkVtpqoCCeEFH39652048;     loLuSgEXPQDkVtpqoCCeEFH39652048 = loLuSgEXPQDkVtpqoCCeEFH46970152;     loLuSgEXPQDkVtpqoCCeEFH46970152 = loLuSgEXPQDkVtpqoCCeEFH64662290;     loLuSgEXPQDkVtpqoCCeEFH64662290 = loLuSgEXPQDkVtpqoCCeEFH41175329;     loLuSgEXPQDkVtpqoCCeEFH41175329 = loLuSgEXPQDkVtpqoCCeEFH46501787;     loLuSgEXPQDkVtpqoCCeEFH46501787 = loLuSgEXPQDkVtpqoCCeEFH65918464;     loLuSgEXPQDkVtpqoCCeEFH65918464 = loLuSgEXPQDkVtpqoCCeEFH62808533;     loLuSgEXPQDkVtpqoCCeEFH62808533 = loLuSgEXPQDkVtpqoCCeEFH63186122;     loLuSgEXPQDkVtpqoCCeEFH63186122 = loLuSgEXPQDkVtpqoCCeEFH44691168;     loLuSgEXPQDkVtpqoCCeEFH44691168 = loLuSgEXPQDkVtpqoCCeEFH53450701;     loLuSgEXPQDkVtpqoCCeEFH53450701 = loLuSgEXPQDkVtpqoCCeEFH94551505;     loLuSgEXPQDkVtpqoCCeEFH94551505 = loLuSgEXPQDkVtpqoCCeEFH15346262;     loLuSgEXPQDkVtpqoCCeEFH15346262 = loLuSgEXPQDkVtpqoCCeEFH46724439;     loLuSgEXPQDkVtpqoCCeEFH46724439 = loLuSgEXPQDkVtpqoCCeEFH43122522;     loLuSgEXPQDkVtpqoCCeEFH43122522 = loLuSgEXPQDkVtpqoCCeEFH49006244;     loLuSgEXPQDkVtpqoCCeEFH49006244 = loLuSgEXPQDkVtpqoCCeEFH54334367;     loLuSgEXPQDkVtpqoCCeEFH54334367 = loLuSgEXPQDkVtpqoCCeEFH61352795;     loLuSgEXPQDkVtpqoCCeEFH61352795 = loLuSgEXPQDkVtpqoCCeEFH20973877;     loLuSgEXPQDkVtpqoCCeEFH20973877 = loLuSgEXPQDkVtpqoCCeEFH29225278;     loLuSgEXPQDkVtpqoCCeEFH29225278 = loLuSgEXPQDkVtpqoCCeEFH8111977;     loLuSgEXPQDkVtpqoCCeEFH8111977 = loLuSgEXPQDkVtpqoCCeEFH2360626;     loLuSgEXPQDkVtpqoCCeEFH2360626 = loLuSgEXPQDkVtpqoCCeEFH18859264;     loLuSgEXPQDkVtpqoCCeEFH18859264 = loLuSgEXPQDkVtpqoCCeEFH74715724;     loLuSgEXPQDkVtpqoCCeEFH74715724 = loLuSgEXPQDkVtpqoCCeEFH60824516;     loLuSgEXPQDkVtpqoCCeEFH60824516 = loLuSgEXPQDkVtpqoCCeEFH19826512;     loLuSgEXPQDkVtpqoCCeEFH19826512 = loLuSgEXPQDkVtpqoCCeEFH85631850;}



void YsUJPolWKdRZmWnntFwWTWemVC98125048() {     float oaFPeMWoVzbNXHQyVcInPZi69355368 = -299041154;    float oaFPeMWoVzbNXHQyVcInPZi58465399 = -667475787;    float oaFPeMWoVzbNXHQyVcInPZi78034466 = -881190805;    float oaFPeMWoVzbNXHQyVcInPZi90716943 = -552487688;    float oaFPeMWoVzbNXHQyVcInPZi57060972 = -121614618;    float oaFPeMWoVzbNXHQyVcInPZi67947362 = -631335831;    float oaFPeMWoVzbNXHQyVcInPZi40989198 = -859589794;    float oaFPeMWoVzbNXHQyVcInPZi90151311 = -56469548;    float oaFPeMWoVzbNXHQyVcInPZi26993768 = -875160977;    float oaFPeMWoVzbNXHQyVcInPZi34266719 = -29006313;    float oaFPeMWoVzbNXHQyVcInPZi10344168 = -299866012;    float oaFPeMWoVzbNXHQyVcInPZi38467196 = -56754897;    float oaFPeMWoVzbNXHQyVcInPZi57551634 = -266072738;    float oaFPeMWoVzbNXHQyVcInPZi4155774 = -58888924;    float oaFPeMWoVzbNXHQyVcInPZi89192270 = -726504214;    float oaFPeMWoVzbNXHQyVcInPZi6514330 = -181795283;    float oaFPeMWoVzbNXHQyVcInPZi97255004 = -609969337;    float oaFPeMWoVzbNXHQyVcInPZi14808168 = 67398239;    float oaFPeMWoVzbNXHQyVcInPZi54471667 = -630292149;    float oaFPeMWoVzbNXHQyVcInPZi28653022 = -650843589;    float oaFPeMWoVzbNXHQyVcInPZi32201064 = -360051942;    float oaFPeMWoVzbNXHQyVcInPZi90534747 = -462679383;    float oaFPeMWoVzbNXHQyVcInPZi86073207 = -236821856;    float oaFPeMWoVzbNXHQyVcInPZi96983032 = -366852017;    float oaFPeMWoVzbNXHQyVcInPZi66324392 = -722213268;    float oaFPeMWoVzbNXHQyVcInPZi46465466 = -140713332;    float oaFPeMWoVzbNXHQyVcInPZi47906491 = -436328786;    float oaFPeMWoVzbNXHQyVcInPZi33620756 = -145653918;    float oaFPeMWoVzbNXHQyVcInPZi12379129 = -787438564;    float oaFPeMWoVzbNXHQyVcInPZi44475767 = -168498858;    float oaFPeMWoVzbNXHQyVcInPZi78115017 = -990377387;    float oaFPeMWoVzbNXHQyVcInPZi57409924 = -754398519;    float oaFPeMWoVzbNXHQyVcInPZi74703333 = -522998581;    float oaFPeMWoVzbNXHQyVcInPZi27047894 = -69259655;    float oaFPeMWoVzbNXHQyVcInPZi12708744 = -173751869;    float oaFPeMWoVzbNXHQyVcInPZi68165561 = -389469440;    float oaFPeMWoVzbNXHQyVcInPZi54724397 = -616564322;    float oaFPeMWoVzbNXHQyVcInPZi35202359 = -656490114;    float oaFPeMWoVzbNXHQyVcInPZi73867924 = -990289612;    float oaFPeMWoVzbNXHQyVcInPZi77594386 = -830674009;    float oaFPeMWoVzbNXHQyVcInPZi80215225 = -871897907;    float oaFPeMWoVzbNXHQyVcInPZi54549232 = -901201831;    float oaFPeMWoVzbNXHQyVcInPZi75362670 = -960757147;    float oaFPeMWoVzbNXHQyVcInPZi99430571 = -198630689;    float oaFPeMWoVzbNXHQyVcInPZi82652258 = -227364645;    float oaFPeMWoVzbNXHQyVcInPZi27487395 = -889738032;    float oaFPeMWoVzbNXHQyVcInPZi60733214 = -923524669;    float oaFPeMWoVzbNXHQyVcInPZi2247866 = -722203189;    float oaFPeMWoVzbNXHQyVcInPZi61709647 = -577499186;    float oaFPeMWoVzbNXHQyVcInPZi31643150 = -85116060;    float oaFPeMWoVzbNXHQyVcInPZi95567570 = -588716046;    float oaFPeMWoVzbNXHQyVcInPZi53704808 = -122850703;    float oaFPeMWoVzbNXHQyVcInPZi62652777 = -348416488;    float oaFPeMWoVzbNXHQyVcInPZi59786798 = -873088835;    float oaFPeMWoVzbNXHQyVcInPZi65923985 = -213049895;    float oaFPeMWoVzbNXHQyVcInPZi78820621 = -836361772;    float oaFPeMWoVzbNXHQyVcInPZi72392191 = -330653931;    float oaFPeMWoVzbNXHQyVcInPZi81051434 = -414338788;    float oaFPeMWoVzbNXHQyVcInPZi24392551 = -830274420;    float oaFPeMWoVzbNXHQyVcInPZi10595506 = -980901286;    float oaFPeMWoVzbNXHQyVcInPZi20040872 = -95007046;    float oaFPeMWoVzbNXHQyVcInPZi7368442 = -613935876;    float oaFPeMWoVzbNXHQyVcInPZi77772182 = -269030984;    float oaFPeMWoVzbNXHQyVcInPZi82518001 = -606662119;    float oaFPeMWoVzbNXHQyVcInPZi56151701 = -38628926;    float oaFPeMWoVzbNXHQyVcInPZi52934244 = -545467493;    float oaFPeMWoVzbNXHQyVcInPZi63763863 = -533756316;    float oaFPeMWoVzbNXHQyVcInPZi30503740 = -96813083;    float oaFPeMWoVzbNXHQyVcInPZi91447029 = -885137056;    float oaFPeMWoVzbNXHQyVcInPZi21026710 = -237034774;    float oaFPeMWoVzbNXHQyVcInPZi51789933 = -565230962;    float oaFPeMWoVzbNXHQyVcInPZi62052646 = -953479223;    float oaFPeMWoVzbNXHQyVcInPZi40940244 = 57687850;    float oaFPeMWoVzbNXHQyVcInPZi76877281 = -799618140;    float oaFPeMWoVzbNXHQyVcInPZi48437796 = -778945683;    float oaFPeMWoVzbNXHQyVcInPZi77651832 = -458850111;    float oaFPeMWoVzbNXHQyVcInPZi15172077 = -501922236;    float oaFPeMWoVzbNXHQyVcInPZi86642636 = 61808832;    float oaFPeMWoVzbNXHQyVcInPZi14330774 = -39487373;    float oaFPeMWoVzbNXHQyVcInPZi38836998 = -832475236;    float oaFPeMWoVzbNXHQyVcInPZi85732252 = -217188664;    float oaFPeMWoVzbNXHQyVcInPZi45658625 = -714125598;    float oaFPeMWoVzbNXHQyVcInPZi71911109 = -568154733;    float oaFPeMWoVzbNXHQyVcInPZi80735979 = -602322504;    float oaFPeMWoVzbNXHQyVcInPZi48908196 = -579782813;    float oaFPeMWoVzbNXHQyVcInPZi24410209 = -767526685;    float oaFPeMWoVzbNXHQyVcInPZi94757147 = -305982032;    float oaFPeMWoVzbNXHQyVcInPZi14916535 = -649909747;    float oaFPeMWoVzbNXHQyVcInPZi61123909 = -856209761;    float oaFPeMWoVzbNXHQyVcInPZi33888122 = -337390097;    float oaFPeMWoVzbNXHQyVcInPZi95773370 = 41184491;    float oaFPeMWoVzbNXHQyVcInPZi73672963 = -102225534;    float oaFPeMWoVzbNXHQyVcInPZi10809808 = -826215694;    float oaFPeMWoVzbNXHQyVcInPZi63272418 = 90611674;    float oaFPeMWoVzbNXHQyVcInPZi57553514 = -635666964;    float oaFPeMWoVzbNXHQyVcInPZi72846783 = -157962031;    float oaFPeMWoVzbNXHQyVcInPZi76777049 = -532170847;    float oaFPeMWoVzbNXHQyVcInPZi92844669 = -254095029;    float oaFPeMWoVzbNXHQyVcInPZi43278871 = -60001764;    float oaFPeMWoVzbNXHQyVcInPZi29718015 = -299041154;     oaFPeMWoVzbNXHQyVcInPZi69355368 = oaFPeMWoVzbNXHQyVcInPZi58465399;     oaFPeMWoVzbNXHQyVcInPZi58465399 = oaFPeMWoVzbNXHQyVcInPZi78034466;     oaFPeMWoVzbNXHQyVcInPZi78034466 = oaFPeMWoVzbNXHQyVcInPZi90716943;     oaFPeMWoVzbNXHQyVcInPZi90716943 = oaFPeMWoVzbNXHQyVcInPZi57060972;     oaFPeMWoVzbNXHQyVcInPZi57060972 = oaFPeMWoVzbNXHQyVcInPZi67947362;     oaFPeMWoVzbNXHQyVcInPZi67947362 = oaFPeMWoVzbNXHQyVcInPZi40989198;     oaFPeMWoVzbNXHQyVcInPZi40989198 = oaFPeMWoVzbNXHQyVcInPZi90151311;     oaFPeMWoVzbNXHQyVcInPZi90151311 = oaFPeMWoVzbNXHQyVcInPZi26993768;     oaFPeMWoVzbNXHQyVcInPZi26993768 = oaFPeMWoVzbNXHQyVcInPZi34266719;     oaFPeMWoVzbNXHQyVcInPZi34266719 = oaFPeMWoVzbNXHQyVcInPZi10344168;     oaFPeMWoVzbNXHQyVcInPZi10344168 = oaFPeMWoVzbNXHQyVcInPZi38467196;     oaFPeMWoVzbNXHQyVcInPZi38467196 = oaFPeMWoVzbNXHQyVcInPZi57551634;     oaFPeMWoVzbNXHQyVcInPZi57551634 = oaFPeMWoVzbNXHQyVcInPZi4155774;     oaFPeMWoVzbNXHQyVcInPZi4155774 = oaFPeMWoVzbNXHQyVcInPZi89192270;     oaFPeMWoVzbNXHQyVcInPZi89192270 = oaFPeMWoVzbNXHQyVcInPZi6514330;     oaFPeMWoVzbNXHQyVcInPZi6514330 = oaFPeMWoVzbNXHQyVcInPZi97255004;     oaFPeMWoVzbNXHQyVcInPZi97255004 = oaFPeMWoVzbNXHQyVcInPZi14808168;     oaFPeMWoVzbNXHQyVcInPZi14808168 = oaFPeMWoVzbNXHQyVcInPZi54471667;     oaFPeMWoVzbNXHQyVcInPZi54471667 = oaFPeMWoVzbNXHQyVcInPZi28653022;     oaFPeMWoVzbNXHQyVcInPZi28653022 = oaFPeMWoVzbNXHQyVcInPZi32201064;     oaFPeMWoVzbNXHQyVcInPZi32201064 = oaFPeMWoVzbNXHQyVcInPZi90534747;     oaFPeMWoVzbNXHQyVcInPZi90534747 = oaFPeMWoVzbNXHQyVcInPZi86073207;     oaFPeMWoVzbNXHQyVcInPZi86073207 = oaFPeMWoVzbNXHQyVcInPZi96983032;     oaFPeMWoVzbNXHQyVcInPZi96983032 = oaFPeMWoVzbNXHQyVcInPZi66324392;     oaFPeMWoVzbNXHQyVcInPZi66324392 = oaFPeMWoVzbNXHQyVcInPZi46465466;     oaFPeMWoVzbNXHQyVcInPZi46465466 = oaFPeMWoVzbNXHQyVcInPZi47906491;     oaFPeMWoVzbNXHQyVcInPZi47906491 = oaFPeMWoVzbNXHQyVcInPZi33620756;     oaFPeMWoVzbNXHQyVcInPZi33620756 = oaFPeMWoVzbNXHQyVcInPZi12379129;     oaFPeMWoVzbNXHQyVcInPZi12379129 = oaFPeMWoVzbNXHQyVcInPZi44475767;     oaFPeMWoVzbNXHQyVcInPZi44475767 = oaFPeMWoVzbNXHQyVcInPZi78115017;     oaFPeMWoVzbNXHQyVcInPZi78115017 = oaFPeMWoVzbNXHQyVcInPZi57409924;     oaFPeMWoVzbNXHQyVcInPZi57409924 = oaFPeMWoVzbNXHQyVcInPZi74703333;     oaFPeMWoVzbNXHQyVcInPZi74703333 = oaFPeMWoVzbNXHQyVcInPZi27047894;     oaFPeMWoVzbNXHQyVcInPZi27047894 = oaFPeMWoVzbNXHQyVcInPZi12708744;     oaFPeMWoVzbNXHQyVcInPZi12708744 = oaFPeMWoVzbNXHQyVcInPZi68165561;     oaFPeMWoVzbNXHQyVcInPZi68165561 = oaFPeMWoVzbNXHQyVcInPZi54724397;     oaFPeMWoVzbNXHQyVcInPZi54724397 = oaFPeMWoVzbNXHQyVcInPZi35202359;     oaFPeMWoVzbNXHQyVcInPZi35202359 = oaFPeMWoVzbNXHQyVcInPZi73867924;     oaFPeMWoVzbNXHQyVcInPZi73867924 = oaFPeMWoVzbNXHQyVcInPZi77594386;     oaFPeMWoVzbNXHQyVcInPZi77594386 = oaFPeMWoVzbNXHQyVcInPZi80215225;     oaFPeMWoVzbNXHQyVcInPZi80215225 = oaFPeMWoVzbNXHQyVcInPZi54549232;     oaFPeMWoVzbNXHQyVcInPZi54549232 = oaFPeMWoVzbNXHQyVcInPZi75362670;     oaFPeMWoVzbNXHQyVcInPZi75362670 = oaFPeMWoVzbNXHQyVcInPZi99430571;     oaFPeMWoVzbNXHQyVcInPZi99430571 = oaFPeMWoVzbNXHQyVcInPZi82652258;     oaFPeMWoVzbNXHQyVcInPZi82652258 = oaFPeMWoVzbNXHQyVcInPZi27487395;     oaFPeMWoVzbNXHQyVcInPZi27487395 = oaFPeMWoVzbNXHQyVcInPZi60733214;     oaFPeMWoVzbNXHQyVcInPZi60733214 = oaFPeMWoVzbNXHQyVcInPZi2247866;     oaFPeMWoVzbNXHQyVcInPZi2247866 = oaFPeMWoVzbNXHQyVcInPZi61709647;     oaFPeMWoVzbNXHQyVcInPZi61709647 = oaFPeMWoVzbNXHQyVcInPZi31643150;     oaFPeMWoVzbNXHQyVcInPZi31643150 = oaFPeMWoVzbNXHQyVcInPZi95567570;     oaFPeMWoVzbNXHQyVcInPZi95567570 = oaFPeMWoVzbNXHQyVcInPZi53704808;     oaFPeMWoVzbNXHQyVcInPZi53704808 = oaFPeMWoVzbNXHQyVcInPZi62652777;     oaFPeMWoVzbNXHQyVcInPZi62652777 = oaFPeMWoVzbNXHQyVcInPZi59786798;     oaFPeMWoVzbNXHQyVcInPZi59786798 = oaFPeMWoVzbNXHQyVcInPZi65923985;     oaFPeMWoVzbNXHQyVcInPZi65923985 = oaFPeMWoVzbNXHQyVcInPZi78820621;     oaFPeMWoVzbNXHQyVcInPZi78820621 = oaFPeMWoVzbNXHQyVcInPZi72392191;     oaFPeMWoVzbNXHQyVcInPZi72392191 = oaFPeMWoVzbNXHQyVcInPZi81051434;     oaFPeMWoVzbNXHQyVcInPZi81051434 = oaFPeMWoVzbNXHQyVcInPZi24392551;     oaFPeMWoVzbNXHQyVcInPZi24392551 = oaFPeMWoVzbNXHQyVcInPZi10595506;     oaFPeMWoVzbNXHQyVcInPZi10595506 = oaFPeMWoVzbNXHQyVcInPZi20040872;     oaFPeMWoVzbNXHQyVcInPZi20040872 = oaFPeMWoVzbNXHQyVcInPZi7368442;     oaFPeMWoVzbNXHQyVcInPZi7368442 = oaFPeMWoVzbNXHQyVcInPZi77772182;     oaFPeMWoVzbNXHQyVcInPZi77772182 = oaFPeMWoVzbNXHQyVcInPZi82518001;     oaFPeMWoVzbNXHQyVcInPZi82518001 = oaFPeMWoVzbNXHQyVcInPZi56151701;     oaFPeMWoVzbNXHQyVcInPZi56151701 = oaFPeMWoVzbNXHQyVcInPZi52934244;     oaFPeMWoVzbNXHQyVcInPZi52934244 = oaFPeMWoVzbNXHQyVcInPZi63763863;     oaFPeMWoVzbNXHQyVcInPZi63763863 = oaFPeMWoVzbNXHQyVcInPZi30503740;     oaFPeMWoVzbNXHQyVcInPZi30503740 = oaFPeMWoVzbNXHQyVcInPZi91447029;     oaFPeMWoVzbNXHQyVcInPZi91447029 = oaFPeMWoVzbNXHQyVcInPZi21026710;     oaFPeMWoVzbNXHQyVcInPZi21026710 = oaFPeMWoVzbNXHQyVcInPZi51789933;     oaFPeMWoVzbNXHQyVcInPZi51789933 = oaFPeMWoVzbNXHQyVcInPZi62052646;     oaFPeMWoVzbNXHQyVcInPZi62052646 = oaFPeMWoVzbNXHQyVcInPZi40940244;     oaFPeMWoVzbNXHQyVcInPZi40940244 = oaFPeMWoVzbNXHQyVcInPZi76877281;     oaFPeMWoVzbNXHQyVcInPZi76877281 = oaFPeMWoVzbNXHQyVcInPZi48437796;     oaFPeMWoVzbNXHQyVcInPZi48437796 = oaFPeMWoVzbNXHQyVcInPZi77651832;     oaFPeMWoVzbNXHQyVcInPZi77651832 = oaFPeMWoVzbNXHQyVcInPZi15172077;     oaFPeMWoVzbNXHQyVcInPZi15172077 = oaFPeMWoVzbNXHQyVcInPZi86642636;     oaFPeMWoVzbNXHQyVcInPZi86642636 = oaFPeMWoVzbNXHQyVcInPZi14330774;     oaFPeMWoVzbNXHQyVcInPZi14330774 = oaFPeMWoVzbNXHQyVcInPZi38836998;     oaFPeMWoVzbNXHQyVcInPZi38836998 = oaFPeMWoVzbNXHQyVcInPZi85732252;     oaFPeMWoVzbNXHQyVcInPZi85732252 = oaFPeMWoVzbNXHQyVcInPZi45658625;     oaFPeMWoVzbNXHQyVcInPZi45658625 = oaFPeMWoVzbNXHQyVcInPZi71911109;     oaFPeMWoVzbNXHQyVcInPZi71911109 = oaFPeMWoVzbNXHQyVcInPZi80735979;     oaFPeMWoVzbNXHQyVcInPZi80735979 = oaFPeMWoVzbNXHQyVcInPZi48908196;     oaFPeMWoVzbNXHQyVcInPZi48908196 = oaFPeMWoVzbNXHQyVcInPZi24410209;     oaFPeMWoVzbNXHQyVcInPZi24410209 = oaFPeMWoVzbNXHQyVcInPZi94757147;     oaFPeMWoVzbNXHQyVcInPZi94757147 = oaFPeMWoVzbNXHQyVcInPZi14916535;     oaFPeMWoVzbNXHQyVcInPZi14916535 = oaFPeMWoVzbNXHQyVcInPZi61123909;     oaFPeMWoVzbNXHQyVcInPZi61123909 = oaFPeMWoVzbNXHQyVcInPZi33888122;     oaFPeMWoVzbNXHQyVcInPZi33888122 = oaFPeMWoVzbNXHQyVcInPZi95773370;     oaFPeMWoVzbNXHQyVcInPZi95773370 = oaFPeMWoVzbNXHQyVcInPZi73672963;     oaFPeMWoVzbNXHQyVcInPZi73672963 = oaFPeMWoVzbNXHQyVcInPZi10809808;     oaFPeMWoVzbNXHQyVcInPZi10809808 = oaFPeMWoVzbNXHQyVcInPZi63272418;     oaFPeMWoVzbNXHQyVcInPZi63272418 = oaFPeMWoVzbNXHQyVcInPZi57553514;     oaFPeMWoVzbNXHQyVcInPZi57553514 = oaFPeMWoVzbNXHQyVcInPZi72846783;     oaFPeMWoVzbNXHQyVcInPZi72846783 = oaFPeMWoVzbNXHQyVcInPZi76777049;     oaFPeMWoVzbNXHQyVcInPZi76777049 = oaFPeMWoVzbNXHQyVcInPZi92844669;     oaFPeMWoVzbNXHQyVcInPZi92844669 = oaFPeMWoVzbNXHQyVcInPZi43278871;     oaFPeMWoVzbNXHQyVcInPZi43278871 = oaFPeMWoVzbNXHQyVcInPZi29718015;     oaFPeMWoVzbNXHQyVcInPZi29718015 = oaFPeMWoVzbNXHQyVcInPZi69355368;}



void NKNzksWuuvtgyFlGDGAJQikwTd83187886() {     float jgvvnbQuXQZLdKeShRcrdxX53078887 = -649063221;    float jgvvnbQuXQZLdKeShRcrdxX56063494 = -471127639;    float jgvvnbQuXQZLdKeShRcrdxX66620583 = -749498496;    float jgvvnbQuXQZLdKeShRcrdxX87864896 = -557369617;    float jgvvnbQuXQZLdKeShRcrdxX36622308 = -519562809;    float jgvvnbQuXQZLdKeShRcrdxX79286880 = -734656500;    float jgvvnbQuXQZLdKeShRcrdxX77869761 = -881103932;    float jgvvnbQuXQZLdKeShRcrdxX33491395 = -676924134;    float jgvvnbQuXQZLdKeShRcrdxX77213296 = -472643673;    float jgvvnbQuXQZLdKeShRcrdxX80372306 = -252047329;    float jgvvnbQuXQZLdKeShRcrdxX53233204 = -140151270;    float jgvvnbQuXQZLdKeShRcrdxX28678967 = 14105832;    float jgvvnbQuXQZLdKeShRcrdxX8605487 = -108904981;    float jgvvnbQuXQZLdKeShRcrdxX98528917 = -408571786;    float jgvvnbQuXQZLdKeShRcrdxX93371529 = -652201519;    float jgvvnbQuXQZLdKeShRcrdxX78328358 = -105922935;    float jgvvnbQuXQZLdKeShRcrdxX97120414 = -768849200;    float jgvvnbQuXQZLdKeShRcrdxX28885438 = -643453335;    float jgvvnbQuXQZLdKeShRcrdxX95448148 = -418941387;    float jgvvnbQuXQZLdKeShRcrdxX2760667 = -267136506;    float jgvvnbQuXQZLdKeShRcrdxX99540573 = 65481537;    float jgvvnbQuXQZLdKeShRcrdxX91536818 = -375834669;    float jgvvnbQuXQZLdKeShRcrdxX55249073 = -266216593;    float jgvvnbQuXQZLdKeShRcrdxX98125727 = 93915329;    float jgvvnbQuXQZLdKeShRcrdxX2926486 = -800544030;    float jgvvnbQuXQZLdKeShRcrdxX99458224 = -583680360;    float jgvvnbQuXQZLdKeShRcrdxX69498430 = -880204209;    float jgvvnbQuXQZLdKeShRcrdxX76219956 = 67904593;    float jgvvnbQuXQZLdKeShRcrdxX84297270 = -211634464;    float jgvvnbQuXQZLdKeShRcrdxX80024093 = -841105233;    float jgvvnbQuXQZLdKeShRcrdxX59479440 = -109475408;    float jgvvnbQuXQZLdKeShRcrdxX82203212 = -203595742;    float jgvvnbQuXQZLdKeShRcrdxX88828034 = -88212673;    float jgvvnbQuXQZLdKeShRcrdxX62772608 = -473356303;    float jgvvnbQuXQZLdKeShRcrdxX80312068 = 91957523;    float jgvvnbQuXQZLdKeShRcrdxX38026795 = -367865917;    float jgvvnbQuXQZLdKeShRcrdxX54487987 = -936905618;    float jgvvnbQuXQZLdKeShRcrdxX85584149 = -555633331;    float jgvvnbQuXQZLdKeShRcrdxX34483643 = -195848493;    float jgvvnbQuXQZLdKeShRcrdxX16783502 = -777320889;    float jgvvnbQuXQZLdKeShRcrdxX44982746 = -521938033;    float jgvvnbQuXQZLdKeShRcrdxX83888958 = -507227500;    float jgvvnbQuXQZLdKeShRcrdxX8162817 = -613161401;    float jgvvnbQuXQZLdKeShRcrdxX46626091 = -877574422;    float jgvvnbQuXQZLdKeShRcrdxX10639510 = -553378723;    float jgvvnbQuXQZLdKeShRcrdxX71754276 = -504769724;    float jgvvnbQuXQZLdKeShRcrdxX93912183 = -945170576;    float jgvvnbQuXQZLdKeShRcrdxX40989713 = -484325087;    float jgvvnbQuXQZLdKeShRcrdxX95583860 = -277283337;    float jgvvnbQuXQZLdKeShRcrdxX67516479 = -717131496;    float jgvvnbQuXQZLdKeShRcrdxX35658402 = -754488447;    float jgvvnbQuXQZLdKeShRcrdxX5210528 = -974903489;    float jgvvnbQuXQZLdKeShRcrdxX8035180 = -360348853;    float jgvvnbQuXQZLdKeShRcrdxX5719405 = -349798023;    float jgvvnbQuXQZLdKeShRcrdxX83647312 = -529776840;    float jgvvnbQuXQZLdKeShRcrdxX61542068 = -173228553;    float jgvvnbQuXQZLdKeShRcrdxX814422 = -104911046;    float jgvvnbQuXQZLdKeShRcrdxX68494856 = -743413826;    float jgvvnbQuXQZLdKeShRcrdxX84938410 = -756825588;    float jgvvnbQuXQZLdKeShRcrdxX37164084 = -935882450;    float jgvvnbQuXQZLdKeShRcrdxX9788451 = -854452292;    float jgvvnbQuXQZLdKeShRcrdxX1649805 = -849008526;    float jgvvnbQuXQZLdKeShRcrdxX49194124 = -365289670;    float jgvvnbQuXQZLdKeShRcrdxX97189202 = -631538441;    float jgvvnbQuXQZLdKeShRcrdxX20892867 = -42571921;    float jgvvnbQuXQZLdKeShRcrdxX71029991 = -936555529;    float jgvvnbQuXQZLdKeShRcrdxX39850933 = -897681495;    float jgvvnbQuXQZLdKeShRcrdxX45832878 = -635548679;    float jgvvnbQuXQZLdKeShRcrdxX18216849 = -400529310;    float jgvvnbQuXQZLdKeShRcrdxX55344734 = -184335602;    float jgvvnbQuXQZLdKeShRcrdxX23840372 = -169017317;    float jgvvnbQuXQZLdKeShRcrdxX11536266 = -113215869;    float jgvvnbQuXQZLdKeShRcrdxX94401794 = -347604843;    float jgvvnbQuXQZLdKeShRcrdxX78664646 = -641620499;    float jgvvnbQuXQZLdKeShRcrdxX57777920 = -745198473;    float jgvvnbQuXQZLdKeShRcrdxX15651616 = -427290963;    float jgvvnbQuXQZLdKeShRcrdxX83374001 = -762673268;    float jgvvnbQuXQZLdKeShRcrdxX8622982 = -388642172;    float jgvvnbQuXQZLdKeShRcrdxX87486217 = -352705948;    float jgvvnbQuXQZLdKeShRcrdxX31172210 = -195774306;    float jgvvnbQuXQZLdKeShRcrdxX5546041 = -638509784;    float jgvvnbQuXQZLdKeShRcrdxX28508717 = -295879123;    float jgvvnbQuXQZLdKeShRcrdxX80636095 = -654812070;    float jgvvnbQuXQZLdKeShRcrdxX16780791 = -494502969;    float jgvvnbQuXQZLdKeShRcrdxX44365692 = 13383214;    float jgvvnbQuXQZLdKeShRcrdxX54268912 = -134571920;    float jgvvnbQuXQZLdKeShRcrdxX74168033 = -843246889;    float jgvvnbQuXQZLdKeShRcrdxX83108629 = -738414650;    float jgvvnbQuXQZLdKeShRcrdxX79125295 = -943579464;    float jgvvnbQuXQZLdKeShRcrdxX18770001 = -734813924;    float jgvvnbQuXQZLdKeShRcrdxX37212374 = -162954872;    float jgvvnbQuXQZLdKeShRcrdxX85993131 = -93491793;    float jgvvnbQuXQZLdKeShRcrdxX645739 = -798807744;    float jgvvnbQuXQZLdKeShRcrdxX97319558 = -259966043;    float jgvvnbQuXQZLdKeShRcrdxX6995052 = -922868598;    float jgvvnbQuXQZLdKeShRcrdxX43332941 = -672929508;    float jgvvnbQuXQZLdKeShRcrdxX34694834 = -41937831;    float jgvvnbQuXQZLdKeShRcrdxX10973615 = -981622961;    float jgvvnbQuXQZLdKeShRcrdxX25733225 = -735002501;    float jgvvnbQuXQZLdKeShRcrdxX39609518 = -649063221;     jgvvnbQuXQZLdKeShRcrdxX53078887 = jgvvnbQuXQZLdKeShRcrdxX56063494;     jgvvnbQuXQZLdKeShRcrdxX56063494 = jgvvnbQuXQZLdKeShRcrdxX66620583;     jgvvnbQuXQZLdKeShRcrdxX66620583 = jgvvnbQuXQZLdKeShRcrdxX87864896;     jgvvnbQuXQZLdKeShRcrdxX87864896 = jgvvnbQuXQZLdKeShRcrdxX36622308;     jgvvnbQuXQZLdKeShRcrdxX36622308 = jgvvnbQuXQZLdKeShRcrdxX79286880;     jgvvnbQuXQZLdKeShRcrdxX79286880 = jgvvnbQuXQZLdKeShRcrdxX77869761;     jgvvnbQuXQZLdKeShRcrdxX77869761 = jgvvnbQuXQZLdKeShRcrdxX33491395;     jgvvnbQuXQZLdKeShRcrdxX33491395 = jgvvnbQuXQZLdKeShRcrdxX77213296;     jgvvnbQuXQZLdKeShRcrdxX77213296 = jgvvnbQuXQZLdKeShRcrdxX80372306;     jgvvnbQuXQZLdKeShRcrdxX80372306 = jgvvnbQuXQZLdKeShRcrdxX53233204;     jgvvnbQuXQZLdKeShRcrdxX53233204 = jgvvnbQuXQZLdKeShRcrdxX28678967;     jgvvnbQuXQZLdKeShRcrdxX28678967 = jgvvnbQuXQZLdKeShRcrdxX8605487;     jgvvnbQuXQZLdKeShRcrdxX8605487 = jgvvnbQuXQZLdKeShRcrdxX98528917;     jgvvnbQuXQZLdKeShRcrdxX98528917 = jgvvnbQuXQZLdKeShRcrdxX93371529;     jgvvnbQuXQZLdKeShRcrdxX93371529 = jgvvnbQuXQZLdKeShRcrdxX78328358;     jgvvnbQuXQZLdKeShRcrdxX78328358 = jgvvnbQuXQZLdKeShRcrdxX97120414;     jgvvnbQuXQZLdKeShRcrdxX97120414 = jgvvnbQuXQZLdKeShRcrdxX28885438;     jgvvnbQuXQZLdKeShRcrdxX28885438 = jgvvnbQuXQZLdKeShRcrdxX95448148;     jgvvnbQuXQZLdKeShRcrdxX95448148 = jgvvnbQuXQZLdKeShRcrdxX2760667;     jgvvnbQuXQZLdKeShRcrdxX2760667 = jgvvnbQuXQZLdKeShRcrdxX99540573;     jgvvnbQuXQZLdKeShRcrdxX99540573 = jgvvnbQuXQZLdKeShRcrdxX91536818;     jgvvnbQuXQZLdKeShRcrdxX91536818 = jgvvnbQuXQZLdKeShRcrdxX55249073;     jgvvnbQuXQZLdKeShRcrdxX55249073 = jgvvnbQuXQZLdKeShRcrdxX98125727;     jgvvnbQuXQZLdKeShRcrdxX98125727 = jgvvnbQuXQZLdKeShRcrdxX2926486;     jgvvnbQuXQZLdKeShRcrdxX2926486 = jgvvnbQuXQZLdKeShRcrdxX99458224;     jgvvnbQuXQZLdKeShRcrdxX99458224 = jgvvnbQuXQZLdKeShRcrdxX69498430;     jgvvnbQuXQZLdKeShRcrdxX69498430 = jgvvnbQuXQZLdKeShRcrdxX76219956;     jgvvnbQuXQZLdKeShRcrdxX76219956 = jgvvnbQuXQZLdKeShRcrdxX84297270;     jgvvnbQuXQZLdKeShRcrdxX84297270 = jgvvnbQuXQZLdKeShRcrdxX80024093;     jgvvnbQuXQZLdKeShRcrdxX80024093 = jgvvnbQuXQZLdKeShRcrdxX59479440;     jgvvnbQuXQZLdKeShRcrdxX59479440 = jgvvnbQuXQZLdKeShRcrdxX82203212;     jgvvnbQuXQZLdKeShRcrdxX82203212 = jgvvnbQuXQZLdKeShRcrdxX88828034;     jgvvnbQuXQZLdKeShRcrdxX88828034 = jgvvnbQuXQZLdKeShRcrdxX62772608;     jgvvnbQuXQZLdKeShRcrdxX62772608 = jgvvnbQuXQZLdKeShRcrdxX80312068;     jgvvnbQuXQZLdKeShRcrdxX80312068 = jgvvnbQuXQZLdKeShRcrdxX38026795;     jgvvnbQuXQZLdKeShRcrdxX38026795 = jgvvnbQuXQZLdKeShRcrdxX54487987;     jgvvnbQuXQZLdKeShRcrdxX54487987 = jgvvnbQuXQZLdKeShRcrdxX85584149;     jgvvnbQuXQZLdKeShRcrdxX85584149 = jgvvnbQuXQZLdKeShRcrdxX34483643;     jgvvnbQuXQZLdKeShRcrdxX34483643 = jgvvnbQuXQZLdKeShRcrdxX16783502;     jgvvnbQuXQZLdKeShRcrdxX16783502 = jgvvnbQuXQZLdKeShRcrdxX44982746;     jgvvnbQuXQZLdKeShRcrdxX44982746 = jgvvnbQuXQZLdKeShRcrdxX83888958;     jgvvnbQuXQZLdKeShRcrdxX83888958 = jgvvnbQuXQZLdKeShRcrdxX8162817;     jgvvnbQuXQZLdKeShRcrdxX8162817 = jgvvnbQuXQZLdKeShRcrdxX46626091;     jgvvnbQuXQZLdKeShRcrdxX46626091 = jgvvnbQuXQZLdKeShRcrdxX10639510;     jgvvnbQuXQZLdKeShRcrdxX10639510 = jgvvnbQuXQZLdKeShRcrdxX71754276;     jgvvnbQuXQZLdKeShRcrdxX71754276 = jgvvnbQuXQZLdKeShRcrdxX93912183;     jgvvnbQuXQZLdKeShRcrdxX93912183 = jgvvnbQuXQZLdKeShRcrdxX40989713;     jgvvnbQuXQZLdKeShRcrdxX40989713 = jgvvnbQuXQZLdKeShRcrdxX95583860;     jgvvnbQuXQZLdKeShRcrdxX95583860 = jgvvnbQuXQZLdKeShRcrdxX67516479;     jgvvnbQuXQZLdKeShRcrdxX67516479 = jgvvnbQuXQZLdKeShRcrdxX35658402;     jgvvnbQuXQZLdKeShRcrdxX35658402 = jgvvnbQuXQZLdKeShRcrdxX5210528;     jgvvnbQuXQZLdKeShRcrdxX5210528 = jgvvnbQuXQZLdKeShRcrdxX8035180;     jgvvnbQuXQZLdKeShRcrdxX8035180 = jgvvnbQuXQZLdKeShRcrdxX5719405;     jgvvnbQuXQZLdKeShRcrdxX5719405 = jgvvnbQuXQZLdKeShRcrdxX83647312;     jgvvnbQuXQZLdKeShRcrdxX83647312 = jgvvnbQuXQZLdKeShRcrdxX61542068;     jgvvnbQuXQZLdKeShRcrdxX61542068 = jgvvnbQuXQZLdKeShRcrdxX814422;     jgvvnbQuXQZLdKeShRcrdxX814422 = jgvvnbQuXQZLdKeShRcrdxX68494856;     jgvvnbQuXQZLdKeShRcrdxX68494856 = jgvvnbQuXQZLdKeShRcrdxX84938410;     jgvvnbQuXQZLdKeShRcrdxX84938410 = jgvvnbQuXQZLdKeShRcrdxX37164084;     jgvvnbQuXQZLdKeShRcrdxX37164084 = jgvvnbQuXQZLdKeShRcrdxX9788451;     jgvvnbQuXQZLdKeShRcrdxX9788451 = jgvvnbQuXQZLdKeShRcrdxX1649805;     jgvvnbQuXQZLdKeShRcrdxX1649805 = jgvvnbQuXQZLdKeShRcrdxX49194124;     jgvvnbQuXQZLdKeShRcrdxX49194124 = jgvvnbQuXQZLdKeShRcrdxX97189202;     jgvvnbQuXQZLdKeShRcrdxX97189202 = jgvvnbQuXQZLdKeShRcrdxX20892867;     jgvvnbQuXQZLdKeShRcrdxX20892867 = jgvvnbQuXQZLdKeShRcrdxX71029991;     jgvvnbQuXQZLdKeShRcrdxX71029991 = jgvvnbQuXQZLdKeShRcrdxX39850933;     jgvvnbQuXQZLdKeShRcrdxX39850933 = jgvvnbQuXQZLdKeShRcrdxX45832878;     jgvvnbQuXQZLdKeShRcrdxX45832878 = jgvvnbQuXQZLdKeShRcrdxX18216849;     jgvvnbQuXQZLdKeShRcrdxX18216849 = jgvvnbQuXQZLdKeShRcrdxX55344734;     jgvvnbQuXQZLdKeShRcrdxX55344734 = jgvvnbQuXQZLdKeShRcrdxX23840372;     jgvvnbQuXQZLdKeShRcrdxX23840372 = jgvvnbQuXQZLdKeShRcrdxX11536266;     jgvvnbQuXQZLdKeShRcrdxX11536266 = jgvvnbQuXQZLdKeShRcrdxX94401794;     jgvvnbQuXQZLdKeShRcrdxX94401794 = jgvvnbQuXQZLdKeShRcrdxX78664646;     jgvvnbQuXQZLdKeShRcrdxX78664646 = jgvvnbQuXQZLdKeShRcrdxX57777920;     jgvvnbQuXQZLdKeShRcrdxX57777920 = jgvvnbQuXQZLdKeShRcrdxX15651616;     jgvvnbQuXQZLdKeShRcrdxX15651616 = jgvvnbQuXQZLdKeShRcrdxX83374001;     jgvvnbQuXQZLdKeShRcrdxX83374001 = jgvvnbQuXQZLdKeShRcrdxX8622982;     jgvvnbQuXQZLdKeShRcrdxX8622982 = jgvvnbQuXQZLdKeShRcrdxX87486217;     jgvvnbQuXQZLdKeShRcrdxX87486217 = jgvvnbQuXQZLdKeShRcrdxX31172210;     jgvvnbQuXQZLdKeShRcrdxX31172210 = jgvvnbQuXQZLdKeShRcrdxX5546041;     jgvvnbQuXQZLdKeShRcrdxX5546041 = jgvvnbQuXQZLdKeShRcrdxX28508717;     jgvvnbQuXQZLdKeShRcrdxX28508717 = jgvvnbQuXQZLdKeShRcrdxX80636095;     jgvvnbQuXQZLdKeShRcrdxX80636095 = jgvvnbQuXQZLdKeShRcrdxX16780791;     jgvvnbQuXQZLdKeShRcrdxX16780791 = jgvvnbQuXQZLdKeShRcrdxX44365692;     jgvvnbQuXQZLdKeShRcrdxX44365692 = jgvvnbQuXQZLdKeShRcrdxX54268912;     jgvvnbQuXQZLdKeShRcrdxX54268912 = jgvvnbQuXQZLdKeShRcrdxX74168033;     jgvvnbQuXQZLdKeShRcrdxX74168033 = jgvvnbQuXQZLdKeShRcrdxX83108629;     jgvvnbQuXQZLdKeShRcrdxX83108629 = jgvvnbQuXQZLdKeShRcrdxX79125295;     jgvvnbQuXQZLdKeShRcrdxX79125295 = jgvvnbQuXQZLdKeShRcrdxX18770001;     jgvvnbQuXQZLdKeShRcrdxX18770001 = jgvvnbQuXQZLdKeShRcrdxX37212374;     jgvvnbQuXQZLdKeShRcrdxX37212374 = jgvvnbQuXQZLdKeShRcrdxX85993131;     jgvvnbQuXQZLdKeShRcrdxX85993131 = jgvvnbQuXQZLdKeShRcrdxX645739;     jgvvnbQuXQZLdKeShRcrdxX645739 = jgvvnbQuXQZLdKeShRcrdxX97319558;     jgvvnbQuXQZLdKeShRcrdxX97319558 = jgvvnbQuXQZLdKeShRcrdxX6995052;     jgvvnbQuXQZLdKeShRcrdxX6995052 = jgvvnbQuXQZLdKeShRcrdxX43332941;     jgvvnbQuXQZLdKeShRcrdxX43332941 = jgvvnbQuXQZLdKeShRcrdxX34694834;     jgvvnbQuXQZLdKeShRcrdxX34694834 = jgvvnbQuXQZLdKeShRcrdxX10973615;     jgvvnbQuXQZLdKeShRcrdxX10973615 = jgvvnbQuXQZLdKeShRcrdxX25733225;     jgvvnbQuXQZLdKeShRcrdxX25733225 = jgvvnbQuXQZLdKeShRcrdxX39609518;     jgvvnbQuXQZLdKeShRcrdxX39609518 = jgvvnbQuXQZLdKeShRcrdxX53078887;}



void cfBzTBfhXkturCWgOtjnygTCQu68250724() {     float IrnVPDgtRbLQybaMUcXyeQr36802405 = -999085288;    float IrnVPDgtRbLQybaMUcXyeQr53661590 = -274779490;    float IrnVPDgtRbLQybaMUcXyeQr55206700 = -617806186;    float IrnVPDgtRbLQybaMUcXyeQr85012849 = -562251546;    float IrnVPDgtRbLQybaMUcXyeQr16183645 = -917511001;    float IrnVPDgtRbLQybaMUcXyeQr90626398 = -837977169;    float IrnVPDgtRbLQybaMUcXyeQr14750324 = -902618070;    float IrnVPDgtRbLQybaMUcXyeQr76831477 = -197378720;    float IrnVPDgtRbLQybaMUcXyeQr27432825 = -70126370;    float IrnVPDgtRbLQybaMUcXyeQr26477895 = -475088344;    float IrnVPDgtRbLQybaMUcXyeQr96122239 = 19563472;    float IrnVPDgtRbLQybaMUcXyeQr18890738 = 84966561;    float IrnVPDgtRbLQybaMUcXyeQr59659338 = 48262775;    float IrnVPDgtRbLQybaMUcXyeQr92902062 = -758254649;    float IrnVPDgtRbLQybaMUcXyeQr97550788 = -577898824;    float IrnVPDgtRbLQybaMUcXyeQr50142387 = -30050586;    float IrnVPDgtRbLQybaMUcXyeQr96985825 = -927729062;    float IrnVPDgtRbLQybaMUcXyeQr42962707 = -254304909;    float IrnVPDgtRbLQybaMUcXyeQr36424629 = -207590626;    float IrnVPDgtRbLQybaMUcXyeQr76868311 = -983429423;    float IrnVPDgtRbLQybaMUcXyeQr66880084 = -608984983;    float IrnVPDgtRbLQybaMUcXyeQr92538889 = -288989955;    float IrnVPDgtRbLQybaMUcXyeQr24424939 = -295611330;    float IrnVPDgtRbLQybaMUcXyeQr99268422 = -545317324;    float IrnVPDgtRbLQybaMUcXyeQr39528578 = -878874791;    float IrnVPDgtRbLQybaMUcXyeQr52450982 = 73352612;    float IrnVPDgtRbLQybaMUcXyeQr91090369 = -224079632;    float IrnVPDgtRbLQybaMUcXyeQr18819157 = -818536895;    float IrnVPDgtRbLQybaMUcXyeQr56215412 = -735830365;    float IrnVPDgtRbLQybaMUcXyeQr15572421 = -413711607;    float IrnVPDgtRbLQybaMUcXyeQr40843863 = -328573429;    float IrnVPDgtRbLQybaMUcXyeQr6996501 = -752792965;    float IrnVPDgtRbLQybaMUcXyeQr2952736 = -753426764;    float IrnVPDgtRbLQybaMUcXyeQr98497322 = -877452951;    float IrnVPDgtRbLQybaMUcXyeQr47915394 = -742333084;    float IrnVPDgtRbLQybaMUcXyeQr7888029 = -346262395;    float IrnVPDgtRbLQybaMUcXyeQr54251577 = -157246914;    float IrnVPDgtRbLQybaMUcXyeQr35965941 = -454776548;    float IrnVPDgtRbLQybaMUcXyeQr95099362 = -501407374;    float IrnVPDgtRbLQybaMUcXyeQr55972618 = -723967768;    float IrnVPDgtRbLQybaMUcXyeQr9750267 = -171978160;    float IrnVPDgtRbLQybaMUcXyeQr13228685 = -113253170;    float IrnVPDgtRbLQybaMUcXyeQr40962963 = -265565656;    float IrnVPDgtRbLQybaMUcXyeQr93821610 = -456518155;    float IrnVPDgtRbLQybaMUcXyeQr38626760 = -879392801;    float IrnVPDgtRbLQybaMUcXyeQr16021158 = -119801416;    float IrnVPDgtRbLQybaMUcXyeQr27091153 = -966816483;    float IrnVPDgtRbLQybaMUcXyeQr79731560 = -246446985;    float IrnVPDgtRbLQybaMUcXyeQr29458074 = 22932512;    float IrnVPDgtRbLQybaMUcXyeQr3389810 = -249146931;    float IrnVPDgtRbLQybaMUcXyeQr75749233 = -920260849;    float IrnVPDgtRbLQybaMUcXyeQr56716248 = -726956275;    float IrnVPDgtRbLQybaMUcXyeQr53417582 = -372281219;    float IrnVPDgtRbLQybaMUcXyeQr51652010 = -926507210;    float IrnVPDgtRbLQybaMUcXyeQr1370641 = -846503785;    float IrnVPDgtRbLQybaMUcXyeQr44263515 = -610095334;    float IrnVPDgtRbLQybaMUcXyeQr29236652 = -979168160;    float IrnVPDgtRbLQybaMUcXyeQr55938278 = 27511137;    float IrnVPDgtRbLQybaMUcXyeQr45484271 = -683376755;    float IrnVPDgtRbLQybaMUcXyeQr63732663 = -890863614;    float IrnVPDgtRbLQybaMUcXyeQr99536028 = -513897537;    float IrnVPDgtRbLQybaMUcXyeQr95931167 = 15918824;    float IrnVPDgtRbLQybaMUcXyeQr20616066 = -461548356;    float IrnVPDgtRbLQybaMUcXyeQr11860404 = -656414763;    float IrnVPDgtRbLQybaMUcXyeQr85634031 = -46514916;    float IrnVPDgtRbLQybaMUcXyeQr89125739 = -227643564;    float IrnVPDgtRbLQybaMUcXyeQr15938002 = -161606675;    float IrnVPDgtRbLQybaMUcXyeQr61162016 = -74284275;    float IrnVPDgtRbLQybaMUcXyeQr44986668 = 84078435;    float IrnVPDgtRbLQybaMUcXyeQr89662759 = -131636430;    float IrnVPDgtRbLQybaMUcXyeQr95890809 = -872803672;    float IrnVPDgtRbLQybaMUcXyeQr61019885 = -372952515;    float IrnVPDgtRbLQybaMUcXyeQr47863345 = -752897535;    float IrnVPDgtRbLQybaMUcXyeQr80452011 = -483622858;    float IrnVPDgtRbLQybaMUcXyeQr67118044 = -711451264;    float IrnVPDgtRbLQybaMUcXyeQr53651399 = -395731814;    float IrnVPDgtRbLQybaMUcXyeQr51575927 = 76575700;    float IrnVPDgtRbLQybaMUcXyeQr30603328 = -839093176;    float IrnVPDgtRbLQybaMUcXyeQr60641662 = -665924523;    float IrnVPDgtRbLQybaMUcXyeQr23507421 = -659073376;    float IrnVPDgtRbLQybaMUcXyeQr25359829 = 40169095;    float IrnVPDgtRbLQybaMUcXyeQr11358809 = -977632648;    float IrnVPDgtRbLQybaMUcXyeQr89361082 = -741469408;    float IrnVPDgtRbLQybaMUcXyeQr52825602 = -386683434;    float IrnVPDgtRbLQybaMUcXyeQr39823188 = -493450759;    float IrnVPDgtRbLQybaMUcXyeQr84127615 = -601617154;    float IrnVPDgtRbLQybaMUcXyeQr53578919 = -280511746;    float IrnVPDgtRbLQybaMUcXyeQr51300725 = -826919554;    float IrnVPDgtRbLQybaMUcXyeQr97126682 = 69050833;    float IrnVPDgtRbLQybaMUcXyeQr3651880 = -32237751;    float IrnVPDgtRbLQybaMUcXyeQr78651377 = -367094235;    float IrnVPDgtRbLQybaMUcXyeQr98313299 = -84758052;    float IrnVPDgtRbLQybaMUcXyeQr90481669 = -771399793;    float IrnVPDgtRbLQybaMUcXyeQr31366699 = -610543761;    float IrnVPDgtRbLQybaMUcXyeQr56436589 = -110070232;    float IrnVPDgtRbLQybaMUcXyeQr13819099 = -87896984;    float IrnVPDgtRbLQybaMUcXyeQr92612619 = -651704814;    float IrnVPDgtRbLQybaMUcXyeQr29102560 = -609150893;    float IrnVPDgtRbLQybaMUcXyeQr8187579 = -310003239;    float IrnVPDgtRbLQybaMUcXyeQr49501021 = -999085288;     IrnVPDgtRbLQybaMUcXyeQr36802405 = IrnVPDgtRbLQybaMUcXyeQr53661590;     IrnVPDgtRbLQybaMUcXyeQr53661590 = IrnVPDgtRbLQybaMUcXyeQr55206700;     IrnVPDgtRbLQybaMUcXyeQr55206700 = IrnVPDgtRbLQybaMUcXyeQr85012849;     IrnVPDgtRbLQybaMUcXyeQr85012849 = IrnVPDgtRbLQybaMUcXyeQr16183645;     IrnVPDgtRbLQybaMUcXyeQr16183645 = IrnVPDgtRbLQybaMUcXyeQr90626398;     IrnVPDgtRbLQybaMUcXyeQr90626398 = IrnVPDgtRbLQybaMUcXyeQr14750324;     IrnVPDgtRbLQybaMUcXyeQr14750324 = IrnVPDgtRbLQybaMUcXyeQr76831477;     IrnVPDgtRbLQybaMUcXyeQr76831477 = IrnVPDgtRbLQybaMUcXyeQr27432825;     IrnVPDgtRbLQybaMUcXyeQr27432825 = IrnVPDgtRbLQybaMUcXyeQr26477895;     IrnVPDgtRbLQybaMUcXyeQr26477895 = IrnVPDgtRbLQybaMUcXyeQr96122239;     IrnVPDgtRbLQybaMUcXyeQr96122239 = IrnVPDgtRbLQybaMUcXyeQr18890738;     IrnVPDgtRbLQybaMUcXyeQr18890738 = IrnVPDgtRbLQybaMUcXyeQr59659338;     IrnVPDgtRbLQybaMUcXyeQr59659338 = IrnVPDgtRbLQybaMUcXyeQr92902062;     IrnVPDgtRbLQybaMUcXyeQr92902062 = IrnVPDgtRbLQybaMUcXyeQr97550788;     IrnVPDgtRbLQybaMUcXyeQr97550788 = IrnVPDgtRbLQybaMUcXyeQr50142387;     IrnVPDgtRbLQybaMUcXyeQr50142387 = IrnVPDgtRbLQybaMUcXyeQr96985825;     IrnVPDgtRbLQybaMUcXyeQr96985825 = IrnVPDgtRbLQybaMUcXyeQr42962707;     IrnVPDgtRbLQybaMUcXyeQr42962707 = IrnVPDgtRbLQybaMUcXyeQr36424629;     IrnVPDgtRbLQybaMUcXyeQr36424629 = IrnVPDgtRbLQybaMUcXyeQr76868311;     IrnVPDgtRbLQybaMUcXyeQr76868311 = IrnVPDgtRbLQybaMUcXyeQr66880084;     IrnVPDgtRbLQybaMUcXyeQr66880084 = IrnVPDgtRbLQybaMUcXyeQr92538889;     IrnVPDgtRbLQybaMUcXyeQr92538889 = IrnVPDgtRbLQybaMUcXyeQr24424939;     IrnVPDgtRbLQybaMUcXyeQr24424939 = IrnVPDgtRbLQybaMUcXyeQr99268422;     IrnVPDgtRbLQybaMUcXyeQr99268422 = IrnVPDgtRbLQybaMUcXyeQr39528578;     IrnVPDgtRbLQybaMUcXyeQr39528578 = IrnVPDgtRbLQybaMUcXyeQr52450982;     IrnVPDgtRbLQybaMUcXyeQr52450982 = IrnVPDgtRbLQybaMUcXyeQr91090369;     IrnVPDgtRbLQybaMUcXyeQr91090369 = IrnVPDgtRbLQybaMUcXyeQr18819157;     IrnVPDgtRbLQybaMUcXyeQr18819157 = IrnVPDgtRbLQybaMUcXyeQr56215412;     IrnVPDgtRbLQybaMUcXyeQr56215412 = IrnVPDgtRbLQybaMUcXyeQr15572421;     IrnVPDgtRbLQybaMUcXyeQr15572421 = IrnVPDgtRbLQybaMUcXyeQr40843863;     IrnVPDgtRbLQybaMUcXyeQr40843863 = IrnVPDgtRbLQybaMUcXyeQr6996501;     IrnVPDgtRbLQybaMUcXyeQr6996501 = IrnVPDgtRbLQybaMUcXyeQr2952736;     IrnVPDgtRbLQybaMUcXyeQr2952736 = IrnVPDgtRbLQybaMUcXyeQr98497322;     IrnVPDgtRbLQybaMUcXyeQr98497322 = IrnVPDgtRbLQybaMUcXyeQr47915394;     IrnVPDgtRbLQybaMUcXyeQr47915394 = IrnVPDgtRbLQybaMUcXyeQr7888029;     IrnVPDgtRbLQybaMUcXyeQr7888029 = IrnVPDgtRbLQybaMUcXyeQr54251577;     IrnVPDgtRbLQybaMUcXyeQr54251577 = IrnVPDgtRbLQybaMUcXyeQr35965941;     IrnVPDgtRbLQybaMUcXyeQr35965941 = IrnVPDgtRbLQybaMUcXyeQr95099362;     IrnVPDgtRbLQybaMUcXyeQr95099362 = IrnVPDgtRbLQybaMUcXyeQr55972618;     IrnVPDgtRbLQybaMUcXyeQr55972618 = IrnVPDgtRbLQybaMUcXyeQr9750267;     IrnVPDgtRbLQybaMUcXyeQr9750267 = IrnVPDgtRbLQybaMUcXyeQr13228685;     IrnVPDgtRbLQybaMUcXyeQr13228685 = IrnVPDgtRbLQybaMUcXyeQr40962963;     IrnVPDgtRbLQybaMUcXyeQr40962963 = IrnVPDgtRbLQybaMUcXyeQr93821610;     IrnVPDgtRbLQybaMUcXyeQr93821610 = IrnVPDgtRbLQybaMUcXyeQr38626760;     IrnVPDgtRbLQybaMUcXyeQr38626760 = IrnVPDgtRbLQybaMUcXyeQr16021158;     IrnVPDgtRbLQybaMUcXyeQr16021158 = IrnVPDgtRbLQybaMUcXyeQr27091153;     IrnVPDgtRbLQybaMUcXyeQr27091153 = IrnVPDgtRbLQybaMUcXyeQr79731560;     IrnVPDgtRbLQybaMUcXyeQr79731560 = IrnVPDgtRbLQybaMUcXyeQr29458074;     IrnVPDgtRbLQybaMUcXyeQr29458074 = IrnVPDgtRbLQybaMUcXyeQr3389810;     IrnVPDgtRbLQybaMUcXyeQr3389810 = IrnVPDgtRbLQybaMUcXyeQr75749233;     IrnVPDgtRbLQybaMUcXyeQr75749233 = IrnVPDgtRbLQybaMUcXyeQr56716248;     IrnVPDgtRbLQybaMUcXyeQr56716248 = IrnVPDgtRbLQybaMUcXyeQr53417582;     IrnVPDgtRbLQybaMUcXyeQr53417582 = IrnVPDgtRbLQybaMUcXyeQr51652010;     IrnVPDgtRbLQybaMUcXyeQr51652010 = IrnVPDgtRbLQybaMUcXyeQr1370641;     IrnVPDgtRbLQybaMUcXyeQr1370641 = IrnVPDgtRbLQybaMUcXyeQr44263515;     IrnVPDgtRbLQybaMUcXyeQr44263515 = IrnVPDgtRbLQybaMUcXyeQr29236652;     IrnVPDgtRbLQybaMUcXyeQr29236652 = IrnVPDgtRbLQybaMUcXyeQr55938278;     IrnVPDgtRbLQybaMUcXyeQr55938278 = IrnVPDgtRbLQybaMUcXyeQr45484271;     IrnVPDgtRbLQybaMUcXyeQr45484271 = IrnVPDgtRbLQybaMUcXyeQr63732663;     IrnVPDgtRbLQybaMUcXyeQr63732663 = IrnVPDgtRbLQybaMUcXyeQr99536028;     IrnVPDgtRbLQybaMUcXyeQr99536028 = IrnVPDgtRbLQybaMUcXyeQr95931167;     IrnVPDgtRbLQybaMUcXyeQr95931167 = IrnVPDgtRbLQybaMUcXyeQr20616066;     IrnVPDgtRbLQybaMUcXyeQr20616066 = IrnVPDgtRbLQybaMUcXyeQr11860404;     IrnVPDgtRbLQybaMUcXyeQr11860404 = IrnVPDgtRbLQybaMUcXyeQr85634031;     IrnVPDgtRbLQybaMUcXyeQr85634031 = IrnVPDgtRbLQybaMUcXyeQr89125739;     IrnVPDgtRbLQybaMUcXyeQr89125739 = IrnVPDgtRbLQybaMUcXyeQr15938002;     IrnVPDgtRbLQybaMUcXyeQr15938002 = IrnVPDgtRbLQybaMUcXyeQr61162016;     IrnVPDgtRbLQybaMUcXyeQr61162016 = IrnVPDgtRbLQybaMUcXyeQr44986668;     IrnVPDgtRbLQybaMUcXyeQr44986668 = IrnVPDgtRbLQybaMUcXyeQr89662759;     IrnVPDgtRbLQybaMUcXyeQr89662759 = IrnVPDgtRbLQybaMUcXyeQr95890809;     IrnVPDgtRbLQybaMUcXyeQr95890809 = IrnVPDgtRbLQybaMUcXyeQr61019885;     IrnVPDgtRbLQybaMUcXyeQr61019885 = IrnVPDgtRbLQybaMUcXyeQr47863345;     IrnVPDgtRbLQybaMUcXyeQr47863345 = IrnVPDgtRbLQybaMUcXyeQr80452011;     IrnVPDgtRbLQybaMUcXyeQr80452011 = IrnVPDgtRbLQybaMUcXyeQr67118044;     IrnVPDgtRbLQybaMUcXyeQr67118044 = IrnVPDgtRbLQybaMUcXyeQr53651399;     IrnVPDgtRbLQybaMUcXyeQr53651399 = IrnVPDgtRbLQybaMUcXyeQr51575927;     IrnVPDgtRbLQybaMUcXyeQr51575927 = IrnVPDgtRbLQybaMUcXyeQr30603328;     IrnVPDgtRbLQybaMUcXyeQr30603328 = IrnVPDgtRbLQybaMUcXyeQr60641662;     IrnVPDgtRbLQybaMUcXyeQr60641662 = IrnVPDgtRbLQybaMUcXyeQr23507421;     IrnVPDgtRbLQybaMUcXyeQr23507421 = IrnVPDgtRbLQybaMUcXyeQr25359829;     IrnVPDgtRbLQybaMUcXyeQr25359829 = IrnVPDgtRbLQybaMUcXyeQr11358809;     IrnVPDgtRbLQybaMUcXyeQr11358809 = IrnVPDgtRbLQybaMUcXyeQr89361082;     IrnVPDgtRbLQybaMUcXyeQr89361082 = IrnVPDgtRbLQybaMUcXyeQr52825602;     IrnVPDgtRbLQybaMUcXyeQr52825602 = IrnVPDgtRbLQybaMUcXyeQr39823188;     IrnVPDgtRbLQybaMUcXyeQr39823188 = IrnVPDgtRbLQybaMUcXyeQr84127615;     IrnVPDgtRbLQybaMUcXyeQr84127615 = IrnVPDgtRbLQybaMUcXyeQr53578919;     IrnVPDgtRbLQybaMUcXyeQr53578919 = IrnVPDgtRbLQybaMUcXyeQr51300725;     IrnVPDgtRbLQybaMUcXyeQr51300725 = IrnVPDgtRbLQybaMUcXyeQr97126682;     IrnVPDgtRbLQybaMUcXyeQr97126682 = IrnVPDgtRbLQybaMUcXyeQr3651880;     IrnVPDgtRbLQybaMUcXyeQr3651880 = IrnVPDgtRbLQybaMUcXyeQr78651377;     IrnVPDgtRbLQybaMUcXyeQr78651377 = IrnVPDgtRbLQybaMUcXyeQr98313299;     IrnVPDgtRbLQybaMUcXyeQr98313299 = IrnVPDgtRbLQybaMUcXyeQr90481669;     IrnVPDgtRbLQybaMUcXyeQr90481669 = IrnVPDgtRbLQybaMUcXyeQr31366699;     IrnVPDgtRbLQybaMUcXyeQr31366699 = IrnVPDgtRbLQybaMUcXyeQr56436589;     IrnVPDgtRbLQybaMUcXyeQr56436589 = IrnVPDgtRbLQybaMUcXyeQr13819099;     IrnVPDgtRbLQybaMUcXyeQr13819099 = IrnVPDgtRbLQybaMUcXyeQr92612619;     IrnVPDgtRbLQybaMUcXyeQr92612619 = IrnVPDgtRbLQybaMUcXyeQr29102560;     IrnVPDgtRbLQybaMUcXyeQr29102560 = IrnVPDgtRbLQybaMUcXyeQr8187579;     IrnVPDgtRbLQybaMUcXyeQr8187579 = IrnVPDgtRbLQybaMUcXyeQr49501021;     IrnVPDgtRbLQybaMUcXyeQr49501021 = IrnVPDgtRbLQybaMUcXyeQr36802405;}



void LEXtBVAXdeEZJNRFhVXuWOCAqD53313562() {     float ssomBKcMcoKjjAfHWFqqjlV20525923 = -249107355;    float ssomBKcMcoKjjAfHWFqqjlV51259685 = -78431341;    float ssomBKcMcoKjjAfHWFqqjlV43792817 = -486113877;    float ssomBKcMcoKjjAfHWFqqjlV82160802 = -567133475;    float ssomBKcMcoKjjAfHWFqqjlV95744981 = -215459192;    float ssomBKcMcoKjjAfHWFqqjlV1965916 = -941297838;    float ssomBKcMcoKjjAfHWFqqjlV51630887 = -924132208;    float ssomBKcMcoKjjAfHWFqqjlV20171560 = -817833306;    float ssomBKcMcoKjjAfHWFqqjlV77652353 = -767609066;    float ssomBKcMcoKjjAfHWFqqjlV72583483 = -698129360;    float ssomBKcMcoKjjAfHWFqqjlV39011276 = -920721786;    float ssomBKcMcoKjjAfHWFqqjlV9102508 = -944172710;    float ssomBKcMcoKjjAfHWFqqjlV10713190 = -894569468;    float ssomBKcMcoKjjAfHWFqqjlV87275206 = -7937511;    float ssomBKcMcoKjjAfHWFqqjlV1730047 = -503596129;    float ssomBKcMcoKjjAfHWFqqjlV21956416 = 45821763;    float ssomBKcMcoKjjAfHWFqqjlV96851236 = 13391075;    float ssomBKcMcoKjjAfHWFqqjlV57039976 = -965156483;    float ssomBKcMcoKjjAfHWFqqjlV77401109 = 3760136;    float ssomBKcMcoKjjAfHWFqqjlV50975955 = -599722340;    float ssomBKcMcoKjjAfHWFqqjlV34219594 = -183451504;    float ssomBKcMcoKjjAfHWFqqjlV93540960 = -202145242;    float ssomBKcMcoKjjAfHWFqqjlV93600803 = -325006067;    float ssomBKcMcoKjjAfHWFqqjlV411118 = -84549977;    float ssomBKcMcoKjjAfHWFqqjlV76130671 = -957205553;    float ssomBKcMcoKjjAfHWFqqjlV5443740 = -369614415;    float ssomBKcMcoKjjAfHWFqqjlV12682309 = -667955056;    float ssomBKcMcoKjjAfHWFqqjlV61418356 = -604978384;    float ssomBKcMcoKjjAfHWFqqjlV28133554 = -160026265;    float ssomBKcMcoKjjAfHWFqqjlV51120748 = 13682018;    float ssomBKcMcoKjjAfHWFqqjlV22208287 = -547671449;    float ssomBKcMcoKjjAfHWFqqjlV31789789 = -201990187;    float ssomBKcMcoKjjAfHWFqqjlV17077437 = -318640855;    float ssomBKcMcoKjjAfHWFqqjlV34222037 = -181549599;    float ssomBKcMcoKjjAfHWFqqjlV15518720 = -476623692;    float ssomBKcMcoKjjAfHWFqqjlV77749262 = -324658872;    float ssomBKcMcoKjjAfHWFqqjlV54015168 = -477588211;    float ssomBKcMcoKjjAfHWFqqjlV86347731 = -353919764;    float ssomBKcMcoKjjAfHWFqqjlV55715081 = -806966256;    float ssomBKcMcoKjjAfHWFqqjlV95161733 = -670614648;    float ssomBKcMcoKjjAfHWFqqjlV74517787 = -922018286;    float ssomBKcMcoKjjAfHWFqqjlV42568412 = -819278839;    float ssomBKcMcoKjjAfHWFqqjlV73763109 = 82030090;    float ssomBKcMcoKjjAfHWFqqjlV41017130 = -35461887;    float ssomBKcMcoKjjAfHWFqqjlV66614011 = -105406879;    float ssomBKcMcoKjjAfHWFqqjlV60288039 = -834833108;    float ssomBKcMcoKjjAfHWFqqjlV60270122 = -988462390;    float ssomBKcMcoKjjAfHWFqqjlV18473408 = -8568882;    float ssomBKcMcoKjjAfHWFqqjlV63332287 = -776851639;    float ssomBKcMcoKjjAfHWFqqjlV39263140 = -881162367;    float ssomBKcMcoKjjAfHWFqqjlV15840065 = 13966750;    float ssomBKcMcoKjjAfHWFqqjlV8221968 = -479009062;    float ssomBKcMcoKjjAfHWFqqjlV98799984 = -384213585;    float ssomBKcMcoKjjAfHWFqqjlV97584616 = -403216398;    float ssomBKcMcoKjjAfHWFqqjlV19093968 = -63230730;    float ssomBKcMcoKjjAfHWFqqjlV26984962 = 53037886;    float ssomBKcMcoKjjAfHWFqqjlV57658882 = -753425275;    float ssomBKcMcoKjjAfHWFqqjlV43381700 = -301563901;    float ssomBKcMcoKjjAfHWFqqjlV6030132 = -609927922;    float ssomBKcMcoKjjAfHWFqqjlV90301241 = -845844777;    float ssomBKcMcoKjjAfHWFqqjlV89283607 = -173342783;    float ssomBKcMcoKjjAfHWFqqjlV90212530 = -219153825;    float ssomBKcMcoKjjAfHWFqqjlV92038006 = -557807042;    float ssomBKcMcoKjjAfHWFqqjlV26531605 = -681291085;    float ssomBKcMcoKjjAfHWFqqjlV50375197 = -50457911;    float ssomBKcMcoKjjAfHWFqqjlV7221487 = -618731599;    float ssomBKcMcoKjjAfHWFqqjlV92025071 = -525531855;    float ssomBKcMcoKjjAfHWFqqjlV76491153 = -613019870;    float ssomBKcMcoKjjAfHWFqqjlV71756487 = -531313820;    float ssomBKcMcoKjjAfHWFqqjlV23980785 = -78937258;    float ssomBKcMcoKjjAfHWFqqjlV67941248 = -476590027;    float ssomBKcMcoKjjAfHWFqqjlV10503505 = -632689161;    float ssomBKcMcoKjjAfHWFqqjlV1324896 = -58190228;    float ssomBKcMcoKjjAfHWFqqjlV82239375 = -325625217;    float ssomBKcMcoKjjAfHWFqqjlV76458167 = -677704054;    float ssomBKcMcoKjjAfHWFqqjlV91651182 = -364172665;    float ssomBKcMcoKjjAfHWFqqjlV19777852 = -184175332;    float ssomBKcMcoKjjAfHWFqqjlV52583673 = -189544180;    float ssomBKcMcoKjjAfHWFqqjlV33797106 = -979143098;    float ssomBKcMcoKjjAfHWFqqjlV15842633 = -22372446;    float ssomBKcMcoKjjAfHWFqqjlV45173617 = -381152026;    float ssomBKcMcoKjjAfHWFqqjlV94208901 = -559386174;    float ssomBKcMcoKjjAfHWFqqjlV98086069 = -828126745;    float ssomBKcMcoKjjAfHWFqqjlV88870414 = -278863899;    float ssomBKcMcoKjjAfHWFqqjlV35280683 = 99715268;    float ssomBKcMcoKjjAfHWFqqjlV13986319 = 31337612;    float ssomBKcMcoKjjAfHWFqqjlV32989805 = -817776603;    float ssomBKcMcoKjjAfHWFqqjlV19492821 = -915424458;    float ssomBKcMcoKjjAfHWFqqjlV15128069 = -18318869;    float ssomBKcMcoKjjAfHWFqqjlV88533757 = -429661578;    float ssomBKcMcoKjjAfHWFqqjlV20090381 = -571233598;    float ssomBKcMcoKjjAfHWFqqjlV10633468 = -76024311;    float ssomBKcMcoKjjAfHWFqqjlV80317600 = -743991842;    float ssomBKcMcoKjjAfHWFqqjlV65413839 = -961121479;    float ssomBKcMcoKjjAfHWFqqjlV5878127 = -397271865;    float ssomBKcMcoKjjAfHWFqqjlV84305257 = -602864461;    float ssomBKcMcoKjjAfHWFqqjlV50530405 = -161471798;    float ssomBKcMcoKjjAfHWFqqjlV47231505 = -236678826;    float ssomBKcMcoKjjAfHWFqqjlV90641933 = -985003976;    float ssomBKcMcoKjjAfHWFqqjlV59392524 = -249107355;     ssomBKcMcoKjjAfHWFqqjlV20525923 = ssomBKcMcoKjjAfHWFqqjlV51259685;     ssomBKcMcoKjjAfHWFqqjlV51259685 = ssomBKcMcoKjjAfHWFqqjlV43792817;     ssomBKcMcoKjjAfHWFqqjlV43792817 = ssomBKcMcoKjjAfHWFqqjlV82160802;     ssomBKcMcoKjjAfHWFqqjlV82160802 = ssomBKcMcoKjjAfHWFqqjlV95744981;     ssomBKcMcoKjjAfHWFqqjlV95744981 = ssomBKcMcoKjjAfHWFqqjlV1965916;     ssomBKcMcoKjjAfHWFqqjlV1965916 = ssomBKcMcoKjjAfHWFqqjlV51630887;     ssomBKcMcoKjjAfHWFqqjlV51630887 = ssomBKcMcoKjjAfHWFqqjlV20171560;     ssomBKcMcoKjjAfHWFqqjlV20171560 = ssomBKcMcoKjjAfHWFqqjlV77652353;     ssomBKcMcoKjjAfHWFqqjlV77652353 = ssomBKcMcoKjjAfHWFqqjlV72583483;     ssomBKcMcoKjjAfHWFqqjlV72583483 = ssomBKcMcoKjjAfHWFqqjlV39011276;     ssomBKcMcoKjjAfHWFqqjlV39011276 = ssomBKcMcoKjjAfHWFqqjlV9102508;     ssomBKcMcoKjjAfHWFqqjlV9102508 = ssomBKcMcoKjjAfHWFqqjlV10713190;     ssomBKcMcoKjjAfHWFqqjlV10713190 = ssomBKcMcoKjjAfHWFqqjlV87275206;     ssomBKcMcoKjjAfHWFqqjlV87275206 = ssomBKcMcoKjjAfHWFqqjlV1730047;     ssomBKcMcoKjjAfHWFqqjlV1730047 = ssomBKcMcoKjjAfHWFqqjlV21956416;     ssomBKcMcoKjjAfHWFqqjlV21956416 = ssomBKcMcoKjjAfHWFqqjlV96851236;     ssomBKcMcoKjjAfHWFqqjlV96851236 = ssomBKcMcoKjjAfHWFqqjlV57039976;     ssomBKcMcoKjjAfHWFqqjlV57039976 = ssomBKcMcoKjjAfHWFqqjlV77401109;     ssomBKcMcoKjjAfHWFqqjlV77401109 = ssomBKcMcoKjjAfHWFqqjlV50975955;     ssomBKcMcoKjjAfHWFqqjlV50975955 = ssomBKcMcoKjjAfHWFqqjlV34219594;     ssomBKcMcoKjjAfHWFqqjlV34219594 = ssomBKcMcoKjjAfHWFqqjlV93540960;     ssomBKcMcoKjjAfHWFqqjlV93540960 = ssomBKcMcoKjjAfHWFqqjlV93600803;     ssomBKcMcoKjjAfHWFqqjlV93600803 = ssomBKcMcoKjjAfHWFqqjlV411118;     ssomBKcMcoKjjAfHWFqqjlV411118 = ssomBKcMcoKjjAfHWFqqjlV76130671;     ssomBKcMcoKjjAfHWFqqjlV76130671 = ssomBKcMcoKjjAfHWFqqjlV5443740;     ssomBKcMcoKjjAfHWFqqjlV5443740 = ssomBKcMcoKjjAfHWFqqjlV12682309;     ssomBKcMcoKjjAfHWFqqjlV12682309 = ssomBKcMcoKjjAfHWFqqjlV61418356;     ssomBKcMcoKjjAfHWFqqjlV61418356 = ssomBKcMcoKjjAfHWFqqjlV28133554;     ssomBKcMcoKjjAfHWFqqjlV28133554 = ssomBKcMcoKjjAfHWFqqjlV51120748;     ssomBKcMcoKjjAfHWFqqjlV51120748 = ssomBKcMcoKjjAfHWFqqjlV22208287;     ssomBKcMcoKjjAfHWFqqjlV22208287 = ssomBKcMcoKjjAfHWFqqjlV31789789;     ssomBKcMcoKjjAfHWFqqjlV31789789 = ssomBKcMcoKjjAfHWFqqjlV17077437;     ssomBKcMcoKjjAfHWFqqjlV17077437 = ssomBKcMcoKjjAfHWFqqjlV34222037;     ssomBKcMcoKjjAfHWFqqjlV34222037 = ssomBKcMcoKjjAfHWFqqjlV15518720;     ssomBKcMcoKjjAfHWFqqjlV15518720 = ssomBKcMcoKjjAfHWFqqjlV77749262;     ssomBKcMcoKjjAfHWFqqjlV77749262 = ssomBKcMcoKjjAfHWFqqjlV54015168;     ssomBKcMcoKjjAfHWFqqjlV54015168 = ssomBKcMcoKjjAfHWFqqjlV86347731;     ssomBKcMcoKjjAfHWFqqjlV86347731 = ssomBKcMcoKjjAfHWFqqjlV55715081;     ssomBKcMcoKjjAfHWFqqjlV55715081 = ssomBKcMcoKjjAfHWFqqjlV95161733;     ssomBKcMcoKjjAfHWFqqjlV95161733 = ssomBKcMcoKjjAfHWFqqjlV74517787;     ssomBKcMcoKjjAfHWFqqjlV74517787 = ssomBKcMcoKjjAfHWFqqjlV42568412;     ssomBKcMcoKjjAfHWFqqjlV42568412 = ssomBKcMcoKjjAfHWFqqjlV73763109;     ssomBKcMcoKjjAfHWFqqjlV73763109 = ssomBKcMcoKjjAfHWFqqjlV41017130;     ssomBKcMcoKjjAfHWFqqjlV41017130 = ssomBKcMcoKjjAfHWFqqjlV66614011;     ssomBKcMcoKjjAfHWFqqjlV66614011 = ssomBKcMcoKjjAfHWFqqjlV60288039;     ssomBKcMcoKjjAfHWFqqjlV60288039 = ssomBKcMcoKjjAfHWFqqjlV60270122;     ssomBKcMcoKjjAfHWFqqjlV60270122 = ssomBKcMcoKjjAfHWFqqjlV18473408;     ssomBKcMcoKjjAfHWFqqjlV18473408 = ssomBKcMcoKjjAfHWFqqjlV63332287;     ssomBKcMcoKjjAfHWFqqjlV63332287 = ssomBKcMcoKjjAfHWFqqjlV39263140;     ssomBKcMcoKjjAfHWFqqjlV39263140 = ssomBKcMcoKjjAfHWFqqjlV15840065;     ssomBKcMcoKjjAfHWFqqjlV15840065 = ssomBKcMcoKjjAfHWFqqjlV8221968;     ssomBKcMcoKjjAfHWFqqjlV8221968 = ssomBKcMcoKjjAfHWFqqjlV98799984;     ssomBKcMcoKjjAfHWFqqjlV98799984 = ssomBKcMcoKjjAfHWFqqjlV97584616;     ssomBKcMcoKjjAfHWFqqjlV97584616 = ssomBKcMcoKjjAfHWFqqjlV19093968;     ssomBKcMcoKjjAfHWFqqjlV19093968 = ssomBKcMcoKjjAfHWFqqjlV26984962;     ssomBKcMcoKjjAfHWFqqjlV26984962 = ssomBKcMcoKjjAfHWFqqjlV57658882;     ssomBKcMcoKjjAfHWFqqjlV57658882 = ssomBKcMcoKjjAfHWFqqjlV43381700;     ssomBKcMcoKjjAfHWFqqjlV43381700 = ssomBKcMcoKjjAfHWFqqjlV6030132;     ssomBKcMcoKjjAfHWFqqjlV6030132 = ssomBKcMcoKjjAfHWFqqjlV90301241;     ssomBKcMcoKjjAfHWFqqjlV90301241 = ssomBKcMcoKjjAfHWFqqjlV89283607;     ssomBKcMcoKjjAfHWFqqjlV89283607 = ssomBKcMcoKjjAfHWFqqjlV90212530;     ssomBKcMcoKjjAfHWFqqjlV90212530 = ssomBKcMcoKjjAfHWFqqjlV92038006;     ssomBKcMcoKjjAfHWFqqjlV92038006 = ssomBKcMcoKjjAfHWFqqjlV26531605;     ssomBKcMcoKjjAfHWFqqjlV26531605 = ssomBKcMcoKjjAfHWFqqjlV50375197;     ssomBKcMcoKjjAfHWFqqjlV50375197 = ssomBKcMcoKjjAfHWFqqjlV7221487;     ssomBKcMcoKjjAfHWFqqjlV7221487 = ssomBKcMcoKjjAfHWFqqjlV92025071;     ssomBKcMcoKjjAfHWFqqjlV92025071 = ssomBKcMcoKjjAfHWFqqjlV76491153;     ssomBKcMcoKjjAfHWFqqjlV76491153 = ssomBKcMcoKjjAfHWFqqjlV71756487;     ssomBKcMcoKjjAfHWFqqjlV71756487 = ssomBKcMcoKjjAfHWFqqjlV23980785;     ssomBKcMcoKjjAfHWFqqjlV23980785 = ssomBKcMcoKjjAfHWFqqjlV67941248;     ssomBKcMcoKjjAfHWFqqjlV67941248 = ssomBKcMcoKjjAfHWFqqjlV10503505;     ssomBKcMcoKjjAfHWFqqjlV10503505 = ssomBKcMcoKjjAfHWFqqjlV1324896;     ssomBKcMcoKjjAfHWFqqjlV1324896 = ssomBKcMcoKjjAfHWFqqjlV82239375;     ssomBKcMcoKjjAfHWFqqjlV82239375 = ssomBKcMcoKjjAfHWFqqjlV76458167;     ssomBKcMcoKjjAfHWFqqjlV76458167 = ssomBKcMcoKjjAfHWFqqjlV91651182;     ssomBKcMcoKjjAfHWFqqjlV91651182 = ssomBKcMcoKjjAfHWFqqjlV19777852;     ssomBKcMcoKjjAfHWFqqjlV19777852 = ssomBKcMcoKjjAfHWFqqjlV52583673;     ssomBKcMcoKjjAfHWFqqjlV52583673 = ssomBKcMcoKjjAfHWFqqjlV33797106;     ssomBKcMcoKjjAfHWFqqjlV33797106 = ssomBKcMcoKjjAfHWFqqjlV15842633;     ssomBKcMcoKjjAfHWFqqjlV15842633 = ssomBKcMcoKjjAfHWFqqjlV45173617;     ssomBKcMcoKjjAfHWFqqjlV45173617 = ssomBKcMcoKjjAfHWFqqjlV94208901;     ssomBKcMcoKjjAfHWFqqjlV94208901 = ssomBKcMcoKjjAfHWFqqjlV98086069;     ssomBKcMcoKjjAfHWFqqjlV98086069 = ssomBKcMcoKjjAfHWFqqjlV88870414;     ssomBKcMcoKjjAfHWFqqjlV88870414 = ssomBKcMcoKjjAfHWFqqjlV35280683;     ssomBKcMcoKjjAfHWFqqjlV35280683 = ssomBKcMcoKjjAfHWFqqjlV13986319;     ssomBKcMcoKjjAfHWFqqjlV13986319 = ssomBKcMcoKjjAfHWFqqjlV32989805;     ssomBKcMcoKjjAfHWFqqjlV32989805 = ssomBKcMcoKjjAfHWFqqjlV19492821;     ssomBKcMcoKjjAfHWFqqjlV19492821 = ssomBKcMcoKjjAfHWFqqjlV15128069;     ssomBKcMcoKjjAfHWFqqjlV15128069 = ssomBKcMcoKjjAfHWFqqjlV88533757;     ssomBKcMcoKjjAfHWFqqjlV88533757 = ssomBKcMcoKjjAfHWFqqjlV20090381;     ssomBKcMcoKjjAfHWFqqjlV20090381 = ssomBKcMcoKjjAfHWFqqjlV10633468;     ssomBKcMcoKjjAfHWFqqjlV10633468 = ssomBKcMcoKjjAfHWFqqjlV80317600;     ssomBKcMcoKjjAfHWFqqjlV80317600 = ssomBKcMcoKjjAfHWFqqjlV65413839;     ssomBKcMcoKjjAfHWFqqjlV65413839 = ssomBKcMcoKjjAfHWFqqjlV5878127;     ssomBKcMcoKjjAfHWFqqjlV5878127 = ssomBKcMcoKjjAfHWFqqjlV84305257;     ssomBKcMcoKjjAfHWFqqjlV84305257 = ssomBKcMcoKjjAfHWFqqjlV50530405;     ssomBKcMcoKjjAfHWFqqjlV50530405 = ssomBKcMcoKjjAfHWFqqjlV47231505;     ssomBKcMcoKjjAfHWFqqjlV47231505 = ssomBKcMcoKjjAfHWFqqjlV90641933;     ssomBKcMcoKjjAfHWFqqjlV90641933 = ssomBKcMcoKjjAfHWFqqjlV59392524;     ssomBKcMcoKjjAfHWFqqjlV59392524 = ssomBKcMcoKjjAfHWFqqjlV20525923;}



void gjDWkqZAxlWFnGNWiHiLcRPaYl38376400() {     float ICqEhvKxxqHplOOEAnhoLTS4249441 = -599129423;    float ICqEhvKxxqHplOOEAnhoLTS48857781 = -982083192;    float ICqEhvKxxqHplOOEAnhoLTS32378935 = -354421568;    float ICqEhvKxxqHplOOEAnhoLTS79308755 = -572015403;    float ICqEhvKxxqHplOOEAnhoLTS75306318 = -613407384;    float ICqEhvKxxqHplOOEAnhoLTS13305434 = 55381493;    float ICqEhvKxxqHplOOEAnhoLTS88511450 = -945646346;    float ICqEhvKxxqHplOOEAnhoLTS63511643 = -338287892;    float ICqEhvKxxqHplOOEAnhoLTS27871882 = -365091763;    float ICqEhvKxxqHplOOEAnhoLTS18689072 = -921170376;    float ICqEhvKxxqHplOOEAnhoLTS81900311 = -761007044;    float ICqEhvKxxqHplOOEAnhoLTS99314278 = -873311981;    float ICqEhvKxxqHplOOEAnhoLTS61767042 = -737401712;    float ICqEhvKxxqHplOOEAnhoLTS81648351 = -357620374;    float ICqEhvKxxqHplOOEAnhoLTS5909306 = -429293434;    float ICqEhvKxxqHplOOEAnhoLTS93770444 = -978305889;    float ICqEhvKxxqHplOOEAnhoLTS96716647 = -145488788;    float ICqEhvKxxqHplOOEAnhoLTS71117246 = -576008057;    float ICqEhvKxxqHplOOEAnhoLTS18377591 = -884889102;    float ICqEhvKxxqHplOOEAnhoLTS25083600 = -216015257;    float ICqEhvKxxqHplOOEAnhoLTS1559105 = -857918025;    float ICqEhvKxxqHplOOEAnhoLTS94543031 = -115300528;    float ICqEhvKxxqHplOOEAnhoLTS62776669 = -354400804;    float ICqEhvKxxqHplOOEAnhoLTS1553813 = -723782630;    float ICqEhvKxxqHplOOEAnhoLTS12732764 = 64463686;    float ICqEhvKxxqHplOOEAnhoLTS58436497 = -812581443;    float ICqEhvKxxqHplOOEAnhoLTS34274248 = -11830479;    float ICqEhvKxxqHplOOEAnhoLTS4017557 = -391419872;    float ICqEhvKxxqHplOOEAnhoLTS51696 = -684222165;    float ICqEhvKxxqHplOOEAnhoLTS86669075 = -658924357;    float ICqEhvKxxqHplOOEAnhoLTS3572710 = -766769470;    float ICqEhvKxxqHplOOEAnhoLTS56583078 = -751187410;    float ICqEhvKxxqHplOOEAnhoLTS31202138 = -983854947;    float ICqEhvKxxqHplOOEAnhoLTS69946751 = -585646246;    float ICqEhvKxxqHplOOEAnhoLTS83122045 = -210914299;    float ICqEhvKxxqHplOOEAnhoLTS47610496 = -303055349;    float ICqEhvKxxqHplOOEAnhoLTS53778758 = -797929507;    float ICqEhvKxxqHplOOEAnhoLTS36729523 = -253062981;    float ICqEhvKxxqHplOOEAnhoLTS16330801 = -12525137;    float ICqEhvKxxqHplOOEAnhoLTS34350850 = -617261527;    float ICqEhvKxxqHplOOEAnhoLTS39285308 = -572058413;    float ICqEhvKxxqHplOOEAnhoLTS71908138 = -425304508;    float ICqEhvKxxqHplOOEAnhoLTS6563256 = -670374164;    float ICqEhvKxxqHplOOEAnhoLTS88212649 = -714405620;    float ICqEhvKxxqHplOOEAnhoLTS94601261 = -431420958;    float ICqEhvKxxqHplOOEAnhoLTS4554921 = -449864799;    float ICqEhvKxxqHplOOEAnhoLTS93449091 = 89891703;    float ICqEhvKxxqHplOOEAnhoLTS57215255 = -870690780;    float ICqEhvKxxqHplOOEAnhoLTS97206500 = -476635790;    float ICqEhvKxxqHplOOEAnhoLTS75136469 = -413177802;    float ICqEhvKxxqHplOOEAnhoLTS55930897 = -151805652;    float ICqEhvKxxqHplOOEAnhoLTS59727687 = -231061848;    float ICqEhvKxxqHplOOEAnhoLTS44182387 = -396145951;    float ICqEhvKxxqHplOOEAnhoLTS43517222 = -979925586;    float ICqEhvKxxqHplOOEAnhoLTS36817296 = -379957675;    float ICqEhvKxxqHplOOEAnhoLTS9706409 = -383828895;    float ICqEhvKxxqHplOOEAnhoLTS86081112 = -527682389;    float ICqEhvKxxqHplOOEAnhoLTS30825122 = -630638938;    float ICqEhvKxxqHplOOEAnhoLTS66575991 = -536479090;    float ICqEhvKxxqHplOOEAnhoLTS16869821 = -800825941;    float ICqEhvKxxqHplOOEAnhoLTS79031185 = -932788029;    float ICqEhvKxxqHplOOEAnhoLTS84493893 = -454226475;    float ICqEhvKxxqHplOOEAnhoLTS63459948 = -654065727;    float ICqEhvKxxqHplOOEAnhoLTS41202806 = -706167407;    float ICqEhvKxxqHplOOEAnhoLTS15116363 = -54400906;    float ICqEhvKxxqHplOOEAnhoLTS25317234 = 90180366;    float ICqEhvKxxqHplOOEAnhoLTS68112140 = -889457035;    float ICqEhvKxxqHplOOEAnhoLTS91820291 = -51755466;    float ICqEhvKxxqHplOOEAnhoLTS98526306 = -46706075;    float ICqEhvKxxqHplOOEAnhoLTS58298810 = -26238086;    float ICqEhvKxxqHplOOEAnhoLTS39991686 = -80376382;    float ICqEhvKxxqHplOOEAnhoLTS59987124 = -892425807;    float ICqEhvKxxqHplOOEAnhoLTS54786446 = -463482920;    float ICqEhvKxxqHplOOEAnhoLTS84026740 = -167627575;    float ICqEhvKxxqHplOOEAnhoLTS85798291 = -643956845;    float ICqEhvKxxqHplOOEAnhoLTS29650966 = -332613517;    float ICqEhvKxxqHplOOEAnhoLTS87979776 = -444926364;    float ICqEhvKxxqHplOOEAnhoLTS74564019 = -639995185;    float ICqEhvKxxqHplOOEAnhoLTS6952551 = -192361673;    float ICqEhvKxxqHplOOEAnhoLTS8177844 = -485671515;    float ICqEhvKxxqHplOOEAnhoLTS64987405 = -802473147;    float ICqEhvKxxqHplOOEAnhoLTS77058993 = -141139699;    float ICqEhvKxxqHplOOEAnhoLTS6811056 = -914784082;    float ICqEhvKxxqHplOOEAnhoLTS24915226 = -171044364;    float ICqEhvKxxqHplOOEAnhoLTS30738179 = -407118705;    float ICqEhvKxxqHplOOEAnhoLTS43845022 = -435707622;    float ICqEhvKxxqHplOOEAnhoLTS12400691 = -255041459;    float ICqEhvKxxqHplOOEAnhoLTS87684915 = 96070638;    float ICqEhvKxxqHplOOEAnhoLTS33129455 = -105688572;    float ICqEhvKxxqHplOOEAnhoLTS73415636 = -827085405;    float ICqEhvKxxqHplOOEAnhoLTS61529384 = -775372961;    float ICqEhvKxxqHplOOEAnhoLTS22953636 = -67290569;    float ICqEhvKxxqHplOOEAnhoLTS70153531 = -716583892;    float ICqEhvKxxqHplOOEAnhoLTS99460979 = -211699196;    float ICqEhvKxxqHplOOEAnhoLTS55319665 = -684473499;    float ICqEhvKxxqHplOOEAnhoLTS54791415 = -17831938;    float ICqEhvKxxqHplOOEAnhoLTS8448191 = -771238782;    float ICqEhvKxxqHplOOEAnhoLTS65360450 = -964206758;    float ICqEhvKxxqHplOOEAnhoLTS73096287 = -560004714;    float ICqEhvKxxqHplOOEAnhoLTS69284027 = -599129423;     ICqEhvKxxqHplOOEAnhoLTS4249441 = ICqEhvKxxqHplOOEAnhoLTS48857781;     ICqEhvKxxqHplOOEAnhoLTS48857781 = ICqEhvKxxqHplOOEAnhoLTS32378935;     ICqEhvKxxqHplOOEAnhoLTS32378935 = ICqEhvKxxqHplOOEAnhoLTS79308755;     ICqEhvKxxqHplOOEAnhoLTS79308755 = ICqEhvKxxqHplOOEAnhoLTS75306318;     ICqEhvKxxqHplOOEAnhoLTS75306318 = ICqEhvKxxqHplOOEAnhoLTS13305434;     ICqEhvKxxqHplOOEAnhoLTS13305434 = ICqEhvKxxqHplOOEAnhoLTS88511450;     ICqEhvKxxqHplOOEAnhoLTS88511450 = ICqEhvKxxqHplOOEAnhoLTS63511643;     ICqEhvKxxqHplOOEAnhoLTS63511643 = ICqEhvKxxqHplOOEAnhoLTS27871882;     ICqEhvKxxqHplOOEAnhoLTS27871882 = ICqEhvKxxqHplOOEAnhoLTS18689072;     ICqEhvKxxqHplOOEAnhoLTS18689072 = ICqEhvKxxqHplOOEAnhoLTS81900311;     ICqEhvKxxqHplOOEAnhoLTS81900311 = ICqEhvKxxqHplOOEAnhoLTS99314278;     ICqEhvKxxqHplOOEAnhoLTS99314278 = ICqEhvKxxqHplOOEAnhoLTS61767042;     ICqEhvKxxqHplOOEAnhoLTS61767042 = ICqEhvKxxqHplOOEAnhoLTS81648351;     ICqEhvKxxqHplOOEAnhoLTS81648351 = ICqEhvKxxqHplOOEAnhoLTS5909306;     ICqEhvKxxqHplOOEAnhoLTS5909306 = ICqEhvKxxqHplOOEAnhoLTS93770444;     ICqEhvKxxqHplOOEAnhoLTS93770444 = ICqEhvKxxqHplOOEAnhoLTS96716647;     ICqEhvKxxqHplOOEAnhoLTS96716647 = ICqEhvKxxqHplOOEAnhoLTS71117246;     ICqEhvKxxqHplOOEAnhoLTS71117246 = ICqEhvKxxqHplOOEAnhoLTS18377591;     ICqEhvKxxqHplOOEAnhoLTS18377591 = ICqEhvKxxqHplOOEAnhoLTS25083600;     ICqEhvKxxqHplOOEAnhoLTS25083600 = ICqEhvKxxqHplOOEAnhoLTS1559105;     ICqEhvKxxqHplOOEAnhoLTS1559105 = ICqEhvKxxqHplOOEAnhoLTS94543031;     ICqEhvKxxqHplOOEAnhoLTS94543031 = ICqEhvKxxqHplOOEAnhoLTS62776669;     ICqEhvKxxqHplOOEAnhoLTS62776669 = ICqEhvKxxqHplOOEAnhoLTS1553813;     ICqEhvKxxqHplOOEAnhoLTS1553813 = ICqEhvKxxqHplOOEAnhoLTS12732764;     ICqEhvKxxqHplOOEAnhoLTS12732764 = ICqEhvKxxqHplOOEAnhoLTS58436497;     ICqEhvKxxqHplOOEAnhoLTS58436497 = ICqEhvKxxqHplOOEAnhoLTS34274248;     ICqEhvKxxqHplOOEAnhoLTS34274248 = ICqEhvKxxqHplOOEAnhoLTS4017557;     ICqEhvKxxqHplOOEAnhoLTS4017557 = ICqEhvKxxqHplOOEAnhoLTS51696;     ICqEhvKxxqHplOOEAnhoLTS51696 = ICqEhvKxxqHplOOEAnhoLTS86669075;     ICqEhvKxxqHplOOEAnhoLTS86669075 = ICqEhvKxxqHplOOEAnhoLTS3572710;     ICqEhvKxxqHplOOEAnhoLTS3572710 = ICqEhvKxxqHplOOEAnhoLTS56583078;     ICqEhvKxxqHplOOEAnhoLTS56583078 = ICqEhvKxxqHplOOEAnhoLTS31202138;     ICqEhvKxxqHplOOEAnhoLTS31202138 = ICqEhvKxxqHplOOEAnhoLTS69946751;     ICqEhvKxxqHplOOEAnhoLTS69946751 = ICqEhvKxxqHplOOEAnhoLTS83122045;     ICqEhvKxxqHplOOEAnhoLTS83122045 = ICqEhvKxxqHplOOEAnhoLTS47610496;     ICqEhvKxxqHplOOEAnhoLTS47610496 = ICqEhvKxxqHplOOEAnhoLTS53778758;     ICqEhvKxxqHplOOEAnhoLTS53778758 = ICqEhvKxxqHplOOEAnhoLTS36729523;     ICqEhvKxxqHplOOEAnhoLTS36729523 = ICqEhvKxxqHplOOEAnhoLTS16330801;     ICqEhvKxxqHplOOEAnhoLTS16330801 = ICqEhvKxxqHplOOEAnhoLTS34350850;     ICqEhvKxxqHplOOEAnhoLTS34350850 = ICqEhvKxxqHplOOEAnhoLTS39285308;     ICqEhvKxxqHplOOEAnhoLTS39285308 = ICqEhvKxxqHplOOEAnhoLTS71908138;     ICqEhvKxxqHplOOEAnhoLTS71908138 = ICqEhvKxxqHplOOEAnhoLTS6563256;     ICqEhvKxxqHplOOEAnhoLTS6563256 = ICqEhvKxxqHplOOEAnhoLTS88212649;     ICqEhvKxxqHplOOEAnhoLTS88212649 = ICqEhvKxxqHplOOEAnhoLTS94601261;     ICqEhvKxxqHplOOEAnhoLTS94601261 = ICqEhvKxxqHplOOEAnhoLTS4554921;     ICqEhvKxxqHplOOEAnhoLTS4554921 = ICqEhvKxxqHplOOEAnhoLTS93449091;     ICqEhvKxxqHplOOEAnhoLTS93449091 = ICqEhvKxxqHplOOEAnhoLTS57215255;     ICqEhvKxxqHplOOEAnhoLTS57215255 = ICqEhvKxxqHplOOEAnhoLTS97206500;     ICqEhvKxxqHplOOEAnhoLTS97206500 = ICqEhvKxxqHplOOEAnhoLTS75136469;     ICqEhvKxxqHplOOEAnhoLTS75136469 = ICqEhvKxxqHplOOEAnhoLTS55930897;     ICqEhvKxxqHplOOEAnhoLTS55930897 = ICqEhvKxxqHplOOEAnhoLTS59727687;     ICqEhvKxxqHplOOEAnhoLTS59727687 = ICqEhvKxxqHplOOEAnhoLTS44182387;     ICqEhvKxxqHplOOEAnhoLTS44182387 = ICqEhvKxxqHplOOEAnhoLTS43517222;     ICqEhvKxxqHplOOEAnhoLTS43517222 = ICqEhvKxxqHplOOEAnhoLTS36817296;     ICqEhvKxxqHplOOEAnhoLTS36817296 = ICqEhvKxxqHplOOEAnhoLTS9706409;     ICqEhvKxxqHplOOEAnhoLTS9706409 = ICqEhvKxxqHplOOEAnhoLTS86081112;     ICqEhvKxxqHplOOEAnhoLTS86081112 = ICqEhvKxxqHplOOEAnhoLTS30825122;     ICqEhvKxxqHplOOEAnhoLTS30825122 = ICqEhvKxxqHplOOEAnhoLTS66575991;     ICqEhvKxxqHplOOEAnhoLTS66575991 = ICqEhvKxxqHplOOEAnhoLTS16869821;     ICqEhvKxxqHplOOEAnhoLTS16869821 = ICqEhvKxxqHplOOEAnhoLTS79031185;     ICqEhvKxxqHplOOEAnhoLTS79031185 = ICqEhvKxxqHplOOEAnhoLTS84493893;     ICqEhvKxxqHplOOEAnhoLTS84493893 = ICqEhvKxxqHplOOEAnhoLTS63459948;     ICqEhvKxxqHplOOEAnhoLTS63459948 = ICqEhvKxxqHplOOEAnhoLTS41202806;     ICqEhvKxxqHplOOEAnhoLTS41202806 = ICqEhvKxxqHplOOEAnhoLTS15116363;     ICqEhvKxxqHplOOEAnhoLTS15116363 = ICqEhvKxxqHplOOEAnhoLTS25317234;     ICqEhvKxxqHplOOEAnhoLTS25317234 = ICqEhvKxxqHplOOEAnhoLTS68112140;     ICqEhvKxxqHplOOEAnhoLTS68112140 = ICqEhvKxxqHplOOEAnhoLTS91820291;     ICqEhvKxxqHplOOEAnhoLTS91820291 = ICqEhvKxxqHplOOEAnhoLTS98526306;     ICqEhvKxxqHplOOEAnhoLTS98526306 = ICqEhvKxxqHplOOEAnhoLTS58298810;     ICqEhvKxxqHplOOEAnhoLTS58298810 = ICqEhvKxxqHplOOEAnhoLTS39991686;     ICqEhvKxxqHplOOEAnhoLTS39991686 = ICqEhvKxxqHplOOEAnhoLTS59987124;     ICqEhvKxxqHplOOEAnhoLTS59987124 = ICqEhvKxxqHplOOEAnhoLTS54786446;     ICqEhvKxxqHplOOEAnhoLTS54786446 = ICqEhvKxxqHplOOEAnhoLTS84026740;     ICqEhvKxxqHplOOEAnhoLTS84026740 = ICqEhvKxxqHplOOEAnhoLTS85798291;     ICqEhvKxxqHplOOEAnhoLTS85798291 = ICqEhvKxxqHplOOEAnhoLTS29650966;     ICqEhvKxxqHplOOEAnhoLTS29650966 = ICqEhvKxxqHplOOEAnhoLTS87979776;     ICqEhvKxxqHplOOEAnhoLTS87979776 = ICqEhvKxxqHplOOEAnhoLTS74564019;     ICqEhvKxxqHplOOEAnhoLTS74564019 = ICqEhvKxxqHplOOEAnhoLTS6952551;     ICqEhvKxxqHplOOEAnhoLTS6952551 = ICqEhvKxxqHplOOEAnhoLTS8177844;     ICqEhvKxxqHplOOEAnhoLTS8177844 = ICqEhvKxxqHplOOEAnhoLTS64987405;     ICqEhvKxxqHplOOEAnhoLTS64987405 = ICqEhvKxxqHplOOEAnhoLTS77058993;     ICqEhvKxxqHplOOEAnhoLTS77058993 = ICqEhvKxxqHplOOEAnhoLTS6811056;     ICqEhvKxxqHplOOEAnhoLTS6811056 = ICqEhvKxxqHplOOEAnhoLTS24915226;     ICqEhvKxxqHplOOEAnhoLTS24915226 = ICqEhvKxxqHplOOEAnhoLTS30738179;     ICqEhvKxxqHplOOEAnhoLTS30738179 = ICqEhvKxxqHplOOEAnhoLTS43845022;     ICqEhvKxxqHplOOEAnhoLTS43845022 = ICqEhvKxxqHplOOEAnhoLTS12400691;     ICqEhvKxxqHplOOEAnhoLTS12400691 = ICqEhvKxxqHplOOEAnhoLTS87684915;     ICqEhvKxxqHplOOEAnhoLTS87684915 = ICqEhvKxxqHplOOEAnhoLTS33129455;     ICqEhvKxxqHplOOEAnhoLTS33129455 = ICqEhvKxxqHplOOEAnhoLTS73415636;     ICqEhvKxxqHplOOEAnhoLTS73415636 = ICqEhvKxxqHplOOEAnhoLTS61529384;     ICqEhvKxxqHplOOEAnhoLTS61529384 = ICqEhvKxxqHplOOEAnhoLTS22953636;     ICqEhvKxxqHplOOEAnhoLTS22953636 = ICqEhvKxxqHplOOEAnhoLTS70153531;     ICqEhvKxxqHplOOEAnhoLTS70153531 = ICqEhvKxxqHplOOEAnhoLTS99460979;     ICqEhvKxxqHplOOEAnhoLTS99460979 = ICqEhvKxxqHplOOEAnhoLTS55319665;     ICqEhvKxxqHplOOEAnhoLTS55319665 = ICqEhvKxxqHplOOEAnhoLTS54791415;     ICqEhvKxxqHplOOEAnhoLTS54791415 = ICqEhvKxxqHplOOEAnhoLTS8448191;     ICqEhvKxxqHplOOEAnhoLTS8448191 = ICqEhvKxxqHplOOEAnhoLTS65360450;     ICqEhvKxxqHplOOEAnhoLTS65360450 = ICqEhvKxxqHplOOEAnhoLTS73096287;     ICqEhvKxxqHplOOEAnhoLTS73096287 = ICqEhvKxxqHplOOEAnhoLTS69284027;     ICqEhvKxxqHplOOEAnhoLTS69284027 = ICqEhvKxxqHplOOEAnhoLTS4249441;}



void VonnPqRKsyjZANjqLJszeUyfEy23439238() {     float OnIwtStCYUIVJdgZTPyHPsa87972958 = -949151490;    float OnIwtStCYUIVJdgZTPyHPsa46455876 = -785735043;    float OnIwtStCYUIVJdgZTPyHPsa20965052 = -222729259;    float OnIwtStCYUIVJdgZTPyHPsa76456708 = -576897332;    float OnIwtStCYUIVJdgZTPyHPsa54867654 = 88644425;    float OnIwtStCYUIVJdgZTPyHPsa24644951 = -47939176;    float OnIwtStCYUIVJdgZTPyHPsa25392013 = -967160484;    float OnIwtStCYUIVJdgZTPyHPsa6851726 = -958742478;    float OnIwtStCYUIVJdgZTPyHPsa78091409 = 37425541;    float OnIwtStCYUIVJdgZTPyHPsa64794660 = -44211391;    float OnIwtStCYUIVJdgZTPyHPsa24789348 = -601292301;    float OnIwtStCYUIVJdgZTPyHPsa89526049 = -802451252;    float OnIwtStCYUIVJdgZTPyHPsa12820894 = -580233956;    float OnIwtStCYUIVJdgZTPyHPsa76021496 = -707303236;    float OnIwtStCYUIVJdgZTPyHPsa10088564 = -354990740;    float OnIwtStCYUIVJdgZTPyHPsa65584473 = -902433540;    float OnIwtStCYUIVJdgZTPyHPsa96582058 = -304368651;    float OnIwtStCYUIVJdgZTPyHPsa85194515 = -186859631;    float OnIwtStCYUIVJdgZTPyHPsa59354071 = -673538341;    float OnIwtStCYUIVJdgZTPyHPsa99191244 = -932308174;    float OnIwtStCYUIVJdgZTPyHPsa68898614 = -432384545;    float OnIwtStCYUIVJdgZTPyHPsa95545103 = -28455814;    float OnIwtStCYUIVJdgZTPyHPsa31952534 = -383795540;    float OnIwtStCYUIVJdgZTPyHPsa2696508 = -263015283;    float OnIwtStCYUIVJdgZTPyHPsa49334857 = -13867076;    float OnIwtStCYUIVJdgZTPyHPsa11429255 = -155548471;    float OnIwtStCYUIVJdgZTPyHPsa55866187 = -455705902;    float OnIwtStCYUIVJdgZTPyHPsa46616757 = -177861360;    float OnIwtStCYUIVJdgZTPyHPsa71969837 = -108418066;    float OnIwtStCYUIVJdgZTPyHPsa22217403 = -231530731;    float OnIwtStCYUIVJdgZTPyHPsa84937132 = -985867490;    float OnIwtStCYUIVJdgZTPyHPsa81376366 = -200384632;    float OnIwtStCYUIVJdgZTPyHPsa45326839 = -549069038;    float OnIwtStCYUIVJdgZTPyHPsa5671465 = -989742894;    float OnIwtStCYUIVJdgZTPyHPsa50725370 = 54795093;    float OnIwtStCYUIVJdgZTPyHPsa17471730 = -281451827;    float OnIwtStCYUIVJdgZTPyHPsa53542348 = -18270804;    float OnIwtStCYUIVJdgZTPyHPsa87111314 = -152206198;    float OnIwtStCYUIVJdgZTPyHPsa76946519 = -318084018;    float OnIwtStCYUIVJdgZTPyHPsa73539965 = -563908407;    float OnIwtStCYUIVJdgZTPyHPsa4052829 = -222098539;    float OnIwtStCYUIVJdgZTPyHPsa1247865 = -31330178;    float OnIwtStCYUIVJdgZTPyHPsa39363402 = -322778419;    float OnIwtStCYUIVJdgZTPyHPsa35408169 = -293349352;    float OnIwtStCYUIVJdgZTPyHPsa22588513 = -757435036;    float OnIwtStCYUIVJdgZTPyHPsa48821802 = -64896491;    float OnIwtStCYUIVJdgZTPyHPsa26628062 = 68245796;    float OnIwtStCYUIVJdgZTPyHPsa95957102 = -632812678;    float OnIwtStCYUIVJdgZTPyHPsa31080714 = -176419941;    float OnIwtStCYUIVJdgZTPyHPsa11009800 = 54806763;    float OnIwtStCYUIVJdgZTPyHPsa96021728 = -317578053;    float OnIwtStCYUIVJdgZTPyHPsa11233407 = 16885366;    float OnIwtStCYUIVJdgZTPyHPsa89564789 = -408078317;    float OnIwtStCYUIVJdgZTPyHPsa89449828 = -456634773;    float OnIwtStCYUIVJdgZTPyHPsa54540623 = -696684619;    float OnIwtStCYUIVJdgZTPyHPsa92427855 = -820695676;    float OnIwtStCYUIVJdgZTPyHPsa14503343 = -301939503;    float OnIwtStCYUIVJdgZTPyHPsa18268545 = -959713976;    float OnIwtStCYUIVJdgZTPyHPsa27121852 = -463030257;    float OnIwtStCYUIVJdgZTPyHPsa43438399 = -755807105;    float OnIwtStCYUIVJdgZTPyHPsa68778763 = -592233275;    float OnIwtStCYUIVJdgZTPyHPsa78775256 = -689299124;    float OnIwtStCYUIVJdgZTPyHPsa34881889 = -750324413;    float OnIwtStCYUIVJdgZTPyHPsa55874007 = -731043729;    float OnIwtStCYUIVJdgZTPyHPsa79857527 = -58343901;    float OnIwtStCYUIVJdgZTPyHPsa43412982 = -300907669;    float OnIwtStCYUIVJdgZTPyHPsa44199210 = -153382214;    float OnIwtStCYUIVJdgZTPyHPsa7149429 = -590491062;    float OnIwtStCYUIVJdgZTPyHPsa25296126 = -662098330;    float OnIwtStCYUIVJdgZTPyHPsa92616834 = 26461086;    float OnIwtStCYUIVJdgZTPyHPsa12042125 = -784162737;    float OnIwtStCYUIVJdgZTPyHPsa9470745 = -52162453;    float OnIwtStCYUIVJdgZTPyHPsa8247997 = -868775613;    float OnIwtStCYUIVJdgZTPyHPsa85814105 = -9629934;    float OnIwtStCYUIVJdgZTPyHPsa95138415 = -610209635;    float OnIwtStCYUIVJdgZTPyHPsa67650749 = -301054368;    float OnIwtStCYUIVJdgZTPyHPsa56181701 = -705677396;    float OnIwtStCYUIVJdgZTPyHPsa96544365 = 9553811;    float OnIwtStCYUIVJdgZTPyHPsa80107994 = -505580248;    float OnIwtStCYUIVJdgZTPyHPsa513055 = -948970585;    float OnIwtStCYUIVJdgZTPyHPsa84801193 = -123794268;    float OnIwtStCYUIVJdgZTPyHPsa59909085 = -822893224;    float OnIwtStCYUIVJdgZTPyHPsa15536043 = 98558580;    float OnIwtStCYUIVJdgZTPyHPsa60960037 = -63224829;    float OnIwtStCYUIVJdgZTPyHPsa26195675 = -913952678;    float OnIwtStCYUIVJdgZTPyHPsa73703725 = -902752857;    float OnIwtStCYUIVJdgZTPyHPsa91811576 = -792306316;    float OnIwtStCYUIVJdgZTPyHPsa55877011 = 7565735;    float OnIwtStCYUIVJdgZTPyHPsa51130842 = -193058275;    float OnIwtStCYUIVJdgZTPyHPsa58297515 = -124509231;    float OnIwtStCYUIVJdgZTPyHPsa2968388 = -979512324;    float OnIwtStCYUIVJdgZTPyHPsa35273804 = -58556828;    float OnIwtStCYUIVJdgZTPyHPsa59989462 = -689175941;    float OnIwtStCYUIVJdgZTPyHPsa33508120 = -562276914;    float OnIwtStCYUIVJdgZTPyHPsa4761203 = -971675133;    float OnIwtStCYUIVJdgZTPyHPsa25277573 = -532799415;    float OnIwtStCYUIVJdgZTPyHPsa66365976 = -281005765;    float OnIwtStCYUIVJdgZTPyHPsa83489395 = -591734690;    float OnIwtStCYUIVJdgZTPyHPsa55550641 = -135005451;    float OnIwtStCYUIVJdgZTPyHPsa79175530 = -949151490;     OnIwtStCYUIVJdgZTPyHPsa87972958 = OnIwtStCYUIVJdgZTPyHPsa46455876;     OnIwtStCYUIVJdgZTPyHPsa46455876 = OnIwtStCYUIVJdgZTPyHPsa20965052;     OnIwtStCYUIVJdgZTPyHPsa20965052 = OnIwtStCYUIVJdgZTPyHPsa76456708;     OnIwtStCYUIVJdgZTPyHPsa76456708 = OnIwtStCYUIVJdgZTPyHPsa54867654;     OnIwtStCYUIVJdgZTPyHPsa54867654 = OnIwtStCYUIVJdgZTPyHPsa24644951;     OnIwtStCYUIVJdgZTPyHPsa24644951 = OnIwtStCYUIVJdgZTPyHPsa25392013;     OnIwtStCYUIVJdgZTPyHPsa25392013 = OnIwtStCYUIVJdgZTPyHPsa6851726;     OnIwtStCYUIVJdgZTPyHPsa6851726 = OnIwtStCYUIVJdgZTPyHPsa78091409;     OnIwtStCYUIVJdgZTPyHPsa78091409 = OnIwtStCYUIVJdgZTPyHPsa64794660;     OnIwtStCYUIVJdgZTPyHPsa64794660 = OnIwtStCYUIVJdgZTPyHPsa24789348;     OnIwtStCYUIVJdgZTPyHPsa24789348 = OnIwtStCYUIVJdgZTPyHPsa89526049;     OnIwtStCYUIVJdgZTPyHPsa89526049 = OnIwtStCYUIVJdgZTPyHPsa12820894;     OnIwtStCYUIVJdgZTPyHPsa12820894 = OnIwtStCYUIVJdgZTPyHPsa76021496;     OnIwtStCYUIVJdgZTPyHPsa76021496 = OnIwtStCYUIVJdgZTPyHPsa10088564;     OnIwtStCYUIVJdgZTPyHPsa10088564 = OnIwtStCYUIVJdgZTPyHPsa65584473;     OnIwtStCYUIVJdgZTPyHPsa65584473 = OnIwtStCYUIVJdgZTPyHPsa96582058;     OnIwtStCYUIVJdgZTPyHPsa96582058 = OnIwtStCYUIVJdgZTPyHPsa85194515;     OnIwtStCYUIVJdgZTPyHPsa85194515 = OnIwtStCYUIVJdgZTPyHPsa59354071;     OnIwtStCYUIVJdgZTPyHPsa59354071 = OnIwtStCYUIVJdgZTPyHPsa99191244;     OnIwtStCYUIVJdgZTPyHPsa99191244 = OnIwtStCYUIVJdgZTPyHPsa68898614;     OnIwtStCYUIVJdgZTPyHPsa68898614 = OnIwtStCYUIVJdgZTPyHPsa95545103;     OnIwtStCYUIVJdgZTPyHPsa95545103 = OnIwtStCYUIVJdgZTPyHPsa31952534;     OnIwtStCYUIVJdgZTPyHPsa31952534 = OnIwtStCYUIVJdgZTPyHPsa2696508;     OnIwtStCYUIVJdgZTPyHPsa2696508 = OnIwtStCYUIVJdgZTPyHPsa49334857;     OnIwtStCYUIVJdgZTPyHPsa49334857 = OnIwtStCYUIVJdgZTPyHPsa11429255;     OnIwtStCYUIVJdgZTPyHPsa11429255 = OnIwtStCYUIVJdgZTPyHPsa55866187;     OnIwtStCYUIVJdgZTPyHPsa55866187 = OnIwtStCYUIVJdgZTPyHPsa46616757;     OnIwtStCYUIVJdgZTPyHPsa46616757 = OnIwtStCYUIVJdgZTPyHPsa71969837;     OnIwtStCYUIVJdgZTPyHPsa71969837 = OnIwtStCYUIVJdgZTPyHPsa22217403;     OnIwtStCYUIVJdgZTPyHPsa22217403 = OnIwtStCYUIVJdgZTPyHPsa84937132;     OnIwtStCYUIVJdgZTPyHPsa84937132 = OnIwtStCYUIVJdgZTPyHPsa81376366;     OnIwtStCYUIVJdgZTPyHPsa81376366 = OnIwtStCYUIVJdgZTPyHPsa45326839;     OnIwtStCYUIVJdgZTPyHPsa45326839 = OnIwtStCYUIVJdgZTPyHPsa5671465;     OnIwtStCYUIVJdgZTPyHPsa5671465 = OnIwtStCYUIVJdgZTPyHPsa50725370;     OnIwtStCYUIVJdgZTPyHPsa50725370 = OnIwtStCYUIVJdgZTPyHPsa17471730;     OnIwtStCYUIVJdgZTPyHPsa17471730 = OnIwtStCYUIVJdgZTPyHPsa53542348;     OnIwtStCYUIVJdgZTPyHPsa53542348 = OnIwtStCYUIVJdgZTPyHPsa87111314;     OnIwtStCYUIVJdgZTPyHPsa87111314 = OnIwtStCYUIVJdgZTPyHPsa76946519;     OnIwtStCYUIVJdgZTPyHPsa76946519 = OnIwtStCYUIVJdgZTPyHPsa73539965;     OnIwtStCYUIVJdgZTPyHPsa73539965 = OnIwtStCYUIVJdgZTPyHPsa4052829;     OnIwtStCYUIVJdgZTPyHPsa4052829 = OnIwtStCYUIVJdgZTPyHPsa1247865;     OnIwtStCYUIVJdgZTPyHPsa1247865 = OnIwtStCYUIVJdgZTPyHPsa39363402;     OnIwtStCYUIVJdgZTPyHPsa39363402 = OnIwtStCYUIVJdgZTPyHPsa35408169;     OnIwtStCYUIVJdgZTPyHPsa35408169 = OnIwtStCYUIVJdgZTPyHPsa22588513;     OnIwtStCYUIVJdgZTPyHPsa22588513 = OnIwtStCYUIVJdgZTPyHPsa48821802;     OnIwtStCYUIVJdgZTPyHPsa48821802 = OnIwtStCYUIVJdgZTPyHPsa26628062;     OnIwtStCYUIVJdgZTPyHPsa26628062 = OnIwtStCYUIVJdgZTPyHPsa95957102;     OnIwtStCYUIVJdgZTPyHPsa95957102 = OnIwtStCYUIVJdgZTPyHPsa31080714;     OnIwtStCYUIVJdgZTPyHPsa31080714 = OnIwtStCYUIVJdgZTPyHPsa11009800;     OnIwtStCYUIVJdgZTPyHPsa11009800 = OnIwtStCYUIVJdgZTPyHPsa96021728;     OnIwtStCYUIVJdgZTPyHPsa96021728 = OnIwtStCYUIVJdgZTPyHPsa11233407;     OnIwtStCYUIVJdgZTPyHPsa11233407 = OnIwtStCYUIVJdgZTPyHPsa89564789;     OnIwtStCYUIVJdgZTPyHPsa89564789 = OnIwtStCYUIVJdgZTPyHPsa89449828;     OnIwtStCYUIVJdgZTPyHPsa89449828 = OnIwtStCYUIVJdgZTPyHPsa54540623;     OnIwtStCYUIVJdgZTPyHPsa54540623 = OnIwtStCYUIVJdgZTPyHPsa92427855;     OnIwtStCYUIVJdgZTPyHPsa92427855 = OnIwtStCYUIVJdgZTPyHPsa14503343;     OnIwtStCYUIVJdgZTPyHPsa14503343 = OnIwtStCYUIVJdgZTPyHPsa18268545;     OnIwtStCYUIVJdgZTPyHPsa18268545 = OnIwtStCYUIVJdgZTPyHPsa27121852;     OnIwtStCYUIVJdgZTPyHPsa27121852 = OnIwtStCYUIVJdgZTPyHPsa43438399;     OnIwtStCYUIVJdgZTPyHPsa43438399 = OnIwtStCYUIVJdgZTPyHPsa68778763;     OnIwtStCYUIVJdgZTPyHPsa68778763 = OnIwtStCYUIVJdgZTPyHPsa78775256;     OnIwtStCYUIVJdgZTPyHPsa78775256 = OnIwtStCYUIVJdgZTPyHPsa34881889;     OnIwtStCYUIVJdgZTPyHPsa34881889 = OnIwtStCYUIVJdgZTPyHPsa55874007;     OnIwtStCYUIVJdgZTPyHPsa55874007 = OnIwtStCYUIVJdgZTPyHPsa79857527;     OnIwtStCYUIVJdgZTPyHPsa79857527 = OnIwtStCYUIVJdgZTPyHPsa43412982;     OnIwtStCYUIVJdgZTPyHPsa43412982 = OnIwtStCYUIVJdgZTPyHPsa44199210;     OnIwtStCYUIVJdgZTPyHPsa44199210 = OnIwtStCYUIVJdgZTPyHPsa7149429;     OnIwtStCYUIVJdgZTPyHPsa7149429 = OnIwtStCYUIVJdgZTPyHPsa25296126;     OnIwtStCYUIVJdgZTPyHPsa25296126 = OnIwtStCYUIVJdgZTPyHPsa92616834;     OnIwtStCYUIVJdgZTPyHPsa92616834 = OnIwtStCYUIVJdgZTPyHPsa12042125;     OnIwtStCYUIVJdgZTPyHPsa12042125 = OnIwtStCYUIVJdgZTPyHPsa9470745;     OnIwtStCYUIVJdgZTPyHPsa9470745 = OnIwtStCYUIVJdgZTPyHPsa8247997;     OnIwtStCYUIVJdgZTPyHPsa8247997 = OnIwtStCYUIVJdgZTPyHPsa85814105;     OnIwtStCYUIVJdgZTPyHPsa85814105 = OnIwtStCYUIVJdgZTPyHPsa95138415;     OnIwtStCYUIVJdgZTPyHPsa95138415 = OnIwtStCYUIVJdgZTPyHPsa67650749;     OnIwtStCYUIVJdgZTPyHPsa67650749 = OnIwtStCYUIVJdgZTPyHPsa56181701;     OnIwtStCYUIVJdgZTPyHPsa56181701 = OnIwtStCYUIVJdgZTPyHPsa96544365;     OnIwtStCYUIVJdgZTPyHPsa96544365 = OnIwtStCYUIVJdgZTPyHPsa80107994;     OnIwtStCYUIVJdgZTPyHPsa80107994 = OnIwtStCYUIVJdgZTPyHPsa513055;     OnIwtStCYUIVJdgZTPyHPsa513055 = OnIwtStCYUIVJdgZTPyHPsa84801193;     OnIwtStCYUIVJdgZTPyHPsa84801193 = OnIwtStCYUIVJdgZTPyHPsa59909085;     OnIwtStCYUIVJdgZTPyHPsa59909085 = OnIwtStCYUIVJdgZTPyHPsa15536043;     OnIwtStCYUIVJdgZTPyHPsa15536043 = OnIwtStCYUIVJdgZTPyHPsa60960037;     OnIwtStCYUIVJdgZTPyHPsa60960037 = OnIwtStCYUIVJdgZTPyHPsa26195675;     OnIwtStCYUIVJdgZTPyHPsa26195675 = OnIwtStCYUIVJdgZTPyHPsa73703725;     OnIwtStCYUIVJdgZTPyHPsa73703725 = OnIwtStCYUIVJdgZTPyHPsa91811576;     OnIwtStCYUIVJdgZTPyHPsa91811576 = OnIwtStCYUIVJdgZTPyHPsa55877011;     OnIwtStCYUIVJdgZTPyHPsa55877011 = OnIwtStCYUIVJdgZTPyHPsa51130842;     OnIwtStCYUIVJdgZTPyHPsa51130842 = OnIwtStCYUIVJdgZTPyHPsa58297515;     OnIwtStCYUIVJdgZTPyHPsa58297515 = OnIwtStCYUIVJdgZTPyHPsa2968388;     OnIwtStCYUIVJdgZTPyHPsa2968388 = OnIwtStCYUIVJdgZTPyHPsa35273804;     OnIwtStCYUIVJdgZTPyHPsa35273804 = OnIwtStCYUIVJdgZTPyHPsa59989462;     OnIwtStCYUIVJdgZTPyHPsa59989462 = OnIwtStCYUIVJdgZTPyHPsa33508120;     OnIwtStCYUIVJdgZTPyHPsa33508120 = OnIwtStCYUIVJdgZTPyHPsa4761203;     OnIwtStCYUIVJdgZTPyHPsa4761203 = OnIwtStCYUIVJdgZTPyHPsa25277573;     OnIwtStCYUIVJdgZTPyHPsa25277573 = OnIwtStCYUIVJdgZTPyHPsa66365976;     OnIwtStCYUIVJdgZTPyHPsa66365976 = OnIwtStCYUIVJdgZTPyHPsa83489395;     OnIwtStCYUIVJdgZTPyHPsa83489395 = OnIwtStCYUIVJdgZTPyHPsa55550641;     OnIwtStCYUIVJdgZTPyHPsa55550641 = OnIwtStCYUIVJdgZTPyHPsa79175530;     OnIwtStCYUIVJdgZTPyHPsa79175530 = OnIwtStCYUIVJdgZTPyHPsa87972958;}



void GGqnqDrMlMYWGdubazHDDtujft8502076() {     float pEkRKHxlksfXqDkIYPmfnNg71696476 = -199173557;    float pEkRKHxlksfXqDkIYPmfnNg44053972 = -589386894;    float pEkRKHxlksfXqDkIYPmfnNg9551169 = -91036949;    float pEkRKHxlksfXqDkIYPmfnNg73604661 = -581779261;    float pEkRKHxlksfXqDkIYPmfnNg34428991 = -309303767;    float pEkRKHxlksfXqDkIYPmfnNg35984469 = -151259845;    float pEkRKHxlksfXqDkIYPmfnNg62272576 = -988674622;    float pEkRKHxlksfXqDkIYPmfnNg50191808 = -479197064;    float pEkRKHxlksfXqDkIYPmfnNg28310938 = -660057156;    float pEkRKHxlksfXqDkIYPmfnNg10900249 = -267252407;    float pEkRKHxlksfXqDkIYPmfnNg67678383 = -441577559;    float pEkRKHxlksfXqDkIYPmfnNg79737819 = -731590523;    float pEkRKHxlksfXqDkIYPmfnNg63874745 = -423066199;    float pEkRKHxlksfXqDkIYPmfnNg70394640 = 43013901;    float pEkRKHxlksfXqDkIYPmfnNg14267823 = -280688045;    float pEkRKHxlksfXqDkIYPmfnNg37398501 = -826561192;    float pEkRKHxlksfXqDkIYPmfnNg96447469 = -463248513;    float pEkRKHxlksfXqDkIYPmfnNg99271785 = -897711205;    float pEkRKHxlksfXqDkIYPmfnNg330552 = -462187579;    float pEkRKHxlksfXqDkIYPmfnNg73298888 = -548601091;    float pEkRKHxlksfXqDkIYPmfnNg36238125 = -6851066;    float pEkRKHxlksfXqDkIYPmfnNg96547174 = 58388900;    float pEkRKHxlksfXqDkIYPmfnNg1128400 = -413190277;    float pEkRKHxlksfXqDkIYPmfnNg3839203 = -902247936;    float pEkRKHxlksfXqDkIYPmfnNg85936949 = -92197837;    float pEkRKHxlksfXqDkIYPmfnNg64422013 = -598515499;    float pEkRKHxlksfXqDkIYPmfnNg77458127 = -899581325;    float pEkRKHxlksfXqDkIYPmfnNg89215956 = 35697151;    float pEkRKHxlksfXqDkIYPmfnNg43887978 = -632613966;    float pEkRKHxlksfXqDkIYPmfnNg57765730 = -904137106;    float pEkRKHxlksfXqDkIYPmfnNg66301555 = -104965511;    float pEkRKHxlksfXqDkIYPmfnNg6169655 = -749581855;    float pEkRKHxlksfXqDkIYPmfnNg59451540 = -114283129;    float pEkRKHxlksfXqDkIYPmfnNg41396179 = -293839542;    float pEkRKHxlksfXqDkIYPmfnNg18328696 = -779495515;    float pEkRKHxlksfXqDkIYPmfnNg87332963 = -259848304;    float pEkRKHxlksfXqDkIYPmfnNg53305938 = -338612100;    float pEkRKHxlksfXqDkIYPmfnNg37493105 = -51349414;    float pEkRKHxlksfXqDkIYPmfnNg37562239 = -623642899;    float pEkRKHxlksfXqDkIYPmfnNg12729082 = -510555286;    float pEkRKHxlksfXqDkIYPmfnNg68820349 = -972138666;    float pEkRKHxlksfXqDkIYPmfnNg30587592 = -737355847;    float pEkRKHxlksfXqDkIYPmfnNg72163548 = 24817327;    float pEkRKHxlksfXqDkIYPmfnNg82603688 = -972293085;    float pEkRKHxlksfXqDkIYPmfnNg50575763 = 16550886;    float pEkRKHxlksfXqDkIYPmfnNg93088683 = -779928183;    float pEkRKHxlksfXqDkIYPmfnNg59807031 = 46599889;    float pEkRKHxlksfXqDkIYPmfnNg34698950 = -394934576;    float pEkRKHxlksfXqDkIYPmfnNg64954927 = -976204092;    float pEkRKHxlksfXqDkIYPmfnNg46883130 = -577208673;    float pEkRKHxlksfXqDkIYPmfnNg36112560 = -483350455;    float pEkRKHxlksfXqDkIYPmfnNg62739127 = -835167421;    float pEkRKHxlksfXqDkIYPmfnNg34947192 = -420010683;    float pEkRKHxlksfXqDkIYPmfnNg35382434 = 66656039;    float pEkRKHxlksfXqDkIYPmfnNg72263951 = 86588436;    float pEkRKHxlksfXqDkIYPmfnNg75149302 = -157562457;    float pEkRKHxlksfXqDkIYPmfnNg42925573 = -76196618;    float pEkRKHxlksfXqDkIYPmfnNg5711967 = -188789014;    float pEkRKHxlksfXqDkIYPmfnNg87667712 = -389581424;    float pEkRKHxlksfXqDkIYPmfnNg70006978 = -710788269;    float pEkRKHxlksfXqDkIYPmfnNg58526342 = -251678520;    float pEkRKHxlksfXqDkIYPmfnNg73056619 = -924371774;    float pEkRKHxlksfXqDkIYPmfnNg6303831 = -846583099;    float pEkRKHxlksfXqDkIYPmfnNg70545208 = -755920051;    float pEkRKHxlksfXqDkIYPmfnNg44598693 = -62286896;    float pEkRKHxlksfXqDkIYPmfnNg61508729 = -691995705;    float pEkRKHxlksfXqDkIYPmfnNg20286280 = -517307394;    float pEkRKHxlksfXqDkIYPmfnNg22478567 = -29226658;    float pEkRKHxlksfXqDkIYPmfnNg52065945 = -177490585;    float pEkRKHxlksfXqDkIYPmfnNg26934860 = 79160259;    float pEkRKHxlksfXqDkIYPmfnNg84092563 = -387949092;    float pEkRKHxlksfXqDkIYPmfnNg58954364 = -311899100;    float pEkRKHxlksfXqDkIYPmfnNg61709547 = -174068306;    float pEkRKHxlksfXqDkIYPmfnNg87601470 = -951632293;    float pEkRKHxlksfXqDkIYPmfnNg4478539 = -576462426;    float pEkRKHxlksfXqDkIYPmfnNg5650534 = -269495220;    float pEkRKHxlksfXqDkIYPmfnNg24383626 = -966428428;    float pEkRKHxlksfXqDkIYPmfnNg18524711 = -440897193;    float pEkRKHxlksfXqDkIYPmfnNg53263439 = -818798823;    float pEkRKHxlksfXqDkIYPmfnNg92848266 = -312269655;    float pEkRKHxlksfXqDkIYPmfnNg4614983 = -545115388;    float pEkRKHxlksfXqDkIYPmfnNg42759178 = -404646750;    float pEkRKHxlksfXqDkIYPmfnNg24261030 = 11901243;    float pEkRKHxlksfXqDkIYPmfnNg97004848 = 44594706;    float pEkRKHxlksfXqDkIYPmfnNg21653170 = -320786651;    float pEkRKHxlksfXqDkIYPmfnNg3562429 = -269798091;    float pEkRKHxlksfXqDkIYPmfnNg71222462 = -229571173;    float pEkRKHxlksfXqDkIYPmfnNg24069107 = -80939169;    float pEkRKHxlksfXqDkIYPmfnNg69132228 = -280427978;    float pEkRKHxlksfXqDkIYPmfnNg43179394 = -521933058;    float pEkRKHxlksfXqDkIYPmfnNg44407391 = -83651687;    float pEkRKHxlksfXqDkIYPmfnNg47593972 = -49823087;    float pEkRKHxlksfXqDkIYPmfnNg49825393 = -661767991;    float pEkRKHxlksfXqDkIYPmfnNg67555260 = -912854631;    float pEkRKHxlksfXqDkIYPmfnNg54202740 = -158876766;    float pEkRKHxlksfXqDkIYPmfnNg95763730 = 52233108;    float pEkRKHxlksfXqDkIYPmfnNg24283762 = -890772749;    float pEkRKHxlksfXqDkIYPmfnNg1618340 = -219262623;    float pEkRKHxlksfXqDkIYPmfnNg38004996 = -810006189;    float pEkRKHxlksfXqDkIYPmfnNg89067033 = -199173557;     pEkRKHxlksfXqDkIYPmfnNg71696476 = pEkRKHxlksfXqDkIYPmfnNg44053972;     pEkRKHxlksfXqDkIYPmfnNg44053972 = pEkRKHxlksfXqDkIYPmfnNg9551169;     pEkRKHxlksfXqDkIYPmfnNg9551169 = pEkRKHxlksfXqDkIYPmfnNg73604661;     pEkRKHxlksfXqDkIYPmfnNg73604661 = pEkRKHxlksfXqDkIYPmfnNg34428991;     pEkRKHxlksfXqDkIYPmfnNg34428991 = pEkRKHxlksfXqDkIYPmfnNg35984469;     pEkRKHxlksfXqDkIYPmfnNg35984469 = pEkRKHxlksfXqDkIYPmfnNg62272576;     pEkRKHxlksfXqDkIYPmfnNg62272576 = pEkRKHxlksfXqDkIYPmfnNg50191808;     pEkRKHxlksfXqDkIYPmfnNg50191808 = pEkRKHxlksfXqDkIYPmfnNg28310938;     pEkRKHxlksfXqDkIYPmfnNg28310938 = pEkRKHxlksfXqDkIYPmfnNg10900249;     pEkRKHxlksfXqDkIYPmfnNg10900249 = pEkRKHxlksfXqDkIYPmfnNg67678383;     pEkRKHxlksfXqDkIYPmfnNg67678383 = pEkRKHxlksfXqDkIYPmfnNg79737819;     pEkRKHxlksfXqDkIYPmfnNg79737819 = pEkRKHxlksfXqDkIYPmfnNg63874745;     pEkRKHxlksfXqDkIYPmfnNg63874745 = pEkRKHxlksfXqDkIYPmfnNg70394640;     pEkRKHxlksfXqDkIYPmfnNg70394640 = pEkRKHxlksfXqDkIYPmfnNg14267823;     pEkRKHxlksfXqDkIYPmfnNg14267823 = pEkRKHxlksfXqDkIYPmfnNg37398501;     pEkRKHxlksfXqDkIYPmfnNg37398501 = pEkRKHxlksfXqDkIYPmfnNg96447469;     pEkRKHxlksfXqDkIYPmfnNg96447469 = pEkRKHxlksfXqDkIYPmfnNg99271785;     pEkRKHxlksfXqDkIYPmfnNg99271785 = pEkRKHxlksfXqDkIYPmfnNg330552;     pEkRKHxlksfXqDkIYPmfnNg330552 = pEkRKHxlksfXqDkIYPmfnNg73298888;     pEkRKHxlksfXqDkIYPmfnNg73298888 = pEkRKHxlksfXqDkIYPmfnNg36238125;     pEkRKHxlksfXqDkIYPmfnNg36238125 = pEkRKHxlksfXqDkIYPmfnNg96547174;     pEkRKHxlksfXqDkIYPmfnNg96547174 = pEkRKHxlksfXqDkIYPmfnNg1128400;     pEkRKHxlksfXqDkIYPmfnNg1128400 = pEkRKHxlksfXqDkIYPmfnNg3839203;     pEkRKHxlksfXqDkIYPmfnNg3839203 = pEkRKHxlksfXqDkIYPmfnNg85936949;     pEkRKHxlksfXqDkIYPmfnNg85936949 = pEkRKHxlksfXqDkIYPmfnNg64422013;     pEkRKHxlksfXqDkIYPmfnNg64422013 = pEkRKHxlksfXqDkIYPmfnNg77458127;     pEkRKHxlksfXqDkIYPmfnNg77458127 = pEkRKHxlksfXqDkIYPmfnNg89215956;     pEkRKHxlksfXqDkIYPmfnNg89215956 = pEkRKHxlksfXqDkIYPmfnNg43887978;     pEkRKHxlksfXqDkIYPmfnNg43887978 = pEkRKHxlksfXqDkIYPmfnNg57765730;     pEkRKHxlksfXqDkIYPmfnNg57765730 = pEkRKHxlksfXqDkIYPmfnNg66301555;     pEkRKHxlksfXqDkIYPmfnNg66301555 = pEkRKHxlksfXqDkIYPmfnNg6169655;     pEkRKHxlksfXqDkIYPmfnNg6169655 = pEkRKHxlksfXqDkIYPmfnNg59451540;     pEkRKHxlksfXqDkIYPmfnNg59451540 = pEkRKHxlksfXqDkIYPmfnNg41396179;     pEkRKHxlksfXqDkIYPmfnNg41396179 = pEkRKHxlksfXqDkIYPmfnNg18328696;     pEkRKHxlksfXqDkIYPmfnNg18328696 = pEkRKHxlksfXqDkIYPmfnNg87332963;     pEkRKHxlksfXqDkIYPmfnNg87332963 = pEkRKHxlksfXqDkIYPmfnNg53305938;     pEkRKHxlksfXqDkIYPmfnNg53305938 = pEkRKHxlksfXqDkIYPmfnNg37493105;     pEkRKHxlksfXqDkIYPmfnNg37493105 = pEkRKHxlksfXqDkIYPmfnNg37562239;     pEkRKHxlksfXqDkIYPmfnNg37562239 = pEkRKHxlksfXqDkIYPmfnNg12729082;     pEkRKHxlksfXqDkIYPmfnNg12729082 = pEkRKHxlksfXqDkIYPmfnNg68820349;     pEkRKHxlksfXqDkIYPmfnNg68820349 = pEkRKHxlksfXqDkIYPmfnNg30587592;     pEkRKHxlksfXqDkIYPmfnNg30587592 = pEkRKHxlksfXqDkIYPmfnNg72163548;     pEkRKHxlksfXqDkIYPmfnNg72163548 = pEkRKHxlksfXqDkIYPmfnNg82603688;     pEkRKHxlksfXqDkIYPmfnNg82603688 = pEkRKHxlksfXqDkIYPmfnNg50575763;     pEkRKHxlksfXqDkIYPmfnNg50575763 = pEkRKHxlksfXqDkIYPmfnNg93088683;     pEkRKHxlksfXqDkIYPmfnNg93088683 = pEkRKHxlksfXqDkIYPmfnNg59807031;     pEkRKHxlksfXqDkIYPmfnNg59807031 = pEkRKHxlksfXqDkIYPmfnNg34698950;     pEkRKHxlksfXqDkIYPmfnNg34698950 = pEkRKHxlksfXqDkIYPmfnNg64954927;     pEkRKHxlksfXqDkIYPmfnNg64954927 = pEkRKHxlksfXqDkIYPmfnNg46883130;     pEkRKHxlksfXqDkIYPmfnNg46883130 = pEkRKHxlksfXqDkIYPmfnNg36112560;     pEkRKHxlksfXqDkIYPmfnNg36112560 = pEkRKHxlksfXqDkIYPmfnNg62739127;     pEkRKHxlksfXqDkIYPmfnNg62739127 = pEkRKHxlksfXqDkIYPmfnNg34947192;     pEkRKHxlksfXqDkIYPmfnNg34947192 = pEkRKHxlksfXqDkIYPmfnNg35382434;     pEkRKHxlksfXqDkIYPmfnNg35382434 = pEkRKHxlksfXqDkIYPmfnNg72263951;     pEkRKHxlksfXqDkIYPmfnNg72263951 = pEkRKHxlksfXqDkIYPmfnNg75149302;     pEkRKHxlksfXqDkIYPmfnNg75149302 = pEkRKHxlksfXqDkIYPmfnNg42925573;     pEkRKHxlksfXqDkIYPmfnNg42925573 = pEkRKHxlksfXqDkIYPmfnNg5711967;     pEkRKHxlksfXqDkIYPmfnNg5711967 = pEkRKHxlksfXqDkIYPmfnNg87667712;     pEkRKHxlksfXqDkIYPmfnNg87667712 = pEkRKHxlksfXqDkIYPmfnNg70006978;     pEkRKHxlksfXqDkIYPmfnNg70006978 = pEkRKHxlksfXqDkIYPmfnNg58526342;     pEkRKHxlksfXqDkIYPmfnNg58526342 = pEkRKHxlksfXqDkIYPmfnNg73056619;     pEkRKHxlksfXqDkIYPmfnNg73056619 = pEkRKHxlksfXqDkIYPmfnNg6303831;     pEkRKHxlksfXqDkIYPmfnNg6303831 = pEkRKHxlksfXqDkIYPmfnNg70545208;     pEkRKHxlksfXqDkIYPmfnNg70545208 = pEkRKHxlksfXqDkIYPmfnNg44598693;     pEkRKHxlksfXqDkIYPmfnNg44598693 = pEkRKHxlksfXqDkIYPmfnNg61508729;     pEkRKHxlksfXqDkIYPmfnNg61508729 = pEkRKHxlksfXqDkIYPmfnNg20286280;     pEkRKHxlksfXqDkIYPmfnNg20286280 = pEkRKHxlksfXqDkIYPmfnNg22478567;     pEkRKHxlksfXqDkIYPmfnNg22478567 = pEkRKHxlksfXqDkIYPmfnNg52065945;     pEkRKHxlksfXqDkIYPmfnNg52065945 = pEkRKHxlksfXqDkIYPmfnNg26934860;     pEkRKHxlksfXqDkIYPmfnNg26934860 = pEkRKHxlksfXqDkIYPmfnNg84092563;     pEkRKHxlksfXqDkIYPmfnNg84092563 = pEkRKHxlksfXqDkIYPmfnNg58954364;     pEkRKHxlksfXqDkIYPmfnNg58954364 = pEkRKHxlksfXqDkIYPmfnNg61709547;     pEkRKHxlksfXqDkIYPmfnNg61709547 = pEkRKHxlksfXqDkIYPmfnNg87601470;     pEkRKHxlksfXqDkIYPmfnNg87601470 = pEkRKHxlksfXqDkIYPmfnNg4478539;     pEkRKHxlksfXqDkIYPmfnNg4478539 = pEkRKHxlksfXqDkIYPmfnNg5650534;     pEkRKHxlksfXqDkIYPmfnNg5650534 = pEkRKHxlksfXqDkIYPmfnNg24383626;     pEkRKHxlksfXqDkIYPmfnNg24383626 = pEkRKHxlksfXqDkIYPmfnNg18524711;     pEkRKHxlksfXqDkIYPmfnNg18524711 = pEkRKHxlksfXqDkIYPmfnNg53263439;     pEkRKHxlksfXqDkIYPmfnNg53263439 = pEkRKHxlksfXqDkIYPmfnNg92848266;     pEkRKHxlksfXqDkIYPmfnNg92848266 = pEkRKHxlksfXqDkIYPmfnNg4614983;     pEkRKHxlksfXqDkIYPmfnNg4614983 = pEkRKHxlksfXqDkIYPmfnNg42759178;     pEkRKHxlksfXqDkIYPmfnNg42759178 = pEkRKHxlksfXqDkIYPmfnNg24261030;     pEkRKHxlksfXqDkIYPmfnNg24261030 = pEkRKHxlksfXqDkIYPmfnNg97004848;     pEkRKHxlksfXqDkIYPmfnNg97004848 = pEkRKHxlksfXqDkIYPmfnNg21653170;     pEkRKHxlksfXqDkIYPmfnNg21653170 = pEkRKHxlksfXqDkIYPmfnNg3562429;     pEkRKHxlksfXqDkIYPmfnNg3562429 = pEkRKHxlksfXqDkIYPmfnNg71222462;     pEkRKHxlksfXqDkIYPmfnNg71222462 = pEkRKHxlksfXqDkIYPmfnNg24069107;     pEkRKHxlksfXqDkIYPmfnNg24069107 = pEkRKHxlksfXqDkIYPmfnNg69132228;     pEkRKHxlksfXqDkIYPmfnNg69132228 = pEkRKHxlksfXqDkIYPmfnNg43179394;     pEkRKHxlksfXqDkIYPmfnNg43179394 = pEkRKHxlksfXqDkIYPmfnNg44407391;     pEkRKHxlksfXqDkIYPmfnNg44407391 = pEkRKHxlksfXqDkIYPmfnNg47593972;     pEkRKHxlksfXqDkIYPmfnNg47593972 = pEkRKHxlksfXqDkIYPmfnNg49825393;     pEkRKHxlksfXqDkIYPmfnNg49825393 = pEkRKHxlksfXqDkIYPmfnNg67555260;     pEkRKHxlksfXqDkIYPmfnNg67555260 = pEkRKHxlksfXqDkIYPmfnNg54202740;     pEkRKHxlksfXqDkIYPmfnNg54202740 = pEkRKHxlksfXqDkIYPmfnNg95763730;     pEkRKHxlksfXqDkIYPmfnNg95763730 = pEkRKHxlksfXqDkIYPmfnNg24283762;     pEkRKHxlksfXqDkIYPmfnNg24283762 = pEkRKHxlksfXqDkIYPmfnNg1618340;     pEkRKHxlksfXqDkIYPmfnNg1618340 = pEkRKHxlksfXqDkIYPmfnNg38004996;     pEkRKHxlksfXqDkIYPmfnNg38004996 = pEkRKHxlksfXqDkIYPmfnNg89067033;     pEkRKHxlksfXqDkIYPmfnNg89067033 = pEkRKHxlksfXqDkIYPmfnNg71696476;}



void nwKigMCHngtqrnFDhipNQSNWYX93564913() {     float dWIwiApJGjCHIrudgtlICKk55419995 = -549195624;    float dWIwiApJGjCHIrudgtlICKk41652067 = -393038746;    float dWIwiApJGjCHIrudgtlICKk98137285 = 40655360;    float dWIwiApJGjCHIrudgtlICKk70752614 = -586661190;    float dWIwiApJGjCHIrudgtlICKk13990328 = -707251958;    float dWIwiApJGjCHIrudgtlICKk47323986 = -254580514;    float dWIwiApJGjCHIrudgtlICKk99153138 = 89811240;    float dWIwiApJGjCHIrudgtlICKk93531891 = 348350;    float dWIwiApJGjCHIrudgtlICKk78530466 = -257539852;    float dWIwiApJGjCHIrudgtlICKk57005836 = -490293422;    float dWIwiApJGjCHIrudgtlICKk10567420 = -281862817;    float dWIwiApJGjCHIrudgtlICKk69949590 = -660729794;    float dWIwiApJGjCHIrudgtlICKk14928598 = -265898443;    float dWIwiApJGjCHIrudgtlICKk64767785 = -306668961;    float dWIwiApJGjCHIrudgtlICKk18447082 = -206385350;    float dWIwiApJGjCHIrudgtlICKk9212530 = -750688843;    float dWIwiApJGjCHIrudgtlICKk96312880 = -622128376;    float dWIwiApJGjCHIrudgtlICKk13349055 = -508562778;    float dWIwiApJGjCHIrudgtlICKk41307032 = -250836817;    float dWIwiApJGjCHIrudgtlICKk47406533 = -164894008;    float dWIwiApJGjCHIrudgtlICKk3577635 = -681317587;    float dWIwiApJGjCHIrudgtlICKk97549245 = -954766387;    float dWIwiApJGjCHIrudgtlICKk70304264 = -442585014;    float dWIwiApJGjCHIrudgtlICKk4981898 = -441480589;    float dWIwiApJGjCHIrudgtlICKk22539043 = -170528599;    float dWIwiApJGjCHIrudgtlICKk17414771 = 58517474;    float dWIwiApJGjCHIrudgtlICKk99050066 = -243456748;    float dWIwiApJGjCHIrudgtlICKk31815157 = -850744337;    float dWIwiApJGjCHIrudgtlICKk15806120 = -56809866;    float dWIwiApJGjCHIrudgtlICKk93314057 = -476743480;    float dWIwiApJGjCHIrudgtlICKk47665978 = -324063532;    float dWIwiApJGjCHIrudgtlICKk30962943 = -198779078;    float dWIwiApJGjCHIrudgtlICKk73576241 = -779497221;    float dWIwiApJGjCHIrudgtlICKk77120893 = -697936189;    float dWIwiApJGjCHIrudgtlICKk85932021 = -513786122;    float dWIwiApJGjCHIrudgtlICKk57194197 = -238244781;    float dWIwiApJGjCHIrudgtlICKk53069529 = -658953396;    float dWIwiApJGjCHIrudgtlICKk87874896 = 49507369;    float dWIwiApJGjCHIrudgtlICKk98177957 = -929201781;    float dWIwiApJGjCHIrudgtlICKk51918197 = -457202166;    float dWIwiApJGjCHIrudgtlICKk33587870 = -622178792;    float dWIwiApJGjCHIrudgtlICKk59927318 = -343381517;    float dWIwiApJGjCHIrudgtlICKk4963695 = -727586927;    float dWIwiApJGjCHIrudgtlICKk29799208 = -551236817;    float dWIwiApJGjCHIrudgtlICKk78563014 = -309463192;    float dWIwiApJGjCHIrudgtlICKk37355565 = -394959875;    float dWIwiApJGjCHIrudgtlICKk92986000 = 24953982;    float dWIwiApJGjCHIrudgtlICKk73440796 = -157056473;    float dWIwiApJGjCHIrudgtlICKk98829140 = -675988243;    float dWIwiApJGjCHIrudgtlICKk82756459 = -109224108;    float dWIwiApJGjCHIrudgtlICKk76203391 = -649122856;    float dWIwiApJGjCHIrudgtlICKk14244847 = -587220207;    float dWIwiApJGjCHIrudgtlICKk80329594 = -431943048;    float dWIwiApJGjCHIrudgtlICKk81315039 = -510053149;    float dWIwiApJGjCHIrudgtlICKk89987278 = -230138509;    float dWIwiApJGjCHIrudgtlICKk57870749 = -594429238;    float dWIwiApJGjCHIrudgtlICKk71347803 = -950453732;    float dWIwiApJGjCHIrudgtlICKk93155388 = -517864051;    float dWIwiApJGjCHIrudgtlICKk48213572 = -316132592;    float dWIwiApJGjCHIrudgtlICKk96575556 = -665769432;    float dWIwiApJGjCHIrudgtlICKk48273920 = 88876234;    float dWIwiApJGjCHIrudgtlICKk67337982 = -59444423;    float dWIwiApJGjCHIrudgtlICKk77725771 = -942841785;    float dWIwiApJGjCHIrudgtlICKk85216409 = -780796372;    float dWIwiApJGjCHIrudgtlICKk9339859 = -66229891;    float dWIwiApJGjCHIrudgtlICKk79604477 = 16916260;    float dWIwiApJGjCHIrudgtlICKk96373348 = -881232574;    float dWIwiApJGjCHIrudgtlICKk37807704 = -567962254;    float dWIwiApJGjCHIrudgtlICKk78835764 = -792882839;    float dWIwiApJGjCHIrudgtlICKk61252885 = -968140569;    float dWIwiApJGjCHIrudgtlICKk56143001 = 8264553;    float dWIwiApJGjCHIrudgtlICKk8437984 = -571635746;    float dWIwiApJGjCHIrudgtlICKk15171098 = -579360998;    float dWIwiApJGjCHIrudgtlICKk89388835 = -793634652;    float dWIwiApJGjCHIrudgtlICKk13818663 = -542715216;    float dWIwiApJGjCHIrudgtlICKk43650317 = -237936071;    float dWIwiApJGjCHIrudgtlICKk92585550 = -127179460;    float dWIwiApJGjCHIrudgtlICKk40505057 = -891348197;    float dWIwiApJGjCHIrudgtlICKk26418884 = -32017397;    float dWIwiApJGjCHIrudgtlICKk85183477 = -775568725;    float dWIwiApJGjCHIrudgtlICKk24428771 = -966436509;    float dWIwiApJGjCHIrudgtlICKk25609270 = 13599725;    float dWIwiApJGjCHIrudgtlICKk32986017 = -74756094;    float dWIwiApJGjCHIrudgtlICKk33049660 = -947585758;    float dWIwiApJGjCHIrudgtlICKk17110666 = -827620624;    float dWIwiApJGjCHIrudgtlICKk33421132 = -736843325;    float dWIwiApJGjCHIrudgtlICKk50633349 = -766836030;    float dWIwiApJGjCHIrudgtlICKk92261202 = -169444073;    float dWIwiApJGjCHIrudgtlICKk87133614 = -367797680;    float dWIwiApJGjCHIrudgtlICKk28061272 = -919356885;    float dWIwiApJGjCHIrudgtlICKk85846393 = -287791050;    float dWIwiApJGjCHIrudgtlICKk59914140 = -41089346;    float dWIwiApJGjCHIrudgtlICKk39661324 = -634360040;    float dWIwiApJGjCHIrudgtlICKk1602401 = -163432349;    float dWIwiApJGjCHIrudgtlICKk3644278 = -446078400;    float dWIwiApJGjCHIrudgtlICKk66249888 = -462734369;    float dWIwiApJGjCHIrudgtlICKk82201547 = -400539732;    float dWIwiApJGjCHIrudgtlICKk19747285 = -946790555;    float dWIwiApJGjCHIrudgtlICKk20459350 = -385006927;    float dWIwiApJGjCHIrudgtlICKk98958536 = -549195624;     dWIwiApJGjCHIrudgtlICKk55419995 = dWIwiApJGjCHIrudgtlICKk41652067;     dWIwiApJGjCHIrudgtlICKk41652067 = dWIwiApJGjCHIrudgtlICKk98137285;     dWIwiApJGjCHIrudgtlICKk98137285 = dWIwiApJGjCHIrudgtlICKk70752614;     dWIwiApJGjCHIrudgtlICKk70752614 = dWIwiApJGjCHIrudgtlICKk13990328;     dWIwiApJGjCHIrudgtlICKk13990328 = dWIwiApJGjCHIrudgtlICKk47323986;     dWIwiApJGjCHIrudgtlICKk47323986 = dWIwiApJGjCHIrudgtlICKk99153138;     dWIwiApJGjCHIrudgtlICKk99153138 = dWIwiApJGjCHIrudgtlICKk93531891;     dWIwiApJGjCHIrudgtlICKk93531891 = dWIwiApJGjCHIrudgtlICKk78530466;     dWIwiApJGjCHIrudgtlICKk78530466 = dWIwiApJGjCHIrudgtlICKk57005836;     dWIwiApJGjCHIrudgtlICKk57005836 = dWIwiApJGjCHIrudgtlICKk10567420;     dWIwiApJGjCHIrudgtlICKk10567420 = dWIwiApJGjCHIrudgtlICKk69949590;     dWIwiApJGjCHIrudgtlICKk69949590 = dWIwiApJGjCHIrudgtlICKk14928598;     dWIwiApJGjCHIrudgtlICKk14928598 = dWIwiApJGjCHIrudgtlICKk64767785;     dWIwiApJGjCHIrudgtlICKk64767785 = dWIwiApJGjCHIrudgtlICKk18447082;     dWIwiApJGjCHIrudgtlICKk18447082 = dWIwiApJGjCHIrudgtlICKk9212530;     dWIwiApJGjCHIrudgtlICKk9212530 = dWIwiApJGjCHIrudgtlICKk96312880;     dWIwiApJGjCHIrudgtlICKk96312880 = dWIwiApJGjCHIrudgtlICKk13349055;     dWIwiApJGjCHIrudgtlICKk13349055 = dWIwiApJGjCHIrudgtlICKk41307032;     dWIwiApJGjCHIrudgtlICKk41307032 = dWIwiApJGjCHIrudgtlICKk47406533;     dWIwiApJGjCHIrudgtlICKk47406533 = dWIwiApJGjCHIrudgtlICKk3577635;     dWIwiApJGjCHIrudgtlICKk3577635 = dWIwiApJGjCHIrudgtlICKk97549245;     dWIwiApJGjCHIrudgtlICKk97549245 = dWIwiApJGjCHIrudgtlICKk70304264;     dWIwiApJGjCHIrudgtlICKk70304264 = dWIwiApJGjCHIrudgtlICKk4981898;     dWIwiApJGjCHIrudgtlICKk4981898 = dWIwiApJGjCHIrudgtlICKk22539043;     dWIwiApJGjCHIrudgtlICKk22539043 = dWIwiApJGjCHIrudgtlICKk17414771;     dWIwiApJGjCHIrudgtlICKk17414771 = dWIwiApJGjCHIrudgtlICKk99050066;     dWIwiApJGjCHIrudgtlICKk99050066 = dWIwiApJGjCHIrudgtlICKk31815157;     dWIwiApJGjCHIrudgtlICKk31815157 = dWIwiApJGjCHIrudgtlICKk15806120;     dWIwiApJGjCHIrudgtlICKk15806120 = dWIwiApJGjCHIrudgtlICKk93314057;     dWIwiApJGjCHIrudgtlICKk93314057 = dWIwiApJGjCHIrudgtlICKk47665978;     dWIwiApJGjCHIrudgtlICKk47665978 = dWIwiApJGjCHIrudgtlICKk30962943;     dWIwiApJGjCHIrudgtlICKk30962943 = dWIwiApJGjCHIrudgtlICKk73576241;     dWIwiApJGjCHIrudgtlICKk73576241 = dWIwiApJGjCHIrudgtlICKk77120893;     dWIwiApJGjCHIrudgtlICKk77120893 = dWIwiApJGjCHIrudgtlICKk85932021;     dWIwiApJGjCHIrudgtlICKk85932021 = dWIwiApJGjCHIrudgtlICKk57194197;     dWIwiApJGjCHIrudgtlICKk57194197 = dWIwiApJGjCHIrudgtlICKk53069529;     dWIwiApJGjCHIrudgtlICKk53069529 = dWIwiApJGjCHIrudgtlICKk87874896;     dWIwiApJGjCHIrudgtlICKk87874896 = dWIwiApJGjCHIrudgtlICKk98177957;     dWIwiApJGjCHIrudgtlICKk98177957 = dWIwiApJGjCHIrudgtlICKk51918197;     dWIwiApJGjCHIrudgtlICKk51918197 = dWIwiApJGjCHIrudgtlICKk33587870;     dWIwiApJGjCHIrudgtlICKk33587870 = dWIwiApJGjCHIrudgtlICKk59927318;     dWIwiApJGjCHIrudgtlICKk59927318 = dWIwiApJGjCHIrudgtlICKk4963695;     dWIwiApJGjCHIrudgtlICKk4963695 = dWIwiApJGjCHIrudgtlICKk29799208;     dWIwiApJGjCHIrudgtlICKk29799208 = dWIwiApJGjCHIrudgtlICKk78563014;     dWIwiApJGjCHIrudgtlICKk78563014 = dWIwiApJGjCHIrudgtlICKk37355565;     dWIwiApJGjCHIrudgtlICKk37355565 = dWIwiApJGjCHIrudgtlICKk92986000;     dWIwiApJGjCHIrudgtlICKk92986000 = dWIwiApJGjCHIrudgtlICKk73440796;     dWIwiApJGjCHIrudgtlICKk73440796 = dWIwiApJGjCHIrudgtlICKk98829140;     dWIwiApJGjCHIrudgtlICKk98829140 = dWIwiApJGjCHIrudgtlICKk82756459;     dWIwiApJGjCHIrudgtlICKk82756459 = dWIwiApJGjCHIrudgtlICKk76203391;     dWIwiApJGjCHIrudgtlICKk76203391 = dWIwiApJGjCHIrudgtlICKk14244847;     dWIwiApJGjCHIrudgtlICKk14244847 = dWIwiApJGjCHIrudgtlICKk80329594;     dWIwiApJGjCHIrudgtlICKk80329594 = dWIwiApJGjCHIrudgtlICKk81315039;     dWIwiApJGjCHIrudgtlICKk81315039 = dWIwiApJGjCHIrudgtlICKk89987278;     dWIwiApJGjCHIrudgtlICKk89987278 = dWIwiApJGjCHIrudgtlICKk57870749;     dWIwiApJGjCHIrudgtlICKk57870749 = dWIwiApJGjCHIrudgtlICKk71347803;     dWIwiApJGjCHIrudgtlICKk71347803 = dWIwiApJGjCHIrudgtlICKk93155388;     dWIwiApJGjCHIrudgtlICKk93155388 = dWIwiApJGjCHIrudgtlICKk48213572;     dWIwiApJGjCHIrudgtlICKk48213572 = dWIwiApJGjCHIrudgtlICKk96575556;     dWIwiApJGjCHIrudgtlICKk96575556 = dWIwiApJGjCHIrudgtlICKk48273920;     dWIwiApJGjCHIrudgtlICKk48273920 = dWIwiApJGjCHIrudgtlICKk67337982;     dWIwiApJGjCHIrudgtlICKk67337982 = dWIwiApJGjCHIrudgtlICKk77725771;     dWIwiApJGjCHIrudgtlICKk77725771 = dWIwiApJGjCHIrudgtlICKk85216409;     dWIwiApJGjCHIrudgtlICKk85216409 = dWIwiApJGjCHIrudgtlICKk9339859;     dWIwiApJGjCHIrudgtlICKk9339859 = dWIwiApJGjCHIrudgtlICKk79604477;     dWIwiApJGjCHIrudgtlICKk79604477 = dWIwiApJGjCHIrudgtlICKk96373348;     dWIwiApJGjCHIrudgtlICKk96373348 = dWIwiApJGjCHIrudgtlICKk37807704;     dWIwiApJGjCHIrudgtlICKk37807704 = dWIwiApJGjCHIrudgtlICKk78835764;     dWIwiApJGjCHIrudgtlICKk78835764 = dWIwiApJGjCHIrudgtlICKk61252885;     dWIwiApJGjCHIrudgtlICKk61252885 = dWIwiApJGjCHIrudgtlICKk56143001;     dWIwiApJGjCHIrudgtlICKk56143001 = dWIwiApJGjCHIrudgtlICKk8437984;     dWIwiApJGjCHIrudgtlICKk8437984 = dWIwiApJGjCHIrudgtlICKk15171098;     dWIwiApJGjCHIrudgtlICKk15171098 = dWIwiApJGjCHIrudgtlICKk89388835;     dWIwiApJGjCHIrudgtlICKk89388835 = dWIwiApJGjCHIrudgtlICKk13818663;     dWIwiApJGjCHIrudgtlICKk13818663 = dWIwiApJGjCHIrudgtlICKk43650317;     dWIwiApJGjCHIrudgtlICKk43650317 = dWIwiApJGjCHIrudgtlICKk92585550;     dWIwiApJGjCHIrudgtlICKk92585550 = dWIwiApJGjCHIrudgtlICKk40505057;     dWIwiApJGjCHIrudgtlICKk40505057 = dWIwiApJGjCHIrudgtlICKk26418884;     dWIwiApJGjCHIrudgtlICKk26418884 = dWIwiApJGjCHIrudgtlICKk85183477;     dWIwiApJGjCHIrudgtlICKk85183477 = dWIwiApJGjCHIrudgtlICKk24428771;     dWIwiApJGjCHIrudgtlICKk24428771 = dWIwiApJGjCHIrudgtlICKk25609270;     dWIwiApJGjCHIrudgtlICKk25609270 = dWIwiApJGjCHIrudgtlICKk32986017;     dWIwiApJGjCHIrudgtlICKk32986017 = dWIwiApJGjCHIrudgtlICKk33049660;     dWIwiApJGjCHIrudgtlICKk33049660 = dWIwiApJGjCHIrudgtlICKk17110666;     dWIwiApJGjCHIrudgtlICKk17110666 = dWIwiApJGjCHIrudgtlICKk33421132;     dWIwiApJGjCHIrudgtlICKk33421132 = dWIwiApJGjCHIrudgtlICKk50633349;     dWIwiApJGjCHIrudgtlICKk50633349 = dWIwiApJGjCHIrudgtlICKk92261202;     dWIwiApJGjCHIrudgtlICKk92261202 = dWIwiApJGjCHIrudgtlICKk87133614;     dWIwiApJGjCHIrudgtlICKk87133614 = dWIwiApJGjCHIrudgtlICKk28061272;     dWIwiApJGjCHIrudgtlICKk28061272 = dWIwiApJGjCHIrudgtlICKk85846393;     dWIwiApJGjCHIrudgtlICKk85846393 = dWIwiApJGjCHIrudgtlICKk59914140;     dWIwiApJGjCHIrudgtlICKk59914140 = dWIwiApJGjCHIrudgtlICKk39661324;     dWIwiApJGjCHIrudgtlICKk39661324 = dWIwiApJGjCHIrudgtlICKk1602401;     dWIwiApJGjCHIrudgtlICKk1602401 = dWIwiApJGjCHIrudgtlICKk3644278;     dWIwiApJGjCHIrudgtlICKk3644278 = dWIwiApJGjCHIrudgtlICKk66249888;     dWIwiApJGjCHIrudgtlICKk66249888 = dWIwiApJGjCHIrudgtlICKk82201547;     dWIwiApJGjCHIrudgtlICKk82201547 = dWIwiApJGjCHIrudgtlICKk19747285;     dWIwiApJGjCHIrudgtlICKk19747285 = dWIwiApJGjCHIrudgtlICKk20459350;     dWIwiApJGjCHIrudgtlICKk20459350 = dWIwiApJGjCHIrudgtlICKk98958536;     dWIwiApJGjCHIrudgtlICKk98958536 = dWIwiApJGjCHIrudgtlICKk55419995;}



void fYRxfSMFRWhcKtynqsFKUHbKQO78627751() {     float wOZdftDSOdMDFPRSyyyLFss39143513 = -899217691;    float wOZdftDSOdMDFPRSyyyLFss39250163 = -196690597;    float wOZdftDSOdMDFPRSyyyLFss86723402 = -927652331;    float wOZdftDSOdMDFPRSyyyLFss67900568 = -591543119;    float wOZdftDSOdMDFPRSyyyLFss93551663 = -5200150;    float wOZdftDSOdMDFPRSyyyLFss58663504 = -357901182;    float wOZdftDSOdMDFPRSyyyLFss36033702 = 68297102;    float wOZdftDSOdMDFPRSyyyLFss36871974 = -620106237;    float wOZdftDSOdMDFPRSyyyLFss28749995 = -955022549;    float wOZdftDSOdMDFPRSyyyLFss3111425 = -713334438;    float wOZdftDSOdMDFPRSyyyLFss53456456 = -122148075;    float wOZdftDSOdMDFPRSyyyLFss60161361 = -589869065;    float wOZdftDSOdMDFPRSyyyLFss65982449 = -108730686;    float wOZdftDSOdMDFPRSyyyLFss59140930 = -656351823;    float wOZdftDSOdMDFPRSyyyLFss22626340 = -132082655;    float wOZdftDSOdMDFPRSyyyLFss81026558 = -674816494;    float wOZdftDSOdMDFPRSyyyLFss96178290 = -781008239;    float wOZdftDSOdMDFPRSyyyLFss27426325 = -119414352;    float wOZdftDSOdMDFPRSyyyLFss82283513 = -39486055;    float wOZdftDSOdMDFPRSyyyLFss21514178 = -881186925;    float wOZdftDSOdMDFPRSyyyLFss70917145 = -255784108;    float wOZdftDSOdMDFPRSyyyLFss98551316 = -867921673;    float wOZdftDSOdMDFPRSyyyLFss39480130 = -471979751;    float wOZdftDSOdMDFPRSyyyLFss6124593 = 19286758;    float wOZdftDSOdMDFPRSyyyLFss59141135 = -248859361;    float wOZdftDSOdMDFPRSyyyLFss70407528 = -384449554;    float wOZdftDSOdMDFPRSyyyLFss20642006 = -687332171;    float wOZdftDSOdMDFPRSyyyLFss74414357 = -637185826;    float wOZdftDSOdMDFPRSyyyLFss87724261 = -581005767;    float wOZdftDSOdMDFPRSyyyLFss28862385 = -49349855;    float wOZdftDSOdMDFPRSyyyLFss29030401 = -543161552;    float wOZdftDSOdMDFPRSyyyLFss55756231 = -747976300;    float wOZdftDSOdMDFPRSyyyLFss87700943 = -344711312;    float wOZdftDSOdMDFPRSyyyLFss12845608 = -2032837;    float wOZdftDSOdMDFPRSyyyLFss53535347 = -248076730;    float wOZdftDSOdMDFPRSyyyLFss27055430 = -216641258;    float wOZdftDSOdMDFPRSyyyLFss52833119 = -979294693;    float wOZdftDSOdMDFPRSyyyLFss38256688 = -949635848;    float wOZdftDSOdMDFPRSyyyLFss58793676 = -134760662;    float wOZdftDSOdMDFPRSyyyLFss91107313 = -403849045;    float wOZdftDSOdMDFPRSyyyLFss98355390 = -272218919;    float wOZdftDSOdMDFPRSyyyLFss89267044 = 50592814;    float wOZdftDSOdMDFPRSyyyLFss37763841 = -379991181;    float wOZdftDSOdMDFPRSyyyLFss76994727 = -130180550;    float wOZdftDSOdMDFPRSyyyLFss6550265 = -635477271;    float wOZdftDSOdMDFPRSyyyLFss81622446 = -9991566;    float wOZdftDSOdMDFPRSyyyLFss26164970 = 3308075;    float wOZdftDSOdMDFPRSyyyLFss12182644 = 80821629;    float wOZdftDSOdMDFPRSyyyLFss32703354 = -375772394;    float wOZdftDSOdMDFPRSyyyLFss18629790 = -741239544;    float wOZdftDSOdMDFPRSyyyLFss16294223 = -814895258;    float wOZdftDSOdMDFPRSyyyLFss65750566 = -339272993;    float wOZdftDSOdMDFPRSyyyLFss25711997 = -443875414;    float wOZdftDSOdMDFPRSyyyLFss27247646 = 13237664;    float wOZdftDSOdMDFPRSyyyLFss7710606 = -546865454;    float wOZdftDSOdMDFPRSyyyLFss40592196 = 68703982;    float wOZdftDSOdMDFPRSyyyLFss99770033 = -724710846;    float wOZdftDSOdMDFPRSyyyLFss80598810 = -846939089;    float wOZdftDSOdMDFPRSyyyLFss8759433 = -242683759;    float wOZdftDSOdMDFPRSyyyLFss23144136 = -620750596;    float wOZdftDSOdMDFPRSyyyLFss38021499 = -670569012;    float wOZdftDSOdMDFPRSyyyLFss61619345 = -294517073;    float wOZdftDSOdMDFPRSyyyLFss49147713 = 60899529;    float wOZdftDSOdMDFPRSyyyLFss99887610 = -805672694;    float wOZdftDSOdMDFPRSyyyLFss74081023 = -70172886;    float wOZdftDSOdMDFPRSyyyLFss97700224 = -374171775;    float wOZdftDSOdMDFPRSyyyLFss72460418 = -145157754;    float wOZdftDSOdMDFPRSyyyLFss53136842 = -6697850;    float wOZdftDSOdMDFPRSyyyLFss5605584 = -308275094;    float wOZdftDSOdMDFPRSyyyLFss95570909 = -915441397;    float wOZdftDSOdMDFPRSyyyLFss28193440 = -695521802;    float wOZdftDSOdMDFPRSyyyLFss57921603 = -831372392;    float wOZdftDSOdMDFPRSyyyLFss68632648 = -984653691;    float wOZdftDSOdMDFPRSyyyLFss91176199 = -635637011;    float wOZdftDSOdMDFPRSyyyLFss23158787 = -508968007;    float wOZdftDSOdMDFPRSyyyLFss81650100 = -206376922;    float wOZdftDSOdMDFPRSyyyLFss60787475 = -387930492;    float wOZdftDSOdMDFPRSyyyLFss62485402 = -241799201;    float wOZdftDSOdMDFPRSyyyLFss99574327 = -345235972;    float wOZdftDSOdMDFPRSyyyLFss77518689 = -138867795;    float wOZdftDSOdMDFPRSyyyLFss44242559 = -287757630;    float wOZdftDSOdMDFPRSyyyLFss8459362 = -668153800;    float wOZdftDSOdMDFPRSyyyLFss41711003 = -161413432;    float wOZdftDSOdMDFPRSyyyLFss69094472 = -839766223;    float wOZdftDSOdMDFPRSyyyLFss12568162 = -234454597;    float wOZdftDSOdMDFPRSyyyLFss63279835 = -103888559;    float wOZdftDSOdMDFPRSyyyLFss30044235 = -204100886;    float wOZdftDSOdMDFPRSyyyLFss60453297 = -257948976;    float wOZdftDSOdMDFPRSyyyLFss5135002 = -455167383;    float wOZdftDSOdMDFPRSyyyLFss12943151 = -216780712;    float wOZdftDSOdMDFPRSyyyLFss27285397 = -491930413;    float wOZdftDSOdMDFPRSyyyLFss72234308 = -32355604;    float wOZdftDSOdMDFPRSyyyLFss29497255 = -606952089;    float wOZdftDSOdMDFPRSyyyLFss35649541 = -514010066;    float wOZdftDSOdMDFPRSyyyLFss53085815 = -733280034;    float wOZdftDSOdMDFPRSyyyLFss36736046 = -977701846;    float wOZdftDSOdMDFPRSyyyLFss40119332 = 89693284;    float wOZdftDSOdMDFPRSyyyLFss37876230 = -574318487;    float wOZdftDSOdMDFPRSyyyLFss2913704 = 39992336;    float wOZdftDSOdMDFPRSyyyLFss8850040 = -899217691;     wOZdftDSOdMDFPRSyyyLFss39143513 = wOZdftDSOdMDFPRSyyyLFss39250163;     wOZdftDSOdMDFPRSyyyLFss39250163 = wOZdftDSOdMDFPRSyyyLFss86723402;     wOZdftDSOdMDFPRSyyyLFss86723402 = wOZdftDSOdMDFPRSyyyLFss67900568;     wOZdftDSOdMDFPRSyyyLFss67900568 = wOZdftDSOdMDFPRSyyyLFss93551663;     wOZdftDSOdMDFPRSyyyLFss93551663 = wOZdftDSOdMDFPRSyyyLFss58663504;     wOZdftDSOdMDFPRSyyyLFss58663504 = wOZdftDSOdMDFPRSyyyLFss36033702;     wOZdftDSOdMDFPRSyyyLFss36033702 = wOZdftDSOdMDFPRSyyyLFss36871974;     wOZdftDSOdMDFPRSyyyLFss36871974 = wOZdftDSOdMDFPRSyyyLFss28749995;     wOZdftDSOdMDFPRSyyyLFss28749995 = wOZdftDSOdMDFPRSyyyLFss3111425;     wOZdftDSOdMDFPRSyyyLFss3111425 = wOZdftDSOdMDFPRSyyyLFss53456456;     wOZdftDSOdMDFPRSyyyLFss53456456 = wOZdftDSOdMDFPRSyyyLFss60161361;     wOZdftDSOdMDFPRSyyyLFss60161361 = wOZdftDSOdMDFPRSyyyLFss65982449;     wOZdftDSOdMDFPRSyyyLFss65982449 = wOZdftDSOdMDFPRSyyyLFss59140930;     wOZdftDSOdMDFPRSyyyLFss59140930 = wOZdftDSOdMDFPRSyyyLFss22626340;     wOZdftDSOdMDFPRSyyyLFss22626340 = wOZdftDSOdMDFPRSyyyLFss81026558;     wOZdftDSOdMDFPRSyyyLFss81026558 = wOZdftDSOdMDFPRSyyyLFss96178290;     wOZdftDSOdMDFPRSyyyLFss96178290 = wOZdftDSOdMDFPRSyyyLFss27426325;     wOZdftDSOdMDFPRSyyyLFss27426325 = wOZdftDSOdMDFPRSyyyLFss82283513;     wOZdftDSOdMDFPRSyyyLFss82283513 = wOZdftDSOdMDFPRSyyyLFss21514178;     wOZdftDSOdMDFPRSyyyLFss21514178 = wOZdftDSOdMDFPRSyyyLFss70917145;     wOZdftDSOdMDFPRSyyyLFss70917145 = wOZdftDSOdMDFPRSyyyLFss98551316;     wOZdftDSOdMDFPRSyyyLFss98551316 = wOZdftDSOdMDFPRSyyyLFss39480130;     wOZdftDSOdMDFPRSyyyLFss39480130 = wOZdftDSOdMDFPRSyyyLFss6124593;     wOZdftDSOdMDFPRSyyyLFss6124593 = wOZdftDSOdMDFPRSyyyLFss59141135;     wOZdftDSOdMDFPRSyyyLFss59141135 = wOZdftDSOdMDFPRSyyyLFss70407528;     wOZdftDSOdMDFPRSyyyLFss70407528 = wOZdftDSOdMDFPRSyyyLFss20642006;     wOZdftDSOdMDFPRSyyyLFss20642006 = wOZdftDSOdMDFPRSyyyLFss74414357;     wOZdftDSOdMDFPRSyyyLFss74414357 = wOZdftDSOdMDFPRSyyyLFss87724261;     wOZdftDSOdMDFPRSyyyLFss87724261 = wOZdftDSOdMDFPRSyyyLFss28862385;     wOZdftDSOdMDFPRSyyyLFss28862385 = wOZdftDSOdMDFPRSyyyLFss29030401;     wOZdftDSOdMDFPRSyyyLFss29030401 = wOZdftDSOdMDFPRSyyyLFss55756231;     wOZdftDSOdMDFPRSyyyLFss55756231 = wOZdftDSOdMDFPRSyyyLFss87700943;     wOZdftDSOdMDFPRSyyyLFss87700943 = wOZdftDSOdMDFPRSyyyLFss12845608;     wOZdftDSOdMDFPRSyyyLFss12845608 = wOZdftDSOdMDFPRSyyyLFss53535347;     wOZdftDSOdMDFPRSyyyLFss53535347 = wOZdftDSOdMDFPRSyyyLFss27055430;     wOZdftDSOdMDFPRSyyyLFss27055430 = wOZdftDSOdMDFPRSyyyLFss52833119;     wOZdftDSOdMDFPRSyyyLFss52833119 = wOZdftDSOdMDFPRSyyyLFss38256688;     wOZdftDSOdMDFPRSyyyLFss38256688 = wOZdftDSOdMDFPRSyyyLFss58793676;     wOZdftDSOdMDFPRSyyyLFss58793676 = wOZdftDSOdMDFPRSyyyLFss91107313;     wOZdftDSOdMDFPRSyyyLFss91107313 = wOZdftDSOdMDFPRSyyyLFss98355390;     wOZdftDSOdMDFPRSyyyLFss98355390 = wOZdftDSOdMDFPRSyyyLFss89267044;     wOZdftDSOdMDFPRSyyyLFss89267044 = wOZdftDSOdMDFPRSyyyLFss37763841;     wOZdftDSOdMDFPRSyyyLFss37763841 = wOZdftDSOdMDFPRSyyyLFss76994727;     wOZdftDSOdMDFPRSyyyLFss76994727 = wOZdftDSOdMDFPRSyyyLFss6550265;     wOZdftDSOdMDFPRSyyyLFss6550265 = wOZdftDSOdMDFPRSyyyLFss81622446;     wOZdftDSOdMDFPRSyyyLFss81622446 = wOZdftDSOdMDFPRSyyyLFss26164970;     wOZdftDSOdMDFPRSyyyLFss26164970 = wOZdftDSOdMDFPRSyyyLFss12182644;     wOZdftDSOdMDFPRSyyyLFss12182644 = wOZdftDSOdMDFPRSyyyLFss32703354;     wOZdftDSOdMDFPRSyyyLFss32703354 = wOZdftDSOdMDFPRSyyyLFss18629790;     wOZdftDSOdMDFPRSyyyLFss18629790 = wOZdftDSOdMDFPRSyyyLFss16294223;     wOZdftDSOdMDFPRSyyyLFss16294223 = wOZdftDSOdMDFPRSyyyLFss65750566;     wOZdftDSOdMDFPRSyyyLFss65750566 = wOZdftDSOdMDFPRSyyyLFss25711997;     wOZdftDSOdMDFPRSyyyLFss25711997 = wOZdftDSOdMDFPRSyyyLFss27247646;     wOZdftDSOdMDFPRSyyyLFss27247646 = wOZdftDSOdMDFPRSyyyLFss7710606;     wOZdftDSOdMDFPRSyyyLFss7710606 = wOZdftDSOdMDFPRSyyyLFss40592196;     wOZdftDSOdMDFPRSyyyLFss40592196 = wOZdftDSOdMDFPRSyyyLFss99770033;     wOZdftDSOdMDFPRSyyyLFss99770033 = wOZdftDSOdMDFPRSyyyLFss80598810;     wOZdftDSOdMDFPRSyyyLFss80598810 = wOZdftDSOdMDFPRSyyyLFss8759433;     wOZdftDSOdMDFPRSyyyLFss8759433 = wOZdftDSOdMDFPRSyyyLFss23144136;     wOZdftDSOdMDFPRSyyyLFss23144136 = wOZdftDSOdMDFPRSyyyLFss38021499;     wOZdftDSOdMDFPRSyyyLFss38021499 = wOZdftDSOdMDFPRSyyyLFss61619345;     wOZdftDSOdMDFPRSyyyLFss61619345 = wOZdftDSOdMDFPRSyyyLFss49147713;     wOZdftDSOdMDFPRSyyyLFss49147713 = wOZdftDSOdMDFPRSyyyLFss99887610;     wOZdftDSOdMDFPRSyyyLFss99887610 = wOZdftDSOdMDFPRSyyyLFss74081023;     wOZdftDSOdMDFPRSyyyLFss74081023 = wOZdftDSOdMDFPRSyyyLFss97700224;     wOZdftDSOdMDFPRSyyyLFss97700224 = wOZdftDSOdMDFPRSyyyLFss72460418;     wOZdftDSOdMDFPRSyyyLFss72460418 = wOZdftDSOdMDFPRSyyyLFss53136842;     wOZdftDSOdMDFPRSyyyLFss53136842 = wOZdftDSOdMDFPRSyyyLFss5605584;     wOZdftDSOdMDFPRSyyyLFss5605584 = wOZdftDSOdMDFPRSyyyLFss95570909;     wOZdftDSOdMDFPRSyyyLFss95570909 = wOZdftDSOdMDFPRSyyyLFss28193440;     wOZdftDSOdMDFPRSyyyLFss28193440 = wOZdftDSOdMDFPRSyyyLFss57921603;     wOZdftDSOdMDFPRSyyyLFss57921603 = wOZdftDSOdMDFPRSyyyLFss68632648;     wOZdftDSOdMDFPRSyyyLFss68632648 = wOZdftDSOdMDFPRSyyyLFss91176199;     wOZdftDSOdMDFPRSyyyLFss91176199 = wOZdftDSOdMDFPRSyyyLFss23158787;     wOZdftDSOdMDFPRSyyyLFss23158787 = wOZdftDSOdMDFPRSyyyLFss81650100;     wOZdftDSOdMDFPRSyyyLFss81650100 = wOZdftDSOdMDFPRSyyyLFss60787475;     wOZdftDSOdMDFPRSyyyLFss60787475 = wOZdftDSOdMDFPRSyyyLFss62485402;     wOZdftDSOdMDFPRSyyyLFss62485402 = wOZdftDSOdMDFPRSyyyLFss99574327;     wOZdftDSOdMDFPRSyyyLFss99574327 = wOZdftDSOdMDFPRSyyyLFss77518689;     wOZdftDSOdMDFPRSyyyLFss77518689 = wOZdftDSOdMDFPRSyyyLFss44242559;     wOZdftDSOdMDFPRSyyyLFss44242559 = wOZdftDSOdMDFPRSyyyLFss8459362;     wOZdftDSOdMDFPRSyyyLFss8459362 = wOZdftDSOdMDFPRSyyyLFss41711003;     wOZdftDSOdMDFPRSyyyLFss41711003 = wOZdftDSOdMDFPRSyyyLFss69094472;     wOZdftDSOdMDFPRSyyyLFss69094472 = wOZdftDSOdMDFPRSyyyLFss12568162;     wOZdftDSOdMDFPRSyyyLFss12568162 = wOZdftDSOdMDFPRSyyyLFss63279835;     wOZdftDSOdMDFPRSyyyLFss63279835 = wOZdftDSOdMDFPRSyyyLFss30044235;     wOZdftDSOdMDFPRSyyyLFss30044235 = wOZdftDSOdMDFPRSyyyLFss60453297;     wOZdftDSOdMDFPRSyyyLFss60453297 = wOZdftDSOdMDFPRSyyyLFss5135002;     wOZdftDSOdMDFPRSyyyLFss5135002 = wOZdftDSOdMDFPRSyyyLFss12943151;     wOZdftDSOdMDFPRSyyyLFss12943151 = wOZdftDSOdMDFPRSyyyLFss27285397;     wOZdftDSOdMDFPRSyyyLFss27285397 = wOZdftDSOdMDFPRSyyyLFss72234308;     wOZdftDSOdMDFPRSyyyLFss72234308 = wOZdftDSOdMDFPRSyyyLFss29497255;     wOZdftDSOdMDFPRSyyyLFss29497255 = wOZdftDSOdMDFPRSyyyLFss35649541;     wOZdftDSOdMDFPRSyyyLFss35649541 = wOZdftDSOdMDFPRSyyyLFss53085815;     wOZdftDSOdMDFPRSyyyLFss53085815 = wOZdftDSOdMDFPRSyyyLFss36736046;     wOZdftDSOdMDFPRSyyyLFss36736046 = wOZdftDSOdMDFPRSyyyLFss40119332;     wOZdftDSOdMDFPRSyyyLFss40119332 = wOZdftDSOdMDFPRSyyyLFss37876230;     wOZdftDSOdMDFPRSyyyLFss37876230 = wOZdftDSOdMDFPRSyyyLFss2913704;     wOZdftDSOdMDFPRSyyyLFss2913704 = wOZdftDSOdMDFPRSyyyLFss8850040;     wOZdftDSOdMDFPRSyyyLFss8850040 = wOZdftDSOdMDFPRSyyyLFss39143513;}



void wFjreNxukAhAmKmvrDtJngGunW63690589() {     float autwIUTyvKRmVCnxSXIqwMM22867031 = -149239758;    float autwIUTyvKRmVCnxSXIqwMM36848259 = -342448;    float autwIUTyvKRmVCnxSXIqwMM75309520 = -795960021;    float autwIUTyvKRmVCnxSXIqwMM65048521 = -596425048;    float autwIUTyvKRmVCnxSXIqwMM73113000 = -403148341;    float autwIUTyvKRmVCnxSXIqwMM70003022 = -461221851;    float autwIUTyvKRmVCnxSXIqwMM72914265 = 46782964;    float autwIUTyvKRmVCnxSXIqwMM80212057 = -140560823;    float autwIUTyvKRmVCnxSXIqwMM78969523 = -552505245;    float autwIUTyvKRmVCnxSXIqwMM49217013 = -936375454;    float autwIUTyvKRmVCnxSXIqwMM96345491 = 37566668;    float autwIUTyvKRmVCnxSXIqwMM50373132 = -519008337;    float autwIUTyvKRmVCnxSXIqwMM17036302 = 48437070;    float autwIUTyvKRmVCnxSXIqwMM53514074 = 93965314;    float autwIUTyvKRmVCnxSXIqwMM26805599 = -57779960;    float autwIUTyvKRmVCnxSXIqwMM52840587 = -598944146;    float autwIUTyvKRmVCnxSXIqwMM96043701 = -939888102;    float autwIUTyvKRmVCnxSXIqwMM41503594 = -830265926;    float autwIUTyvKRmVCnxSXIqwMM23259994 = -928135294;    float autwIUTyvKRmVCnxSXIqwMM95621821 = -497479842;    float autwIUTyvKRmVCnxSXIqwMM38256656 = -930250628;    float autwIUTyvKRmVCnxSXIqwMM99553387 = -781076959;    float autwIUTyvKRmVCnxSXIqwMM8655996 = -501374488;    float autwIUTyvKRmVCnxSXIqwMM7267288 = -619945896;    float autwIUTyvKRmVCnxSXIqwMM95743228 = -327190122;    float autwIUTyvKRmVCnxSXIqwMM23400286 = -827416582;    float autwIUTyvKRmVCnxSXIqwMM42233945 = -31207594;    float autwIUTyvKRmVCnxSXIqwMM17013557 = -423627314;    float autwIUTyvKRmVCnxSXIqwMM59642403 = -5201667;    float autwIUTyvKRmVCnxSXIqwMM64410711 = -721956229;    float autwIUTyvKRmVCnxSXIqwMM10394825 = -762259573;    float autwIUTyvKRmVCnxSXIqwMM80549519 = -197173523;    float autwIUTyvKRmVCnxSXIqwMM1825645 = 90074597;    float autwIUTyvKRmVCnxSXIqwMM48570321 = -406129485;    float autwIUTyvKRmVCnxSXIqwMM21138672 = 17632663;    float autwIUTyvKRmVCnxSXIqwMM96916663 = -195037736;    float autwIUTyvKRmVCnxSXIqwMM52596709 = -199635989;    float autwIUTyvKRmVCnxSXIqwMM88638478 = -848779065;    float autwIUTyvKRmVCnxSXIqwMM19409396 = -440319543;    float autwIUTyvKRmVCnxSXIqwMM30296429 = -350495925;    float autwIUTyvKRmVCnxSXIqwMM63122911 = 77740955;    float autwIUTyvKRmVCnxSXIqwMM18606772 = -655432855;    float autwIUTyvKRmVCnxSXIqwMM70563987 = -32395436;    float autwIUTyvKRmVCnxSXIqwMM24190247 = -809124282;    float autwIUTyvKRmVCnxSXIqwMM34537515 = -961491349;    float autwIUTyvKRmVCnxSXIqwMM25889328 = -725023258;    float autwIUTyvKRmVCnxSXIqwMM59343939 = -18337832;    float autwIUTyvKRmVCnxSXIqwMM50924491 = -781300269;    float autwIUTyvKRmVCnxSXIqwMM66577567 = -75556545;    float autwIUTyvKRmVCnxSXIqwMM54503120 = -273254979;    float autwIUTyvKRmVCnxSXIqwMM56385055 = -980667659;    float autwIUTyvKRmVCnxSXIqwMM17256286 = -91325779;    float autwIUTyvKRmVCnxSXIqwMM71094399 = -455807780;    float autwIUTyvKRmVCnxSXIqwMM73180251 = -563471524;    float autwIUTyvKRmVCnxSXIqwMM25433934 = -863592399;    float autwIUTyvKRmVCnxSXIqwMM23313643 = -368162799;    float autwIUTyvKRmVCnxSXIqwMM28192264 = -498967961;    float autwIUTyvKRmVCnxSXIqwMM68042233 = -76014126;    float autwIUTyvKRmVCnxSXIqwMM69305293 = -169234926;    float autwIUTyvKRmVCnxSXIqwMM49712714 = -575731760;    float autwIUTyvKRmVCnxSXIqwMM27769077 = -330014258;    float autwIUTyvKRmVCnxSXIqwMM55900708 = -529589723;    float autwIUTyvKRmVCnxSXIqwMM20569654 = -35359156;    float autwIUTyvKRmVCnxSXIqwMM14558812 = -830549016;    float autwIUTyvKRmVCnxSXIqwMM38822189 = -74115881;    float autwIUTyvKRmVCnxSXIqwMM15795973 = -765259810;    float autwIUTyvKRmVCnxSXIqwMM48547487 = -509082934;    float autwIUTyvKRmVCnxSXIqwMM68465980 = -545433446;    float autwIUTyvKRmVCnxSXIqwMM32375402 = -923667349;    float autwIUTyvKRmVCnxSXIqwMM29888935 = -862742225;    float autwIUTyvKRmVCnxSXIqwMM243879 = -299308157;    float autwIUTyvKRmVCnxSXIqwMM7405223 = 8890962;    float autwIUTyvKRmVCnxSXIqwMM22094199 = -289946383;    float autwIUTyvKRmVCnxSXIqwMM92963564 = -477639370;    float autwIUTyvKRmVCnxSXIqwMM32498910 = -475220797;    float autwIUTyvKRmVCnxSXIqwMM19649884 = -174817774;    float autwIUTyvKRmVCnxSXIqwMM28989401 = -648681524;    float autwIUTyvKRmVCnxSXIqwMM84465748 = -692250206;    float autwIUTyvKRmVCnxSXIqwMM72729772 = -658454547;    float autwIUTyvKRmVCnxSXIqwMM69853900 = -602166865;    float autwIUTyvKRmVCnxSXIqwMM64056347 = -709078751;    float autwIUTyvKRmVCnxSXIqwMM91309453 = -249907326;    float autwIUTyvKRmVCnxSXIqwMM50435990 = -248070769;    float autwIUTyvKRmVCnxSXIqwMM5139284 = -731946688;    float autwIUTyvKRmVCnxSXIqwMM8025657 = -741288570;    float autwIUTyvKRmVCnxSXIqwMM93138538 = -570933794;    float autwIUTyvKRmVCnxSXIqwMM9455121 = -741365743;    float autwIUTyvKRmVCnxSXIqwMM28645393 = -346453880;    float autwIUTyvKRmVCnxSXIqwMM23136388 = -542537086;    float autwIUTyvKRmVCnxSXIqwMM97825029 = -614204539;    float autwIUTyvKRmVCnxSXIqwMM68724400 = -696069775;    float autwIUTyvKRmVCnxSXIqwMM84554476 = -23621863;    float autwIUTyvKRmVCnxSXIqwMM19333186 = -579544139;    float autwIUTyvKRmVCnxSXIqwMM69696681 = -864587784;    float autwIUTyvKRmVCnxSXIqwMM2527353 = 79518333;    float autwIUTyvKRmVCnxSXIqwMM7222204 = -392669323;    float autwIUTyvKRmVCnxSXIqwMM98037117 = -520073699;    float autwIUTyvKRmVCnxSXIqwMM56005175 = -201846420;    float autwIUTyvKRmVCnxSXIqwMM85368057 = -635008402;    float autwIUTyvKRmVCnxSXIqwMM18741543 = -149239758;     autwIUTyvKRmVCnxSXIqwMM22867031 = autwIUTyvKRmVCnxSXIqwMM36848259;     autwIUTyvKRmVCnxSXIqwMM36848259 = autwIUTyvKRmVCnxSXIqwMM75309520;     autwIUTyvKRmVCnxSXIqwMM75309520 = autwIUTyvKRmVCnxSXIqwMM65048521;     autwIUTyvKRmVCnxSXIqwMM65048521 = autwIUTyvKRmVCnxSXIqwMM73113000;     autwIUTyvKRmVCnxSXIqwMM73113000 = autwIUTyvKRmVCnxSXIqwMM70003022;     autwIUTyvKRmVCnxSXIqwMM70003022 = autwIUTyvKRmVCnxSXIqwMM72914265;     autwIUTyvKRmVCnxSXIqwMM72914265 = autwIUTyvKRmVCnxSXIqwMM80212057;     autwIUTyvKRmVCnxSXIqwMM80212057 = autwIUTyvKRmVCnxSXIqwMM78969523;     autwIUTyvKRmVCnxSXIqwMM78969523 = autwIUTyvKRmVCnxSXIqwMM49217013;     autwIUTyvKRmVCnxSXIqwMM49217013 = autwIUTyvKRmVCnxSXIqwMM96345491;     autwIUTyvKRmVCnxSXIqwMM96345491 = autwIUTyvKRmVCnxSXIqwMM50373132;     autwIUTyvKRmVCnxSXIqwMM50373132 = autwIUTyvKRmVCnxSXIqwMM17036302;     autwIUTyvKRmVCnxSXIqwMM17036302 = autwIUTyvKRmVCnxSXIqwMM53514074;     autwIUTyvKRmVCnxSXIqwMM53514074 = autwIUTyvKRmVCnxSXIqwMM26805599;     autwIUTyvKRmVCnxSXIqwMM26805599 = autwIUTyvKRmVCnxSXIqwMM52840587;     autwIUTyvKRmVCnxSXIqwMM52840587 = autwIUTyvKRmVCnxSXIqwMM96043701;     autwIUTyvKRmVCnxSXIqwMM96043701 = autwIUTyvKRmVCnxSXIqwMM41503594;     autwIUTyvKRmVCnxSXIqwMM41503594 = autwIUTyvKRmVCnxSXIqwMM23259994;     autwIUTyvKRmVCnxSXIqwMM23259994 = autwIUTyvKRmVCnxSXIqwMM95621821;     autwIUTyvKRmVCnxSXIqwMM95621821 = autwIUTyvKRmVCnxSXIqwMM38256656;     autwIUTyvKRmVCnxSXIqwMM38256656 = autwIUTyvKRmVCnxSXIqwMM99553387;     autwIUTyvKRmVCnxSXIqwMM99553387 = autwIUTyvKRmVCnxSXIqwMM8655996;     autwIUTyvKRmVCnxSXIqwMM8655996 = autwIUTyvKRmVCnxSXIqwMM7267288;     autwIUTyvKRmVCnxSXIqwMM7267288 = autwIUTyvKRmVCnxSXIqwMM95743228;     autwIUTyvKRmVCnxSXIqwMM95743228 = autwIUTyvKRmVCnxSXIqwMM23400286;     autwIUTyvKRmVCnxSXIqwMM23400286 = autwIUTyvKRmVCnxSXIqwMM42233945;     autwIUTyvKRmVCnxSXIqwMM42233945 = autwIUTyvKRmVCnxSXIqwMM17013557;     autwIUTyvKRmVCnxSXIqwMM17013557 = autwIUTyvKRmVCnxSXIqwMM59642403;     autwIUTyvKRmVCnxSXIqwMM59642403 = autwIUTyvKRmVCnxSXIqwMM64410711;     autwIUTyvKRmVCnxSXIqwMM64410711 = autwIUTyvKRmVCnxSXIqwMM10394825;     autwIUTyvKRmVCnxSXIqwMM10394825 = autwIUTyvKRmVCnxSXIqwMM80549519;     autwIUTyvKRmVCnxSXIqwMM80549519 = autwIUTyvKRmVCnxSXIqwMM1825645;     autwIUTyvKRmVCnxSXIqwMM1825645 = autwIUTyvKRmVCnxSXIqwMM48570321;     autwIUTyvKRmVCnxSXIqwMM48570321 = autwIUTyvKRmVCnxSXIqwMM21138672;     autwIUTyvKRmVCnxSXIqwMM21138672 = autwIUTyvKRmVCnxSXIqwMM96916663;     autwIUTyvKRmVCnxSXIqwMM96916663 = autwIUTyvKRmVCnxSXIqwMM52596709;     autwIUTyvKRmVCnxSXIqwMM52596709 = autwIUTyvKRmVCnxSXIqwMM88638478;     autwIUTyvKRmVCnxSXIqwMM88638478 = autwIUTyvKRmVCnxSXIqwMM19409396;     autwIUTyvKRmVCnxSXIqwMM19409396 = autwIUTyvKRmVCnxSXIqwMM30296429;     autwIUTyvKRmVCnxSXIqwMM30296429 = autwIUTyvKRmVCnxSXIqwMM63122911;     autwIUTyvKRmVCnxSXIqwMM63122911 = autwIUTyvKRmVCnxSXIqwMM18606772;     autwIUTyvKRmVCnxSXIqwMM18606772 = autwIUTyvKRmVCnxSXIqwMM70563987;     autwIUTyvKRmVCnxSXIqwMM70563987 = autwIUTyvKRmVCnxSXIqwMM24190247;     autwIUTyvKRmVCnxSXIqwMM24190247 = autwIUTyvKRmVCnxSXIqwMM34537515;     autwIUTyvKRmVCnxSXIqwMM34537515 = autwIUTyvKRmVCnxSXIqwMM25889328;     autwIUTyvKRmVCnxSXIqwMM25889328 = autwIUTyvKRmVCnxSXIqwMM59343939;     autwIUTyvKRmVCnxSXIqwMM59343939 = autwIUTyvKRmVCnxSXIqwMM50924491;     autwIUTyvKRmVCnxSXIqwMM50924491 = autwIUTyvKRmVCnxSXIqwMM66577567;     autwIUTyvKRmVCnxSXIqwMM66577567 = autwIUTyvKRmVCnxSXIqwMM54503120;     autwIUTyvKRmVCnxSXIqwMM54503120 = autwIUTyvKRmVCnxSXIqwMM56385055;     autwIUTyvKRmVCnxSXIqwMM56385055 = autwIUTyvKRmVCnxSXIqwMM17256286;     autwIUTyvKRmVCnxSXIqwMM17256286 = autwIUTyvKRmVCnxSXIqwMM71094399;     autwIUTyvKRmVCnxSXIqwMM71094399 = autwIUTyvKRmVCnxSXIqwMM73180251;     autwIUTyvKRmVCnxSXIqwMM73180251 = autwIUTyvKRmVCnxSXIqwMM25433934;     autwIUTyvKRmVCnxSXIqwMM25433934 = autwIUTyvKRmVCnxSXIqwMM23313643;     autwIUTyvKRmVCnxSXIqwMM23313643 = autwIUTyvKRmVCnxSXIqwMM28192264;     autwIUTyvKRmVCnxSXIqwMM28192264 = autwIUTyvKRmVCnxSXIqwMM68042233;     autwIUTyvKRmVCnxSXIqwMM68042233 = autwIUTyvKRmVCnxSXIqwMM69305293;     autwIUTyvKRmVCnxSXIqwMM69305293 = autwIUTyvKRmVCnxSXIqwMM49712714;     autwIUTyvKRmVCnxSXIqwMM49712714 = autwIUTyvKRmVCnxSXIqwMM27769077;     autwIUTyvKRmVCnxSXIqwMM27769077 = autwIUTyvKRmVCnxSXIqwMM55900708;     autwIUTyvKRmVCnxSXIqwMM55900708 = autwIUTyvKRmVCnxSXIqwMM20569654;     autwIUTyvKRmVCnxSXIqwMM20569654 = autwIUTyvKRmVCnxSXIqwMM14558812;     autwIUTyvKRmVCnxSXIqwMM14558812 = autwIUTyvKRmVCnxSXIqwMM38822189;     autwIUTyvKRmVCnxSXIqwMM38822189 = autwIUTyvKRmVCnxSXIqwMM15795973;     autwIUTyvKRmVCnxSXIqwMM15795973 = autwIUTyvKRmVCnxSXIqwMM48547487;     autwIUTyvKRmVCnxSXIqwMM48547487 = autwIUTyvKRmVCnxSXIqwMM68465980;     autwIUTyvKRmVCnxSXIqwMM68465980 = autwIUTyvKRmVCnxSXIqwMM32375402;     autwIUTyvKRmVCnxSXIqwMM32375402 = autwIUTyvKRmVCnxSXIqwMM29888935;     autwIUTyvKRmVCnxSXIqwMM29888935 = autwIUTyvKRmVCnxSXIqwMM243879;     autwIUTyvKRmVCnxSXIqwMM243879 = autwIUTyvKRmVCnxSXIqwMM7405223;     autwIUTyvKRmVCnxSXIqwMM7405223 = autwIUTyvKRmVCnxSXIqwMM22094199;     autwIUTyvKRmVCnxSXIqwMM22094199 = autwIUTyvKRmVCnxSXIqwMM92963564;     autwIUTyvKRmVCnxSXIqwMM92963564 = autwIUTyvKRmVCnxSXIqwMM32498910;     autwIUTyvKRmVCnxSXIqwMM32498910 = autwIUTyvKRmVCnxSXIqwMM19649884;     autwIUTyvKRmVCnxSXIqwMM19649884 = autwIUTyvKRmVCnxSXIqwMM28989401;     autwIUTyvKRmVCnxSXIqwMM28989401 = autwIUTyvKRmVCnxSXIqwMM84465748;     autwIUTyvKRmVCnxSXIqwMM84465748 = autwIUTyvKRmVCnxSXIqwMM72729772;     autwIUTyvKRmVCnxSXIqwMM72729772 = autwIUTyvKRmVCnxSXIqwMM69853900;     autwIUTyvKRmVCnxSXIqwMM69853900 = autwIUTyvKRmVCnxSXIqwMM64056347;     autwIUTyvKRmVCnxSXIqwMM64056347 = autwIUTyvKRmVCnxSXIqwMM91309453;     autwIUTyvKRmVCnxSXIqwMM91309453 = autwIUTyvKRmVCnxSXIqwMM50435990;     autwIUTyvKRmVCnxSXIqwMM50435990 = autwIUTyvKRmVCnxSXIqwMM5139284;     autwIUTyvKRmVCnxSXIqwMM5139284 = autwIUTyvKRmVCnxSXIqwMM8025657;     autwIUTyvKRmVCnxSXIqwMM8025657 = autwIUTyvKRmVCnxSXIqwMM93138538;     autwIUTyvKRmVCnxSXIqwMM93138538 = autwIUTyvKRmVCnxSXIqwMM9455121;     autwIUTyvKRmVCnxSXIqwMM9455121 = autwIUTyvKRmVCnxSXIqwMM28645393;     autwIUTyvKRmVCnxSXIqwMM28645393 = autwIUTyvKRmVCnxSXIqwMM23136388;     autwIUTyvKRmVCnxSXIqwMM23136388 = autwIUTyvKRmVCnxSXIqwMM97825029;     autwIUTyvKRmVCnxSXIqwMM97825029 = autwIUTyvKRmVCnxSXIqwMM68724400;     autwIUTyvKRmVCnxSXIqwMM68724400 = autwIUTyvKRmVCnxSXIqwMM84554476;     autwIUTyvKRmVCnxSXIqwMM84554476 = autwIUTyvKRmVCnxSXIqwMM19333186;     autwIUTyvKRmVCnxSXIqwMM19333186 = autwIUTyvKRmVCnxSXIqwMM69696681;     autwIUTyvKRmVCnxSXIqwMM69696681 = autwIUTyvKRmVCnxSXIqwMM2527353;     autwIUTyvKRmVCnxSXIqwMM2527353 = autwIUTyvKRmVCnxSXIqwMM7222204;     autwIUTyvKRmVCnxSXIqwMM7222204 = autwIUTyvKRmVCnxSXIqwMM98037117;     autwIUTyvKRmVCnxSXIqwMM98037117 = autwIUTyvKRmVCnxSXIqwMM56005175;     autwIUTyvKRmVCnxSXIqwMM56005175 = autwIUTyvKRmVCnxSXIqwMM85368057;     autwIUTyvKRmVCnxSXIqwMM85368057 = autwIUTyvKRmVCnxSXIqwMM18741543;     autwIUTyvKRmVCnxSXIqwMM18741543 = autwIUTyvKRmVCnxSXIqwMM22867031;}





































































































































































































































































































































































































































































































































































































































































































































