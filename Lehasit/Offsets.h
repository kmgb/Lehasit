#pragma once

struct Offsets
{
	struct
	{
		int m_vecViewOffset,
			m_nTickBase;
	}LocalPlayerExclusive;

	struct
	{
		int m_iTeamNum;
	}BaseEntity;

	struct
	{
		int m_iHealth,
			m_lifeState,
			m_fFlags;
	}BasePlayer;

	struct
	{
		int m_hActiveWeapon;
	}BaseCombatCharacter;

	struct
	{
		int m_flNextPrimaryAttack,
			m_flNextSecondaryAttack;
	}LocalActiveWeaponData;

	struct
	{
		int m_nHitboxSet;
	}BaseAnimating;

	struct
	{
		int m_nPlayerCond,
			m_nPlayerCondEx,
			m_nPlayerCondEx2;
	}TFPlayerShared;

	struct
	{
		int m_bGlowEnabled,
			m_nForceTauntCam,
			m_angEyeAngles;
	}TFPlayer;

	bool load();
};

template<typename T>
inline T* OffsetPointer(void* ptr, size_t offset)
{
	return reinterpret_cast<T*>((size_t)ptr + offset);
}

extern Offsets g_offsets;