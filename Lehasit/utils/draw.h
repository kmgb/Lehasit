#pragma once

#include "../sdk/Vector.h"

struct Color
{
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		: r(r), g(g), b(b), a(a)
	{
	}

	char r, g, b, a;
};

const Color COLOR_WHITE = { 255, 255, 255, 255 },
COLOR_RED = { 255, 0, 0, 255 },
COLOR_GREEN = { 0, 255, 0, 255 },
COLOR_BLUE = { 0, 0, 255, 255 },
COLOR_BLACK = { 0, 0, 0, 255 },
COLOR_ENEMY = { 255, 50, 50, 255 },
COLOR_TEAM = { 50, 255, 50, 255 };

struct ScreenPoint
{
	int x, y;
};

using HFont = unsigned long;
namespace utils
{

	bool InitializeDrawing();

	void DrawText(int x, int y, HFont font, Color color, const char* text, ...);

	// Accepts 3d position coordinates instead of screen coordinates
	void DrawCube(Vector min, Vector max, Color color);

	void DrawLine(int x, int y, int x2, int y2, Color color);

	void DrawRectangle(int x, int y, int w, int h, Color color);
	void DrawFilledRectangle(int x, int y, int w, int h, Color color);

	bool ToScreenCoordinates(Vector world_pos, int& out_x, int& out_y);

	int GetTextWidth(HFont font, const char* text);

}