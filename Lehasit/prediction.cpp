#include "stdafx.h"
#include "cheats.h"

#include "Globals.h"
#include "sdk/CUserCmd.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "sdk/tf.h"

float oldCurtime, oldFrametime;

void prediction::Apply(CUserCmd* pCmd)
{
	oldCurtime = g_pGlobals->curtime;
	oldFrametime = g_pGlobals->frametime;

	CTFPlayer* pLocalPlayer = utils::GetLocalPlayer();
	if (!pLocalPlayer || pLocalPlayer->getLifestate() != LIFE_ALIVE)
		return;

	g_pGlobals->curtime = *OffsetPointer<int>(pLocalPlayer, g_offsets.LocalPlayerExclusive.m_nTickBase) * g_pGlobals->interval_per_tick;
	g_pGlobals->frametime = g_pGlobals->interval_per_tick;

	if (g_pMoveHelper)
	{
		CUserCmd tempCmd = *pCmd;
		tempCmd.buttons &= ~(IN_ATTACK | IN_ATTACK2);
		int oldFlags = pLocalPlayer->getFlags();
		g_interfaces.prediction->runCommand(pLocalPlayer, &tempCmd, g_pMoveHelper);
		*OffsetPointer<int>(pLocalPlayer, g_offsets.BasePlayer.m_fFlags) = oldFlags;
	}
}

void prediction::Restore()
{
	g_pGlobals->curtime = oldCurtime;
	g_pGlobals->frametime = oldFrametime;
}