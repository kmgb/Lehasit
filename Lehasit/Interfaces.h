#pragma once

#include "utils/memory.h"
#include "sdk/mathlib.h"

class CBaseEntity;
class ClientClass;
class CUserCmd;
class ITraceFilter;
struct studiohdr_t;
struct Ray_t;
struct studiohdr_t;
struct trace_t;
class IMoveHelper;

class IBaseClientDLL;
class IClientEntityList;
class IClientMode;
class IEngineTrace;
class IVModelInfoClient;
class IVEngineClient;
class IPrediction;
class ISurface;
class IPanel;

class INetChannelInfo;

using HFont = unsigned long;

struct Interfaces
{
	IBaseClientDLL* client;
	IClientMode* clientMode;
	IClientEntityList* entityList;
	IEngineTrace* trace;
	IVEngineClient* engine;
	IVModelInfoClient* modelInfo;
	IPrediction* prediction;
	ISurface* surface;
	IPanel* panel;

	bool load(); // Returns whether it got all the interfaces
};

extern Interfaces g_interfaces;

class IBaseClientDLL
{
public:
	inline ClientClass* get_all_classes()
	{
		typedef ClientClass*(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(this, 8))(this);
	}
};

class IClientEntityList
{
public:
	inline void* get_client_entity(int index)
	{
		typedef void*(__thiscall* Fn)(void*, int);
		return ((Fn)utils::GetVirtualMethod(this, 3))(this, index);
	}

	inline void* get_client_entity_by_handle(unsigned long handle)
	{
		typedef void*(__thiscall* Fn)(void*, unsigned long);
		return ((Fn)utils::GetVirtualMethod(this, 4))(this, handle);
	}

	inline int GetHighestEntityIndex()
	{
		typedef int(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(this, 6))(this);
	}
};

class IClientMode
{
public:
};

class IEngineTrace
{
public:
	inline void traceRay(const Ray_t& ray, unsigned int mask, ITraceFilter* pTraceFilter, trace_t* pTrace)
	{
		typedef void(__thiscall* Fn)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		((Fn)utils::GetVirtualMethod(this, 4))(this, ray, mask, pTraceFilter, pTrace);
	}
};

class IVEngineClient
{
public:
	void GetScreenSize(int& width, int& height)
	{
		typedef void(__thiscall* Fn)(void*, int&, int&);
		return ((Fn)utils::GetVirtualMethod(this, 5))(this, width, height);
	}

	inline int GetLocalPlayer()
	{
		typedef int(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(this, 12))(this);
	}

	inline void set_view_angles(Vector& v)
	{
		typedef void(__thiscall* Fn)(void*, Vector&);
		return ((Fn)utils::GetVirtualMethod(this, 20))(this, v);
	}

	inline int GetMaxClients()
	{
		typedef int(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(this, 21))(this);
	}

	inline bool IsInGame()
	{
		typedef bool(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(this, 26))(this);
	}

	inline const matrix3x4_t& WorldToScreenMatrix(void)
	{
		typedef const matrix3x4_t& (__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(this, 36))(this);
	}

	inline INetChannelInfo* GetNetChannelInfo()
	{
		using Fn = INetChannelInfo*(__thiscall*)(void*);
		return ((Fn)utils::GetVirtualMethod(this, 72))(this);
	}

	inline void clientCmd(const char* command)
	{
		typedef void(__thiscall* Fn)(void*, const char*);
		return ((Fn)utils::GetVirtualMethod(this, 106))(this, command);
	}
};

class IVModelInfoClient
{
public:
	inline studiohdr_t* getStudioModel(void* pModel)
	{
		typedef studiohdr_t*(__thiscall* Fn)(void*, void*);
		return ((Fn)utils::GetVirtualMethod(this, 28))(this, pModel);
	}
};

class IPrediction
{
public:
	inline void runCommand(CBaseEntity* player, CUserCmd* cmd, IMoveHelper* movehelper)
	{
		typedef void(__thiscall* Fn)(void*, CBaseEntity*, CUserCmd*, IMoveHelper*);
		((Fn)utils::GetVirtualMethod(this, 17))(this, player, cmd, movehelper);
	}
};

class ISurface
{
public:
	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* Fn)(void*, int, int, int, int);
		((Fn)utils::GetVirtualMethod(this, 11))(this, r, g, b, a);
	}
	void DrawFillRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn)(void*, int, int, int, int);
		((Fn)utils::GetVirtualMethod(this, 12))(this, x0, y0, x1, y1);
	}
	void DrawRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn)(void*, int, int, int, int);
		((Fn)utils::GetVirtualMethod(this, 14))(this, x0, y0, x1, y1);
	}
	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn)(void*, int, int, int, int);
		((Fn)utils::GetVirtualMethod(this, 15))(this, x0, y0, x1, y1);
	}
	void set_text_font(HFont font)
	{
		typedef void(__thiscall* Fn)(void*, HFont);
		((Fn)utils::GetVirtualMethod(this, 17))(this, font);
	}
	void set_text_color(int r, int g, int b, int a)
	{
		typedef void(__thiscall* Fn)(void*, int, int, int, int);
		((Fn)utils::GetVirtualMethod(this, 19))(this, r, g, b, a);
	}
	void set_text_position(int x, int y)
	{
		typedef void(__thiscall* Fn)(void*, int, int);
		((Fn)utils::GetVirtualMethod(this, 20))(this, x, y);
	}
	void DrawText(const wchar_t *text, int textLen)
	{
		typedef void(__thiscall* Fn)(void*, const wchar_t *, int, int);
		return ((Fn)utils::GetVirtualMethod(this, 22))(this, text, textLen, 0);
	}
	unsigned long CreateFont()
	{
		typedef unsigned int(__thiscall* Fn)(void*);
		return ((Fn)utils::GetVirtualMethod(this, 66))(this);
	}
	void SetFontGlyphSet(unsigned long &font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* Fn)(void*, unsigned long, const char*, int, int, int, int, int, int, int);
		((Fn)utils::GetVirtualMethod(this, 67))(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}
	bool add_custom_font_file(const char *fontName, const char *fontFileName)
	{
		typedef bool(__thiscall* Fn)(void*, const char*, const char*);
		return ((Fn)utils::GetVirtualMethod(this, 68))(this, fontName, fontFileName);
	}
	void GetTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall)
	{
		typedef void(__thiscall* Fn)(void*, unsigned long, const wchar_t *, int&, int&);
		((Fn)utils::GetVirtualMethod(this, 75))(this, font, text, wide, tall);
	}
	void draw_circle_outline(int x, int y, int radius, int segments)
	{
		typedef void(__thiscall* Fn)(void*, int, int, int, int);
		((Fn)utils::GetVirtualMethod(this, 99))(this, x, y, radius, segments);
	}
};

class IPanel
{
public:
	const char* get_name(unsigned int vgui_panel)
	{
		typedef const char*(__thiscall* Fn)(void*, unsigned int);
		return ((Fn)utils::GetVirtualMethod(this, 36))(this, vgui_panel);
	}
};