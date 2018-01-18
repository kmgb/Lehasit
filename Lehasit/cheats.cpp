#include "stdafx.h"

#include "cheats.h"
#include "sdk\tf.h"
#include "sdk\Recv.h"

std::vector<CBaseEntity*> GetDeflectableProjectiles(CTFPlayer& localPlayer)
{
	std::vector<CBaseEntity*> result;

	// +1 since 0 is the world
	for (int i = g_interfaces.engine->GetMaxClients() + 1; i <= g_interfaces.entityList->GetHighestEntityIndex(); i++)
	{
		auto* pEntity = utils::GetEntity<CBaseEntity>(i);
		if (!pEntity || pEntity->IsDormant() || pEntity->GetTeamNum() == localPlayer.GetTeamNum())
			continue;

		static const char* deflectableNames[] = { "CTFProjectile_Rocket", "CTFProjectile_Flare",
			"CTFProjectile_EnergyBall", "CTFProjectile_HealingBolt",
			"CTFProjectile_Arrow", "CTFProjectile_SentryRocket",
			"CTFProjectile_Throwable", "CTFThrowable",
			"CTFProjectile_Cleaver", "CTFProjectile_JarMilk",
			"CTFProjectile_Jar", "CTFStunBall",
			"CTFGrenadePipebombProjectile", "CTFBall_Ornament" };

		const char* classname = pEntity->GetClientClass()->name;

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