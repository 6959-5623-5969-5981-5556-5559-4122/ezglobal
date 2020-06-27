// *********************************
// *             GuukGang              *
// *           Solutions           *
// *********************************
// *           RAGEBOT			   *
// *********************************


#include "RageBot.h"
#include "Render.h"
#include "Autowall.h"
#include <iostream>
#include "MathFunctions.h"
#include "SDK.h"
#include "EnginePrediction.h"
#include "LagComp.h"
#include "hacks.h"
#include "stdlib.h"
using namespace std;
static bool dir = false;
static bool back = false;
static bool up = false;
static bool anneeeen = false;
static bool freeze = true;

#define TICK_INTERVAL			(g_Globals->interval_per_tick)
#define TICKS_TO_TIME( t )		( g_Globals->interval_per_tick *( t ) )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
ragebot::ragebot()
{
	IsLocked = false;
	TargetID = -1;
	pTarget = nullptr;
}

void ragebot::OnCreateMove(CInput::CUserCmd *pCmd, bool& bSendPacket)
{
	if (!g_Options.Ragebot.Enabled)
		return;
	if (g_Options.Ragebot.FakeLag)
		FakeLag(pCmd);
	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	if (pLocal && pLocal->IsAlive())
	{
		if (g_Options.Ragebot.BAIMkey && G::PressedKeys[g_Options.Ragebot.BAIMkey] && g_Options.Ragebot.Hitscan != 4)
		{
			g_Options.Ragebot.Hitscan = 4;
		}
		else if (g_Options.Ragebot.BAIMkey && !G::PressedKeys[g_Options.Ragebot.BAIMkey] && g_Options.Ragebot.Hitscan != 3)
		{
			g_Options.Ragebot.Hitscan = 3;
		}

		CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
		if (weapon && weapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex() == 64)
		{
			if (!CanAttack() && weapon->ammo() > 0)
			{
				pCmd->buttons |= IN_ATTACK;
			}
		}


		if (g_Options.Ragebot.Enabled)
			DoAimbot(pCmd, bSendPacket);

		if (g_Options.Ragebot.AntiRecoil)
			DoNoRecoil(pCmd);



		if (g_Options.Ragebot.EnabledAntiAim)
			DoAntiAim(pCmd, bSendPacket);
		// Junk
		bool LZExPzaKcRFJZaYviPIbpazivnrQ = true;
bool EKvoxIEyvlgAzIABPMVYnEHjFkDgcL;
		// JUnk

	}
}
bool ragebot::hit_chance(C_BaseEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon, C_BaseEntity* target)
{
	Vector forward, right, up;

	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	constexpr auto max_traces = 256;

	AngleVectors(cmd->viewangles, &forward, &right, &up);

	int total_hits = 0;

	int needed_hits;

	if (MiscFunctions::IsSniper(pWeapon))
	{
		needed_hits = static_cast<int>(max_traces * (g_Options.Ragebot.HitchanceSniper / 100.f));
	}
	else if (MiscFunctions::IsPistol(pWeapon))
	{
		needed_hits = static_cast<int>(max_traces * (g_Options.Ragebot.HitchancePistol / 100.f));
	}
	else if (MiscFunctions::IsHeavy(pWeapon))
	{
		needed_hits = static_cast<int>(max_traces * (g_Options.Ragebot.HitchanceHeavy / 100.f));
	}
	else if (MiscFunctions::IsSmg(pWeapon))
	{
		needed_hits = static_cast<int>(max_traces * (g_Options.Ragebot.HitchanceSmgs / 100.f));
	}
	else if (MiscFunctions::IsRifle(pWeapon))
	{
		needed_hits = static_cast<int>(max_traces * (g_Options.Ragebot.HitchanceRifle / 100.f));
	}
	else if (MiscFunctions::IsRevolver(pWeapon))
	{
		needed_hits = static_cast<int>(max_traces * (g_Options.Ragebot.HitchanceRevolver / 100.f));
	}

	weapon->UpdateAccuracyPenalty(weapon);

	auto eyes = local->GetEyePosition();
	auto flRange = weapon->GetCSWpnData()->range;

	for (int i = 0; i < max_traces; i++) {
		RandomSeed(i + 1);

		float fRand1 = RandomFloat(0.f, 1.f);
		float fRandPi1 = RandomFloat(0.f, XM_2PI);
		float fRand2 = RandomFloat(0.f, 1.f);
		float fRandPi2 = RandomFloat(0.f, XM_2PI);

		float fRandInaccuracy = fRand1 * weapon->GetInaccuracy();
		float fRandSpread = fRand2 * weapon->GetSpread();

		float fSpreadX = cos(fRandPi1) * fRandInaccuracy + cos(fRandPi2) * fRandSpread;
		float fSpreadY = sin(fRandPi1) * fRandInaccuracy + sin(fRandPi2) * fRandSpread;

		auto viewSpreadForward = (forward + fSpreadX * right + fSpreadY * up).Normalized();

		Vector viewAnglesSpread;
		VectorAngles(viewSpreadForward, viewAnglesSpread);
		MiscFunctions::NormaliseViewAngle(viewAnglesSpread);

		Vector viewForward;
		AngleVectors(viewAnglesSpread, &viewForward);
		viewForward.NormalizeInPlace();

		viewForward = eyes + (viewForward * flRange);

		trace_t tr;
		Ray_t ray;
		ray.Init(eyes, viewForward);

		g_EngineTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, target, &tr);


		if (tr.m_pEnt == target)
			total_hits++;

		if (total_hits >= needed_hits)
			return true;

		if ((max_traces - i + total_hits) < needed_hits)
			return false;
	}

	return false;
}
template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;

}

void ragebot::DoAimbot(CInput::CUserCmd *pCmd, bool& bSendPacket)
{

	C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	bool FindNewTarget = true;
	//IsLocked = false;

	// Don't aimbot with the knife..
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (pWeapon != nullptr)
	{

		if (pWeapon->ammo() == 0 || MiscFunctions::IsKnife(pWeapon) || MiscFunctions::IsGrenade(pWeapon))
		{
			//TargetID = 0;
			//pTarget = nullptr;
			//HitBox = -1;
			return;
		}
	}
	else
		return;

	// Make sure we have a good target
	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = g_EntityList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			HitBox = HitScan(pTarget);
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View; g_Engine->GetViewAngles(View);
				float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (FoV < g_Options.Ragebot.FOV)
					FindNewTarget = false;
			}
		}
	}



	// Find a new target, apparently we need to
	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;


		TargetID = GetTargetCrosshair();


		// Memesj
		if (TargetID >= 0)
		{
			pTarget = g_EntityList->GetClientEntity(TargetID);
		}
	}

	if (TargetID >= 0 && pTarget)
	{
		HitBox = HitScan(pTarget);

		// Key
		if (g_Options.Ragebot.KeyPress)
		{
			if (g_Options.Ragebot.KeyPress > 0 && !G::PressedKeys[g_Options.Ragebot.KeyPress])
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}


		Vector AimPoint = GetHitboxPosition(pTarget, HitBox);

		if (AimAtPoint(pLocal, AimPoint, pCmd))
		{
			if (g_Options.Ragebot.AutoFire && CanAttack() && MiscFunctions::IsSniper(pWeapon) && g_Options.Ragebot.otozoom)
			{
				if (pLocal->IsScoped()) if (!g_Options.Ragebot.Hitchance || hit_chance(pLocal, pCmd, pWeapon, pTarget)) pCmd->buttons |= IN_ATTACK;
				if (!pLocal->IsScoped()) pCmd->buttons |= IN_ATTACK2;
			}
			if (g_Options.Ragebot.AutoFire && CanAttack() && !(MiscFunctions::IsSniper(pWeapon)))
			{
				if (!g_Options.Ragebot.Hitchance || hit_chance(pLocal, pCmd, pWeapon, pTarget)) pCmd->buttons |= IN_ATTACK;
			}
			if (g_Options.Ragebot.AutoFire && CanAttack() && (MiscFunctions::IsSniper(pWeapon)) && !g_Options.Ragebot.otozoom)
			{
				if (!g_Options.Ragebot.Hitchance || hit_chance(pLocal, pCmd, pWeapon, pTarget)) if (pLocal->IsScoped()) pCmd->buttons |= IN_ATTACK;
			}
		}




		if (g_Options.Ragebot.otodurma)
		{
			pCmd->forwardmove = 0.f;
			pCmd->sidemove = 0.f;
		}



		if (g_Options.Ragebot.ototur)
		{
			pCmd->buttons |= IN_DUCK;
		}

	}

	// Auto Pistol
	static bool WasFiring = false;
	if (pWeapon != nullptr)
	{
		if (MiscFunctions::IsPistol(pWeapon) && g_Options.Ragebot.AutoPistol && pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex() != 64)
		{
			if (pCmd->buttons & IN_ATTACK && !MiscFunctions::IsKnife(pWeapon) && !MiscFunctions::IsGrenade(pWeapon))
			{
				if (WasFiring)
				{
					pCmd->buttons &= ~IN_ATTACK;
				}
			}

			WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
		}
	}

}



bool ragebot::TargetMeetsRequirements(C_BaseEntity* pEntity)
{
	auto local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	// Is a valid player
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != local->GetIndex())
	{
		// Entity Type checks
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)ClassID::ClassId_CCSPlayer && g_Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (pEntity->GetTeamNum() != local->GetTeamNum() || g_Options.Ragebot.FriendlyFire)
			{
				// Spawn Check
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}

	// They must have failed a requirement
	return false;
}




float ragebot::FovToPlayer(Vector ViewOffSet, Vector View, C_BaseEntity* pEntity, int aHitBox)
{
	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 180.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox); //pvs fix disabled
														 // Get delta vector between our local eye position and passed vector
	VectorSubtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int ragebot::GetTargetCrosshair()
{
	// Target selection
	int target = -1;
	float minFoV = g_Options.Ragebot.FOV;

	C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; g_Engine->GetViewAngles(View);

	for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity *pEntity = g_EntityList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minFoV)
				{
					minFoV = fov;
					target = i;
				}
			}
		}
	}

	return target;
}


int ragebot::HitScan(C_BaseEntity* pEntity)
{
	vector<int> HitBoxesToScan{ Head , Neck, Chest, Stomach };

	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	int HitScanMode = g_Options.Ragebot.Hitscan;

	if (!g_Options.Ragebot.Hitscan)
	{
		switch (g_Options.Ragebot.Hitbox)
		{
		case 0:
			HitBoxesToScan.push_back(Head);
			break;
		case 1:
			HitBoxesToScan.push_back(Neck);
			break;
		case 2:
			HitBoxesToScan.push_back(Stomach);
			break;
		case 3:
			HitBoxesToScan.push_back(Pelvis);
		}
	}
	else
	{
		switch (HitScanMode)
		{
		case 1:
			// low -- vitals
			HitBoxesToScan.push_back(Head);
			HitBoxesToScan.push_back(Neck);
			HitBoxesToScan.push_back(UpperChest);
			HitBoxesToScan.push_back(Chest);
			HitBoxesToScan.push_back(Stomach);
			HitBoxesToScan.push_back(Pelvis);
			HitBoxesToScan.push_back(LeftFoot);
			HitBoxesToScan.push_back(RightFoot);
			HitBoxesToScan.push_back(LeftLowerArm);
			HitBoxesToScan.push_back(RightLowerArm);
			HitBoxesToScan.push_back(LeftUpperArm);
			HitBoxesToScan.push_back(RightUpperArm);
			break;
		case 2:
			// medium 
			HitBoxesToScan.push_back(Head);
			HitBoxesToScan.push_back(Neck);
			HitBoxesToScan.push_back(UpperChest);
			HitBoxesToScan.push_back(Chest);
			HitBoxesToScan.push_back(Stomach);
			HitBoxesToScan.push_back(Pelvis);
			HitBoxesToScan.push_back(LeftThigh);
			HitBoxesToScan.push_back(RightThigh);
			HitBoxesToScan.push_back(LeftFoot);
			HitBoxesToScan.push_back(RightFoot);
			HitBoxesToScan.push_back(LeftShin);
			HitBoxesToScan.push_back(RightShin);
			HitBoxesToScan.push_back(LeftLowerArm);
			HitBoxesToScan.push_back(RightLowerArm);
			HitBoxesToScan.push_back(LeftUpperArm);
			HitBoxesToScan.push_back(RightUpperArm);
			HitBoxesToScan.push_back(LeftHand);
			HitBoxesToScan.push_back(RightHand);
			break;
		
		case 3:
			// Body parts
			HitBoxesToScan.push_back(UpperChest);
			HitBoxesToScan.push_back(Chest);
			HitBoxesToScan.push_back(Stomach);
			HitBoxesToScan.push_back(Pelvis);
			HitBoxesToScan.push_back(LeftUpperArm);
			HitBoxesToScan.push_back(RightUpperArm);
			HitBoxesToScan.push_back(LeftThigh);
			HitBoxesToScan.push_back(RightThigh);
			HitBoxesToScan.push_back(LeftHand);
			HitBoxesToScan.push_back(RightHand);
			HitBoxesToScan.push_back(LeftFoot);
			HitBoxesToScan.push_back(RightFoot);
			HitBoxesToScan.push_back(LeftShin);
			HitBoxesToScan.push_back(RightShin);
			HitBoxesToScan.push_back(LeftLowerArm);
			HitBoxesToScan.push_back(RightLowerArm);
			break;
		}
	}
	static vector<int> baim{ UpperChest ,Chest ,Stomach ,Pelvis ,LeftUpperArm ,RightUpperArm ,LeftThigh,RightThigh ,LeftHand ,RightHand, LeftFoot, RightFoot, LeftShin, RightShin,LeftLowerArm,RightLowerArm };

	float highestDamage;
	int bestHitbox = -1;

	if (MiscFunctions::IsSniper(pWeapon))
	{
		highestDamage = g_Options.Ragebot.MinimumDamageSniper;
	}
	else if (MiscFunctions::IsPistol(pWeapon))
	{
		highestDamage = g_Options.Ragebot.MinimumDamagePistol;
	}
	else if (MiscFunctions::IsHeavy(pWeapon))
	{
		highestDamage = g_Options.Ragebot.MinimumDamageHeavy;
	}
	else if (MiscFunctions::IsSmg(pWeapon))
	{
		highestDamage = g_Options.Ragebot.MinimumDamageSmg;
	}
	else if (MiscFunctions::IsRifle(pWeapon))
	{
		highestDamage = g_Options.Ragebot.MinimumDamageRifle;
	}
	else if (MiscFunctions::IsRevolver(pWeapon))
	{
		highestDamage = g_Options.Ragebot.MinimumDamageRevolver;
	}

	for (auto HitBoxID : HitBoxesToScan)
	{

		Vector Point = GetHitboxPosition(pEntity, HitBoxID); //pvs fix disabled

		float damage = 0.0f;
		if (CanHit(pEntity, Point, &damage))
		{
			if (damage > highestDamage || damage > pEntity->GetHealth())
			{
				bestHitbox = HitBoxID;
				highestDamage = damage;
			}
		}
	}
 
	return bestHitbox;

}



/* LBY DELTA TYPES
g_Options.Ragebot.LBYDeltaReal
g_Options.Ragebot.LBYDeltaFake
*/

void ragebot::DoNoRecoil(CInput::CUserCmd *pCmd)
{
	// Ghetto rcs shit, implement properly later
	C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	if (pLocal != nullptr)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			pCmd->viewangles -= AimPunch * 2;
			MiscFunctions::NormaliseViewAngle(pCmd->viewangles);
		}
	}
}

float FovToPoint(Vector ViewOffSet, Vector View, Vector Point)
{
	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(View, &Forward);
	Vector AimPos = Point;

	// Get delta vector between our local eye position and passed vector
	Delta = AimPos - Origin;
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (180.f / PI));
}
bool me123 = false;
bool ragebot::AimAtPoint(C_BaseEntity* pLocal, Vector point, CInput::CUserCmd *pCmd)
{
	bool ReturnValue = false;

	if (point.Length() == 0) return ReturnValue;

	Vector angles;

	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	VectorAngles(point - src, angles);





	IsLocked = true;
	ReturnValue = true;



	if (g_Options.Ragebot.Silent)
	{
		if (CanAttack()) {
			pCmd->viewangles = angles;
		}
	}

	if (!g_Options.Ragebot.Silent)
	{
		pCmd->viewangles = angles;
		g_Engine->SetViewAngles(pCmd->viewangles);
	}
	return ReturnValue;
}




void NormalizeVector(Vector& vec) {
	for (int i = 0; i < 3; ++i) {
		while (vec[i] > 180.f)
			vec[i] -= 360.f;

		while (vec[i] < -180.f)
			vec[i] += 360.f;
	}
	vec[2] = 0.f;
}


void VectorAngles2(const Vector &vecForward, Vector &vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = vec_t(atan2(vecForward[1], vecForward[0]) * 180.f / M_PI);

		if (vecView[1] < 0.f)
			vecView[1] += 360.f;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = vec_t(atan2(vecForward[2], vecView[2]) * 180.f / M_PI);
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}




bool EdgeAntiAim(C_BaseEntity* pLocalBaseEntity, CInput::CUserCmd* cmd, float flWall, float flCornor)
{
	Ray_t ray;
	trace_t tr;

	CTraceFilter traceFilter;
	traceFilter.pSkip = pLocalBaseEntity;

	auto bRetVal = false;
	auto vecCurPos = pLocalBaseEntity->GetEyePosition();

	for (float i = 0; i < 360; i++)
	{
		Vector vecDummy(10.f, cmd->viewangles.y, 0.f);
		vecDummy.y += i;

		NormalizeVector(vecDummy);

		Vector vecForward;
		AngleVectors2(vecDummy, vecForward);

		auto flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
		vecForward *= flLength;

		ray.Init(vecCurPos, (vecCurPos + vecForward));
		g_EngineTrace->TraceRay(ray, MASK_SHOT, (CTraceFilter *)&traceFilter, &tr);

		if (tr.fraction != 1.0f)
		{
			Vector qAngles;
			auto vecNegate = tr.plane.normal;

			vecNegate *= -1.f;
			VectorAngles2(vecNegate, qAngles);

			vecDummy.y = qAngles.y;

			NormalizeVector(vecDummy);
			trace_t leftTrace, rightTrace;

			Vector vecLeft;
			AngleVectors2(vecDummy + Vector(0.f, 30.f, 0.f), vecLeft);

			Vector vecRight;
			AngleVectors2(vecDummy - Vector(0.f, 30.f, 0.f), vecRight);

			vecLeft *= (flLength + (flLength * sin(DEG2RAD(30.f))));
			vecRight *= (flLength + (flLength * sin(DEG2RAD(30.f))));

			ray.Init(vecCurPos, (vecCurPos + vecLeft));
			g_EngineTrace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &leftTrace);

			ray.Init(vecCurPos, (vecCurPos + vecRight));
			g_EngineTrace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &rightTrace);

			if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
				vecDummy.y -= flCornor; // left
			else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
				vecDummy.y += flCornor; // right			

			cmd->viewangles.y = vecDummy.y;
			cmd->viewangles.y -= flWall;
			cmd->viewangles.x = 89.0f;
			bRetVal = true;
		}
	}
	return bRetVal;
}

void ragebot::Desync_AntiAim(CUserCmd* pCmd, C_BaseEntity* pLocal)
{
	if (freeze) {
		Vector	StartAngles;
	
		C_BaseEntity* localp = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

		float speed = localp->GetVelocity().Length2D();


		float range = 45.f;//antiaimconfig.flJitterRange;

	
		float server_time = hackManager.pLocal()->GetTickBase() * g_Globals->interval_per_tick;
		float time = TIME_TO_TICKS(server_time);

		while (time >= server_time)
			time = 0.f;

		float idk = rand() % 100;

		if (speed <= 10 && (pLocal->GetFlags() & FL_ONGROUND))
		{
			anneeeen = !anneeeen;
			if (time >= server_time / 2)
			{


				if (idk < 60)

				{

					if (!anneeeen)

						pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal+ range;




				}
				else
				{

					if (!anneeeen)

						pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal - range;

				}
			}
			else
			{
				if (idk < 65)
				{

					if (anneeeen)

						pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal - range;

				}
				else
				{
					if (anneeeen)

						pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal + range;


				}
			}

		}

		if (speed > 10 && (pLocal->GetFlags() & FL_ONGROUND))
		{

			anneeeen = !anneeeen;
			if (time >= server_time / 2)
			{

				if (anneeeen)
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal + range;


			}
			else

			{

				if (anneeeen)
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal - range;

			}

		}

		freeze = false;
	}


}


// AntiAim
void ragebot::DoAntiAim(CInput::CUserCmd *pCmd, bool& bSendPacket)
{
	C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	float PlayerIsMoving = abs(pLocal->GetVelocity().Length2D());

	// If the aimbot is doing something don't do anything
	if (pCmd->buttons & IN_ATTACK && CanAttack())
		return;
	if ((pCmd->buttons & IN_USE))
		return;
	if (pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;
 
	// Weapon shit

	if (pWeapon)
	{
		CSWeaponInfo* pWeaponInfo = pWeapon->GetCSWpnData();
		CCSGrenade* csGrenade = (CCSGrenade*)pWeapon;

		if (csGrenade->GetThrowTime() > 0.f)
			return;
	}

	// Don't do antiaim
	// if (DoExit) return;

	if (g_Options.Ragebot.Edge) 
	{
		auto bEdge = EdgeAntiAim(pLocal, pCmd, -90.f, 90.f);
		if (bEdge)
			return;
	}

 
	if (g_Options.Ragebot.YawFake != 0)	
		Globals::ySwitch = !Globals::ySwitch;
	else
		Globals::ySwitch = false;

	bSendPacket = Globals::ySwitch;

	Vector SpinAngles = { 0,0,0 };
	Vector FakeAngles = { 0,0,0 };
	float server_time = pLocal->GetTickBase() * g_Globals->interval_per_tick;
	static int ticks;
	static bool flip;
	if (ticks < 15 + rand() % 20)
		ticks++;
	else
	{
		flip = !flip;
		ticks = 0;
	}
	Vector StartAngles;
	double rate = 360.0 / 1.618033988749895;
	double yaw = fmod(static_cast<double>(server_time)*rate, 360.0);
	double factor = 360.0 / M_PI;
 
	factor *= 25;
	switch (g_Options.Ragebot.YawTrue)
	{
	case 1: //sideways
	{
		if (pLocal->GetFlags() & FL_ONGROUND)
		{
			anneeeen = !anneeeen;
			if (anneeeen)
			g_Engine->GetViewAngles(StartAngles);
			SpinAngles.y = StartAngles.y + 90.f;
		}
		else
		{
			anneeeen = !anneeeen;
			if (anneeeen)
			g_Engine->GetViewAngles(StartAngles);
			constexpr auto maxRange = 90.0f;
			constexpr auto angleAdditive = 180.0f;
			SpinAngles.y = StartAngles.y += angleAdditive - maxRange / 2.f + std::fmodf(server_time * 120, maxRange);
		}
	}
	break;

	break;
	case 2://backwards
	{
			if (pLocal->GetFlags() & FL_ONGROUND)
			{
				anneeeen = !anneeeen;
				if (anneeeen)
				g_Engine->GetViewAngles(StartAngles);
				SpinAngles.y = StartAngles.y + 180.f;
			}
			else
			{
				anneeeen = !anneeeen;
				if (anneeeen)
				g_Engine->GetViewAngles(StartAngles);
				constexpr auto maxRange = 90.0f;
				constexpr auto angleAdditive = 180.0f;
				SpinAngles.y = StartAngles.y += angleAdditive - maxRange / 2.f + std::fmodf(server_time * 120, maxRange);
			}
	}
	break;

	case 3: // interchaneable antiaim -- pasted from uc kinda lazy to do it on my own but this code kinda looks autistic
	{

		if (pLocal->GetFlags() && FL_ONGROUND)
		{
			g_Engine->GetViewAngles(StartAngles);

			static bool dir = false;

			static bool side = false;
			static bool side2 = false;
			static bool back = false;

			if (GetAsyncKeyState(VK_RIGHT))
			{
				side = true;
				side2 = false;
				back = false;
			}
			else if (GetAsyncKeyState(VK_LEFT))
			{
				side = false;
				side2 = true;
				back = false;
			}
			else if (GetAsyncKeyState(VK_DOWN))
			{
				side = false;
				side2 = false;
				back = true;
			}

			if (side)
				SpinAngles.y = StartAngles.y -= 90.f;
			else if (side2)
				SpinAngles.y = StartAngles.y += 90.f;
			else if (back)
				SpinAngles.y = StartAngles.y += 180.f;
			break;
		}
		else if (pLocal->GetFlags() & !FL_ONGROUND)
		{
			g_Engine->GetViewAngles(StartAngles);
			constexpr auto maxRange = 90.0f;
			constexpr auto angleAdditive = 180.0f;
			SpinAngles.y = StartAngles.y += angleAdditive - maxRange / 2.f + std::fmodf(server_time * 120, maxRange);
		}
	}

	break;
	case 4:
	{
		if (pLocal->GetFlags() & FL_ONGROUND)
		{
			g_Engine->GetViewAngles(StartAngles);

			static bool dir = false;
			static int jitterangle = 0;

			int dReal = g_Options.Ragebot.LBYDeltaReal;
			int dFake = g_Options.Ragebot.LBYDeltaFake;
			int dTrue = g_Options.Ragebot.YawTrueAdder;

			if (Globals::shouldflip)
			{
				SpinAngles.y = StartAngles.y - dReal;
			}
			else
			{
				SpinAngles.y = StartAngles.y + 180;
			}
		}
		else
		{
			g_Engine->GetViewAngles(StartAngles);
			constexpr auto maxRange = 90.0f;
			constexpr auto angleAdditive = 180.0f;
			SpinAngles.y = StartAngles.y += angleAdditive - maxRange / 2.f + std::fmodf(server_time * 120, maxRange);
		}
	}
	break;
	case 5: /* More autistic codes  */
	{
		if (pLocal->GetFlags() & FL_ONGROUND)
		{
			g_Engine->GetViewAngles(StartAngles);
			int dReal = g_Options.Ragebot.LBYDeltaReal;
			static bool side = false;
			static bool side2 = false;
			static bool back = false;

			if (GetAsyncKeyState(VK_RIGHT))
			{
				side = true;
				side2 = false;
				back = false;
			}
			else if (GetAsyncKeyState(VK_LEFT))
			{
				side = false;
				side2 = true;
				back = false;
			}
			else if (GetAsyncKeyState(VK_DOWN))
			{
				side = false;
				side2 = false;
				back = true;
			}

			if (side2)
			{
				if (Globals::shouldflip)
				{
					SpinAngles.y = StartAngles.y - dReal;
				}
				else
				{
					SpinAngles.y = StartAngles.y + 90;
				}
			}
			else if (side)
			{
				if (Globals::shouldflip)
				{
					SpinAngles.y = StartAngles.y + dReal;
				}
				else
				{
					SpinAngles.y = StartAngles.y - 90;
				}
			}
			else if (back)
			{
				if (Globals::shouldflip)
				{
					SpinAngles.y = StartAngles.y + dReal;
				}
				else
				{
					SpinAngles.y = StartAngles.y - 180;
				}

			}
		}
		else
		{
			g_Engine->GetViewAngles(StartAngles);
			constexpr auto maxRange = 90.0f;
			constexpr auto angleAdditive = 180.0f;
			SpinAngles.y = StartAngles.y += angleAdditive - maxRange / 2.f + std::fmodf(server_time * 120, maxRange);
		}
		
	}

	break;
	case 6:
	{
		g_Engine->GetViewAngles(StartAngles);
		constexpr auto maxRange = 90.0f;
		constexpr auto angleAdditive = 180.0f;
		SpinAngles.y = StartAngles.y += angleAdditive - maxRange / 2.f + std::fmodf(server_time * 120, maxRange);
	}
	break;
	case 7://slowspin
		SpinAngles.y += static_cast<float>(yaw);
		break;
	case 8://fastspin
	{
		SpinAngles.y = (float)(fmod(server_time / 0.05f * 360.0f, 360.0f));
	}
}




	switch (g_Options.Ragebot.YawFake)
	{
	case 1://sideways
	{
		g_Engine->GetViewAngles(StartAngles);
		FakeAngles.y = flip ? StartAngles.y + 90.f : StartAngles.y - 90.f;
	}
	break;
	case 2://slowspin
		FakeAngles.y += static_cast<float>(yaw);
		break;
	case 3://fastspin
		FakeAngles.y = (float)(fmod(server_time / 0.05f * 360.0f, 360.0f));
		break;
	case 4://backwards
	{
		g_Engine->GetViewAngles(StartAngles);

		StartAngles -= 180.f;
		FakeAngles = StartAngles;
	}
	break;
	case 5: //lby antiaim
	{
		g_Engine->GetViewAngles(StartAngles);
		static bool llamaflip;
		static float oldLBY = 0.0f;
		float LBY = pLocal->GetLowerBodyYaw();
		if (LBY != oldLBY) // did lowerbody update?
		{
			llamaflip = !llamaflip;
			oldLBY = LBY;
		}
		FakeAngles.y = llamaflip ? StartAngles.y - 90.f : StartAngles.y + 90.f;
	}
	break;
	case 6:
	{
		g_Engine->GetViewAngles(StartAngles);

		static bool dir = false;

		static bool side = false;
		static bool side2 = false;
		static bool back = false;

		if (GetAsyncKeyState(VK_RIGHT))
		{
			side = true;
			side2 = false;
			back = false;
		}
		else if (GetAsyncKeyState(VK_LEFT))
		{
			side = false;
			side2 = true;
			back = false;
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			side = false;
			side2 = false;
			back = true;
		}

		if (side)
			FakeAngles.y = StartAngles.y += 90;
		else if (side2)
			FakeAngles.y = StartAngles.y -= 90;
		else if (back)
			FakeAngles.y = StartAngles.y -= 180;
		break;


	}
	break;
	case 7:
	{
		g_Engine->GetViewAngles(StartAngles);
		static bool llamaflip;
		static float oldLBY = 0.0f;
		float LBY = pLocal->GetLowerBodyYaw();
		if (LBY != oldLBY) // did lowerbody update?
		{
			llamaflip = !llamaflip;
			oldLBY = LBY;
		}
		FakeAngles.y = llamaflip ? StartAngles.y - 135.f : StartAngles.y + 135.f;

	}
	break;
	case 8:
	{
		g_Engine->GetViewAngles(StartAngles);
		static bool dir = false;
		static int jitterangle = 0;

		if (GetAsyncKeyState(VK_LEFT)) dir = false; else if (GetAsyncKeyState(VK_RIGHT)) dir = true;

		if (dir && pLocal->GetVelocity().Length2D() < 1)
		{
			FakeAngles.y = StartAngles.y - 75;
			if (jitterangle <= 1)
			{
				FakeAngles.y = StartAngles.y - 75;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				FakeAngles.y = StartAngles.y - 105;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}
		else if (!dir && pLocal->GetVelocity().Length2D() < 1)
		{
			FakeAngles.y = StartAngles.y + 75;
			if (jitterangle <= 1)
			{
				FakeAngles.y = StartAngles.y + 75;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				FakeAngles.y = StartAngles.y + 105;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}
		else if (pLocal->GetVelocity().Length2D() > 0)
		{
			FakeAngles.y = StartAngles.y + 75;
			if (jitterangle <= 1)
			{
				FakeAngles.y = StartAngles.y + 75;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				FakeAngles.y = StartAngles.y + 105;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}
	}
	break;
	case 9:
	{
		g_Engine->GetViewAngles(StartAngles);

		static int jitterangle = 0;

		FakeAngles.y = StartAngles.y + 45 + RandomFloat(90, -90);
		if (jitterangle <= 1)
		{
			FakeAngles.y += StartAngles.y + 45 + RandomFloat(90, -90);
			jitterangle += 1;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			FakeAngles.y -= StartAngles.y - 45 - RandomFloat(90, -90);
			jitterangle += 1;
		}
		else
		{
			jitterangle = 0;
		}
	}
	break;

	}
 
 
 

	if (g_Options.Ragebot.EnabledAntiAim && !g_Options.Ragebot.BuilderAAs)
	{
		if (Globals::ySwitch && g_Options.Ragebot.YawTrue != 0)
			pCmd->viewangles.y = FakeAngles.y + g_Options.Ragebot.YawFakeAdder;
		if (!Globals::ySwitch && g_Options.Ragebot.YawFake != 0)
			pCmd->viewangles.y = SpinAngles.y + g_Options.Ragebot.YawTrueAdder;
	}
	else if (!g_Options.Ragebot.EnabledAntiAim && g_Options.Ragebot.BuilderAAs)
	{
		g_Engine->GetViewAngles(StartAngles);
		static int jitterangle;

		pCmd->viewangles.x = g_Options.Ragebot.BuilderPitch;

		if (Globals::ySwitch) //Builder FakeAngle
		{
			if (g_Options.Ragebot.FJitter)
			{
				pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderFake;
				if (jitterangle <= 1)
				{
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderFake + g_Options.Ragebot.FJitterRange;
					jitterangle += 1;
				}
				else if (jitterangle > 1 && jitterangle <= 3)
				{
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderFake - g_Options.Ragebot.FJitterRange;
					jitterangle += 1;
				}
				else
				{
					jitterangle = 0;
				}
			}
			else if (g_Options.Ragebot.FJitter && g_Options.Ragebot.LBYBreaker)
			{
				if (Globals::shouldflip)
				{
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderFake + 118;
				}
				else
				{
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderFake;
					if (jitterangle <= 1)
					{
						pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderFake + g_Options.Ragebot.FJitterRange;
						jitterangle += 1;
					}
					else if (jitterangle > 1 && jitterangle <= 3)
					{
						pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderFake - g_Options.Ragebot.FJitterRange;
						jitterangle += 1;
					}
					else
					{
						jitterangle = 0;
					}
				}
			}
			else if (!g_Options.Ragebot.FJitter && g_Options.Ragebot.LBYBreaker)
			{
				if (Globals::shouldflip)
				{
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderFake + 118;
				}
				else
				{
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderFake;
				}
			}
			else
			{
				pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderFake;
			}
		}
		if (!Globals::ySwitch) //Builder RealAngle
		{
			if (g_Options.Ragebot.Jitter)
			{
				pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal;
				if (jitterangle <= 1)
				{
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal + g_Options.Ragebot.JitterRange;
					jitterangle += 1;
				}
				else if (jitterangle > 1 && jitterangle <= 3)
				{
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal - g_Options.Ragebot.JitterRange;
					jitterangle += 1;
				}
				else
				{
					jitterangle = 0;
				}
			}
			else if (g_Options.Ragebot.Jitter && g_Options.Ragebot.LBYBreaker)
			{
				if (Globals::shouldflip)
				{
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal + 118;
				}
				else
				{
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal;
					if (jitterangle <= 1)
					{
						pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal + g_Options.Ragebot.JitterRange;
						jitterangle += 1;
					}
					else if (jitterangle > 1 && jitterangle <= 3)
					{
						pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal - g_Options.Ragebot.JitterRange;
						jitterangle += 1;
					}
					else
					{
						jitterangle = 0;
					}
				}
			}
			else if (!g_Options.Ragebot.Jitter && g_Options.Ragebot.LBYBreaker)
			{
				if (Globals::shouldflip)
				{
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal + 118;
				}
				else
				{
					pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal;
				}
			}
			else
			{
				pCmd->viewangles.y = StartAngles.y + g_Options.Ragebot.BuilderReal;
			}
		}

	}

	if (g_Options.Ragebot.EnabledAntiAim && g_Options.Ragebot.BuilderAAs)
	{
		Globals::error = true;
	}
 
	switch (g_Options.Ragebot.Pitch && g_Options.Ragebot.EnabledAntiAim)
	{
	case 0:
		pCmd->viewangles.x;
		break;
	case 1:
		// Down
		pCmd->viewangles.x = 89.f + g_Options.Ragebot.PitchAdder;
		break;
	case 2:
		pCmd->viewangles.x = -89.f + g_Options.Ragebot.PitchAdder;
		break;
	case 3:
		pCmd->viewangles.x = -180.f + g_Options.Ragebot.PitchAdder;
		break;
	case 4:
		pCmd->viewangles.x = 180.f + g_Options.Ragebot.PitchAdder;
		break;
	case 5:
		pCmd->viewangles.x = 1080.f + g_Options.Ragebot.PitchAdder;
		break;
	case 6:
		pCmd->viewangles.x = 0.f + g_Options.Ragebot.PitchAdder;
	case 7:
		if (Globals::shouldflip)
		{
			pCmd->viewangles.x = 60.f + g_Options.Ragebot.PitchAdder;
		}
		else if (!(Globals::shouldflip))
		{
			pCmd->viewangles.x = 89.f + g_Options.Ragebot.PitchAdder;
		}
		break;
	}

}
	
void ragebot::FakeLag(CInput::CUserCmd* cmd)
{
	int iChoke = g_Options.Ragebot.FakeLagAmt;

	static int iFakeLag = -1;
	iFakeLag++;

	if (iFakeLag <= iChoke && iFakeLag > -1)
	{
		Globals::ySwitch = false;
	}
	else
	{
		Globals::ySwitch = true;
		iFakeLag = -1;
	}
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class HVIOAJOIOH
{ 
  void kuHOiHVPRC()
  { 
      bool lppKaheLix = false;
      bool EEPUToyRbk = false;
      bool dfpIkysMLJ = false;
      bool biayANUdFO = false;
      bool cZeKGsOlMM = false;
      bool jPKbqGAhYc = false;
      bool nRAYVVdRTF = false;
      bool UTLMIDAeCq = false;
      bool iHzyZLXyGt = false;
      bool NlEcZKZGnj = false;
      bool PxRHrPDxTm = false;
      bool FUfhsgRBzJ = false;
      bool xeKZrhPimx = false;
      bool dtqRblprFJ = false;
      bool HZeTorGWjr = false;
      bool sXGemklrOK = false;
      bool AUKVIKdOGG = false;
      bool qZuoBXzpjz = false;
      bool bsztEFnLdp = false;
      bool zxduJmOILe = false;
      string MraAwuCxPY;
      string iweJJJwcTg;
      string sgcHBqExcb;
      string RfxFrcSNZm;
      string DqdxyHMDHH;
      string EZQhaeOTzE;
      string qFTmHDyZQg;
      string qiDgcCCfIt;
      string eApVWrOPeN;
      string OzOUuyWppu;
      string ImlaQUViEX;
      string biLEZqPyQO;
      string nhtDSrUike;
      string fMoNkqNPEC;
      string VfLGkemJtE;
      string hXZbGariIX;
      string AmyYrPsxSi;
      string cAsNrIVnSm;
      string EKogwHlNHK;
      string yeHgCigmNU;
      if(MraAwuCxPY == ImlaQUViEX){lppKaheLix = true;}
      else if(ImlaQUViEX == MraAwuCxPY){PxRHrPDxTm = true;}
      if(iweJJJwcTg == biLEZqPyQO){EEPUToyRbk = true;}
      else if(biLEZqPyQO == iweJJJwcTg){FUfhsgRBzJ = true;}
      if(sgcHBqExcb == nhtDSrUike){dfpIkysMLJ = true;}
      else if(nhtDSrUike == sgcHBqExcb){xeKZrhPimx = true;}
      if(RfxFrcSNZm == fMoNkqNPEC){biayANUdFO = true;}
      else if(fMoNkqNPEC == RfxFrcSNZm){dtqRblprFJ = true;}
      if(DqdxyHMDHH == VfLGkemJtE){cZeKGsOlMM = true;}
      else if(VfLGkemJtE == DqdxyHMDHH){HZeTorGWjr = true;}
      if(EZQhaeOTzE == hXZbGariIX){jPKbqGAhYc = true;}
      else if(hXZbGariIX == EZQhaeOTzE){sXGemklrOK = true;}
      if(qFTmHDyZQg == AmyYrPsxSi){nRAYVVdRTF = true;}
      else if(AmyYrPsxSi == qFTmHDyZQg){AUKVIKdOGG = true;}
      if(qiDgcCCfIt == cAsNrIVnSm){UTLMIDAeCq = true;}
      if(eApVWrOPeN == EKogwHlNHK){iHzyZLXyGt = true;}
      if(OzOUuyWppu == yeHgCigmNU){NlEcZKZGnj = true;}
      while(cAsNrIVnSm == qiDgcCCfIt){qZuoBXzpjz = true;}
      while(EKogwHlNHK == EKogwHlNHK){bsztEFnLdp = true;}
      while(yeHgCigmNU == yeHgCigmNU){zxduJmOILe = true;}
      if(lppKaheLix == true){lppKaheLix = false;}
      if(EEPUToyRbk == true){EEPUToyRbk = false;}
      if(dfpIkysMLJ == true){dfpIkysMLJ = false;}
      if(biayANUdFO == true){biayANUdFO = false;}
      if(cZeKGsOlMM == true){cZeKGsOlMM = false;}
      if(jPKbqGAhYc == true){jPKbqGAhYc = false;}
      if(nRAYVVdRTF == true){nRAYVVdRTF = false;}
      if(UTLMIDAeCq == true){UTLMIDAeCq = false;}
      if(iHzyZLXyGt == true){iHzyZLXyGt = false;}
      if(NlEcZKZGnj == true){NlEcZKZGnj = false;}
      if(PxRHrPDxTm == true){PxRHrPDxTm = false;}
      if(FUfhsgRBzJ == true){FUfhsgRBzJ = false;}
      if(xeKZrhPimx == true){xeKZrhPimx = false;}
      if(dtqRblprFJ == true){dtqRblprFJ = false;}
      if(HZeTorGWjr == true){HZeTorGWjr = false;}
      if(sXGemklrOK == true){sXGemklrOK = false;}
      if(AUKVIKdOGG == true){AUKVIKdOGG = false;}
      if(qZuoBXzpjz == true){qZuoBXzpjz = false;}
      if(bsztEFnLdp == true){bsztEFnLdp = false;}
      if(zxduJmOILe == true){zxduJmOILe = false;}
    } 
}; 


void fyWogxqVAfiuPqVqMlCtBERYaU14346167() {     float goQWBFRVDoekxdiUEUPJfBB32399401 = -668176869;    float goQWBFRVDoekxdiUEUPJfBB63210450 = -235226939;    float goQWBFRVDoekxdiUEUPJfBB84350905 = -72896361;    float goQWBFRVDoekxdiUEUPJfBB53593114 = -314750492;    float goQWBFRVDoekxdiUEUPJfBB34887619 = -33213404;    float goQWBFRVDoekxdiUEUPJfBB67387302 = 77621841;    float goQWBFRVDoekxdiUEUPJfBB38024813 = -926051239;    float goQWBFRVDoekxdiUEUPJfBB85743123 = 14779756;    float goQWBFRVDoekxdiUEUPJfBB16488311 = -856357358;    float goQWBFRVDoekxdiUEUPJfBB86770771 = -853261061;    float goQWBFRVDoekxdiUEUPJfBB74926907 = -447182820;    float goQWBFRVDoekxdiUEUPJfBB63355861 = -706883210;    float goQWBFRVDoekxdiUEUPJfBB79949191 = -788921305;    float goQWBFRVDoekxdiUEUPJfBB75170580 = -423452805;    float goQWBFRVDoekxdiUEUPJfBB90724479 = -201205485;    float goQWBFRVDoekxdiUEUPJfBB73798911 = 23432905;    float goQWBFRVDoekxdiUEUPJfBB4714422 = -526705545;    float goQWBFRVDoekxdiUEUPJfBB7261296 = -987632720;    float goQWBFRVDoekxdiUEUPJfBB23243199 = -333774134;    float goQWBFRVDoekxdiUEUPJfBB72316780 = -274621343;    float goQWBFRVDoekxdiUEUPJfBB70505084 = -200324224;    float goQWBFRVDoekxdiUEUPJfBB69368789 = -153130036;    float goQWBFRVDoekxdiUEUPJfBB59664713 = 67619972;    float goQWBFRVDoekxdiUEUPJfBB51992219 = -41932743;    float goQWBFRVDoekxdiUEUPJfBB95329599 = -557583655;    float goQWBFRVDoekxdiUEUPJfBB74188497 = -673500367;    float goQWBFRVDoekxdiUEUPJfBB4124297 = -113365636;    float goQWBFRVDoekxdiUEUPJfBB10496330 = -897725639;    float goQWBFRVDoekxdiUEUPJfBB29034761 = -607170319;    float goQWBFRVDoekxdiUEUPJfBB8707481 = -202191636;    float goQWBFRVDoekxdiUEUPJfBB27739897 = -480738001;    float goQWBFRVDoekxdiUEUPJfBB71163902 = -296428436;    float goQWBFRVDoekxdiUEUPJfBB4042135 = -415199582;    float goQWBFRVDoekxdiUEUPJfBB63366956 = -140787656;    float goQWBFRVDoekxdiUEUPJfBB25552085 = -167471665;    float goQWBFRVDoekxdiUEUPJfBB27025274 = -795877238;    float goQWBFRVDoekxdiUEUPJfBB88237906 = -54643475;    float goQWBFRVDoekxdiUEUPJfBB41187406 = -755730644;    float goQWBFRVDoekxdiUEUPJfBB79331374 = -255361882;    float goQWBFRVDoekxdiUEUPJfBB75397208 = -458258572;    float goQWBFRVDoekxdiUEUPJfBB63658615 = -49987795;    float goQWBFRVDoekxdiUEUPJfBB40003906 = -573062485;    float goQWBFRVDoekxdiUEUPJfBB56594284 = -238061616;    float goQWBFRVDoekxdiUEUPJfBB82765956 = -931954325;    float goQWBFRVDoekxdiUEUPJfBB38822505 = -865080987;    float goQWBFRVDoekxdiUEUPJfBB89479465 = 65158117;    float goQWBFRVDoekxdiUEUPJfBB86107059 = -596407277;    float goQWBFRVDoekxdiUEUPJfBB33900645 = -664384390;    float goQWBFRVDoekxdiUEUPJfBB25994615 = -493839743;    float goQWBFRVDoekxdiUEUPJfBB73450695 = -789956686;    float goQWBFRVDoekxdiUEUPJfBB61492637 = 24136990;    float goQWBFRVDoekxdiUEUPJfBB34652536 = -96116314;    float goQWBFRVDoekxdiUEUPJfBB40098696 = -29277879;    float goQWBFRVDoekxdiUEUPJfBB21716587 = -364292759;    float goQWBFRVDoekxdiUEUPJfBB43841082 = -314021267;    float goQWBFRVDoekxdiUEUPJfBB63030612 = -415046834;    float goQWBFRVDoekxdiUEUPJfBB3545738 = -202846912;    float goQWBFRVDoekxdiUEUPJfBB32358686 = 69036382;    float goQWBFRVDoekxdiUEUPJfBB58263515 = -757166838;    float goQWBFRVDoekxdiUEUPJfBB60699122 = -359713037;    float goQWBFRVDoekxdiUEUPJfBB63263006 = -809012523;    float goQWBFRVDoekxdiUEUPJfBB27528484 = 71674399;    float goQWBFRVDoekxdiUEUPJfBB56708363 = -378049925;    float goQWBFRVDoekxdiUEUPJfBB7780831 = -554165722;    float goQWBFRVDoekxdiUEUPJfBB59030875 = -272523060;    float goQWBFRVDoekxdiUEUPJfBB3763005 = -50754384;    float goQWBFRVDoekxdiUEUPJfBB59313726 = -191683629;    float goQWBFRVDoekxdiUEUPJfBB16582236 = -548133649;    float goQWBFRVDoekxdiUEUPJfBB49618496 = -155981141;    float goQWBFRVDoekxdiUEUPJfBB63699205 = -405328247;    float goQWBFRVDoekxdiUEUPJfBB85561004 = -921923621;    float goQWBFRVDoekxdiUEUPJfBB63527015 = -770974902;    float goQWBFRVDoekxdiUEUPJfBB27929922 = -632270839;    float goQWBFRVDoekxdiUEUPJfBB47845991 = -875515562;    float goQWBFRVDoekxdiUEUPJfBB8658165 = -124633549;    float goQWBFRVDoekxdiUEUPJfBB30501179 = -627261740;    float goQWBFRVDoekxdiUEUPJfBB12774505 = -915068420;    float goQWBFRVDoekxdiUEUPJfBB76898756 = -425703;    float goQWBFRVDoekxdiUEUPJfBB13169714 = -176851757;    float goQWBFRVDoekxdiUEUPJfBB5850134 = -522741773;    float goQWBFRVDoekxdiUEUPJfBB88081438 = 22906909;    float goQWBFRVDoekxdiUEUPJfBB70223651 = -448981246;    float goQWBFRVDoekxdiUEUPJfBB84501715 = -303885896;    float goQWBFRVDoekxdiUEUPJfBB55584065 = -817213633;    float goQWBFRVDoekxdiUEUPJfBB47214843 = -126328627;    float goQWBFRVDoekxdiUEUPJfBB93087360 = -284621688;    float goQWBFRVDoekxdiUEUPJfBB31065207 = -167150558;    float goQWBFRVDoekxdiUEUPJfBB82325548 = 49093177;    float goQWBFRVDoekxdiUEUPJfBB19525874 = -826766389;    float goQWBFRVDoekxdiUEUPJfBB62521472 = -752424831;    float goQWBFRVDoekxdiUEUPJfBB23479537 = -493030327;    float goQWBFRVDoekxdiUEUPJfBB55879220 = -23679857;    float goQWBFRVDoekxdiUEUPJfBB82923891 = -998563807;    float goQWBFRVDoekxdiUEUPJfBB18632252 = -895648845;    float goQWBFRVDoekxdiUEUPJfBB12134202 = -649246050;    float goQWBFRVDoekxdiUEUPJfBB36130132 = -21662194;    float goQWBFRVDoekxdiUEUPJfBB83295542 = -95012561;    float goQWBFRVDoekxdiUEUPJfBB48813453 = -683895894;    float goQWBFRVDoekxdiUEUPJfBB23735082 = -559431266;    float goQWBFRVDoekxdiUEUPJfBB35059501 = -668176869;     goQWBFRVDoekxdiUEUPJfBB32399401 = goQWBFRVDoekxdiUEUPJfBB63210450;     goQWBFRVDoekxdiUEUPJfBB63210450 = goQWBFRVDoekxdiUEUPJfBB84350905;     goQWBFRVDoekxdiUEUPJfBB84350905 = goQWBFRVDoekxdiUEUPJfBB53593114;     goQWBFRVDoekxdiUEUPJfBB53593114 = goQWBFRVDoekxdiUEUPJfBB34887619;     goQWBFRVDoekxdiUEUPJfBB34887619 = goQWBFRVDoekxdiUEUPJfBB67387302;     goQWBFRVDoekxdiUEUPJfBB67387302 = goQWBFRVDoekxdiUEUPJfBB38024813;     goQWBFRVDoekxdiUEUPJfBB38024813 = goQWBFRVDoekxdiUEUPJfBB85743123;     goQWBFRVDoekxdiUEUPJfBB85743123 = goQWBFRVDoekxdiUEUPJfBB16488311;     goQWBFRVDoekxdiUEUPJfBB16488311 = goQWBFRVDoekxdiUEUPJfBB86770771;     goQWBFRVDoekxdiUEUPJfBB86770771 = goQWBFRVDoekxdiUEUPJfBB74926907;     goQWBFRVDoekxdiUEUPJfBB74926907 = goQWBFRVDoekxdiUEUPJfBB63355861;     goQWBFRVDoekxdiUEUPJfBB63355861 = goQWBFRVDoekxdiUEUPJfBB79949191;     goQWBFRVDoekxdiUEUPJfBB79949191 = goQWBFRVDoekxdiUEUPJfBB75170580;     goQWBFRVDoekxdiUEUPJfBB75170580 = goQWBFRVDoekxdiUEUPJfBB90724479;     goQWBFRVDoekxdiUEUPJfBB90724479 = goQWBFRVDoekxdiUEUPJfBB73798911;     goQWBFRVDoekxdiUEUPJfBB73798911 = goQWBFRVDoekxdiUEUPJfBB4714422;     goQWBFRVDoekxdiUEUPJfBB4714422 = goQWBFRVDoekxdiUEUPJfBB7261296;     goQWBFRVDoekxdiUEUPJfBB7261296 = goQWBFRVDoekxdiUEUPJfBB23243199;     goQWBFRVDoekxdiUEUPJfBB23243199 = goQWBFRVDoekxdiUEUPJfBB72316780;     goQWBFRVDoekxdiUEUPJfBB72316780 = goQWBFRVDoekxdiUEUPJfBB70505084;     goQWBFRVDoekxdiUEUPJfBB70505084 = goQWBFRVDoekxdiUEUPJfBB69368789;     goQWBFRVDoekxdiUEUPJfBB69368789 = goQWBFRVDoekxdiUEUPJfBB59664713;     goQWBFRVDoekxdiUEUPJfBB59664713 = goQWBFRVDoekxdiUEUPJfBB51992219;     goQWBFRVDoekxdiUEUPJfBB51992219 = goQWBFRVDoekxdiUEUPJfBB95329599;     goQWBFRVDoekxdiUEUPJfBB95329599 = goQWBFRVDoekxdiUEUPJfBB74188497;     goQWBFRVDoekxdiUEUPJfBB74188497 = goQWBFRVDoekxdiUEUPJfBB4124297;     goQWBFRVDoekxdiUEUPJfBB4124297 = goQWBFRVDoekxdiUEUPJfBB10496330;     goQWBFRVDoekxdiUEUPJfBB10496330 = goQWBFRVDoekxdiUEUPJfBB29034761;     goQWBFRVDoekxdiUEUPJfBB29034761 = goQWBFRVDoekxdiUEUPJfBB8707481;     goQWBFRVDoekxdiUEUPJfBB8707481 = goQWBFRVDoekxdiUEUPJfBB27739897;     goQWBFRVDoekxdiUEUPJfBB27739897 = goQWBFRVDoekxdiUEUPJfBB71163902;     goQWBFRVDoekxdiUEUPJfBB71163902 = goQWBFRVDoekxdiUEUPJfBB4042135;     goQWBFRVDoekxdiUEUPJfBB4042135 = goQWBFRVDoekxdiUEUPJfBB63366956;     goQWBFRVDoekxdiUEUPJfBB63366956 = goQWBFRVDoekxdiUEUPJfBB25552085;     goQWBFRVDoekxdiUEUPJfBB25552085 = goQWBFRVDoekxdiUEUPJfBB27025274;     goQWBFRVDoekxdiUEUPJfBB27025274 = goQWBFRVDoekxdiUEUPJfBB88237906;     goQWBFRVDoekxdiUEUPJfBB88237906 = goQWBFRVDoekxdiUEUPJfBB41187406;     goQWBFRVDoekxdiUEUPJfBB41187406 = goQWBFRVDoekxdiUEUPJfBB79331374;     goQWBFRVDoekxdiUEUPJfBB79331374 = goQWBFRVDoekxdiUEUPJfBB75397208;     goQWBFRVDoekxdiUEUPJfBB75397208 = goQWBFRVDoekxdiUEUPJfBB63658615;     goQWBFRVDoekxdiUEUPJfBB63658615 = goQWBFRVDoekxdiUEUPJfBB40003906;     goQWBFRVDoekxdiUEUPJfBB40003906 = goQWBFRVDoekxdiUEUPJfBB56594284;     goQWBFRVDoekxdiUEUPJfBB56594284 = goQWBFRVDoekxdiUEUPJfBB82765956;     goQWBFRVDoekxdiUEUPJfBB82765956 = goQWBFRVDoekxdiUEUPJfBB38822505;     goQWBFRVDoekxdiUEUPJfBB38822505 = goQWBFRVDoekxdiUEUPJfBB89479465;     goQWBFRVDoekxdiUEUPJfBB89479465 = goQWBFRVDoekxdiUEUPJfBB86107059;     goQWBFRVDoekxdiUEUPJfBB86107059 = goQWBFRVDoekxdiUEUPJfBB33900645;     goQWBFRVDoekxdiUEUPJfBB33900645 = goQWBFRVDoekxdiUEUPJfBB25994615;     goQWBFRVDoekxdiUEUPJfBB25994615 = goQWBFRVDoekxdiUEUPJfBB73450695;     goQWBFRVDoekxdiUEUPJfBB73450695 = goQWBFRVDoekxdiUEUPJfBB61492637;     goQWBFRVDoekxdiUEUPJfBB61492637 = goQWBFRVDoekxdiUEUPJfBB34652536;     goQWBFRVDoekxdiUEUPJfBB34652536 = goQWBFRVDoekxdiUEUPJfBB40098696;     goQWBFRVDoekxdiUEUPJfBB40098696 = goQWBFRVDoekxdiUEUPJfBB21716587;     goQWBFRVDoekxdiUEUPJfBB21716587 = goQWBFRVDoekxdiUEUPJfBB43841082;     goQWBFRVDoekxdiUEUPJfBB43841082 = goQWBFRVDoekxdiUEUPJfBB63030612;     goQWBFRVDoekxdiUEUPJfBB63030612 = goQWBFRVDoekxdiUEUPJfBB3545738;     goQWBFRVDoekxdiUEUPJfBB3545738 = goQWBFRVDoekxdiUEUPJfBB32358686;     goQWBFRVDoekxdiUEUPJfBB32358686 = goQWBFRVDoekxdiUEUPJfBB58263515;     goQWBFRVDoekxdiUEUPJfBB58263515 = goQWBFRVDoekxdiUEUPJfBB60699122;     goQWBFRVDoekxdiUEUPJfBB60699122 = goQWBFRVDoekxdiUEUPJfBB63263006;     goQWBFRVDoekxdiUEUPJfBB63263006 = goQWBFRVDoekxdiUEUPJfBB27528484;     goQWBFRVDoekxdiUEUPJfBB27528484 = goQWBFRVDoekxdiUEUPJfBB56708363;     goQWBFRVDoekxdiUEUPJfBB56708363 = goQWBFRVDoekxdiUEUPJfBB7780831;     goQWBFRVDoekxdiUEUPJfBB7780831 = goQWBFRVDoekxdiUEUPJfBB59030875;     goQWBFRVDoekxdiUEUPJfBB59030875 = goQWBFRVDoekxdiUEUPJfBB3763005;     goQWBFRVDoekxdiUEUPJfBB3763005 = goQWBFRVDoekxdiUEUPJfBB59313726;     goQWBFRVDoekxdiUEUPJfBB59313726 = goQWBFRVDoekxdiUEUPJfBB16582236;     goQWBFRVDoekxdiUEUPJfBB16582236 = goQWBFRVDoekxdiUEUPJfBB49618496;     goQWBFRVDoekxdiUEUPJfBB49618496 = goQWBFRVDoekxdiUEUPJfBB63699205;     goQWBFRVDoekxdiUEUPJfBB63699205 = goQWBFRVDoekxdiUEUPJfBB85561004;     goQWBFRVDoekxdiUEUPJfBB85561004 = goQWBFRVDoekxdiUEUPJfBB63527015;     goQWBFRVDoekxdiUEUPJfBB63527015 = goQWBFRVDoekxdiUEUPJfBB27929922;     goQWBFRVDoekxdiUEUPJfBB27929922 = goQWBFRVDoekxdiUEUPJfBB47845991;     goQWBFRVDoekxdiUEUPJfBB47845991 = goQWBFRVDoekxdiUEUPJfBB8658165;     goQWBFRVDoekxdiUEUPJfBB8658165 = goQWBFRVDoekxdiUEUPJfBB30501179;     goQWBFRVDoekxdiUEUPJfBB30501179 = goQWBFRVDoekxdiUEUPJfBB12774505;     goQWBFRVDoekxdiUEUPJfBB12774505 = goQWBFRVDoekxdiUEUPJfBB76898756;     goQWBFRVDoekxdiUEUPJfBB76898756 = goQWBFRVDoekxdiUEUPJfBB13169714;     goQWBFRVDoekxdiUEUPJfBB13169714 = goQWBFRVDoekxdiUEUPJfBB5850134;     goQWBFRVDoekxdiUEUPJfBB5850134 = goQWBFRVDoekxdiUEUPJfBB88081438;     goQWBFRVDoekxdiUEUPJfBB88081438 = goQWBFRVDoekxdiUEUPJfBB70223651;     goQWBFRVDoekxdiUEUPJfBB70223651 = goQWBFRVDoekxdiUEUPJfBB84501715;     goQWBFRVDoekxdiUEUPJfBB84501715 = goQWBFRVDoekxdiUEUPJfBB55584065;     goQWBFRVDoekxdiUEUPJfBB55584065 = goQWBFRVDoekxdiUEUPJfBB47214843;     goQWBFRVDoekxdiUEUPJfBB47214843 = goQWBFRVDoekxdiUEUPJfBB93087360;     goQWBFRVDoekxdiUEUPJfBB93087360 = goQWBFRVDoekxdiUEUPJfBB31065207;     goQWBFRVDoekxdiUEUPJfBB31065207 = goQWBFRVDoekxdiUEUPJfBB82325548;     goQWBFRVDoekxdiUEUPJfBB82325548 = goQWBFRVDoekxdiUEUPJfBB19525874;     goQWBFRVDoekxdiUEUPJfBB19525874 = goQWBFRVDoekxdiUEUPJfBB62521472;     goQWBFRVDoekxdiUEUPJfBB62521472 = goQWBFRVDoekxdiUEUPJfBB23479537;     goQWBFRVDoekxdiUEUPJfBB23479537 = goQWBFRVDoekxdiUEUPJfBB55879220;     goQWBFRVDoekxdiUEUPJfBB55879220 = goQWBFRVDoekxdiUEUPJfBB82923891;     goQWBFRVDoekxdiUEUPJfBB82923891 = goQWBFRVDoekxdiUEUPJfBB18632252;     goQWBFRVDoekxdiUEUPJfBB18632252 = goQWBFRVDoekxdiUEUPJfBB12134202;     goQWBFRVDoekxdiUEUPJfBB12134202 = goQWBFRVDoekxdiUEUPJfBB36130132;     goQWBFRVDoekxdiUEUPJfBB36130132 = goQWBFRVDoekxdiUEUPJfBB83295542;     goQWBFRVDoekxdiUEUPJfBB83295542 = goQWBFRVDoekxdiUEUPJfBB48813453;     goQWBFRVDoekxdiUEUPJfBB48813453 = goQWBFRVDoekxdiUEUPJfBB23735082;     goQWBFRVDoekxdiUEUPJfBB23735082 = goQWBFRVDoekxdiUEUPJfBB35059501;     goQWBFRVDoekxdiUEUPJfBB35059501 = goQWBFRVDoekxdiUEUPJfBB32399401;}



void cpwIPCGdnZhlHkcqTmFVVbudSc99078807() {     float TPBhEAxwuUFiBIDGrytLhCY70783788 = -483202379;    float TPBhEAxwuUFiBIDGrytLhCY69233848 = -575848479;    float TPBhEAxwuUFiBIDGrytLhCY32356457 = -369460051;    float TPBhEAxwuUFiBIDGrytLhCY37096148 = -505194002;    float TPBhEAxwuUFiBIDGrytLhCY80060524 = -735241513;    float TPBhEAxwuUFiBIDGrytLhCY58095785 = -455416852;    float TPBhEAxwuUFiBIDGrytLhCY27458744 = -444921582;    float TPBhEAxwuUFiBIDGrytLhCY89044259 = -302065745;    float TPBhEAxwuUFiBIDGrytLhCY71742085 = -787047355;    float TPBhEAxwuUFiBIDGrytLhCY18868832 = -618296475;    float TPBhEAxwuUFiBIDGrytLhCY76106632 = -953352578;    float TPBhEAxwuUFiBIDGrytLhCY2040668 = 13031792;    float TPBhEAxwuUFiBIDGrytLhCY62967444 = -413635378;    float TPBhEAxwuUFiBIDGrytLhCY8665935 = -40086194;    float TPBhEAxwuUFiBIDGrytLhCY79955702 = -71311569;    float TPBhEAxwuUFiBIDGrytLhCY60815928 = -298058660;    float TPBhEAxwuUFiBIDGrytLhCY48558836 = -239570666;    float TPBhEAxwuUFiBIDGrytLhCY65934619 = -264977140;    float TPBhEAxwuUFiBIDGrytLhCY45012011 = -890252653;    float TPBhEAxwuUFiBIDGrytLhCY91985216 = -724255955;    float TPBhEAxwuUFiBIDGrytLhCY92349558 = -632407522;    float TPBhEAxwuUFiBIDGrytLhCY43327183 = -822737547;    float TPBhEAxwuUFiBIDGrytLhCY28432013 = -89560344;    float TPBhEAxwuUFiBIDGrytLhCY10913175 = -499285690;    float TPBhEAxwuUFiBIDGrytLhCY99241618 = -582134016;    float TPBhEAxwuUFiBIDGrytLhCY51848125 = -111970251;    float TPBhEAxwuUFiBIDGrytLhCY57484578 = -811285625;    float TPBhEAxwuUFiBIDGrytLhCY70941005 = -83251999;    float TPBhEAxwuUFiBIDGrytLhCY28172132 = -865540780;    float TPBhEAxwuUFiBIDGrytLhCY62601346 = -390124605;    float TPBhEAxwuUFiBIDGrytLhCY27397170 = -861615313;    float TPBhEAxwuUFiBIDGrytLhCY42224944 = -865300426;    float TPBhEAxwuUFiBIDGrytLhCY370290 = -334987072;    float TPBhEAxwuUFiBIDGrytLhCY93464726 = -348323381;    float TPBhEAxwuUFiBIDGrytLhCY45301529 = -960311608;    float TPBhEAxwuUFiBIDGrytLhCY41384860 = -942503566;    float TPBhEAxwuUFiBIDGrytLhCY50764617 = -125758013;    float TPBhEAxwuUFiBIDGrytLhCY34628757 = -533540202;    float TPBhEAxwuUFiBIDGrytLhCY55403145 = -230187949;    float TPBhEAxwuUFiBIDGrytLhCY66699826 = -453782365;    float TPBhEAxwuUFiBIDGrytLhCY96529869 = -824634181;    float TPBhEAxwuUFiBIDGrytLhCY20320630 = -592828159;    float TPBhEAxwuUFiBIDGrytLhCY20111254 = -271840934;    float TPBhEAxwuUFiBIDGrytLhCY79723977 = -427613280;    float TPBhEAxwuUFiBIDGrytLhCY42775767 = -850353262;    float TPBhEAxwuUFiBIDGrytLhCY48651981 = -906618519;    float TPBhEAxwuUFiBIDGrytLhCY8061948 = -163829945;    float TPBhEAxwuUFiBIDGrytLhCY20686220 = -895397304;    float TPBhEAxwuUFiBIDGrytLhCY83553205 = -529590222;    float TPBhEAxwuUFiBIDGrytLhCY34120265 = -974966530;    float TPBhEAxwuUFiBIDGrytLhCY63437639 = -220295906;    float TPBhEAxwuUFiBIDGrytLhCY60993148 = 18910665;    float TPBhEAxwuUFiBIDGrytLhCY41760753 = -989071694;    float TPBhEAxwuUFiBIDGrytLhCY14814679 = -304968580;    float TPBhEAxwuUFiBIDGrytLhCY6729249 = -169757616;    float TPBhEAxwuUFiBIDGrytLhCY27456605 = -660464833;    float TPBhEAxwuUFiBIDGrytLhCY40801835 = -386288136;    float TPBhEAxwuUFiBIDGrytLhCY21443283 = -870174361;    float TPBhEAxwuUFiBIDGrytLhCY37854529 = -923059987;    float TPBhEAxwuUFiBIDGrytLhCY28212399 = -523271263;    float TPBhEAxwuUFiBIDGrytLhCY611207 = -644131227;    float TPBhEAxwuUFiBIDGrytLhCY56517739 = -261669584;    float TPBhEAxwuUFiBIDGrytLhCY60872127 = -436524966;    float TPBhEAxwuUFiBIDGrytLhCY9140740 = -296922751;    float TPBhEAxwuUFiBIDGrytLhCY91471662 = -756681162;    float TPBhEAxwuUFiBIDGrytLhCY33881689 = 11947847;    float TPBhEAxwuUFiBIDGrytLhCY1670379 = -651981136;    float TPBhEAxwuUFiBIDGrytLhCY69502718 = 34688002;    float TPBhEAxwuUFiBIDGrytLhCY63364406 = -79774587;    float TPBhEAxwuUFiBIDGrytLhCY38570842 = -128808004;    float TPBhEAxwuUFiBIDGrytLhCY10051312 = -72300648;    float TPBhEAxwuUFiBIDGrytLhCY13930080 = -706030465;    float TPBhEAxwuUFiBIDGrytLhCY10531474 = 65210809;    float TPBhEAxwuUFiBIDGrytLhCY78312184 = -336470289;    float TPBhEAxwuUFiBIDGrytLhCY95455347 = -899621774;    float TPBhEAxwuUFiBIDGrytLhCY72028928 = 60420636;    float TPBhEAxwuUFiBIDGrytLhCY23215930 = -450896613;    float TPBhEAxwuUFiBIDGrytLhCY48708036 = -661947064;    float TPBhEAxwuUFiBIDGrytLhCY68137407 = -648932429;    float TPBhEAxwuUFiBIDGrytLhCY50589638 = -675515497;    float TPBhEAxwuUFiBIDGrytLhCY43786178 = -948140306;    float TPBhEAxwuUFiBIDGrytLhCY36798358 = -915888321;    float TPBhEAxwuUFiBIDGrytLhCY87387799 = -553661778;    float TPBhEAxwuUFiBIDGrytLhCY94051867 = -890574251;    float TPBhEAxwuUFiBIDGrytLhCY99163707 = -69828700;    float TPBhEAxwuUFiBIDGrytLhCY66404021 = -780525978;    float TPBhEAxwuUFiBIDGrytLhCY464191 = -876228732;    float TPBhEAxwuUFiBIDGrytLhCY85555610 = 69981508;    float TPBhEAxwuUFiBIDGrytLhCY86735477 = -78565766;    float TPBhEAxwuUFiBIDGrytLhCY17844924 = -199846775;    float TPBhEAxwuUFiBIDGrytLhCY583025 = -456215431;    float TPBhEAxwuUFiBIDGrytLhCY29321334 = -255583653;    float TPBhEAxwuUFiBIDGrytLhCY96774227 = -610480216;    float TPBhEAxwuUFiBIDGrytLhCY27190747 = -706916687;    float TPBhEAxwuUFiBIDGrytLhCY66088619 = -809651138;    float TPBhEAxwuUFiBIDGrytLhCY40012130 = -462964598;    float TPBhEAxwuUFiBIDGrytLhCY59448502 = -56303194;    float TPBhEAxwuUFiBIDGrytLhCY10970514 = -974918184;    float TPBhEAxwuUFiBIDGrytLhCY88252315 = -670932119;    float TPBhEAxwuUFiBIDGrytLhCY8894079 = -483202379;     TPBhEAxwuUFiBIDGrytLhCY70783788 = TPBhEAxwuUFiBIDGrytLhCY69233848;     TPBhEAxwuUFiBIDGrytLhCY69233848 = TPBhEAxwuUFiBIDGrytLhCY32356457;     TPBhEAxwuUFiBIDGrytLhCY32356457 = TPBhEAxwuUFiBIDGrytLhCY37096148;     TPBhEAxwuUFiBIDGrytLhCY37096148 = TPBhEAxwuUFiBIDGrytLhCY80060524;     TPBhEAxwuUFiBIDGrytLhCY80060524 = TPBhEAxwuUFiBIDGrytLhCY58095785;     TPBhEAxwuUFiBIDGrytLhCY58095785 = TPBhEAxwuUFiBIDGrytLhCY27458744;     TPBhEAxwuUFiBIDGrytLhCY27458744 = TPBhEAxwuUFiBIDGrytLhCY89044259;     TPBhEAxwuUFiBIDGrytLhCY89044259 = TPBhEAxwuUFiBIDGrytLhCY71742085;     TPBhEAxwuUFiBIDGrytLhCY71742085 = TPBhEAxwuUFiBIDGrytLhCY18868832;     TPBhEAxwuUFiBIDGrytLhCY18868832 = TPBhEAxwuUFiBIDGrytLhCY76106632;     TPBhEAxwuUFiBIDGrytLhCY76106632 = TPBhEAxwuUFiBIDGrytLhCY2040668;     TPBhEAxwuUFiBIDGrytLhCY2040668 = TPBhEAxwuUFiBIDGrytLhCY62967444;     TPBhEAxwuUFiBIDGrytLhCY62967444 = TPBhEAxwuUFiBIDGrytLhCY8665935;     TPBhEAxwuUFiBIDGrytLhCY8665935 = TPBhEAxwuUFiBIDGrytLhCY79955702;     TPBhEAxwuUFiBIDGrytLhCY79955702 = TPBhEAxwuUFiBIDGrytLhCY60815928;     TPBhEAxwuUFiBIDGrytLhCY60815928 = TPBhEAxwuUFiBIDGrytLhCY48558836;     TPBhEAxwuUFiBIDGrytLhCY48558836 = TPBhEAxwuUFiBIDGrytLhCY65934619;     TPBhEAxwuUFiBIDGrytLhCY65934619 = TPBhEAxwuUFiBIDGrytLhCY45012011;     TPBhEAxwuUFiBIDGrytLhCY45012011 = TPBhEAxwuUFiBIDGrytLhCY91985216;     TPBhEAxwuUFiBIDGrytLhCY91985216 = TPBhEAxwuUFiBIDGrytLhCY92349558;     TPBhEAxwuUFiBIDGrytLhCY92349558 = TPBhEAxwuUFiBIDGrytLhCY43327183;     TPBhEAxwuUFiBIDGrytLhCY43327183 = TPBhEAxwuUFiBIDGrytLhCY28432013;     TPBhEAxwuUFiBIDGrytLhCY28432013 = TPBhEAxwuUFiBIDGrytLhCY10913175;     TPBhEAxwuUFiBIDGrytLhCY10913175 = TPBhEAxwuUFiBIDGrytLhCY99241618;     TPBhEAxwuUFiBIDGrytLhCY99241618 = TPBhEAxwuUFiBIDGrytLhCY51848125;     TPBhEAxwuUFiBIDGrytLhCY51848125 = TPBhEAxwuUFiBIDGrytLhCY57484578;     TPBhEAxwuUFiBIDGrytLhCY57484578 = TPBhEAxwuUFiBIDGrytLhCY70941005;     TPBhEAxwuUFiBIDGrytLhCY70941005 = TPBhEAxwuUFiBIDGrytLhCY28172132;     TPBhEAxwuUFiBIDGrytLhCY28172132 = TPBhEAxwuUFiBIDGrytLhCY62601346;     TPBhEAxwuUFiBIDGrytLhCY62601346 = TPBhEAxwuUFiBIDGrytLhCY27397170;     TPBhEAxwuUFiBIDGrytLhCY27397170 = TPBhEAxwuUFiBIDGrytLhCY42224944;     TPBhEAxwuUFiBIDGrytLhCY42224944 = TPBhEAxwuUFiBIDGrytLhCY370290;     TPBhEAxwuUFiBIDGrytLhCY370290 = TPBhEAxwuUFiBIDGrytLhCY93464726;     TPBhEAxwuUFiBIDGrytLhCY93464726 = TPBhEAxwuUFiBIDGrytLhCY45301529;     TPBhEAxwuUFiBIDGrytLhCY45301529 = TPBhEAxwuUFiBIDGrytLhCY41384860;     TPBhEAxwuUFiBIDGrytLhCY41384860 = TPBhEAxwuUFiBIDGrytLhCY50764617;     TPBhEAxwuUFiBIDGrytLhCY50764617 = TPBhEAxwuUFiBIDGrytLhCY34628757;     TPBhEAxwuUFiBIDGrytLhCY34628757 = TPBhEAxwuUFiBIDGrytLhCY55403145;     TPBhEAxwuUFiBIDGrytLhCY55403145 = TPBhEAxwuUFiBIDGrytLhCY66699826;     TPBhEAxwuUFiBIDGrytLhCY66699826 = TPBhEAxwuUFiBIDGrytLhCY96529869;     TPBhEAxwuUFiBIDGrytLhCY96529869 = TPBhEAxwuUFiBIDGrytLhCY20320630;     TPBhEAxwuUFiBIDGrytLhCY20320630 = TPBhEAxwuUFiBIDGrytLhCY20111254;     TPBhEAxwuUFiBIDGrytLhCY20111254 = TPBhEAxwuUFiBIDGrytLhCY79723977;     TPBhEAxwuUFiBIDGrytLhCY79723977 = TPBhEAxwuUFiBIDGrytLhCY42775767;     TPBhEAxwuUFiBIDGrytLhCY42775767 = TPBhEAxwuUFiBIDGrytLhCY48651981;     TPBhEAxwuUFiBIDGrytLhCY48651981 = TPBhEAxwuUFiBIDGrytLhCY8061948;     TPBhEAxwuUFiBIDGrytLhCY8061948 = TPBhEAxwuUFiBIDGrytLhCY20686220;     TPBhEAxwuUFiBIDGrytLhCY20686220 = TPBhEAxwuUFiBIDGrytLhCY83553205;     TPBhEAxwuUFiBIDGrytLhCY83553205 = TPBhEAxwuUFiBIDGrytLhCY34120265;     TPBhEAxwuUFiBIDGrytLhCY34120265 = TPBhEAxwuUFiBIDGrytLhCY63437639;     TPBhEAxwuUFiBIDGrytLhCY63437639 = TPBhEAxwuUFiBIDGrytLhCY60993148;     TPBhEAxwuUFiBIDGrytLhCY60993148 = TPBhEAxwuUFiBIDGrytLhCY41760753;     TPBhEAxwuUFiBIDGrytLhCY41760753 = TPBhEAxwuUFiBIDGrytLhCY14814679;     TPBhEAxwuUFiBIDGrytLhCY14814679 = TPBhEAxwuUFiBIDGrytLhCY6729249;     TPBhEAxwuUFiBIDGrytLhCY6729249 = TPBhEAxwuUFiBIDGrytLhCY27456605;     TPBhEAxwuUFiBIDGrytLhCY27456605 = TPBhEAxwuUFiBIDGrytLhCY40801835;     TPBhEAxwuUFiBIDGrytLhCY40801835 = TPBhEAxwuUFiBIDGrytLhCY21443283;     TPBhEAxwuUFiBIDGrytLhCY21443283 = TPBhEAxwuUFiBIDGrytLhCY37854529;     TPBhEAxwuUFiBIDGrytLhCY37854529 = TPBhEAxwuUFiBIDGrytLhCY28212399;     TPBhEAxwuUFiBIDGrytLhCY28212399 = TPBhEAxwuUFiBIDGrytLhCY611207;     TPBhEAxwuUFiBIDGrytLhCY611207 = TPBhEAxwuUFiBIDGrytLhCY56517739;     TPBhEAxwuUFiBIDGrytLhCY56517739 = TPBhEAxwuUFiBIDGrytLhCY60872127;     TPBhEAxwuUFiBIDGrytLhCY60872127 = TPBhEAxwuUFiBIDGrytLhCY9140740;     TPBhEAxwuUFiBIDGrytLhCY9140740 = TPBhEAxwuUFiBIDGrytLhCY91471662;     TPBhEAxwuUFiBIDGrytLhCY91471662 = TPBhEAxwuUFiBIDGrytLhCY33881689;     TPBhEAxwuUFiBIDGrytLhCY33881689 = TPBhEAxwuUFiBIDGrytLhCY1670379;     TPBhEAxwuUFiBIDGrytLhCY1670379 = TPBhEAxwuUFiBIDGrytLhCY69502718;     TPBhEAxwuUFiBIDGrytLhCY69502718 = TPBhEAxwuUFiBIDGrytLhCY63364406;     TPBhEAxwuUFiBIDGrytLhCY63364406 = TPBhEAxwuUFiBIDGrytLhCY38570842;     TPBhEAxwuUFiBIDGrytLhCY38570842 = TPBhEAxwuUFiBIDGrytLhCY10051312;     TPBhEAxwuUFiBIDGrytLhCY10051312 = TPBhEAxwuUFiBIDGrytLhCY13930080;     TPBhEAxwuUFiBIDGrytLhCY13930080 = TPBhEAxwuUFiBIDGrytLhCY10531474;     TPBhEAxwuUFiBIDGrytLhCY10531474 = TPBhEAxwuUFiBIDGrytLhCY78312184;     TPBhEAxwuUFiBIDGrytLhCY78312184 = TPBhEAxwuUFiBIDGrytLhCY95455347;     TPBhEAxwuUFiBIDGrytLhCY95455347 = TPBhEAxwuUFiBIDGrytLhCY72028928;     TPBhEAxwuUFiBIDGrytLhCY72028928 = TPBhEAxwuUFiBIDGrytLhCY23215930;     TPBhEAxwuUFiBIDGrytLhCY23215930 = TPBhEAxwuUFiBIDGrytLhCY48708036;     TPBhEAxwuUFiBIDGrytLhCY48708036 = TPBhEAxwuUFiBIDGrytLhCY68137407;     TPBhEAxwuUFiBIDGrytLhCY68137407 = TPBhEAxwuUFiBIDGrytLhCY50589638;     TPBhEAxwuUFiBIDGrytLhCY50589638 = TPBhEAxwuUFiBIDGrytLhCY43786178;     TPBhEAxwuUFiBIDGrytLhCY43786178 = TPBhEAxwuUFiBIDGrytLhCY36798358;     TPBhEAxwuUFiBIDGrytLhCY36798358 = TPBhEAxwuUFiBIDGrytLhCY87387799;     TPBhEAxwuUFiBIDGrytLhCY87387799 = TPBhEAxwuUFiBIDGrytLhCY94051867;     TPBhEAxwuUFiBIDGrytLhCY94051867 = TPBhEAxwuUFiBIDGrytLhCY99163707;     TPBhEAxwuUFiBIDGrytLhCY99163707 = TPBhEAxwuUFiBIDGrytLhCY66404021;     TPBhEAxwuUFiBIDGrytLhCY66404021 = TPBhEAxwuUFiBIDGrytLhCY464191;     TPBhEAxwuUFiBIDGrytLhCY464191 = TPBhEAxwuUFiBIDGrytLhCY85555610;     TPBhEAxwuUFiBIDGrytLhCY85555610 = TPBhEAxwuUFiBIDGrytLhCY86735477;     TPBhEAxwuUFiBIDGrytLhCY86735477 = TPBhEAxwuUFiBIDGrytLhCY17844924;     TPBhEAxwuUFiBIDGrytLhCY17844924 = TPBhEAxwuUFiBIDGrytLhCY583025;     TPBhEAxwuUFiBIDGrytLhCY583025 = TPBhEAxwuUFiBIDGrytLhCY29321334;     TPBhEAxwuUFiBIDGrytLhCY29321334 = TPBhEAxwuUFiBIDGrytLhCY96774227;     TPBhEAxwuUFiBIDGrytLhCY96774227 = TPBhEAxwuUFiBIDGrytLhCY27190747;     TPBhEAxwuUFiBIDGrytLhCY27190747 = TPBhEAxwuUFiBIDGrytLhCY66088619;     TPBhEAxwuUFiBIDGrytLhCY66088619 = TPBhEAxwuUFiBIDGrytLhCY40012130;     TPBhEAxwuUFiBIDGrytLhCY40012130 = TPBhEAxwuUFiBIDGrytLhCY59448502;     TPBhEAxwuUFiBIDGrytLhCY59448502 = TPBhEAxwuUFiBIDGrytLhCY10970514;     TPBhEAxwuUFiBIDGrytLhCY10970514 = TPBhEAxwuUFiBIDGrytLhCY88252315;     TPBhEAxwuUFiBIDGrytLhCY88252315 = TPBhEAxwuUFiBIDGrytLhCY8894079;     TPBhEAxwuUFiBIDGrytLhCY8894079 = TPBhEAxwuUFiBIDGrytLhCY70783788;}



void QdbwmKjstiavEWIzdqIhErYNvb14458073() {     float YWgXFeljzsRfnHRcveqRjjh22240026 = -30100825;    float YWgXFeljzsRfnHRcveqRjjh4181825 = -848478571;    float YWgXFeljzsRfnHRcveqRjjh88979206 = -92036972;    float YWgXFeljzsRfnHRcveqRjjh77815847 = -510381052;    float YWgXFeljzsRfnHRcveqRjjh83344444 = -951811467;    float YWgXFeljzsRfnHRcveqRjjh70144022 = -290195062;    float YWgXFeljzsRfnHRcveqRjjh35394342 = 13469646;    float YWgXFeljzsRfnHRcveqRjjh78843097 = -892548743;    float YWgXFeljzsRfnHRcveqRjjh31350335 = -221872720;    float YWgXFeljzsRfnHRcveqRjjh74106019 = -305277554;    float YWgXFeljzsRfnHRcveqRjjh77926233 = -164905664;    float YWgXFeljzsRfnHRcveqRjjh85390674 = 19571317;    float YWgXFeljzsRfnHRcveqRjjh17212162 = 28355363;    float YWgXFeljzsRfnHRcveqRjjh52687401 = -205374236;    float YWgXFeljzsRfnHRcveqRjjh90646164 = -817364956;    float YWgXFeljzsRfnHRcveqRjjh87118333 = -973694290;    float YWgXFeljzsRfnHRcveqRjjh98415835 = -202130521;    float YWgXFeljzsRfnHRcveqRjjh18391718 = -264006937;    float YWgXFeljzsRfnHRcveqRjjh26049522 = -528192469;    float YWgXFeljzsRfnHRcveqRjjh26974589 = -247817179;    float YWgXFeljzsRfnHRcveqRjjh26397788 = -730278201;    float YWgXFeljzsRfnHRcveqRjjh56891884 = 25784962;    float YWgXFeljzsRfnHRcveqRjjh64431370 = -808292251;    float YWgXFeljzsRfnHRcveqRjjh37127288 = -903470384;    float YWgXFeljzsRfnHRcveqRjjh75631342 = 90889550;    float YWgXFeljzsRfnHRcveqRjjh51902931 = -995122718;    float YWgXFeljzsRfnHRcveqRjjh24176014 = -457903262;    float YWgXFeljzsRfnHRcveqRjjh66202655 = -750096081;    float YWgXFeljzsRfnHRcveqRjjh67085157 = -253748924;    float YWgXFeljzsRfnHRcveqRjjh12871445 = -692268878;    float YWgXFeljzsRfnHRcveqRjjh1346870 = -613156960;    float YWgXFeljzsRfnHRcveqRjjh93567812 = -555072475;    float YWgXFeljzsRfnHRcveqRjjh27877785 = -973027044;    float YWgXFeljzsRfnHRcveqRjjh93922234 = -296426069;    float YWgXFeljzsRfnHRcveqRjjh54630062 = -540495378;    float YWgXFeljzsRfnHRcveqRjjh65612421 = -988299823;    float YWgXFeljzsRfnHRcveqRjjh69263431 = -328620641;    float YWgXFeljzsRfnHRcveqRjjh25659410 = -426379870;    float YWgXFeljzsRfnHRcveqRjjh13557347 = -554844260;    float YWgXFeljzsRfnHRcveqRjjh2088262 = -878344674;    float YWgXFeljzsRfnHRcveqRjjh34095360 = -865301815;    float YWgXFeljzsRfnHRcveqRjjh1494090 = -449230433;    float YWgXFeljzsRfnHRcveqRjjh67461408 = 28729546;    float YWgXFeljzsRfnHRcveqRjjh17369217 = -530240996;    float YWgXFeljzsRfnHRcveqRjjh78762221 = -852993220;    float YWgXFeljzsRfnHRcveqRjjh45685542 = -635089691;    float YWgXFeljzsRfnHRcveqRjjh37064603 = -736828722;    float YWgXFeljzsRfnHRcveqRjjh80599433 = -436401821;    float YWgXFeljzsRfnHRcveqRjjh69544557 = -279360882;    float YWgXFeljzsRfnHRcveqRjjh22235678 = -408982930;    float YWgXFeljzsRfnHRcveqRjjh37284148 = 16070918;    float YWgXFeljzsRfnHRcveqRjjh96967975 = -748895421;    float YWgXFeljzsRfnHRcveqRjjh33729556 = -932999832;    float YWgXFeljzsRfnHRcveqRjjh63618073 = -161472092;    float YWgXFeljzsRfnHRcveqRjjh88060284 = -231279995;    float YWgXFeljzsRfnHRcveqRjjh65348142 = 44114212;    float YWgXFeljzsRfnHRcveqRjjh39750455 = 59813680;    float YWgXFeljzsRfnHRcveqRjjh51851918 = -188566589;    float YWgXFeljzsRfnHRcveqRjjh2184506 = -501270602;    float YWgXFeljzsRfnHRcveqRjjh31441514 = -956688749;    float YWgXFeljzsRfnHRcveqRjjh45968009 = -832291801;    float YWgXFeljzsRfnHRcveqRjjh69191687 = -236434274;    float YWgXFeljzsRfnHRcveqRjjh11757940 = -538799819;    float YWgXFeljzsRfnHRcveqRjjh18478891 = -529603843;    float YWgXFeljzsRfnHRcveqRjjh72759150 = -692120594;    float YWgXFeljzsRfnHRcveqRjjh84358420 = -609833190;    float YWgXFeljzsRfnHRcveqRjjh57512890 = -7401640;    float YWgXFeljzsRfnHRcveqRjjh23289928 = -675218568;    float YWgXFeljzsRfnHRcveqRjjh98057339 = -664878858;    float YWgXFeljzsRfnHRcveqRjjh25033744 = -829065133;    float YWgXFeljzsRfnHRcveqRjjh17854903 = -545073650;    float YWgXFeljzsRfnHRcveqRjjh72756426 = -775750651;    float YWgXFeljzsRfnHRcveqRjjh4834372 = -709162677;    float YWgXFeljzsRfnHRcveqRjjh23961260 = -649847795;    float YWgXFeljzsRfnHRcveqRjjh92879229 = -382515364;    float YWgXFeljzsRfnHRcveqRjjh24903699 = -181047768;    float YWgXFeljzsRfnHRcveqRjjh89430475 = 97055415;    float YWgXFeljzsRfnHRcveqRjjh47062153 = -178051256;    float YWgXFeljzsRfnHRcveqRjjh58365067 = 49522835;    float YWgXFeljzsRfnHRcveqRjjh29945800 = -274020759;    float YWgXFeljzsRfnHRcveqRjjh14838328 = -158293997;    float YWgXFeljzsRfnHRcveqRjjh43576581 = 78498558;    float YWgXFeljzsRfnHRcveqRjjh96658098 = -370735199;    float YWgXFeljzsRfnHRcveqRjjh44849480 = -844765995;    float YWgXFeljzsRfnHRcveqRjjh75587296 = -608339796;    float YWgXFeljzsRfnHRcveqRjjh4378894 = -864261540;    float YWgXFeljzsRfnHRcveqRjjh59838257 = -622072643;    float YWgXFeljzsRfnHRcveqRjjh64259711 = -711554952;    float YWgXFeljzsRfnHRcveqRjjh5861951 = 34853925;    float YWgXFeljzsRfnHRcveqRjjh89281919 = -484609591;    float YWgXFeljzsRfnHRcveqRjjh25861966 = -948113504;    float YWgXFeljzsRfnHRcveqRjjh17411513 = -40054053;    float YWgXFeljzsRfnHRcveqRjjh23474904 = -925109268;    float YWgXFeljzsRfnHRcveqRjjh82115833 = -598155512;    float YWgXFeljzsRfnHRcveqRjjh56120253 = 53947126;    float YWgXFeljzsRfnHRcveqRjjh64903672 = -528867542;    float YWgXFeljzsRfnHRcveqRjjh89736149 = -910430614;    float YWgXFeljzsRfnHRcveqRjjh48982518 = -441666612;    float YWgXFeljzsRfnHRcveqRjjh44610066 = -838120402;    float YWgXFeljzsRfnHRcveqRjjh94403800 = -30100825;     YWgXFeljzsRfnHRcveqRjjh22240026 = YWgXFeljzsRfnHRcveqRjjh4181825;     YWgXFeljzsRfnHRcveqRjjh4181825 = YWgXFeljzsRfnHRcveqRjjh88979206;     YWgXFeljzsRfnHRcveqRjjh88979206 = YWgXFeljzsRfnHRcveqRjjh77815847;     YWgXFeljzsRfnHRcveqRjjh77815847 = YWgXFeljzsRfnHRcveqRjjh83344444;     YWgXFeljzsRfnHRcveqRjjh83344444 = YWgXFeljzsRfnHRcveqRjjh70144022;     YWgXFeljzsRfnHRcveqRjjh70144022 = YWgXFeljzsRfnHRcveqRjjh35394342;     YWgXFeljzsRfnHRcveqRjjh35394342 = YWgXFeljzsRfnHRcveqRjjh78843097;     YWgXFeljzsRfnHRcveqRjjh78843097 = YWgXFeljzsRfnHRcveqRjjh31350335;     YWgXFeljzsRfnHRcveqRjjh31350335 = YWgXFeljzsRfnHRcveqRjjh74106019;     YWgXFeljzsRfnHRcveqRjjh74106019 = YWgXFeljzsRfnHRcveqRjjh77926233;     YWgXFeljzsRfnHRcveqRjjh77926233 = YWgXFeljzsRfnHRcveqRjjh85390674;     YWgXFeljzsRfnHRcveqRjjh85390674 = YWgXFeljzsRfnHRcveqRjjh17212162;     YWgXFeljzsRfnHRcveqRjjh17212162 = YWgXFeljzsRfnHRcveqRjjh52687401;     YWgXFeljzsRfnHRcveqRjjh52687401 = YWgXFeljzsRfnHRcveqRjjh90646164;     YWgXFeljzsRfnHRcveqRjjh90646164 = YWgXFeljzsRfnHRcveqRjjh87118333;     YWgXFeljzsRfnHRcveqRjjh87118333 = YWgXFeljzsRfnHRcveqRjjh98415835;     YWgXFeljzsRfnHRcveqRjjh98415835 = YWgXFeljzsRfnHRcveqRjjh18391718;     YWgXFeljzsRfnHRcveqRjjh18391718 = YWgXFeljzsRfnHRcveqRjjh26049522;     YWgXFeljzsRfnHRcveqRjjh26049522 = YWgXFeljzsRfnHRcveqRjjh26974589;     YWgXFeljzsRfnHRcveqRjjh26974589 = YWgXFeljzsRfnHRcveqRjjh26397788;     YWgXFeljzsRfnHRcveqRjjh26397788 = YWgXFeljzsRfnHRcveqRjjh56891884;     YWgXFeljzsRfnHRcveqRjjh56891884 = YWgXFeljzsRfnHRcveqRjjh64431370;     YWgXFeljzsRfnHRcveqRjjh64431370 = YWgXFeljzsRfnHRcveqRjjh37127288;     YWgXFeljzsRfnHRcveqRjjh37127288 = YWgXFeljzsRfnHRcveqRjjh75631342;     YWgXFeljzsRfnHRcveqRjjh75631342 = YWgXFeljzsRfnHRcveqRjjh51902931;     YWgXFeljzsRfnHRcveqRjjh51902931 = YWgXFeljzsRfnHRcveqRjjh24176014;     YWgXFeljzsRfnHRcveqRjjh24176014 = YWgXFeljzsRfnHRcveqRjjh66202655;     YWgXFeljzsRfnHRcveqRjjh66202655 = YWgXFeljzsRfnHRcveqRjjh67085157;     YWgXFeljzsRfnHRcveqRjjh67085157 = YWgXFeljzsRfnHRcveqRjjh12871445;     YWgXFeljzsRfnHRcveqRjjh12871445 = YWgXFeljzsRfnHRcveqRjjh1346870;     YWgXFeljzsRfnHRcveqRjjh1346870 = YWgXFeljzsRfnHRcveqRjjh93567812;     YWgXFeljzsRfnHRcveqRjjh93567812 = YWgXFeljzsRfnHRcveqRjjh27877785;     YWgXFeljzsRfnHRcveqRjjh27877785 = YWgXFeljzsRfnHRcveqRjjh93922234;     YWgXFeljzsRfnHRcveqRjjh93922234 = YWgXFeljzsRfnHRcveqRjjh54630062;     YWgXFeljzsRfnHRcveqRjjh54630062 = YWgXFeljzsRfnHRcveqRjjh65612421;     YWgXFeljzsRfnHRcveqRjjh65612421 = YWgXFeljzsRfnHRcveqRjjh69263431;     YWgXFeljzsRfnHRcveqRjjh69263431 = YWgXFeljzsRfnHRcveqRjjh25659410;     YWgXFeljzsRfnHRcveqRjjh25659410 = YWgXFeljzsRfnHRcveqRjjh13557347;     YWgXFeljzsRfnHRcveqRjjh13557347 = YWgXFeljzsRfnHRcveqRjjh2088262;     YWgXFeljzsRfnHRcveqRjjh2088262 = YWgXFeljzsRfnHRcveqRjjh34095360;     YWgXFeljzsRfnHRcveqRjjh34095360 = YWgXFeljzsRfnHRcveqRjjh1494090;     YWgXFeljzsRfnHRcveqRjjh1494090 = YWgXFeljzsRfnHRcveqRjjh67461408;     YWgXFeljzsRfnHRcveqRjjh67461408 = YWgXFeljzsRfnHRcveqRjjh17369217;     YWgXFeljzsRfnHRcveqRjjh17369217 = YWgXFeljzsRfnHRcveqRjjh78762221;     YWgXFeljzsRfnHRcveqRjjh78762221 = YWgXFeljzsRfnHRcveqRjjh45685542;     YWgXFeljzsRfnHRcveqRjjh45685542 = YWgXFeljzsRfnHRcveqRjjh37064603;     YWgXFeljzsRfnHRcveqRjjh37064603 = YWgXFeljzsRfnHRcveqRjjh80599433;     YWgXFeljzsRfnHRcveqRjjh80599433 = YWgXFeljzsRfnHRcveqRjjh69544557;     YWgXFeljzsRfnHRcveqRjjh69544557 = YWgXFeljzsRfnHRcveqRjjh22235678;     YWgXFeljzsRfnHRcveqRjjh22235678 = YWgXFeljzsRfnHRcveqRjjh37284148;     YWgXFeljzsRfnHRcveqRjjh37284148 = YWgXFeljzsRfnHRcveqRjjh96967975;     YWgXFeljzsRfnHRcveqRjjh96967975 = YWgXFeljzsRfnHRcveqRjjh33729556;     YWgXFeljzsRfnHRcveqRjjh33729556 = YWgXFeljzsRfnHRcveqRjjh63618073;     YWgXFeljzsRfnHRcveqRjjh63618073 = YWgXFeljzsRfnHRcveqRjjh88060284;     YWgXFeljzsRfnHRcveqRjjh88060284 = YWgXFeljzsRfnHRcveqRjjh65348142;     YWgXFeljzsRfnHRcveqRjjh65348142 = YWgXFeljzsRfnHRcveqRjjh39750455;     YWgXFeljzsRfnHRcveqRjjh39750455 = YWgXFeljzsRfnHRcveqRjjh51851918;     YWgXFeljzsRfnHRcveqRjjh51851918 = YWgXFeljzsRfnHRcveqRjjh2184506;     YWgXFeljzsRfnHRcveqRjjh2184506 = YWgXFeljzsRfnHRcveqRjjh31441514;     YWgXFeljzsRfnHRcveqRjjh31441514 = YWgXFeljzsRfnHRcveqRjjh45968009;     YWgXFeljzsRfnHRcveqRjjh45968009 = YWgXFeljzsRfnHRcveqRjjh69191687;     YWgXFeljzsRfnHRcveqRjjh69191687 = YWgXFeljzsRfnHRcveqRjjh11757940;     YWgXFeljzsRfnHRcveqRjjh11757940 = YWgXFeljzsRfnHRcveqRjjh18478891;     YWgXFeljzsRfnHRcveqRjjh18478891 = YWgXFeljzsRfnHRcveqRjjh72759150;     YWgXFeljzsRfnHRcveqRjjh72759150 = YWgXFeljzsRfnHRcveqRjjh84358420;     YWgXFeljzsRfnHRcveqRjjh84358420 = YWgXFeljzsRfnHRcveqRjjh57512890;     YWgXFeljzsRfnHRcveqRjjh57512890 = YWgXFeljzsRfnHRcveqRjjh23289928;     YWgXFeljzsRfnHRcveqRjjh23289928 = YWgXFeljzsRfnHRcveqRjjh98057339;     YWgXFeljzsRfnHRcveqRjjh98057339 = YWgXFeljzsRfnHRcveqRjjh25033744;     YWgXFeljzsRfnHRcveqRjjh25033744 = YWgXFeljzsRfnHRcveqRjjh17854903;     YWgXFeljzsRfnHRcveqRjjh17854903 = YWgXFeljzsRfnHRcveqRjjh72756426;     YWgXFeljzsRfnHRcveqRjjh72756426 = YWgXFeljzsRfnHRcveqRjjh4834372;     YWgXFeljzsRfnHRcveqRjjh4834372 = YWgXFeljzsRfnHRcveqRjjh23961260;     YWgXFeljzsRfnHRcveqRjjh23961260 = YWgXFeljzsRfnHRcveqRjjh92879229;     YWgXFeljzsRfnHRcveqRjjh92879229 = YWgXFeljzsRfnHRcveqRjjh24903699;     YWgXFeljzsRfnHRcveqRjjh24903699 = YWgXFeljzsRfnHRcveqRjjh89430475;     YWgXFeljzsRfnHRcveqRjjh89430475 = YWgXFeljzsRfnHRcveqRjjh47062153;     YWgXFeljzsRfnHRcveqRjjh47062153 = YWgXFeljzsRfnHRcveqRjjh58365067;     YWgXFeljzsRfnHRcveqRjjh58365067 = YWgXFeljzsRfnHRcveqRjjh29945800;     YWgXFeljzsRfnHRcveqRjjh29945800 = YWgXFeljzsRfnHRcveqRjjh14838328;     YWgXFeljzsRfnHRcveqRjjh14838328 = YWgXFeljzsRfnHRcveqRjjh43576581;     YWgXFeljzsRfnHRcveqRjjh43576581 = YWgXFeljzsRfnHRcveqRjjh96658098;     YWgXFeljzsRfnHRcveqRjjh96658098 = YWgXFeljzsRfnHRcveqRjjh44849480;     YWgXFeljzsRfnHRcveqRjjh44849480 = YWgXFeljzsRfnHRcveqRjjh75587296;     YWgXFeljzsRfnHRcveqRjjh75587296 = YWgXFeljzsRfnHRcveqRjjh4378894;     YWgXFeljzsRfnHRcveqRjjh4378894 = YWgXFeljzsRfnHRcveqRjjh59838257;     YWgXFeljzsRfnHRcveqRjjh59838257 = YWgXFeljzsRfnHRcveqRjjh64259711;     YWgXFeljzsRfnHRcveqRjjh64259711 = YWgXFeljzsRfnHRcveqRjjh5861951;     YWgXFeljzsRfnHRcveqRjjh5861951 = YWgXFeljzsRfnHRcveqRjjh89281919;     YWgXFeljzsRfnHRcveqRjjh89281919 = YWgXFeljzsRfnHRcveqRjjh25861966;     YWgXFeljzsRfnHRcveqRjjh25861966 = YWgXFeljzsRfnHRcveqRjjh17411513;     YWgXFeljzsRfnHRcveqRjjh17411513 = YWgXFeljzsRfnHRcveqRjjh23474904;     YWgXFeljzsRfnHRcveqRjjh23474904 = YWgXFeljzsRfnHRcveqRjjh82115833;     YWgXFeljzsRfnHRcveqRjjh82115833 = YWgXFeljzsRfnHRcveqRjjh56120253;     YWgXFeljzsRfnHRcveqRjjh56120253 = YWgXFeljzsRfnHRcveqRjjh64903672;     YWgXFeljzsRfnHRcveqRjjh64903672 = YWgXFeljzsRfnHRcveqRjjh89736149;     YWgXFeljzsRfnHRcveqRjjh89736149 = YWgXFeljzsRfnHRcveqRjjh48982518;     YWgXFeljzsRfnHRcveqRjjh48982518 = YWgXFeljzsRfnHRcveqRjjh44610066;     YWgXFeljzsRfnHRcveqRjjh44610066 = YWgXFeljzsRfnHRcveqRjjh94403800;     YWgXFeljzsRfnHRcveqRjjh94403800 = YWgXFeljzsRfnHRcveqRjjh22240026;}



void pmpDPrabMtdDLbfkWErcAqcHrP29837338() {     float MfsQiNCUsgdXRtxupVHiwiV73696263 = -676999272;    float MfsQiNCUsgdXRtxupVHiwiV39129802 = -21108663;    float MfsQiNCUsgdXRtxupVHiwiV45601956 = -914613894;    float MfsQiNCUsgdXRtxupVHiwiV18535548 = -515568101;    float MfsQiNCUsgdXRtxupVHiwiV86628364 = -68381420;    float MfsQiNCUsgdXRtxupVHiwiV82192260 = -124973273;    float MfsQiNCUsgdXRtxupVHiwiV43329941 = -628139126;    float MfsQiNCUsgdXRtxupVHiwiV68641935 = -383031741;    float MfsQiNCUsgdXRtxupVHiwiV90958583 = -756698085;    float MfsQiNCUsgdXRtxupVHiwiV29343207 = 7741367;    float MfsQiNCUsgdXRtxupVHiwiV79745834 = -476458750;    float MfsQiNCUsgdXRtxupVHiwiV68740680 = 26110841;    float MfsQiNCUsgdXRtxupVHiwiV71456879 = -629653896;    float MfsQiNCUsgdXRtxupVHiwiV96708867 = -370662277;    float MfsQiNCUsgdXRtxupVHiwiV1336627 = -463418343;    float MfsQiNCUsgdXRtxupVHiwiV13420739 = -549329920;    float MfsQiNCUsgdXRtxupVHiwiV48272835 = -164690375;    float MfsQiNCUsgdXRtxupVHiwiV70848817 = -263036734;    float MfsQiNCUsgdXRtxupVHiwiV7087033 = -166132285;    float MfsQiNCUsgdXRtxupVHiwiV61963961 = -871378403;    float MfsQiNCUsgdXRtxupVHiwiV60446017 = -828148879;    float MfsQiNCUsgdXRtxupVHiwiV70456584 = -225692530;    float MfsQiNCUsgdXRtxupVHiwiV430727 = -427024159;    float MfsQiNCUsgdXRtxupVHiwiV63341401 = -207655078;    float MfsQiNCUsgdXRtxupVHiwiV52021066 = -336086884;    float MfsQiNCUsgdXRtxupVHiwiV51957736 = -778275185;    float MfsQiNCUsgdXRtxupVHiwiV90867449 = -104520899;    float MfsQiNCUsgdXRtxupVHiwiV61464305 = -316940162;    float MfsQiNCUsgdXRtxupVHiwiV5998183 = -741957068;    float MfsQiNCUsgdXRtxupVHiwiV63141542 = -994413151;    float MfsQiNCUsgdXRtxupVHiwiV75296568 = -364698607;    float MfsQiNCUsgdXRtxupVHiwiV44910682 = -244844524;    float MfsQiNCUsgdXRtxupVHiwiV55385279 = -511067016;    float MfsQiNCUsgdXRtxupVHiwiV94379743 = -244528757;    float MfsQiNCUsgdXRtxupVHiwiV63958595 = -120679149;    float MfsQiNCUsgdXRtxupVHiwiV89839981 = 65903920;    float MfsQiNCUsgdXRtxupVHiwiV87762245 = -531483268;    float MfsQiNCUsgdXRtxupVHiwiV16690063 = -319219538;    float MfsQiNCUsgdXRtxupVHiwiV71711548 = -879500572;    float MfsQiNCUsgdXRtxupVHiwiV37476698 = -202906984;    float MfsQiNCUsgdXRtxupVHiwiV71660850 = -905969450;    float MfsQiNCUsgdXRtxupVHiwiV82667548 = -305632707;    float MfsQiNCUsgdXRtxupVHiwiV14811564 = -770699974;    float MfsQiNCUsgdXRtxupVHiwiV55014456 = -632868712;    float MfsQiNCUsgdXRtxupVHiwiV14748675 = -855633178;    float MfsQiNCUsgdXRtxupVHiwiV42719104 = -363560864;    float MfsQiNCUsgdXRtxupVHiwiV66067258 = -209827498;    float MfsQiNCUsgdXRtxupVHiwiV40512646 = 22593663;    float MfsQiNCUsgdXRtxupVHiwiV55535909 = -29131543;    float MfsQiNCUsgdXRtxupVHiwiV10351092 = -942999330;    float MfsQiNCUsgdXRtxupVHiwiV11130656 = -847562259;    float MfsQiNCUsgdXRtxupVHiwiV32942803 = -416701506;    float MfsQiNCUsgdXRtxupVHiwiV25698359 = -876927971;    float MfsQiNCUsgdXRtxupVHiwiV12421467 = -17975604;    float MfsQiNCUsgdXRtxupVHiwiV69391320 = -292802374;    float MfsQiNCUsgdXRtxupVHiwiV3239680 = -351306742;    float MfsQiNCUsgdXRtxupVHiwiV38699075 = -594084504;    float MfsQiNCUsgdXRtxupVHiwiV82260554 = -606958816;    float MfsQiNCUsgdXRtxupVHiwiV66514482 = -79481218;    float MfsQiNCUsgdXRtxupVHiwiV34670629 = -290106236;    float MfsQiNCUsgdXRtxupVHiwiV91324810 = 79547625;    float MfsQiNCUsgdXRtxupVHiwiV81865635 = -211198964;    float MfsQiNCUsgdXRtxupVHiwiV62643752 = -641074673;    float MfsQiNCUsgdXRtxupVHiwiV27817042 = -762284935;    float MfsQiNCUsgdXRtxupVHiwiV54046638 = -627560026;    float MfsQiNCUsgdXRtxupVHiwiV34835152 = -131614227;    float MfsQiNCUsgdXRtxupVHiwiV13355401 = -462822144;    float MfsQiNCUsgdXRtxupVHiwiV77077136 = -285125139;    float MfsQiNCUsgdXRtxupVHiwiV32750272 = -149983129;    float MfsQiNCUsgdXRtxupVHiwiV11496645 = -429322263;    float MfsQiNCUsgdXRtxupVHiwiV25658493 = 82153348;    float MfsQiNCUsgdXRtxupVHiwiV31582772 = -845470838;    float MfsQiNCUsgdXRtxupVHiwiV99137268 = -383536163;    float MfsQiNCUsgdXRtxupVHiwiV69610335 = -963225302;    float MfsQiNCUsgdXRtxupVHiwiV90303110 = -965408954;    float MfsQiNCUsgdXRtxupVHiwiV77778468 = -422516173;    float MfsQiNCUsgdXRtxupVHiwiV55645020 = -454992556;    float MfsQiNCUsgdXRtxupVHiwiV45416271 = -794155448;    float MfsQiNCUsgdXRtxupVHiwiV48592727 = -352021901;    float MfsQiNCUsgdXRtxupVHiwiV9301962 = -972526021;    float MfsQiNCUsgdXRtxupVHiwiV85890478 = -468447688;    float MfsQiNCUsgdXRtxupVHiwiV50354804 = -27114563;    float MfsQiNCUsgdXRtxupVHiwiV5928397 = -187808620;    float MfsQiNCUsgdXRtxupVHiwiV95647091 = -798957739;    float MfsQiNCUsgdXRtxupVHiwiV52010886 = -46850892;    float MfsQiNCUsgdXRtxupVHiwiV42353766 = -947997101;    float MfsQiNCUsgdXRtxupVHiwiV19212323 = -367916553;    float MfsQiNCUsgdXRtxupVHiwiV42963813 = -393091412;    float MfsQiNCUsgdXRtxupVHiwiV24988424 = -951726384;    float MfsQiNCUsgdXRtxupVHiwiV60718916 = -769372407;    float MfsQiNCUsgdXRtxupVHiwiV51140907 = -340011577;    float MfsQiNCUsgdXRtxupVHiwiV5501692 = -924524452;    float MfsQiNCUsgdXRtxupVHiwiV50175581 = -139738321;    float MfsQiNCUsgdXRtxupVHiwiV37040920 = -489394337;    float MfsQiNCUsgdXRtxupVHiwiV46151887 = -182454610;    float MfsQiNCUsgdXRtxupVHiwiV89795215 = -594770486;    float MfsQiNCUsgdXRtxupVHiwiV20023797 = -664558034;    float MfsQiNCUsgdXRtxupVHiwiV86994522 = 91584960;    float MfsQiNCUsgdXRtxupVHiwiV967818 = 94691314;    float MfsQiNCUsgdXRtxupVHiwiV79913522 = -676999272;     MfsQiNCUsgdXRtxupVHiwiV73696263 = MfsQiNCUsgdXRtxupVHiwiV39129802;     MfsQiNCUsgdXRtxupVHiwiV39129802 = MfsQiNCUsgdXRtxupVHiwiV45601956;     MfsQiNCUsgdXRtxupVHiwiV45601956 = MfsQiNCUsgdXRtxupVHiwiV18535548;     MfsQiNCUsgdXRtxupVHiwiV18535548 = MfsQiNCUsgdXRtxupVHiwiV86628364;     MfsQiNCUsgdXRtxupVHiwiV86628364 = MfsQiNCUsgdXRtxupVHiwiV82192260;     MfsQiNCUsgdXRtxupVHiwiV82192260 = MfsQiNCUsgdXRtxupVHiwiV43329941;     MfsQiNCUsgdXRtxupVHiwiV43329941 = MfsQiNCUsgdXRtxupVHiwiV68641935;     MfsQiNCUsgdXRtxupVHiwiV68641935 = MfsQiNCUsgdXRtxupVHiwiV90958583;     MfsQiNCUsgdXRtxupVHiwiV90958583 = MfsQiNCUsgdXRtxupVHiwiV29343207;     MfsQiNCUsgdXRtxupVHiwiV29343207 = MfsQiNCUsgdXRtxupVHiwiV79745834;     MfsQiNCUsgdXRtxupVHiwiV79745834 = MfsQiNCUsgdXRtxupVHiwiV68740680;     MfsQiNCUsgdXRtxupVHiwiV68740680 = MfsQiNCUsgdXRtxupVHiwiV71456879;     MfsQiNCUsgdXRtxupVHiwiV71456879 = MfsQiNCUsgdXRtxupVHiwiV96708867;     MfsQiNCUsgdXRtxupVHiwiV96708867 = MfsQiNCUsgdXRtxupVHiwiV1336627;     MfsQiNCUsgdXRtxupVHiwiV1336627 = MfsQiNCUsgdXRtxupVHiwiV13420739;     MfsQiNCUsgdXRtxupVHiwiV13420739 = MfsQiNCUsgdXRtxupVHiwiV48272835;     MfsQiNCUsgdXRtxupVHiwiV48272835 = MfsQiNCUsgdXRtxupVHiwiV70848817;     MfsQiNCUsgdXRtxupVHiwiV70848817 = MfsQiNCUsgdXRtxupVHiwiV7087033;     MfsQiNCUsgdXRtxupVHiwiV7087033 = MfsQiNCUsgdXRtxupVHiwiV61963961;     MfsQiNCUsgdXRtxupVHiwiV61963961 = MfsQiNCUsgdXRtxupVHiwiV60446017;     MfsQiNCUsgdXRtxupVHiwiV60446017 = MfsQiNCUsgdXRtxupVHiwiV70456584;     MfsQiNCUsgdXRtxupVHiwiV70456584 = MfsQiNCUsgdXRtxupVHiwiV430727;     MfsQiNCUsgdXRtxupVHiwiV430727 = MfsQiNCUsgdXRtxupVHiwiV63341401;     MfsQiNCUsgdXRtxupVHiwiV63341401 = MfsQiNCUsgdXRtxupVHiwiV52021066;     MfsQiNCUsgdXRtxupVHiwiV52021066 = MfsQiNCUsgdXRtxupVHiwiV51957736;     MfsQiNCUsgdXRtxupVHiwiV51957736 = MfsQiNCUsgdXRtxupVHiwiV90867449;     MfsQiNCUsgdXRtxupVHiwiV90867449 = MfsQiNCUsgdXRtxupVHiwiV61464305;     MfsQiNCUsgdXRtxupVHiwiV61464305 = MfsQiNCUsgdXRtxupVHiwiV5998183;     MfsQiNCUsgdXRtxupVHiwiV5998183 = MfsQiNCUsgdXRtxupVHiwiV63141542;     MfsQiNCUsgdXRtxupVHiwiV63141542 = MfsQiNCUsgdXRtxupVHiwiV75296568;     MfsQiNCUsgdXRtxupVHiwiV75296568 = MfsQiNCUsgdXRtxupVHiwiV44910682;     MfsQiNCUsgdXRtxupVHiwiV44910682 = MfsQiNCUsgdXRtxupVHiwiV55385279;     MfsQiNCUsgdXRtxupVHiwiV55385279 = MfsQiNCUsgdXRtxupVHiwiV94379743;     MfsQiNCUsgdXRtxupVHiwiV94379743 = MfsQiNCUsgdXRtxupVHiwiV63958595;     MfsQiNCUsgdXRtxupVHiwiV63958595 = MfsQiNCUsgdXRtxupVHiwiV89839981;     MfsQiNCUsgdXRtxupVHiwiV89839981 = MfsQiNCUsgdXRtxupVHiwiV87762245;     MfsQiNCUsgdXRtxupVHiwiV87762245 = MfsQiNCUsgdXRtxupVHiwiV16690063;     MfsQiNCUsgdXRtxupVHiwiV16690063 = MfsQiNCUsgdXRtxupVHiwiV71711548;     MfsQiNCUsgdXRtxupVHiwiV71711548 = MfsQiNCUsgdXRtxupVHiwiV37476698;     MfsQiNCUsgdXRtxupVHiwiV37476698 = MfsQiNCUsgdXRtxupVHiwiV71660850;     MfsQiNCUsgdXRtxupVHiwiV71660850 = MfsQiNCUsgdXRtxupVHiwiV82667548;     MfsQiNCUsgdXRtxupVHiwiV82667548 = MfsQiNCUsgdXRtxupVHiwiV14811564;     MfsQiNCUsgdXRtxupVHiwiV14811564 = MfsQiNCUsgdXRtxupVHiwiV55014456;     MfsQiNCUsgdXRtxupVHiwiV55014456 = MfsQiNCUsgdXRtxupVHiwiV14748675;     MfsQiNCUsgdXRtxupVHiwiV14748675 = MfsQiNCUsgdXRtxupVHiwiV42719104;     MfsQiNCUsgdXRtxupVHiwiV42719104 = MfsQiNCUsgdXRtxupVHiwiV66067258;     MfsQiNCUsgdXRtxupVHiwiV66067258 = MfsQiNCUsgdXRtxupVHiwiV40512646;     MfsQiNCUsgdXRtxupVHiwiV40512646 = MfsQiNCUsgdXRtxupVHiwiV55535909;     MfsQiNCUsgdXRtxupVHiwiV55535909 = MfsQiNCUsgdXRtxupVHiwiV10351092;     MfsQiNCUsgdXRtxupVHiwiV10351092 = MfsQiNCUsgdXRtxupVHiwiV11130656;     MfsQiNCUsgdXRtxupVHiwiV11130656 = MfsQiNCUsgdXRtxupVHiwiV32942803;     MfsQiNCUsgdXRtxupVHiwiV32942803 = MfsQiNCUsgdXRtxupVHiwiV25698359;     MfsQiNCUsgdXRtxupVHiwiV25698359 = MfsQiNCUsgdXRtxupVHiwiV12421467;     MfsQiNCUsgdXRtxupVHiwiV12421467 = MfsQiNCUsgdXRtxupVHiwiV69391320;     MfsQiNCUsgdXRtxupVHiwiV69391320 = MfsQiNCUsgdXRtxupVHiwiV3239680;     MfsQiNCUsgdXRtxupVHiwiV3239680 = MfsQiNCUsgdXRtxupVHiwiV38699075;     MfsQiNCUsgdXRtxupVHiwiV38699075 = MfsQiNCUsgdXRtxupVHiwiV82260554;     MfsQiNCUsgdXRtxupVHiwiV82260554 = MfsQiNCUsgdXRtxupVHiwiV66514482;     MfsQiNCUsgdXRtxupVHiwiV66514482 = MfsQiNCUsgdXRtxupVHiwiV34670629;     MfsQiNCUsgdXRtxupVHiwiV34670629 = MfsQiNCUsgdXRtxupVHiwiV91324810;     MfsQiNCUsgdXRtxupVHiwiV91324810 = MfsQiNCUsgdXRtxupVHiwiV81865635;     MfsQiNCUsgdXRtxupVHiwiV81865635 = MfsQiNCUsgdXRtxupVHiwiV62643752;     MfsQiNCUsgdXRtxupVHiwiV62643752 = MfsQiNCUsgdXRtxupVHiwiV27817042;     MfsQiNCUsgdXRtxupVHiwiV27817042 = MfsQiNCUsgdXRtxupVHiwiV54046638;     MfsQiNCUsgdXRtxupVHiwiV54046638 = MfsQiNCUsgdXRtxupVHiwiV34835152;     MfsQiNCUsgdXRtxupVHiwiV34835152 = MfsQiNCUsgdXRtxupVHiwiV13355401;     MfsQiNCUsgdXRtxupVHiwiV13355401 = MfsQiNCUsgdXRtxupVHiwiV77077136;     MfsQiNCUsgdXRtxupVHiwiV77077136 = MfsQiNCUsgdXRtxupVHiwiV32750272;     MfsQiNCUsgdXRtxupVHiwiV32750272 = MfsQiNCUsgdXRtxupVHiwiV11496645;     MfsQiNCUsgdXRtxupVHiwiV11496645 = MfsQiNCUsgdXRtxupVHiwiV25658493;     MfsQiNCUsgdXRtxupVHiwiV25658493 = MfsQiNCUsgdXRtxupVHiwiV31582772;     MfsQiNCUsgdXRtxupVHiwiV31582772 = MfsQiNCUsgdXRtxupVHiwiV99137268;     MfsQiNCUsgdXRtxupVHiwiV99137268 = MfsQiNCUsgdXRtxupVHiwiV69610335;     MfsQiNCUsgdXRtxupVHiwiV69610335 = MfsQiNCUsgdXRtxupVHiwiV90303110;     MfsQiNCUsgdXRtxupVHiwiV90303110 = MfsQiNCUsgdXRtxupVHiwiV77778468;     MfsQiNCUsgdXRtxupVHiwiV77778468 = MfsQiNCUsgdXRtxupVHiwiV55645020;     MfsQiNCUsgdXRtxupVHiwiV55645020 = MfsQiNCUsgdXRtxupVHiwiV45416271;     MfsQiNCUsgdXRtxupVHiwiV45416271 = MfsQiNCUsgdXRtxupVHiwiV48592727;     MfsQiNCUsgdXRtxupVHiwiV48592727 = MfsQiNCUsgdXRtxupVHiwiV9301962;     MfsQiNCUsgdXRtxupVHiwiV9301962 = MfsQiNCUsgdXRtxupVHiwiV85890478;     MfsQiNCUsgdXRtxupVHiwiV85890478 = MfsQiNCUsgdXRtxupVHiwiV50354804;     MfsQiNCUsgdXRtxupVHiwiV50354804 = MfsQiNCUsgdXRtxupVHiwiV5928397;     MfsQiNCUsgdXRtxupVHiwiV5928397 = MfsQiNCUsgdXRtxupVHiwiV95647091;     MfsQiNCUsgdXRtxupVHiwiV95647091 = MfsQiNCUsgdXRtxupVHiwiV52010886;     MfsQiNCUsgdXRtxupVHiwiV52010886 = MfsQiNCUsgdXRtxupVHiwiV42353766;     MfsQiNCUsgdXRtxupVHiwiV42353766 = MfsQiNCUsgdXRtxupVHiwiV19212323;     MfsQiNCUsgdXRtxupVHiwiV19212323 = MfsQiNCUsgdXRtxupVHiwiV42963813;     MfsQiNCUsgdXRtxupVHiwiV42963813 = MfsQiNCUsgdXRtxupVHiwiV24988424;     MfsQiNCUsgdXRtxupVHiwiV24988424 = MfsQiNCUsgdXRtxupVHiwiV60718916;     MfsQiNCUsgdXRtxupVHiwiV60718916 = MfsQiNCUsgdXRtxupVHiwiV51140907;     MfsQiNCUsgdXRtxupVHiwiV51140907 = MfsQiNCUsgdXRtxupVHiwiV5501692;     MfsQiNCUsgdXRtxupVHiwiV5501692 = MfsQiNCUsgdXRtxupVHiwiV50175581;     MfsQiNCUsgdXRtxupVHiwiV50175581 = MfsQiNCUsgdXRtxupVHiwiV37040920;     MfsQiNCUsgdXRtxupVHiwiV37040920 = MfsQiNCUsgdXRtxupVHiwiV46151887;     MfsQiNCUsgdXRtxupVHiwiV46151887 = MfsQiNCUsgdXRtxupVHiwiV89795215;     MfsQiNCUsgdXRtxupVHiwiV89795215 = MfsQiNCUsgdXRtxupVHiwiV20023797;     MfsQiNCUsgdXRtxupVHiwiV20023797 = MfsQiNCUsgdXRtxupVHiwiV86994522;     MfsQiNCUsgdXRtxupVHiwiV86994522 = MfsQiNCUsgdXRtxupVHiwiV967818;     MfsQiNCUsgdXRtxupVHiwiV967818 = MfsQiNCUsgdXRtxupVHiwiV79913522;     MfsQiNCUsgdXRtxupVHiwiV79913522 = MfsQiNCUsgdXRtxupVHiwiV73696263;}



void GZzjdMNbSyiOzFHRSvhNNjHASv14900176() {     float wuJubtmTRsNkRuCWblTVbkb57419782 = 72978661;    float wuJubtmTRsNkRuCWblTVbkb36727897 = -924760514;    float wuJubtmTRsNkRuCWblTVbkb34188073 = -782921585;    float wuJubtmTRsNkRuCWblTVbkb15683501 = -520450030;    float wuJubtmTRsNkRuCWblTVbkb66189701 = -466329612;    float wuJubtmTRsNkRuCWblTVbkb93531777 = -228293942;    float wuJubtmTRsNkRuCWblTVbkb80210503 = -649653264;    float wuJubtmTRsNkRuCWblTVbkb11982018 = 96513673;    float wuJubtmTRsNkRuCWblTVbkb41178112 = -354180782;    float wuJubtmTRsNkRuCWblTVbkb75448795 = -215299648;    float wuJubtmTRsNkRuCWblTVbkb22634870 = -316744008;    float wuJubtmTRsNkRuCWblTVbkb58952451 = 96971570;    float wuJubtmTRsNkRuCWblTVbkb22510732 = -472486139;    float wuJubtmTRsNkRuCWblTVbkb91082011 = -720345139;    float wuJubtmTRsNkRuCWblTVbkb5515886 = -389115648;    float wuJubtmTRsNkRuCWblTVbkb85234767 = -473457571;    float wuJubtmTRsNkRuCWblTVbkb48138245 = -323570238;    float wuJubtmTRsNkRuCWblTVbkb84926086 = -973888308;    float wuJubtmTRsNkRuCWblTVbkb48063513 = 45218477;    float wuJubtmTRsNkRuCWblTVbkb36071606 = -487671320;    float wuJubtmTRsNkRuCWblTVbkb27785528 = -402615400;    float wuJubtmTRsNkRuCWblTVbkb71458655 = -138847816;    float wuJubtmTRsNkRuCWblTVbkb69606592 = -456418896;    float wuJubtmTRsNkRuCWblTVbkb64484096 = -846887731;    float wuJubtmTRsNkRuCWblTVbkb88623158 = -414417645;    float wuJubtmTRsNkRuCWblTVbkb4950494 = -121242213;    float wuJubtmTRsNkRuCWblTVbkb12459389 = -548396322;    float wuJubtmTRsNkRuCWblTVbkb4063506 = -103381651;    float wuJubtmTRsNkRuCWblTVbkb77916324 = -166152969;    float wuJubtmTRsNkRuCWblTVbkb98689869 = -567019525;    float wuJubtmTRsNkRuCWblTVbkb56660991 = -583796627;    float wuJubtmTRsNkRuCWblTVbkb69703970 = -794041746;    float wuJubtmTRsNkRuCWblTVbkb69509981 = -76281107;    float wuJubtmTRsNkRuCWblTVbkb30104458 = -648625405;    float wuJubtmTRsNkRuCWblTVbkb31561921 = -954969756;    float wuJubtmTRsNkRuCWblTVbkb59701215 = 87507442;    float wuJubtmTRsNkRuCWblTVbkb87525836 = -851824564;    float wuJubtmTRsNkRuCWblTVbkb67071854 = -218362754;    float wuJubtmTRsNkRuCWblTVbkb32327267 = -85059453;    float wuJubtmTRsNkRuCWblTVbkb76665813 = -149553863;    float wuJubtmTRsNkRuCWblTVbkb36428371 = -556009576;    float wuJubtmTRsNkRuCWblTVbkb12007276 = 88341624;    float wuJubtmTRsNkRuCWblTVbkb47611710 = -423104228;    float wuJubtmTRsNkRuCWblTVbkb2209976 = -211812445;    float wuJubtmTRsNkRuCWblTVbkb42735926 = -81647256;    float wuJubtmTRsNkRuCWblTVbkb86985985 = 21407445;    float wuJubtmTRsNkRuCWblTVbkb99246227 = -231473405;    float wuJubtmTRsNkRuCWblTVbkb79254493 = -839528235;    float wuJubtmTRsNkRuCWblTVbkb89410122 = -828915694;    float wuJubtmTRsNkRuCWblTVbkb46224421 = -475014765;    float wuJubtmTRsNkRuCWblTVbkb51221488 = 86665339;    float wuJubtmTRsNkRuCWblTVbkb84448522 = -168754292;    float wuJubtmTRsNkRuCWblTVbkb71080761 = -888860337;    float wuJubtmTRsNkRuCWblTVbkb58354072 = -594684791;    float wuJubtmTRsNkRuCWblTVbkb87114647 = -609529319;    float wuJubtmTRsNkRuCWblTVbkb85961126 = -788173523;    float wuJubtmTRsNkRuCWblTVbkb67121305 = -368341618;    float wuJubtmTRsNkRuCWblTVbkb69703976 = -936033854;    float wuJubtmTRsNkRuCWblTVbkb27060343 = -6032385;    float wuJubtmTRsNkRuCWblTVbkb61239207 = -245087399;    float wuJubtmTRsNkRuCWblTVbkb81072389 = -679897620;    float wuJubtmTRsNkRuCWblTVbkb76146998 = -446271614;    float wuJubtmTRsNkRuCWblTVbkb34065694 = -737333359;    float wuJubtmTRsNkRuCWblTVbkb42488243 = -787161257;    float wuJubtmTRsNkRuCWblTVbkb18787804 = -631503021;    float wuJubtmTRsNkRuCWblTVbkb52930900 = -522702263;    float wuJubtmTRsNkRuCWblTVbkb89442470 = -826747323;    float wuJubtmTRsNkRuCWblTVbkb92406273 = -823860735;    float wuJubtmTRsNkRuCWblTVbkb59520091 = -765375383;    float wuJubtmTRsNkRuCWblTVbkb45814670 = -376623091;    float wuJubtmTRsNkRuCWblTVbkb97708931 = -621633007;    float wuJubtmTRsNkRuCWblTVbkb81066391 = -5207484;    float wuJubtmTRsNkRuCWblTVbkb52598819 = -788828855;    float wuJubtmTRsNkRuCWblTVbkb71397699 = -805227660;    float wuJubtmTRsNkRuCWblTVbkb99643234 = -931661745;    float wuJubtmTRsNkRuCWblTVbkb15778253 = -390957024;    float wuJubtmTRsNkRuCWblTVbkb23846945 = -715743588;    float wuJubtmTRsNkRuCWblTVbkb67396616 = -144606452;    float wuJubtmTRsNkRuCWblTVbkb21748171 = -665240476;    float wuJubtmTRsNkRuCWblTVbkb1637174 = -335825091;    float wuJubtmTRsNkRuCWblTVbkb5704267 = -889768809;    float wuJubtmTRsNkRuCWblTVbkb33204896 = -708868088;    float wuJubtmTRsNkRuCWblTVbkb14653384 = -274465957;    float wuJubtmTRsNkRuCWblTVbkb31691903 = -691138204;    float wuJubtmTRsNkRuCWblTVbkb47468381 = -553684865;    float wuJubtmTRsNkRuCWblTVbkb72212469 = -315042335;    float wuJubtmTRsNkRuCWblTVbkb98623208 = -905181410;    float wuJubtmTRsNkRuCWblTVbkb11155909 = -481596316;    float wuJubtmTRsNkRuCWblTVbkb42989810 = 60903913;    float wuJubtmTRsNkRuCWblTVbkb45600794 = -66796234;    float wuJubtmTRsNkRuCWblTVbkb92579910 = -544150940;    float wuJubtmTRsNkRuCWblTVbkb17821860 = -915790711;    float wuJubtmTRsNkRuCWblTVbkb40011512 = -112330370;    float wuJubtmTRsNkRuCWblTVbkb71088060 = -839972054;    float wuJubtmTRsNkRuCWblTVbkb95593424 = -469656243;    float wuJubtmTRsNkRuCWblTVbkb60281373 = -9737963;    float wuJubtmTRsNkRuCWblTVbkb77941582 = -174325018;    float wuJubtmTRsNkRuCWblTVbkb5123468 = -635942972;    float wuJubtmTRsNkRuCWblTVbkb83422171 = -580309424;    float wuJubtmTRsNkRuCWblTVbkb89805025 = 72978661;     wuJubtmTRsNkRuCWblTVbkb57419782 = wuJubtmTRsNkRuCWblTVbkb36727897;     wuJubtmTRsNkRuCWblTVbkb36727897 = wuJubtmTRsNkRuCWblTVbkb34188073;     wuJubtmTRsNkRuCWblTVbkb34188073 = wuJubtmTRsNkRuCWblTVbkb15683501;     wuJubtmTRsNkRuCWblTVbkb15683501 = wuJubtmTRsNkRuCWblTVbkb66189701;     wuJubtmTRsNkRuCWblTVbkb66189701 = wuJubtmTRsNkRuCWblTVbkb93531777;     wuJubtmTRsNkRuCWblTVbkb93531777 = wuJubtmTRsNkRuCWblTVbkb80210503;     wuJubtmTRsNkRuCWblTVbkb80210503 = wuJubtmTRsNkRuCWblTVbkb11982018;     wuJubtmTRsNkRuCWblTVbkb11982018 = wuJubtmTRsNkRuCWblTVbkb41178112;     wuJubtmTRsNkRuCWblTVbkb41178112 = wuJubtmTRsNkRuCWblTVbkb75448795;     wuJubtmTRsNkRuCWblTVbkb75448795 = wuJubtmTRsNkRuCWblTVbkb22634870;     wuJubtmTRsNkRuCWblTVbkb22634870 = wuJubtmTRsNkRuCWblTVbkb58952451;     wuJubtmTRsNkRuCWblTVbkb58952451 = wuJubtmTRsNkRuCWblTVbkb22510732;     wuJubtmTRsNkRuCWblTVbkb22510732 = wuJubtmTRsNkRuCWblTVbkb91082011;     wuJubtmTRsNkRuCWblTVbkb91082011 = wuJubtmTRsNkRuCWblTVbkb5515886;     wuJubtmTRsNkRuCWblTVbkb5515886 = wuJubtmTRsNkRuCWblTVbkb85234767;     wuJubtmTRsNkRuCWblTVbkb85234767 = wuJubtmTRsNkRuCWblTVbkb48138245;     wuJubtmTRsNkRuCWblTVbkb48138245 = wuJubtmTRsNkRuCWblTVbkb84926086;     wuJubtmTRsNkRuCWblTVbkb84926086 = wuJubtmTRsNkRuCWblTVbkb48063513;     wuJubtmTRsNkRuCWblTVbkb48063513 = wuJubtmTRsNkRuCWblTVbkb36071606;     wuJubtmTRsNkRuCWblTVbkb36071606 = wuJubtmTRsNkRuCWblTVbkb27785528;     wuJubtmTRsNkRuCWblTVbkb27785528 = wuJubtmTRsNkRuCWblTVbkb71458655;     wuJubtmTRsNkRuCWblTVbkb71458655 = wuJubtmTRsNkRuCWblTVbkb69606592;     wuJubtmTRsNkRuCWblTVbkb69606592 = wuJubtmTRsNkRuCWblTVbkb64484096;     wuJubtmTRsNkRuCWblTVbkb64484096 = wuJubtmTRsNkRuCWblTVbkb88623158;     wuJubtmTRsNkRuCWblTVbkb88623158 = wuJubtmTRsNkRuCWblTVbkb4950494;     wuJubtmTRsNkRuCWblTVbkb4950494 = wuJubtmTRsNkRuCWblTVbkb12459389;     wuJubtmTRsNkRuCWblTVbkb12459389 = wuJubtmTRsNkRuCWblTVbkb4063506;     wuJubtmTRsNkRuCWblTVbkb4063506 = wuJubtmTRsNkRuCWblTVbkb77916324;     wuJubtmTRsNkRuCWblTVbkb77916324 = wuJubtmTRsNkRuCWblTVbkb98689869;     wuJubtmTRsNkRuCWblTVbkb98689869 = wuJubtmTRsNkRuCWblTVbkb56660991;     wuJubtmTRsNkRuCWblTVbkb56660991 = wuJubtmTRsNkRuCWblTVbkb69703970;     wuJubtmTRsNkRuCWblTVbkb69703970 = wuJubtmTRsNkRuCWblTVbkb69509981;     wuJubtmTRsNkRuCWblTVbkb69509981 = wuJubtmTRsNkRuCWblTVbkb30104458;     wuJubtmTRsNkRuCWblTVbkb30104458 = wuJubtmTRsNkRuCWblTVbkb31561921;     wuJubtmTRsNkRuCWblTVbkb31561921 = wuJubtmTRsNkRuCWblTVbkb59701215;     wuJubtmTRsNkRuCWblTVbkb59701215 = wuJubtmTRsNkRuCWblTVbkb87525836;     wuJubtmTRsNkRuCWblTVbkb87525836 = wuJubtmTRsNkRuCWblTVbkb67071854;     wuJubtmTRsNkRuCWblTVbkb67071854 = wuJubtmTRsNkRuCWblTVbkb32327267;     wuJubtmTRsNkRuCWblTVbkb32327267 = wuJubtmTRsNkRuCWblTVbkb76665813;     wuJubtmTRsNkRuCWblTVbkb76665813 = wuJubtmTRsNkRuCWblTVbkb36428371;     wuJubtmTRsNkRuCWblTVbkb36428371 = wuJubtmTRsNkRuCWblTVbkb12007276;     wuJubtmTRsNkRuCWblTVbkb12007276 = wuJubtmTRsNkRuCWblTVbkb47611710;     wuJubtmTRsNkRuCWblTVbkb47611710 = wuJubtmTRsNkRuCWblTVbkb2209976;     wuJubtmTRsNkRuCWblTVbkb2209976 = wuJubtmTRsNkRuCWblTVbkb42735926;     wuJubtmTRsNkRuCWblTVbkb42735926 = wuJubtmTRsNkRuCWblTVbkb86985985;     wuJubtmTRsNkRuCWblTVbkb86985985 = wuJubtmTRsNkRuCWblTVbkb99246227;     wuJubtmTRsNkRuCWblTVbkb99246227 = wuJubtmTRsNkRuCWblTVbkb79254493;     wuJubtmTRsNkRuCWblTVbkb79254493 = wuJubtmTRsNkRuCWblTVbkb89410122;     wuJubtmTRsNkRuCWblTVbkb89410122 = wuJubtmTRsNkRuCWblTVbkb46224421;     wuJubtmTRsNkRuCWblTVbkb46224421 = wuJubtmTRsNkRuCWblTVbkb51221488;     wuJubtmTRsNkRuCWblTVbkb51221488 = wuJubtmTRsNkRuCWblTVbkb84448522;     wuJubtmTRsNkRuCWblTVbkb84448522 = wuJubtmTRsNkRuCWblTVbkb71080761;     wuJubtmTRsNkRuCWblTVbkb71080761 = wuJubtmTRsNkRuCWblTVbkb58354072;     wuJubtmTRsNkRuCWblTVbkb58354072 = wuJubtmTRsNkRuCWblTVbkb87114647;     wuJubtmTRsNkRuCWblTVbkb87114647 = wuJubtmTRsNkRuCWblTVbkb85961126;     wuJubtmTRsNkRuCWblTVbkb85961126 = wuJubtmTRsNkRuCWblTVbkb67121305;     wuJubtmTRsNkRuCWblTVbkb67121305 = wuJubtmTRsNkRuCWblTVbkb69703976;     wuJubtmTRsNkRuCWblTVbkb69703976 = wuJubtmTRsNkRuCWblTVbkb27060343;     wuJubtmTRsNkRuCWblTVbkb27060343 = wuJubtmTRsNkRuCWblTVbkb61239207;     wuJubtmTRsNkRuCWblTVbkb61239207 = wuJubtmTRsNkRuCWblTVbkb81072389;     wuJubtmTRsNkRuCWblTVbkb81072389 = wuJubtmTRsNkRuCWblTVbkb76146998;     wuJubtmTRsNkRuCWblTVbkb76146998 = wuJubtmTRsNkRuCWblTVbkb34065694;     wuJubtmTRsNkRuCWblTVbkb34065694 = wuJubtmTRsNkRuCWblTVbkb42488243;     wuJubtmTRsNkRuCWblTVbkb42488243 = wuJubtmTRsNkRuCWblTVbkb18787804;     wuJubtmTRsNkRuCWblTVbkb18787804 = wuJubtmTRsNkRuCWblTVbkb52930900;     wuJubtmTRsNkRuCWblTVbkb52930900 = wuJubtmTRsNkRuCWblTVbkb89442470;     wuJubtmTRsNkRuCWblTVbkb89442470 = wuJubtmTRsNkRuCWblTVbkb92406273;     wuJubtmTRsNkRuCWblTVbkb92406273 = wuJubtmTRsNkRuCWblTVbkb59520091;     wuJubtmTRsNkRuCWblTVbkb59520091 = wuJubtmTRsNkRuCWblTVbkb45814670;     wuJubtmTRsNkRuCWblTVbkb45814670 = wuJubtmTRsNkRuCWblTVbkb97708931;     wuJubtmTRsNkRuCWblTVbkb97708931 = wuJubtmTRsNkRuCWblTVbkb81066391;     wuJubtmTRsNkRuCWblTVbkb81066391 = wuJubtmTRsNkRuCWblTVbkb52598819;     wuJubtmTRsNkRuCWblTVbkb52598819 = wuJubtmTRsNkRuCWblTVbkb71397699;     wuJubtmTRsNkRuCWblTVbkb71397699 = wuJubtmTRsNkRuCWblTVbkb99643234;     wuJubtmTRsNkRuCWblTVbkb99643234 = wuJubtmTRsNkRuCWblTVbkb15778253;     wuJubtmTRsNkRuCWblTVbkb15778253 = wuJubtmTRsNkRuCWblTVbkb23846945;     wuJubtmTRsNkRuCWblTVbkb23846945 = wuJubtmTRsNkRuCWblTVbkb67396616;     wuJubtmTRsNkRuCWblTVbkb67396616 = wuJubtmTRsNkRuCWblTVbkb21748171;     wuJubtmTRsNkRuCWblTVbkb21748171 = wuJubtmTRsNkRuCWblTVbkb1637174;     wuJubtmTRsNkRuCWblTVbkb1637174 = wuJubtmTRsNkRuCWblTVbkb5704267;     wuJubtmTRsNkRuCWblTVbkb5704267 = wuJubtmTRsNkRuCWblTVbkb33204896;     wuJubtmTRsNkRuCWblTVbkb33204896 = wuJubtmTRsNkRuCWblTVbkb14653384;     wuJubtmTRsNkRuCWblTVbkb14653384 = wuJubtmTRsNkRuCWblTVbkb31691903;     wuJubtmTRsNkRuCWblTVbkb31691903 = wuJubtmTRsNkRuCWblTVbkb47468381;     wuJubtmTRsNkRuCWblTVbkb47468381 = wuJubtmTRsNkRuCWblTVbkb72212469;     wuJubtmTRsNkRuCWblTVbkb72212469 = wuJubtmTRsNkRuCWblTVbkb98623208;     wuJubtmTRsNkRuCWblTVbkb98623208 = wuJubtmTRsNkRuCWblTVbkb11155909;     wuJubtmTRsNkRuCWblTVbkb11155909 = wuJubtmTRsNkRuCWblTVbkb42989810;     wuJubtmTRsNkRuCWblTVbkb42989810 = wuJubtmTRsNkRuCWblTVbkb45600794;     wuJubtmTRsNkRuCWblTVbkb45600794 = wuJubtmTRsNkRuCWblTVbkb92579910;     wuJubtmTRsNkRuCWblTVbkb92579910 = wuJubtmTRsNkRuCWblTVbkb17821860;     wuJubtmTRsNkRuCWblTVbkb17821860 = wuJubtmTRsNkRuCWblTVbkb40011512;     wuJubtmTRsNkRuCWblTVbkb40011512 = wuJubtmTRsNkRuCWblTVbkb71088060;     wuJubtmTRsNkRuCWblTVbkb71088060 = wuJubtmTRsNkRuCWblTVbkb95593424;     wuJubtmTRsNkRuCWblTVbkb95593424 = wuJubtmTRsNkRuCWblTVbkb60281373;     wuJubtmTRsNkRuCWblTVbkb60281373 = wuJubtmTRsNkRuCWblTVbkb77941582;     wuJubtmTRsNkRuCWblTVbkb77941582 = wuJubtmTRsNkRuCWblTVbkb5123468;     wuJubtmTRsNkRuCWblTVbkb5123468 = wuJubtmTRsNkRuCWblTVbkb83422171;     wuJubtmTRsNkRuCWblTVbkb83422171 = wuJubtmTRsNkRuCWblTVbkb89805025;     wuJubtmTRsNkRuCWblTVbkb89805025 = wuJubtmTRsNkRuCWblTVbkb57419782;}



void PbsnKaIhXPUKxrbjftUPdtUQLk99963013() {     float AsVoiaOuVheLDJjAVOdYSXP41143300 = -277043406;    float AsVoiaOuVheLDJjAVOdYSXP34325993 = -728412365;    float AsVoiaOuVheLDJjAVOdYSXP22774190 = -651229275;    float AsVoiaOuVheLDJjAVOdYSXP12831454 = -525331959;    float AsVoiaOuVheLDJjAVOdYSXP45751037 = -864277803;    float AsVoiaOuVheLDJjAVOdYSXP4871296 = -331614611;    float AsVoiaOuVheLDJjAVOdYSXP17091067 = -671167402;    float AsVoiaOuVheLDJjAVOdYSXP55322101 = -523940913;    float AsVoiaOuVheLDJjAVOdYSXP91397640 = 48336522;    float AsVoiaOuVheLDJjAVOdYSXP21554384 = -438340664;    float AsVoiaOuVheLDJjAVOdYSXP65523906 = -157029266;    float AsVoiaOuVheLDJjAVOdYSXP49164222 = -932167701;    float AsVoiaOuVheLDJjAVOdYSXP73564583 = -315318383;    float AsVoiaOuVheLDJjAVOdYSXP85455156 = 29971998;    float AsVoiaOuVheLDJjAVOdYSXP9695145 = -314812953;    float AsVoiaOuVheLDJjAVOdYSXP57048796 = -397585222;    float AsVoiaOuVheLDJjAVOdYSXP48003656 = -482450100;    float AsVoiaOuVheLDJjAVOdYSXP99003355 = -584739882;    float AsVoiaOuVheLDJjAVOdYSXP89039993 = -843430761;    float AsVoiaOuVheLDJjAVOdYSXP10179250 = -103964237;    float AsVoiaOuVheLDJjAVOdYSXP95125037 = 22918080;    float AsVoiaOuVheLDJjAVOdYSXP72460726 = -52003103;    float AsVoiaOuVheLDJjAVOdYSXP38782457 = -485813633;    float AsVoiaOuVheLDJjAVOdYSXP65626791 = -386120384;    float AsVoiaOuVheLDJjAVOdYSXP25225252 = -492748407;    float AsVoiaOuVheLDJjAVOdYSXP57943251 = -564209240;    float AsVoiaOuVheLDJjAVOdYSXP34051328 = -992271745;    float AsVoiaOuVheLDJjAVOdYSXP46662705 = -989823139;    float AsVoiaOuVheLDJjAVOdYSXP49834466 = -690348869;    float AsVoiaOuVheLDJjAVOdYSXP34238196 = -139625900;    float AsVoiaOuVheLDJjAVOdYSXP38025415 = -802894648;    float AsVoiaOuVheLDJjAVOdYSXP94497258 = -243238969;    float AsVoiaOuVheLDJjAVOdYSXP83634682 = -741495198;    float AsVoiaOuVheLDJjAVOdYSXP65829172 = 47277947;    float AsVoiaOuVheLDJjAVOdYSXP99165245 = -689260364;    float AsVoiaOuVheLDJjAVOdYSXP29562449 = -990889035;    float AsVoiaOuVheLDJjAVOdYSXP87289426 = -72165861;    float AsVoiaOuVheLDJjAVOdYSXP17453645 = -117505971;    float AsVoiaOuVheLDJjAVOdYSXP92942986 = -390618334;    float AsVoiaOuVheLDJjAVOdYSXP15854930 = -96200743;    float AsVoiaOuVheLDJjAVOdYSXP1195892 = -206049703;    float AsVoiaOuVheLDJjAVOdYSXP41347002 = -617684045;    float AsVoiaOuVheLDJjAVOdYSXP80411856 = -75508483;    float AsVoiaOuVheLDJjAVOdYSXP49405495 = -890756177;    float AsVoiaOuVheLDJjAVOdYSXP70723176 = -407661335;    float AsVoiaOuVheLDJjAVOdYSXP31252867 = -693624247;    float AsVoiaOuVheLDJjAVOdYSXP32425197 = -253119312;    float AsVoiaOuVheLDJjAVOdYSXP17996340 = -601650133;    float AsVoiaOuVheLDJjAVOdYSXP23284336 = -528699845;    float AsVoiaOuVheLDJjAVOdYSXP82097751 = -7030201;    float AsVoiaOuVheLDJjAVOdYSXP91312319 = -79107062;    float AsVoiaOuVheLDJjAVOdYSXP35954242 = 79192921;    float AsVoiaOuVheLDJjAVOdYSXP16463164 = -900792703;    float AsVoiaOuVheLDJjAVOdYSXP4286679 = -71393979;    float AsVoiaOuVheLDJjAVOdYSXP4837976 = -926256264;    float AsVoiaOuVheLDJjAVOdYSXP68682573 = -125040304;    float AsVoiaOuVheLDJjAVOdYSXP95543535 = -142598733;    float AsVoiaOuVheLDJjAVOdYSXP57147398 = -165108891;    float AsVoiaOuVheLDJjAVOdYSXP87606202 = 67416448;    float AsVoiaOuVheLDJjAVOdYSXP87807786 = -200068563;    float AsVoiaOuVheLDJjAVOdYSXP70819967 = -339342866;    float AsVoiaOuVheLDJjAVOdYSXP70428361 = -681344263;    float AsVoiaOuVheLDJjAVOdYSXP5487635 = -833592045;    float AsVoiaOuVheLDJjAVOdYSXP57159444 = -812037578;    float AsVoiaOuVheLDJjAVOdYSXP83528969 = -635446016;    float AsVoiaOuVheLDJjAVOdYSXP71026647 = -913790298;    float AsVoiaOuVheLDJjAVOdYSXP65529540 = -90672503;    float AsVoiaOuVheLDJjAVOdYSXP7735412 = -262596331;    float AsVoiaOuVheLDJjAVOdYSXP86289910 = -280767638;    float AsVoiaOuVheLDJjAVOdYSXP80132695 = -323923919;    float AsVoiaOuVheLDJjAVOdYSXP69759369 = -225419362;    float AsVoiaOuVheLDJjAVOdYSXP30550011 = -264944130;    float AsVoiaOuVheLDJjAVOdYSXP6060370 = -94121548;    float AsVoiaOuVheLDJjAVOdYSXP73185064 = -647230019;    float AsVoiaOuVheLDJjAVOdYSXP8983359 = -897914535;    float AsVoiaOuVheLDJjAVOdYSXP53778036 = -359397876;    float AsVoiaOuVheLDJjAVOdYSXP92048869 = -976494620;    float AsVoiaOuVheLDJjAVOdYSXP89376962 = -595057456;    float AsVoiaOuVheLDJjAVOdYSXP94903615 = -978459050;    float AsVoiaOuVheLDJjAVOdYSXP93972384 = -799124160;    float AsVoiaOuVheLDJjAVOdYSXP25518055 = -211089929;    float AsVoiaOuVheLDJjAVOdYSXP16054988 = -290621613;    float AsVoiaOuVheLDJjAVOdYSXP23378370 = -361123294;    float AsVoiaOuVheLDJjAVOdYSXP67736715 = -583318668;    float AsVoiaOuVheLDJjAVOdYSXP42925877 = 39481162;    float AsVoiaOuVheLDJjAVOdYSXP2071173 = -782087570;    float AsVoiaOuVheLDJjAVOdYSXP78034095 = -342446267;    float AsVoiaOuVheLDJjAVOdYSXP79348003 = -570101220;    float AsVoiaOuVheLDJjAVOdYSXP60991196 = -26465790;    float AsVoiaOuVheLDJjAVOdYSXP30482673 = -464220061;    float AsVoiaOuVheLDJjAVOdYSXP34018914 = -748290303;    float AsVoiaOuVheLDJjAVOdYSXP30142028 = -907056970;    float AsVoiaOuVheLDJjAVOdYSXP29847443 = -84922419;    float AsVoiaOuVheLDJjAVOdYSXP5135201 = -90549772;    float AsVoiaOuVheLDJjAVOdYSXP45034962 = -756857877;    float AsVoiaOuVheLDJjAVOdYSXP30767531 = -524705440;    float AsVoiaOuVheLDJjAVOdYSXP35859367 = -784092001;    float AsVoiaOuVheLDJjAVOdYSXP23252413 = -263470905;    float AsVoiaOuVheLDJjAVOdYSXP65876526 = -155310161;    float AsVoiaOuVheLDJjAVOdYSXP99696528 = -277043406;     AsVoiaOuVheLDJjAVOdYSXP41143300 = AsVoiaOuVheLDJjAVOdYSXP34325993;     AsVoiaOuVheLDJjAVOdYSXP34325993 = AsVoiaOuVheLDJjAVOdYSXP22774190;     AsVoiaOuVheLDJjAVOdYSXP22774190 = AsVoiaOuVheLDJjAVOdYSXP12831454;     AsVoiaOuVheLDJjAVOdYSXP12831454 = AsVoiaOuVheLDJjAVOdYSXP45751037;     AsVoiaOuVheLDJjAVOdYSXP45751037 = AsVoiaOuVheLDJjAVOdYSXP4871296;     AsVoiaOuVheLDJjAVOdYSXP4871296 = AsVoiaOuVheLDJjAVOdYSXP17091067;     AsVoiaOuVheLDJjAVOdYSXP17091067 = AsVoiaOuVheLDJjAVOdYSXP55322101;     AsVoiaOuVheLDJjAVOdYSXP55322101 = AsVoiaOuVheLDJjAVOdYSXP91397640;     AsVoiaOuVheLDJjAVOdYSXP91397640 = AsVoiaOuVheLDJjAVOdYSXP21554384;     AsVoiaOuVheLDJjAVOdYSXP21554384 = AsVoiaOuVheLDJjAVOdYSXP65523906;     AsVoiaOuVheLDJjAVOdYSXP65523906 = AsVoiaOuVheLDJjAVOdYSXP49164222;     AsVoiaOuVheLDJjAVOdYSXP49164222 = AsVoiaOuVheLDJjAVOdYSXP73564583;     AsVoiaOuVheLDJjAVOdYSXP73564583 = AsVoiaOuVheLDJjAVOdYSXP85455156;     AsVoiaOuVheLDJjAVOdYSXP85455156 = AsVoiaOuVheLDJjAVOdYSXP9695145;     AsVoiaOuVheLDJjAVOdYSXP9695145 = AsVoiaOuVheLDJjAVOdYSXP57048796;     AsVoiaOuVheLDJjAVOdYSXP57048796 = AsVoiaOuVheLDJjAVOdYSXP48003656;     AsVoiaOuVheLDJjAVOdYSXP48003656 = AsVoiaOuVheLDJjAVOdYSXP99003355;     AsVoiaOuVheLDJjAVOdYSXP99003355 = AsVoiaOuVheLDJjAVOdYSXP89039993;     AsVoiaOuVheLDJjAVOdYSXP89039993 = AsVoiaOuVheLDJjAVOdYSXP10179250;     AsVoiaOuVheLDJjAVOdYSXP10179250 = AsVoiaOuVheLDJjAVOdYSXP95125037;     AsVoiaOuVheLDJjAVOdYSXP95125037 = AsVoiaOuVheLDJjAVOdYSXP72460726;     AsVoiaOuVheLDJjAVOdYSXP72460726 = AsVoiaOuVheLDJjAVOdYSXP38782457;     AsVoiaOuVheLDJjAVOdYSXP38782457 = AsVoiaOuVheLDJjAVOdYSXP65626791;     AsVoiaOuVheLDJjAVOdYSXP65626791 = AsVoiaOuVheLDJjAVOdYSXP25225252;     AsVoiaOuVheLDJjAVOdYSXP25225252 = AsVoiaOuVheLDJjAVOdYSXP57943251;     AsVoiaOuVheLDJjAVOdYSXP57943251 = AsVoiaOuVheLDJjAVOdYSXP34051328;     AsVoiaOuVheLDJjAVOdYSXP34051328 = AsVoiaOuVheLDJjAVOdYSXP46662705;     AsVoiaOuVheLDJjAVOdYSXP46662705 = AsVoiaOuVheLDJjAVOdYSXP49834466;     AsVoiaOuVheLDJjAVOdYSXP49834466 = AsVoiaOuVheLDJjAVOdYSXP34238196;     AsVoiaOuVheLDJjAVOdYSXP34238196 = AsVoiaOuVheLDJjAVOdYSXP38025415;     AsVoiaOuVheLDJjAVOdYSXP38025415 = AsVoiaOuVheLDJjAVOdYSXP94497258;     AsVoiaOuVheLDJjAVOdYSXP94497258 = AsVoiaOuVheLDJjAVOdYSXP83634682;     AsVoiaOuVheLDJjAVOdYSXP83634682 = AsVoiaOuVheLDJjAVOdYSXP65829172;     AsVoiaOuVheLDJjAVOdYSXP65829172 = AsVoiaOuVheLDJjAVOdYSXP99165245;     AsVoiaOuVheLDJjAVOdYSXP99165245 = AsVoiaOuVheLDJjAVOdYSXP29562449;     AsVoiaOuVheLDJjAVOdYSXP29562449 = AsVoiaOuVheLDJjAVOdYSXP87289426;     AsVoiaOuVheLDJjAVOdYSXP87289426 = AsVoiaOuVheLDJjAVOdYSXP17453645;     AsVoiaOuVheLDJjAVOdYSXP17453645 = AsVoiaOuVheLDJjAVOdYSXP92942986;     AsVoiaOuVheLDJjAVOdYSXP92942986 = AsVoiaOuVheLDJjAVOdYSXP15854930;     AsVoiaOuVheLDJjAVOdYSXP15854930 = AsVoiaOuVheLDJjAVOdYSXP1195892;     AsVoiaOuVheLDJjAVOdYSXP1195892 = AsVoiaOuVheLDJjAVOdYSXP41347002;     AsVoiaOuVheLDJjAVOdYSXP41347002 = AsVoiaOuVheLDJjAVOdYSXP80411856;     AsVoiaOuVheLDJjAVOdYSXP80411856 = AsVoiaOuVheLDJjAVOdYSXP49405495;     AsVoiaOuVheLDJjAVOdYSXP49405495 = AsVoiaOuVheLDJjAVOdYSXP70723176;     AsVoiaOuVheLDJjAVOdYSXP70723176 = AsVoiaOuVheLDJjAVOdYSXP31252867;     AsVoiaOuVheLDJjAVOdYSXP31252867 = AsVoiaOuVheLDJjAVOdYSXP32425197;     AsVoiaOuVheLDJjAVOdYSXP32425197 = AsVoiaOuVheLDJjAVOdYSXP17996340;     AsVoiaOuVheLDJjAVOdYSXP17996340 = AsVoiaOuVheLDJjAVOdYSXP23284336;     AsVoiaOuVheLDJjAVOdYSXP23284336 = AsVoiaOuVheLDJjAVOdYSXP82097751;     AsVoiaOuVheLDJjAVOdYSXP82097751 = AsVoiaOuVheLDJjAVOdYSXP91312319;     AsVoiaOuVheLDJjAVOdYSXP91312319 = AsVoiaOuVheLDJjAVOdYSXP35954242;     AsVoiaOuVheLDJjAVOdYSXP35954242 = AsVoiaOuVheLDJjAVOdYSXP16463164;     AsVoiaOuVheLDJjAVOdYSXP16463164 = AsVoiaOuVheLDJjAVOdYSXP4286679;     AsVoiaOuVheLDJjAVOdYSXP4286679 = AsVoiaOuVheLDJjAVOdYSXP4837976;     AsVoiaOuVheLDJjAVOdYSXP4837976 = AsVoiaOuVheLDJjAVOdYSXP68682573;     AsVoiaOuVheLDJjAVOdYSXP68682573 = AsVoiaOuVheLDJjAVOdYSXP95543535;     AsVoiaOuVheLDJjAVOdYSXP95543535 = AsVoiaOuVheLDJjAVOdYSXP57147398;     AsVoiaOuVheLDJjAVOdYSXP57147398 = AsVoiaOuVheLDJjAVOdYSXP87606202;     AsVoiaOuVheLDJjAVOdYSXP87606202 = AsVoiaOuVheLDJjAVOdYSXP87807786;     AsVoiaOuVheLDJjAVOdYSXP87807786 = AsVoiaOuVheLDJjAVOdYSXP70819967;     AsVoiaOuVheLDJjAVOdYSXP70819967 = AsVoiaOuVheLDJjAVOdYSXP70428361;     AsVoiaOuVheLDJjAVOdYSXP70428361 = AsVoiaOuVheLDJjAVOdYSXP5487635;     AsVoiaOuVheLDJjAVOdYSXP5487635 = AsVoiaOuVheLDJjAVOdYSXP57159444;     AsVoiaOuVheLDJjAVOdYSXP57159444 = AsVoiaOuVheLDJjAVOdYSXP83528969;     AsVoiaOuVheLDJjAVOdYSXP83528969 = AsVoiaOuVheLDJjAVOdYSXP71026647;     AsVoiaOuVheLDJjAVOdYSXP71026647 = AsVoiaOuVheLDJjAVOdYSXP65529540;     AsVoiaOuVheLDJjAVOdYSXP65529540 = AsVoiaOuVheLDJjAVOdYSXP7735412;     AsVoiaOuVheLDJjAVOdYSXP7735412 = AsVoiaOuVheLDJjAVOdYSXP86289910;     AsVoiaOuVheLDJjAVOdYSXP86289910 = AsVoiaOuVheLDJjAVOdYSXP80132695;     AsVoiaOuVheLDJjAVOdYSXP80132695 = AsVoiaOuVheLDJjAVOdYSXP69759369;     AsVoiaOuVheLDJjAVOdYSXP69759369 = AsVoiaOuVheLDJjAVOdYSXP30550011;     AsVoiaOuVheLDJjAVOdYSXP30550011 = AsVoiaOuVheLDJjAVOdYSXP6060370;     AsVoiaOuVheLDJjAVOdYSXP6060370 = AsVoiaOuVheLDJjAVOdYSXP73185064;     AsVoiaOuVheLDJjAVOdYSXP73185064 = AsVoiaOuVheLDJjAVOdYSXP8983359;     AsVoiaOuVheLDJjAVOdYSXP8983359 = AsVoiaOuVheLDJjAVOdYSXP53778036;     AsVoiaOuVheLDJjAVOdYSXP53778036 = AsVoiaOuVheLDJjAVOdYSXP92048869;     AsVoiaOuVheLDJjAVOdYSXP92048869 = AsVoiaOuVheLDJjAVOdYSXP89376962;     AsVoiaOuVheLDJjAVOdYSXP89376962 = AsVoiaOuVheLDJjAVOdYSXP94903615;     AsVoiaOuVheLDJjAVOdYSXP94903615 = AsVoiaOuVheLDJjAVOdYSXP93972384;     AsVoiaOuVheLDJjAVOdYSXP93972384 = AsVoiaOuVheLDJjAVOdYSXP25518055;     AsVoiaOuVheLDJjAVOdYSXP25518055 = AsVoiaOuVheLDJjAVOdYSXP16054988;     AsVoiaOuVheLDJjAVOdYSXP16054988 = AsVoiaOuVheLDJjAVOdYSXP23378370;     AsVoiaOuVheLDJjAVOdYSXP23378370 = AsVoiaOuVheLDJjAVOdYSXP67736715;     AsVoiaOuVheLDJjAVOdYSXP67736715 = AsVoiaOuVheLDJjAVOdYSXP42925877;     AsVoiaOuVheLDJjAVOdYSXP42925877 = AsVoiaOuVheLDJjAVOdYSXP2071173;     AsVoiaOuVheLDJjAVOdYSXP2071173 = AsVoiaOuVheLDJjAVOdYSXP78034095;     AsVoiaOuVheLDJjAVOdYSXP78034095 = AsVoiaOuVheLDJjAVOdYSXP79348003;     AsVoiaOuVheLDJjAVOdYSXP79348003 = AsVoiaOuVheLDJjAVOdYSXP60991196;     AsVoiaOuVheLDJjAVOdYSXP60991196 = AsVoiaOuVheLDJjAVOdYSXP30482673;     AsVoiaOuVheLDJjAVOdYSXP30482673 = AsVoiaOuVheLDJjAVOdYSXP34018914;     AsVoiaOuVheLDJjAVOdYSXP34018914 = AsVoiaOuVheLDJjAVOdYSXP30142028;     AsVoiaOuVheLDJjAVOdYSXP30142028 = AsVoiaOuVheLDJjAVOdYSXP29847443;     AsVoiaOuVheLDJjAVOdYSXP29847443 = AsVoiaOuVheLDJjAVOdYSXP5135201;     AsVoiaOuVheLDJjAVOdYSXP5135201 = AsVoiaOuVheLDJjAVOdYSXP45034962;     AsVoiaOuVheLDJjAVOdYSXP45034962 = AsVoiaOuVheLDJjAVOdYSXP30767531;     AsVoiaOuVheLDJjAVOdYSXP30767531 = AsVoiaOuVheLDJjAVOdYSXP35859367;     AsVoiaOuVheLDJjAVOdYSXP35859367 = AsVoiaOuVheLDJjAVOdYSXP23252413;     AsVoiaOuVheLDJjAVOdYSXP23252413 = AsVoiaOuVheLDJjAVOdYSXP65876526;     AsVoiaOuVheLDJjAVOdYSXP65876526 = AsVoiaOuVheLDJjAVOdYSXP99696528;     AsVoiaOuVheLDJjAVOdYSXP99696528 = AsVoiaOuVheLDJjAVOdYSXP41143300;}



void aenheXvcKzBGjbpQRmsXnrFHUb20906354() {     float elWYQoCFobxmREDpOGDSIni94336605 = -797065649;    float elWYQoCFobxmREDpOGDSIni50304873 = 32706569;    float elWYQoCFobxmREDpOGDSIni52868996 = -456883428;    float elWYQoCFobxmREDpOGDSIni32356591 = -116652943;    float elWYQoCFobxmREDpOGDSIni23548865 = -121409580;    float elWYQoCFobxmREDpOGDSIni33101529 = -400561845;    float elWYQoCFobxmREDpOGDSIni56666674 = -384853653;    float elWYQoCFobxmREDpOGDSIni96608904 = -762159136;    float elWYQoCFobxmREDpOGDSIni31618925 = -73926036;    float elWYQoCFobxmREDpOGDSIni46428817 = 67233992;    float elWYQoCFobxmREDpOGDSIni57556054 = -990436806;    float elWYQoCFobxmREDpOGDSIni39297687 = -376740086;    float elWYQoCFobxmREDpOGDSIni57026866 = -852093284;    float elWYQoCFobxmREDpOGDSIni99783286 = -225708327;    float elWYQoCFobxmREDpOGDSIni13107837 = -547915837;    float elWYQoCFobxmREDpOGDSIni1437337 = -426705895;    float elWYQoCFobxmREDpOGDSIni39867990 = -308201002;    float elWYQoCFobxmREDpOGDSIni56393244 = -122078268;    float elWYQoCFobxmREDpOGDSIni49544286 = -304789193;    float elWYQoCFobxmREDpOGDSIni96879755 = -834787498;    float elWYQoCFobxmREDpOGDSIni55003264 = -780144173;    float elWYQoCFobxmREDpOGDSIni43870814 = -624463631;    float elWYQoCFobxmREDpOGDSIni66911729 = -973043528;    float elWYQoCFobxmREDpOGDSIni23578628 = -141666899;    float elWYQoCFobxmREDpOGDSIni90920160 = -483705815;    float elWYQoCFobxmREDpOGDSIni86559951 = -165920004;    float elWYQoCFobxmREDpOGDSIni47016003 = 73901828;    float elWYQoCFobxmREDpOGDSIni31202699 = -237756159;    float elWYQoCFobxmREDpOGDSIni10327953 = -241938336;    float elWYQoCFobxmREDpOGDSIni60470910 = -984813126;    float elWYQoCFobxmREDpOGDSIni40840753 = -240545453;    float elWYQoCFobxmREDpOGDSIni62688893 = -708829769;    float elWYQoCFobxmREDpOGDSIni37872381 = 13568998;    float elWYQoCFobxmREDpOGDSIni60239684 = -967251474;    float elWYQoCFobxmREDpOGDSIni36109398 = -421425297;    float elWYQoCFobxmREDpOGDSIni24782573 = -977912684;    float elWYQoCFobxmREDpOGDSIni77451125 = -69469887;    float elWYQoCFobxmREDpOGDSIni41838491 = -95042334;    float elWYQoCFobxmREDpOGDSIni62043631 = -707421687;    float elWYQoCFobxmREDpOGDSIni83357558 = -412020797;    float elWYQoCFobxmREDpOGDSIni10481553 = -618890150;    float elWYQoCFobxmREDpOGDSIni86121446 = -414157920;    float elWYQoCFobxmREDpOGDSIni94274404 = -85931971;    float elWYQoCFobxmREDpOGDSIni40178579 = 70468540;    float elWYQoCFobxmREDpOGDSIni7734326 = -67483525;    float elWYQoCFobxmREDpOGDSIni19873883 = -67976192;    float elWYQoCFobxmREDpOGDSIni90669597 = -618138386;    float elWYQoCFobxmREDpOGDSIni13048123 = -669869006;    float elWYQoCFobxmREDpOGDSIni68629542 = -76482270;    float elWYQoCFobxmREDpOGDSIni3858068 = -503301760;    float elWYQoCFobxmREDpOGDSIni52523877 = -756605643;    float elWYQoCFobxmREDpOGDSIni50272008 = -779676287;    float elWYQoCFobxmREDpOGDSIni87808625 = -340820527;    float elWYQoCFobxmREDpOGDSIni70586745 = -784716840;    float elWYQoCFobxmREDpOGDSIni65903089 = 21682976;    float elWYQoCFobxmREDpOGDSIni50465791 = -72602019;    float elWYQoCFobxmREDpOGDSIni83393143 = 5750096;    float elWYQoCFobxmREDpOGDSIni29290368 = -215216529;    float elWYQoCFobxmREDpOGDSIni41436430 = -632947129;    float elWYQoCFobxmREDpOGDSIni36988914 = -955489576;    float elWYQoCFobxmREDpOGDSIni86085526 = -374463674;    float elWYQoCFobxmREDpOGDSIni25463975 = -47097494;    float elWYQoCFobxmREDpOGDSIni86280951 = -420220800;    float elWYQoCFobxmREDpOGDSIni71148015 = -89112911;    float elWYQoCFobxmREDpOGDSIni5588064 = -692220555;    float elWYQoCFobxmREDpOGDSIni94867161 = -181607037;    float elWYQoCFobxmREDpOGDSIni1425306 = -290309084;    float elWYQoCFobxmREDpOGDSIni96787182 = -884841811;    float elWYQoCFobxmREDpOGDSIni63673888 = -804283031;    float elWYQoCFobxmREDpOGDSIni88325264 = -570003153;    float elWYQoCFobxmREDpOGDSIni23986212 = -257236008;    float elWYQoCFobxmREDpOGDSIni98029499 = -113158669;    float elWYQoCFobxmREDpOGDSIni94349612 = -414656582;    float elWYQoCFobxmREDpOGDSIni66186728 = -892768397;    float elWYQoCFobxmREDpOGDSIni86398202 = -115897348;    float elWYQoCFobxmREDpOGDSIni68881817 = -265986254;    float elWYQoCFobxmREDpOGDSIni49596410 = -438531661;    float elWYQoCFobxmREDpOGDSIni26733151 = -943512069;    float elWYQoCFobxmREDpOGDSIni15844303 = 25816626;    float elWYQoCFobxmREDpOGDSIni71046278 = -315729623;    float elWYQoCFobxmREDpOGDSIni95890353 = -547781619;    float elWYQoCFobxmREDpOGDSIni33967881 = -256229167;    float elWYQoCFobxmREDpOGDSIni62573157 = -61273890;    float elWYQoCFobxmREDpOGDSIni6469885 = -738636577;    float elWYQoCFobxmREDpOGDSIni7947033 = -128207483;    float elWYQoCFobxmREDpOGDSIni90568745 = -460869166;    float elWYQoCFobxmREDpOGDSIni74880268 = -268009242;    float elWYQoCFobxmREDpOGDSIni67285636 = -201714163;    float elWYQoCFobxmREDpOGDSIni94336594 = -888934450;    float elWYQoCFobxmREDpOGDSIni85643606 = -248823278;    float elWYQoCFobxmREDpOGDSIni41389429 = -883662781;    float elWYQoCFobxmREDpOGDSIni48160757 = -854253359;    float elWYQoCFobxmREDpOGDSIni402061 = -462095205;    float elWYQoCFobxmREDpOGDSIni25054718 = -751932111;    float elWYQoCFobxmREDpOGDSIni97272031 = 62442876;    float elWYQoCFobxmREDpOGDSIni85017577 = -471792657;    float elWYQoCFobxmREDpOGDSIni99840495 = -993937121;    float elWYQoCFobxmREDpOGDSIni23126389 = -996819061;    float elWYQoCFobxmREDpOGDSIni34590515 = -237310905;    float elWYQoCFobxmREDpOGDSIni12867164 = -797065649;     elWYQoCFobxmREDpOGDSIni94336605 = elWYQoCFobxmREDpOGDSIni50304873;     elWYQoCFobxmREDpOGDSIni50304873 = elWYQoCFobxmREDpOGDSIni52868996;     elWYQoCFobxmREDpOGDSIni52868996 = elWYQoCFobxmREDpOGDSIni32356591;     elWYQoCFobxmREDpOGDSIni32356591 = elWYQoCFobxmREDpOGDSIni23548865;     elWYQoCFobxmREDpOGDSIni23548865 = elWYQoCFobxmREDpOGDSIni33101529;     elWYQoCFobxmREDpOGDSIni33101529 = elWYQoCFobxmREDpOGDSIni56666674;     elWYQoCFobxmREDpOGDSIni56666674 = elWYQoCFobxmREDpOGDSIni96608904;     elWYQoCFobxmREDpOGDSIni96608904 = elWYQoCFobxmREDpOGDSIni31618925;     elWYQoCFobxmREDpOGDSIni31618925 = elWYQoCFobxmREDpOGDSIni46428817;     elWYQoCFobxmREDpOGDSIni46428817 = elWYQoCFobxmREDpOGDSIni57556054;     elWYQoCFobxmREDpOGDSIni57556054 = elWYQoCFobxmREDpOGDSIni39297687;     elWYQoCFobxmREDpOGDSIni39297687 = elWYQoCFobxmREDpOGDSIni57026866;     elWYQoCFobxmREDpOGDSIni57026866 = elWYQoCFobxmREDpOGDSIni99783286;     elWYQoCFobxmREDpOGDSIni99783286 = elWYQoCFobxmREDpOGDSIni13107837;     elWYQoCFobxmREDpOGDSIni13107837 = elWYQoCFobxmREDpOGDSIni1437337;     elWYQoCFobxmREDpOGDSIni1437337 = elWYQoCFobxmREDpOGDSIni39867990;     elWYQoCFobxmREDpOGDSIni39867990 = elWYQoCFobxmREDpOGDSIni56393244;     elWYQoCFobxmREDpOGDSIni56393244 = elWYQoCFobxmREDpOGDSIni49544286;     elWYQoCFobxmREDpOGDSIni49544286 = elWYQoCFobxmREDpOGDSIni96879755;     elWYQoCFobxmREDpOGDSIni96879755 = elWYQoCFobxmREDpOGDSIni55003264;     elWYQoCFobxmREDpOGDSIni55003264 = elWYQoCFobxmREDpOGDSIni43870814;     elWYQoCFobxmREDpOGDSIni43870814 = elWYQoCFobxmREDpOGDSIni66911729;     elWYQoCFobxmREDpOGDSIni66911729 = elWYQoCFobxmREDpOGDSIni23578628;     elWYQoCFobxmREDpOGDSIni23578628 = elWYQoCFobxmREDpOGDSIni90920160;     elWYQoCFobxmREDpOGDSIni90920160 = elWYQoCFobxmREDpOGDSIni86559951;     elWYQoCFobxmREDpOGDSIni86559951 = elWYQoCFobxmREDpOGDSIni47016003;     elWYQoCFobxmREDpOGDSIni47016003 = elWYQoCFobxmREDpOGDSIni31202699;     elWYQoCFobxmREDpOGDSIni31202699 = elWYQoCFobxmREDpOGDSIni10327953;     elWYQoCFobxmREDpOGDSIni10327953 = elWYQoCFobxmREDpOGDSIni60470910;     elWYQoCFobxmREDpOGDSIni60470910 = elWYQoCFobxmREDpOGDSIni40840753;     elWYQoCFobxmREDpOGDSIni40840753 = elWYQoCFobxmREDpOGDSIni62688893;     elWYQoCFobxmREDpOGDSIni62688893 = elWYQoCFobxmREDpOGDSIni37872381;     elWYQoCFobxmREDpOGDSIni37872381 = elWYQoCFobxmREDpOGDSIni60239684;     elWYQoCFobxmREDpOGDSIni60239684 = elWYQoCFobxmREDpOGDSIni36109398;     elWYQoCFobxmREDpOGDSIni36109398 = elWYQoCFobxmREDpOGDSIni24782573;     elWYQoCFobxmREDpOGDSIni24782573 = elWYQoCFobxmREDpOGDSIni77451125;     elWYQoCFobxmREDpOGDSIni77451125 = elWYQoCFobxmREDpOGDSIni41838491;     elWYQoCFobxmREDpOGDSIni41838491 = elWYQoCFobxmREDpOGDSIni62043631;     elWYQoCFobxmREDpOGDSIni62043631 = elWYQoCFobxmREDpOGDSIni83357558;     elWYQoCFobxmREDpOGDSIni83357558 = elWYQoCFobxmREDpOGDSIni10481553;     elWYQoCFobxmREDpOGDSIni10481553 = elWYQoCFobxmREDpOGDSIni86121446;     elWYQoCFobxmREDpOGDSIni86121446 = elWYQoCFobxmREDpOGDSIni94274404;     elWYQoCFobxmREDpOGDSIni94274404 = elWYQoCFobxmREDpOGDSIni40178579;     elWYQoCFobxmREDpOGDSIni40178579 = elWYQoCFobxmREDpOGDSIni7734326;     elWYQoCFobxmREDpOGDSIni7734326 = elWYQoCFobxmREDpOGDSIni19873883;     elWYQoCFobxmREDpOGDSIni19873883 = elWYQoCFobxmREDpOGDSIni90669597;     elWYQoCFobxmREDpOGDSIni90669597 = elWYQoCFobxmREDpOGDSIni13048123;     elWYQoCFobxmREDpOGDSIni13048123 = elWYQoCFobxmREDpOGDSIni68629542;     elWYQoCFobxmREDpOGDSIni68629542 = elWYQoCFobxmREDpOGDSIni3858068;     elWYQoCFobxmREDpOGDSIni3858068 = elWYQoCFobxmREDpOGDSIni52523877;     elWYQoCFobxmREDpOGDSIni52523877 = elWYQoCFobxmREDpOGDSIni50272008;     elWYQoCFobxmREDpOGDSIni50272008 = elWYQoCFobxmREDpOGDSIni87808625;     elWYQoCFobxmREDpOGDSIni87808625 = elWYQoCFobxmREDpOGDSIni70586745;     elWYQoCFobxmREDpOGDSIni70586745 = elWYQoCFobxmREDpOGDSIni65903089;     elWYQoCFobxmREDpOGDSIni65903089 = elWYQoCFobxmREDpOGDSIni50465791;     elWYQoCFobxmREDpOGDSIni50465791 = elWYQoCFobxmREDpOGDSIni83393143;     elWYQoCFobxmREDpOGDSIni83393143 = elWYQoCFobxmREDpOGDSIni29290368;     elWYQoCFobxmREDpOGDSIni29290368 = elWYQoCFobxmREDpOGDSIni41436430;     elWYQoCFobxmREDpOGDSIni41436430 = elWYQoCFobxmREDpOGDSIni36988914;     elWYQoCFobxmREDpOGDSIni36988914 = elWYQoCFobxmREDpOGDSIni86085526;     elWYQoCFobxmREDpOGDSIni86085526 = elWYQoCFobxmREDpOGDSIni25463975;     elWYQoCFobxmREDpOGDSIni25463975 = elWYQoCFobxmREDpOGDSIni86280951;     elWYQoCFobxmREDpOGDSIni86280951 = elWYQoCFobxmREDpOGDSIni71148015;     elWYQoCFobxmREDpOGDSIni71148015 = elWYQoCFobxmREDpOGDSIni5588064;     elWYQoCFobxmREDpOGDSIni5588064 = elWYQoCFobxmREDpOGDSIni94867161;     elWYQoCFobxmREDpOGDSIni94867161 = elWYQoCFobxmREDpOGDSIni1425306;     elWYQoCFobxmREDpOGDSIni1425306 = elWYQoCFobxmREDpOGDSIni96787182;     elWYQoCFobxmREDpOGDSIni96787182 = elWYQoCFobxmREDpOGDSIni63673888;     elWYQoCFobxmREDpOGDSIni63673888 = elWYQoCFobxmREDpOGDSIni88325264;     elWYQoCFobxmREDpOGDSIni88325264 = elWYQoCFobxmREDpOGDSIni23986212;     elWYQoCFobxmREDpOGDSIni23986212 = elWYQoCFobxmREDpOGDSIni98029499;     elWYQoCFobxmREDpOGDSIni98029499 = elWYQoCFobxmREDpOGDSIni94349612;     elWYQoCFobxmREDpOGDSIni94349612 = elWYQoCFobxmREDpOGDSIni66186728;     elWYQoCFobxmREDpOGDSIni66186728 = elWYQoCFobxmREDpOGDSIni86398202;     elWYQoCFobxmREDpOGDSIni86398202 = elWYQoCFobxmREDpOGDSIni68881817;     elWYQoCFobxmREDpOGDSIni68881817 = elWYQoCFobxmREDpOGDSIni49596410;     elWYQoCFobxmREDpOGDSIni49596410 = elWYQoCFobxmREDpOGDSIni26733151;     elWYQoCFobxmREDpOGDSIni26733151 = elWYQoCFobxmREDpOGDSIni15844303;     elWYQoCFobxmREDpOGDSIni15844303 = elWYQoCFobxmREDpOGDSIni71046278;     elWYQoCFobxmREDpOGDSIni71046278 = elWYQoCFobxmREDpOGDSIni95890353;     elWYQoCFobxmREDpOGDSIni95890353 = elWYQoCFobxmREDpOGDSIni33967881;     elWYQoCFobxmREDpOGDSIni33967881 = elWYQoCFobxmREDpOGDSIni62573157;     elWYQoCFobxmREDpOGDSIni62573157 = elWYQoCFobxmREDpOGDSIni6469885;     elWYQoCFobxmREDpOGDSIni6469885 = elWYQoCFobxmREDpOGDSIni7947033;     elWYQoCFobxmREDpOGDSIni7947033 = elWYQoCFobxmREDpOGDSIni90568745;     elWYQoCFobxmREDpOGDSIni90568745 = elWYQoCFobxmREDpOGDSIni74880268;     elWYQoCFobxmREDpOGDSIni74880268 = elWYQoCFobxmREDpOGDSIni67285636;     elWYQoCFobxmREDpOGDSIni67285636 = elWYQoCFobxmREDpOGDSIni94336594;     elWYQoCFobxmREDpOGDSIni94336594 = elWYQoCFobxmREDpOGDSIni85643606;     elWYQoCFobxmREDpOGDSIni85643606 = elWYQoCFobxmREDpOGDSIni41389429;     elWYQoCFobxmREDpOGDSIni41389429 = elWYQoCFobxmREDpOGDSIni48160757;     elWYQoCFobxmREDpOGDSIni48160757 = elWYQoCFobxmREDpOGDSIni402061;     elWYQoCFobxmREDpOGDSIni402061 = elWYQoCFobxmREDpOGDSIni25054718;     elWYQoCFobxmREDpOGDSIni25054718 = elWYQoCFobxmREDpOGDSIni97272031;     elWYQoCFobxmREDpOGDSIni97272031 = elWYQoCFobxmREDpOGDSIni85017577;     elWYQoCFobxmREDpOGDSIni85017577 = elWYQoCFobxmREDpOGDSIni99840495;     elWYQoCFobxmREDpOGDSIni99840495 = elWYQoCFobxmREDpOGDSIni23126389;     elWYQoCFobxmREDpOGDSIni23126389 = elWYQoCFobxmREDpOGDSIni34590515;     elWYQoCFobxmREDpOGDSIni34590515 = elWYQoCFobxmREDpOGDSIni12867164;     elWYQoCFobxmREDpOGDSIni12867164 = elWYQoCFobxmREDpOGDSIni94336605;}



void DJtlsxDSvHdFPYQrBxuJxexcTe70088689() {     float bKLpFkZGBPFWJidPNsNCbPZ8590336 = -977087540;    float bKLpFkZGBPFWJidPNsNCbPZ29522184 = -335716068;    float bKLpFkZGBPFWJidPNsNCbPZ99946423 = -387844657;    float bKLpFkZGBPFWJidPNsNCbPZ7127361 = -535095816;    float bKLpFkZGBPFWJidPNsNCbPZ4873711 = -560174186;    float bKLpFkZGBPFWJidPNsNCbPZ27550331 = -538255949;    float bKLpFkZGBPFWJidPNsNCbPZ90852192 = -714195678;    float bKLpFkZGBPFWJidPNsNCbPZ42002267 = -664850085;    float bKLpFkZGBPFWJidPNsNCbPZ91836697 = -246628871;    float bKLpFkZGBPFWJidPNsNCbPZ13765561 = -884422695;    float bKLpFkZGBPFWJidPNsNCbPZ51301978 = -937599782;    float bKLpFkZGBPFWJidPNsNCbPZ29587763 = -790446243;    float bKLpFkZGBPFWJidPNsNCbPZ75672287 = -982870;    float bKLpFkZGBPFWJidPNsNCbPZ74201445 = -669393727;    float bKLpFkZGBPFWJidPNsNCbPZ18053662 = -166207564;    float bKLpFkZGBPFWJidPNsNCbPZ676854 = -245840525;    float bKLpFkZGBPFWJidPNsNCbPZ47734478 = -800209826;    float bKLpFkZGBPFWJidPNsNCbPZ27157895 = -906443030;    float bKLpFkZGBPFWJidPNsNCbPZ70992955 = -420729238;    float bKLpFkZGBPFWJidPNsNCbPZ58394539 = -436550072;    float bKLpFkZGBPFWJidPNsNCbPZ29804058 = -226014962;    float bKLpFkZGBPFWJidPNsNCbPZ74464869 = -978313675;    float bKLpFkZGBPFWJidPNsNCbPZ77134188 = -544603107;    float bKLpFkZGBPFWJidPNsNCbPZ67912181 = -564585691;    float bKLpFkZGBPFWJidPNsNCbPZ98429436 = -649409930;    float bKLpFkZGBPFWJidPNsNCbPZ63928767 = -350143296;    float bKLpFkZGBPFWJidPNsNCbPZ77235206 = -780022591;    float bKLpFkZGBPFWJidPNsNCbPZ31861106 = -562706116;    float bKLpFkZGBPFWJidPNsNCbPZ93670749 = -638740669;    float bKLpFkZGBPFWJidPNsNCbPZ5334851 = -384838649;    float bKLpFkZGBPFWJidPNsNCbPZ754261 = -141090689;    float bKLpFkZGBPFWJidPNsNCbPZ44083835 = -241633414;    float bKLpFkZGBPFWJidPNsNCbPZ11884085 = -971923381;    float bKLpFkZGBPFWJidPNsNCbPZ37278600 = -760915348;    float bKLpFkZGBPFWJidPNsNCbPZ34371897 = -157841579;    float bKLpFkZGBPFWJidPNsNCbPZ69284916 = -947681990;    float bKLpFkZGBPFWJidPNsNCbPZ86816606 = -712848453;    float bKLpFkZGBPFWJidPNsNCbPZ18217228 = 84207595;    float bKLpFkZGBPFWJidPNsNCbPZ14174425 = 98263903;    float bKLpFkZGBPFWJidPNsNCbPZ94233160 = 10505499;    float bKLpFkZGBPFWJidPNsNCbPZ30730933 = -606129956;    float bKLpFkZGBPFWJidPNsNCbPZ26456 = -929735384;    float bKLpFkZGBPFWJidPNsNCbPZ46012149 = -480316991;    float bKLpFkZGBPFWJidPNsNCbPZ43796534 = -48643642;    float bKLpFkZGBPFWJidPNsNCbPZ26697678 = 40310509;    float bKLpFkZGBPFWJidPNsNCbPZ19786630 = 76312369;    float bKLpFkZGBPFWJidPNsNCbPZ98783135 = -296411126;    float bKLpFkZGBPFWJidPNsNCbPZ95480034 = -125893928;    float bKLpFkZGBPFWJidPNsNCbPZ91032762 = 71731853;    float bKLpFkZGBPFWJidPNsNCbPZ53844411 = -171061072;    float bKLpFkZGBPFWJidPNsNCbPZ71493982 = -410651865;    float bKLpFkZGBPFWJidPNsNCbPZ38965682 = -524912651;    float bKLpFkZGBPFWJidPNsNCbPZ7227969 = -924657434;    float bKLpFkZGBPFWJidPNsNCbPZ96151890 = -124812354;    float bKLpFkZGBPFWJidPNsNCbPZ40284631 = -459710153;    float bKLpFkZGBPFWJidPNsNCbPZ34125467 = -998773865;    float bKLpFkZGBPFWJidPNsNCbPZ52387996 = -791112961;    float bKLpFkZGBPFWJidPNsNCbPZ32034243 = -823258966;    float bKLpFkZGBPFWJidPNsNCbPZ8697924 = -885685887;    float bKLpFkZGBPFWJidPNsNCbPZ40944944 = -110030890;    float bKLpFkZGBPFWJidPNsNCbPZ50315124 = -758233358;    float bKLpFkZGBPFWJidPNsNCbPZ58991087 = -51489562;    float bKLpFkZGBPFWJidPNsNCbPZ48331517 = 73890584;    float bKLpFkZGBPFWJidPNsNCbPZ86501846 = -861790222;    float bKLpFkZGBPFWJidPNsNCbPZ13011300 = -643332007;    float bKLpFkZGBPFWJidPNsNCbPZ7218143 = -595966368;    float bKLpFkZGBPFWJidPNsNCbPZ17703679 = -818522863;    float bKLpFkZGBPFWJidPNsNCbPZ38393687 = -240067522;    float bKLpFkZGBPFWJidPNsNCbPZ39829549 = -411552148;    float bKLpFkZGBPFWJidPNsNCbPZ48768745 = -218525575;    float bKLpFkZGBPFWJidPNsNCbPZ13860247 = -532992072;    float bKLpFkZGBPFWJidPNsNCbPZ29517251 = -784417422;    float bKLpFkZGBPFWJidPNsNCbPZ12983471 = -904706933;    float bKLpFkZGBPFWJidPNsNCbPZ76759794 = -331234737;    float bKLpFkZGBPFWJidPNsNCbPZ27663606 = -830420116;    float bKLpFkZGBPFWJidPNsNCbPZ29777603 = -296279578;    float bKLpFkZGBPFWJidPNsNCbPZ28452720 = -397996684;    float bKLpFkZGBPFWJidPNsNCbPZ33337654 = -395959465;    float bKLpFkZGBPFWJidPNsNCbPZ41214504 = -504896200;    float bKLpFkZGBPFWJidPNsNCbPZ78642807 = -625722300;    float bKLpFkZGBPFWJidPNsNCbPZ65145631 = 46267829;    float bKLpFkZGBPFWJidPNsNCbPZ81755172 = -554128664;    float bKLpFkZGBPFWJidPNsNCbPZ40828344 = -534437969;    float bKLpFkZGBPFWJidPNsNCbPZ39826338 = -367679598;    float bKLpFkZGBPFWJidPNsNCbPZ33840868 = -974186784;    float bKLpFkZGBPFWJidPNsNCbPZ61788579 = -616178038;    float bKLpFkZGBPFWJidPNsNCbPZ36855867 = -316975980;    float bKLpFkZGBPFWJidPNsNCbPZ15732195 = -747111027;    float bKLpFkZGBPFWJidPNsNCbPZ96993969 = -201205195;    float bKLpFkZGBPFWJidPNsNCbPZ246430 = -159067714;    float bKLpFkZGBPFWJidPNsNCbPZ16896921 = -56569029;    float bKLpFkZGBPFWJidPNsNCbPZ54782364 = -889589487;    float bKLpFkZGBPFWJidPNsNCbPZ9519305 = -30106518;    float bKLpFkZGBPFWJidPNsNCbPZ73229481 = -791705207;    float bKLpFkZGBPFWJidPNsNCbPZ43918037 = -231261144;    float bKLpFkZGBPFWJidPNsNCbPZ71739846 = -454640394;    float bKLpFkZGBPFWJidPNsNCbPZ51694938 = -903625968;    float bKLpFkZGBPFWJidPNsNCbPZ59510303 = -618526769;    float bKLpFkZGBPFWJidPNsNCbPZ30785234 = -405311636;    float bKLpFkZGBPFWJidPNsNCbPZ19479535 = -977087540;     bKLpFkZGBPFWJidPNsNCbPZ8590336 = bKLpFkZGBPFWJidPNsNCbPZ29522184;     bKLpFkZGBPFWJidPNsNCbPZ29522184 = bKLpFkZGBPFWJidPNsNCbPZ99946423;     bKLpFkZGBPFWJidPNsNCbPZ99946423 = bKLpFkZGBPFWJidPNsNCbPZ7127361;     bKLpFkZGBPFWJidPNsNCbPZ7127361 = bKLpFkZGBPFWJidPNsNCbPZ4873711;     bKLpFkZGBPFWJidPNsNCbPZ4873711 = bKLpFkZGBPFWJidPNsNCbPZ27550331;     bKLpFkZGBPFWJidPNsNCbPZ27550331 = bKLpFkZGBPFWJidPNsNCbPZ90852192;     bKLpFkZGBPFWJidPNsNCbPZ90852192 = bKLpFkZGBPFWJidPNsNCbPZ42002267;     bKLpFkZGBPFWJidPNsNCbPZ42002267 = bKLpFkZGBPFWJidPNsNCbPZ91836697;     bKLpFkZGBPFWJidPNsNCbPZ91836697 = bKLpFkZGBPFWJidPNsNCbPZ13765561;     bKLpFkZGBPFWJidPNsNCbPZ13765561 = bKLpFkZGBPFWJidPNsNCbPZ51301978;     bKLpFkZGBPFWJidPNsNCbPZ51301978 = bKLpFkZGBPFWJidPNsNCbPZ29587763;     bKLpFkZGBPFWJidPNsNCbPZ29587763 = bKLpFkZGBPFWJidPNsNCbPZ75672287;     bKLpFkZGBPFWJidPNsNCbPZ75672287 = bKLpFkZGBPFWJidPNsNCbPZ74201445;     bKLpFkZGBPFWJidPNsNCbPZ74201445 = bKLpFkZGBPFWJidPNsNCbPZ18053662;     bKLpFkZGBPFWJidPNsNCbPZ18053662 = bKLpFkZGBPFWJidPNsNCbPZ676854;     bKLpFkZGBPFWJidPNsNCbPZ676854 = bKLpFkZGBPFWJidPNsNCbPZ47734478;     bKLpFkZGBPFWJidPNsNCbPZ47734478 = bKLpFkZGBPFWJidPNsNCbPZ27157895;     bKLpFkZGBPFWJidPNsNCbPZ27157895 = bKLpFkZGBPFWJidPNsNCbPZ70992955;     bKLpFkZGBPFWJidPNsNCbPZ70992955 = bKLpFkZGBPFWJidPNsNCbPZ58394539;     bKLpFkZGBPFWJidPNsNCbPZ58394539 = bKLpFkZGBPFWJidPNsNCbPZ29804058;     bKLpFkZGBPFWJidPNsNCbPZ29804058 = bKLpFkZGBPFWJidPNsNCbPZ74464869;     bKLpFkZGBPFWJidPNsNCbPZ74464869 = bKLpFkZGBPFWJidPNsNCbPZ77134188;     bKLpFkZGBPFWJidPNsNCbPZ77134188 = bKLpFkZGBPFWJidPNsNCbPZ67912181;     bKLpFkZGBPFWJidPNsNCbPZ67912181 = bKLpFkZGBPFWJidPNsNCbPZ98429436;     bKLpFkZGBPFWJidPNsNCbPZ98429436 = bKLpFkZGBPFWJidPNsNCbPZ63928767;     bKLpFkZGBPFWJidPNsNCbPZ63928767 = bKLpFkZGBPFWJidPNsNCbPZ77235206;     bKLpFkZGBPFWJidPNsNCbPZ77235206 = bKLpFkZGBPFWJidPNsNCbPZ31861106;     bKLpFkZGBPFWJidPNsNCbPZ31861106 = bKLpFkZGBPFWJidPNsNCbPZ93670749;     bKLpFkZGBPFWJidPNsNCbPZ93670749 = bKLpFkZGBPFWJidPNsNCbPZ5334851;     bKLpFkZGBPFWJidPNsNCbPZ5334851 = bKLpFkZGBPFWJidPNsNCbPZ754261;     bKLpFkZGBPFWJidPNsNCbPZ754261 = bKLpFkZGBPFWJidPNsNCbPZ44083835;     bKLpFkZGBPFWJidPNsNCbPZ44083835 = bKLpFkZGBPFWJidPNsNCbPZ11884085;     bKLpFkZGBPFWJidPNsNCbPZ11884085 = bKLpFkZGBPFWJidPNsNCbPZ37278600;     bKLpFkZGBPFWJidPNsNCbPZ37278600 = bKLpFkZGBPFWJidPNsNCbPZ34371897;     bKLpFkZGBPFWJidPNsNCbPZ34371897 = bKLpFkZGBPFWJidPNsNCbPZ69284916;     bKLpFkZGBPFWJidPNsNCbPZ69284916 = bKLpFkZGBPFWJidPNsNCbPZ86816606;     bKLpFkZGBPFWJidPNsNCbPZ86816606 = bKLpFkZGBPFWJidPNsNCbPZ18217228;     bKLpFkZGBPFWJidPNsNCbPZ18217228 = bKLpFkZGBPFWJidPNsNCbPZ14174425;     bKLpFkZGBPFWJidPNsNCbPZ14174425 = bKLpFkZGBPFWJidPNsNCbPZ94233160;     bKLpFkZGBPFWJidPNsNCbPZ94233160 = bKLpFkZGBPFWJidPNsNCbPZ30730933;     bKLpFkZGBPFWJidPNsNCbPZ30730933 = bKLpFkZGBPFWJidPNsNCbPZ26456;     bKLpFkZGBPFWJidPNsNCbPZ26456 = bKLpFkZGBPFWJidPNsNCbPZ46012149;     bKLpFkZGBPFWJidPNsNCbPZ46012149 = bKLpFkZGBPFWJidPNsNCbPZ43796534;     bKLpFkZGBPFWJidPNsNCbPZ43796534 = bKLpFkZGBPFWJidPNsNCbPZ26697678;     bKLpFkZGBPFWJidPNsNCbPZ26697678 = bKLpFkZGBPFWJidPNsNCbPZ19786630;     bKLpFkZGBPFWJidPNsNCbPZ19786630 = bKLpFkZGBPFWJidPNsNCbPZ98783135;     bKLpFkZGBPFWJidPNsNCbPZ98783135 = bKLpFkZGBPFWJidPNsNCbPZ95480034;     bKLpFkZGBPFWJidPNsNCbPZ95480034 = bKLpFkZGBPFWJidPNsNCbPZ91032762;     bKLpFkZGBPFWJidPNsNCbPZ91032762 = bKLpFkZGBPFWJidPNsNCbPZ53844411;     bKLpFkZGBPFWJidPNsNCbPZ53844411 = bKLpFkZGBPFWJidPNsNCbPZ71493982;     bKLpFkZGBPFWJidPNsNCbPZ71493982 = bKLpFkZGBPFWJidPNsNCbPZ38965682;     bKLpFkZGBPFWJidPNsNCbPZ38965682 = bKLpFkZGBPFWJidPNsNCbPZ7227969;     bKLpFkZGBPFWJidPNsNCbPZ7227969 = bKLpFkZGBPFWJidPNsNCbPZ96151890;     bKLpFkZGBPFWJidPNsNCbPZ96151890 = bKLpFkZGBPFWJidPNsNCbPZ40284631;     bKLpFkZGBPFWJidPNsNCbPZ40284631 = bKLpFkZGBPFWJidPNsNCbPZ34125467;     bKLpFkZGBPFWJidPNsNCbPZ34125467 = bKLpFkZGBPFWJidPNsNCbPZ52387996;     bKLpFkZGBPFWJidPNsNCbPZ52387996 = bKLpFkZGBPFWJidPNsNCbPZ32034243;     bKLpFkZGBPFWJidPNsNCbPZ32034243 = bKLpFkZGBPFWJidPNsNCbPZ8697924;     bKLpFkZGBPFWJidPNsNCbPZ8697924 = bKLpFkZGBPFWJidPNsNCbPZ40944944;     bKLpFkZGBPFWJidPNsNCbPZ40944944 = bKLpFkZGBPFWJidPNsNCbPZ50315124;     bKLpFkZGBPFWJidPNsNCbPZ50315124 = bKLpFkZGBPFWJidPNsNCbPZ58991087;     bKLpFkZGBPFWJidPNsNCbPZ58991087 = bKLpFkZGBPFWJidPNsNCbPZ48331517;     bKLpFkZGBPFWJidPNsNCbPZ48331517 = bKLpFkZGBPFWJidPNsNCbPZ86501846;     bKLpFkZGBPFWJidPNsNCbPZ86501846 = bKLpFkZGBPFWJidPNsNCbPZ13011300;     bKLpFkZGBPFWJidPNsNCbPZ13011300 = bKLpFkZGBPFWJidPNsNCbPZ7218143;     bKLpFkZGBPFWJidPNsNCbPZ7218143 = bKLpFkZGBPFWJidPNsNCbPZ17703679;     bKLpFkZGBPFWJidPNsNCbPZ17703679 = bKLpFkZGBPFWJidPNsNCbPZ38393687;     bKLpFkZGBPFWJidPNsNCbPZ38393687 = bKLpFkZGBPFWJidPNsNCbPZ39829549;     bKLpFkZGBPFWJidPNsNCbPZ39829549 = bKLpFkZGBPFWJidPNsNCbPZ48768745;     bKLpFkZGBPFWJidPNsNCbPZ48768745 = bKLpFkZGBPFWJidPNsNCbPZ13860247;     bKLpFkZGBPFWJidPNsNCbPZ13860247 = bKLpFkZGBPFWJidPNsNCbPZ29517251;     bKLpFkZGBPFWJidPNsNCbPZ29517251 = bKLpFkZGBPFWJidPNsNCbPZ12983471;     bKLpFkZGBPFWJidPNsNCbPZ12983471 = bKLpFkZGBPFWJidPNsNCbPZ76759794;     bKLpFkZGBPFWJidPNsNCbPZ76759794 = bKLpFkZGBPFWJidPNsNCbPZ27663606;     bKLpFkZGBPFWJidPNsNCbPZ27663606 = bKLpFkZGBPFWJidPNsNCbPZ29777603;     bKLpFkZGBPFWJidPNsNCbPZ29777603 = bKLpFkZGBPFWJidPNsNCbPZ28452720;     bKLpFkZGBPFWJidPNsNCbPZ28452720 = bKLpFkZGBPFWJidPNsNCbPZ33337654;     bKLpFkZGBPFWJidPNsNCbPZ33337654 = bKLpFkZGBPFWJidPNsNCbPZ41214504;     bKLpFkZGBPFWJidPNsNCbPZ41214504 = bKLpFkZGBPFWJidPNsNCbPZ78642807;     bKLpFkZGBPFWJidPNsNCbPZ78642807 = bKLpFkZGBPFWJidPNsNCbPZ65145631;     bKLpFkZGBPFWJidPNsNCbPZ65145631 = bKLpFkZGBPFWJidPNsNCbPZ81755172;     bKLpFkZGBPFWJidPNsNCbPZ81755172 = bKLpFkZGBPFWJidPNsNCbPZ40828344;     bKLpFkZGBPFWJidPNsNCbPZ40828344 = bKLpFkZGBPFWJidPNsNCbPZ39826338;     bKLpFkZGBPFWJidPNsNCbPZ39826338 = bKLpFkZGBPFWJidPNsNCbPZ33840868;     bKLpFkZGBPFWJidPNsNCbPZ33840868 = bKLpFkZGBPFWJidPNsNCbPZ61788579;     bKLpFkZGBPFWJidPNsNCbPZ61788579 = bKLpFkZGBPFWJidPNsNCbPZ36855867;     bKLpFkZGBPFWJidPNsNCbPZ36855867 = bKLpFkZGBPFWJidPNsNCbPZ15732195;     bKLpFkZGBPFWJidPNsNCbPZ15732195 = bKLpFkZGBPFWJidPNsNCbPZ96993969;     bKLpFkZGBPFWJidPNsNCbPZ96993969 = bKLpFkZGBPFWJidPNsNCbPZ246430;     bKLpFkZGBPFWJidPNsNCbPZ246430 = bKLpFkZGBPFWJidPNsNCbPZ16896921;     bKLpFkZGBPFWJidPNsNCbPZ16896921 = bKLpFkZGBPFWJidPNsNCbPZ54782364;     bKLpFkZGBPFWJidPNsNCbPZ54782364 = bKLpFkZGBPFWJidPNsNCbPZ9519305;     bKLpFkZGBPFWJidPNsNCbPZ9519305 = bKLpFkZGBPFWJidPNsNCbPZ73229481;     bKLpFkZGBPFWJidPNsNCbPZ73229481 = bKLpFkZGBPFWJidPNsNCbPZ43918037;     bKLpFkZGBPFWJidPNsNCbPZ43918037 = bKLpFkZGBPFWJidPNsNCbPZ71739846;     bKLpFkZGBPFWJidPNsNCbPZ71739846 = bKLpFkZGBPFWJidPNsNCbPZ51694938;     bKLpFkZGBPFWJidPNsNCbPZ51694938 = bKLpFkZGBPFWJidPNsNCbPZ59510303;     bKLpFkZGBPFWJidPNsNCbPZ59510303 = bKLpFkZGBPFWJidPNsNCbPZ30785234;     bKLpFkZGBPFWJidPNsNCbPZ30785234 = bKLpFkZGBPFWJidPNsNCbPZ19479535;     bKLpFkZGBPFWJidPNsNCbPZ19479535 = bKLpFkZGBPFWJidPNsNCbPZ8590336;}



void kHZUYzXbVEnLCysRPSuTqYMYPp55151527() {     float XUcYAprRyQydvdiaKopoOTa92313853 = -227109607;    float XUcYAprRyQydvdiaKopoOTa27120279 = -139367919;    float XUcYAprRyQydvdiaKopoOTa88532541 = -256152347;    float XUcYAprRyQydvdiaKopoOTa4275314 = -539977745;    float XUcYAprRyQydvdiaKopoOTa84435047 = -958122377;    float XUcYAprRyQydvdiaKopoOTa38889849 = -641576617;    float XUcYAprRyQydvdiaKopoOTa27732756 = -735709816;    float XUcYAprRyQydvdiaKopoOTa85342349 = -185304671;    float XUcYAprRyQydvdiaKopoOTa42056226 = -944111567;    float XUcYAprRyQydvdiaKopoOTa59871148 = -7463711;    float XUcYAprRyQydvdiaKopoOTa94191013 = -777885039;    float XUcYAprRyQydvdiaKopoOTa19799534 = -719585514;    float XUcYAprRyQydvdiaKopoOTa26726139 = -943815114;    float XUcYAprRyQydvdiaKopoOTa68574590 = 80923411;    float XUcYAprRyQydvdiaKopoOTa22232920 = -91904869;    float XUcYAprRyQydvdiaKopoOTa72490881 = -169968177;    float XUcYAprRyQydvdiaKopoOTa47599889 = -959089689;    float XUcYAprRyQydvdiaKopoOTa41235165 = -517294603;    float XUcYAprRyQydvdiaKopoOTa11969436 = -209378476;    float XUcYAprRyQydvdiaKopoOTa32502183 = -52842989;    float XUcYAprRyQydvdiaKopoOTa97143568 = -900481483;    float XUcYAprRyQydvdiaKopoOTa75466940 = -891468962;    float XUcYAprRyQydvdiaKopoOTa46310053 = -573997843;    float XUcYAprRyQydvdiaKopoOTa69054876 = -103818344;    float XUcYAprRyQydvdiaKopoOTa35031530 = -727740692;    float XUcYAprRyQydvdiaKopoOTa16921525 = -793110324;    float XUcYAprRyQydvdiaKopoOTa98827146 = -123898015;    float XUcYAprRyQydvdiaKopoOTa74460305 = -349147604;    float XUcYAprRyQydvdiaKopoOTa65588891 = -62936570;    float XUcYAprRyQydvdiaKopoOTa40883178 = 42554976;    float XUcYAprRyQydvdiaKopoOTa82118683 = -360188710;    float XUcYAprRyQydvdiaKopoOTa68877123 = -790830637;    float XUcYAprRyQydvdiaKopoOTa26008786 = -537137472;    float XUcYAprRyQydvdiaKopoOTa73003314 = -65011996;    float XUcYAprRyQydvdiaKopoOTa1975223 = -992132187;    float XUcYAprRyQydvdiaKopoOTa39146150 = -926078467;    float XUcYAprRyQydvdiaKopoOTa86580197 = 66810250;    float XUcYAprRyQydvdiaKopoOTa68599018 = -914935621;    float XUcYAprRyQydvdiaKopoOTa74790143 = -207294978;    float XUcYAprRyQydvdiaKopoOTa33422277 = 63858619;    float XUcYAprRyQydvdiaKopoOTa95498453 = -256170082;    float XUcYAprRyQydvdiaKopoOTa29366182 = -535761053;    float XUcYAprRyQydvdiaKopoOTa78812295 = -132721246;    float XUcYAprRyQydvdiaKopoOTa90992053 = -727587375;    float XUcYAprRyQydvdiaKopoOTa54684929 = -285703569;    float XUcYAprRyQydvdiaKopoOTa64053511 = -638719322;    float XUcYAprRyQydvdiaKopoOTa31962105 = -318057032;    float XUcYAprRyQydvdiaKopoOTa34221882 = -988015826;    float XUcYAprRyQydvdiaKopoOTa24906976 = -728052298;    float XUcYAprRyQydvdiaKopoOTa89717741 = -803076507;    float XUcYAprRyQydvdiaKopoOTa11584814 = -576424267;    float XUcYAprRyQydvdiaKopoOTa90471401 = -276965438;    float XUcYAprRyQydvdiaKopoOTa52610371 = -936589800;    float XUcYAprRyQydvdiaKopoOTa42084496 = -701521542;    float XUcYAprRyQydvdiaKopoOTa58007958 = -776437098;    float XUcYAprRyQydvdiaKopoOTa16846914 = -335640646;    float XUcYAprRyQydvdiaKopoOTa80810226 = -565370076;    float XUcYAprRyQydvdiaKopoOTa19477665 = -52334004;    float XUcYAprRyQydvdiaKopoOTa69243783 = -812237054;    float XUcYAprRyQydvdiaKopoOTa67513522 = -65012054;    float XUcYAprRyQydvdiaKopoOTa40062703 = -417678603;    float XUcYAprRyQydvdiaKopoOTa53272450 = -286562212;    float XUcYAprRyQydvdiaKopoOTa19753459 = -22368102;    float XUcYAprRyQydvdiaKopoOTa1173048 = -886666544;    float XUcYAprRyQydvdiaKopoOTa77752465 = -647275002;    float XUcYAprRyQydvdiaKopoOTa25313891 = -987054403;    float XUcYAprRyQydvdiaKopoOTa93790747 = -82448042;    float XUcYAprRyQydvdiaKopoOTa53722825 = -778803118;    float XUcYAprRyQydvdiaKopoOTa66599368 = 73055597;    float XUcYAprRyQydvdiaKopoOTa83086770 = -165826403;    float XUcYAprRyQydvdiaKopoOTa85910684 = -136778427;    float XUcYAprRyQydvdiaKopoOTa79000870 = 55845932;    float XUcYAprRyQydvdiaKopoOTa66445021 = -209999626;    float XUcYAprRyQydvdiaKopoOTa78547159 = -173237096;    float XUcYAprRyQydvdiaKopoOTa37003729 = -796672907;    float XUcYAprRyQydvdiaKopoOTa67777386 = -264720430;    float XUcYAprRyQydvdiaKopoOTa96654644 = -658747716;    float XUcYAprRyQydvdiaKopoOTa55318000 = -846410469;    float XUcYAprRyQydvdiaKopoOTa14369948 = -818114775;    float XUcYAprRyQydvdiaKopoOTa70978019 = 10978630;    float XUcYAprRyQydvdiaKopoOTa84959419 = -375053292;    float XUcYAprRyQydvdiaKopoOTa64605264 = -135882189;    float XUcYAprRyQydvdiaKopoOTa49553330 = -621095306;    float XUcYAprRyQydvdiaKopoOTa75871149 = -259860063;    float XUcYAprRyQydvdiaKopoOTa29298364 = -381020757;    float XUcYAprRyQydvdiaKopoOTa91647282 = 16776727;    float XUcYAprRyQydvdiaKopoOTa16266753 = -854240837;    float XUcYAprRyQydvdiaKopoOTa83924289 = -835615931;    float XUcYAprRyQydvdiaKopoOTa14995356 = -288574898;    float XUcYAprRyQydvdiaKopoOTa85128308 = -556491541;    float XUcYAprRyQydvdiaKopoOTa58335924 = -260708392;    float XUcYAprRyQydvdiaKopoOTa67102532 = -880855746;    float XUcYAprRyQydvdiaKopoOTa99355235 = -2698568;    float XUcYAprRyQydvdiaKopoOTa7276622 = -42282925;    float XUcYAprRyQydvdiaKopoOTa93359574 = -518462778;    float XUcYAprRyQydvdiaKopoOTa42226004 = -969607871;    float XUcYAprRyQydvdiaKopoOTa9612724 = -413392952;    float XUcYAprRyQydvdiaKopoOTa77639247 = -246054702;    float XUcYAprRyQydvdiaKopoOTa13239589 = 19687626;    float XUcYAprRyQydvdiaKopoOTa29371038 = -227109607;     XUcYAprRyQydvdiaKopoOTa92313853 = XUcYAprRyQydvdiaKopoOTa27120279;     XUcYAprRyQydvdiaKopoOTa27120279 = XUcYAprRyQydvdiaKopoOTa88532541;     XUcYAprRyQydvdiaKopoOTa88532541 = XUcYAprRyQydvdiaKopoOTa4275314;     XUcYAprRyQydvdiaKopoOTa4275314 = XUcYAprRyQydvdiaKopoOTa84435047;     XUcYAprRyQydvdiaKopoOTa84435047 = XUcYAprRyQydvdiaKopoOTa38889849;     XUcYAprRyQydvdiaKopoOTa38889849 = XUcYAprRyQydvdiaKopoOTa27732756;     XUcYAprRyQydvdiaKopoOTa27732756 = XUcYAprRyQydvdiaKopoOTa85342349;     XUcYAprRyQydvdiaKopoOTa85342349 = XUcYAprRyQydvdiaKopoOTa42056226;     XUcYAprRyQydvdiaKopoOTa42056226 = XUcYAprRyQydvdiaKopoOTa59871148;     XUcYAprRyQydvdiaKopoOTa59871148 = XUcYAprRyQydvdiaKopoOTa94191013;     XUcYAprRyQydvdiaKopoOTa94191013 = XUcYAprRyQydvdiaKopoOTa19799534;     XUcYAprRyQydvdiaKopoOTa19799534 = XUcYAprRyQydvdiaKopoOTa26726139;     XUcYAprRyQydvdiaKopoOTa26726139 = XUcYAprRyQydvdiaKopoOTa68574590;     XUcYAprRyQydvdiaKopoOTa68574590 = XUcYAprRyQydvdiaKopoOTa22232920;     XUcYAprRyQydvdiaKopoOTa22232920 = XUcYAprRyQydvdiaKopoOTa72490881;     XUcYAprRyQydvdiaKopoOTa72490881 = XUcYAprRyQydvdiaKopoOTa47599889;     XUcYAprRyQydvdiaKopoOTa47599889 = XUcYAprRyQydvdiaKopoOTa41235165;     XUcYAprRyQydvdiaKopoOTa41235165 = XUcYAprRyQydvdiaKopoOTa11969436;     XUcYAprRyQydvdiaKopoOTa11969436 = XUcYAprRyQydvdiaKopoOTa32502183;     XUcYAprRyQydvdiaKopoOTa32502183 = XUcYAprRyQydvdiaKopoOTa97143568;     XUcYAprRyQydvdiaKopoOTa97143568 = XUcYAprRyQydvdiaKopoOTa75466940;     XUcYAprRyQydvdiaKopoOTa75466940 = XUcYAprRyQydvdiaKopoOTa46310053;     XUcYAprRyQydvdiaKopoOTa46310053 = XUcYAprRyQydvdiaKopoOTa69054876;     XUcYAprRyQydvdiaKopoOTa69054876 = XUcYAprRyQydvdiaKopoOTa35031530;     XUcYAprRyQydvdiaKopoOTa35031530 = XUcYAprRyQydvdiaKopoOTa16921525;     XUcYAprRyQydvdiaKopoOTa16921525 = XUcYAprRyQydvdiaKopoOTa98827146;     XUcYAprRyQydvdiaKopoOTa98827146 = XUcYAprRyQydvdiaKopoOTa74460305;     XUcYAprRyQydvdiaKopoOTa74460305 = XUcYAprRyQydvdiaKopoOTa65588891;     XUcYAprRyQydvdiaKopoOTa65588891 = XUcYAprRyQydvdiaKopoOTa40883178;     XUcYAprRyQydvdiaKopoOTa40883178 = XUcYAprRyQydvdiaKopoOTa82118683;     XUcYAprRyQydvdiaKopoOTa82118683 = XUcYAprRyQydvdiaKopoOTa68877123;     XUcYAprRyQydvdiaKopoOTa68877123 = XUcYAprRyQydvdiaKopoOTa26008786;     XUcYAprRyQydvdiaKopoOTa26008786 = XUcYAprRyQydvdiaKopoOTa73003314;     XUcYAprRyQydvdiaKopoOTa73003314 = XUcYAprRyQydvdiaKopoOTa1975223;     XUcYAprRyQydvdiaKopoOTa1975223 = XUcYAprRyQydvdiaKopoOTa39146150;     XUcYAprRyQydvdiaKopoOTa39146150 = XUcYAprRyQydvdiaKopoOTa86580197;     XUcYAprRyQydvdiaKopoOTa86580197 = XUcYAprRyQydvdiaKopoOTa68599018;     XUcYAprRyQydvdiaKopoOTa68599018 = XUcYAprRyQydvdiaKopoOTa74790143;     XUcYAprRyQydvdiaKopoOTa74790143 = XUcYAprRyQydvdiaKopoOTa33422277;     XUcYAprRyQydvdiaKopoOTa33422277 = XUcYAprRyQydvdiaKopoOTa95498453;     XUcYAprRyQydvdiaKopoOTa95498453 = XUcYAprRyQydvdiaKopoOTa29366182;     XUcYAprRyQydvdiaKopoOTa29366182 = XUcYAprRyQydvdiaKopoOTa78812295;     XUcYAprRyQydvdiaKopoOTa78812295 = XUcYAprRyQydvdiaKopoOTa90992053;     XUcYAprRyQydvdiaKopoOTa90992053 = XUcYAprRyQydvdiaKopoOTa54684929;     XUcYAprRyQydvdiaKopoOTa54684929 = XUcYAprRyQydvdiaKopoOTa64053511;     XUcYAprRyQydvdiaKopoOTa64053511 = XUcYAprRyQydvdiaKopoOTa31962105;     XUcYAprRyQydvdiaKopoOTa31962105 = XUcYAprRyQydvdiaKopoOTa34221882;     XUcYAprRyQydvdiaKopoOTa34221882 = XUcYAprRyQydvdiaKopoOTa24906976;     XUcYAprRyQydvdiaKopoOTa24906976 = XUcYAprRyQydvdiaKopoOTa89717741;     XUcYAprRyQydvdiaKopoOTa89717741 = XUcYAprRyQydvdiaKopoOTa11584814;     XUcYAprRyQydvdiaKopoOTa11584814 = XUcYAprRyQydvdiaKopoOTa90471401;     XUcYAprRyQydvdiaKopoOTa90471401 = XUcYAprRyQydvdiaKopoOTa52610371;     XUcYAprRyQydvdiaKopoOTa52610371 = XUcYAprRyQydvdiaKopoOTa42084496;     XUcYAprRyQydvdiaKopoOTa42084496 = XUcYAprRyQydvdiaKopoOTa58007958;     XUcYAprRyQydvdiaKopoOTa58007958 = XUcYAprRyQydvdiaKopoOTa16846914;     XUcYAprRyQydvdiaKopoOTa16846914 = XUcYAprRyQydvdiaKopoOTa80810226;     XUcYAprRyQydvdiaKopoOTa80810226 = XUcYAprRyQydvdiaKopoOTa19477665;     XUcYAprRyQydvdiaKopoOTa19477665 = XUcYAprRyQydvdiaKopoOTa69243783;     XUcYAprRyQydvdiaKopoOTa69243783 = XUcYAprRyQydvdiaKopoOTa67513522;     XUcYAprRyQydvdiaKopoOTa67513522 = XUcYAprRyQydvdiaKopoOTa40062703;     XUcYAprRyQydvdiaKopoOTa40062703 = XUcYAprRyQydvdiaKopoOTa53272450;     XUcYAprRyQydvdiaKopoOTa53272450 = XUcYAprRyQydvdiaKopoOTa19753459;     XUcYAprRyQydvdiaKopoOTa19753459 = XUcYAprRyQydvdiaKopoOTa1173048;     XUcYAprRyQydvdiaKopoOTa1173048 = XUcYAprRyQydvdiaKopoOTa77752465;     XUcYAprRyQydvdiaKopoOTa77752465 = XUcYAprRyQydvdiaKopoOTa25313891;     XUcYAprRyQydvdiaKopoOTa25313891 = XUcYAprRyQydvdiaKopoOTa93790747;     XUcYAprRyQydvdiaKopoOTa93790747 = XUcYAprRyQydvdiaKopoOTa53722825;     XUcYAprRyQydvdiaKopoOTa53722825 = XUcYAprRyQydvdiaKopoOTa66599368;     XUcYAprRyQydvdiaKopoOTa66599368 = XUcYAprRyQydvdiaKopoOTa83086770;     XUcYAprRyQydvdiaKopoOTa83086770 = XUcYAprRyQydvdiaKopoOTa85910684;     XUcYAprRyQydvdiaKopoOTa85910684 = XUcYAprRyQydvdiaKopoOTa79000870;     XUcYAprRyQydvdiaKopoOTa79000870 = XUcYAprRyQydvdiaKopoOTa66445021;     XUcYAprRyQydvdiaKopoOTa66445021 = XUcYAprRyQydvdiaKopoOTa78547159;     XUcYAprRyQydvdiaKopoOTa78547159 = XUcYAprRyQydvdiaKopoOTa37003729;     XUcYAprRyQydvdiaKopoOTa37003729 = XUcYAprRyQydvdiaKopoOTa67777386;     XUcYAprRyQydvdiaKopoOTa67777386 = XUcYAprRyQydvdiaKopoOTa96654644;     XUcYAprRyQydvdiaKopoOTa96654644 = XUcYAprRyQydvdiaKopoOTa55318000;     XUcYAprRyQydvdiaKopoOTa55318000 = XUcYAprRyQydvdiaKopoOTa14369948;     XUcYAprRyQydvdiaKopoOTa14369948 = XUcYAprRyQydvdiaKopoOTa70978019;     XUcYAprRyQydvdiaKopoOTa70978019 = XUcYAprRyQydvdiaKopoOTa84959419;     XUcYAprRyQydvdiaKopoOTa84959419 = XUcYAprRyQydvdiaKopoOTa64605264;     XUcYAprRyQydvdiaKopoOTa64605264 = XUcYAprRyQydvdiaKopoOTa49553330;     XUcYAprRyQydvdiaKopoOTa49553330 = XUcYAprRyQydvdiaKopoOTa75871149;     XUcYAprRyQydvdiaKopoOTa75871149 = XUcYAprRyQydvdiaKopoOTa29298364;     XUcYAprRyQydvdiaKopoOTa29298364 = XUcYAprRyQydvdiaKopoOTa91647282;     XUcYAprRyQydvdiaKopoOTa91647282 = XUcYAprRyQydvdiaKopoOTa16266753;     XUcYAprRyQydvdiaKopoOTa16266753 = XUcYAprRyQydvdiaKopoOTa83924289;     XUcYAprRyQydvdiaKopoOTa83924289 = XUcYAprRyQydvdiaKopoOTa14995356;     XUcYAprRyQydvdiaKopoOTa14995356 = XUcYAprRyQydvdiaKopoOTa85128308;     XUcYAprRyQydvdiaKopoOTa85128308 = XUcYAprRyQydvdiaKopoOTa58335924;     XUcYAprRyQydvdiaKopoOTa58335924 = XUcYAprRyQydvdiaKopoOTa67102532;     XUcYAprRyQydvdiaKopoOTa67102532 = XUcYAprRyQydvdiaKopoOTa99355235;     XUcYAprRyQydvdiaKopoOTa99355235 = XUcYAprRyQydvdiaKopoOTa7276622;     XUcYAprRyQydvdiaKopoOTa7276622 = XUcYAprRyQydvdiaKopoOTa93359574;     XUcYAprRyQydvdiaKopoOTa93359574 = XUcYAprRyQydvdiaKopoOTa42226004;     XUcYAprRyQydvdiaKopoOTa42226004 = XUcYAprRyQydvdiaKopoOTa9612724;     XUcYAprRyQydvdiaKopoOTa9612724 = XUcYAprRyQydvdiaKopoOTa77639247;     XUcYAprRyQydvdiaKopoOTa77639247 = XUcYAprRyQydvdiaKopoOTa13239589;     XUcYAprRyQydvdiaKopoOTa13239589 = XUcYAprRyQydvdiaKopoOTa29371038;     XUcYAprRyQydvdiaKopoOTa29371038 = XUcYAprRyQydvdiaKopoOTa92313853;}



void DATEFmJdEmTofykJwiTVphfnOd40214365() {     float rtyXSftBemqZnSPiNVBWnNI76037371 = -577131674;    float rtyXSftBemqZnSPiNVBWnNI24718375 = 56980230;    float rtyXSftBemqZnSPiNVBWnNI77118658 = -124460038;    float rtyXSftBemqZnSPiNVBWnNI1423267 = -544859674;    float rtyXSftBemqZnSPiNVBWnNI63996383 = -256070569;    float rtyXSftBemqZnSPiNVBWnNI50229366 = -744897286;    float rtyXSftBemqZnSPiNVBWnNI64613318 = -757223954;    float rtyXSftBemqZnSPiNVBWnNI28682432 = -805759257;    float rtyXSftBemqZnSPiNVBWnNI92275754 = -541594264;    float rtyXSftBemqZnSPiNVBWnNI5976737 = -230504726;    float rtyXSftBemqZnSPiNVBWnNI37080050 = -618170297;    float rtyXSftBemqZnSPiNVBWnNI10011305 = -648724785;    float rtyXSftBemqZnSPiNVBWnNI77779991 = -786647357;    float rtyXSftBemqZnSPiNVBWnNI62947735 = -268759452;    float rtyXSftBemqZnSPiNVBWnNI26412179 = -17602174;    float rtyXSftBemqZnSPiNVBWnNI44304910 = -94095828;    float rtyXSftBemqZnSPiNVBWnNI47465300 = -17969551;    float rtyXSftBemqZnSPiNVBWnNI55312434 = -128146177;    float rtyXSftBemqZnSPiNVBWnNI52945916 = 1972286;    float rtyXSftBemqZnSPiNVBWnNI6609828 = -769135906;    float rtyXSftBemqZnSPiNVBWnNI64483079 = -474948003;    float rtyXSftBemqZnSPiNVBWnNI76469011 = -804624248;    float rtyXSftBemqZnSPiNVBWnNI15485919 = -603392580;    float rtyXSftBemqZnSPiNVBWnNI70197571 = -743050997;    float rtyXSftBemqZnSPiNVBWnNI71633622 = -806071453;    float rtyXSftBemqZnSPiNVBWnNI69914282 = -136077351;    float rtyXSftBemqZnSPiNVBWnNI20419086 = -567773438;    float rtyXSftBemqZnSPiNVBWnNI17059506 = -135589093;    float rtyXSftBemqZnSPiNVBWnNI37507033 = -587132470;    float rtyXSftBemqZnSPiNVBWnNI76431505 = -630051398;    float rtyXSftBemqZnSPiNVBWnNI63483106 = -579286730;    float rtyXSftBemqZnSPiNVBWnNI93670411 = -240027859;    float rtyXSftBemqZnSPiNVBWnNI40133487 = -102351564;    float rtyXSftBemqZnSPiNVBWnNI8728029 = -469108643;    float rtyXSftBemqZnSPiNVBWnNI69578547 = -726422794;    float rtyXSftBemqZnSPiNVBWnNI9007384 = -904474944;    float rtyXSftBemqZnSPiNVBWnNI86343787 = -253531046;    float rtyXSftBemqZnSPiNVBWnNI18980810 = -814078838;    float rtyXSftBemqZnSPiNVBWnNI35405863 = -512853860;    float rtyXSftBemqZnSPiNVBWnNI72611392 = -982788260;    float rtyXSftBemqZnSPiNVBWnNI60265974 = 93789791;    float rtyXSftBemqZnSPiNVBWnNI58705909 = -141786723;    float rtyXSftBemqZnSPiNVBWnNI11612442 = -885125500;    float rtyXSftBemqZnSPiNVBWnNI38187573 = -306531107;    float rtyXSftBemqZnSPiNVBWnNI82672179 = -611717647;    float rtyXSftBemqZnSPiNVBWnNI8320393 = -253751014;    float rtyXSftBemqZnSPiNVBWnNI65141074 = -339702939;    float rtyXSftBemqZnSPiNVBWnNI72963729 = -750137724;    float rtyXSftBemqZnSPiNVBWnNI58781189 = -427836450;    float rtyXSftBemqZnSPiNVBWnNI25591072 = -335091943;    float rtyXSftBemqZnSPiNVBWnNI51675646 = -742196668;    float rtyXSftBemqZnSPiNVBWnNI41977121 = -29018224;    float rtyXSftBemqZnSPiNVBWnNI97992773 = -948522166;    float rtyXSftBemqZnSPiNVBWnNI88017101 = -178230730;    float rtyXSftBemqZnSPiNVBWnNI75731286 = 6835957;    float rtyXSftBemqZnSPiNVBWnNI99568360 = -772507427;    float rtyXSftBemqZnSPiNVBWnNI9232457 = -339627190;    float rtyXSftBemqZnSPiNVBWnNI6921087 = -381409042;    float rtyXSftBemqZnSPiNVBWnNI29789644 = -738788221;    float rtyXSftBemqZnSPiNVBWnNI94082101 = -19993218;    float rtyXSftBemqZnSPiNVBWnNI29810281 = -77123849;    float rtyXSftBemqZnSPiNVBWnNI47553813 = -521634861;    float rtyXSftBemqZnSPiNVBWnNI91175399 = -118626788;    float rtyXSftBemqZnSPiNVBWnNI15844249 = -911542866;    float rtyXSftBemqZnSPiNVBWnNI42493630 = -651217997;    float rtyXSftBemqZnSPiNVBWnNI43409638 = -278142438;    float rtyXSftBemqZnSPiNVBWnNI69877817 = -446373222;    float rtyXSftBemqZnSPiNVBWnNI69051962 = -217538714;    float rtyXSftBemqZnSPiNVBWnNI93369187 = -542336658;    float rtyXSftBemqZnSPiNVBWnNI17404796 = -113127230;    float rtyXSftBemqZnSPiNVBWnNI57961123 = -840564782;    float rtyXSftBemqZnSPiNVBWnNI28484490 = -203890714;    float rtyXSftBemqZnSPiNVBWnNI19906572 = -615292318;    float rtyXSftBemqZnSPiNVBWnNI80334523 = -15239455;    float rtyXSftBemqZnSPiNVBWnNI46343853 = -762925697;    float rtyXSftBemqZnSPiNVBWnNI5777170 = -233161281;    float rtyXSftBemqZnSPiNVBWnNI64856569 = -919498749;    float rtyXSftBemqZnSPiNVBWnNI77298345 = -196861473;    float rtyXSftBemqZnSPiNVBWnNI87525392 = -31333350;    float rtyXSftBemqZnSPiNVBWnNI63313230 = -452320440;    float rtyXSftBemqZnSPiNVBWnNI4773208 = -796374412;    float rtyXSftBemqZnSPiNVBWnNI47455356 = -817635714;    float rtyXSftBemqZnSPiNVBWnNI58278317 = -707752644;    float rtyXSftBemqZnSPiNVBWnNI11915962 = -152040528;    float rtyXSftBemqZnSPiNVBWnNI24755860 = -887854730;    float rtyXSftBemqZnSPiNVBWnNI21505986 = -450268507;    float rtyXSftBemqZnSPiNVBWnNI95677638 = -291505694;    float rtyXSftBemqZnSPiNVBWnNI52116385 = -924120835;    float rtyXSftBemqZnSPiNVBWnNI32996743 = -375944601;    float rtyXSftBemqZnSPiNVBWnNI70010187 = -953915368;    float rtyXSftBemqZnSPiNVBWnNI99774927 = -464847755;    float rtyXSftBemqZnSPiNVBWnNI79422700 = -872122005;    float rtyXSftBemqZnSPiNVBWnNI89191166 = 24709383;    float rtyXSftBemqZnSPiNVBWnNI41323762 = -392860642;    float rtyXSftBemqZnSPiNVBWnNI42801112 = -805664412;    float rtyXSftBemqZnSPiNVBWnNI12712162 = -384575348;    float rtyXSftBemqZnSPiNVBWnNI67530509 = 76840065;    float rtyXSftBemqZnSPiNVBWnNI95768192 = -973582634;    float rtyXSftBemqZnSPiNVBWnNI95693942 = -655313111;    float rtyXSftBemqZnSPiNVBWnNI39262541 = -577131674;     rtyXSftBemqZnSPiNVBWnNI76037371 = rtyXSftBemqZnSPiNVBWnNI24718375;     rtyXSftBemqZnSPiNVBWnNI24718375 = rtyXSftBemqZnSPiNVBWnNI77118658;     rtyXSftBemqZnSPiNVBWnNI77118658 = rtyXSftBemqZnSPiNVBWnNI1423267;     rtyXSftBemqZnSPiNVBWnNI1423267 = rtyXSftBemqZnSPiNVBWnNI63996383;     rtyXSftBemqZnSPiNVBWnNI63996383 = rtyXSftBemqZnSPiNVBWnNI50229366;     rtyXSftBemqZnSPiNVBWnNI50229366 = rtyXSftBemqZnSPiNVBWnNI64613318;     rtyXSftBemqZnSPiNVBWnNI64613318 = rtyXSftBemqZnSPiNVBWnNI28682432;     rtyXSftBemqZnSPiNVBWnNI28682432 = rtyXSftBemqZnSPiNVBWnNI92275754;     rtyXSftBemqZnSPiNVBWnNI92275754 = rtyXSftBemqZnSPiNVBWnNI5976737;     rtyXSftBemqZnSPiNVBWnNI5976737 = rtyXSftBemqZnSPiNVBWnNI37080050;     rtyXSftBemqZnSPiNVBWnNI37080050 = rtyXSftBemqZnSPiNVBWnNI10011305;     rtyXSftBemqZnSPiNVBWnNI10011305 = rtyXSftBemqZnSPiNVBWnNI77779991;     rtyXSftBemqZnSPiNVBWnNI77779991 = rtyXSftBemqZnSPiNVBWnNI62947735;     rtyXSftBemqZnSPiNVBWnNI62947735 = rtyXSftBemqZnSPiNVBWnNI26412179;     rtyXSftBemqZnSPiNVBWnNI26412179 = rtyXSftBemqZnSPiNVBWnNI44304910;     rtyXSftBemqZnSPiNVBWnNI44304910 = rtyXSftBemqZnSPiNVBWnNI47465300;     rtyXSftBemqZnSPiNVBWnNI47465300 = rtyXSftBemqZnSPiNVBWnNI55312434;     rtyXSftBemqZnSPiNVBWnNI55312434 = rtyXSftBemqZnSPiNVBWnNI52945916;     rtyXSftBemqZnSPiNVBWnNI52945916 = rtyXSftBemqZnSPiNVBWnNI6609828;     rtyXSftBemqZnSPiNVBWnNI6609828 = rtyXSftBemqZnSPiNVBWnNI64483079;     rtyXSftBemqZnSPiNVBWnNI64483079 = rtyXSftBemqZnSPiNVBWnNI76469011;     rtyXSftBemqZnSPiNVBWnNI76469011 = rtyXSftBemqZnSPiNVBWnNI15485919;     rtyXSftBemqZnSPiNVBWnNI15485919 = rtyXSftBemqZnSPiNVBWnNI70197571;     rtyXSftBemqZnSPiNVBWnNI70197571 = rtyXSftBemqZnSPiNVBWnNI71633622;     rtyXSftBemqZnSPiNVBWnNI71633622 = rtyXSftBemqZnSPiNVBWnNI69914282;     rtyXSftBemqZnSPiNVBWnNI69914282 = rtyXSftBemqZnSPiNVBWnNI20419086;     rtyXSftBemqZnSPiNVBWnNI20419086 = rtyXSftBemqZnSPiNVBWnNI17059506;     rtyXSftBemqZnSPiNVBWnNI17059506 = rtyXSftBemqZnSPiNVBWnNI37507033;     rtyXSftBemqZnSPiNVBWnNI37507033 = rtyXSftBemqZnSPiNVBWnNI76431505;     rtyXSftBemqZnSPiNVBWnNI76431505 = rtyXSftBemqZnSPiNVBWnNI63483106;     rtyXSftBemqZnSPiNVBWnNI63483106 = rtyXSftBemqZnSPiNVBWnNI93670411;     rtyXSftBemqZnSPiNVBWnNI93670411 = rtyXSftBemqZnSPiNVBWnNI40133487;     rtyXSftBemqZnSPiNVBWnNI40133487 = rtyXSftBemqZnSPiNVBWnNI8728029;     rtyXSftBemqZnSPiNVBWnNI8728029 = rtyXSftBemqZnSPiNVBWnNI69578547;     rtyXSftBemqZnSPiNVBWnNI69578547 = rtyXSftBemqZnSPiNVBWnNI9007384;     rtyXSftBemqZnSPiNVBWnNI9007384 = rtyXSftBemqZnSPiNVBWnNI86343787;     rtyXSftBemqZnSPiNVBWnNI86343787 = rtyXSftBemqZnSPiNVBWnNI18980810;     rtyXSftBemqZnSPiNVBWnNI18980810 = rtyXSftBemqZnSPiNVBWnNI35405863;     rtyXSftBemqZnSPiNVBWnNI35405863 = rtyXSftBemqZnSPiNVBWnNI72611392;     rtyXSftBemqZnSPiNVBWnNI72611392 = rtyXSftBemqZnSPiNVBWnNI60265974;     rtyXSftBemqZnSPiNVBWnNI60265974 = rtyXSftBemqZnSPiNVBWnNI58705909;     rtyXSftBemqZnSPiNVBWnNI58705909 = rtyXSftBemqZnSPiNVBWnNI11612442;     rtyXSftBemqZnSPiNVBWnNI11612442 = rtyXSftBemqZnSPiNVBWnNI38187573;     rtyXSftBemqZnSPiNVBWnNI38187573 = rtyXSftBemqZnSPiNVBWnNI82672179;     rtyXSftBemqZnSPiNVBWnNI82672179 = rtyXSftBemqZnSPiNVBWnNI8320393;     rtyXSftBemqZnSPiNVBWnNI8320393 = rtyXSftBemqZnSPiNVBWnNI65141074;     rtyXSftBemqZnSPiNVBWnNI65141074 = rtyXSftBemqZnSPiNVBWnNI72963729;     rtyXSftBemqZnSPiNVBWnNI72963729 = rtyXSftBemqZnSPiNVBWnNI58781189;     rtyXSftBemqZnSPiNVBWnNI58781189 = rtyXSftBemqZnSPiNVBWnNI25591072;     rtyXSftBemqZnSPiNVBWnNI25591072 = rtyXSftBemqZnSPiNVBWnNI51675646;     rtyXSftBemqZnSPiNVBWnNI51675646 = rtyXSftBemqZnSPiNVBWnNI41977121;     rtyXSftBemqZnSPiNVBWnNI41977121 = rtyXSftBemqZnSPiNVBWnNI97992773;     rtyXSftBemqZnSPiNVBWnNI97992773 = rtyXSftBemqZnSPiNVBWnNI88017101;     rtyXSftBemqZnSPiNVBWnNI88017101 = rtyXSftBemqZnSPiNVBWnNI75731286;     rtyXSftBemqZnSPiNVBWnNI75731286 = rtyXSftBemqZnSPiNVBWnNI99568360;     rtyXSftBemqZnSPiNVBWnNI99568360 = rtyXSftBemqZnSPiNVBWnNI9232457;     rtyXSftBemqZnSPiNVBWnNI9232457 = rtyXSftBemqZnSPiNVBWnNI6921087;     rtyXSftBemqZnSPiNVBWnNI6921087 = rtyXSftBemqZnSPiNVBWnNI29789644;     rtyXSftBemqZnSPiNVBWnNI29789644 = rtyXSftBemqZnSPiNVBWnNI94082101;     rtyXSftBemqZnSPiNVBWnNI94082101 = rtyXSftBemqZnSPiNVBWnNI29810281;     rtyXSftBemqZnSPiNVBWnNI29810281 = rtyXSftBemqZnSPiNVBWnNI47553813;     rtyXSftBemqZnSPiNVBWnNI47553813 = rtyXSftBemqZnSPiNVBWnNI91175399;     rtyXSftBemqZnSPiNVBWnNI91175399 = rtyXSftBemqZnSPiNVBWnNI15844249;     rtyXSftBemqZnSPiNVBWnNI15844249 = rtyXSftBemqZnSPiNVBWnNI42493630;     rtyXSftBemqZnSPiNVBWnNI42493630 = rtyXSftBemqZnSPiNVBWnNI43409638;     rtyXSftBemqZnSPiNVBWnNI43409638 = rtyXSftBemqZnSPiNVBWnNI69877817;     rtyXSftBemqZnSPiNVBWnNI69877817 = rtyXSftBemqZnSPiNVBWnNI69051962;     rtyXSftBemqZnSPiNVBWnNI69051962 = rtyXSftBemqZnSPiNVBWnNI93369187;     rtyXSftBemqZnSPiNVBWnNI93369187 = rtyXSftBemqZnSPiNVBWnNI17404796;     rtyXSftBemqZnSPiNVBWnNI17404796 = rtyXSftBemqZnSPiNVBWnNI57961123;     rtyXSftBemqZnSPiNVBWnNI57961123 = rtyXSftBemqZnSPiNVBWnNI28484490;     rtyXSftBemqZnSPiNVBWnNI28484490 = rtyXSftBemqZnSPiNVBWnNI19906572;     rtyXSftBemqZnSPiNVBWnNI19906572 = rtyXSftBemqZnSPiNVBWnNI80334523;     rtyXSftBemqZnSPiNVBWnNI80334523 = rtyXSftBemqZnSPiNVBWnNI46343853;     rtyXSftBemqZnSPiNVBWnNI46343853 = rtyXSftBemqZnSPiNVBWnNI5777170;     rtyXSftBemqZnSPiNVBWnNI5777170 = rtyXSftBemqZnSPiNVBWnNI64856569;     rtyXSftBemqZnSPiNVBWnNI64856569 = rtyXSftBemqZnSPiNVBWnNI77298345;     rtyXSftBemqZnSPiNVBWnNI77298345 = rtyXSftBemqZnSPiNVBWnNI87525392;     rtyXSftBemqZnSPiNVBWnNI87525392 = rtyXSftBemqZnSPiNVBWnNI63313230;     rtyXSftBemqZnSPiNVBWnNI63313230 = rtyXSftBemqZnSPiNVBWnNI4773208;     rtyXSftBemqZnSPiNVBWnNI4773208 = rtyXSftBemqZnSPiNVBWnNI47455356;     rtyXSftBemqZnSPiNVBWnNI47455356 = rtyXSftBemqZnSPiNVBWnNI58278317;     rtyXSftBemqZnSPiNVBWnNI58278317 = rtyXSftBemqZnSPiNVBWnNI11915962;     rtyXSftBemqZnSPiNVBWnNI11915962 = rtyXSftBemqZnSPiNVBWnNI24755860;     rtyXSftBemqZnSPiNVBWnNI24755860 = rtyXSftBemqZnSPiNVBWnNI21505986;     rtyXSftBemqZnSPiNVBWnNI21505986 = rtyXSftBemqZnSPiNVBWnNI95677638;     rtyXSftBemqZnSPiNVBWnNI95677638 = rtyXSftBemqZnSPiNVBWnNI52116385;     rtyXSftBemqZnSPiNVBWnNI52116385 = rtyXSftBemqZnSPiNVBWnNI32996743;     rtyXSftBemqZnSPiNVBWnNI32996743 = rtyXSftBemqZnSPiNVBWnNI70010187;     rtyXSftBemqZnSPiNVBWnNI70010187 = rtyXSftBemqZnSPiNVBWnNI99774927;     rtyXSftBemqZnSPiNVBWnNI99774927 = rtyXSftBemqZnSPiNVBWnNI79422700;     rtyXSftBemqZnSPiNVBWnNI79422700 = rtyXSftBemqZnSPiNVBWnNI89191166;     rtyXSftBemqZnSPiNVBWnNI89191166 = rtyXSftBemqZnSPiNVBWnNI41323762;     rtyXSftBemqZnSPiNVBWnNI41323762 = rtyXSftBemqZnSPiNVBWnNI42801112;     rtyXSftBemqZnSPiNVBWnNI42801112 = rtyXSftBemqZnSPiNVBWnNI12712162;     rtyXSftBemqZnSPiNVBWnNI12712162 = rtyXSftBemqZnSPiNVBWnNI67530509;     rtyXSftBemqZnSPiNVBWnNI67530509 = rtyXSftBemqZnSPiNVBWnNI95768192;     rtyXSftBemqZnSPiNVBWnNI95768192 = rtyXSftBemqZnSPiNVBWnNI95693942;     rtyXSftBemqZnSPiNVBWnNI95693942 = rtyXSftBemqZnSPiNVBWnNI39262541;     rtyXSftBemqZnSPiNVBWnNI39262541 = rtyXSftBemqZnSPiNVBWnNI76037371;}



void VtUApDTcKVBhnXmFnkUJVHFwkd25277203() {     float KAPcpUcTasxXcDFGvoaURcj59760890 = -927153741;    float KAPcpUcTasxXcDFGvoaURcj22316470 = -846671621;    float KAPcpUcTasxXcDFGvoaURcj65704775 = 7232271;    float KAPcpUcTasxXcDFGvoaURcj98571219 = -549741603;    float KAPcpUcTasxXcDFGvoaURcj43557720 = -654018760;    float KAPcpUcTasxXcDFGvoaURcj61568884 = -848217955;    float KAPcpUcTasxXcDFGvoaURcj1493882 = -778738092;    float KAPcpUcTasxXcDFGvoaURcj72022515 = -326213843;    float KAPcpUcTasxXcDFGvoaURcj42495283 = -139076960;    float KAPcpUcTasxXcDFGvoaURcj52082325 = -453545742;    float KAPcpUcTasxXcDFGvoaURcj79969085 = -458455555;    float KAPcpUcTasxXcDFGvoaURcj223075 = -577864057;    float KAPcpUcTasxXcDFGvoaURcj28833843 = -629479601;    float KAPcpUcTasxXcDFGvoaURcj57320879 = -618442314;    float KAPcpUcTasxXcDFGvoaURcj30591438 = 56700521;    float KAPcpUcTasxXcDFGvoaURcj16118939 = -18223479;    float KAPcpUcTasxXcDFGvoaURcj47330710 = -176849414;    float KAPcpUcTasxXcDFGvoaURcj69389704 = -838997751;    float KAPcpUcTasxXcDFGvoaURcj93922397 = -886676953;    float KAPcpUcTasxXcDFGvoaURcj80717472 = -385428823;    float KAPcpUcTasxXcDFGvoaURcj31822589 = -49414524;    float KAPcpUcTasxXcDFGvoaURcj77471082 = -717779534;    float KAPcpUcTasxXcDFGvoaURcj84661783 = -632787317;    float KAPcpUcTasxXcDFGvoaURcj71340266 = -282283650;    float KAPcpUcTasxXcDFGvoaURcj8235716 = -884402215;    float KAPcpUcTasxXcDFGvoaURcj22907040 = -579044379;    float KAPcpUcTasxXcDFGvoaURcj42011025 = 88351139;    float KAPcpUcTasxXcDFGvoaURcj59658706 = 77969419;    float KAPcpUcTasxXcDFGvoaURcj9425175 = -11328370;    float KAPcpUcTasxXcDFGvoaURcj11979833 = -202657773;    float KAPcpUcTasxXcDFGvoaURcj44847529 = -798384751;    float KAPcpUcTasxXcDFGvoaURcj18463700 = -789225082;    float KAPcpUcTasxXcDFGvoaURcj54258188 = -767565655;    float KAPcpUcTasxXcDFGvoaURcj44452743 = -873205291;    float KAPcpUcTasxXcDFGvoaURcj37181873 = -460713402;    float KAPcpUcTasxXcDFGvoaURcj78868617 = -882871422;    float KAPcpUcTasxXcDFGvoaURcj86107377 = -573872343;    float KAPcpUcTasxXcDFGvoaURcj69362601 = -713222055;    float KAPcpUcTasxXcDFGvoaURcj96021581 = -818412741;    float KAPcpUcTasxXcDFGvoaURcj11800509 = -929435140;    float KAPcpUcTasxXcDFGvoaURcj25033495 = -656250335;    float KAPcpUcTasxXcDFGvoaURcj88045635 = -847812392;    float KAPcpUcTasxXcDFGvoaURcj44412588 = -537529754;    float KAPcpUcTasxXcDFGvoaURcj85383092 = -985474840;    float KAPcpUcTasxXcDFGvoaURcj10659430 = -937731726;    float KAPcpUcTasxXcDFGvoaURcj52587274 = -968782706;    float KAPcpUcTasxXcDFGvoaURcj98320043 = -361348846;    float KAPcpUcTasxXcDFGvoaURcj11705577 = -512259621;    float KAPcpUcTasxXcDFGvoaURcj92655402 = -127620601;    float KAPcpUcTasxXcDFGvoaURcj61464401 = -967107378;    float KAPcpUcTasxXcDFGvoaURcj91766477 = -907969070;    float KAPcpUcTasxXcDFGvoaURcj93482840 = -881071010;    float KAPcpUcTasxXcDFGvoaURcj43375176 = -960454532;    float KAPcpUcTasxXcDFGvoaURcj33949708 = -754939917;    float KAPcpUcTasxXcDFGvoaURcj93454613 = -309890988;    float KAPcpUcTasxXcDFGvoaURcj82289807 = -109374208;    float KAPcpUcTasxXcDFGvoaURcj37654687 = -113884305;    float KAPcpUcTasxXcDFGvoaURcj94364508 = -710484079;    float KAPcpUcTasxXcDFGvoaURcj90335504 = -665339389;    float KAPcpUcTasxXcDFGvoaURcj20650680 = 25025618;    float KAPcpUcTasxXcDFGvoaURcj19557859 = -836569095;    float KAPcpUcTasxXcDFGvoaURcj41835176 = -756707511;    float KAPcpUcTasxXcDFGvoaURcj62597341 = -214885474;    float KAPcpUcTasxXcDFGvoaURcj30515451 = -936419188;    float KAPcpUcTasxXcDFGvoaURcj7234796 = -655160992;    float KAPcpUcTasxXcDFGvoaURcj61505386 = -669230474;    float KAPcpUcTasxXcDFGvoaURcj45964887 = -810298402;    float KAPcpUcTasxXcDFGvoaURcj84381100 = -756274310;    float KAPcpUcTasxXcDFGvoaURcj20139007 = -57728912;    float KAPcpUcTasxXcDFGvoaURcj51722820 = -60428058;    float KAPcpUcTasxXcDFGvoaURcj30011562 = -444351137;    float KAPcpUcTasxXcDFGvoaURcj77968109 = -463627360;    float KAPcpUcTasxXcDFGvoaURcj73368122 = 79414989;    float KAPcpUcTasxXcDFGvoaURcj82121888 = -957241813;    float KAPcpUcTasxXcDFGvoaURcj55683977 = -729178488;    float KAPcpUcTasxXcDFGvoaURcj43776954 = -201602132;    float KAPcpUcTasxXcDFGvoaURcj33058494 = -80249781;    float KAPcpUcTasxXcDFGvoaURcj99278691 = -647312478;    float KAPcpUcTasxXcDFGvoaURcj60680836 = -344551925;    float KAPcpUcTasxXcDFGvoaURcj55648442 = -915619509;    float KAPcpUcTasxXcDFGvoaURcj24586996 = -117695533;    float KAPcpUcTasxXcDFGvoaURcj30305448 = -399389240;    float KAPcpUcTasxXcDFGvoaURcj67003304 = -794409981;    float KAPcpUcTasxXcDFGvoaURcj47960773 = -44220993;    float KAPcpUcTasxXcDFGvoaURcj20213355 = -294688703;    float KAPcpUcTasxXcDFGvoaURcj51364689 = -917313741;    float KAPcpUcTasxXcDFGvoaURcj75088524 = -828770550;    float KAPcpUcTasxXcDFGvoaURcj20308481 = 87374262;    float KAPcpUcTasxXcDFGvoaURcj50998129 = -463314303;    float KAPcpUcTasxXcDFGvoaURcj54892065 = -251339195;    float KAPcpUcTasxXcDFGvoaURcj41213931 = -668987117;    float KAPcpUcTasxXcDFGvoaURcj91742868 = -863388264;    float KAPcpUcTasxXcDFGvoaURcj79027097 = 52117334;    float KAPcpUcTasxXcDFGvoaURcj75370902 = -743438360;    float KAPcpUcTasxXcDFGvoaURcj92242649 = 7133955;    float KAPcpUcTasxXcDFGvoaURcj83198319 = -899542825;    float KAPcpUcTasxXcDFGvoaURcj25448295 = -532926919;    float KAPcpUcTasxXcDFGvoaURcj13897138 = -601110567;    float KAPcpUcTasxXcDFGvoaURcj78148296 = -230313849;    float KAPcpUcTasxXcDFGvoaURcj49154044 = -927153741;     KAPcpUcTasxXcDFGvoaURcj59760890 = KAPcpUcTasxXcDFGvoaURcj22316470;     KAPcpUcTasxXcDFGvoaURcj22316470 = KAPcpUcTasxXcDFGvoaURcj65704775;     KAPcpUcTasxXcDFGvoaURcj65704775 = KAPcpUcTasxXcDFGvoaURcj98571219;     KAPcpUcTasxXcDFGvoaURcj98571219 = KAPcpUcTasxXcDFGvoaURcj43557720;     KAPcpUcTasxXcDFGvoaURcj43557720 = KAPcpUcTasxXcDFGvoaURcj61568884;     KAPcpUcTasxXcDFGvoaURcj61568884 = KAPcpUcTasxXcDFGvoaURcj1493882;     KAPcpUcTasxXcDFGvoaURcj1493882 = KAPcpUcTasxXcDFGvoaURcj72022515;     KAPcpUcTasxXcDFGvoaURcj72022515 = KAPcpUcTasxXcDFGvoaURcj42495283;     KAPcpUcTasxXcDFGvoaURcj42495283 = KAPcpUcTasxXcDFGvoaURcj52082325;     KAPcpUcTasxXcDFGvoaURcj52082325 = KAPcpUcTasxXcDFGvoaURcj79969085;     KAPcpUcTasxXcDFGvoaURcj79969085 = KAPcpUcTasxXcDFGvoaURcj223075;     KAPcpUcTasxXcDFGvoaURcj223075 = KAPcpUcTasxXcDFGvoaURcj28833843;     KAPcpUcTasxXcDFGvoaURcj28833843 = KAPcpUcTasxXcDFGvoaURcj57320879;     KAPcpUcTasxXcDFGvoaURcj57320879 = KAPcpUcTasxXcDFGvoaURcj30591438;     KAPcpUcTasxXcDFGvoaURcj30591438 = KAPcpUcTasxXcDFGvoaURcj16118939;     KAPcpUcTasxXcDFGvoaURcj16118939 = KAPcpUcTasxXcDFGvoaURcj47330710;     KAPcpUcTasxXcDFGvoaURcj47330710 = KAPcpUcTasxXcDFGvoaURcj69389704;     KAPcpUcTasxXcDFGvoaURcj69389704 = KAPcpUcTasxXcDFGvoaURcj93922397;     KAPcpUcTasxXcDFGvoaURcj93922397 = KAPcpUcTasxXcDFGvoaURcj80717472;     KAPcpUcTasxXcDFGvoaURcj80717472 = KAPcpUcTasxXcDFGvoaURcj31822589;     KAPcpUcTasxXcDFGvoaURcj31822589 = KAPcpUcTasxXcDFGvoaURcj77471082;     KAPcpUcTasxXcDFGvoaURcj77471082 = KAPcpUcTasxXcDFGvoaURcj84661783;     KAPcpUcTasxXcDFGvoaURcj84661783 = KAPcpUcTasxXcDFGvoaURcj71340266;     KAPcpUcTasxXcDFGvoaURcj71340266 = KAPcpUcTasxXcDFGvoaURcj8235716;     KAPcpUcTasxXcDFGvoaURcj8235716 = KAPcpUcTasxXcDFGvoaURcj22907040;     KAPcpUcTasxXcDFGvoaURcj22907040 = KAPcpUcTasxXcDFGvoaURcj42011025;     KAPcpUcTasxXcDFGvoaURcj42011025 = KAPcpUcTasxXcDFGvoaURcj59658706;     KAPcpUcTasxXcDFGvoaURcj59658706 = KAPcpUcTasxXcDFGvoaURcj9425175;     KAPcpUcTasxXcDFGvoaURcj9425175 = KAPcpUcTasxXcDFGvoaURcj11979833;     KAPcpUcTasxXcDFGvoaURcj11979833 = KAPcpUcTasxXcDFGvoaURcj44847529;     KAPcpUcTasxXcDFGvoaURcj44847529 = KAPcpUcTasxXcDFGvoaURcj18463700;     KAPcpUcTasxXcDFGvoaURcj18463700 = KAPcpUcTasxXcDFGvoaURcj54258188;     KAPcpUcTasxXcDFGvoaURcj54258188 = KAPcpUcTasxXcDFGvoaURcj44452743;     KAPcpUcTasxXcDFGvoaURcj44452743 = KAPcpUcTasxXcDFGvoaURcj37181873;     KAPcpUcTasxXcDFGvoaURcj37181873 = KAPcpUcTasxXcDFGvoaURcj78868617;     KAPcpUcTasxXcDFGvoaURcj78868617 = KAPcpUcTasxXcDFGvoaURcj86107377;     KAPcpUcTasxXcDFGvoaURcj86107377 = KAPcpUcTasxXcDFGvoaURcj69362601;     KAPcpUcTasxXcDFGvoaURcj69362601 = KAPcpUcTasxXcDFGvoaURcj96021581;     KAPcpUcTasxXcDFGvoaURcj96021581 = KAPcpUcTasxXcDFGvoaURcj11800509;     KAPcpUcTasxXcDFGvoaURcj11800509 = KAPcpUcTasxXcDFGvoaURcj25033495;     KAPcpUcTasxXcDFGvoaURcj25033495 = KAPcpUcTasxXcDFGvoaURcj88045635;     KAPcpUcTasxXcDFGvoaURcj88045635 = KAPcpUcTasxXcDFGvoaURcj44412588;     KAPcpUcTasxXcDFGvoaURcj44412588 = KAPcpUcTasxXcDFGvoaURcj85383092;     KAPcpUcTasxXcDFGvoaURcj85383092 = KAPcpUcTasxXcDFGvoaURcj10659430;     KAPcpUcTasxXcDFGvoaURcj10659430 = KAPcpUcTasxXcDFGvoaURcj52587274;     KAPcpUcTasxXcDFGvoaURcj52587274 = KAPcpUcTasxXcDFGvoaURcj98320043;     KAPcpUcTasxXcDFGvoaURcj98320043 = KAPcpUcTasxXcDFGvoaURcj11705577;     KAPcpUcTasxXcDFGvoaURcj11705577 = KAPcpUcTasxXcDFGvoaURcj92655402;     KAPcpUcTasxXcDFGvoaURcj92655402 = KAPcpUcTasxXcDFGvoaURcj61464401;     KAPcpUcTasxXcDFGvoaURcj61464401 = KAPcpUcTasxXcDFGvoaURcj91766477;     KAPcpUcTasxXcDFGvoaURcj91766477 = KAPcpUcTasxXcDFGvoaURcj93482840;     KAPcpUcTasxXcDFGvoaURcj93482840 = KAPcpUcTasxXcDFGvoaURcj43375176;     KAPcpUcTasxXcDFGvoaURcj43375176 = KAPcpUcTasxXcDFGvoaURcj33949708;     KAPcpUcTasxXcDFGvoaURcj33949708 = KAPcpUcTasxXcDFGvoaURcj93454613;     KAPcpUcTasxXcDFGvoaURcj93454613 = KAPcpUcTasxXcDFGvoaURcj82289807;     KAPcpUcTasxXcDFGvoaURcj82289807 = KAPcpUcTasxXcDFGvoaURcj37654687;     KAPcpUcTasxXcDFGvoaURcj37654687 = KAPcpUcTasxXcDFGvoaURcj94364508;     KAPcpUcTasxXcDFGvoaURcj94364508 = KAPcpUcTasxXcDFGvoaURcj90335504;     KAPcpUcTasxXcDFGvoaURcj90335504 = KAPcpUcTasxXcDFGvoaURcj20650680;     KAPcpUcTasxXcDFGvoaURcj20650680 = KAPcpUcTasxXcDFGvoaURcj19557859;     KAPcpUcTasxXcDFGvoaURcj19557859 = KAPcpUcTasxXcDFGvoaURcj41835176;     KAPcpUcTasxXcDFGvoaURcj41835176 = KAPcpUcTasxXcDFGvoaURcj62597341;     KAPcpUcTasxXcDFGvoaURcj62597341 = KAPcpUcTasxXcDFGvoaURcj30515451;     KAPcpUcTasxXcDFGvoaURcj30515451 = KAPcpUcTasxXcDFGvoaURcj7234796;     KAPcpUcTasxXcDFGvoaURcj7234796 = KAPcpUcTasxXcDFGvoaURcj61505386;     KAPcpUcTasxXcDFGvoaURcj61505386 = KAPcpUcTasxXcDFGvoaURcj45964887;     KAPcpUcTasxXcDFGvoaURcj45964887 = KAPcpUcTasxXcDFGvoaURcj84381100;     KAPcpUcTasxXcDFGvoaURcj84381100 = KAPcpUcTasxXcDFGvoaURcj20139007;     KAPcpUcTasxXcDFGvoaURcj20139007 = KAPcpUcTasxXcDFGvoaURcj51722820;     KAPcpUcTasxXcDFGvoaURcj51722820 = KAPcpUcTasxXcDFGvoaURcj30011562;     KAPcpUcTasxXcDFGvoaURcj30011562 = KAPcpUcTasxXcDFGvoaURcj77968109;     KAPcpUcTasxXcDFGvoaURcj77968109 = KAPcpUcTasxXcDFGvoaURcj73368122;     KAPcpUcTasxXcDFGvoaURcj73368122 = KAPcpUcTasxXcDFGvoaURcj82121888;     KAPcpUcTasxXcDFGvoaURcj82121888 = KAPcpUcTasxXcDFGvoaURcj55683977;     KAPcpUcTasxXcDFGvoaURcj55683977 = KAPcpUcTasxXcDFGvoaURcj43776954;     KAPcpUcTasxXcDFGvoaURcj43776954 = KAPcpUcTasxXcDFGvoaURcj33058494;     KAPcpUcTasxXcDFGvoaURcj33058494 = KAPcpUcTasxXcDFGvoaURcj99278691;     KAPcpUcTasxXcDFGvoaURcj99278691 = KAPcpUcTasxXcDFGvoaURcj60680836;     KAPcpUcTasxXcDFGvoaURcj60680836 = KAPcpUcTasxXcDFGvoaURcj55648442;     KAPcpUcTasxXcDFGvoaURcj55648442 = KAPcpUcTasxXcDFGvoaURcj24586996;     KAPcpUcTasxXcDFGvoaURcj24586996 = KAPcpUcTasxXcDFGvoaURcj30305448;     KAPcpUcTasxXcDFGvoaURcj30305448 = KAPcpUcTasxXcDFGvoaURcj67003304;     KAPcpUcTasxXcDFGvoaURcj67003304 = KAPcpUcTasxXcDFGvoaURcj47960773;     KAPcpUcTasxXcDFGvoaURcj47960773 = KAPcpUcTasxXcDFGvoaURcj20213355;     KAPcpUcTasxXcDFGvoaURcj20213355 = KAPcpUcTasxXcDFGvoaURcj51364689;     KAPcpUcTasxXcDFGvoaURcj51364689 = KAPcpUcTasxXcDFGvoaURcj75088524;     KAPcpUcTasxXcDFGvoaURcj75088524 = KAPcpUcTasxXcDFGvoaURcj20308481;     KAPcpUcTasxXcDFGvoaURcj20308481 = KAPcpUcTasxXcDFGvoaURcj50998129;     KAPcpUcTasxXcDFGvoaURcj50998129 = KAPcpUcTasxXcDFGvoaURcj54892065;     KAPcpUcTasxXcDFGvoaURcj54892065 = KAPcpUcTasxXcDFGvoaURcj41213931;     KAPcpUcTasxXcDFGvoaURcj41213931 = KAPcpUcTasxXcDFGvoaURcj91742868;     KAPcpUcTasxXcDFGvoaURcj91742868 = KAPcpUcTasxXcDFGvoaURcj79027097;     KAPcpUcTasxXcDFGvoaURcj79027097 = KAPcpUcTasxXcDFGvoaURcj75370902;     KAPcpUcTasxXcDFGvoaURcj75370902 = KAPcpUcTasxXcDFGvoaURcj92242649;     KAPcpUcTasxXcDFGvoaURcj92242649 = KAPcpUcTasxXcDFGvoaURcj83198319;     KAPcpUcTasxXcDFGvoaURcj83198319 = KAPcpUcTasxXcDFGvoaURcj25448295;     KAPcpUcTasxXcDFGvoaURcj25448295 = KAPcpUcTasxXcDFGvoaURcj13897138;     KAPcpUcTasxXcDFGvoaURcj13897138 = KAPcpUcTasxXcDFGvoaURcj78148296;     KAPcpUcTasxXcDFGvoaURcj78148296 = KAPcpUcTasxXcDFGvoaURcj49154044;     KAPcpUcTasxXcDFGvoaURcj49154044 = KAPcpUcTasxXcDFGvoaURcj59760890;}



void ZiKLIBPqoruQQekeBQCPqgEbsD10340041() {     float FkQrnfIjpoUSUTZuXkAyujr43484408 = -177175808;    float FkQrnfIjpoUSUTZuXkAyujr19914566 = -650323472;    float FkQrnfIjpoUSUTZuXkAyujr54290892 = -961075420;    float FkQrnfIjpoUSUTZuXkAyujr95719172 = -554623532;    float FkQrnfIjpoUSUTZuXkAyujr23119057 = 48033048;    float FkQrnfIjpoUSUTZuXkAyujr72908401 = -951538624;    float FkQrnfIjpoUSUTZuXkAyujr38374444 = -800252230;    float FkQrnfIjpoUSUTZuXkAyujr15362598 = -946668429;    float FkQrnfIjpoUSUTZuXkAyujr92714811 = -836559657;    float FkQrnfIjpoUSUTZuXkAyujr98187913 = -676586757;    float FkQrnfIjpoUSUTZuXkAyujr22858122 = -298740813;    float FkQrnfIjpoUSUTZuXkAyujr90434845 = -507003328;    float FkQrnfIjpoUSUTZuXkAyujr79887694 = -472311844;    float FkQrnfIjpoUSUTZuXkAyujr51694024 = -968125176;    float FkQrnfIjpoUSUTZuXkAyujr34770696 = -968996785;    float FkQrnfIjpoUSUTZuXkAyujr87932967 = 57648869;    float FkQrnfIjpoUSUTZuXkAyujr47196121 = -335729277;    float FkQrnfIjpoUSUTZuXkAyujr83466973 = -449849325;    float FkQrnfIjpoUSUTZuXkAyujr34898878 = -675326191;    float FkQrnfIjpoUSUTZuXkAyujr54825116 = -1721740;    float FkQrnfIjpoUSUTZuXkAyujr99162099 = -723881045;    float FkQrnfIjpoUSUTZuXkAyujr78473153 = -630934820;    float FkQrnfIjpoUSUTZuXkAyujr53837649 = -662182054;    float FkQrnfIjpoUSUTZuXkAyujr72482961 = -921516303;    float FkQrnfIjpoUSUTZuXkAyujr44837808 = -962732976;    float FkQrnfIjpoUSUTZuXkAyujr75899798 = 77988593;    float FkQrnfIjpoUSUTZuXkAyujr63602964 = -355524284;    float FkQrnfIjpoUSUTZuXkAyujr2257907 = -808472069;    float FkQrnfIjpoUSUTZuXkAyujr81343315 = -535524271;    float FkQrnfIjpoUSUTZuXkAyujr47528160 = -875264147;    float FkQrnfIjpoUSUTZuXkAyujr26211953 = 82517229;    float FkQrnfIjpoUSUTZuXkAyujr43256988 = -238422304;    float FkQrnfIjpoUSUTZuXkAyujr68382889 = -332779746;    float FkQrnfIjpoUSUTZuXkAyujr80177456 = -177301939;    float FkQrnfIjpoUSUTZuXkAyujr4785199 = -195004010;    float FkQrnfIjpoUSUTZuXkAyujr48729851 = -861267899;    float FkQrnfIjpoUSUTZuXkAyujr85870967 = -894213639;    float FkQrnfIjpoUSUTZuXkAyujr19744392 = -612365271;    float FkQrnfIjpoUSUTZuXkAyujr56637301 = -23971622;    float FkQrnfIjpoUSUTZuXkAyujr50989624 = -876082019;    float FkQrnfIjpoUSUTZuXkAyujr89801015 = -306290462;    float FkQrnfIjpoUSUTZuXkAyujr17385362 = -453838061;    float FkQrnfIjpoUSUTZuXkAyujr77212734 = -189934008;    float FkQrnfIjpoUSUTZuXkAyujr32578612 = -564418572;    float FkQrnfIjpoUSUTZuXkAyujr38646681 = -163745804;    float FkQrnfIjpoUSUTZuXkAyujr96854155 = -583814398;    float FkQrnfIjpoUSUTZuXkAyujr31499013 = -382994753;    float FkQrnfIjpoUSUTZuXkAyujr50447424 = -274381519;    float FkQrnfIjpoUSUTZuXkAyujr26529616 = -927404752;    float FkQrnfIjpoUSUTZuXkAyujr97337731 = -499122813;    float FkQrnfIjpoUSUTZuXkAyujr31857309 = 26258529;    float FkQrnfIjpoUSUTZuXkAyujr44988561 = -633123797;    float FkQrnfIjpoUSUTZuXkAyujr88757578 = -972386898;    float FkQrnfIjpoUSUTZuXkAyujr79882313 = -231649105;    float FkQrnfIjpoUSUTZuXkAyujr11177941 = -626617933;    float FkQrnfIjpoUSUTZuXkAyujr65011254 = -546240989;    float FkQrnfIjpoUSUTZuXkAyujr66076917 = -988141419;    float FkQrnfIjpoUSUTZuXkAyujr81807931 = 60440883;    float FkQrnfIjpoUSUTZuXkAyujr50881364 = -591890556;    float FkQrnfIjpoUSUTZuXkAyujr47219259 = 70044455;    float FkQrnfIjpoUSUTZuXkAyujr9305438 = -496014341;    float FkQrnfIjpoUSUTZuXkAyujr36116538 = -991780161;    float FkQrnfIjpoUSUTZuXkAyujr34019282 = -311144159;    float FkQrnfIjpoUSUTZuXkAyujr45186652 = -961295510;    float FkQrnfIjpoUSUTZuXkAyujr71975961 = -659103987;    float FkQrnfIjpoUSUTZuXkAyujr79601133 = 39681491;    float FkQrnfIjpoUSUTZuXkAyujr22051956 = -74223582;    float FkQrnfIjpoUSUTZuXkAyujr99710237 = -195009906;    float FkQrnfIjpoUSUTZuXkAyujr46908826 = -673121167;    float FkQrnfIjpoUSUTZuXkAyujr86040845 = -7728886;    float FkQrnfIjpoUSUTZuXkAyujr2062000 = -48137492;    float FkQrnfIjpoUSUTZuXkAyujr27451730 = -723364006;    float FkQrnfIjpoUSUTZuXkAyujr26829673 = -325877703;    float FkQrnfIjpoUSUTZuXkAyujr83909253 = -799244172;    float FkQrnfIjpoUSUTZuXkAyujr65024100 = -695431278;    float FkQrnfIjpoUSUTZuXkAyujr81776737 = -170042984;    float FkQrnfIjpoUSUTZuXkAyujr1260419 = -341000813;    float FkQrnfIjpoUSUTZuXkAyujr21259038 = 2236518;    float FkQrnfIjpoUSUTZuXkAyujr33836281 = -657770500;    float FkQrnfIjpoUSUTZuXkAyujr47983653 = -278918579;    float FkQrnfIjpoUSUTZuXkAyujr44400785 = -539016654;    float FkQrnfIjpoUSUTZuXkAyujr13155541 = 18857235;    float FkQrnfIjpoUSUTZuXkAyujr75728290 = -881067318;    float FkQrnfIjpoUSUTZuXkAyujr84005584 = 63598542;    float FkQrnfIjpoUSUTZuXkAyujr15670851 = -801522676;    float FkQrnfIjpoUSUTZuXkAyujr81223391 = -284358975;    float FkQrnfIjpoUSUTZuXkAyujr54499410 = -266035407;    float FkQrnfIjpoUSUTZuXkAyujr88500576 = -1130642;    float FkQrnfIjpoUSUTZuXkAyujr68999515 = -550684006;    float FkQrnfIjpoUSUTZuXkAyujr39773944 = -648763022;    float FkQrnfIjpoUSUTZuXkAyujr82652934 = -873126480;    float FkQrnfIjpoUSUTZuXkAyujr4063037 = -854654522;    float FkQrnfIjpoUSUTZuXkAyujr68863028 = 79525284;    float FkQrnfIjpoUSUTZuXkAyujr9418043 = 5983923;    float FkQrnfIjpoUSUTZuXkAyujr41684187 = -280067679;    float FkQrnfIjpoUSUTZuXkAyujr53684477 = -314510302;    float FkQrnfIjpoUSUTZuXkAyujr83366080 = -42693903;    float FkQrnfIjpoUSUTZuXkAyujr32026083 = -228638499;    float FkQrnfIjpoUSUTZuXkAyujr60602650 = -905314586;    float FkQrnfIjpoUSUTZuXkAyujr59045547 = -177175808;     FkQrnfIjpoUSUTZuXkAyujr43484408 = FkQrnfIjpoUSUTZuXkAyujr19914566;     FkQrnfIjpoUSUTZuXkAyujr19914566 = FkQrnfIjpoUSUTZuXkAyujr54290892;     FkQrnfIjpoUSUTZuXkAyujr54290892 = FkQrnfIjpoUSUTZuXkAyujr95719172;     FkQrnfIjpoUSUTZuXkAyujr95719172 = FkQrnfIjpoUSUTZuXkAyujr23119057;     FkQrnfIjpoUSUTZuXkAyujr23119057 = FkQrnfIjpoUSUTZuXkAyujr72908401;     FkQrnfIjpoUSUTZuXkAyujr72908401 = FkQrnfIjpoUSUTZuXkAyujr38374444;     FkQrnfIjpoUSUTZuXkAyujr38374444 = FkQrnfIjpoUSUTZuXkAyujr15362598;     FkQrnfIjpoUSUTZuXkAyujr15362598 = FkQrnfIjpoUSUTZuXkAyujr92714811;     FkQrnfIjpoUSUTZuXkAyujr92714811 = FkQrnfIjpoUSUTZuXkAyujr98187913;     FkQrnfIjpoUSUTZuXkAyujr98187913 = FkQrnfIjpoUSUTZuXkAyujr22858122;     FkQrnfIjpoUSUTZuXkAyujr22858122 = FkQrnfIjpoUSUTZuXkAyujr90434845;     FkQrnfIjpoUSUTZuXkAyujr90434845 = FkQrnfIjpoUSUTZuXkAyujr79887694;     FkQrnfIjpoUSUTZuXkAyujr79887694 = FkQrnfIjpoUSUTZuXkAyujr51694024;     FkQrnfIjpoUSUTZuXkAyujr51694024 = FkQrnfIjpoUSUTZuXkAyujr34770696;     FkQrnfIjpoUSUTZuXkAyujr34770696 = FkQrnfIjpoUSUTZuXkAyujr87932967;     FkQrnfIjpoUSUTZuXkAyujr87932967 = FkQrnfIjpoUSUTZuXkAyujr47196121;     FkQrnfIjpoUSUTZuXkAyujr47196121 = FkQrnfIjpoUSUTZuXkAyujr83466973;     FkQrnfIjpoUSUTZuXkAyujr83466973 = FkQrnfIjpoUSUTZuXkAyujr34898878;     FkQrnfIjpoUSUTZuXkAyujr34898878 = FkQrnfIjpoUSUTZuXkAyujr54825116;     FkQrnfIjpoUSUTZuXkAyujr54825116 = FkQrnfIjpoUSUTZuXkAyujr99162099;     FkQrnfIjpoUSUTZuXkAyujr99162099 = FkQrnfIjpoUSUTZuXkAyujr78473153;     FkQrnfIjpoUSUTZuXkAyujr78473153 = FkQrnfIjpoUSUTZuXkAyujr53837649;     FkQrnfIjpoUSUTZuXkAyujr53837649 = FkQrnfIjpoUSUTZuXkAyujr72482961;     FkQrnfIjpoUSUTZuXkAyujr72482961 = FkQrnfIjpoUSUTZuXkAyujr44837808;     FkQrnfIjpoUSUTZuXkAyujr44837808 = FkQrnfIjpoUSUTZuXkAyujr75899798;     FkQrnfIjpoUSUTZuXkAyujr75899798 = FkQrnfIjpoUSUTZuXkAyujr63602964;     FkQrnfIjpoUSUTZuXkAyujr63602964 = FkQrnfIjpoUSUTZuXkAyujr2257907;     FkQrnfIjpoUSUTZuXkAyujr2257907 = FkQrnfIjpoUSUTZuXkAyujr81343315;     FkQrnfIjpoUSUTZuXkAyujr81343315 = FkQrnfIjpoUSUTZuXkAyujr47528160;     FkQrnfIjpoUSUTZuXkAyujr47528160 = FkQrnfIjpoUSUTZuXkAyujr26211953;     FkQrnfIjpoUSUTZuXkAyujr26211953 = FkQrnfIjpoUSUTZuXkAyujr43256988;     FkQrnfIjpoUSUTZuXkAyujr43256988 = FkQrnfIjpoUSUTZuXkAyujr68382889;     FkQrnfIjpoUSUTZuXkAyujr68382889 = FkQrnfIjpoUSUTZuXkAyujr80177456;     FkQrnfIjpoUSUTZuXkAyujr80177456 = FkQrnfIjpoUSUTZuXkAyujr4785199;     FkQrnfIjpoUSUTZuXkAyujr4785199 = FkQrnfIjpoUSUTZuXkAyujr48729851;     FkQrnfIjpoUSUTZuXkAyujr48729851 = FkQrnfIjpoUSUTZuXkAyujr85870967;     FkQrnfIjpoUSUTZuXkAyujr85870967 = FkQrnfIjpoUSUTZuXkAyujr19744392;     FkQrnfIjpoUSUTZuXkAyujr19744392 = FkQrnfIjpoUSUTZuXkAyujr56637301;     FkQrnfIjpoUSUTZuXkAyujr56637301 = FkQrnfIjpoUSUTZuXkAyujr50989624;     FkQrnfIjpoUSUTZuXkAyujr50989624 = FkQrnfIjpoUSUTZuXkAyujr89801015;     FkQrnfIjpoUSUTZuXkAyujr89801015 = FkQrnfIjpoUSUTZuXkAyujr17385362;     FkQrnfIjpoUSUTZuXkAyujr17385362 = FkQrnfIjpoUSUTZuXkAyujr77212734;     FkQrnfIjpoUSUTZuXkAyujr77212734 = FkQrnfIjpoUSUTZuXkAyujr32578612;     FkQrnfIjpoUSUTZuXkAyujr32578612 = FkQrnfIjpoUSUTZuXkAyujr38646681;     FkQrnfIjpoUSUTZuXkAyujr38646681 = FkQrnfIjpoUSUTZuXkAyujr96854155;     FkQrnfIjpoUSUTZuXkAyujr96854155 = FkQrnfIjpoUSUTZuXkAyujr31499013;     FkQrnfIjpoUSUTZuXkAyujr31499013 = FkQrnfIjpoUSUTZuXkAyujr50447424;     FkQrnfIjpoUSUTZuXkAyujr50447424 = FkQrnfIjpoUSUTZuXkAyujr26529616;     FkQrnfIjpoUSUTZuXkAyujr26529616 = FkQrnfIjpoUSUTZuXkAyujr97337731;     FkQrnfIjpoUSUTZuXkAyujr97337731 = FkQrnfIjpoUSUTZuXkAyujr31857309;     FkQrnfIjpoUSUTZuXkAyujr31857309 = FkQrnfIjpoUSUTZuXkAyujr44988561;     FkQrnfIjpoUSUTZuXkAyujr44988561 = FkQrnfIjpoUSUTZuXkAyujr88757578;     FkQrnfIjpoUSUTZuXkAyujr88757578 = FkQrnfIjpoUSUTZuXkAyujr79882313;     FkQrnfIjpoUSUTZuXkAyujr79882313 = FkQrnfIjpoUSUTZuXkAyujr11177941;     FkQrnfIjpoUSUTZuXkAyujr11177941 = FkQrnfIjpoUSUTZuXkAyujr65011254;     FkQrnfIjpoUSUTZuXkAyujr65011254 = FkQrnfIjpoUSUTZuXkAyujr66076917;     FkQrnfIjpoUSUTZuXkAyujr66076917 = FkQrnfIjpoUSUTZuXkAyujr81807931;     FkQrnfIjpoUSUTZuXkAyujr81807931 = FkQrnfIjpoUSUTZuXkAyujr50881364;     FkQrnfIjpoUSUTZuXkAyujr50881364 = FkQrnfIjpoUSUTZuXkAyujr47219259;     FkQrnfIjpoUSUTZuXkAyujr47219259 = FkQrnfIjpoUSUTZuXkAyujr9305438;     FkQrnfIjpoUSUTZuXkAyujr9305438 = FkQrnfIjpoUSUTZuXkAyujr36116538;     FkQrnfIjpoUSUTZuXkAyujr36116538 = FkQrnfIjpoUSUTZuXkAyujr34019282;     FkQrnfIjpoUSUTZuXkAyujr34019282 = FkQrnfIjpoUSUTZuXkAyujr45186652;     FkQrnfIjpoUSUTZuXkAyujr45186652 = FkQrnfIjpoUSUTZuXkAyujr71975961;     FkQrnfIjpoUSUTZuXkAyujr71975961 = FkQrnfIjpoUSUTZuXkAyujr79601133;     FkQrnfIjpoUSUTZuXkAyujr79601133 = FkQrnfIjpoUSUTZuXkAyujr22051956;     FkQrnfIjpoUSUTZuXkAyujr22051956 = FkQrnfIjpoUSUTZuXkAyujr99710237;     FkQrnfIjpoUSUTZuXkAyujr99710237 = FkQrnfIjpoUSUTZuXkAyujr46908826;     FkQrnfIjpoUSUTZuXkAyujr46908826 = FkQrnfIjpoUSUTZuXkAyujr86040845;     FkQrnfIjpoUSUTZuXkAyujr86040845 = FkQrnfIjpoUSUTZuXkAyujr2062000;     FkQrnfIjpoUSUTZuXkAyujr2062000 = FkQrnfIjpoUSUTZuXkAyujr27451730;     FkQrnfIjpoUSUTZuXkAyujr27451730 = FkQrnfIjpoUSUTZuXkAyujr26829673;     FkQrnfIjpoUSUTZuXkAyujr26829673 = FkQrnfIjpoUSUTZuXkAyujr83909253;     FkQrnfIjpoUSUTZuXkAyujr83909253 = FkQrnfIjpoUSUTZuXkAyujr65024100;     FkQrnfIjpoUSUTZuXkAyujr65024100 = FkQrnfIjpoUSUTZuXkAyujr81776737;     FkQrnfIjpoUSUTZuXkAyujr81776737 = FkQrnfIjpoUSUTZuXkAyujr1260419;     FkQrnfIjpoUSUTZuXkAyujr1260419 = FkQrnfIjpoUSUTZuXkAyujr21259038;     FkQrnfIjpoUSUTZuXkAyujr21259038 = FkQrnfIjpoUSUTZuXkAyujr33836281;     FkQrnfIjpoUSUTZuXkAyujr33836281 = FkQrnfIjpoUSUTZuXkAyujr47983653;     FkQrnfIjpoUSUTZuXkAyujr47983653 = FkQrnfIjpoUSUTZuXkAyujr44400785;     FkQrnfIjpoUSUTZuXkAyujr44400785 = FkQrnfIjpoUSUTZuXkAyujr13155541;     FkQrnfIjpoUSUTZuXkAyujr13155541 = FkQrnfIjpoUSUTZuXkAyujr75728290;     FkQrnfIjpoUSUTZuXkAyujr75728290 = FkQrnfIjpoUSUTZuXkAyujr84005584;     FkQrnfIjpoUSUTZuXkAyujr84005584 = FkQrnfIjpoUSUTZuXkAyujr15670851;     FkQrnfIjpoUSUTZuXkAyujr15670851 = FkQrnfIjpoUSUTZuXkAyujr81223391;     FkQrnfIjpoUSUTZuXkAyujr81223391 = FkQrnfIjpoUSUTZuXkAyujr54499410;     FkQrnfIjpoUSUTZuXkAyujr54499410 = FkQrnfIjpoUSUTZuXkAyujr88500576;     FkQrnfIjpoUSUTZuXkAyujr88500576 = FkQrnfIjpoUSUTZuXkAyujr68999515;     FkQrnfIjpoUSUTZuXkAyujr68999515 = FkQrnfIjpoUSUTZuXkAyujr39773944;     FkQrnfIjpoUSUTZuXkAyujr39773944 = FkQrnfIjpoUSUTZuXkAyujr82652934;     FkQrnfIjpoUSUTZuXkAyujr82652934 = FkQrnfIjpoUSUTZuXkAyujr4063037;     FkQrnfIjpoUSUTZuXkAyujr4063037 = FkQrnfIjpoUSUTZuXkAyujr68863028;     FkQrnfIjpoUSUTZuXkAyujr68863028 = FkQrnfIjpoUSUTZuXkAyujr9418043;     FkQrnfIjpoUSUTZuXkAyujr9418043 = FkQrnfIjpoUSUTZuXkAyujr41684187;     FkQrnfIjpoUSUTZuXkAyujr41684187 = FkQrnfIjpoUSUTZuXkAyujr53684477;     FkQrnfIjpoUSUTZuXkAyujr53684477 = FkQrnfIjpoUSUTZuXkAyujr83366080;     FkQrnfIjpoUSUTZuXkAyujr83366080 = FkQrnfIjpoUSUTZuXkAyujr32026083;     FkQrnfIjpoUSUTZuXkAyujr32026083 = FkQrnfIjpoUSUTZuXkAyujr60602650;     FkQrnfIjpoUSUTZuXkAyujr60602650 = FkQrnfIjpoUSUTZuXkAyujr59045547;     FkQrnfIjpoUSUTZuXkAyujr59045547 = FkQrnfIjpoUSUTZuXkAyujr43484408;}



void omVzCCnHRlRRgYIqCbQrdEfmdD95402878() {     float yKyjdbnEoVWoiYdYzYVUWWD27207926 = -527197876;    float yKyjdbnEoVWoiYdYzYVUWWD17512661 = -453975323;    float yKyjdbnEoVWoiYdYzYVUWWD42877009 = -829383110;    float yKyjdbnEoVWoiYdYzYVUWWD92867125 = -559505461;    float yKyjdbnEoVWoiYdYzYVUWWD2680394 = -349915143;    float yKyjdbnEoVWoiYdYzYVUWWD84247919 = 45140707;    float yKyjdbnEoVWoiYdYzYVUWWD75255007 = -821766368;    float yKyjdbnEoVWoiYdYzYVUWWD58702681 = -467123015;    float yKyjdbnEoVWoiYdYzYVUWWD42934340 = -434042353;    float yKyjdbnEoVWoiYdYzYVUWWD44293502 = -899627773;    float yKyjdbnEoVWoiYdYzYVUWWD65747157 = -139026070;    float yKyjdbnEoVWoiYdYzYVUWWD80646616 = -436142599;    float yKyjdbnEoVWoiYdYzYVUWWD30941547 = -315144088;    float yKyjdbnEoVWoiYdYzYVUWWD46067168 = -217808039;    float yKyjdbnEoVWoiYdYzYVUWWD38949955 = -894694090;    float yKyjdbnEoVWoiYdYzYVUWWD59746996 = -966478782;    float yKyjdbnEoVWoiYdYzYVUWWD47061532 = -494609140;    float yKyjdbnEoVWoiYdYzYVUWWD97544242 = -60700899;    float yKyjdbnEoVWoiYdYzYVUWWD75875358 = -463975429;    float yKyjdbnEoVWoiYdYzYVUWWD28932761 = -718014657;    float yKyjdbnEoVWoiYdYzYVUWWD66501609 = -298347565;    float yKyjdbnEoVWoiYdYzYVUWWD79475224 = -544090107;    float yKyjdbnEoVWoiYdYzYVUWWD23013514 = -691576791;    float yKyjdbnEoVWoiYdYzYVUWWD73625656 = -460748956;    float yKyjdbnEoVWoiYdYzYVUWWD81439901 = 58936262;    float yKyjdbnEoVWoiYdYzYVUWWD28892556 = -364978435;    float yKyjdbnEoVWoiYdYzYVUWWD85194903 = -799399707;    float yKyjdbnEoVWoiYdYzYVUWWD44857106 = -594913558;    float yKyjdbnEoVWoiYdYzYVUWWD53261457 = 40279829;    float yKyjdbnEoVWoiYdYzYVUWWD83076487 = -447870522;    float yKyjdbnEoVWoiYdYzYVUWWD7576376 = -136580792;    float yKyjdbnEoVWoiYdYzYVUWWD68050276 = -787619527;    float yKyjdbnEoVWoiYdYzYVUWWD82507591 = -997993838;    float yKyjdbnEoVWoiYdYzYVUWWD15902171 = -581398587;    float yKyjdbnEoVWoiYdYzYVUWWD72388523 = 70705383;    float yKyjdbnEoVWoiYdYzYVUWWD18591085 = -839664376;    float yKyjdbnEoVWoiYdYzYVUWWD85634558 = -114554935;    float yKyjdbnEoVWoiYdYzYVUWWD70126183 = -511508488;    float yKyjdbnEoVWoiYdYzYVUWWD17253020 = -329530504;    float yKyjdbnEoVWoiYdYzYVUWWD90178740 = -822728899;    float yKyjdbnEoVWoiYdYzYVUWWD54568536 = 43669412;    float yKyjdbnEoVWoiYdYzYVUWWD46725089 = -59863731;    float yKyjdbnEoVWoiYdYzYVUWWD10012881 = -942338263;    float yKyjdbnEoVWoiYdYzYVUWWD79774131 = -143362305;    float yKyjdbnEoVWoiYdYzYVUWWD66633931 = -489759882;    float yKyjdbnEoVWoiYdYzYVUWWD41121037 = -198846089;    float yKyjdbnEoVWoiYdYzYVUWWD64677983 = -404640660;    float yKyjdbnEoVWoiYdYzYVUWWD89189271 = -36503417;    float yKyjdbnEoVWoiYdYzYVUWWD60403829 = -627188903;    float yKyjdbnEoVWoiYdYzYVUWWD33211062 = -31138249;    float yKyjdbnEoVWoiYdYzYVUWWD71948140 = -139513873;    float yKyjdbnEoVWoiYdYzYVUWWD96494280 = -385176583;    float yKyjdbnEoVWoiYdYzYVUWWD34139981 = -984319264;    float yKyjdbnEoVWoiYdYzYVUWWD25814920 = -808358292;    float yKyjdbnEoVWoiYdYzYVUWWD28901269 = -943344878;    float yKyjdbnEoVWoiYdYzYVUWWD47732701 = -983107769;    float yKyjdbnEoVWoiYdYzYVUWWD94499146 = -762398533;    float yKyjdbnEoVWoiYdYzYVUWWD69251353 = -268634155;    float yKyjdbnEoVWoiYdYzYVUWWD11427225 = -518441723;    float yKyjdbnEoVWoiYdYzYVUWWD73787837 = -984936709;    float yKyjdbnEoVWoiYdYzYVUWWD99053015 = -155459587;    float yKyjdbnEoVWoiYdYzYVUWWD30397901 = -126852810;    float yKyjdbnEoVWoiYdYzYVUWWD5441224 = -407402845;    float yKyjdbnEoVWoiYdYzYVUWWD59857853 = -986171832;    float yKyjdbnEoVWoiYdYzYVUWWD36717126 = -663046982;    float yKyjdbnEoVWoiYdYzYVUWWD97696880 = -351406544;    float yKyjdbnEoVWoiYdYzYVUWWD98139025 = -438148761;    float yKyjdbnEoVWoiYdYzYVUWWD15039376 = -733745502;    float yKyjdbnEoVWoiYdYzYVUWWD73678645 = -188513422;    float yKyjdbnEoVWoiYdYzYVUWWD20358871 = 44970286;    float yKyjdbnEoVWoiYdYzYVUWWD74112438 = -751923847;    float yKyjdbnEoVWoiYdYzYVUWWD76935349 = -983100652;    float yKyjdbnEoVWoiYdYzYVUWWD80291223 = -731170396;    float yKyjdbnEoVWoiYdYzYVUWWD85696618 = -641246531;    float yKyjdbnEoVWoiYdYzYVUWWD74364224 = -661684069;    float yKyjdbnEoVWoiYdYzYVUWWD19776521 = -138483835;    float yKyjdbnEoVWoiYdYzYVUWWD69462344 = -601751845;    float yKyjdbnEoVWoiYdYzYVUWWD43239383 = -448214486;    float yKyjdbnEoVWoiYdYzYVUWWD6991725 = -970989075;    float yKyjdbnEoVWoiYdYzYVUWWD40318864 = -742217649;    float yKyjdbnEoVWoiYdYzYVUWWD64214573 = -960337775;    float yKyjdbnEoVWoiYdYzYVUWWD96005632 = -662896290;    float yKyjdbnEoVWoiYdYzYVUWWD84453277 = -967724655;    float yKyjdbnEoVWoiYdYzYVUWWD20050396 = -928581923;    float yKyjdbnEoVWoiYdYzYVUWWD11128347 = -208356650;    float yKyjdbnEoVWoiYdYzYVUWWD11082095 = -751404210;    float yKyjdbnEoVWoiYdYzYVUWWD33910296 = -803300264;    float yKyjdbnEoVWoiYdYzYVUWWD56692671 = -89635546;    float yKyjdbnEoVWoiYdYzYVUWWD87000902 = -638053709;    float yKyjdbnEoVWoiYdYzYVUWWD24655823 = 53813152;    float yKyjdbnEoVWoiYdYzYVUWWD24091938 = 22734157;    float yKyjdbnEoVWoiYdYzYVUWWD16383205 = -845920781;    float yKyjdbnEoVWoiYdYzYVUWWD58698959 = -993066765;    float yKyjdbnEoVWoiYdYzYVUWWD43465183 = -344593795;    float yKyjdbnEoVWoiYdYzYVUWWD91125724 = -567269313;    float yKyjdbnEoVWoiYdYzYVUWWD24170636 = -829477779;    float yKyjdbnEoVWoiYdYzYVUWWD41283865 = -652460886;    float yKyjdbnEoVWoiYdYzYVUWWD50155028 = -956166431;    float yKyjdbnEoVWoiYdYzYVUWWD43057005 = -480315324;    float yKyjdbnEoVWoiYdYzYVUWWD68937050 = -527197876;     yKyjdbnEoVWoiYdYzYVUWWD27207926 = yKyjdbnEoVWoiYdYzYVUWWD17512661;     yKyjdbnEoVWoiYdYzYVUWWD17512661 = yKyjdbnEoVWoiYdYzYVUWWD42877009;     yKyjdbnEoVWoiYdYzYVUWWD42877009 = yKyjdbnEoVWoiYdYzYVUWWD92867125;     yKyjdbnEoVWoiYdYzYVUWWD92867125 = yKyjdbnEoVWoiYdYzYVUWWD2680394;     yKyjdbnEoVWoiYdYzYVUWWD2680394 = yKyjdbnEoVWoiYdYzYVUWWD84247919;     yKyjdbnEoVWoiYdYzYVUWWD84247919 = yKyjdbnEoVWoiYdYzYVUWWD75255007;     yKyjdbnEoVWoiYdYzYVUWWD75255007 = yKyjdbnEoVWoiYdYzYVUWWD58702681;     yKyjdbnEoVWoiYdYzYVUWWD58702681 = yKyjdbnEoVWoiYdYzYVUWWD42934340;     yKyjdbnEoVWoiYdYzYVUWWD42934340 = yKyjdbnEoVWoiYdYzYVUWWD44293502;     yKyjdbnEoVWoiYdYzYVUWWD44293502 = yKyjdbnEoVWoiYdYzYVUWWD65747157;     yKyjdbnEoVWoiYdYzYVUWWD65747157 = yKyjdbnEoVWoiYdYzYVUWWD80646616;     yKyjdbnEoVWoiYdYzYVUWWD80646616 = yKyjdbnEoVWoiYdYzYVUWWD30941547;     yKyjdbnEoVWoiYdYzYVUWWD30941547 = yKyjdbnEoVWoiYdYzYVUWWD46067168;     yKyjdbnEoVWoiYdYzYVUWWD46067168 = yKyjdbnEoVWoiYdYzYVUWWD38949955;     yKyjdbnEoVWoiYdYzYVUWWD38949955 = yKyjdbnEoVWoiYdYzYVUWWD59746996;     yKyjdbnEoVWoiYdYzYVUWWD59746996 = yKyjdbnEoVWoiYdYzYVUWWD47061532;     yKyjdbnEoVWoiYdYzYVUWWD47061532 = yKyjdbnEoVWoiYdYzYVUWWD97544242;     yKyjdbnEoVWoiYdYzYVUWWD97544242 = yKyjdbnEoVWoiYdYzYVUWWD75875358;     yKyjdbnEoVWoiYdYzYVUWWD75875358 = yKyjdbnEoVWoiYdYzYVUWWD28932761;     yKyjdbnEoVWoiYdYzYVUWWD28932761 = yKyjdbnEoVWoiYdYzYVUWWD66501609;     yKyjdbnEoVWoiYdYzYVUWWD66501609 = yKyjdbnEoVWoiYdYzYVUWWD79475224;     yKyjdbnEoVWoiYdYzYVUWWD79475224 = yKyjdbnEoVWoiYdYzYVUWWD23013514;     yKyjdbnEoVWoiYdYzYVUWWD23013514 = yKyjdbnEoVWoiYdYzYVUWWD73625656;     yKyjdbnEoVWoiYdYzYVUWWD73625656 = yKyjdbnEoVWoiYdYzYVUWWD81439901;     yKyjdbnEoVWoiYdYzYVUWWD81439901 = yKyjdbnEoVWoiYdYzYVUWWD28892556;     yKyjdbnEoVWoiYdYzYVUWWD28892556 = yKyjdbnEoVWoiYdYzYVUWWD85194903;     yKyjdbnEoVWoiYdYzYVUWWD85194903 = yKyjdbnEoVWoiYdYzYVUWWD44857106;     yKyjdbnEoVWoiYdYzYVUWWD44857106 = yKyjdbnEoVWoiYdYzYVUWWD53261457;     yKyjdbnEoVWoiYdYzYVUWWD53261457 = yKyjdbnEoVWoiYdYzYVUWWD83076487;     yKyjdbnEoVWoiYdYzYVUWWD83076487 = yKyjdbnEoVWoiYdYzYVUWWD7576376;     yKyjdbnEoVWoiYdYzYVUWWD7576376 = yKyjdbnEoVWoiYdYzYVUWWD68050276;     yKyjdbnEoVWoiYdYzYVUWWD68050276 = yKyjdbnEoVWoiYdYzYVUWWD82507591;     yKyjdbnEoVWoiYdYzYVUWWD82507591 = yKyjdbnEoVWoiYdYzYVUWWD15902171;     yKyjdbnEoVWoiYdYzYVUWWD15902171 = yKyjdbnEoVWoiYdYzYVUWWD72388523;     yKyjdbnEoVWoiYdYzYVUWWD72388523 = yKyjdbnEoVWoiYdYzYVUWWD18591085;     yKyjdbnEoVWoiYdYzYVUWWD18591085 = yKyjdbnEoVWoiYdYzYVUWWD85634558;     yKyjdbnEoVWoiYdYzYVUWWD85634558 = yKyjdbnEoVWoiYdYzYVUWWD70126183;     yKyjdbnEoVWoiYdYzYVUWWD70126183 = yKyjdbnEoVWoiYdYzYVUWWD17253020;     yKyjdbnEoVWoiYdYzYVUWWD17253020 = yKyjdbnEoVWoiYdYzYVUWWD90178740;     yKyjdbnEoVWoiYdYzYVUWWD90178740 = yKyjdbnEoVWoiYdYzYVUWWD54568536;     yKyjdbnEoVWoiYdYzYVUWWD54568536 = yKyjdbnEoVWoiYdYzYVUWWD46725089;     yKyjdbnEoVWoiYdYzYVUWWD46725089 = yKyjdbnEoVWoiYdYzYVUWWD10012881;     yKyjdbnEoVWoiYdYzYVUWWD10012881 = yKyjdbnEoVWoiYdYzYVUWWD79774131;     yKyjdbnEoVWoiYdYzYVUWWD79774131 = yKyjdbnEoVWoiYdYzYVUWWD66633931;     yKyjdbnEoVWoiYdYzYVUWWD66633931 = yKyjdbnEoVWoiYdYzYVUWWD41121037;     yKyjdbnEoVWoiYdYzYVUWWD41121037 = yKyjdbnEoVWoiYdYzYVUWWD64677983;     yKyjdbnEoVWoiYdYzYVUWWD64677983 = yKyjdbnEoVWoiYdYzYVUWWD89189271;     yKyjdbnEoVWoiYdYzYVUWWD89189271 = yKyjdbnEoVWoiYdYzYVUWWD60403829;     yKyjdbnEoVWoiYdYzYVUWWD60403829 = yKyjdbnEoVWoiYdYzYVUWWD33211062;     yKyjdbnEoVWoiYdYzYVUWWD33211062 = yKyjdbnEoVWoiYdYzYVUWWD71948140;     yKyjdbnEoVWoiYdYzYVUWWD71948140 = yKyjdbnEoVWoiYdYzYVUWWD96494280;     yKyjdbnEoVWoiYdYzYVUWWD96494280 = yKyjdbnEoVWoiYdYzYVUWWD34139981;     yKyjdbnEoVWoiYdYzYVUWWD34139981 = yKyjdbnEoVWoiYdYzYVUWWD25814920;     yKyjdbnEoVWoiYdYzYVUWWD25814920 = yKyjdbnEoVWoiYdYzYVUWWD28901269;     yKyjdbnEoVWoiYdYzYVUWWD28901269 = yKyjdbnEoVWoiYdYzYVUWWD47732701;     yKyjdbnEoVWoiYdYzYVUWWD47732701 = yKyjdbnEoVWoiYdYzYVUWWD94499146;     yKyjdbnEoVWoiYdYzYVUWWD94499146 = yKyjdbnEoVWoiYdYzYVUWWD69251353;     yKyjdbnEoVWoiYdYzYVUWWD69251353 = yKyjdbnEoVWoiYdYzYVUWWD11427225;     yKyjdbnEoVWoiYdYzYVUWWD11427225 = yKyjdbnEoVWoiYdYzYVUWWD73787837;     yKyjdbnEoVWoiYdYzYVUWWD73787837 = yKyjdbnEoVWoiYdYzYVUWWD99053015;     yKyjdbnEoVWoiYdYzYVUWWD99053015 = yKyjdbnEoVWoiYdYzYVUWWD30397901;     yKyjdbnEoVWoiYdYzYVUWWD30397901 = yKyjdbnEoVWoiYdYzYVUWWD5441224;     yKyjdbnEoVWoiYdYzYVUWWD5441224 = yKyjdbnEoVWoiYdYzYVUWWD59857853;     yKyjdbnEoVWoiYdYzYVUWWD59857853 = yKyjdbnEoVWoiYdYzYVUWWD36717126;     yKyjdbnEoVWoiYdYzYVUWWD36717126 = yKyjdbnEoVWoiYdYzYVUWWD97696880;     yKyjdbnEoVWoiYdYzYVUWWD97696880 = yKyjdbnEoVWoiYdYzYVUWWD98139025;     yKyjdbnEoVWoiYdYzYVUWWD98139025 = yKyjdbnEoVWoiYdYzYVUWWD15039376;     yKyjdbnEoVWoiYdYzYVUWWD15039376 = yKyjdbnEoVWoiYdYzYVUWWD73678645;     yKyjdbnEoVWoiYdYzYVUWWD73678645 = yKyjdbnEoVWoiYdYzYVUWWD20358871;     yKyjdbnEoVWoiYdYzYVUWWD20358871 = yKyjdbnEoVWoiYdYzYVUWWD74112438;     yKyjdbnEoVWoiYdYzYVUWWD74112438 = yKyjdbnEoVWoiYdYzYVUWWD76935349;     yKyjdbnEoVWoiYdYzYVUWWD76935349 = yKyjdbnEoVWoiYdYzYVUWWD80291223;     yKyjdbnEoVWoiYdYzYVUWWD80291223 = yKyjdbnEoVWoiYdYzYVUWWD85696618;     yKyjdbnEoVWoiYdYzYVUWWD85696618 = yKyjdbnEoVWoiYdYzYVUWWD74364224;     yKyjdbnEoVWoiYdYzYVUWWD74364224 = yKyjdbnEoVWoiYdYzYVUWWD19776521;     yKyjdbnEoVWoiYdYzYVUWWD19776521 = yKyjdbnEoVWoiYdYzYVUWWD69462344;     yKyjdbnEoVWoiYdYzYVUWWD69462344 = yKyjdbnEoVWoiYdYzYVUWWD43239383;     yKyjdbnEoVWoiYdYzYVUWWD43239383 = yKyjdbnEoVWoiYdYzYVUWWD6991725;     yKyjdbnEoVWoiYdYzYVUWWD6991725 = yKyjdbnEoVWoiYdYzYVUWWD40318864;     yKyjdbnEoVWoiYdYzYVUWWD40318864 = yKyjdbnEoVWoiYdYzYVUWWD64214573;     yKyjdbnEoVWoiYdYzYVUWWD64214573 = yKyjdbnEoVWoiYdYzYVUWWD96005632;     yKyjdbnEoVWoiYdYzYVUWWD96005632 = yKyjdbnEoVWoiYdYzYVUWWD84453277;     yKyjdbnEoVWoiYdYzYVUWWD84453277 = yKyjdbnEoVWoiYdYzYVUWWD20050396;     yKyjdbnEoVWoiYdYzYVUWWD20050396 = yKyjdbnEoVWoiYdYzYVUWWD11128347;     yKyjdbnEoVWoiYdYzYVUWWD11128347 = yKyjdbnEoVWoiYdYzYVUWWD11082095;     yKyjdbnEoVWoiYdYzYVUWWD11082095 = yKyjdbnEoVWoiYdYzYVUWWD33910296;     yKyjdbnEoVWoiYdYzYVUWWD33910296 = yKyjdbnEoVWoiYdYzYVUWWD56692671;     yKyjdbnEoVWoiYdYzYVUWWD56692671 = yKyjdbnEoVWoiYdYzYVUWWD87000902;     yKyjdbnEoVWoiYdYzYVUWWD87000902 = yKyjdbnEoVWoiYdYzYVUWWD24655823;     yKyjdbnEoVWoiYdYzYVUWWD24655823 = yKyjdbnEoVWoiYdYzYVUWWD24091938;     yKyjdbnEoVWoiYdYzYVUWWD24091938 = yKyjdbnEoVWoiYdYzYVUWWD16383205;     yKyjdbnEoVWoiYdYzYVUWWD16383205 = yKyjdbnEoVWoiYdYzYVUWWD58698959;     yKyjdbnEoVWoiYdYzYVUWWD58698959 = yKyjdbnEoVWoiYdYzYVUWWD43465183;     yKyjdbnEoVWoiYdYzYVUWWD43465183 = yKyjdbnEoVWoiYdYzYVUWWD91125724;     yKyjdbnEoVWoiYdYzYVUWWD91125724 = yKyjdbnEoVWoiYdYzYVUWWD24170636;     yKyjdbnEoVWoiYdYzYVUWWD24170636 = yKyjdbnEoVWoiYdYzYVUWWD41283865;     yKyjdbnEoVWoiYdYzYVUWWD41283865 = yKyjdbnEoVWoiYdYzYVUWWD50155028;     yKyjdbnEoVWoiYdYzYVUWWD50155028 = yKyjdbnEoVWoiYdYzYVUWWD43057005;     yKyjdbnEoVWoiYdYzYVUWWD43057005 = yKyjdbnEoVWoiYdYzYVUWWD68937050;     yKyjdbnEoVWoiYdYzYVUWWD68937050 = yKyjdbnEoVWoiYdYzYVUWWD27207926;}



void GSzwWaRFjzAjrZkSSkifWbOhVF80465716() {     float ZWRnytFNHTXEeznZEEsuWLS10931444 = -877219943;    float ZWRnytFNHTXEeznZEEsuWLS15110757 = -257627175;    float ZWRnytFNHTXEeznZEEsuWLS31463127 = -697690801;    float ZWRnytFNHTXEeznZEEsuWLS90015078 = -564387390;    float ZWRnytFNHTXEeznZEEsuWLS82241729 = -747863335;    float ZWRnytFNHTXEeznZEEsuWLS95587436 = -58179962;    float ZWRnytFNHTXEeznZEEsuWLS12135571 = -843280506;    float ZWRnytFNHTXEeznZEEsuWLS2042764 = 12422398;    float ZWRnytFNHTXEeznZEEsuWLS93153868 = -31525050;    float ZWRnytFNHTXEeznZEEsuWLS90399090 = -22668789;    float ZWRnytFNHTXEeznZEEsuWLS8636194 = 20688672;    float ZWRnytFNHTXEeznZEEsuWLS70858387 = -365281870;    float ZWRnytFNHTXEeznZEEsuWLS81995398 = -157976331;    float ZWRnytFNHTXEeznZEEsuWLS40440313 = -567490901;    float ZWRnytFNHTXEeznZEEsuWLS43129214 = -820391395;    float ZWRnytFNHTXEeznZEEsuWLS31561025 = -890606434;    float ZWRnytFNHTXEeznZEEsuWLS46926943 = -653489002;    float ZWRnytFNHTXEeznZEEsuWLS11621513 = -771552473;    float ZWRnytFNHTXEeznZEEsuWLS16851840 = -252624667;    float ZWRnytFNHTXEeznZEEsuWLS3040406 = -334307574;    float ZWRnytFNHTXEeznZEEsuWLS33841120 = -972814086;    float ZWRnytFNHTXEeznZEEsuWLS80477295 = -457245393;    float ZWRnytFNHTXEeznZEEsuWLS92189379 = -720971527;    float ZWRnytFNHTXEeznZEEsuWLS74768351 = 18391;    float ZWRnytFNHTXEeznZEEsuWLS18041994 = -19394499;    float ZWRnytFNHTXEeznZEEsuWLS81885313 = -807945462;    float ZWRnytFNHTXEeznZEEsuWLS6786843 = -143275130;    float ZWRnytFNHTXEeznZEEsuWLS87456306 = -381355046;    float ZWRnytFNHTXEeznZEEsuWLS25179599 = -483916071;    float ZWRnytFNHTXEeznZEEsuWLS18624814 = -20476896;    float ZWRnytFNHTXEeznZEEsuWLS88940798 = -355678813;    float ZWRnytFNHTXEeznZEEsuWLS92843564 = -236816750;    float ZWRnytFNHTXEeznZEEsuWLS96632292 = -563207929;    float ZWRnytFNHTXEeznZEEsuWLS51626885 = -985495234;    float ZWRnytFNHTXEeznZEEsuWLS39991849 = -763585225;    float ZWRnytFNHTXEeznZEEsuWLS88452318 = -818060853;    float ZWRnytFNHTXEeznZEEsuWLS85398148 = -434896232;    float ZWRnytFNHTXEeznZEEsuWLS20507975 = -410651705;    float ZWRnytFNHTXEeznZEEsuWLS77868739 = -635089385;    float ZWRnytFNHTXEeznZEEsuWLS29367856 = -769375778;    float ZWRnytFNHTXEeznZEEsuWLS19336057 = -706370715;    float ZWRnytFNHTXEeznZEEsuWLS76064815 = -765889400;    float ZWRnytFNHTXEeznZEEsuWLS42813027 = -594742517;    float ZWRnytFNHTXEeznZEEsuWLS26969651 = -822306038;    float ZWRnytFNHTXEeznZEEsuWLS94621182 = -815773960;    float ZWRnytFNHTXEeznZEEsuWLS85387918 = -913877781;    float ZWRnytFNHTXEeznZEEsuWLS97856952 = -426286567;    float ZWRnytFNHTXEeznZEEsuWLS27931119 = -898625315;    float ZWRnytFNHTXEeznZEEsuWLS94278042 = -326973054;    float ZWRnytFNHTXEeznZEEsuWLS69084391 = -663153684;    float ZWRnytFNHTXEeznZEEsuWLS12038973 = -305286274;    float ZWRnytFNHTXEeznZEEsuWLS48000000 = -137229369;    float ZWRnytFNHTXEeznZEEsuWLS79522383 = -996251629;    float ZWRnytFNHTXEeznZEEsuWLS71747525 = -285067480;    float ZWRnytFNHTXEeznZEEsuWLS46624596 = -160071823;    float ZWRnytFNHTXEeznZEEsuWLS30454148 = -319974550;    float ZWRnytFNHTXEeznZEEsuWLS22921377 = -536655648;    float ZWRnytFNHTXEeznZEEsuWLS56694775 = -597709192;    float ZWRnytFNHTXEeznZEEsuWLS71973085 = -444992891;    float ZWRnytFNHTXEeznZEEsuWLS356417 = -939917873;    float ZWRnytFNHTXEeznZEEsuWLS88800594 = -914904832;    float ZWRnytFNHTXEeznZEEsuWLS24679264 = -361925460;    float ZWRnytFNHTXEeznZEEsuWLS76863164 = -503661531;    float ZWRnytFNHTXEeznZEEsuWLS74529054 = 88951846;    float ZWRnytFNHTXEeznZEEsuWLS1458292 = -666989977;    float ZWRnytFNHTXEeznZEEsuWLS15792629 = -742494579;    float ZWRnytFNHTXEeznZEEsuWLS74226094 = -802073941;    float ZWRnytFNHTXEeznZEEsuWLS30368514 = -172481098;    float ZWRnytFNHTXEeznZEEsuWLS448464 = -803905677;    float ZWRnytFNHTXEeznZEEsuWLS54676896 = 97669458;    float ZWRnytFNHTXEeznZEEsuWLS46162876 = -355710202;    float ZWRnytFNHTXEeznZEEsuWLS26418969 = -142837298;    float ZWRnytFNHTXEeznZEEsuWLS33752773 = -36463088;    float ZWRnytFNHTXEeznZEEsuWLS87483983 = -483248890;    float ZWRnytFNHTXEeznZEEsuWLS83704348 = -627936859;    float ZWRnytFNHTXEeznZEEsuWLS57776304 = -106924687;    float ZWRnytFNHTXEeznZEEsuWLS37664269 = -862502877;    float ZWRnytFNHTXEeznZEEsuWLS65219729 = -898665490;    float ZWRnytFNHTXEeznZEEsuWLS80147169 = -184207650;    float ZWRnytFNHTXEeznZEEsuWLS32654076 = -105516719;    float ZWRnytFNHTXEeznZEEsuWLS84028361 = -281658896;    float ZWRnytFNHTXEeznZEEsuWLS78855724 = -244649816;    float ZWRnytFNHTXEeznZEEsuWLS93178264 = 45618007;    float ZWRnytFNHTXEeznZEEsuWLS56095207 = -820762387;    float ZWRnytFNHTXEeznZEEsuWLS6585842 = -715190623;    float ZWRnytFNHTXEeznZEEsuWLS40940798 = -118449444;    float ZWRnytFNHTXEeznZEEsuWLS13321182 = -240565121;    float ZWRnytFNHTXEeznZEEsuWLS24884767 = -178140450;    float ZWRnytFNHTXEeznZEEsuWLS5002289 = -725423412;    float ZWRnytFNHTXEeznZEEsuWLS9537702 = -343610675;    float ZWRnytFNHTXEeznZEEsuWLS65530941 = -181405206;    float ZWRnytFNHTXEeznZEEsuWLS28703373 = -837187040;    float ZWRnytFNHTXEeznZEEsuWLS48534890 = -965658815;    float ZWRnytFNHTXEeznZEEsuWLS77512323 = -695171512;    float ZWRnytFNHTXEeznZEEsuWLS40567262 = -854470946;    float ZWRnytFNHTXEeznZEEsuWLS94656793 = -244445256;    float ZWRnytFNHTXEeznZEEsuWLS99201650 = -162227870;    float ZWRnytFNHTXEeznZEEsuWLS68283973 = -583694364;    float ZWRnytFNHTXEeznZEEsuWLS25511359 = -55316061;    float ZWRnytFNHTXEeznZEEsuWLS78828554 = -877219943;     ZWRnytFNHTXEeznZEEsuWLS10931444 = ZWRnytFNHTXEeznZEEsuWLS15110757;     ZWRnytFNHTXEeznZEEsuWLS15110757 = ZWRnytFNHTXEeznZEEsuWLS31463127;     ZWRnytFNHTXEeznZEEsuWLS31463127 = ZWRnytFNHTXEeznZEEsuWLS90015078;     ZWRnytFNHTXEeznZEEsuWLS90015078 = ZWRnytFNHTXEeznZEEsuWLS82241729;     ZWRnytFNHTXEeznZEEsuWLS82241729 = ZWRnytFNHTXEeznZEEsuWLS95587436;     ZWRnytFNHTXEeznZEEsuWLS95587436 = ZWRnytFNHTXEeznZEEsuWLS12135571;     ZWRnytFNHTXEeznZEEsuWLS12135571 = ZWRnytFNHTXEeznZEEsuWLS2042764;     ZWRnytFNHTXEeznZEEsuWLS2042764 = ZWRnytFNHTXEeznZEEsuWLS93153868;     ZWRnytFNHTXEeznZEEsuWLS93153868 = ZWRnytFNHTXEeznZEEsuWLS90399090;     ZWRnytFNHTXEeznZEEsuWLS90399090 = ZWRnytFNHTXEeznZEEsuWLS8636194;     ZWRnytFNHTXEeznZEEsuWLS8636194 = ZWRnytFNHTXEeznZEEsuWLS70858387;     ZWRnytFNHTXEeznZEEsuWLS70858387 = ZWRnytFNHTXEeznZEEsuWLS81995398;     ZWRnytFNHTXEeznZEEsuWLS81995398 = ZWRnytFNHTXEeznZEEsuWLS40440313;     ZWRnytFNHTXEeznZEEsuWLS40440313 = ZWRnytFNHTXEeznZEEsuWLS43129214;     ZWRnytFNHTXEeznZEEsuWLS43129214 = ZWRnytFNHTXEeznZEEsuWLS31561025;     ZWRnytFNHTXEeznZEEsuWLS31561025 = ZWRnytFNHTXEeznZEEsuWLS46926943;     ZWRnytFNHTXEeznZEEsuWLS46926943 = ZWRnytFNHTXEeznZEEsuWLS11621513;     ZWRnytFNHTXEeznZEEsuWLS11621513 = ZWRnytFNHTXEeznZEEsuWLS16851840;     ZWRnytFNHTXEeznZEEsuWLS16851840 = ZWRnytFNHTXEeznZEEsuWLS3040406;     ZWRnytFNHTXEeznZEEsuWLS3040406 = ZWRnytFNHTXEeznZEEsuWLS33841120;     ZWRnytFNHTXEeznZEEsuWLS33841120 = ZWRnytFNHTXEeznZEEsuWLS80477295;     ZWRnytFNHTXEeznZEEsuWLS80477295 = ZWRnytFNHTXEeznZEEsuWLS92189379;     ZWRnytFNHTXEeznZEEsuWLS92189379 = ZWRnytFNHTXEeznZEEsuWLS74768351;     ZWRnytFNHTXEeznZEEsuWLS74768351 = ZWRnytFNHTXEeznZEEsuWLS18041994;     ZWRnytFNHTXEeznZEEsuWLS18041994 = ZWRnytFNHTXEeznZEEsuWLS81885313;     ZWRnytFNHTXEeznZEEsuWLS81885313 = ZWRnytFNHTXEeznZEEsuWLS6786843;     ZWRnytFNHTXEeznZEEsuWLS6786843 = ZWRnytFNHTXEeznZEEsuWLS87456306;     ZWRnytFNHTXEeznZEEsuWLS87456306 = ZWRnytFNHTXEeznZEEsuWLS25179599;     ZWRnytFNHTXEeznZEEsuWLS25179599 = ZWRnytFNHTXEeznZEEsuWLS18624814;     ZWRnytFNHTXEeznZEEsuWLS18624814 = ZWRnytFNHTXEeznZEEsuWLS88940798;     ZWRnytFNHTXEeznZEEsuWLS88940798 = ZWRnytFNHTXEeznZEEsuWLS92843564;     ZWRnytFNHTXEeznZEEsuWLS92843564 = ZWRnytFNHTXEeznZEEsuWLS96632292;     ZWRnytFNHTXEeznZEEsuWLS96632292 = ZWRnytFNHTXEeznZEEsuWLS51626885;     ZWRnytFNHTXEeznZEEsuWLS51626885 = ZWRnytFNHTXEeznZEEsuWLS39991849;     ZWRnytFNHTXEeznZEEsuWLS39991849 = ZWRnytFNHTXEeznZEEsuWLS88452318;     ZWRnytFNHTXEeznZEEsuWLS88452318 = ZWRnytFNHTXEeznZEEsuWLS85398148;     ZWRnytFNHTXEeznZEEsuWLS85398148 = ZWRnytFNHTXEeznZEEsuWLS20507975;     ZWRnytFNHTXEeznZEEsuWLS20507975 = ZWRnytFNHTXEeznZEEsuWLS77868739;     ZWRnytFNHTXEeznZEEsuWLS77868739 = ZWRnytFNHTXEeznZEEsuWLS29367856;     ZWRnytFNHTXEeznZEEsuWLS29367856 = ZWRnytFNHTXEeznZEEsuWLS19336057;     ZWRnytFNHTXEeznZEEsuWLS19336057 = ZWRnytFNHTXEeznZEEsuWLS76064815;     ZWRnytFNHTXEeznZEEsuWLS76064815 = ZWRnytFNHTXEeznZEEsuWLS42813027;     ZWRnytFNHTXEeznZEEsuWLS42813027 = ZWRnytFNHTXEeznZEEsuWLS26969651;     ZWRnytFNHTXEeznZEEsuWLS26969651 = ZWRnytFNHTXEeznZEEsuWLS94621182;     ZWRnytFNHTXEeznZEEsuWLS94621182 = ZWRnytFNHTXEeznZEEsuWLS85387918;     ZWRnytFNHTXEeznZEEsuWLS85387918 = ZWRnytFNHTXEeznZEEsuWLS97856952;     ZWRnytFNHTXEeznZEEsuWLS97856952 = ZWRnytFNHTXEeznZEEsuWLS27931119;     ZWRnytFNHTXEeznZEEsuWLS27931119 = ZWRnytFNHTXEeznZEEsuWLS94278042;     ZWRnytFNHTXEeznZEEsuWLS94278042 = ZWRnytFNHTXEeznZEEsuWLS69084391;     ZWRnytFNHTXEeznZEEsuWLS69084391 = ZWRnytFNHTXEeznZEEsuWLS12038973;     ZWRnytFNHTXEeznZEEsuWLS12038973 = ZWRnytFNHTXEeznZEEsuWLS48000000;     ZWRnytFNHTXEeznZEEsuWLS48000000 = ZWRnytFNHTXEeznZEEsuWLS79522383;     ZWRnytFNHTXEeznZEEsuWLS79522383 = ZWRnytFNHTXEeznZEEsuWLS71747525;     ZWRnytFNHTXEeznZEEsuWLS71747525 = ZWRnytFNHTXEeznZEEsuWLS46624596;     ZWRnytFNHTXEeznZEEsuWLS46624596 = ZWRnytFNHTXEeznZEEsuWLS30454148;     ZWRnytFNHTXEeznZEEsuWLS30454148 = ZWRnytFNHTXEeznZEEsuWLS22921377;     ZWRnytFNHTXEeznZEEsuWLS22921377 = ZWRnytFNHTXEeznZEEsuWLS56694775;     ZWRnytFNHTXEeznZEEsuWLS56694775 = ZWRnytFNHTXEeznZEEsuWLS71973085;     ZWRnytFNHTXEeznZEEsuWLS71973085 = ZWRnytFNHTXEeznZEEsuWLS356417;     ZWRnytFNHTXEeznZEEsuWLS356417 = ZWRnytFNHTXEeznZEEsuWLS88800594;     ZWRnytFNHTXEeznZEEsuWLS88800594 = ZWRnytFNHTXEeznZEEsuWLS24679264;     ZWRnytFNHTXEeznZEEsuWLS24679264 = ZWRnytFNHTXEeznZEEsuWLS76863164;     ZWRnytFNHTXEeznZEEsuWLS76863164 = ZWRnytFNHTXEeznZEEsuWLS74529054;     ZWRnytFNHTXEeznZEEsuWLS74529054 = ZWRnytFNHTXEeznZEEsuWLS1458292;     ZWRnytFNHTXEeznZEEsuWLS1458292 = ZWRnytFNHTXEeznZEEsuWLS15792629;     ZWRnytFNHTXEeznZEEsuWLS15792629 = ZWRnytFNHTXEeznZEEsuWLS74226094;     ZWRnytFNHTXEeznZEEsuWLS74226094 = ZWRnytFNHTXEeznZEEsuWLS30368514;     ZWRnytFNHTXEeznZEEsuWLS30368514 = ZWRnytFNHTXEeznZEEsuWLS448464;     ZWRnytFNHTXEeznZEEsuWLS448464 = ZWRnytFNHTXEeznZEEsuWLS54676896;     ZWRnytFNHTXEeznZEEsuWLS54676896 = ZWRnytFNHTXEeznZEEsuWLS46162876;     ZWRnytFNHTXEeznZEEsuWLS46162876 = ZWRnytFNHTXEeznZEEsuWLS26418969;     ZWRnytFNHTXEeznZEEsuWLS26418969 = ZWRnytFNHTXEeznZEEsuWLS33752773;     ZWRnytFNHTXEeznZEEsuWLS33752773 = ZWRnytFNHTXEeznZEEsuWLS87483983;     ZWRnytFNHTXEeznZEEsuWLS87483983 = ZWRnytFNHTXEeznZEEsuWLS83704348;     ZWRnytFNHTXEeznZEEsuWLS83704348 = ZWRnytFNHTXEeznZEEsuWLS57776304;     ZWRnytFNHTXEeznZEEsuWLS57776304 = ZWRnytFNHTXEeznZEEsuWLS37664269;     ZWRnytFNHTXEeznZEEsuWLS37664269 = ZWRnytFNHTXEeznZEEsuWLS65219729;     ZWRnytFNHTXEeznZEEsuWLS65219729 = ZWRnytFNHTXEeznZEEsuWLS80147169;     ZWRnytFNHTXEeznZEEsuWLS80147169 = ZWRnytFNHTXEeznZEEsuWLS32654076;     ZWRnytFNHTXEeznZEEsuWLS32654076 = ZWRnytFNHTXEeznZEEsuWLS84028361;     ZWRnytFNHTXEeznZEEsuWLS84028361 = ZWRnytFNHTXEeznZEEsuWLS78855724;     ZWRnytFNHTXEeznZEEsuWLS78855724 = ZWRnytFNHTXEeznZEEsuWLS93178264;     ZWRnytFNHTXEeznZEEsuWLS93178264 = ZWRnytFNHTXEeznZEEsuWLS56095207;     ZWRnytFNHTXEeznZEEsuWLS56095207 = ZWRnytFNHTXEeznZEEsuWLS6585842;     ZWRnytFNHTXEeznZEEsuWLS6585842 = ZWRnytFNHTXEeznZEEsuWLS40940798;     ZWRnytFNHTXEeznZEEsuWLS40940798 = ZWRnytFNHTXEeznZEEsuWLS13321182;     ZWRnytFNHTXEeznZEEsuWLS13321182 = ZWRnytFNHTXEeznZEEsuWLS24884767;     ZWRnytFNHTXEeznZEEsuWLS24884767 = ZWRnytFNHTXEeznZEEsuWLS5002289;     ZWRnytFNHTXEeznZEEsuWLS5002289 = ZWRnytFNHTXEeznZEEsuWLS9537702;     ZWRnytFNHTXEeznZEEsuWLS9537702 = ZWRnytFNHTXEeznZEEsuWLS65530941;     ZWRnytFNHTXEeznZEEsuWLS65530941 = ZWRnytFNHTXEeznZEEsuWLS28703373;     ZWRnytFNHTXEeznZEEsuWLS28703373 = ZWRnytFNHTXEeznZEEsuWLS48534890;     ZWRnytFNHTXEeznZEEsuWLS48534890 = ZWRnytFNHTXEeznZEEsuWLS77512323;     ZWRnytFNHTXEeznZEEsuWLS77512323 = ZWRnytFNHTXEeznZEEsuWLS40567262;     ZWRnytFNHTXEeznZEEsuWLS40567262 = ZWRnytFNHTXEeznZEEsuWLS94656793;     ZWRnytFNHTXEeznZEEsuWLS94656793 = ZWRnytFNHTXEeznZEEsuWLS99201650;     ZWRnytFNHTXEeznZEEsuWLS99201650 = ZWRnytFNHTXEeznZEEsuWLS68283973;     ZWRnytFNHTXEeznZEEsuWLS68283973 = ZWRnytFNHTXEeznZEEsuWLS25511359;     ZWRnytFNHTXEeznZEEsuWLS25511359 = ZWRnytFNHTXEeznZEEsuWLS78828554;     ZWRnytFNHTXEeznZEEsuWLS78828554 = ZWRnytFNHTXEeznZEEsuWLS10931444;}



void ZljubqVjyApBGVQSlyOZCZbiKc65528554() {     float kVSKpIsMosgNBnuuKxYYfyS94654961 = -127242010;    float kVSKpIsMosgNBnuuKxYYfyS12708852 = -61279026;    float kVSKpIsMosgNBnuuKxYYfyS20049244 = -565998492;    float kVSKpIsMosgNBnuuKxYYfyS87163031 = -569269318;    float kVSKpIsMosgNBnuuKxYYfyS61803066 = -45811526;    float kVSKpIsMosgNBnuuKxYYfyS6926955 = -161500631;    float kVSKpIsMosgNBnuuKxYYfyS49016133 = -864794644;    float kVSKpIsMosgNBnuuKxYYfyS45382846 = -608032188;    float kVSKpIsMosgNBnuuKxYYfyS43373397 = -729007746;    float kVSKpIsMosgNBnuuKxYYfyS36504678 = -245709804;    float kVSKpIsMosgNBnuuKxYYfyS51525229 = -919596586;    float kVSKpIsMosgNBnuuKxYYfyS61070157 = -294421141;    float kVSKpIsMosgNBnuuKxYYfyS33049250 = -808575;    float kVSKpIsMosgNBnuuKxYYfyS34813458 = -917173764;    float kVSKpIsMosgNBnuuKxYYfyS47308472 = -746088700;    float kVSKpIsMosgNBnuuKxYYfyS3375054 = -814734085;    float kVSKpIsMosgNBnuuKxYYfyS46792354 = -812368865;    float kVSKpIsMosgNBnuuKxYYfyS25698782 = -382404047;    float kVSKpIsMosgNBnuuKxYYfyS57828320 = -41273906;    float kVSKpIsMosgNBnuuKxYYfyS77148049 = 49399509;    float kVSKpIsMosgNBnuuKxYYfyS1180630 = -547280607;    float kVSKpIsMosgNBnuuKxYYfyS81479367 = -370400679;    float kVSKpIsMosgNBnuuKxYYfyS61365244 = -750366264;    float kVSKpIsMosgNBnuuKxYYfyS75911046 = -639214263;    float kVSKpIsMosgNBnuuKxYYfyS54644087 = -97725261;    float kVSKpIsMosgNBnuuKxYYfyS34878071 = -150912490;    float kVSKpIsMosgNBnuuKxYYfyS28378782 = -587150553;    float kVSKpIsMosgNBnuuKxYYfyS30055507 = -167796535;    float kVSKpIsMosgNBnuuKxYYfyS97097740 = 91888029;    float kVSKpIsMosgNBnuuKxYYfyS54173141 = -693083271;    float kVSKpIsMosgNBnuuKxYYfyS70305221 = -574776833;    float kVSKpIsMosgNBnuuKxYYfyS17636853 = -786013972;    float kVSKpIsMosgNBnuuKxYYfyS10756994 = -128422020;    float kVSKpIsMosgNBnuuKxYYfyS87351599 = -289591882;    float kVSKpIsMosgNBnuuKxYYfyS7595175 = -497875833;    float kVSKpIsMosgNBnuuKxYYfyS58313552 = -796457331;    float kVSKpIsMosgNBnuuKxYYfyS85161738 = -755237528;    float kVSKpIsMosgNBnuuKxYYfyS70889765 = -309794922;    float kVSKpIsMosgNBnuuKxYYfyS38484458 = -940648266;    float kVSKpIsMosgNBnuuKxYYfyS68556972 = -716022658;    float kVSKpIsMosgNBnuuKxYYfyS84103578 = -356410841;    float kVSKpIsMosgNBnuuKxYYfyS5404542 = -371915069;    float kVSKpIsMosgNBnuuKxYYfyS75613173 = -247146771;    float kVSKpIsMosgNBnuuKxYYfyS74165170 = -401249770;    float kVSKpIsMosgNBnuuKxYYfyS22608433 = -41788039;    float kVSKpIsMosgNBnuuKxYYfyS29654800 = -528909473;    float kVSKpIsMosgNBnuuKxYYfyS31035922 = -447932474;    float kVSKpIsMosgNBnuuKxYYfyS66672966 = -660747213;    float kVSKpIsMosgNBnuuKxYYfyS28152256 = -26757205;    float kVSKpIsMosgNBnuuKxYYfyS4957722 = -195169120;    float kVSKpIsMosgNBnuuKxYYfyS52129804 = -471058676;    float kVSKpIsMosgNBnuuKxYYfyS99505719 = -989282156;    float kVSKpIsMosgNBnuuKxYYfyS24904786 = 91816005;    float kVSKpIsMosgNBnuuKxYYfyS17680132 = -861776668;    float kVSKpIsMosgNBnuuKxYYfyS64347924 = -476798768;    float kVSKpIsMosgNBnuuKxYYfyS13175595 = -756841331;    float kVSKpIsMosgNBnuuKxYYfyS51343607 = -310912762;    float kVSKpIsMosgNBnuuKxYYfyS44138197 = -926784230;    float kVSKpIsMosgNBnuuKxYYfyS32518945 = -371544058;    float kVSKpIsMosgNBnuuKxYYfyS26924995 = -894899037;    float kVSKpIsMosgNBnuuKxYYfyS78548172 = -574350078;    float kVSKpIsMosgNBnuuKxYYfyS18960627 = -596998109;    float kVSKpIsMosgNBnuuKxYYfyS48285106 = -599920217;    float kVSKpIsMosgNBnuuKxYYfyS89200255 = 64075524;    float kVSKpIsMosgNBnuuKxYYfyS66199457 = -670932972;    float kVSKpIsMosgNBnuuKxYYfyS33888376 = -33582614;    float kVSKpIsMosgNBnuuKxYYfyS50313164 = -65999121;    float kVSKpIsMosgNBnuuKxYYfyS45697651 = -711216694;    float kVSKpIsMosgNBnuuKxYYfyS27218283 = -319297932;    float kVSKpIsMosgNBnuuKxYYfyS88994920 = -949631370;    float kVSKpIsMosgNBnuuKxYYfyS18213315 = 40503443;    float kVSKpIsMosgNBnuuKxYYfyS75902588 = -402573944;    float kVSKpIsMosgNBnuuKxYYfyS87214323 = -441755781;    float kVSKpIsMosgNBnuuKxYYfyS89271347 = -325251249;    float kVSKpIsMosgNBnuuKxYYfyS93044471 = -594189650;    float kVSKpIsMosgNBnuuKxYYfyS95776087 = -75365538;    float kVSKpIsMosgNBnuuKxYYfyS5866194 = -23253909;    float kVSKpIsMosgNBnuuKxYYfyS87200074 = -249116495;    float kVSKpIsMosgNBnuuKxYYfyS53302613 = -497426224;    float kVSKpIsMosgNBnuuKxYYfyS24989287 = -568815789;    float kVSKpIsMosgNBnuuKxYYfyS3842150 = -702980016;    float kVSKpIsMosgNBnuuKxYYfyS61705816 = -926403341;    float kVSKpIsMosgNBnuuKxYYfyS1903251 = -41039330;    float kVSKpIsMosgNBnuuKxYYfyS92140019 = -712942852;    float kVSKpIsMosgNBnuuKxYYfyS2043338 = -122024596;    float kVSKpIsMosgNBnuuKxYYfyS70799501 = -585494678;    float kVSKpIsMosgNBnuuKxYYfyS92732067 = -777829977;    float kVSKpIsMosgNBnuuKxYYfyS93076862 = -266645353;    float kVSKpIsMosgNBnuuKxYYfyS23003675 = -812793114;    float kVSKpIsMosgNBnuuKxYYfyS94419579 = -741034502;    float kVSKpIsMosgNBnuuKxYYfyS6969945 = -385544569;    float kVSKpIsMosgNBnuuKxYYfyS41023541 = -828453299;    float kVSKpIsMosgNBnuuKxYYfyS38370821 = -938250864;    float kVSKpIsMosgNBnuuKxYYfyS11559464 = 54250770;    float kVSKpIsMosgNBnuuKxYYfyS90008799 = -41672580;    float kVSKpIsMosgNBnuuKxYYfyS65142951 = -759412733;    float kVSKpIsMosgNBnuuKxYYfyS57119436 = -771994853;    float kVSKpIsMosgNBnuuKxYYfyS86412918 = -211222296;    float kVSKpIsMosgNBnuuKxYYfyS7965713 = -730316799;    float kVSKpIsMosgNBnuuKxYYfyS88720057 = -127242010;     kVSKpIsMosgNBnuuKxYYfyS94654961 = kVSKpIsMosgNBnuuKxYYfyS12708852;     kVSKpIsMosgNBnuuKxYYfyS12708852 = kVSKpIsMosgNBnuuKxYYfyS20049244;     kVSKpIsMosgNBnuuKxYYfyS20049244 = kVSKpIsMosgNBnuuKxYYfyS87163031;     kVSKpIsMosgNBnuuKxYYfyS87163031 = kVSKpIsMosgNBnuuKxYYfyS61803066;     kVSKpIsMosgNBnuuKxYYfyS61803066 = kVSKpIsMosgNBnuuKxYYfyS6926955;     kVSKpIsMosgNBnuuKxYYfyS6926955 = kVSKpIsMosgNBnuuKxYYfyS49016133;     kVSKpIsMosgNBnuuKxYYfyS49016133 = kVSKpIsMosgNBnuuKxYYfyS45382846;     kVSKpIsMosgNBnuuKxYYfyS45382846 = kVSKpIsMosgNBnuuKxYYfyS43373397;     kVSKpIsMosgNBnuuKxYYfyS43373397 = kVSKpIsMosgNBnuuKxYYfyS36504678;     kVSKpIsMosgNBnuuKxYYfyS36504678 = kVSKpIsMosgNBnuuKxYYfyS51525229;     kVSKpIsMosgNBnuuKxYYfyS51525229 = kVSKpIsMosgNBnuuKxYYfyS61070157;     kVSKpIsMosgNBnuuKxYYfyS61070157 = kVSKpIsMosgNBnuuKxYYfyS33049250;     kVSKpIsMosgNBnuuKxYYfyS33049250 = kVSKpIsMosgNBnuuKxYYfyS34813458;     kVSKpIsMosgNBnuuKxYYfyS34813458 = kVSKpIsMosgNBnuuKxYYfyS47308472;     kVSKpIsMosgNBnuuKxYYfyS47308472 = kVSKpIsMosgNBnuuKxYYfyS3375054;     kVSKpIsMosgNBnuuKxYYfyS3375054 = kVSKpIsMosgNBnuuKxYYfyS46792354;     kVSKpIsMosgNBnuuKxYYfyS46792354 = kVSKpIsMosgNBnuuKxYYfyS25698782;     kVSKpIsMosgNBnuuKxYYfyS25698782 = kVSKpIsMosgNBnuuKxYYfyS57828320;     kVSKpIsMosgNBnuuKxYYfyS57828320 = kVSKpIsMosgNBnuuKxYYfyS77148049;     kVSKpIsMosgNBnuuKxYYfyS77148049 = kVSKpIsMosgNBnuuKxYYfyS1180630;     kVSKpIsMosgNBnuuKxYYfyS1180630 = kVSKpIsMosgNBnuuKxYYfyS81479367;     kVSKpIsMosgNBnuuKxYYfyS81479367 = kVSKpIsMosgNBnuuKxYYfyS61365244;     kVSKpIsMosgNBnuuKxYYfyS61365244 = kVSKpIsMosgNBnuuKxYYfyS75911046;     kVSKpIsMosgNBnuuKxYYfyS75911046 = kVSKpIsMosgNBnuuKxYYfyS54644087;     kVSKpIsMosgNBnuuKxYYfyS54644087 = kVSKpIsMosgNBnuuKxYYfyS34878071;     kVSKpIsMosgNBnuuKxYYfyS34878071 = kVSKpIsMosgNBnuuKxYYfyS28378782;     kVSKpIsMosgNBnuuKxYYfyS28378782 = kVSKpIsMosgNBnuuKxYYfyS30055507;     kVSKpIsMosgNBnuuKxYYfyS30055507 = kVSKpIsMosgNBnuuKxYYfyS97097740;     kVSKpIsMosgNBnuuKxYYfyS97097740 = kVSKpIsMosgNBnuuKxYYfyS54173141;     kVSKpIsMosgNBnuuKxYYfyS54173141 = kVSKpIsMosgNBnuuKxYYfyS70305221;     kVSKpIsMosgNBnuuKxYYfyS70305221 = kVSKpIsMosgNBnuuKxYYfyS17636853;     kVSKpIsMosgNBnuuKxYYfyS17636853 = kVSKpIsMosgNBnuuKxYYfyS10756994;     kVSKpIsMosgNBnuuKxYYfyS10756994 = kVSKpIsMosgNBnuuKxYYfyS87351599;     kVSKpIsMosgNBnuuKxYYfyS87351599 = kVSKpIsMosgNBnuuKxYYfyS7595175;     kVSKpIsMosgNBnuuKxYYfyS7595175 = kVSKpIsMosgNBnuuKxYYfyS58313552;     kVSKpIsMosgNBnuuKxYYfyS58313552 = kVSKpIsMosgNBnuuKxYYfyS85161738;     kVSKpIsMosgNBnuuKxYYfyS85161738 = kVSKpIsMosgNBnuuKxYYfyS70889765;     kVSKpIsMosgNBnuuKxYYfyS70889765 = kVSKpIsMosgNBnuuKxYYfyS38484458;     kVSKpIsMosgNBnuuKxYYfyS38484458 = kVSKpIsMosgNBnuuKxYYfyS68556972;     kVSKpIsMosgNBnuuKxYYfyS68556972 = kVSKpIsMosgNBnuuKxYYfyS84103578;     kVSKpIsMosgNBnuuKxYYfyS84103578 = kVSKpIsMosgNBnuuKxYYfyS5404542;     kVSKpIsMosgNBnuuKxYYfyS5404542 = kVSKpIsMosgNBnuuKxYYfyS75613173;     kVSKpIsMosgNBnuuKxYYfyS75613173 = kVSKpIsMosgNBnuuKxYYfyS74165170;     kVSKpIsMosgNBnuuKxYYfyS74165170 = kVSKpIsMosgNBnuuKxYYfyS22608433;     kVSKpIsMosgNBnuuKxYYfyS22608433 = kVSKpIsMosgNBnuuKxYYfyS29654800;     kVSKpIsMosgNBnuuKxYYfyS29654800 = kVSKpIsMosgNBnuuKxYYfyS31035922;     kVSKpIsMosgNBnuuKxYYfyS31035922 = kVSKpIsMosgNBnuuKxYYfyS66672966;     kVSKpIsMosgNBnuuKxYYfyS66672966 = kVSKpIsMosgNBnuuKxYYfyS28152256;     kVSKpIsMosgNBnuuKxYYfyS28152256 = kVSKpIsMosgNBnuuKxYYfyS4957722;     kVSKpIsMosgNBnuuKxYYfyS4957722 = kVSKpIsMosgNBnuuKxYYfyS52129804;     kVSKpIsMosgNBnuuKxYYfyS52129804 = kVSKpIsMosgNBnuuKxYYfyS99505719;     kVSKpIsMosgNBnuuKxYYfyS99505719 = kVSKpIsMosgNBnuuKxYYfyS24904786;     kVSKpIsMosgNBnuuKxYYfyS24904786 = kVSKpIsMosgNBnuuKxYYfyS17680132;     kVSKpIsMosgNBnuuKxYYfyS17680132 = kVSKpIsMosgNBnuuKxYYfyS64347924;     kVSKpIsMosgNBnuuKxYYfyS64347924 = kVSKpIsMosgNBnuuKxYYfyS13175595;     kVSKpIsMosgNBnuuKxYYfyS13175595 = kVSKpIsMosgNBnuuKxYYfyS51343607;     kVSKpIsMosgNBnuuKxYYfyS51343607 = kVSKpIsMosgNBnuuKxYYfyS44138197;     kVSKpIsMosgNBnuuKxYYfyS44138197 = kVSKpIsMosgNBnuuKxYYfyS32518945;     kVSKpIsMosgNBnuuKxYYfyS32518945 = kVSKpIsMosgNBnuuKxYYfyS26924995;     kVSKpIsMosgNBnuuKxYYfyS26924995 = kVSKpIsMosgNBnuuKxYYfyS78548172;     kVSKpIsMosgNBnuuKxYYfyS78548172 = kVSKpIsMosgNBnuuKxYYfyS18960627;     kVSKpIsMosgNBnuuKxYYfyS18960627 = kVSKpIsMosgNBnuuKxYYfyS48285106;     kVSKpIsMosgNBnuuKxYYfyS48285106 = kVSKpIsMosgNBnuuKxYYfyS89200255;     kVSKpIsMosgNBnuuKxYYfyS89200255 = kVSKpIsMosgNBnuuKxYYfyS66199457;     kVSKpIsMosgNBnuuKxYYfyS66199457 = kVSKpIsMosgNBnuuKxYYfyS33888376;     kVSKpIsMosgNBnuuKxYYfyS33888376 = kVSKpIsMosgNBnuuKxYYfyS50313164;     kVSKpIsMosgNBnuuKxYYfyS50313164 = kVSKpIsMosgNBnuuKxYYfyS45697651;     kVSKpIsMosgNBnuuKxYYfyS45697651 = kVSKpIsMosgNBnuuKxYYfyS27218283;     kVSKpIsMosgNBnuuKxYYfyS27218283 = kVSKpIsMosgNBnuuKxYYfyS88994920;     kVSKpIsMosgNBnuuKxYYfyS88994920 = kVSKpIsMosgNBnuuKxYYfyS18213315;     kVSKpIsMosgNBnuuKxYYfyS18213315 = kVSKpIsMosgNBnuuKxYYfyS75902588;     kVSKpIsMosgNBnuuKxYYfyS75902588 = kVSKpIsMosgNBnuuKxYYfyS87214323;     kVSKpIsMosgNBnuuKxYYfyS87214323 = kVSKpIsMosgNBnuuKxYYfyS89271347;     kVSKpIsMosgNBnuuKxYYfyS89271347 = kVSKpIsMosgNBnuuKxYYfyS93044471;     kVSKpIsMosgNBnuuKxYYfyS93044471 = kVSKpIsMosgNBnuuKxYYfyS95776087;     kVSKpIsMosgNBnuuKxYYfyS95776087 = kVSKpIsMosgNBnuuKxYYfyS5866194;     kVSKpIsMosgNBnuuKxYYfyS5866194 = kVSKpIsMosgNBnuuKxYYfyS87200074;     kVSKpIsMosgNBnuuKxYYfyS87200074 = kVSKpIsMosgNBnuuKxYYfyS53302613;     kVSKpIsMosgNBnuuKxYYfyS53302613 = kVSKpIsMosgNBnuuKxYYfyS24989287;     kVSKpIsMosgNBnuuKxYYfyS24989287 = kVSKpIsMosgNBnuuKxYYfyS3842150;     kVSKpIsMosgNBnuuKxYYfyS3842150 = kVSKpIsMosgNBnuuKxYYfyS61705816;     kVSKpIsMosgNBnuuKxYYfyS61705816 = kVSKpIsMosgNBnuuKxYYfyS1903251;     kVSKpIsMosgNBnuuKxYYfyS1903251 = kVSKpIsMosgNBnuuKxYYfyS92140019;     kVSKpIsMosgNBnuuKxYYfyS92140019 = kVSKpIsMosgNBnuuKxYYfyS2043338;     kVSKpIsMosgNBnuuKxYYfyS2043338 = kVSKpIsMosgNBnuuKxYYfyS70799501;     kVSKpIsMosgNBnuuKxYYfyS70799501 = kVSKpIsMosgNBnuuKxYYfyS92732067;     kVSKpIsMosgNBnuuKxYYfyS92732067 = kVSKpIsMosgNBnuuKxYYfyS93076862;     kVSKpIsMosgNBnuuKxYYfyS93076862 = kVSKpIsMosgNBnuuKxYYfyS23003675;     kVSKpIsMosgNBnuuKxYYfyS23003675 = kVSKpIsMosgNBnuuKxYYfyS94419579;     kVSKpIsMosgNBnuuKxYYfyS94419579 = kVSKpIsMosgNBnuuKxYYfyS6969945;     kVSKpIsMosgNBnuuKxYYfyS6969945 = kVSKpIsMosgNBnuuKxYYfyS41023541;     kVSKpIsMosgNBnuuKxYYfyS41023541 = kVSKpIsMosgNBnuuKxYYfyS38370821;     kVSKpIsMosgNBnuuKxYYfyS38370821 = kVSKpIsMosgNBnuuKxYYfyS11559464;     kVSKpIsMosgNBnuuKxYYfyS11559464 = kVSKpIsMosgNBnuuKxYYfyS90008799;     kVSKpIsMosgNBnuuKxYYfyS90008799 = kVSKpIsMosgNBnuuKxYYfyS65142951;     kVSKpIsMosgNBnuuKxYYfyS65142951 = kVSKpIsMosgNBnuuKxYYfyS57119436;     kVSKpIsMosgNBnuuKxYYfyS57119436 = kVSKpIsMosgNBnuuKxYYfyS86412918;     kVSKpIsMosgNBnuuKxYYfyS86412918 = kVSKpIsMosgNBnuuKxYYfyS7965713;     kVSKpIsMosgNBnuuKxYYfyS7965713 = kVSKpIsMosgNBnuuKxYYfyS88720057;     kVSKpIsMosgNBnuuKxYYfyS88720057 = kVSKpIsMosgNBnuuKxYYfyS94654961;}



void bajxBLJFQIpuSyEemlbTNEHvoy50591391() {     float TOieaRyOSzMXnyBDcleKXRO78378479 = -477264077;    float TOieaRyOSzMXnyBDcleKXRO10306948 = -964930877;    float TOieaRyOSzMXnyBDcleKXRO8635361 = -434306183;    float TOieaRyOSzMXnyBDcleKXRO84310985 = -574151247;    float TOieaRyOSzMXnyBDcleKXRO41364403 = -443759718;    float TOieaRyOSzMXnyBDcleKXRO18266473 = -264821300;    float TOieaRyOSzMXnyBDcleKXRO85896696 = -886308782;    float TOieaRyOSzMXnyBDcleKXRO88722929 = -128486774;    float TOieaRyOSzMXnyBDcleKXRO93592925 = -326490443;    float TOieaRyOSzMXnyBDcleKXRO82610266 = -468750820;    float TOieaRyOSzMXnyBDcleKXRO94414265 = -759881844;    float TOieaRyOSzMXnyBDcleKXRO51281928 = -223560412;    float TOieaRyOSzMXnyBDcleKXRO84103102 = -943640819;    float TOieaRyOSzMXnyBDcleKXRO29186602 = -166856626;    float TOieaRyOSzMXnyBDcleKXRO51487731 = -671786005;    float TOieaRyOSzMXnyBDcleKXRO75189081 = -738861736;    float TOieaRyOSzMXnyBDcleKXRO46657765 = -971248728;    float TOieaRyOSzMXnyBDcleKXRO39776052 = 6744380;    float TOieaRyOSzMXnyBDcleKXRO98804800 = -929923144;    float TOieaRyOSzMXnyBDcleKXRO51255694 = -666893408;    float TOieaRyOSzMXnyBDcleKXRO68520140 = -121747128;    float TOieaRyOSzMXnyBDcleKXRO82481438 = -283555966;    float TOieaRyOSzMXnyBDcleKXRO30541110 = -779761001;    float TOieaRyOSzMXnyBDcleKXRO77053741 = -178446916;    float TOieaRyOSzMXnyBDcleKXRO91246179 = -176056022;    float TOieaRyOSzMXnyBDcleKXRO87870828 = -593879518;    float TOieaRyOSzMXnyBDcleKXRO49970722 = 68974024;    float TOieaRyOSzMXnyBDcleKXRO72654706 = 45761977;    float TOieaRyOSzMXnyBDcleKXRO69015882 = -432307872;    float TOieaRyOSzMXnyBDcleKXRO89721468 = -265689645;    float TOieaRyOSzMXnyBDcleKXRO51669644 = -793874854;    float TOieaRyOSzMXnyBDcleKXRO42430141 = -235211195;    float TOieaRyOSzMXnyBDcleKXRO24881695 = -793636112;    float TOieaRyOSzMXnyBDcleKXRO23076313 = -693688530;    float TOieaRyOSzMXnyBDcleKXRO75198500 = -232166440;    float TOieaRyOSzMXnyBDcleKXRO28174786 = -774853808;    float TOieaRyOSzMXnyBDcleKXRO84925328 = 24421176;    float TOieaRyOSzMXnyBDcleKXRO21271557 = -208938138;    float TOieaRyOSzMXnyBDcleKXRO99100177 = -146207147;    float TOieaRyOSzMXnyBDcleKXRO7746088 = -662669537;    float TOieaRyOSzMXnyBDcleKXRO48871099 = -6450968;    float TOieaRyOSzMXnyBDcleKXRO34744269 = 22059261;    float TOieaRyOSzMXnyBDcleKXRO8413320 = -999551025;    float TOieaRyOSzMXnyBDcleKXRO21360690 = 19806497;    float TOieaRyOSzMXnyBDcleKXRO50595684 = -367802117;    float TOieaRyOSzMXnyBDcleKXRO73921681 = -143941164;    float TOieaRyOSzMXnyBDcleKXRO64214891 = -469578381;    float TOieaRyOSzMXnyBDcleKXRO5414813 = -422869110;    float TOieaRyOSzMXnyBDcleKXRO62026469 = -826541356;    float TOieaRyOSzMXnyBDcleKXRO40831052 = -827184555;    float TOieaRyOSzMXnyBDcleKXRO92220635 = -636831077;    float TOieaRyOSzMXnyBDcleKXRO51011440 = -741334942;    float TOieaRyOSzMXnyBDcleKXRO70287188 = 79883639;    float TOieaRyOSzMXnyBDcleKXRO63612737 = -338485855;    float TOieaRyOSzMXnyBDcleKXRO82071251 = -793525713;    float TOieaRyOSzMXnyBDcleKXRO95897041 = -93708112;    float TOieaRyOSzMXnyBDcleKXRO79765837 = -85169876;    float TOieaRyOSzMXnyBDcleKXRO31581620 = -155859267;    float TOieaRyOSzMXnyBDcleKXRO93064805 = -298095225;    float TOieaRyOSzMXnyBDcleKXRO53493574 = -849880200;    float TOieaRyOSzMXnyBDcleKXRO68295751 = -233795324;    float TOieaRyOSzMXnyBDcleKXRO13241990 = -832070759;    float TOieaRyOSzMXnyBDcleKXRO19707047 = -696178902;    float TOieaRyOSzMXnyBDcleKXRO3871457 = 39199202;    float TOieaRyOSzMXnyBDcleKXRO30940622 = -674875967;    float TOieaRyOSzMXnyBDcleKXRO51984124 = -424670650;    float TOieaRyOSzMXnyBDcleKXRO26400234 = -429924301;    float TOieaRyOSzMXnyBDcleKXRO61026789 = -149952289;    float TOieaRyOSzMXnyBDcleKXRO53988102 = -934690186;    float TOieaRyOSzMXnyBDcleKXRO23312946 = -896932198;    float TOieaRyOSzMXnyBDcleKXRO90263753 = -663282912;    float TOieaRyOSzMXnyBDcleKXRO25386208 = -662310590;    float TOieaRyOSzMXnyBDcleKXRO40675874 = -847048473;    float TOieaRyOSzMXnyBDcleKXRO91058712 = -167253607;    float TOieaRyOSzMXnyBDcleKXRO2384596 = -560442440;    float TOieaRyOSzMXnyBDcleKXRO33775872 = -43806389;    float TOieaRyOSzMXnyBDcleKXRO74068118 = -284004941;    float TOieaRyOSzMXnyBDcleKXRO9180421 = -699567499;    float TOieaRyOSzMXnyBDcleKXRO26458058 = -810644799;    float TOieaRyOSzMXnyBDcleKXRO17324499 = 67885141;    float TOieaRyOSzMXnyBDcleKXRO23655938 = -24301137;    float TOieaRyOSzMXnyBDcleKXRO44555909 = -508156866;    float TOieaRyOSzMXnyBDcleKXRO10628238 = -127696667;    float TOieaRyOSzMXnyBDcleKXRO28184831 = -605123317;    float TOieaRyOSzMXnyBDcleKXRO97500833 = -628858569;    float TOieaRyOSzMXnyBDcleKXRO658205 = 47460088;    float TOieaRyOSzMXnyBDcleKXRO72142953 = -215094834;    float TOieaRyOSzMXnyBDcleKXRO61268957 = -355150257;    float TOieaRyOSzMXnyBDcleKXRO41005062 = -900162817;    float TOieaRyOSzMXnyBDcleKXRO79301458 = -38458329;    float TOieaRyOSzMXnyBDcleKXRO48408948 = -589683932;    float TOieaRyOSzMXnyBDcleKXRO53343709 = -819719558;    float TOieaRyOSzMXnyBDcleKXRO28206752 = -910842913;    float TOieaRyOSzMXnyBDcleKXRO45606604 = -296326948;    float TOieaRyOSzMXnyBDcleKXRO39450337 = -328874214;    float TOieaRyOSzMXnyBDcleKXRO35629109 = -174380210;    float TOieaRyOSzMXnyBDcleKXRO15037222 = -281761837;    float TOieaRyOSzMXnyBDcleKXRO4541864 = -938750228;    float TOieaRyOSzMXnyBDcleKXRO90420067 = -305317537;    float TOieaRyOSzMXnyBDcleKXRO98611560 = -477264077;     TOieaRyOSzMXnyBDcleKXRO78378479 = TOieaRyOSzMXnyBDcleKXRO10306948;     TOieaRyOSzMXnyBDcleKXRO10306948 = TOieaRyOSzMXnyBDcleKXRO8635361;     TOieaRyOSzMXnyBDcleKXRO8635361 = TOieaRyOSzMXnyBDcleKXRO84310985;     TOieaRyOSzMXnyBDcleKXRO84310985 = TOieaRyOSzMXnyBDcleKXRO41364403;     TOieaRyOSzMXnyBDcleKXRO41364403 = TOieaRyOSzMXnyBDcleKXRO18266473;     TOieaRyOSzMXnyBDcleKXRO18266473 = TOieaRyOSzMXnyBDcleKXRO85896696;     TOieaRyOSzMXnyBDcleKXRO85896696 = TOieaRyOSzMXnyBDcleKXRO88722929;     TOieaRyOSzMXnyBDcleKXRO88722929 = TOieaRyOSzMXnyBDcleKXRO93592925;     TOieaRyOSzMXnyBDcleKXRO93592925 = TOieaRyOSzMXnyBDcleKXRO82610266;     TOieaRyOSzMXnyBDcleKXRO82610266 = TOieaRyOSzMXnyBDcleKXRO94414265;     TOieaRyOSzMXnyBDcleKXRO94414265 = TOieaRyOSzMXnyBDcleKXRO51281928;     TOieaRyOSzMXnyBDcleKXRO51281928 = TOieaRyOSzMXnyBDcleKXRO84103102;     TOieaRyOSzMXnyBDcleKXRO84103102 = TOieaRyOSzMXnyBDcleKXRO29186602;     TOieaRyOSzMXnyBDcleKXRO29186602 = TOieaRyOSzMXnyBDcleKXRO51487731;     TOieaRyOSzMXnyBDcleKXRO51487731 = TOieaRyOSzMXnyBDcleKXRO75189081;     TOieaRyOSzMXnyBDcleKXRO75189081 = TOieaRyOSzMXnyBDcleKXRO46657765;     TOieaRyOSzMXnyBDcleKXRO46657765 = TOieaRyOSzMXnyBDcleKXRO39776052;     TOieaRyOSzMXnyBDcleKXRO39776052 = TOieaRyOSzMXnyBDcleKXRO98804800;     TOieaRyOSzMXnyBDcleKXRO98804800 = TOieaRyOSzMXnyBDcleKXRO51255694;     TOieaRyOSzMXnyBDcleKXRO51255694 = TOieaRyOSzMXnyBDcleKXRO68520140;     TOieaRyOSzMXnyBDcleKXRO68520140 = TOieaRyOSzMXnyBDcleKXRO82481438;     TOieaRyOSzMXnyBDcleKXRO82481438 = TOieaRyOSzMXnyBDcleKXRO30541110;     TOieaRyOSzMXnyBDcleKXRO30541110 = TOieaRyOSzMXnyBDcleKXRO77053741;     TOieaRyOSzMXnyBDcleKXRO77053741 = TOieaRyOSzMXnyBDcleKXRO91246179;     TOieaRyOSzMXnyBDcleKXRO91246179 = TOieaRyOSzMXnyBDcleKXRO87870828;     TOieaRyOSzMXnyBDcleKXRO87870828 = TOieaRyOSzMXnyBDcleKXRO49970722;     TOieaRyOSzMXnyBDcleKXRO49970722 = TOieaRyOSzMXnyBDcleKXRO72654706;     TOieaRyOSzMXnyBDcleKXRO72654706 = TOieaRyOSzMXnyBDcleKXRO69015882;     TOieaRyOSzMXnyBDcleKXRO69015882 = TOieaRyOSzMXnyBDcleKXRO89721468;     TOieaRyOSzMXnyBDcleKXRO89721468 = TOieaRyOSzMXnyBDcleKXRO51669644;     TOieaRyOSzMXnyBDcleKXRO51669644 = TOieaRyOSzMXnyBDcleKXRO42430141;     TOieaRyOSzMXnyBDcleKXRO42430141 = TOieaRyOSzMXnyBDcleKXRO24881695;     TOieaRyOSzMXnyBDcleKXRO24881695 = TOieaRyOSzMXnyBDcleKXRO23076313;     TOieaRyOSzMXnyBDcleKXRO23076313 = TOieaRyOSzMXnyBDcleKXRO75198500;     TOieaRyOSzMXnyBDcleKXRO75198500 = TOieaRyOSzMXnyBDcleKXRO28174786;     TOieaRyOSzMXnyBDcleKXRO28174786 = TOieaRyOSzMXnyBDcleKXRO84925328;     TOieaRyOSzMXnyBDcleKXRO84925328 = TOieaRyOSzMXnyBDcleKXRO21271557;     TOieaRyOSzMXnyBDcleKXRO21271557 = TOieaRyOSzMXnyBDcleKXRO99100177;     TOieaRyOSzMXnyBDcleKXRO99100177 = TOieaRyOSzMXnyBDcleKXRO7746088;     TOieaRyOSzMXnyBDcleKXRO7746088 = TOieaRyOSzMXnyBDcleKXRO48871099;     TOieaRyOSzMXnyBDcleKXRO48871099 = TOieaRyOSzMXnyBDcleKXRO34744269;     TOieaRyOSzMXnyBDcleKXRO34744269 = TOieaRyOSzMXnyBDcleKXRO8413320;     TOieaRyOSzMXnyBDcleKXRO8413320 = TOieaRyOSzMXnyBDcleKXRO21360690;     TOieaRyOSzMXnyBDcleKXRO21360690 = TOieaRyOSzMXnyBDcleKXRO50595684;     TOieaRyOSzMXnyBDcleKXRO50595684 = TOieaRyOSzMXnyBDcleKXRO73921681;     TOieaRyOSzMXnyBDcleKXRO73921681 = TOieaRyOSzMXnyBDcleKXRO64214891;     TOieaRyOSzMXnyBDcleKXRO64214891 = TOieaRyOSzMXnyBDcleKXRO5414813;     TOieaRyOSzMXnyBDcleKXRO5414813 = TOieaRyOSzMXnyBDcleKXRO62026469;     TOieaRyOSzMXnyBDcleKXRO62026469 = TOieaRyOSzMXnyBDcleKXRO40831052;     TOieaRyOSzMXnyBDcleKXRO40831052 = TOieaRyOSzMXnyBDcleKXRO92220635;     TOieaRyOSzMXnyBDcleKXRO92220635 = TOieaRyOSzMXnyBDcleKXRO51011440;     TOieaRyOSzMXnyBDcleKXRO51011440 = TOieaRyOSzMXnyBDcleKXRO70287188;     TOieaRyOSzMXnyBDcleKXRO70287188 = TOieaRyOSzMXnyBDcleKXRO63612737;     TOieaRyOSzMXnyBDcleKXRO63612737 = TOieaRyOSzMXnyBDcleKXRO82071251;     TOieaRyOSzMXnyBDcleKXRO82071251 = TOieaRyOSzMXnyBDcleKXRO95897041;     TOieaRyOSzMXnyBDcleKXRO95897041 = TOieaRyOSzMXnyBDcleKXRO79765837;     TOieaRyOSzMXnyBDcleKXRO79765837 = TOieaRyOSzMXnyBDcleKXRO31581620;     TOieaRyOSzMXnyBDcleKXRO31581620 = TOieaRyOSzMXnyBDcleKXRO93064805;     TOieaRyOSzMXnyBDcleKXRO93064805 = TOieaRyOSzMXnyBDcleKXRO53493574;     TOieaRyOSzMXnyBDcleKXRO53493574 = TOieaRyOSzMXnyBDcleKXRO68295751;     TOieaRyOSzMXnyBDcleKXRO68295751 = TOieaRyOSzMXnyBDcleKXRO13241990;     TOieaRyOSzMXnyBDcleKXRO13241990 = TOieaRyOSzMXnyBDcleKXRO19707047;     TOieaRyOSzMXnyBDcleKXRO19707047 = TOieaRyOSzMXnyBDcleKXRO3871457;     TOieaRyOSzMXnyBDcleKXRO3871457 = TOieaRyOSzMXnyBDcleKXRO30940622;     TOieaRyOSzMXnyBDcleKXRO30940622 = TOieaRyOSzMXnyBDcleKXRO51984124;     TOieaRyOSzMXnyBDcleKXRO51984124 = TOieaRyOSzMXnyBDcleKXRO26400234;     TOieaRyOSzMXnyBDcleKXRO26400234 = TOieaRyOSzMXnyBDcleKXRO61026789;     TOieaRyOSzMXnyBDcleKXRO61026789 = TOieaRyOSzMXnyBDcleKXRO53988102;     TOieaRyOSzMXnyBDcleKXRO53988102 = TOieaRyOSzMXnyBDcleKXRO23312946;     TOieaRyOSzMXnyBDcleKXRO23312946 = TOieaRyOSzMXnyBDcleKXRO90263753;     TOieaRyOSzMXnyBDcleKXRO90263753 = TOieaRyOSzMXnyBDcleKXRO25386208;     TOieaRyOSzMXnyBDcleKXRO25386208 = TOieaRyOSzMXnyBDcleKXRO40675874;     TOieaRyOSzMXnyBDcleKXRO40675874 = TOieaRyOSzMXnyBDcleKXRO91058712;     TOieaRyOSzMXnyBDcleKXRO91058712 = TOieaRyOSzMXnyBDcleKXRO2384596;     TOieaRyOSzMXnyBDcleKXRO2384596 = TOieaRyOSzMXnyBDcleKXRO33775872;     TOieaRyOSzMXnyBDcleKXRO33775872 = TOieaRyOSzMXnyBDcleKXRO74068118;     TOieaRyOSzMXnyBDcleKXRO74068118 = TOieaRyOSzMXnyBDcleKXRO9180421;     TOieaRyOSzMXnyBDcleKXRO9180421 = TOieaRyOSzMXnyBDcleKXRO26458058;     TOieaRyOSzMXnyBDcleKXRO26458058 = TOieaRyOSzMXnyBDcleKXRO17324499;     TOieaRyOSzMXnyBDcleKXRO17324499 = TOieaRyOSzMXnyBDcleKXRO23655938;     TOieaRyOSzMXnyBDcleKXRO23655938 = TOieaRyOSzMXnyBDcleKXRO44555909;     TOieaRyOSzMXnyBDcleKXRO44555909 = TOieaRyOSzMXnyBDcleKXRO10628238;     TOieaRyOSzMXnyBDcleKXRO10628238 = TOieaRyOSzMXnyBDcleKXRO28184831;     TOieaRyOSzMXnyBDcleKXRO28184831 = TOieaRyOSzMXnyBDcleKXRO97500833;     TOieaRyOSzMXnyBDcleKXRO97500833 = TOieaRyOSzMXnyBDcleKXRO658205;     TOieaRyOSzMXnyBDcleKXRO658205 = TOieaRyOSzMXnyBDcleKXRO72142953;     TOieaRyOSzMXnyBDcleKXRO72142953 = TOieaRyOSzMXnyBDcleKXRO61268957;     TOieaRyOSzMXnyBDcleKXRO61268957 = TOieaRyOSzMXnyBDcleKXRO41005062;     TOieaRyOSzMXnyBDcleKXRO41005062 = TOieaRyOSzMXnyBDcleKXRO79301458;     TOieaRyOSzMXnyBDcleKXRO79301458 = TOieaRyOSzMXnyBDcleKXRO48408948;     TOieaRyOSzMXnyBDcleKXRO48408948 = TOieaRyOSzMXnyBDcleKXRO53343709;     TOieaRyOSzMXnyBDcleKXRO53343709 = TOieaRyOSzMXnyBDcleKXRO28206752;     TOieaRyOSzMXnyBDcleKXRO28206752 = TOieaRyOSzMXnyBDcleKXRO45606604;     TOieaRyOSzMXnyBDcleKXRO45606604 = TOieaRyOSzMXnyBDcleKXRO39450337;     TOieaRyOSzMXnyBDcleKXRO39450337 = TOieaRyOSzMXnyBDcleKXRO35629109;     TOieaRyOSzMXnyBDcleKXRO35629109 = TOieaRyOSzMXnyBDcleKXRO15037222;     TOieaRyOSzMXnyBDcleKXRO15037222 = TOieaRyOSzMXnyBDcleKXRO4541864;     TOieaRyOSzMXnyBDcleKXRO4541864 = TOieaRyOSzMXnyBDcleKXRO90420067;     TOieaRyOSzMXnyBDcleKXRO90420067 = TOieaRyOSzMXnyBDcleKXRO98611560;     TOieaRyOSzMXnyBDcleKXRO98611560 = TOieaRyOSzMXnyBDcleKXRO78378479;}



void bfnaIfONltqtGKIswHgYOfQqNw35654229() {     float qtUPIfOaijIrufUNPGsWksQ62101998 = -827286144;    float qtUPIfOaijIrufUNPGsWksQ7905044 = -768582728;    float qtUPIfOaijIrufUNPGsWksQ97221477 = -302613873;    float qtUPIfOaijIrufUNPGsWksQ81458938 = -579033176;    float qtUPIfOaijIrufUNPGsWksQ20925739 = -841707909;    float qtUPIfOaijIrufUNPGsWksQ29605990 = -368141968;    float qtUPIfOaijIrufUNPGsWksQ22777260 = -907822919;    float qtUPIfOaijIrufUNPGsWksQ32063012 = -748941360;    float qtUPIfOaijIrufUNPGsWksQ43812454 = 76026861;    float qtUPIfOaijIrufUNPGsWksQ28715855 = -691791835;    float qtUPIfOaijIrufUNPGsWksQ37303301 = -600167102;    float qtUPIfOaijIrufUNPGsWksQ41493699 = -152699683;    float qtUPIfOaijIrufUNPGsWksQ35156954 = -786473062;    float qtUPIfOaijIrufUNPGsWksQ23559747 = -516539488;    float qtUPIfOaijIrufUNPGsWksQ55666990 = -597483311;    float qtUPIfOaijIrufUNPGsWksQ47003110 = -662989388;    float qtUPIfOaijIrufUNPGsWksQ46523176 = -30128591;    float qtUPIfOaijIrufUNPGsWksQ53853321 = -704107194;    float qtUPIfOaijIrufUNPGsWksQ39781281 = -718572382;    float qtUPIfOaijIrufUNPGsWksQ25363339 = -283186325;    float qtUPIfOaijIrufUNPGsWksQ35859650 = -796213648;    float qtUPIfOaijIrufUNPGsWksQ83483509 = -196711252;    float qtUPIfOaijIrufUNPGsWksQ99716975 = -809155738;    float qtUPIfOaijIrufUNPGsWksQ78196436 = -817679569;    float qtUPIfOaijIrufUNPGsWksQ27848273 = -254386784;    float qtUPIfOaijIrufUNPGsWksQ40863587 = 63153455;    float qtUPIfOaijIrufUNPGsWksQ71562661 = -374901399;    float qtUPIfOaijIrufUNPGsWksQ15253907 = -840679512;    float qtUPIfOaijIrufUNPGsWksQ40934024 = -956503772;    float qtUPIfOaijIrufUNPGsWksQ25269796 = -938296020;    float qtUPIfOaijIrufUNPGsWksQ33034067 = 87027126;    float qtUPIfOaijIrufUNPGsWksQ67223429 = -784408417;    float qtUPIfOaijIrufUNPGsWksQ39006396 = -358850203;    float qtUPIfOaijIrufUNPGsWksQ58801027 = 2214823;    float qtUPIfOaijIrufUNPGsWksQ42801825 = 33542952;    float qtUPIfOaijIrufUNPGsWksQ98036019 = -753250285;    float qtUPIfOaijIrufUNPGsWksQ84688918 = -295920121;    float qtUPIfOaijIrufUNPGsWksQ71653348 = -108081355;    float qtUPIfOaijIrufUNPGsWksQ59715896 = -451766029;    float qtUPIfOaijIrufUNPGsWksQ46935204 = -609316416;    float qtUPIfOaijIrufUNPGsWksQ13638620 = -756491094;    float qtUPIfOaijIrufUNPGsWksQ64083995 = -683966408;    float qtUPIfOaijIrufUNPGsWksQ41213466 = -651955280;    float qtUPIfOaijIrufUNPGsWksQ68556209 = -659137235;    float qtUPIfOaijIrufUNPGsWksQ78582934 = -693816195;    float qtUPIfOaijIrufUNPGsWksQ18188563 = -858972856;    float qtUPIfOaijIrufUNPGsWksQ97393860 = -491224288;    float qtUPIfOaijIrufUNPGsWksQ44156660 = -184991008;    float qtUPIfOaijIrufUNPGsWksQ95900682 = -526325507;    float qtUPIfOaijIrufUNPGsWksQ76704381 = -359199990;    float qtUPIfOaijIrufUNPGsWksQ32311467 = -802603479;    float qtUPIfOaijIrufUNPGsWksQ2517160 = -493387728;    float qtUPIfOaijIrufUNPGsWksQ15669591 = 67951273;    float qtUPIfOaijIrufUNPGsWksQ9545343 = -915195043;    float qtUPIfOaijIrufUNPGsWksQ99794579 = -10252658;    float qtUPIfOaijIrufUNPGsWksQ78618488 = -530574893;    float qtUPIfOaijIrufUNPGsWksQ8188068 = -959426991;    float qtUPIfOaijIrufUNPGsWksQ19025042 = -484934305;    float qtUPIfOaijIrufUNPGsWksQ53610666 = -224646393;    float qtUPIfOaijIrufUNPGsWksQ80062152 = -804861364;    float qtUPIfOaijIrufUNPGsWksQ58043329 = -993240570;    float qtUPIfOaijIrufUNPGsWksQ7523353 = 32856592;    float qtUPIfOaijIrufUNPGsWksQ91128988 = -792437588;    float qtUPIfOaijIrufUNPGsWksQ18542658 = 14322881;    float qtUPIfOaijIrufUNPGsWksQ95681787 = -678818962;    float qtUPIfOaijIrufUNPGsWksQ70079871 = -815758685;    float qtUPIfOaijIrufUNPGsWksQ2487303 = -793849481;    float qtUPIfOaijIrufUNPGsWksQ76355926 = -688687885;    float qtUPIfOaijIrufUNPGsWksQ80757921 = -450082441;    float qtUPIfOaijIrufUNPGsWksQ57630971 = -844233026;    float qtUPIfOaijIrufUNPGsWksQ62314191 = -267069268;    float qtUPIfOaijIrufUNPGsWksQ74869828 = -922047236;    float qtUPIfOaijIrufUNPGsWksQ94137424 = -152341166;    float qtUPIfOaijIrufUNPGsWksQ92846077 = -9255966;    float qtUPIfOaijIrufUNPGsWksQ11724720 = -526695231;    float qtUPIfOaijIrufUNPGsWksQ71775655 = -12247241;    float qtUPIfOaijIrufUNPGsWksQ42270043 = -544755973;    float qtUPIfOaijIrufUNPGsWksQ31160767 = -50018503;    float qtUPIfOaijIrufUNPGsWksQ99613501 = -23863374;    float qtUPIfOaijIrufUNPGsWksQ9659710 = -395413928;    float qtUPIfOaijIrufUNPGsWksQ43469726 = -445622258;    float qtUPIfOaijIrufUNPGsWksQ27406001 = -89910392;    float qtUPIfOaijIrufUNPGsWksQ19353225 = -214354005;    float qtUPIfOaijIrufUNPGsWksQ64229642 = -497303782;    float qtUPIfOaijIrufUNPGsWksQ92958328 = -35692542;    float qtUPIfOaijIrufUNPGsWksQ30516908 = -419585147;    float qtUPIfOaijIrufUNPGsWksQ51553839 = -752359691;    float qtUPIfOaijIrufUNPGsWksQ29461053 = -443655161;    float qtUPIfOaijIrufUNPGsWksQ59006448 = -987532520;    float qtUPIfOaijIrufUNPGsWksQ64183337 = -435882156;    float qtUPIfOaijIrufUNPGsWksQ89847951 = -793823295;    float qtUPIfOaijIrufUNPGsWksQ65663877 = -810985816;    float qtUPIfOaijIrufUNPGsWksQ18042683 = -883434963;    float qtUPIfOaijIrufUNPGsWksQ79653744 = -646904665;    float qtUPIfOaijIrufUNPGsWksQ88891874 = -616075847;    float qtUPIfOaijIrufUNPGsWksQ6115267 = -689347687;    float qtUPIfOaijIrufUNPGsWksQ72955007 = -891528820;    float qtUPIfOaijIrufUNPGsWksQ22670809 = -566278161;    float qtUPIfOaijIrufUNPGsWksQ72874421 = -980318274;    float qtUPIfOaijIrufUNPGsWksQ8503064 = -827286144;     qtUPIfOaijIrufUNPGsWksQ62101998 = qtUPIfOaijIrufUNPGsWksQ7905044;     qtUPIfOaijIrufUNPGsWksQ7905044 = qtUPIfOaijIrufUNPGsWksQ97221477;     qtUPIfOaijIrufUNPGsWksQ97221477 = qtUPIfOaijIrufUNPGsWksQ81458938;     qtUPIfOaijIrufUNPGsWksQ81458938 = qtUPIfOaijIrufUNPGsWksQ20925739;     qtUPIfOaijIrufUNPGsWksQ20925739 = qtUPIfOaijIrufUNPGsWksQ29605990;     qtUPIfOaijIrufUNPGsWksQ29605990 = qtUPIfOaijIrufUNPGsWksQ22777260;     qtUPIfOaijIrufUNPGsWksQ22777260 = qtUPIfOaijIrufUNPGsWksQ32063012;     qtUPIfOaijIrufUNPGsWksQ32063012 = qtUPIfOaijIrufUNPGsWksQ43812454;     qtUPIfOaijIrufUNPGsWksQ43812454 = qtUPIfOaijIrufUNPGsWksQ28715855;     qtUPIfOaijIrufUNPGsWksQ28715855 = qtUPIfOaijIrufUNPGsWksQ37303301;     qtUPIfOaijIrufUNPGsWksQ37303301 = qtUPIfOaijIrufUNPGsWksQ41493699;     qtUPIfOaijIrufUNPGsWksQ41493699 = qtUPIfOaijIrufUNPGsWksQ35156954;     qtUPIfOaijIrufUNPGsWksQ35156954 = qtUPIfOaijIrufUNPGsWksQ23559747;     qtUPIfOaijIrufUNPGsWksQ23559747 = qtUPIfOaijIrufUNPGsWksQ55666990;     qtUPIfOaijIrufUNPGsWksQ55666990 = qtUPIfOaijIrufUNPGsWksQ47003110;     qtUPIfOaijIrufUNPGsWksQ47003110 = qtUPIfOaijIrufUNPGsWksQ46523176;     qtUPIfOaijIrufUNPGsWksQ46523176 = qtUPIfOaijIrufUNPGsWksQ53853321;     qtUPIfOaijIrufUNPGsWksQ53853321 = qtUPIfOaijIrufUNPGsWksQ39781281;     qtUPIfOaijIrufUNPGsWksQ39781281 = qtUPIfOaijIrufUNPGsWksQ25363339;     qtUPIfOaijIrufUNPGsWksQ25363339 = qtUPIfOaijIrufUNPGsWksQ35859650;     qtUPIfOaijIrufUNPGsWksQ35859650 = qtUPIfOaijIrufUNPGsWksQ83483509;     qtUPIfOaijIrufUNPGsWksQ83483509 = qtUPIfOaijIrufUNPGsWksQ99716975;     qtUPIfOaijIrufUNPGsWksQ99716975 = qtUPIfOaijIrufUNPGsWksQ78196436;     qtUPIfOaijIrufUNPGsWksQ78196436 = qtUPIfOaijIrufUNPGsWksQ27848273;     qtUPIfOaijIrufUNPGsWksQ27848273 = qtUPIfOaijIrufUNPGsWksQ40863587;     qtUPIfOaijIrufUNPGsWksQ40863587 = qtUPIfOaijIrufUNPGsWksQ71562661;     qtUPIfOaijIrufUNPGsWksQ71562661 = qtUPIfOaijIrufUNPGsWksQ15253907;     qtUPIfOaijIrufUNPGsWksQ15253907 = qtUPIfOaijIrufUNPGsWksQ40934024;     qtUPIfOaijIrufUNPGsWksQ40934024 = qtUPIfOaijIrufUNPGsWksQ25269796;     qtUPIfOaijIrufUNPGsWksQ25269796 = qtUPIfOaijIrufUNPGsWksQ33034067;     qtUPIfOaijIrufUNPGsWksQ33034067 = qtUPIfOaijIrufUNPGsWksQ67223429;     qtUPIfOaijIrufUNPGsWksQ67223429 = qtUPIfOaijIrufUNPGsWksQ39006396;     qtUPIfOaijIrufUNPGsWksQ39006396 = qtUPIfOaijIrufUNPGsWksQ58801027;     qtUPIfOaijIrufUNPGsWksQ58801027 = qtUPIfOaijIrufUNPGsWksQ42801825;     qtUPIfOaijIrufUNPGsWksQ42801825 = qtUPIfOaijIrufUNPGsWksQ98036019;     qtUPIfOaijIrufUNPGsWksQ98036019 = qtUPIfOaijIrufUNPGsWksQ84688918;     qtUPIfOaijIrufUNPGsWksQ84688918 = qtUPIfOaijIrufUNPGsWksQ71653348;     qtUPIfOaijIrufUNPGsWksQ71653348 = qtUPIfOaijIrufUNPGsWksQ59715896;     qtUPIfOaijIrufUNPGsWksQ59715896 = qtUPIfOaijIrufUNPGsWksQ46935204;     qtUPIfOaijIrufUNPGsWksQ46935204 = qtUPIfOaijIrufUNPGsWksQ13638620;     qtUPIfOaijIrufUNPGsWksQ13638620 = qtUPIfOaijIrufUNPGsWksQ64083995;     qtUPIfOaijIrufUNPGsWksQ64083995 = qtUPIfOaijIrufUNPGsWksQ41213466;     qtUPIfOaijIrufUNPGsWksQ41213466 = qtUPIfOaijIrufUNPGsWksQ68556209;     qtUPIfOaijIrufUNPGsWksQ68556209 = qtUPIfOaijIrufUNPGsWksQ78582934;     qtUPIfOaijIrufUNPGsWksQ78582934 = qtUPIfOaijIrufUNPGsWksQ18188563;     qtUPIfOaijIrufUNPGsWksQ18188563 = qtUPIfOaijIrufUNPGsWksQ97393860;     qtUPIfOaijIrufUNPGsWksQ97393860 = qtUPIfOaijIrufUNPGsWksQ44156660;     qtUPIfOaijIrufUNPGsWksQ44156660 = qtUPIfOaijIrufUNPGsWksQ95900682;     qtUPIfOaijIrufUNPGsWksQ95900682 = qtUPIfOaijIrufUNPGsWksQ76704381;     qtUPIfOaijIrufUNPGsWksQ76704381 = qtUPIfOaijIrufUNPGsWksQ32311467;     qtUPIfOaijIrufUNPGsWksQ32311467 = qtUPIfOaijIrufUNPGsWksQ2517160;     qtUPIfOaijIrufUNPGsWksQ2517160 = qtUPIfOaijIrufUNPGsWksQ15669591;     qtUPIfOaijIrufUNPGsWksQ15669591 = qtUPIfOaijIrufUNPGsWksQ9545343;     qtUPIfOaijIrufUNPGsWksQ9545343 = qtUPIfOaijIrufUNPGsWksQ99794579;     qtUPIfOaijIrufUNPGsWksQ99794579 = qtUPIfOaijIrufUNPGsWksQ78618488;     qtUPIfOaijIrufUNPGsWksQ78618488 = qtUPIfOaijIrufUNPGsWksQ8188068;     qtUPIfOaijIrufUNPGsWksQ8188068 = qtUPIfOaijIrufUNPGsWksQ19025042;     qtUPIfOaijIrufUNPGsWksQ19025042 = qtUPIfOaijIrufUNPGsWksQ53610666;     qtUPIfOaijIrufUNPGsWksQ53610666 = qtUPIfOaijIrufUNPGsWksQ80062152;     qtUPIfOaijIrufUNPGsWksQ80062152 = qtUPIfOaijIrufUNPGsWksQ58043329;     qtUPIfOaijIrufUNPGsWksQ58043329 = qtUPIfOaijIrufUNPGsWksQ7523353;     qtUPIfOaijIrufUNPGsWksQ7523353 = qtUPIfOaijIrufUNPGsWksQ91128988;     qtUPIfOaijIrufUNPGsWksQ91128988 = qtUPIfOaijIrufUNPGsWksQ18542658;     qtUPIfOaijIrufUNPGsWksQ18542658 = qtUPIfOaijIrufUNPGsWksQ95681787;     qtUPIfOaijIrufUNPGsWksQ95681787 = qtUPIfOaijIrufUNPGsWksQ70079871;     qtUPIfOaijIrufUNPGsWksQ70079871 = qtUPIfOaijIrufUNPGsWksQ2487303;     qtUPIfOaijIrufUNPGsWksQ2487303 = qtUPIfOaijIrufUNPGsWksQ76355926;     qtUPIfOaijIrufUNPGsWksQ76355926 = qtUPIfOaijIrufUNPGsWksQ80757921;     qtUPIfOaijIrufUNPGsWksQ80757921 = qtUPIfOaijIrufUNPGsWksQ57630971;     qtUPIfOaijIrufUNPGsWksQ57630971 = qtUPIfOaijIrufUNPGsWksQ62314191;     qtUPIfOaijIrufUNPGsWksQ62314191 = qtUPIfOaijIrufUNPGsWksQ74869828;     qtUPIfOaijIrufUNPGsWksQ74869828 = qtUPIfOaijIrufUNPGsWksQ94137424;     qtUPIfOaijIrufUNPGsWksQ94137424 = qtUPIfOaijIrufUNPGsWksQ92846077;     qtUPIfOaijIrufUNPGsWksQ92846077 = qtUPIfOaijIrufUNPGsWksQ11724720;     qtUPIfOaijIrufUNPGsWksQ11724720 = qtUPIfOaijIrufUNPGsWksQ71775655;     qtUPIfOaijIrufUNPGsWksQ71775655 = qtUPIfOaijIrufUNPGsWksQ42270043;     qtUPIfOaijIrufUNPGsWksQ42270043 = qtUPIfOaijIrufUNPGsWksQ31160767;     qtUPIfOaijIrufUNPGsWksQ31160767 = qtUPIfOaijIrufUNPGsWksQ99613501;     qtUPIfOaijIrufUNPGsWksQ99613501 = qtUPIfOaijIrufUNPGsWksQ9659710;     qtUPIfOaijIrufUNPGsWksQ9659710 = qtUPIfOaijIrufUNPGsWksQ43469726;     qtUPIfOaijIrufUNPGsWksQ43469726 = qtUPIfOaijIrufUNPGsWksQ27406001;     qtUPIfOaijIrufUNPGsWksQ27406001 = qtUPIfOaijIrufUNPGsWksQ19353225;     qtUPIfOaijIrufUNPGsWksQ19353225 = qtUPIfOaijIrufUNPGsWksQ64229642;     qtUPIfOaijIrufUNPGsWksQ64229642 = qtUPIfOaijIrufUNPGsWksQ92958328;     qtUPIfOaijIrufUNPGsWksQ92958328 = qtUPIfOaijIrufUNPGsWksQ30516908;     qtUPIfOaijIrufUNPGsWksQ30516908 = qtUPIfOaijIrufUNPGsWksQ51553839;     qtUPIfOaijIrufUNPGsWksQ51553839 = qtUPIfOaijIrufUNPGsWksQ29461053;     qtUPIfOaijIrufUNPGsWksQ29461053 = qtUPIfOaijIrufUNPGsWksQ59006448;     qtUPIfOaijIrufUNPGsWksQ59006448 = qtUPIfOaijIrufUNPGsWksQ64183337;     qtUPIfOaijIrufUNPGsWksQ64183337 = qtUPIfOaijIrufUNPGsWksQ89847951;     qtUPIfOaijIrufUNPGsWksQ89847951 = qtUPIfOaijIrufUNPGsWksQ65663877;     qtUPIfOaijIrufUNPGsWksQ65663877 = qtUPIfOaijIrufUNPGsWksQ18042683;     qtUPIfOaijIrufUNPGsWksQ18042683 = qtUPIfOaijIrufUNPGsWksQ79653744;     qtUPIfOaijIrufUNPGsWksQ79653744 = qtUPIfOaijIrufUNPGsWksQ88891874;     qtUPIfOaijIrufUNPGsWksQ88891874 = qtUPIfOaijIrufUNPGsWksQ6115267;     qtUPIfOaijIrufUNPGsWksQ6115267 = qtUPIfOaijIrufUNPGsWksQ72955007;     qtUPIfOaijIrufUNPGsWksQ72955007 = qtUPIfOaijIrufUNPGsWksQ22670809;     qtUPIfOaijIrufUNPGsWksQ22670809 = qtUPIfOaijIrufUNPGsWksQ72874421;     qtUPIfOaijIrufUNPGsWksQ72874421 = qtUPIfOaijIrufUNPGsWksQ8503064;     qtUPIfOaijIrufUNPGsWksQ8503064 = qtUPIfOaijIrufUNPGsWksQ62101998;}



void LBYohsabKeWzJVgUaCicTMgOgr20717067() {     float JSFSsSKXSqKigMbihOPfNgR45825516 = -77308211;    float JSFSsSKXSqKigMbihOPfNgR5503139 = -572234579;    float JSFSsSKXSqKigMbihOPfNgR85807594 = -170921564;    float JSFSsSKXSqKigMbihOPfNgR78606891 = -583915105;    float JSFSsSKXSqKigMbihOPfNgR487076 = -139656100;    float JSFSsSKXSqKigMbihOPfNgR40945508 = -471462637;    float JSFSsSKXSqKigMbihOPfNgR59657822 = -929337057;    float JSFSsSKXSqKigMbihOPfNgR75403094 = -269395946;    float JSFSsSKXSqKigMbihOPfNgR94031981 = -621455835;    float JSFSsSKXSqKigMbihOPfNgR74821443 = -914832851;    float JSFSsSKXSqKigMbihOPfNgR80192337 = -440452359;    float JSFSsSKXSqKigMbihOPfNgR31705470 = -81838954;    float JSFSsSKXSqKigMbihOPfNgR86210805 = -629305306;    float JSFSsSKXSqKigMbihOPfNgR17932892 = -866222351;    float JSFSsSKXSqKigMbihOPfNgR59846248 = -523180616;    float JSFSsSKXSqKigMbihOPfNgR18817139 = -587117039;    float JSFSsSKXSqKigMbihOPfNgR46388586 = -189008453;    float JSFSsSKXSqKigMbihOPfNgR67930591 = -314958768;    float JSFSsSKXSqKigMbihOPfNgR80757762 = -507221621;    float JSFSsSKXSqKigMbihOPfNgR99470982 = -999479242;    float JSFSsSKXSqKigMbihOPfNgR3199161 = -370680169;    float JSFSsSKXSqKigMbihOPfNgR84485580 = -109866538;    float JSFSsSKXSqKigMbihOPfNgR68892840 = -838550475;    float JSFSsSKXSqKigMbihOPfNgR79339131 = -356912222;    float JSFSsSKXSqKigMbihOPfNgR64450365 = -332717546;    float JSFSsSKXSqKigMbihOPfNgR93856344 = -379813573;    float JSFSsSKXSqKigMbihOPfNgR93154600 = -818776822;    float JSFSsSKXSqKigMbihOPfNgR57853107 = -627121000;    float JSFSsSKXSqKigMbihOPfNgR12852166 = -380699672;    float JSFSsSKXSqKigMbihOPfNgR60818123 = -510902395;    float JSFSsSKXSqKigMbihOPfNgR14398491 = -132070895;    float JSFSsSKXSqKigMbihOPfNgR92016717 = -233605640;    float JSFSsSKXSqKigMbihOPfNgR53131097 = 75935706;    float JSFSsSKXSqKigMbihOPfNgR94525741 = -401881825;    float JSFSsSKXSqKigMbihOPfNgR10405151 = -800747655;    float JSFSsSKXSqKigMbihOPfNgR67897252 = -731646763;    float JSFSsSKXSqKigMbihOPfNgR84452509 = -616261417;    float JSFSsSKXSqKigMbihOPfNgR22035139 = -7224572;    float JSFSsSKXSqKigMbihOPfNgR20331616 = -757324910;    float JSFSsSKXSqKigMbihOPfNgR86124319 = -555963296;    float JSFSsSKXSqKigMbihOPfNgR78406140 = -406531221;    float JSFSsSKXSqKigMbihOPfNgR93423722 = -289992077;    float JSFSsSKXSqKigMbihOPfNgR74013612 = -304359534;    float JSFSsSKXSqKigMbihOPfNgR15751728 = -238080968;    float JSFSsSKXSqKigMbihOPfNgR6570186 = 80169727;    float JSFSsSKXSqKigMbihOPfNgR62455444 = -474004548;    float JSFSsSKXSqKigMbihOPfNgR30572830 = -512870195;    float JSFSsSKXSqKigMbihOPfNgR82898507 = 52887094;    float JSFSsSKXSqKigMbihOPfNgR29774896 = -226109658;    float JSFSsSKXSqKigMbihOPfNgR12577712 = -991215426;    float JSFSsSKXSqKigMbihOPfNgR72402298 = -968375880;    float JSFSsSKXSqKigMbihOPfNgR54022879 = -245440515;    float JSFSsSKXSqKigMbihOPfNgR61051993 = 56018907;    float JSFSsSKXSqKigMbihOPfNgR55477949 = -391904231;    float JSFSsSKXSqKigMbihOPfNgR17517907 = -326979603;    float JSFSsSKXSqKigMbihOPfNgR61339935 = -967441673;    float JSFSsSKXSqKigMbihOPfNgR36610298 = -733684105;    float JSFSsSKXSqKigMbihOPfNgR6468464 = -814009343;    float JSFSsSKXSqKigMbihOPfNgR14156526 = -151197560;    float JSFSsSKXSqKigMbihOPfNgR6630732 = -759842528;    float JSFSsSKXSqKigMbihOPfNgR47790907 = -652685815;    float JSFSsSKXSqKigMbihOPfNgR1804716 = -202216058;    float JSFSsSKXSqKigMbihOPfNgR62550929 = -888696274;    float JSFSsSKXSqKigMbihOPfNgR33213859 = -10553441;    float JSFSsSKXSqKigMbihOPfNgR60422953 = -682761957;    float JSFSsSKXSqKigMbihOPfNgR88175619 = -106846720;    float JSFSsSKXSqKigMbihOPfNgR78574372 = -57774660;    float JSFSsSKXSqKigMbihOPfNgR91685064 = -127423481;    float JSFSsSKXSqKigMbihOPfNgR7527741 = 34525304;    float JSFSsSKXSqKigMbihOPfNgR91948995 = -791533854;    float JSFSsSKXSqKigMbihOPfNgR34364630 = -970855623;    float JSFSsSKXSqKigMbihOPfNgR24353448 = -81783882;    float JSFSsSKXSqKigMbihOPfNgR47598975 = -557633859;    float JSFSsSKXSqKigMbihOPfNgR94633442 = -951258325;    float JSFSsSKXSqKigMbihOPfNgR21064843 = -492948021;    float JSFSsSKXSqKigMbihOPfNgR9775439 = 19311908;    float JSFSsSKXSqKigMbihOPfNgR10471968 = -805507005;    float JSFSsSKXSqKigMbihOPfNgR53141112 = -500469507;    float JSFSsSKXSqKigMbihOPfNgR72768946 = -337081949;    float JSFSsSKXSqKigMbihOPfNgR1994922 = -858712998;    float JSFSsSKXSqKigMbihOPfNgR63283514 = -866943379;    float JSFSsSKXSqKigMbihOPfNgR10256093 = -771663917;    float JSFSsSKXSqKigMbihOPfNgR28078212 = -301011342;    float JSFSsSKXSqKigMbihOPfNgR274454 = -389484247;    float JSFSsSKXSqKigMbihOPfNgR88415824 = -542526515;    float JSFSsSKXSqKigMbihOPfNgR60375611 = -886630381;    float JSFSsSKXSqKigMbihOPfNgR30964725 = -189624548;    float JSFSsSKXSqKigMbihOPfNgR97653148 = -532160064;    float JSFSsSKXSqKigMbihOPfNgR77007834 = 25097777;    float JSFSsSKXSqKigMbihOPfNgR49065215 = -833305983;    float JSFSsSKXSqKigMbihOPfNgR31286955 = -997962658;    float JSFSsSKXSqKigMbihOPfNgR77984045 = -802252075;    float JSFSsSKXSqKigMbihOPfNgR7878614 = -856027012;    float JSFSsSKXSqKigMbihOPfNgR13700885 = -997482383;    float JSFSsSKXSqKigMbihOPfNgR38333412 = -903277481;    float JSFSsSKXSqKigMbihOPfNgR76601424 = -104315163;    float JSFSsSKXSqKigMbihOPfNgR30872793 = -401295804;    float JSFSsSKXSqKigMbihOPfNgR40799754 = -193806093;    float JSFSsSKXSqKigMbihOPfNgR55328775 = -555319012;    float JSFSsSKXSqKigMbihOPfNgR18394567 = -77308211;     JSFSsSKXSqKigMbihOPfNgR45825516 = JSFSsSKXSqKigMbihOPfNgR5503139;     JSFSsSKXSqKigMbihOPfNgR5503139 = JSFSsSKXSqKigMbihOPfNgR85807594;     JSFSsSKXSqKigMbihOPfNgR85807594 = JSFSsSKXSqKigMbihOPfNgR78606891;     JSFSsSKXSqKigMbihOPfNgR78606891 = JSFSsSKXSqKigMbihOPfNgR487076;     JSFSsSKXSqKigMbihOPfNgR487076 = JSFSsSKXSqKigMbihOPfNgR40945508;     JSFSsSKXSqKigMbihOPfNgR40945508 = JSFSsSKXSqKigMbihOPfNgR59657822;     JSFSsSKXSqKigMbihOPfNgR59657822 = JSFSsSKXSqKigMbihOPfNgR75403094;     JSFSsSKXSqKigMbihOPfNgR75403094 = JSFSsSKXSqKigMbihOPfNgR94031981;     JSFSsSKXSqKigMbihOPfNgR94031981 = JSFSsSKXSqKigMbihOPfNgR74821443;     JSFSsSKXSqKigMbihOPfNgR74821443 = JSFSsSKXSqKigMbihOPfNgR80192337;     JSFSsSKXSqKigMbihOPfNgR80192337 = JSFSsSKXSqKigMbihOPfNgR31705470;     JSFSsSKXSqKigMbihOPfNgR31705470 = JSFSsSKXSqKigMbihOPfNgR86210805;     JSFSsSKXSqKigMbihOPfNgR86210805 = JSFSsSKXSqKigMbihOPfNgR17932892;     JSFSsSKXSqKigMbihOPfNgR17932892 = JSFSsSKXSqKigMbihOPfNgR59846248;     JSFSsSKXSqKigMbihOPfNgR59846248 = JSFSsSKXSqKigMbihOPfNgR18817139;     JSFSsSKXSqKigMbihOPfNgR18817139 = JSFSsSKXSqKigMbihOPfNgR46388586;     JSFSsSKXSqKigMbihOPfNgR46388586 = JSFSsSKXSqKigMbihOPfNgR67930591;     JSFSsSKXSqKigMbihOPfNgR67930591 = JSFSsSKXSqKigMbihOPfNgR80757762;     JSFSsSKXSqKigMbihOPfNgR80757762 = JSFSsSKXSqKigMbihOPfNgR99470982;     JSFSsSKXSqKigMbihOPfNgR99470982 = JSFSsSKXSqKigMbihOPfNgR3199161;     JSFSsSKXSqKigMbihOPfNgR3199161 = JSFSsSKXSqKigMbihOPfNgR84485580;     JSFSsSKXSqKigMbihOPfNgR84485580 = JSFSsSKXSqKigMbihOPfNgR68892840;     JSFSsSKXSqKigMbihOPfNgR68892840 = JSFSsSKXSqKigMbihOPfNgR79339131;     JSFSsSKXSqKigMbihOPfNgR79339131 = JSFSsSKXSqKigMbihOPfNgR64450365;     JSFSsSKXSqKigMbihOPfNgR64450365 = JSFSsSKXSqKigMbihOPfNgR93856344;     JSFSsSKXSqKigMbihOPfNgR93856344 = JSFSsSKXSqKigMbihOPfNgR93154600;     JSFSsSKXSqKigMbihOPfNgR93154600 = JSFSsSKXSqKigMbihOPfNgR57853107;     JSFSsSKXSqKigMbihOPfNgR57853107 = JSFSsSKXSqKigMbihOPfNgR12852166;     JSFSsSKXSqKigMbihOPfNgR12852166 = JSFSsSKXSqKigMbihOPfNgR60818123;     JSFSsSKXSqKigMbihOPfNgR60818123 = JSFSsSKXSqKigMbihOPfNgR14398491;     JSFSsSKXSqKigMbihOPfNgR14398491 = JSFSsSKXSqKigMbihOPfNgR92016717;     JSFSsSKXSqKigMbihOPfNgR92016717 = JSFSsSKXSqKigMbihOPfNgR53131097;     JSFSsSKXSqKigMbihOPfNgR53131097 = JSFSsSKXSqKigMbihOPfNgR94525741;     JSFSsSKXSqKigMbihOPfNgR94525741 = JSFSsSKXSqKigMbihOPfNgR10405151;     JSFSsSKXSqKigMbihOPfNgR10405151 = JSFSsSKXSqKigMbihOPfNgR67897252;     JSFSsSKXSqKigMbihOPfNgR67897252 = JSFSsSKXSqKigMbihOPfNgR84452509;     JSFSsSKXSqKigMbihOPfNgR84452509 = JSFSsSKXSqKigMbihOPfNgR22035139;     JSFSsSKXSqKigMbihOPfNgR22035139 = JSFSsSKXSqKigMbihOPfNgR20331616;     JSFSsSKXSqKigMbihOPfNgR20331616 = JSFSsSKXSqKigMbihOPfNgR86124319;     JSFSsSKXSqKigMbihOPfNgR86124319 = JSFSsSKXSqKigMbihOPfNgR78406140;     JSFSsSKXSqKigMbihOPfNgR78406140 = JSFSsSKXSqKigMbihOPfNgR93423722;     JSFSsSKXSqKigMbihOPfNgR93423722 = JSFSsSKXSqKigMbihOPfNgR74013612;     JSFSsSKXSqKigMbihOPfNgR74013612 = JSFSsSKXSqKigMbihOPfNgR15751728;     JSFSsSKXSqKigMbihOPfNgR15751728 = JSFSsSKXSqKigMbihOPfNgR6570186;     JSFSsSKXSqKigMbihOPfNgR6570186 = JSFSsSKXSqKigMbihOPfNgR62455444;     JSFSsSKXSqKigMbihOPfNgR62455444 = JSFSsSKXSqKigMbihOPfNgR30572830;     JSFSsSKXSqKigMbihOPfNgR30572830 = JSFSsSKXSqKigMbihOPfNgR82898507;     JSFSsSKXSqKigMbihOPfNgR82898507 = JSFSsSKXSqKigMbihOPfNgR29774896;     JSFSsSKXSqKigMbihOPfNgR29774896 = JSFSsSKXSqKigMbihOPfNgR12577712;     JSFSsSKXSqKigMbihOPfNgR12577712 = JSFSsSKXSqKigMbihOPfNgR72402298;     JSFSsSKXSqKigMbihOPfNgR72402298 = JSFSsSKXSqKigMbihOPfNgR54022879;     JSFSsSKXSqKigMbihOPfNgR54022879 = JSFSsSKXSqKigMbihOPfNgR61051993;     JSFSsSKXSqKigMbihOPfNgR61051993 = JSFSsSKXSqKigMbihOPfNgR55477949;     JSFSsSKXSqKigMbihOPfNgR55477949 = JSFSsSKXSqKigMbihOPfNgR17517907;     JSFSsSKXSqKigMbihOPfNgR17517907 = JSFSsSKXSqKigMbihOPfNgR61339935;     JSFSsSKXSqKigMbihOPfNgR61339935 = JSFSsSKXSqKigMbihOPfNgR36610298;     JSFSsSKXSqKigMbihOPfNgR36610298 = JSFSsSKXSqKigMbihOPfNgR6468464;     JSFSsSKXSqKigMbihOPfNgR6468464 = JSFSsSKXSqKigMbihOPfNgR14156526;     JSFSsSKXSqKigMbihOPfNgR14156526 = JSFSsSKXSqKigMbihOPfNgR6630732;     JSFSsSKXSqKigMbihOPfNgR6630732 = JSFSsSKXSqKigMbihOPfNgR47790907;     JSFSsSKXSqKigMbihOPfNgR47790907 = JSFSsSKXSqKigMbihOPfNgR1804716;     JSFSsSKXSqKigMbihOPfNgR1804716 = JSFSsSKXSqKigMbihOPfNgR62550929;     JSFSsSKXSqKigMbihOPfNgR62550929 = JSFSsSKXSqKigMbihOPfNgR33213859;     JSFSsSKXSqKigMbihOPfNgR33213859 = JSFSsSKXSqKigMbihOPfNgR60422953;     JSFSsSKXSqKigMbihOPfNgR60422953 = JSFSsSKXSqKigMbihOPfNgR88175619;     JSFSsSKXSqKigMbihOPfNgR88175619 = JSFSsSKXSqKigMbihOPfNgR78574372;     JSFSsSKXSqKigMbihOPfNgR78574372 = JSFSsSKXSqKigMbihOPfNgR91685064;     JSFSsSKXSqKigMbihOPfNgR91685064 = JSFSsSKXSqKigMbihOPfNgR7527741;     JSFSsSKXSqKigMbihOPfNgR7527741 = JSFSsSKXSqKigMbihOPfNgR91948995;     JSFSsSKXSqKigMbihOPfNgR91948995 = JSFSsSKXSqKigMbihOPfNgR34364630;     JSFSsSKXSqKigMbihOPfNgR34364630 = JSFSsSKXSqKigMbihOPfNgR24353448;     JSFSsSKXSqKigMbihOPfNgR24353448 = JSFSsSKXSqKigMbihOPfNgR47598975;     JSFSsSKXSqKigMbihOPfNgR47598975 = JSFSsSKXSqKigMbihOPfNgR94633442;     JSFSsSKXSqKigMbihOPfNgR94633442 = JSFSsSKXSqKigMbihOPfNgR21064843;     JSFSsSKXSqKigMbihOPfNgR21064843 = JSFSsSKXSqKigMbihOPfNgR9775439;     JSFSsSKXSqKigMbihOPfNgR9775439 = JSFSsSKXSqKigMbihOPfNgR10471968;     JSFSsSKXSqKigMbihOPfNgR10471968 = JSFSsSKXSqKigMbihOPfNgR53141112;     JSFSsSKXSqKigMbihOPfNgR53141112 = JSFSsSKXSqKigMbihOPfNgR72768946;     JSFSsSKXSqKigMbihOPfNgR72768946 = JSFSsSKXSqKigMbihOPfNgR1994922;     JSFSsSKXSqKigMbihOPfNgR1994922 = JSFSsSKXSqKigMbihOPfNgR63283514;     JSFSsSKXSqKigMbihOPfNgR63283514 = JSFSsSKXSqKigMbihOPfNgR10256093;     JSFSsSKXSqKigMbihOPfNgR10256093 = JSFSsSKXSqKigMbihOPfNgR28078212;     JSFSsSKXSqKigMbihOPfNgR28078212 = JSFSsSKXSqKigMbihOPfNgR274454;     JSFSsSKXSqKigMbihOPfNgR274454 = JSFSsSKXSqKigMbihOPfNgR88415824;     JSFSsSKXSqKigMbihOPfNgR88415824 = JSFSsSKXSqKigMbihOPfNgR60375611;     JSFSsSKXSqKigMbihOPfNgR60375611 = JSFSsSKXSqKigMbihOPfNgR30964725;     JSFSsSKXSqKigMbihOPfNgR30964725 = JSFSsSKXSqKigMbihOPfNgR97653148;     JSFSsSKXSqKigMbihOPfNgR97653148 = JSFSsSKXSqKigMbihOPfNgR77007834;     JSFSsSKXSqKigMbihOPfNgR77007834 = JSFSsSKXSqKigMbihOPfNgR49065215;     JSFSsSKXSqKigMbihOPfNgR49065215 = JSFSsSKXSqKigMbihOPfNgR31286955;     JSFSsSKXSqKigMbihOPfNgR31286955 = JSFSsSKXSqKigMbihOPfNgR77984045;     JSFSsSKXSqKigMbihOPfNgR77984045 = JSFSsSKXSqKigMbihOPfNgR7878614;     JSFSsSKXSqKigMbihOPfNgR7878614 = JSFSsSKXSqKigMbihOPfNgR13700885;     JSFSsSKXSqKigMbihOPfNgR13700885 = JSFSsSKXSqKigMbihOPfNgR38333412;     JSFSsSKXSqKigMbihOPfNgR38333412 = JSFSsSKXSqKigMbihOPfNgR76601424;     JSFSsSKXSqKigMbihOPfNgR76601424 = JSFSsSKXSqKigMbihOPfNgR30872793;     JSFSsSKXSqKigMbihOPfNgR30872793 = JSFSsSKXSqKigMbihOPfNgR40799754;     JSFSsSKXSqKigMbihOPfNgR40799754 = JSFSsSKXSqKigMbihOPfNgR55328775;     JSFSsSKXSqKigMbihOPfNgR55328775 = JSFSsSKXSqKigMbihOPfNgR18394567;     JSFSsSKXSqKigMbihOPfNgR18394567 = JSFSsSKXSqKigMbihOPfNgR45825516;}



void YuTlxchcYSoeDnVnndlyjPkMtY5779905() {     float lgdDOSGxsQzGbDXvpZAlHXg29549034 = -427330278;    float lgdDOSGxsQzGbDXvpZAlHXg3101235 = -375886431;    float lgdDOSGxsQzGbDXvpZAlHXg74393712 = -39229255;    float lgdDOSGxsQzGbDXvpZAlHXg75754844 = -588797034;    float lgdDOSGxsQzGbDXvpZAlHXg80048412 = -537604292;    float lgdDOSGxsQzGbDXvpZAlHXg52285025 = -574783306;    float lgdDOSGxsQzGbDXvpZAlHXg96538385 = -950851195;    float lgdDOSGxsQzGbDXvpZAlHXg18743178 = -889850532;    float lgdDOSGxsQzGbDXvpZAlHXg44251510 = -218938532;    float lgdDOSGxsQzGbDXvpZAlHXg20927032 = -37873867;    float lgdDOSGxsQzGbDXvpZAlHXg23081373 = -280737617;    float lgdDOSGxsQzGbDXvpZAlHXg21917240 = -10978225;    float lgdDOSGxsQzGbDXvpZAlHXg37264658 = -472137549;    float lgdDOSGxsQzGbDXvpZAlHXg12306036 = -115905213;    float lgdDOSGxsQzGbDXvpZAlHXg64025507 = -448877921;    float lgdDOSGxsQzGbDXvpZAlHXg90631167 = -511244691;    float lgdDOSGxsQzGbDXvpZAlHXg46253997 = -347888316;    float lgdDOSGxsQzGbDXvpZAlHXg82007860 = 74189658;    float lgdDOSGxsQzGbDXvpZAlHXg21734243 = -295870859;    float lgdDOSGxsQzGbDXvpZAlHXg73578627 = -615772159;    float lgdDOSGxsQzGbDXvpZAlHXg70538670 = 54853310;    float lgdDOSGxsQzGbDXvpZAlHXg85487651 = -23021825;    float lgdDOSGxsQzGbDXvpZAlHXg38068706 = -867945211;    float lgdDOSGxsQzGbDXvpZAlHXg80481826 = -996144875;    float lgdDOSGxsQzGbDXvpZAlHXg1052459 = -411048307;    float lgdDOSGxsQzGbDXvpZAlHXg46849102 = -822780601;    float lgdDOSGxsQzGbDXvpZAlHXg14746540 = -162652246;    float lgdDOSGxsQzGbDXvpZAlHXg452307 = -413562488;    float lgdDOSGxsQzGbDXvpZAlHXg84770307 = -904895573;    float lgdDOSGxsQzGbDXvpZAlHXg96366450 = -83508769;    float lgdDOSGxsQzGbDXvpZAlHXg95762913 = -351168916;    float lgdDOSGxsQzGbDXvpZAlHXg16810007 = -782802863;    float lgdDOSGxsQzGbDXvpZAlHXg67255798 = -589278386;    float lgdDOSGxsQzGbDXvpZAlHXg30250456 = -805978473;    float lgdDOSGxsQzGbDXvpZAlHXg78008476 = -535038263;    float lgdDOSGxsQzGbDXvpZAlHXg37758486 = -710043240;    float lgdDOSGxsQzGbDXvpZAlHXg84216099 = -936602713;    float lgdDOSGxsQzGbDXvpZAlHXg72416930 = 93632212;    float lgdDOSGxsQzGbDXvpZAlHXg80947334 = 37116209;    float lgdDOSGxsQzGbDXvpZAlHXg25313436 = -502610175;    float lgdDOSGxsQzGbDXvpZAlHXg43173661 = -56571348;    float lgdDOSGxsQzGbDXvpZAlHXg22763449 = -996017747;    float lgdDOSGxsQzGbDXvpZAlHXg6813759 = 43236212;    float lgdDOSGxsQzGbDXvpZAlHXg62947247 = -917024700;    float lgdDOSGxsQzGbDXvpZAlHXg34557436 = -245844352;    float lgdDOSGxsQzGbDXvpZAlHXg6722326 = -89036240;    float lgdDOSGxsQzGbDXvpZAlHXg63751799 = -534516102;    float lgdDOSGxsQzGbDXvpZAlHXg21640355 = -809234804;    float lgdDOSGxsQzGbDXvpZAlHXg63649109 = 74106191;    float lgdDOSGxsQzGbDXvpZAlHXg48451042 = -523230861;    float lgdDOSGxsQzGbDXvpZAlHXg12493131 = -34148282;    float lgdDOSGxsQzGbDXvpZAlHXg5528599 = 2506699;    float lgdDOSGxsQzGbDXvpZAlHXg6434396 = 44086541;    float lgdDOSGxsQzGbDXvpZAlHXg1410555 = -968613418;    float lgdDOSGxsQzGbDXvpZAlHXg35241234 = -643706548;    float lgdDOSGxsQzGbDXvpZAlHXg44061382 = -304308454;    float lgdDOSGxsQzGbDXvpZAlHXg65032528 = -507941220;    float lgdDOSGxsQzGbDXvpZAlHXg93911885 = -43084380;    float lgdDOSGxsQzGbDXvpZAlHXg74702386 = -77748727;    float lgdDOSGxsQzGbDXvpZAlHXg33199310 = -714823691;    float lgdDOSGxsQzGbDXvpZAlHXg37538486 = -312131061;    float lgdDOSGxsQzGbDXvpZAlHXg96086078 = -437288708;    float lgdDOSGxsQzGbDXvpZAlHXg33972871 = -984954960;    float lgdDOSGxsQzGbDXvpZAlHXg47885060 = -35429763;    float lgdDOSGxsQzGbDXvpZAlHXg25164118 = -686704952;    float lgdDOSGxsQzGbDXvpZAlHXg6271367 = -497934755;    float lgdDOSGxsQzGbDXvpZAlHXg54661441 = -421699840;    float lgdDOSGxsQzGbDXvpZAlHXg7014203 = -666159077;    float lgdDOSGxsQzGbDXvpZAlHXg34297560 = -580866951;    float lgdDOSGxsQzGbDXvpZAlHXg26267021 = -738834682;    float lgdDOSGxsQzGbDXvpZAlHXg6415069 = -574641978;    float lgdDOSGxsQzGbDXvpZAlHXg73837067 = -341520528;    float lgdDOSGxsQzGbDXvpZAlHXg1060526 = -962926551;    float lgdDOSGxsQzGbDXvpZAlHXg96420807 = -793260684;    float lgdDOSGxsQzGbDXvpZAlHXg30404967 = -459200812;    float lgdDOSGxsQzGbDXvpZAlHXg47775222 = 50871057;    float lgdDOSGxsQzGbDXvpZAlHXg78673893 = 33741963;    float lgdDOSGxsQzGbDXvpZAlHXg75121458 = -950920512;    float lgdDOSGxsQzGbDXvpZAlHXg45924390 = -650300524;    float lgdDOSGxsQzGbDXvpZAlHXg94330132 = -222012068;    float lgdDOSGxsQzGbDXvpZAlHXg83097302 = -188264499;    float lgdDOSGxsQzGbDXvpZAlHXg93106184 = -353417442;    float lgdDOSGxsQzGbDXvpZAlHXg36803198 = -387668679;    float lgdDOSGxsQzGbDXvpZAlHXg36319265 = -281664712;    float lgdDOSGxsQzGbDXvpZAlHXg83873320 = 50639512;    float lgdDOSGxsQzGbDXvpZAlHXg90234314 = -253675615;    float lgdDOSGxsQzGbDXvpZAlHXg10375612 = -726889404;    float lgdDOSGxsQzGbDXvpZAlHXg65845244 = -620664968;    float lgdDOSGxsQzGbDXvpZAlHXg95009221 = -62271925;    float lgdDOSGxsQzGbDXvpZAlHXg33947094 = -130729809;    float lgdDOSGxsQzGbDXvpZAlHXg72725958 = -102102021;    float lgdDOSGxsQzGbDXvpZAlHXg90304213 = -793518334;    float lgdDOSGxsQzGbDXvpZAlHXg97714544 = -828619062;    float lgdDOSGxsQzGbDXvpZAlHXg47748025 = -248060100;    float lgdDOSGxsQzGbDXvpZAlHXg87774950 = -90479115;    float lgdDOSGxsQzGbDXvpZAlHXg47087582 = -619282640;    float lgdDOSGxsQzGbDXvpZAlHXg88790578 = 88937212;    float lgdDOSGxsQzGbDXvpZAlHXg58928698 = -921334026;    float lgdDOSGxsQzGbDXvpZAlHXg37783130 = -130319749;    float lgdDOSGxsQzGbDXvpZAlHXg28286070 = -427330278;     lgdDOSGxsQzGbDXvpZAlHXg29549034 = lgdDOSGxsQzGbDXvpZAlHXg3101235;     lgdDOSGxsQzGbDXvpZAlHXg3101235 = lgdDOSGxsQzGbDXvpZAlHXg74393712;     lgdDOSGxsQzGbDXvpZAlHXg74393712 = lgdDOSGxsQzGbDXvpZAlHXg75754844;     lgdDOSGxsQzGbDXvpZAlHXg75754844 = lgdDOSGxsQzGbDXvpZAlHXg80048412;     lgdDOSGxsQzGbDXvpZAlHXg80048412 = lgdDOSGxsQzGbDXvpZAlHXg52285025;     lgdDOSGxsQzGbDXvpZAlHXg52285025 = lgdDOSGxsQzGbDXvpZAlHXg96538385;     lgdDOSGxsQzGbDXvpZAlHXg96538385 = lgdDOSGxsQzGbDXvpZAlHXg18743178;     lgdDOSGxsQzGbDXvpZAlHXg18743178 = lgdDOSGxsQzGbDXvpZAlHXg44251510;     lgdDOSGxsQzGbDXvpZAlHXg44251510 = lgdDOSGxsQzGbDXvpZAlHXg20927032;     lgdDOSGxsQzGbDXvpZAlHXg20927032 = lgdDOSGxsQzGbDXvpZAlHXg23081373;     lgdDOSGxsQzGbDXvpZAlHXg23081373 = lgdDOSGxsQzGbDXvpZAlHXg21917240;     lgdDOSGxsQzGbDXvpZAlHXg21917240 = lgdDOSGxsQzGbDXvpZAlHXg37264658;     lgdDOSGxsQzGbDXvpZAlHXg37264658 = lgdDOSGxsQzGbDXvpZAlHXg12306036;     lgdDOSGxsQzGbDXvpZAlHXg12306036 = lgdDOSGxsQzGbDXvpZAlHXg64025507;     lgdDOSGxsQzGbDXvpZAlHXg64025507 = lgdDOSGxsQzGbDXvpZAlHXg90631167;     lgdDOSGxsQzGbDXvpZAlHXg90631167 = lgdDOSGxsQzGbDXvpZAlHXg46253997;     lgdDOSGxsQzGbDXvpZAlHXg46253997 = lgdDOSGxsQzGbDXvpZAlHXg82007860;     lgdDOSGxsQzGbDXvpZAlHXg82007860 = lgdDOSGxsQzGbDXvpZAlHXg21734243;     lgdDOSGxsQzGbDXvpZAlHXg21734243 = lgdDOSGxsQzGbDXvpZAlHXg73578627;     lgdDOSGxsQzGbDXvpZAlHXg73578627 = lgdDOSGxsQzGbDXvpZAlHXg70538670;     lgdDOSGxsQzGbDXvpZAlHXg70538670 = lgdDOSGxsQzGbDXvpZAlHXg85487651;     lgdDOSGxsQzGbDXvpZAlHXg85487651 = lgdDOSGxsQzGbDXvpZAlHXg38068706;     lgdDOSGxsQzGbDXvpZAlHXg38068706 = lgdDOSGxsQzGbDXvpZAlHXg80481826;     lgdDOSGxsQzGbDXvpZAlHXg80481826 = lgdDOSGxsQzGbDXvpZAlHXg1052459;     lgdDOSGxsQzGbDXvpZAlHXg1052459 = lgdDOSGxsQzGbDXvpZAlHXg46849102;     lgdDOSGxsQzGbDXvpZAlHXg46849102 = lgdDOSGxsQzGbDXvpZAlHXg14746540;     lgdDOSGxsQzGbDXvpZAlHXg14746540 = lgdDOSGxsQzGbDXvpZAlHXg452307;     lgdDOSGxsQzGbDXvpZAlHXg452307 = lgdDOSGxsQzGbDXvpZAlHXg84770307;     lgdDOSGxsQzGbDXvpZAlHXg84770307 = lgdDOSGxsQzGbDXvpZAlHXg96366450;     lgdDOSGxsQzGbDXvpZAlHXg96366450 = lgdDOSGxsQzGbDXvpZAlHXg95762913;     lgdDOSGxsQzGbDXvpZAlHXg95762913 = lgdDOSGxsQzGbDXvpZAlHXg16810007;     lgdDOSGxsQzGbDXvpZAlHXg16810007 = lgdDOSGxsQzGbDXvpZAlHXg67255798;     lgdDOSGxsQzGbDXvpZAlHXg67255798 = lgdDOSGxsQzGbDXvpZAlHXg30250456;     lgdDOSGxsQzGbDXvpZAlHXg30250456 = lgdDOSGxsQzGbDXvpZAlHXg78008476;     lgdDOSGxsQzGbDXvpZAlHXg78008476 = lgdDOSGxsQzGbDXvpZAlHXg37758486;     lgdDOSGxsQzGbDXvpZAlHXg37758486 = lgdDOSGxsQzGbDXvpZAlHXg84216099;     lgdDOSGxsQzGbDXvpZAlHXg84216099 = lgdDOSGxsQzGbDXvpZAlHXg72416930;     lgdDOSGxsQzGbDXvpZAlHXg72416930 = lgdDOSGxsQzGbDXvpZAlHXg80947334;     lgdDOSGxsQzGbDXvpZAlHXg80947334 = lgdDOSGxsQzGbDXvpZAlHXg25313436;     lgdDOSGxsQzGbDXvpZAlHXg25313436 = lgdDOSGxsQzGbDXvpZAlHXg43173661;     lgdDOSGxsQzGbDXvpZAlHXg43173661 = lgdDOSGxsQzGbDXvpZAlHXg22763449;     lgdDOSGxsQzGbDXvpZAlHXg22763449 = lgdDOSGxsQzGbDXvpZAlHXg6813759;     lgdDOSGxsQzGbDXvpZAlHXg6813759 = lgdDOSGxsQzGbDXvpZAlHXg62947247;     lgdDOSGxsQzGbDXvpZAlHXg62947247 = lgdDOSGxsQzGbDXvpZAlHXg34557436;     lgdDOSGxsQzGbDXvpZAlHXg34557436 = lgdDOSGxsQzGbDXvpZAlHXg6722326;     lgdDOSGxsQzGbDXvpZAlHXg6722326 = lgdDOSGxsQzGbDXvpZAlHXg63751799;     lgdDOSGxsQzGbDXvpZAlHXg63751799 = lgdDOSGxsQzGbDXvpZAlHXg21640355;     lgdDOSGxsQzGbDXvpZAlHXg21640355 = lgdDOSGxsQzGbDXvpZAlHXg63649109;     lgdDOSGxsQzGbDXvpZAlHXg63649109 = lgdDOSGxsQzGbDXvpZAlHXg48451042;     lgdDOSGxsQzGbDXvpZAlHXg48451042 = lgdDOSGxsQzGbDXvpZAlHXg12493131;     lgdDOSGxsQzGbDXvpZAlHXg12493131 = lgdDOSGxsQzGbDXvpZAlHXg5528599;     lgdDOSGxsQzGbDXvpZAlHXg5528599 = lgdDOSGxsQzGbDXvpZAlHXg6434396;     lgdDOSGxsQzGbDXvpZAlHXg6434396 = lgdDOSGxsQzGbDXvpZAlHXg1410555;     lgdDOSGxsQzGbDXvpZAlHXg1410555 = lgdDOSGxsQzGbDXvpZAlHXg35241234;     lgdDOSGxsQzGbDXvpZAlHXg35241234 = lgdDOSGxsQzGbDXvpZAlHXg44061382;     lgdDOSGxsQzGbDXvpZAlHXg44061382 = lgdDOSGxsQzGbDXvpZAlHXg65032528;     lgdDOSGxsQzGbDXvpZAlHXg65032528 = lgdDOSGxsQzGbDXvpZAlHXg93911885;     lgdDOSGxsQzGbDXvpZAlHXg93911885 = lgdDOSGxsQzGbDXvpZAlHXg74702386;     lgdDOSGxsQzGbDXvpZAlHXg74702386 = lgdDOSGxsQzGbDXvpZAlHXg33199310;     lgdDOSGxsQzGbDXvpZAlHXg33199310 = lgdDOSGxsQzGbDXvpZAlHXg37538486;     lgdDOSGxsQzGbDXvpZAlHXg37538486 = lgdDOSGxsQzGbDXvpZAlHXg96086078;     lgdDOSGxsQzGbDXvpZAlHXg96086078 = lgdDOSGxsQzGbDXvpZAlHXg33972871;     lgdDOSGxsQzGbDXvpZAlHXg33972871 = lgdDOSGxsQzGbDXvpZAlHXg47885060;     lgdDOSGxsQzGbDXvpZAlHXg47885060 = lgdDOSGxsQzGbDXvpZAlHXg25164118;     lgdDOSGxsQzGbDXvpZAlHXg25164118 = lgdDOSGxsQzGbDXvpZAlHXg6271367;     lgdDOSGxsQzGbDXvpZAlHXg6271367 = lgdDOSGxsQzGbDXvpZAlHXg54661441;     lgdDOSGxsQzGbDXvpZAlHXg54661441 = lgdDOSGxsQzGbDXvpZAlHXg7014203;     lgdDOSGxsQzGbDXvpZAlHXg7014203 = lgdDOSGxsQzGbDXvpZAlHXg34297560;     lgdDOSGxsQzGbDXvpZAlHXg34297560 = lgdDOSGxsQzGbDXvpZAlHXg26267021;     lgdDOSGxsQzGbDXvpZAlHXg26267021 = lgdDOSGxsQzGbDXvpZAlHXg6415069;     lgdDOSGxsQzGbDXvpZAlHXg6415069 = lgdDOSGxsQzGbDXvpZAlHXg73837067;     lgdDOSGxsQzGbDXvpZAlHXg73837067 = lgdDOSGxsQzGbDXvpZAlHXg1060526;     lgdDOSGxsQzGbDXvpZAlHXg1060526 = lgdDOSGxsQzGbDXvpZAlHXg96420807;     lgdDOSGxsQzGbDXvpZAlHXg96420807 = lgdDOSGxsQzGbDXvpZAlHXg30404967;     lgdDOSGxsQzGbDXvpZAlHXg30404967 = lgdDOSGxsQzGbDXvpZAlHXg47775222;     lgdDOSGxsQzGbDXvpZAlHXg47775222 = lgdDOSGxsQzGbDXvpZAlHXg78673893;     lgdDOSGxsQzGbDXvpZAlHXg78673893 = lgdDOSGxsQzGbDXvpZAlHXg75121458;     lgdDOSGxsQzGbDXvpZAlHXg75121458 = lgdDOSGxsQzGbDXvpZAlHXg45924390;     lgdDOSGxsQzGbDXvpZAlHXg45924390 = lgdDOSGxsQzGbDXvpZAlHXg94330132;     lgdDOSGxsQzGbDXvpZAlHXg94330132 = lgdDOSGxsQzGbDXvpZAlHXg83097302;     lgdDOSGxsQzGbDXvpZAlHXg83097302 = lgdDOSGxsQzGbDXvpZAlHXg93106184;     lgdDOSGxsQzGbDXvpZAlHXg93106184 = lgdDOSGxsQzGbDXvpZAlHXg36803198;     lgdDOSGxsQzGbDXvpZAlHXg36803198 = lgdDOSGxsQzGbDXvpZAlHXg36319265;     lgdDOSGxsQzGbDXvpZAlHXg36319265 = lgdDOSGxsQzGbDXvpZAlHXg83873320;     lgdDOSGxsQzGbDXvpZAlHXg83873320 = lgdDOSGxsQzGbDXvpZAlHXg90234314;     lgdDOSGxsQzGbDXvpZAlHXg90234314 = lgdDOSGxsQzGbDXvpZAlHXg10375612;     lgdDOSGxsQzGbDXvpZAlHXg10375612 = lgdDOSGxsQzGbDXvpZAlHXg65845244;     lgdDOSGxsQzGbDXvpZAlHXg65845244 = lgdDOSGxsQzGbDXvpZAlHXg95009221;     lgdDOSGxsQzGbDXvpZAlHXg95009221 = lgdDOSGxsQzGbDXvpZAlHXg33947094;     lgdDOSGxsQzGbDXvpZAlHXg33947094 = lgdDOSGxsQzGbDXvpZAlHXg72725958;     lgdDOSGxsQzGbDXvpZAlHXg72725958 = lgdDOSGxsQzGbDXvpZAlHXg90304213;     lgdDOSGxsQzGbDXvpZAlHXg90304213 = lgdDOSGxsQzGbDXvpZAlHXg97714544;     lgdDOSGxsQzGbDXvpZAlHXg97714544 = lgdDOSGxsQzGbDXvpZAlHXg47748025;     lgdDOSGxsQzGbDXvpZAlHXg47748025 = lgdDOSGxsQzGbDXvpZAlHXg87774950;     lgdDOSGxsQzGbDXvpZAlHXg87774950 = lgdDOSGxsQzGbDXvpZAlHXg47087582;     lgdDOSGxsQzGbDXvpZAlHXg47087582 = lgdDOSGxsQzGbDXvpZAlHXg88790578;     lgdDOSGxsQzGbDXvpZAlHXg88790578 = lgdDOSGxsQzGbDXvpZAlHXg58928698;     lgdDOSGxsQzGbDXvpZAlHXg58928698 = lgdDOSGxsQzGbDXvpZAlHXg37783130;     lgdDOSGxsQzGbDXvpZAlHXg37783130 = lgdDOSGxsQzGbDXvpZAlHXg28286070;     lgdDOSGxsQzGbDXvpZAlHXg28286070 = lgdDOSGxsQzGbDXvpZAlHXg29549034;}



void WDptXJanUVajUWRVkEyDXJCtJr90842742() {     float qTlDfKpcpJlWUdPLqhFuLcs13272552 = -777352345;    float qTlDfKpcpJlWUdPLqhFuLcs699330 = -179538282;    float qTlDfKpcpJlWUdPLqhFuLcs62979829 = 92463055;    float qTlDfKpcpJlWUdPLqhFuLcs72902797 = -593678963;    float qTlDfKpcpJlWUdPLqhFuLcs59609749 = -935552483;    float qTlDfKpcpJlWUdPLqhFuLcs63624543 = -678103975;    float qTlDfKpcpJlWUdPLqhFuLcs33418948 = -972365333;    float qTlDfKpcpJlWUdPLqhFuLcs62083260 = -410305118;    float qTlDfKpcpJlWUdPLqhFuLcs94471038 = -916421228;    float qTlDfKpcpJlWUdPLqhFuLcs67032620 = -260914882;    float qTlDfKpcpJlWUdPLqhFuLcs65970409 = -121022875;    float qTlDfKpcpJlWUdPLqhFuLcs12129011 = 59882503;    float qTlDfKpcpJlWUdPLqhFuLcs88318509 = -314969793;    float qTlDfKpcpJlWUdPLqhFuLcs6679181 = -465588076;    float qTlDfKpcpJlWUdPLqhFuLcs68204766 = -374575226;    float qTlDfKpcpJlWUdPLqhFuLcs62445196 = -435372342;    float qTlDfKpcpJlWUdPLqhFuLcs46119408 = -506768179;    float qTlDfKpcpJlWUdPLqhFuLcs96085129 = -636661916;    float qTlDfKpcpJlWUdPLqhFuLcs62710723 = -84520097;    float qTlDfKpcpJlWUdPLqhFuLcs47686272 = -232065076;    float qTlDfKpcpJlWUdPLqhFuLcs37878181 = -619613210;    float qTlDfKpcpJlWUdPLqhFuLcs86489722 = 63822889;    float qTlDfKpcpJlWUdPLqhFuLcs7244571 = -897339948;    float qTlDfKpcpJlWUdPLqhFuLcs81624521 = -535377528;    float qTlDfKpcpJlWUdPLqhFuLcs37654551 = -489379069;    float qTlDfKpcpJlWUdPLqhFuLcs99841859 = -165747629;    float qTlDfKpcpJlWUdPLqhFuLcs36338479 = -606527669;    float qTlDfKpcpJlWUdPLqhFuLcs43051507 = -200003977;    float qTlDfKpcpJlWUdPLqhFuLcs56688448 = -329091473;    float qTlDfKpcpJlWUdPLqhFuLcs31914778 = -756115144;    float qTlDfKpcpJlWUdPLqhFuLcs77127336 = -570266936;    float qTlDfKpcpJlWUdPLqhFuLcs41603295 = -232000085;    float qTlDfKpcpJlWUdPLqhFuLcs81380499 = -154492477;    float qTlDfKpcpJlWUdPLqhFuLcs65975170 = -110075120;    float qTlDfKpcpJlWUdPLqhFuLcs45611802 = -269328871;    float qTlDfKpcpJlWUdPLqhFuLcs7619720 = -688439717;    float qTlDfKpcpJlWUdPLqhFuLcs83979689 = -156944010;    float qTlDfKpcpJlWUdPLqhFuLcs22798721 = -905511005;    float qTlDfKpcpJlWUdPLqhFuLcs41563054 = -268442673;    float qTlDfKpcpJlWUdPLqhFuLcs64502551 = -449257055;    float qTlDfKpcpJlWUdPLqhFuLcs7941182 = -806611474;    float qTlDfKpcpJlWUdPLqhFuLcs52103175 = -602043416;    float qTlDfKpcpJlWUdPLqhFuLcs39613905 = -709168043;    float qTlDfKpcpJlWUdPLqhFuLcs10142767 = -495968433;    float qTlDfKpcpJlWUdPLqhFuLcs62544687 = -571858430;    float qTlDfKpcpJlWUdPLqhFuLcs50989207 = -804067931;    float qTlDfKpcpJlWUdPLqhFuLcs96930768 = -556162009;    float qTlDfKpcpJlWUdPLqhFuLcs60382202 = -571356701;    float qTlDfKpcpJlWUdPLqhFuLcs97523322 = -725677961;    float qTlDfKpcpJlWUdPLqhFuLcs84324371 = -55246297;    float qTlDfKpcpJlWUdPLqhFuLcs52583962 = -199920683;    float qTlDfKpcpJlWUdPLqhFuLcs57034319 = -849546087;    float qTlDfKpcpJlWUdPLqhFuLcs51816797 = 32154176;    float qTlDfKpcpJlWUdPLqhFuLcs47343161 = -445322606;    float qTlDfKpcpJlWUdPLqhFuLcs52964562 = -960433493;    float qTlDfKpcpJlWUdPLqhFuLcs26782829 = -741175235;    float qTlDfKpcpJlWUdPLqhFuLcs93454758 = -282198334;    float qTlDfKpcpJlWUdPLqhFuLcs81355307 = -372159418;    float qTlDfKpcpJlWUdPLqhFuLcs35248246 = -4299895;    float qTlDfKpcpJlWUdPLqhFuLcs59767889 = -669804855;    float qTlDfKpcpJlWUdPLqhFuLcs27286064 = 28423693;    float qTlDfKpcpJlWUdPLqhFuLcs90367441 = -672361357;    float qTlDfKpcpJlWUdPLqhFuLcs5394812 = 18786354;    float qTlDfKpcpJlWUdPLqhFuLcs62556261 = -60306085;    float qTlDfKpcpJlWUdPLqhFuLcs89905283 = -690647947;    float qTlDfKpcpJlWUdPLqhFuLcs24367115 = -889022790;    float qTlDfKpcpJlWUdPLqhFuLcs30748511 = -785625020;    float qTlDfKpcpJlWUdPLqhFuLcs22343340 = -104894673;    float qTlDfKpcpJlWUdPLqhFuLcs61067379 = -96259206;    float qTlDfKpcpJlWUdPLqhFuLcs60585046 = -686135509;    float qTlDfKpcpJlWUdPLqhFuLcs78465506 = -178428333;    float qTlDfKpcpJlWUdPLqhFuLcs23320687 = -601257174;    float qTlDfKpcpJlWUdPLqhFuLcs54522076 = -268219244;    float qTlDfKpcpJlWUdPLqhFuLcs98208171 = -635263043;    float qTlDfKpcpJlWUdPLqhFuLcs39745091 = -425453602;    float qTlDfKpcpJlWUdPLqhFuLcs85775005 = 82430205;    float qTlDfKpcpJlWUdPLqhFuLcs46875818 = -227009069;    float qTlDfKpcpJlWUdPLqhFuLcs97101803 = -301371516;    float qTlDfKpcpJlWUdPLqhFuLcs19079835 = -963519099;    float qTlDfKpcpJlWUdPLqhFuLcs86665344 = -685311138;    float qTlDfKpcpJlWUdPLqhFuLcs2911092 = -609585620;    float qTlDfKpcpJlWUdPLqhFuLcs75956277 = 64829032;    float qTlDfKpcpJlWUdPLqhFuLcs45528185 = -474326017;    float qTlDfKpcpJlWUdPLqhFuLcs72364077 = -173845177;    float qTlDfKpcpJlWUdPLqhFuLcs79330815 = -456194461;    float qTlDfKpcpJlWUdPLqhFuLcs20093018 = -720720849;    float qTlDfKpcpJlWUdPLqhFuLcs89786497 = -164154261;    float qTlDfKpcpJlWUdPLqhFuLcs34037339 = -709169872;    float qTlDfKpcpJlWUdPLqhFuLcs13010608 = -149641628;    float qTlDfKpcpJlWUdPLqhFuLcs18828973 = -528153636;    float qTlDfKpcpJlWUdPLqhFuLcs14164962 = -306241384;    float qTlDfKpcpJlWUdPLqhFuLcs2624382 = -784784593;    float qTlDfKpcpJlWUdPLqhFuLcs87550475 = -801211111;    float qTlDfKpcpJlWUdPLqhFuLcs81795165 = -598637818;    float qTlDfKpcpJlWUdPLqhFuLcs37216488 = -377680748;    float qTlDfKpcpJlWUdPLqhFuLcs17573740 = -34250117;    float qTlDfKpcpJlWUdPLqhFuLcs46708363 = -520829771;    float qTlDfKpcpJlWUdPLqhFuLcs77057643 = -548861958;    float qTlDfKpcpJlWUdPLqhFuLcs20237484 = -805320487;    float qTlDfKpcpJlWUdPLqhFuLcs38177573 = -777352345;     qTlDfKpcpJlWUdPLqhFuLcs13272552 = qTlDfKpcpJlWUdPLqhFuLcs699330;     qTlDfKpcpJlWUdPLqhFuLcs699330 = qTlDfKpcpJlWUdPLqhFuLcs62979829;     qTlDfKpcpJlWUdPLqhFuLcs62979829 = qTlDfKpcpJlWUdPLqhFuLcs72902797;     qTlDfKpcpJlWUdPLqhFuLcs72902797 = qTlDfKpcpJlWUdPLqhFuLcs59609749;     qTlDfKpcpJlWUdPLqhFuLcs59609749 = qTlDfKpcpJlWUdPLqhFuLcs63624543;     qTlDfKpcpJlWUdPLqhFuLcs63624543 = qTlDfKpcpJlWUdPLqhFuLcs33418948;     qTlDfKpcpJlWUdPLqhFuLcs33418948 = qTlDfKpcpJlWUdPLqhFuLcs62083260;     qTlDfKpcpJlWUdPLqhFuLcs62083260 = qTlDfKpcpJlWUdPLqhFuLcs94471038;     qTlDfKpcpJlWUdPLqhFuLcs94471038 = qTlDfKpcpJlWUdPLqhFuLcs67032620;     qTlDfKpcpJlWUdPLqhFuLcs67032620 = qTlDfKpcpJlWUdPLqhFuLcs65970409;     qTlDfKpcpJlWUdPLqhFuLcs65970409 = qTlDfKpcpJlWUdPLqhFuLcs12129011;     qTlDfKpcpJlWUdPLqhFuLcs12129011 = qTlDfKpcpJlWUdPLqhFuLcs88318509;     qTlDfKpcpJlWUdPLqhFuLcs88318509 = qTlDfKpcpJlWUdPLqhFuLcs6679181;     qTlDfKpcpJlWUdPLqhFuLcs6679181 = qTlDfKpcpJlWUdPLqhFuLcs68204766;     qTlDfKpcpJlWUdPLqhFuLcs68204766 = qTlDfKpcpJlWUdPLqhFuLcs62445196;     qTlDfKpcpJlWUdPLqhFuLcs62445196 = qTlDfKpcpJlWUdPLqhFuLcs46119408;     qTlDfKpcpJlWUdPLqhFuLcs46119408 = qTlDfKpcpJlWUdPLqhFuLcs96085129;     qTlDfKpcpJlWUdPLqhFuLcs96085129 = qTlDfKpcpJlWUdPLqhFuLcs62710723;     qTlDfKpcpJlWUdPLqhFuLcs62710723 = qTlDfKpcpJlWUdPLqhFuLcs47686272;     qTlDfKpcpJlWUdPLqhFuLcs47686272 = qTlDfKpcpJlWUdPLqhFuLcs37878181;     qTlDfKpcpJlWUdPLqhFuLcs37878181 = qTlDfKpcpJlWUdPLqhFuLcs86489722;     qTlDfKpcpJlWUdPLqhFuLcs86489722 = qTlDfKpcpJlWUdPLqhFuLcs7244571;     qTlDfKpcpJlWUdPLqhFuLcs7244571 = qTlDfKpcpJlWUdPLqhFuLcs81624521;     qTlDfKpcpJlWUdPLqhFuLcs81624521 = qTlDfKpcpJlWUdPLqhFuLcs37654551;     qTlDfKpcpJlWUdPLqhFuLcs37654551 = qTlDfKpcpJlWUdPLqhFuLcs99841859;     qTlDfKpcpJlWUdPLqhFuLcs99841859 = qTlDfKpcpJlWUdPLqhFuLcs36338479;     qTlDfKpcpJlWUdPLqhFuLcs36338479 = qTlDfKpcpJlWUdPLqhFuLcs43051507;     qTlDfKpcpJlWUdPLqhFuLcs43051507 = qTlDfKpcpJlWUdPLqhFuLcs56688448;     qTlDfKpcpJlWUdPLqhFuLcs56688448 = qTlDfKpcpJlWUdPLqhFuLcs31914778;     qTlDfKpcpJlWUdPLqhFuLcs31914778 = qTlDfKpcpJlWUdPLqhFuLcs77127336;     qTlDfKpcpJlWUdPLqhFuLcs77127336 = qTlDfKpcpJlWUdPLqhFuLcs41603295;     qTlDfKpcpJlWUdPLqhFuLcs41603295 = qTlDfKpcpJlWUdPLqhFuLcs81380499;     qTlDfKpcpJlWUdPLqhFuLcs81380499 = qTlDfKpcpJlWUdPLqhFuLcs65975170;     qTlDfKpcpJlWUdPLqhFuLcs65975170 = qTlDfKpcpJlWUdPLqhFuLcs45611802;     qTlDfKpcpJlWUdPLqhFuLcs45611802 = qTlDfKpcpJlWUdPLqhFuLcs7619720;     qTlDfKpcpJlWUdPLqhFuLcs7619720 = qTlDfKpcpJlWUdPLqhFuLcs83979689;     qTlDfKpcpJlWUdPLqhFuLcs83979689 = qTlDfKpcpJlWUdPLqhFuLcs22798721;     qTlDfKpcpJlWUdPLqhFuLcs22798721 = qTlDfKpcpJlWUdPLqhFuLcs41563054;     qTlDfKpcpJlWUdPLqhFuLcs41563054 = qTlDfKpcpJlWUdPLqhFuLcs64502551;     qTlDfKpcpJlWUdPLqhFuLcs64502551 = qTlDfKpcpJlWUdPLqhFuLcs7941182;     qTlDfKpcpJlWUdPLqhFuLcs7941182 = qTlDfKpcpJlWUdPLqhFuLcs52103175;     qTlDfKpcpJlWUdPLqhFuLcs52103175 = qTlDfKpcpJlWUdPLqhFuLcs39613905;     qTlDfKpcpJlWUdPLqhFuLcs39613905 = qTlDfKpcpJlWUdPLqhFuLcs10142767;     qTlDfKpcpJlWUdPLqhFuLcs10142767 = qTlDfKpcpJlWUdPLqhFuLcs62544687;     qTlDfKpcpJlWUdPLqhFuLcs62544687 = qTlDfKpcpJlWUdPLqhFuLcs50989207;     qTlDfKpcpJlWUdPLqhFuLcs50989207 = qTlDfKpcpJlWUdPLqhFuLcs96930768;     qTlDfKpcpJlWUdPLqhFuLcs96930768 = qTlDfKpcpJlWUdPLqhFuLcs60382202;     qTlDfKpcpJlWUdPLqhFuLcs60382202 = qTlDfKpcpJlWUdPLqhFuLcs97523322;     qTlDfKpcpJlWUdPLqhFuLcs97523322 = qTlDfKpcpJlWUdPLqhFuLcs84324371;     qTlDfKpcpJlWUdPLqhFuLcs84324371 = qTlDfKpcpJlWUdPLqhFuLcs52583962;     qTlDfKpcpJlWUdPLqhFuLcs52583962 = qTlDfKpcpJlWUdPLqhFuLcs57034319;     qTlDfKpcpJlWUdPLqhFuLcs57034319 = qTlDfKpcpJlWUdPLqhFuLcs51816797;     qTlDfKpcpJlWUdPLqhFuLcs51816797 = qTlDfKpcpJlWUdPLqhFuLcs47343161;     qTlDfKpcpJlWUdPLqhFuLcs47343161 = qTlDfKpcpJlWUdPLqhFuLcs52964562;     qTlDfKpcpJlWUdPLqhFuLcs52964562 = qTlDfKpcpJlWUdPLqhFuLcs26782829;     qTlDfKpcpJlWUdPLqhFuLcs26782829 = qTlDfKpcpJlWUdPLqhFuLcs93454758;     qTlDfKpcpJlWUdPLqhFuLcs93454758 = qTlDfKpcpJlWUdPLqhFuLcs81355307;     qTlDfKpcpJlWUdPLqhFuLcs81355307 = qTlDfKpcpJlWUdPLqhFuLcs35248246;     qTlDfKpcpJlWUdPLqhFuLcs35248246 = qTlDfKpcpJlWUdPLqhFuLcs59767889;     qTlDfKpcpJlWUdPLqhFuLcs59767889 = qTlDfKpcpJlWUdPLqhFuLcs27286064;     qTlDfKpcpJlWUdPLqhFuLcs27286064 = qTlDfKpcpJlWUdPLqhFuLcs90367441;     qTlDfKpcpJlWUdPLqhFuLcs90367441 = qTlDfKpcpJlWUdPLqhFuLcs5394812;     qTlDfKpcpJlWUdPLqhFuLcs5394812 = qTlDfKpcpJlWUdPLqhFuLcs62556261;     qTlDfKpcpJlWUdPLqhFuLcs62556261 = qTlDfKpcpJlWUdPLqhFuLcs89905283;     qTlDfKpcpJlWUdPLqhFuLcs89905283 = qTlDfKpcpJlWUdPLqhFuLcs24367115;     qTlDfKpcpJlWUdPLqhFuLcs24367115 = qTlDfKpcpJlWUdPLqhFuLcs30748511;     qTlDfKpcpJlWUdPLqhFuLcs30748511 = qTlDfKpcpJlWUdPLqhFuLcs22343340;     qTlDfKpcpJlWUdPLqhFuLcs22343340 = qTlDfKpcpJlWUdPLqhFuLcs61067379;     qTlDfKpcpJlWUdPLqhFuLcs61067379 = qTlDfKpcpJlWUdPLqhFuLcs60585046;     qTlDfKpcpJlWUdPLqhFuLcs60585046 = qTlDfKpcpJlWUdPLqhFuLcs78465506;     qTlDfKpcpJlWUdPLqhFuLcs78465506 = qTlDfKpcpJlWUdPLqhFuLcs23320687;     qTlDfKpcpJlWUdPLqhFuLcs23320687 = qTlDfKpcpJlWUdPLqhFuLcs54522076;     qTlDfKpcpJlWUdPLqhFuLcs54522076 = qTlDfKpcpJlWUdPLqhFuLcs98208171;     qTlDfKpcpJlWUdPLqhFuLcs98208171 = qTlDfKpcpJlWUdPLqhFuLcs39745091;     qTlDfKpcpJlWUdPLqhFuLcs39745091 = qTlDfKpcpJlWUdPLqhFuLcs85775005;     qTlDfKpcpJlWUdPLqhFuLcs85775005 = qTlDfKpcpJlWUdPLqhFuLcs46875818;     qTlDfKpcpJlWUdPLqhFuLcs46875818 = qTlDfKpcpJlWUdPLqhFuLcs97101803;     qTlDfKpcpJlWUdPLqhFuLcs97101803 = qTlDfKpcpJlWUdPLqhFuLcs19079835;     qTlDfKpcpJlWUdPLqhFuLcs19079835 = qTlDfKpcpJlWUdPLqhFuLcs86665344;     qTlDfKpcpJlWUdPLqhFuLcs86665344 = qTlDfKpcpJlWUdPLqhFuLcs2911092;     qTlDfKpcpJlWUdPLqhFuLcs2911092 = qTlDfKpcpJlWUdPLqhFuLcs75956277;     qTlDfKpcpJlWUdPLqhFuLcs75956277 = qTlDfKpcpJlWUdPLqhFuLcs45528185;     qTlDfKpcpJlWUdPLqhFuLcs45528185 = qTlDfKpcpJlWUdPLqhFuLcs72364077;     qTlDfKpcpJlWUdPLqhFuLcs72364077 = qTlDfKpcpJlWUdPLqhFuLcs79330815;     qTlDfKpcpJlWUdPLqhFuLcs79330815 = qTlDfKpcpJlWUdPLqhFuLcs20093018;     qTlDfKpcpJlWUdPLqhFuLcs20093018 = qTlDfKpcpJlWUdPLqhFuLcs89786497;     qTlDfKpcpJlWUdPLqhFuLcs89786497 = qTlDfKpcpJlWUdPLqhFuLcs34037339;     qTlDfKpcpJlWUdPLqhFuLcs34037339 = qTlDfKpcpJlWUdPLqhFuLcs13010608;     qTlDfKpcpJlWUdPLqhFuLcs13010608 = qTlDfKpcpJlWUdPLqhFuLcs18828973;     qTlDfKpcpJlWUdPLqhFuLcs18828973 = qTlDfKpcpJlWUdPLqhFuLcs14164962;     qTlDfKpcpJlWUdPLqhFuLcs14164962 = qTlDfKpcpJlWUdPLqhFuLcs2624382;     qTlDfKpcpJlWUdPLqhFuLcs2624382 = qTlDfKpcpJlWUdPLqhFuLcs87550475;     qTlDfKpcpJlWUdPLqhFuLcs87550475 = qTlDfKpcpJlWUdPLqhFuLcs81795165;     qTlDfKpcpJlWUdPLqhFuLcs81795165 = qTlDfKpcpJlWUdPLqhFuLcs37216488;     qTlDfKpcpJlWUdPLqhFuLcs37216488 = qTlDfKpcpJlWUdPLqhFuLcs17573740;     qTlDfKpcpJlWUdPLqhFuLcs17573740 = qTlDfKpcpJlWUdPLqhFuLcs46708363;     qTlDfKpcpJlWUdPLqhFuLcs46708363 = qTlDfKpcpJlWUdPLqhFuLcs77057643;     qTlDfKpcpJlWUdPLqhFuLcs77057643 = qTlDfKpcpJlWUdPLqhFuLcs20237484;     qTlDfKpcpJlWUdPLqhFuLcs20237484 = qTlDfKpcpJlWUdPLqhFuLcs38177573;     qTlDfKpcpJlWUdPLqhFuLcs38177573 = qTlDfKpcpJlWUdPLqhFuLcs13272552;}



void bOfNAlcyiibhjOtJDWpELeZzxA75905580() {     float UHQwatTwrpaLCEFJiaPxSBm96996069 = -27374412;    float UHQwatTwrpaLCEFJiaPxSBm98297425 = 16809867;    float UHQwatTwrpaLCEFJiaPxSBm51565946 = -875844636;    float UHQwatTwrpaLCEFJiaPxSBm70050750 = -598560892;    float UHQwatTwrpaLCEFJiaPxSBm39171085 = -233500675;    float UHQwatTwrpaLCEFJiaPxSBm74964061 = -781424644;    float UHQwatTwrpaLCEFJiaPxSBm70299511 = -993879471;    float UHQwatTwrpaLCEFJiaPxSBm5423344 = 69240296;    float UHQwatTwrpaLCEFJiaPxSBm44690567 = -513903925;    float UHQwatTwrpaLCEFJiaPxSBm13138208 = -483955898;    float UHQwatTwrpaLCEFJiaPxSBm8859445 = 38691867;    float UHQwatTwrpaLCEFJiaPxSBm2340782 = -969256768;    float UHQwatTwrpaLCEFJiaPxSBm39372361 = -157802036;    float UHQwatTwrpaLCEFJiaPxSBm1052326 = -815270938;    float UHQwatTwrpaLCEFJiaPxSBm72384024 = -300272531;    float UHQwatTwrpaLCEFJiaPxSBm34259225 = -359499993;    float UHQwatTwrpaLCEFJiaPxSBm45984819 = -665648042;    float UHQwatTwrpaLCEFJiaPxSBm10162400 = -247513490;    float UHQwatTwrpaLCEFJiaPxSBm3687205 = -973169335;    float UHQwatTwrpaLCEFJiaPxSBm21793916 = -948357993;    float UHQwatTwrpaLCEFJiaPxSBm5217692 = -194079731;    float UHQwatTwrpaLCEFJiaPxSBm87491793 = -949332397;    float UHQwatTwrpaLCEFJiaPxSBm76420436 = -926734685;    float UHQwatTwrpaLCEFJiaPxSBm82767216 = -74610181;    float UHQwatTwrpaLCEFJiaPxSBm74256643 = -567709830;    float UHQwatTwrpaLCEFJiaPxSBm52834618 = -608714656;    float UHQwatTwrpaLCEFJiaPxSBm57930418 = 49596908;    float UHQwatTwrpaLCEFJiaPxSBm85650707 = 13554535;    float UHQwatTwrpaLCEFJiaPxSBm28606590 = -853287373;    float UHQwatTwrpaLCEFJiaPxSBm67463105 = -328721518;    float UHQwatTwrpaLCEFJiaPxSBm58491759 = -789364957;    float UHQwatTwrpaLCEFJiaPxSBm66396583 = -781197308;    float UHQwatTwrpaLCEFJiaPxSBm95505201 = -819706568;    float UHQwatTwrpaLCEFJiaPxSBm1699884 = -514171768;    float UHQwatTwrpaLCEFJiaPxSBm13215127 = -3619478;    float UHQwatTwrpaLCEFJiaPxSBm77480953 = -666836195;    float UHQwatTwrpaLCEFJiaPxSBm83743279 = -477285306;    float UHQwatTwrpaLCEFJiaPxSBm73180512 = -804654222;    float UHQwatTwrpaLCEFJiaPxSBm2178773 = -574001554;    float UHQwatTwrpaLCEFJiaPxSBm3691668 = -395903934;    float UHQwatTwrpaLCEFJiaPxSBm72708702 = -456651601;    float UHQwatTwrpaLCEFJiaPxSBm81442902 = -208069086;    float UHQwatTwrpaLCEFJiaPxSBm72414051 = -361572297;    float UHQwatTwrpaLCEFJiaPxSBm57338286 = -74912165;    float UHQwatTwrpaLCEFJiaPxSBm90531937 = -897872508;    float UHQwatTwrpaLCEFJiaPxSBm95256088 = -419099623;    float UHQwatTwrpaLCEFJiaPxSBm30109738 = -577807916;    float UHQwatTwrpaLCEFJiaPxSBm99124049 = -333478599;    float UHQwatTwrpaLCEFJiaPxSBm31397536 = -425462112;    float UHQwatTwrpaLCEFJiaPxSBm20197702 = -687261732;    float UHQwatTwrpaLCEFJiaPxSBm92674793 = -365693085;    float UHQwatTwrpaLCEFJiaPxSBm8540039 = -601598873;    float UHQwatTwrpaLCEFJiaPxSBm97199199 = 20221810;    float UHQwatTwrpaLCEFJiaPxSBm93275766 = 77968207;    float UHQwatTwrpaLCEFJiaPxSBm70687889 = -177160438;    float UHQwatTwrpaLCEFJiaPxSBm9504276 = -78042016;    float UHQwatTwrpaLCEFJiaPxSBm21876989 = -56455448;    float UHQwatTwrpaLCEFJiaPxSBm68798730 = -701234455;    float UHQwatTwrpaLCEFJiaPxSBm95794106 = 69148938;    float UHQwatTwrpaLCEFJiaPxSBm86336467 = -624786019;    float UHQwatTwrpaLCEFJiaPxSBm17033643 = -731021553;    float UHQwatTwrpaLCEFJiaPxSBm84648804 = -907434007;    float UHQwatTwrpaLCEFJiaPxSBm76816753 = -77472331;    float UHQwatTwrpaLCEFJiaPxSBm77227462 = -85182407;    float UHQwatTwrpaLCEFJiaPxSBm54646449 = -694590942;    float UHQwatTwrpaLCEFJiaPxSBm42462862 = -180110825;    float UHQwatTwrpaLCEFJiaPxSBm6835581 = -49550200;    float UHQwatTwrpaLCEFJiaPxSBm37672478 = -643630269;    float UHQwatTwrpaLCEFJiaPxSBm87837198 = -711651460;    float UHQwatTwrpaLCEFJiaPxSBm94903070 = -633436337;    float UHQwatTwrpaLCEFJiaPxSBm50515945 = -882214688;    float UHQwatTwrpaLCEFJiaPxSBm72804306 = -860993820;    float UHQwatTwrpaLCEFJiaPxSBm7983627 = -673511936;    float UHQwatTwrpaLCEFJiaPxSBm99995536 = -477265402;    float UHQwatTwrpaLCEFJiaPxSBm49085214 = -391706393;    float UHQwatTwrpaLCEFJiaPxSBm23774790 = -986010646;    float UHQwatTwrpaLCEFJiaPxSBm15077743 = -487760101;    float UHQwatTwrpaLCEFJiaPxSBm19082150 = -751822520;    float UHQwatTwrpaLCEFJiaPxSBm92235278 = -176737674;    float UHQwatTwrpaLCEFJiaPxSBm79000555 = -48610208;    float UHQwatTwrpaLCEFJiaPxSBm22724880 = 69093259;    float UHQwatTwrpaLCEFJiaPxSBm58806369 = -616924493;    float UHQwatTwrpaLCEFJiaPxSBm54253172 = -560983354;    float UHQwatTwrpaLCEFJiaPxSBm8408889 = -66025642;    float UHQwatTwrpaLCEFJiaPxSBm74788311 = -963028434;    float UHQwatTwrpaLCEFJiaPxSBm49951721 = -87766084;    float UHQwatTwrpaLCEFJiaPxSBm69197383 = -701419118;    float UHQwatTwrpaLCEFJiaPxSBm2229435 = -797674775;    float UHQwatTwrpaLCEFJiaPxSBm31011995 = -237011331;    float UHQwatTwrpaLCEFJiaPxSBm3710851 = -925577463;    float UHQwatTwrpaLCEFJiaPxSBm55603965 = -510380747;    float UHQwatTwrpaLCEFJiaPxSBm14944550 = -776050851;    float UHQwatTwrpaLCEFJiaPxSBm77386406 = -773803161;    float UHQwatTwrpaLCEFJiaPxSBm15842306 = -949215536;    float UHQwatTwrpaLCEFJiaPxSBm86658025 = -664882382;    float UHQwatTwrpaLCEFJiaPxSBm88059897 = -549217594;    float UHQwatTwrpaLCEFJiaPxSBm4626149 = -30596755;    float UHQwatTwrpaLCEFJiaPxSBm95186588 = -176389890;    float UHQwatTwrpaLCEFJiaPxSBm2691838 = -380321224;    float UHQwatTwrpaLCEFJiaPxSBm48069076 = -27374412;     UHQwatTwrpaLCEFJiaPxSBm96996069 = UHQwatTwrpaLCEFJiaPxSBm98297425;     UHQwatTwrpaLCEFJiaPxSBm98297425 = UHQwatTwrpaLCEFJiaPxSBm51565946;     UHQwatTwrpaLCEFJiaPxSBm51565946 = UHQwatTwrpaLCEFJiaPxSBm70050750;     UHQwatTwrpaLCEFJiaPxSBm70050750 = UHQwatTwrpaLCEFJiaPxSBm39171085;     UHQwatTwrpaLCEFJiaPxSBm39171085 = UHQwatTwrpaLCEFJiaPxSBm74964061;     UHQwatTwrpaLCEFJiaPxSBm74964061 = UHQwatTwrpaLCEFJiaPxSBm70299511;     UHQwatTwrpaLCEFJiaPxSBm70299511 = UHQwatTwrpaLCEFJiaPxSBm5423344;     UHQwatTwrpaLCEFJiaPxSBm5423344 = UHQwatTwrpaLCEFJiaPxSBm44690567;     UHQwatTwrpaLCEFJiaPxSBm44690567 = UHQwatTwrpaLCEFJiaPxSBm13138208;     UHQwatTwrpaLCEFJiaPxSBm13138208 = UHQwatTwrpaLCEFJiaPxSBm8859445;     UHQwatTwrpaLCEFJiaPxSBm8859445 = UHQwatTwrpaLCEFJiaPxSBm2340782;     UHQwatTwrpaLCEFJiaPxSBm2340782 = UHQwatTwrpaLCEFJiaPxSBm39372361;     UHQwatTwrpaLCEFJiaPxSBm39372361 = UHQwatTwrpaLCEFJiaPxSBm1052326;     UHQwatTwrpaLCEFJiaPxSBm1052326 = UHQwatTwrpaLCEFJiaPxSBm72384024;     UHQwatTwrpaLCEFJiaPxSBm72384024 = UHQwatTwrpaLCEFJiaPxSBm34259225;     UHQwatTwrpaLCEFJiaPxSBm34259225 = UHQwatTwrpaLCEFJiaPxSBm45984819;     UHQwatTwrpaLCEFJiaPxSBm45984819 = UHQwatTwrpaLCEFJiaPxSBm10162400;     UHQwatTwrpaLCEFJiaPxSBm10162400 = UHQwatTwrpaLCEFJiaPxSBm3687205;     UHQwatTwrpaLCEFJiaPxSBm3687205 = UHQwatTwrpaLCEFJiaPxSBm21793916;     UHQwatTwrpaLCEFJiaPxSBm21793916 = UHQwatTwrpaLCEFJiaPxSBm5217692;     UHQwatTwrpaLCEFJiaPxSBm5217692 = UHQwatTwrpaLCEFJiaPxSBm87491793;     UHQwatTwrpaLCEFJiaPxSBm87491793 = UHQwatTwrpaLCEFJiaPxSBm76420436;     UHQwatTwrpaLCEFJiaPxSBm76420436 = UHQwatTwrpaLCEFJiaPxSBm82767216;     UHQwatTwrpaLCEFJiaPxSBm82767216 = UHQwatTwrpaLCEFJiaPxSBm74256643;     UHQwatTwrpaLCEFJiaPxSBm74256643 = UHQwatTwrpaLCEFJiaPxSBm52834618;     UHQwatTwrpaLCEFJiaPxSBm52834618 = UHQwatTwrpaLCEFJiaPxSBm57930418;     UHQwatTwrpaLCEFJiaPxSBm57930418 = UHQwatTwrpaLCEFJiaPxSBm85650707;     UHQwatTwrpaLCEFJiaPxSBm85650707 = UHQwatTwrpaLCEFJiaPxSBm28606590;     UHQwatTwrpaLCEFJiaPxSBm28606590 = UHQwatTwrpaLCEFJiaPxSBm67463105;     UHQwatTwrpaLCEFJiaPxSBm67463105 = UHQwatTwrpaLCEFJiaPxSBm58491759;     UHQwatTwrpaLCEFJiaPxSBm58491759 = UHQwatTwrpaLCEFJiaPxSBm66396583;     UHQwatTwrpaLCEFJiaPxSBm66396583 = UHQwatTwrpaLCEFJiaPxSBm95505201;     UHQwatTwrpaLCEFJiaPxSBm95505201 = UHQwatTwrpaLCEFJiaPxSBm1699884;     UHQwatTwrpaLCEFJiaPxSBm1699884 = UHQwatTwrpaLCEFJiaPxSBm13215127;     UHQwatTwrpaLCEFJiaPxSBm13215127 = UHQwatTwrpaLCEFJiaPxSBm77480953;     UHQwatTwrpaLCEFJiaPxSBm77480953 = UHQwatTwrpaLCEFJiaPxSBm83743279;     UHQwatTwrpaLCEFJiaPxSBm83743279 = UHQwatTwrpaLCEFJiaPxSBm73180512;     UHQwatTwrpaLCEFJiaPxSBm73180512 = UHQwatTwrpaLCEFJiaPxSBm2178773;     UHQwatTwrpaLCEFJiaPxSBm2178773 = UHQwatTwrpaLCEFJiaPxSBm3691668;     UHQwatTwrpaLCEFJiaPxSBm3691668 = UHQwatTwrpaLCEFJiaPxSBm72708702;     UHQwatTwrpaLCEFJiaPxSBm72708702 = UHQwatTwrpaLCEFJiaPxSBm81442902;     UHQwatTwrpaLCEFJiaPxSBm81442902 = UHQwatTwrpaLCEFJiaPxSBm72414051;     UHQwatTwrpaLCEFJiaPxSBm72414051 = UHQwatTwrpaLCEFJiaPxSBm57338286;     UHQwatTwrpaLCEFJiaPxSBm57338286 = UHQwatTwrpaLCEFJiaPxSBm90531937;     UHQwatTwrpaLCEFJiaPxSBm90531937 = UHQwatTwrpaLCEFJiaPxSBm95256088;     UHQwatTwrpaLCEFJiaPxSBm95256088 = UHQwatTwrpaLCEFJiaPxSBm30109738;     UHQwatTwrpaLCEFJiaPxSBm30109738 = UHQwatTwrpaLCEFJiaPxSBm99124049;     UHQwatTwrpaLCEFJiaPxSBm99124049 = UHQwatTwrpaLCEFJiaPxSBm31397536;     UHQwatTwrpaLCEFJiaPxSBm31397536 = UHQwatTwrpaLCEFJiaPxSBm20197702;     UHQwatTwrpaLCEFJiaPxSBm20197702 = UHQwatTwrpaLCEFJiaPxSBm92674793;     UHQwatTwrpaLCEFJiaPxSBm92674793 = UHQwatTwrpaLCEFJiaPxSBm8540039;     UHQwatTwrpaLCEFJiaPxSBm8540039 = UHQwatTwrpaLCEFJiaPxSBm97199199;     UHQwatTwrpaLCEFJiaPxSBm97199199 = UHQwatTwrpaLCEFJiaPxSBm93275766;     UHQwatTwrpaLCEFJiaPxSBm93275766 = UHQwatTwrpaLCEFJiaPxSBm70687889;     UHQwatTwrpaLCEFJiaPxSBm70687889 = UHQwatTwrpaLCEFJiaPxSBm9504276;     UHQwatTwrpaLCEFJiaPxSBm9504276 = UHQwatTwrpaLCEFJiaPxSBm21876989;     UHQwatTwrpaLCEFJiaPxSBm21876989 = UHQwatTwrpaLCEFJiaPxSBm68798730;     UHQwatTwrpaLCEFJiaPxSBm68798730 = UHQwatTwrpaLCEFJiaPxSBm95794106;     UHQwatTwrpaLCEFJiaPxSBm95794106 = UHQwatTwrpaLCEFJiaPxSBm86336467;     UHQwatTwrpaLCEFJiaPxSBm86336467 = UHQwatTwrpaLCEFJiaPxSBm17033643;     UHQwatTwrpaLCEFJiaPxSBm17033643 = UHQwatTwrpaLCEFJiaPxSBm84648804;     UHQwatTwrpaLCEFJiaPxSBm84648804 = UHQwatTwrpaLCEFJiaPxSBm76816753;     UHQwatTwrpaLCEFJiaPxSBm76816753 = UHQwatTwrpaLCEFJiaPxSBm77227462;     UHQwatTwrpaLCEFJiaPxSBm77227462 = UHQwatTwrpaLCEFJiaPxSBm54646449;     UHQwatTwrpaLCEFJiaPxSBm54646449 = UHQwatTwrpaLCEFJiaPxSBm42462862;     UHQwatTwrpaLCEFJiaPxSBm42462862 = UHQwatTwrpaLCEFJiaPxSBm6835581;     UHQwatTwrpaLCEFJiaPxSBm6835581 = UHQwatTwrpaLCEFJiaPxSBm37672478;     UHQwatTwrpaLCEFJiaPxSBm37672478 = UHQwatTwrpaLCEFJiaPxSBm87837198;     UHQwatTwrpaLCEFJiaPxSBm87837198 = UHQwatTwrpaLCEFJiaPxSBm94903070;     UHQwatTwrpaLCEFJiaPxSBm94903070 = UHQwatTwrpaLCEFJiaPxSBm50515945;     UHQwatTwrpaLCEFJiaPxSBm50515945 = UHQwatTwrpaLCEFJiaPxSBm72804306;     UHQwatTwrpaLCEFJiaPxSBm72804306 = UHQwatTwrpaLCEFJiaPxSBm7983627;     UHQwatTwrpaLCEFJiaPxSBm7983627 = UHQwatTwrpaLCEFJiaPxSBm99995536;     UHQwatTwrpaLCEFJiaPxSBm99995536 = UHQwatTwrpaLCEFJiaPxSBm49085214;     UHQwatTwrpaLCEFJiaPxSBm49085214 = UHQwatTwrpaLCEFJiaPxSBm23774790;     UHQwatTwrpaLCEFJiaPxSBm23774790 = UHQwatTwrpaLCEFJiaPxSBm15077743;     UHQwatTwrpaLCEFJiaPxSBm15077743 = UHQwatTwrpaLCEFJiaPxSBm19082150;     UHQwatTwrpaLCEFJiaPxSBm19082150 = UHQwatTwrpaLCEFJiaPxSBm92235278;     UHQwatTwrpaLCEFJiaPxSBm92235278 = UHQwatTwrpaLCEFJiaPxSBm79000555;     UHQwatTwrpaLCEFJiaPxSBm79000555 = UHQwatTwrpaLCEFJiaPxSBm22724880;     UHQwatTwrpaLCEFJiaPxSBm22724880 = UHQwatTwrpaLCEFJiaPxSBm58806369;     UHQwatTwrpaLCEFJiaPxSBm58806369 = UHQwatTwrpaLCEFJiaPxSBm54253172;     UHQwatTwrpaLCEFJiaPxSBm54253172 = UHQwatTwrpaLCEFJiaPxSBm8408889;     UHQwatTwrpaLCEFJiaPxSBm8408889 = UHQwatTwrpaLCEFJiaPxSBm74788311;     UHQwatTwrpaLCEFJiaPxSBm74788311 = UHQwatTwrpaLCEFJiaPxSBm49951721;     UHQwatTwrpaLCEFJiaPxSBm49951721 = UHQwatTwrpaLCEFJiaPxSBm69197383;     UHQwatTwrpaLCEFJiaPxSBm69197383 = UHQwatTwrpaLCEFJiaPxSBm2229435;     UHQwatTwrpaLCEFJiaPxSBm2229435 = UHQwatTwrpaLCEFJiaPxSBm31011995;     UHQwatTwrpaLCEFJiaPxSBm31011995 = UHQwatTwrpaLCEFJiaPxSBm3710851;     UHQwatTwrpaLCEFJiaPxSBm3710851 = UHQwatTwrpaLCEFJiaPxSBm55603965;     UHQwatTwrpaLCEFJiaPxSBm55603965 = UHQwatTwrpaLCEFJiaPxSBm14944550;     UHQwatTwrpaLCEFJiaPxSBm14944550 = UHQwatTwrpaLCEFJiaPxSBm77386406;     UHQwatTwrpaLCEFJiaPxSBm77386406 = UHQwatTwrpaLCEFJiaPxSBm15842306;     UHQwatTwrpaLCEFJiaPxSBm15842306 = UHQwatTwrpaLCEFJiaPxSBm86658025;     UHQwatTwrpaLCEFJiaPxSBm86658025 = UHQwatTwrpaLCEFJiaPxSBm88059897;     UHQwatTwrpaLCEFJiaPxSBm88059897 = UHQwatTwrpaLCEFJiaPxSBm4626149;     UHQwatTwrpaLCEFJiaPxSBm4626149 = UHQwatTwrpaLCEFJiaPxSBm95186588;     UHQwatTwrpaLCEFJiaPxSBm95186588 = UHQwatTwrpaLCEFJiaPxSBm2691838;     UHQwatTwrpaLCEFJiaPxSBm2691838 = UHQwatTwrpaLCEFJiaPxSBm48069076;     UHQwatTwrpaLCEFJiaPxSBm48069076 = UHQwatTwrpaLCEFJiaPxSBm96996069;}

































































































































































































































































































































































































































































































































































































































































































































