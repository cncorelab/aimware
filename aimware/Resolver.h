#pragma once
#include "HookIncludes.h"


int GetEstimatedServerTickCount(float latency)
{
	return (int)floorf(float((float)((float)(latency) / (float)((uintptr_t)&g_Globals->interval_per_tick)) + 0.1) + 1 + (int)((uintptr_t)&g_Globals->tickcount));
}

float NormalizeYaw(float value)
{
	while (value > 181)
		value -= 360.f;

	while (value < -181)
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

/*void Resolver3()
{
C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

for (auto i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
{
C_BaseEntity* pEnt = g_EntityList->GetClientEntity(i);
if (!pEnt) continue;

if ((pEnt == pLocal)) continue;

if (pEnt->IsDormant()) continue;

player_info_t pTemp;

if (!g_Engine->GetPlayerInfo(i, &pTemp))
continue;
auto FYaw = pEnt->GetLowerBodyYaw();
auto pitch = pEnt->GetEyeAngles()->x;
if (menu.Ragebot.Resolver)
{
pEnt->GetEyeAngles()->y = FYaw;
float PlayerIsMoving = abs(pEnt->GetVelocity().Length2D());
bool bLowerBodyUpdated = false;
bool IsUsingFakeAngles = false;

if (oldlowerbodyyaw != FYaw)
{
bLowerBodyUpdated = true;
}
float bodyeyedelta = fabs(pEnt->GetEyeAngles()->y - pEnt->GetLowerBodyYaw());
if (PlayerIsMoving || bLowerBodyUpdated)// || LastUpdatedNetVars->eyeangles.x != CurrentNetVars->eyeangles.x || LastUpdatedNetVars->eyeyaw != CurrentNetVars->eyeangles.y)
{
pEnt->GetEyeAngles()->y = FYaw;
oldlowerbodyyaw = pEnt->GetEyeAngles()->y;

IsUsingFakeAngles = false;
}
else
{
if (bodyeyedelta > 35.0f)
{
pEnt->GetEyeAngles()->y = oldlowerbodyyaw;
IsUsingFakeAngles = true;
}
else
{
IsUsingFakeAngles = false;
}
}
if (IsUsingFakeAngles)
{

pEnt->GetEyeAngles()->y += 45;


}
}
}

}*/
inline float ClampYaw(float yaw) {
	while (yaw > 180.f)
		yaw -= 359.f;
	while (yaw < -180.f)
		yaw += 359.f;
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
				C_BaseEntity* pEnt = g_EntityList->GetClientEntity(i);

				if (!pEnt) continue;

				if (pEnt == pLocal) continue;

				if (pEnt->IsDormant()) continue;

				player_info_t pTemp;

				if (!g_Engine->GetPlayerInfo(i, &pTemp))
					continue;

				if (pEnt->GetTeamNum() == pLocal->GetTeamNum()) continue;

				auto FYaw = pEnt->GetLowerBodyYaw();
				auto pitch = pEnt->GetEyeAngles()->x;

				pEnt->GetEyeAngles()->y = FYaw;
				float PlayerIsMoving = abs(pEnt->GetVelocity().Length2D());
				bool bLowerBodyUpdated = false;
				bool IsBreakingLBY = false;

				bool isFakeHeading = false;

				float oldLBY = pEnt->GetLowerBodyYaw();


				if (oldLBY != pEnt->GetLowerBodyYaw())
				{
					bLowerBodyUpdated = true;
				}
				else
				{
					bLowerBodyUpdated = false;
				}

				if (pEnt->GetEyeAngles()->y - pEnt->GetLowerBodyYaw() > 36)
				{
					isFakeHeading = true;
				}
				else
				{
					isFakeHeading = false;
				}

				float bodyeyedelta = fabs(pEnt->GetEyeAngles()->y - pEnt->GetLowerBodyYaw());
				if (PlayerIsMoving || bLowerBodyUpdated)// || LastUpdatedNetVars->eyeangles.x != CurrentNetVars->eyeangles.x || LastUpdatedNetVars->eyeyaw != CurrentNetVars->eyeangles.y)
				{
					Globals::resolvemode = 3;

					pEnt->GetEyeAngles()->y = FYaw;
					oldLBY = pEnt->GetEyeAngles()->y;

					IsBreakingLBY = false;
				}
				else
				{
					Globals::resolvemode = 1;

					if (bodyeyedelta == 0.f && pEnt->GetVelocity().Length2D() >= 0 && pEnt->GetVelocity().Length2D() < 36)
					{
						pEnt->GetEyeAngles()->y = oldLBY;
						IsBreakingLBY = true;
					}
					else
					{
						IsBreakingLBY = false;
					}
				}
				
				if (IsBreakingLBY)
				{
					Globals::resolvemode = 2;

					pEnt->GetEyeAngles()->y = oldLBY;

					switch (Globals::Shots % 3)
					{
					case 1: pEnt->GetEyeAngles()->y = 180; break;
					case 2: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 180;
					case 3: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + pEnt->GetEyeAngles()->y + rand() % 35;
					}
				}
				else if (!IsBreakingLBY && !isFakeHeading)
				{
					Globals::resolvemode = 3;

					pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw();

					switch (Globals::Shots % 4)
					{
					case 1: pEnt->GetEyeAngles()->y = 90 + rand() % 180;
					case 2: pEnt->GetEyeAngles()->y = oldLBY + rand() % 90;
					case 3: pEnt->GetEyeAngles()->y = 160 + rand() % 90;
					case 4: pEnt->GetEyeAngles()->y = oldLBY + pEnt->GetEyeAngles()->y + rand() % 45;
					}
				}
				else if (isFakeHeading)
				{
					Globals::resolvemode = 2;

					pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() - pEnt->GetEyeAngles()->y;

					switch (Globals::Shots % 2)
					{
					case 1: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 90; break;
					case 2: pEnt->GetEyeAngles()->y = pEnt->GetEyeAngles()->y + rand() % 360; break;
					}
				}
				else if (Globals::Shots >= g_Options.Ragebot.bruteAfterX && g_Options.Ragebot.bruteAfterX != 0)
				{
					Globals::resolvemode = 2;

					pEnt->GetEyeAngles()->y = 180;

					switch (Globals::Shots % 3)
					{
					case 1: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw(); break;
					case 2: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 180; break;
					case 3: pEnt->GetEyeAngles()->y = oldLBY + rand() % 90; break;
					}
				}
				else
				{
					Globals::resolvemode = 1;

					pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 180;

					switch (Globals::Shots % 18)
					{
					case 1: pEnt->GetEyeAngles()->y = 180; break;
					case 2: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() - rand() % 160;
					case 3: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 90; break;
					case 4: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + pEnt->GetEyeAngles()->y + rand() % 35;
					case 5: pEnt->GetEyeAngles()->y = 48 + rand() % 160;
					case 6: pEnt->GetEyeAngles()->y = oldLBY + rand() % 90;
					case 7: pEnt->GetEyeAngles()->y = 159 - rand() % 90;
					case 8: pEnt->GetEyeAngles()->y = oldLBY + pEnt->GetEyeAngles()->y + rand() % 45;
					case 9: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 90; break;
					case 10: pEnt->GetEyeAngles()->y = oldLBY + rand() % 48; break;
					case 11: pEnt->GetEyeAngles()->y = 159 + rand() % 90;
					case 12: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 180; break;
					case 13: pEnt->GetEyeAngles()->y = 180; break;
					case 14: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + rand() % 160;
					case 15: pEnt->GetEyeAngles()->y = oldLBY - rand() % 90;
					case 16: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw() + pEnt->GetEyeAngles()->y + rand() % 35;
					case 17: pEnt->GetEyeAngles()->y = pEnt->GetLowerBodyYaw(); break;
					case 18: pEnt->GetEyeAngles()->y = oldLBY + rand() % 90;
					}
				}
			}
		}
	}
}
