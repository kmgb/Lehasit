#include "stdafx.h"
#include "hack.h"

#include <math.h>

#include "../sdk/CUserCmd.h"
#include "../sdk/mathlib.h"
#include "../sdk/studio.h"
#include "../sdk/tf.h"
#include "../sdk/trace.h"
#include "../Globals.h"

QAngle utils::CalculateAngle(Vector start, Vector end)
{
	QAngle ang;
	Vector delta = start - end;

	ang[PITCH] = RadToDeg(asinf(delta.z / delta.length()));
	ang[YAW] = RadToDeg(atan2f(delta.y, delta.x)) - 180.f; // 0..360 -> -180..180
	ang[ROLL] = 0.f;

	return ang;
}

void utils::ClampViewAngles(QAngle& ang)
{
	ang[YAW] += 180.f; // -180..180 -> 0..360
	ang[YAW] = fmodf(ang[YAW], 360.f);

	if (ang[YAW] < 0.f) // Loop negative angles
		ang[YAW] += 360.f;

	ang[YAW] -= 180.f; // 0..360 -> -180..180

	if (ang[PITCH] > 89.f)
	{
		ang[PITCH] = 89.f;
	}
	else if (ang[PITCH] < -89.f)
	{
		ang[PITCH] = -89.f;
	}

	ang[ROLL] = 0.f;
}

void utils::NormalizeAngles(QAngle& angles)
{
	angles[YAW] = NormalizeAngle(angles[YAW]);
	angles[PITCH] = NormalizeAngle(angles[PITCH]);
	angles[ROLL] = NormalizeAngle(angles[ROLL]);
}

void utils::MovementFix(CUserCmd& cmd, const QAngle& oldAngles)
{
	float newYaw = cmd.viewangles[YAW];
	float oldYaw = oldAngles[YAW];

	// The angle of movement relative to viewangles
	float moveYawRad = atan2(cmd.sidemove, cmd.forwardmove);

	// Rotate movement yaw by the yaw delta so that movement is
	// relative to oldYaw instead of newYaw
	//moveYaw += DegToRad((newYaw + 180.f) - (oldYaw + 180.f));
	moveYawRad += DegToRad(NormalizeAngle(newYaw - oldYaw));

	float spd = Vector(cmd.forwardmove, cmd.sidemove, 0.f).length2D();
	cmd.forwardmove = cos(moveYawRad) * spd;
	cmd.sidemove = sin(moveYawRad) * spd;

	// If pitch goes upside down (> 90 or <= -90), forwardmove reverses
	// meaning, so we have to re-reverse forwardmove
	//Vector3 viewPunch = *OffsetPointer<Vector3>(GetLocalPlayer(), g_offsets.player_viewPunch);

	float oldPitch = NormalizeAngle(cmd.viewangles[PITCH]);
	float newPitch = NormalizeAngle(oldAngles[PITCH]);// +viewPunch[PITCH];

	bool oldBackwards = (oldPitch > 90.f || oldPitch <= -90.f);
	bool newBackwards = (newPitch > 90.f || newPitch <= -90.f);
	if (oldBackwards != newBackwards)
		cmd.forwardmove = -cmd.forwardmove;
}

Vector utils::GetHitboxCenter(CTFPlayer* player, int hitbox_id)
{
	static matrix3x4_t boneToWorld[128];
	if (player->setupBones(boneToWorld, 128, 0x100, g_pGlobals->curtime))
	{
		studiohdr_t* hdr = g_interfaces.modelInfo->getStudioModel(player->getModel());

		if (hdr)
		{
			mstudiobbox_t* hitbox = hdr->pHitbox(hitbox_id, player->getHitboxSet());

			if (hitbox)
			{
				Vector hitbox_min, hitbox_max;
				TransformVector(hitbox->bbmin, boneToWorld[hitbox->bone], hitbox_min);
				TransformVector(hitbox->bbmax, boneToWorld[hitbox->bone], hitbox_max);

				return (hitbox_min + hitbox_max) / 2.f;
			}
		}
	}

	return{0, 0, 0};
}

class CTraceFilterPlayerAndWorld : public ITraceFilter
{
public:
	void* pSkip;

	virtual bool shouldHitEntity(void* pEntity, int mask)
	{
		CBaseEntity* pBaseEntity = (CBaseEntity*)pEntity;

		return (pEntity != pSkip && pBaseEntity->getIndex() > 0 && pBaseEntity->getIndex() < utils::GetMaxPlayers() + 1);
	}

	virtual TraceType_t getTraceType() const
	{
		return TRACE_EVERYTHING;
	}
};

void utils::TraceBullet(CBaseEntity& localPlayer, Vector end, trace_t& out_trace)
{
	// TODO: Make into nice std::vector list of skips
	class CTraceFilterSkip : public ITraceFilter
	{
	public:
		bool shouldHitEntity(void* pEntityHandle, int contentsMask)
		{
			return (pEntityHandle != pSkip);
		}

		TraceType_t getTraceType() const
		{
			return TRACE_EVERYTHING;
		}

		CBaseEntity* pSkip;
	};

	CTraceFilterSkip filter; // Used to filter out local player collisions
	Ray_t ray; // Defines the bounds of the traceray

	Vector start = utils::GetLocalViewOrigin();

	ray.init(start, end);

	filter.pSkip = &localPlayer;

	g_interfaces.trace->traceRay(ray, MASK_SOLID | CONTENTS_HITBOX, &filter, &out_trace);
}

Vector utils::GetLocalViewOrigin()
{
	CTFPlayer* pLocalPlayer = GetLocalPlayer();
	if (!pLocalPlayer)
		return { 0, 0, 0 };

	return pLocalPlayer->getOrigin() + *OffsetPointer<Vector>(GetLocalPlayer(), g_offsets.LocalPlayerExclusive.m_vecViewOffset);
}
