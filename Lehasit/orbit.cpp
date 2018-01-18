#include "stdafx.h"

#include "cheats.h"
#include "sdk/Vector.h"
#include "sdk\tf.h"
#include "config.h"
#include "sdk/network.h"
#include "sdk\CUserCmd.h"

void orbit::Run(CUserCmd& cmd, std::vector<CBaseEntity*> deflectableProjectiles)
{
	if (!g_config.airblast_tfdb_orbit_enable)
		return;

	float avgLatency = g_interfaces.engine->GetNetChannelInfo()->GetAvgLatency(MAX_FLOWS);
	CBaseEntity* pClosestProjectile{};
	Vector closestPredictedCenter;
	float closestDistance = -1337.f;
	for (CBaseEntity* pProjectile : deflectableProjectiles)
	{
		Vector vel;
		pProjectile->EstimateAbsVelocity(vel);

		Vector predictpos = pProjectile->GetWorldSpaceCenter() + (vel * avgLatency);
		float dist = (utils::GetLocalViewOrigin() - predictpos).length();

		if (dist < closestDistance || !pClosestProjectile)
		{
			pClosestProjectile = pProjectile;
			closestDistance = dist;
			closestPredictedCenter = predictpos;
		}
	}

	if (!pClosestProjectile || closestDistance > 1024.f)
		return;

	Vector vel; pClosestProjectile->EstimateAbsVelocity(vel);

	Vector perpendicular_hvel = Vector(vel.y, -vel.x, 0);

	VectorAngles(perpendicular_hvel, cmd.viewangles);
	cmd.forwardmove = 450.f;
	cmd.sidemove = 0.f;
}