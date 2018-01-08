#include "stdafx.h"
#include "cheats.h"

#include "config.h"
#include "sdk/CUserCmd.h"
#include "sdk/CBaseCombatWeapon.h"
#include "sdk/tf.h"
#include "sdk/Recv.h"
#include "Globals.h"
#include "sdk/trace.h"
#include "sdk/network.h"
#include "sdk/mathlib.h"

// Return the extrapolated position of a player after the current latency
/*Vector LatencyOffset(CTFPlayer* pPlayer, int* numticks = nullptr)
{
	Vector vel;
	pPlayer->EstimateAbsVelocity(vel);

	auto netinfo = g_interfaces.engine->GetNetChannelInfo();
	float latency = netinfo->GetLatency(FLOW_INCOMING) + netinfo->GetLatency(FLOW_OUTGOING);

	if (numticks)
	{
		*numticks = (int)(latency / g_pGlobals->interval_per_tick);
	}

	return vel*latency;
}*/

std::vector<CBaseEntity*> GetDeflectableProjectiles(CTFPlayer* pLocalPlayer)
{
	std::vector<CBaseEntity*> result;

	// +1 since 0 is the world
	for (int i = g_interfaces.engine->GetMaxClients() + 1; i <= g_interfaces.entityList->GetHighestEntityIndex(); i++)
	{
		auto* pEntity = utils::GetEntity<CBaseEntity>(i);
		if (!pEntity || pEntity->isDormant() || pEntity->getTeamNum() == pLocalPlayer->getTeamNum())
			continue;

		static const char* deflectableNames[] = { "CTFProjectile_Rocket", "CTFProjectile_Flare",
			"CTFProjectile_EnergyBall", "CTFProjectile_HealingBolt",
			"CTFProjectile_Arrow", "CTFProjectile_SentryRocket",
			"CTFProjectile_Throwable", "CTFThrowable",
			"CTFProjectile_Cleaver", "CTFProjectile_JarMilk",
			"CTFProjectile_Jar", "CTFStunBall",
			"CTFGrenadePipebombProjectile", "CTFBall_Ornament" };

		const char* classname = pEntity->get_client_class()->name;

		for (auto name : deflectableNames)
		{
			if (strcmp(classname, name) == 0)
			{
				result.push_back(pEntity);
				break;
			}
		}
	}

	return result;
}

enum SpikeType
{
	SPIKE_NONE,
	SPIKE_MANUAL,
	SPIKE_UP,
	SPIKE_DOWN,
	SPIKE_LEFT,
	SPIKE_RIGHT
};

void airblast::Run(CUserCmd* pCmd)
{
	static QAngle lastAirblastAngle = { 0, 0, 0 };

	if (!g_config.airblast_enable)
		return;

	// Store default angles for later movement fixing
	QAngle oldAngles = pCmd->viewangles;

	switch (g_config.airblast_spike_type)
	{
		case SPIKE_NONE:
			pCmd->viewangles = lastAirblastAngle;
			break;

		case SPIKE_MANUAL:
			break;

		case SPIKE_UP:
			pCmd->viewangles[PITCH] = -89;
			pCmd->viewangles[YAW] = lastAirblastAngle[YAW];
			break;

		case SPIKE_DOWN:
			pCmd->viewangles[PITCH] = 89;
			pCmd->viewangles[YAW] = lastAirblastAngle[YAW];
			break;

		case SPIKE_LEFT:
			pCmd->viewangles[PITCH] = lastAirblastAngle[PITCH];
			pCmd->viewangles[YAW] = NormalizeAngle(lastAirblastAngle[YAW] + 90.f);
			break;

		case SPIKE_RIGHT:
			pCmd->viewangles[PITCH] = lastAirblastAngle[PITCH];
			pCmd->viewangles[YAW] = NormalizeAngle(lastAirblastAngle[YAW] - 90.f);
			break;
	}

	utils::MovementFix(pCmd, oldAngles);

	CTFPlayer* pLocalPlayer = utils::GetLocalPlayer();

	if (pLocalPlayer->getLifestate() != LIFE_ALIVE)
		return;

	bool canAirblast = false;
	CBaseCombatWeapon* pWeapon = pLocalPlayer->getActiveWeapon();
	// No weapon
	if (!pWeapon)
		return;

	// Not a flamethrower
	if (strcmp(pWeapon->get_client_class()->name, "CTFFlameThrower") != 0)
		return;

	// Can't airblast now
	if (pWeapon->getNextSecondaryAttackTime() > g_pGlobals->curtime)
	{
		pCmd->buttons &= ~IN_ATTACK2;
		return;
	}

	// Default tf2 doesn't need special lag compensation
	float latency = 0;

	// There is no lag compensation applied to tfdb tracking rockets, meaning we have to do prediction ourselves
	if (g_config.airblast_tfdb_lagfix)
	{
		auto netinfo = g_interfaces.engine->GetNetChannelInfo();
		latency = netinfo->GetLatency(FLOW_INCOMING) + netinfo->GetLatency(FLOW_OUTGOING);
	}

	CBaseEntity* pClosestProjectile{};
	Vector closestPredictedCenter;
	float closestDistance = -1337.f;
	for (CBaseEntity* pProjectile : GetDeflectableProjectiles(pLocalPlayer))
	{
		Vector vel;
		pProjectile->EstimateAbsVelocity(vel);

		Vector predictpos = pProjectile->GetWorldSpaceCenter() + (vel * latency);
		float dist = (utils::GetLocalViewOrigin() - predictpos).length();

		if (dist < closestDistance || !pClosestProjectile)
		{
			pClosestProjectile = pProjectile;
			closestDistance = dist;
			closestPredictedCenter = predictpos;
		}
	}

	if (!pClosestProjectile)
		return;

	if (pClosestProjectile && closestDistance <= 185)
	{
		QAngle angles = utils::CalculateAngle(utils::GetLocalViewOrigin(), closestPredictedCenter);
		pCmd->viewangles = angles;
		utils::MovementFix(pCmd, oldAngles);
		pCmd->buttons |= IN_ATTACK2;

		lastAirblastAngle = pCmd->viewangles;
	}
}