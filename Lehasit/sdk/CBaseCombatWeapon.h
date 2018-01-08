#pragma once

#include "../Offsets.h"
#include "CBaseEntity.h"

class CBaseCombatWeapon : public CBaseEntity {
public:
	inline float getNextPrimaryAttackTime() {
		return *OffsetPointer<float>(this, g_offsets.LocalActiveWeaponData.m_flNextPrimaryAttack);
	}

	inline float getNextSecondaryAttackTime()
	{
		return *OffsetPointer<float>(this, g_offsets.LocalActiveWeaponData.m_flNextSecondaryAttack);
	}

	inline float getSpread() {
		typedef float(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(this, 453))(this);
	}
};