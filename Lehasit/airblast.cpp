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

enum SpikeType
{
	SPIKE_NONE,
	SPIKE_MANUAL,
	SPIKE_UP,
	SPIKE_DOWN,
	SPIKE_LEFT,
	SPIKE_RIGHT
};

bool airblast::Run(CUserCmd& cmd, std::vector<CBaseEntity*> deflectableProjectiles)
{
	static QAngle lastAirblastAngle = {0, 0, 0};
	static bool didAirblastLastRun = false;

	if (!g_config.airblast_enable)
		return false;

	// Store default angles for later movement fixing
	QAngle oldAngles = cmd.viewangles;

	// Spike if possible, airblast angles will overwrite these ones if needed
	if (didAirblastLastRun)
	{
		switch (g_config.airblast_spike_type)
		{
		case SPIKE_NONE:
			cmd.viewangles = lastAirblastAngle;
			break;

		case SPIKE_MANUAL:
			break;

		case SPIKE_UP:
			cmd.viewangles[PITCH] = -89;
			cmd.viewangles[YAW] = lastAirblastAngle[YAW];
			break;

		case SPIKE_DOWN:
			cmd.viewangles[PITCH] = 89;
			cmd.viewangles[YAW] = lastAirblastAngle[YAW];
			break;

		case SPIKE_LEFT:
			cmd.viewangles[PITCH] = lastAirblastAngle[PITCH];
			cmd.viewangles[YAW] = NormalizeAngle(lastAirblastAngle[YAW] + 90.f);
			break;

		case SPIKE_RIGHT:
			cmd.viewangles[PITCH] = lastAirblastAngle[PITCH];
			cmd.viewangles[YAW] = NormalizeAngle(lastAirblastAngle[YAW] - 90.f);
			break;
		}

		utils::MovementFix(cmd, oldAngles);

		didAirblastLastRun = false;
		return true;
	}

	CTFPlayer* pLocalPlayer = utils::GetLocalPlayer();

	if (pLocalPlayer->getLifestate() != LIFE_ALIVE)
		return false;

	CBaseCombatWeapon* pWeapon = pLocalPlayer->getActiveWeapon();
	// No weapon
	if (!pWeapon)
		return false;

	// Not a flamethrower
	if (strcmp(pWeapon->GetClientClass()->name, "CTFFlameThrower") != 0)
		return false;

	// Can't airblast now
	if (pWeapon->getNextSecondaryAttackTime() > g_pGlobals->curtime)
	{
		cmd.buttons &= ~IN_ATTACK2;
		return false;
	}

	// Default tf2 doesn't need special lag compensation
	float latency = 0;

	// There is no lag compensation applied to tfdb tracking rockets, meaning we have to do prediction ourselves
	if (g_config.airblast_tfdb_lagfix)
	{
		auto netinfo = g_interfaces.engine->GetNetChannelInfo();
		latency = netinfo->GetLatency(FLOW_INCOMING) + netinfo->GetLatency(FLOW_OUTGOING); // Get round trip time
	}

	CBaseEntity* pClosestProjectile {};
	Vector closestPredictedCenter;
	float closestDistance = -1337.f;
	for (CBaseEntity* pProjectile : deflectableProjectiles)
	{
		Vector estvel;
		pProjectile->EstimateAbsVelocity(estvel);
		float speed = estvel.length();

		// The tfdodgeballs face the way they're going, no need to estimate direction, just speed
		Vector vel;
		AngleVectors(pProjectile->getAngles(), vel);
		vel *= speed;

		Vector predictpos = pProjectile->GetWorldSpaceCenter() + (vel * latency);
		float dist = (utils::GetLocalViewOrigin() - predictpos).length();

		if (dist < closestDistance || !pClosestProjectile)
		{
			pClosestProjectile = pProjectile;
			closestDistance = dist;
			closestPredictedCenter = predictpos;
		}
	}

	if (pClosestProjectile && closestDistance <= 185)
	{
		QAngle angles = utils::CalculateAngle(utils::GetLocalViewOrigin(), closestPredictedCenter);
		cmd.viewangles = angles;
		utils::MovementFix(cmd, oldAngles);
		cmd.buttons |= IN_ATTACK2;

		lastAirblastAngle = cmd.viewangles;
		didAirblastLastRun = true;

		return true;
	}

	return false;
}