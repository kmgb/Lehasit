#pragma once

#include "../utils/memory.h"
#include "../Globals.h"
#include "../Offsets.h"
#include "mathlib.h"

class ClientClass;

class CBaseEntity
{
public:
	inline const Vector& getOrigin()
	{
		typedef Vector&(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(this, 9))(this);
	}
	Vector GetWorldSpaceCenter()
	{
		Vector min, max;
		get_render_bounds(min, max);
		Vector center = getOrigin();
		center.z += (max.z + min.z) / 2;
		return center;
	}

	inline const QAngle& getAngles()
	{
		typedef QAngle&(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(this, 10))(this);
	}
	inline void* getModel()
	{
		void* pRenderable = (char*)this + 4;
		typedef void*(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(pRenderable, 9))(pRenderable);
	}
	inline bool setupBones(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		void* pRenderable = (char*)this + 4;
		typedef bool(__thiscall* Fn)(void*, matrix3x4_t*, int, int, float);
		return ((Fn)utils::GetVirtualMethod(pRenderable, 16))(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}
	inline void get_render_bounds(Vector& mins, Vector& maxs)
	{
		void* pRenderable = (char*)this + 4;
		typedef void(__thiscall* Fn)(void*, Vector&, Vector&);
		((Fn)utils::GetVirtualMethod(pRenderable, 20))(pRenderable, mins, maxs);
	}
	inline ClientClass* GetClientClass()
	{
		void* pNetworkable = (char*)this + 8;
		typedef ClientClass*(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(pNetworkable, 2))(pNetworkable);
	}
	inline bool IsDormant()
	{
		void* pNetworkable = (char*)this + 8;
		typedef bool(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(pNetworkable, 8))(pNetworkable);
	}
	inline int getIndex()
	{
		void* pNetworkable = (char*)this + 8;
		typedef int(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(pNetworkable, 9))(pNetworkable);
	}

	inline int GetTeamNum()
	{
		return *OffsetPointer<int>(this, g_offsets.BaseEntity.m_iTeamNum);
	}

	inline void EstimateAbsVelocity(Vector& vel)
	{
		g_CBaseEntity_EstimateAbsVelocity(this, vel);
	}
};