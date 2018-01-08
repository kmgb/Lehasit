#include "stdafx.h"
#include "draw.h"

#include <stdio.h>
#include <Windows.h>
#undef DrawText

#include "../Interfaces.h"
#include "../sdk/mathlib.h"

HFont menuFont;

enum FontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

bool utils::InitializeDrawing()
{
	if (!g_interfaces.surface)
		return false;

	menuFont = g_interfaces.surface->CreateFont();
	g_interfaces.surface->SetFontGlyphSet(menuFont, "Tahoma", 14, 700, 0, 0, FONTFLAG_ANTIALIAS);

	return true;
}

// Returns true if the position is within view
bool utils::ToScreenCoordinates(Vector world_pos, int& out_x, int& out_y)
{
	// Basically pa$te from gir's ds base, don't know how it works, but it works fine
	const matrix3x4_t& world_to_screen = g_interfaces.engine->WorldToScreenMatrix();
	float w = (world_to_screen[3][0] * world_pos.x + world_to_screen[3][1] * world_pos.y + world_to_screen[3][2] * world_pos.z + world_to_screen[3][3]); // Calculate angle in comparison to viewangs
	if (w > 0.001f) // If the object is within view.
	{
		float iw = 1.f / w; // Divide 1 by the angle.
		int width, height;
		g_interfaces.engine->GetScreenSize(width, height);
		out_x = (int)((width / 2.f) + (0.5f * ((world_to_screen[0][0] * world_pos.x + world_to_screen[0][1] * world_pos.y + world_to_screen[0][2] * world_pos.z + world_to_screen[0][3]) * iw) * width + 0.5f));
		out_y = (int)((height / 2.f) - (0.5f * ((world_to_screen[1][0] * world_pos.x + world_to_screen[1][1] * world_pos.y + world_to_screen[1][2] * world_pos.z + world_to_screen[1][3]) * iw) * height + 0.5f));
		return true;
	}

	return false;
}

void utils::DrawCube(Vector min, Vector max, Color color)
{
	int points_x[8], points_y[8];

	/*
	    5 - - - 6
	  /       / |
	1 - - - 2   |
	|       |   |
	|   7   |   8
	|       | /
	3 - - - 4
	*/

	if (!ToScreenCoordinates(min, points_x[0], points_y[0])
		|| !ToScreenCoordinates(Vector(max.x, min.y, min.z), points_x[1], points_y[1])
		|| !ToScreenCoordinates(Vector(min.x, min.y, max.z), points_x[2], points_y[2])
		|| !ToScreenCoordinates(Vector(max.x, min.y, max.z), points_x[3], points_y[3])
		|| !ToScreenCoordinates(Vector(min.x, max.y, min.z), points_x[4], points_y[4])
		|| !ToScreenCoordinates(Vector(max.x, max.y, min.z), points_x[5], points_y[5])
		|| !ToScreenCoordinates(Vector(min.x, max.y, max.z), points_x[6], points_y[6])
		|| !ToScreenCoordinates(max, points_x[7], points_y[7])
		) return;

	g_interfaces.surface->DrawSetColor(color.r, color.g, color.b, color.a);

	g_interfaces.surface->DrawLine(points_x[0], points_y[0], points_x[1], points_y[1]); // 1 -> 2
	g_interfaces.surface->DrawLine(points_x[0], points_y[0], points_x[2], points_y[2]); // 1 -> 3
	g_interfaces.surface->DrawLine(points_x[0], points_y[0], points_x[4], points_y[4]); // 1 -> 5
	g_interfaces.surface->DrawLine(points_x[1], points_y[1], points_x[3], points_y[3]); // 2 -> 4
	g_interfaces.surface->DrawLine(points_x[1], points_y[1], points_x[5], points_y[5]); // 2 -> 6
	g_interfaces.surface->DrawLine(points_x[2], points_y[2], points_x[3], points_y[3]); // 3 -> 4
	g_interfaces.surface->DrawLine(points_x[2], points_y[2], points_x[6], points_y[6]); // 3 -> 7
	g_interfaces.surface->DrawLine(points_x[3], points_y[3], points_x[7], points_y[7]); // 4 -> 8
	g_interfaces.surface->DrawLine(points_x[4], points_y[4], points_x[5], points_y[5]); // 5 -> 6
	g_interfaces.surface->DrawLine(points_x[4], points_y[4], points_x[6], points_y[6]); // 5 -> 7
	g_interfaces.surface->DrawLine(points_x[5], points_y[5], points_x[7], points_y[7]); // 6 -> 8
	g_interfaces.surface->DrawLine(points_x[6], points_y[6], points_x[7], points_y[7]); // 7 -> 8
}

void utils::DrawLine(int x, int y, int x2, int y2, Color color)
{
	g_interfaces.surface->DrawSetColor(color.r, color.g, color.b, color.a);
	g_interfaces.surface->DrawLine(x, y, x2, y2);
}

void utils::DrawRectangle(int x, int y, int w, int h, Color color)
{
	g_interfaces.surface->DrawSetColor(color.r, color.g, color.b, color.a);
	g_interfaces.surface->DrawRect(x, y, x + w, y + h);
}

void utils::DrawFilledRectangle(int x, int y, int w, int h, Color color)
{
	g_interfaces.surface->DrawSetColor(color.r, color.g, color.b, color.a);
	g_interfaces.surface->DrawFillRect(x, y, x + w, y + h);
}

void utils::DrawText(int x, int y, HFont font, Color color, const char* text, ...)
{
	va_list arg_list;
	char buffer[200];
	wchar_t w_buffer[200];

	va_start(arg_list, text);
	vsprintf_s(buffer, text, arg_list);
	va_end(arg_list);
	wsprintfW(w_buffer, L"%S", buffer); // Convert to wide char

	g_interfaces.surface->set_text_font(font);
	g_interfaces.surface->set_text_color(color.r, color.g, color.b, color.a);
	g_interfaces.surface->set_text_position(x, y);
	g_interfaces.surface->DrawText(w_buffer, wcslen(w_buffer));
}

int utils::GetTextWidth(HFont font, const char* text)
{
	wchar_t w_buffer[128];

	wsprintfW(w_buffer, L"%S", text); // Convert to wide char

	int tall, wide;
	g_interfaces.surface->GetTextSize(font, w_buffer, wide, tall);
	return wide;
}