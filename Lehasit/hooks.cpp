#include "stdafx.h"
#include "hooks.h"

#include "sdk/CUserCmd.h"
#include "sdk/buttons.h"
#include "Interfaces.h"
#include "Globals.h"
#include "menu.h"

#include "cheats.h"
#include "Offsets.h"
#include "utils/hack.h"
#include "utils/draw.h"
#include "config.h"
#include "sdk/tf.h"

#include <Windows.h>

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/client/clientmode_shared.cpp#L396
bool __fastcall New_CreateMove(IClientMode* this_, int edx, float inputSampleTime, CUserCmd* pCmd);
void __fastcall New_RunCommand(IPrediction* this_, int edx, CBaseEntity* player, CUserCmd* pCmd, IMoveHelper* moveHelper);
int __fastcall New_IN_KeyEvent(IBaseClientDLL* this_, int edx, int eventcode, ButtonCode_t keynum, const char* currentBinding);
void __fastcall New_PaintTraverse(IPanel* this_, int edx, unsigned int panel, bool forceRepaint, bool allowForce);

typedef bool(__thiscall* CreateMoveFn)(void*, float, CUserCmd*);
typedef void(__thiscall* RunCommandFn)(void*, CBaseEntity*, CUserCmd*, IMoveHelper*);
typedef int(__thiscall* IN_KeyEventFn)(void*, int, ButtonCode_t, const char*);
typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);

CreateMoveFn Original_CreateMove;
RunCommandFn Original_RunCommand;
IN_KeyEventFn Original_IN_KeyEvent;
PaintTraverseFn Original_PaintTraverse;

void ApplyHooks()
{
	Original_IN_KeyEvent = (IN_KeyEventFn)utils::GetVirtualMethod(g_interfaces.client, 20);
	utils::HookVirtualMethod(g_interfaces.client, 20, New_IN_KeyEvent);

	Original_CreateMove = (CreateMoveFn)utils::GetVirtualMethod(g_interfaces.clientMode, 21);
	utils::HookVirtualMethod(g_interfaces.clientMode, 21, New_CreateMove);

	Original_PaintTraverse = (PaintTraverseFn)utils::GetVirtualMethod(g_interfaces.panel, 41);
	utils::HookVirtualMethod(g_interfaces.panel, 41, New_PaintTraverse);

	g_pMoveHelper = nullptr;

	Original_RunCommand = (RunCommandFn)utils::GetVirtualMethod(g_interfaces.prediction, 17);
	utils::HookVirtualMethod(g_interfaces.prediction, 17, New_RunCommand);
}

// Use __fastcall to match a bunch of __thiscall things, in thiscall, the this ptr is passed via ecx. In fastcall, the first two
// word sized params are passed via ecx and edx, so the first parameter matches up with `this' and we just need a dummy param
// to cover the edx push.
bool __fastcall New_CreateMove(IClientMode* this_, int edx, float inputSampleTime, CUserCmd* pCmd)
{
	bool returnValue = Original_CreateMove(this_, inputSampleTime, pCmd);

	if (pCmd->command_number == 0) // Ignore call from CInput::ExtraMouseSample
		return returnValue;

	CTFPlayer* pLocalPlayer = utils::GetLocalPlayer();
	if (!pLocalPlayer)
		return false;

	if (g_config.thirdperson_enable)
	{
		*OffsetPointer<int>(pLocalPlayer, g_offsets.TFPlayer.m_nForceTauntCam) = 1;
	}
	else
	{
		*OffsetPointer<int>(pLocalPlayer, g_offsets.TFPlayer.m_nForceTauntCam) = 0;
	}

	if (pLocalPlayer->getLifestate() == LIFE_ALIVE)
	{
		prediction::Apply(pCmd);

		std::vector<CBaseEntity*> deflectableProjectiles = GetDeflectableProjectiles(pLocalPlayer);

		if (!airblast::Run(pCmd, deflectableProjectiles))
			orbit::Run(pCmd, deflectableProjectiles);

		prediction::Restore();
	}

	return false; // Returning true calls SetViewAngles(cmd->viewangles), which we want the aimbot to be able to decide
}

void __fastcall New_RunCommand(IPrediction* this_, int edx, CBaseEntity* pPlayer, CUserCmd* pCmd, IMoveHelper* pMoveHelper)
{
	Original_RunCommand(this_, pPlayer, pCmd, pMoveHelper);

	g_pMoveHelper = pMoveHelper;

	// Unhook ourselves, we got the pointer
	utils::HookVirtualMethod(g_interfaces.prediction, 17, Original_RunCommand);
}

int __fastcall New_IN_KeyEvent(IBaseClientDLL* this_, int edx, int eventcode, ButtonCode_t keynum, const char* current_binding)
{
	if (eventcode == 1)
	{
		if (g_menu.handleInput(keynum))
			return 0; // Input was consumed
	}

	return Original_IN_KeyEvent(this_, eventcode, keynum, current_binding);
}

void __fastcall New_PaintTraverse(IPanel* this_, int edx, unsigned int vgui_panel, bool force_repaint, bool allow_force)
{
	Original_PaintTraverse(this_, vgui_panel, force_repaint, allow_force);

	// TODO: Do this better
	static unsigned int focus_overlay_panel = -1;

	if (focus_overlay_panel == -1)
	{
		if (strcmp(g_interfaces.panel->get_name(vgui_panel), "FocusOverlayPanel") == 0)
			focus_overlay_panel = vgui_panel;

		g_menu.setup();
	}

	if (vgui_panel != focus_overlay_panel)
		return;

	if (!g_interfaces.engine->IsInGame())
		return;

	g_menu.draw(0, 0);

	// 3d cross at eye position
	if (g_config.thirdperson_enable)
	{
		Vector eye = utils::GetLocalViewOrigin();
		ScreenPoint points[3][2];
		utils::ToScreenCoordinates(eye + Vector(0, 0, +2), points[0][0].x, points[0][0].y);
		utils::ToScreenCoordinates(eye + Vector(0, 0, -2), points[0][1].x, points[0][1].y);
		utils::ToScreenCoordinates(eye + Vector(0, +2, 0), points[1][0].x, points[1][0].y);
		utils::ToScreenCoordinates(eye + Vector(0, -2, 0), points[1][1].x, points[1][1].y);
		utils::ToScreenCoordinates(eye + Vector(+2, 0, 0), points[2][0].x, points[2][0].y);
		utils::ToScreenCoordinates(eye + Vector(-2, 0, 0), points[2][1].x, points[2][1].y);

		for (ScreenPoint(&p)[2] : points)
		{
			utils::DrawLine(p[0].x, p[0].y, p[1].x, p[1].y, COLOR_RED);
		}
	}

	glow::Run(); // Only need to update once a frame
}
