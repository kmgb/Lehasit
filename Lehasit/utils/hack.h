#pragma once

#include "../Interfaces.h" // TODO: Don't include this in a header file, we don't want things to expose raw interfaces
#include "../Offsets.h"

class CTFPlayer;
class CUserCmd;
enum HitboxID;
struct trace_t;
class Vector;
typedef Vector QAngle;

namespace utils
{
	QAngle CalculateAngle(Vector start, Vector end);
	void ClampViewAngles(QAngle& angles);
	//float NormalizeAngle(float angle); // Moved to mathlib.h
	void NormalizeAngles(QAngle& angles);
	void MovementFix(CUserCmd* cmd, const QAngle& old_angles);
	Vector GetHitboxCenter(CTFPlayer* player, int hitbox_id);
	void TraceBullet(CBaseEntity* pLocalPlayer, Vector end, trace_t& out_trace);

	// TODO: Figure out how to forward declare templates so I don't have to include interfaces
	// in a header
	template<typename T>
	inline T* GetEntity(int index)
	{
		return reinterpret_cast<T*>(g_interfaces.entityList->get_client_entity(index));
	}

	template<typename T>
	inline T* get_entity_by_handle(unsigned long handle)
	{
		return (T*)g_interfaces.entityList->get_client_entity_from_handle(handle);
	}

	inline CTFPlayer* GetLocalPlayer()
	{
		return (CTFPlayer*)g_interfaces.entityList->get_client_entity(g_interfaces.engine->GetLocalPlayer());
	}

	Vector GetLocalViewOrigin();

	inline int GetMaxPlayers()
	{
		return g_interfaces.engine->GetMaxClients();
	}

	inline int GetMaxEntities()
	{
		return g_interfaces.entityList->GetHighestEntityIndex();
	}
}