
// *********************************
// *             GuukGang          *
// *           Solutions           *
// *********************************
// *           RESOLVER			   *
// *********************************
// *		Needs an overhaul      *
// *********************************
// *		Pure bruteforce		   *
// *			action			   *
// *********************************


#pragma once
#include "HookIncludes.h"
#include "Entities.h"

int GetEstimatedServerTickCount(float latency)
{
	return (int)floorf(float((float)((float)(latency) / (float)((uintptr_t)&g_Globals->interval_per_tick)) + 0.5) + 1 + (int)((uintptr_t)&g_Globals->tickcount));
}

float NormalizeYaw(float value)
{
	while (value > 180)
		value -= 360.f;

	while (value < -180)
		value += 360.f;
	return value;
}

namespace Globals
{
	int Shots;
	int missedshots;
	float RealAngle;
	float FakeAngle;
	Vector AimPoint;
	bool shouldflip;
	bool ySwitch;
	float NextTime;
	int resolvemode = 1;
	float fakeAngle;
	float OldSimulationTime[65];
	bool error;
}


inline float ClampYaw(float yaw) {
	while (yaw > 180.f)
		yaw -= 360.f;
	while (yaw < -180.f)
		yaw += 360.f;
	return yaw;
}



void Resolver3()
{

	if (g_Options.Ragebot.Resolver)
	{
		C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		{
			for (auto i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
			{
				C_BaseEntity* entity = g_EntityList->GetClientEntity(i);

				if (!entity) continue;

				if (entity == pLocal) continue;

				if (entity->IsDormant()) continue;

				player_info_t pTemp;

				if (!g_Engine->GetPlayerInfo(i, &pTemp))
					continue;

				if (entity->GetTeamNum() == pLocal->GetTeamNum()) continue;


				/*---------------------------------------------------------------------------*/
				auto FYaw					= entity->GetLowerBodyYaw();
				auto pitch					= entity->GetEyeAngles()->x;
				float PlayerIsMoving		= abs(entity->GetVelocity().Length2D());
				float bodyeyedelta			= fabs(entity->GetEyeAngles()->y - entity->GetLowerBodyYaw());
				bool bLowerBodyUpdated		= false;
				bool IsBreakingLBY			= false;
				bool isFakeHeading			= false;
				bool isOnGround				= false;
				bool isFakeWalking			= false; 
				bool isStationary			= false;
				float oldLBY				= entity->GetLowerBodyYaw();
				/*---------------------------------------------------------------------------*/
				entity->GetEyeAngles()->y = FYaw;
				if (entity->GetVelocity().Length2D() <= .4 || .14 )
				{
					isFakeWalking = true;
				}
				else if (entity->GetFlags() & FL_DUCKING) 
				{
					isFakeWalking = false;
				}

				if (oldLBY != entity->GetLowerBodyYaw())
				{
					bLowerBodyUpdated = true;
				}
				else
				{
					bLowerBodyUpdated = false;
				}
				if (entity->GetEyeAngles()->y - entity->GetLowerBodyYaw() > 35)
				{
					isFakeHeading = true;
				}
				else
				{
					isFakeHeading = false;
				}


				if (entity->GetFlags() & FL_ONGROUND)
				{
					isOnGround = true;
					Globals::resolvemode = 1;
				}
				else
				{
					isOnGround = false;
				}
				if (entity->GetVelocity().Length2D() >= 0 && entity->GetVelocity().Length2D() < 36)
				{
					IsBreakingLBY = true;

				}
				else
				{
					IsBreakingLBY = false;
				}

				if (entity->GetVelocity().Length2D() <= 0) 
				{
					isStationary = true;
					Globals::resolvemode = 5;
				}
				else
				{
					isStationary = false;
				}
					

			   /*--------------------------------------------------------------------------------
												NEW RESOLVER ATTEMPT #1
			   ----------------------------------------------------------------------------------*/


				if (entity->GetVelocity().Length2D() > 36 && entity->GetFlags() & FL_ONGROUND)  
				{
					Globals::resolvemode = 1;
					entity->GetEyeAngles()->y = FYaw;
					oldLBY = entity->GetEyeAngles()->y;
					IsBreakingLBY = false;
				}

				if (isFakeWalking)
				{
					Globals::resolvemode = 2;
					switch (Globals::missedshots % 3)
					{

					case 1:
						entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 90.f;
					case 2:
						entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() - 90.f;
					case 3: 
						entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 180.f;
					}
				}

				if (!isOnGround)
				{
					Globals::resolvemode = 3;
					switch (Globals::Shots % 4)
					{
					case 1:
						entity->GetEyeAngles()->y = entity->GetLowerBodyYaw();
					case 2:
						entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 90.f; 
					case 3:
						entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 180.f;  
					case 4:
						entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 130.f;  
				}
	
					if (isStationary)
					{
						oldLBY = entity->GetEyeAngles()->y;
						entity->GetEyeAngles()->y = FYaw;
						// IsBreakingLBY = true; vvv refering to this <--
						// this couldn't always be true we would need an actual animation check or something, but whatever frosty.pw codenz pro (((BUT WE BRUTEFORCE EVERY ANGLE SO WHO CARES)))))
						// although, their resolver was just a spin resolver with no logging fuckin pros
						// another reason for shooting fake is b/c i get their lowerbodyyaw angle so wouldnt that be where they're switching to most cheats use common angles (90, -90, 180) of course
						// i could just brute force other random angles like 131, 70, -70
					}
 
					if (IsBreakingLBY)
					{
 
						switch (Globals::missedshots % 6)
						{
						case 1:
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 90.f;
						case 2:
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() - 90.f;
						case 3:
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 180.f;
						case 4:
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 131.f;
						case 5:
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 70.f;
						case 6: 
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() - 70.f;
						}				
					}
					else if (!IsBreakingLBY)
					{
						Globals::resolvemode = 3;
						switch (Globals::missedshots % 4)
						{
						case 1:
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 180.f;
						case 2:
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 70.f;
						case 3:
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() - 70.f;
						case 4:
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 180.f;
			
						}
					}
					if (isFakeHeading)
					{
						switch (Globals::missedshots % 3)
						{
						case 1: 
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 90.f;
						case 2:
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() - 90.f;
						case 3:
							entity->GetEyeAngles()->y = entity->GetLowerBodyYaw() + 180.f;
						}
					}
				}
			}
		}
	}
}
 


 
 

