#include "stdafx.h"
#include "cheats.h"

#include "utils/hack.h"
#include "sdk/tf.h"
#include "config.h"

void glow::Run()
{
	CTFPlayer* local_player = utils::GetLocalPlayer();

	for (int i = 1; i < utils::GetMaxPlayers() + 1; ++i)
	{
		CTFPlayer* player = utils::GetEntity<CTFPlayer>(i);
		if (!player)
			continue;

		bool should_glow = (!player->IsDormant() && player->getLifestate() == LIFE_ALIVE);


		if (g_config.glow_type == 0)
			should_glow = false;
		else if (g_config.glow_type == 1)
			should_glow = should_glow && player->GetTeamNum() != local_player->GetTeamNum();

		bool* glow_enabled = OffsetPointer<bool>(player, g_offsets.TFPlayer.m_bGlowEnabled);

		// Update the glow effect only if needed
		if (*glow_enabled != should_glow)
		{
			*glow_enabled = should_glow;
			player->updateGlowEffect();
		}
	}
}