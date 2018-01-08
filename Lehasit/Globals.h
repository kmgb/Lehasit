#pragma once

#include "sdk/CGlobalVarsBase.h"
#include "sdk/IMoveHelper.h"
class Vector;

extern CGlobalVarsBase* g_pGlobals;
extern IMoveHelper* g_pMoveHelper;

using CBaseEntity_EstimateAbsVelocity = void(__thiscall*)(void*, Vector&);
extern CBaseEntity_EstimateAbsVelocity g_CBaseEntity_EstimateAbsVelocity;