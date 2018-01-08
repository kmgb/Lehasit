#include "stdafx.h"
#include "Offsets.h"

#include <assert.h>
#include <Windows.h>
#include <stdio.h>

#include "sdk/Recv.h"
#include "utils/recv.h"

Offsets g_offsets;

int GetOffset(const char* table_name, const char* prop_name);

bool Offsets::load()
{
	memset(this, -1, sizeof(*this));

	LocalPlayerExclusive.m_vecViewOffset = GetOffset("DT_LocalPlayerExclusive", "m_vecViewOffset[0]");
	LocalPlayerExclusive.m_nTickBase = GetOffset("DT_LocalPlayerExclusive", "m_nTickBase");
	BaseAnimating.m_nHitboxSet = GetOffset("DT_BaseAnimating", "m_nHitboxSet");
	BaseEntity.m_iTeamNum = GetOffset("DT_BaseEntity", "m_iTeamNum");
	BasePlayer.m_iHealth = GetOffset("DT_BasePlayer", "m_iHealth");
	BasePlayer.m_lifeState = GetOffset("DT_BasePlayer", "m_lifeState");
	BasePlayer.m_fFlags = GetOffset("DT_BasePlayer", "m_fFlags");
	BaseCombatCharacter.m_hActiveWeapon = GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
	LocalActiveWeaponData.m_flNextPrimaryAttack = GetOffset("DT_LocalActiveWeaponData", "m_flNextPrimaryAttack");
	LocalActiveWeaponData.m_flNextSecondaryAttack = GetOffset("DT_LocalActiveWeaponData", "m_flNextSecondaryAttack");

	int m_Shared = GetOffset("DT_TFPlayer", "m_Shared");
	assert(m_Shared != -1);
	TFPlayerShared.m_nPlayerCond = GetOffset("DT_TFPlayerShared", "m_nPlayerCond") + m_Shared;
	TFPlayerShared.m_nPlayerCondEx = GetOffset("DT_TFPlayerShared", "m_nPlayerCondEx") + m_Shared;
	TFPlayerShared.m_nPlayerCondEx2 = GetOffset("DT_TFPlayerShared", "m_nPlayerCondEx2") + m_Shared;
	TFPlayer.m_bGlowEnabled = GetOffset("DT_TFPlayer", "m_bGlowEnabled");
	TFPlayer.m_nForceTauntCam = GetOffset("DT_TFPlayer", "m_nForceTauntCam");
	TFPlayer.m_angEyeAngles = GetOffset("DT_TFNonLocalPlayerExclusive", "m_angEyeAngles[0]");

	// Make sure we have all of our offsets
	for (int* pI = (int*)this; pI < (int*)((char*)this + sizeof(*this)); pI++)
	{
		if (*pI == -1)
		{
			/*char buff[50];
			sprintf_s(buff, "%d", ((size_t)pI - (size_t)this) / sizeof(int));
			MessageBoxA(NULL, buff, NULL, MB_OK);*/
			return false;
		}
	}

	return true;
}

int GetOffset(const char* table_name, const char* prop_name)
{
	// Find the data table
	RecvTable* table = utils::find_table(table_name);

	if (table)
	{
		RecvProp* prop = utils::find_prop_in_table(table, prop_name);
		if (prop)
			return prop->offset;
	}

	return -1;
}