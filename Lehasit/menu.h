#pragma once

#include <vector>
#include <memory>

#include "sdk/buttons.h"

using HFont = unsigned long;

class IMenuItem
{
protected:
	const char* title;
	void* value;

public:
	IMenuItem(const char* title, void* value)
		:title(title), value(value)
	{

	}

	virtual void clicked() = 0;
	virtual void rightClicked() = 0;
	virtual std::string getStatusText() = 0;

	const char* getTitle()
	{
		return title;
	}

	const int getHeight()
	{
		return 14;
	}
};

class CheatMenu
{
	std::vector<std::unique_ptr<IMenuItem>> m_items;
	int m_width, m_height;
	bool m_is_active;
	int m_selection;
	HFont m_font;

public:
	CheatMenu();
	// ~CheatMenu() -- No need for it since usage of unique_ptr guarantees destruction in default destructor

	void setup();

	void draw(int x, int y);

	// Return: true if key was effective
	bool handleInput(ButtonCode_t keynum);
	bool isActive();
};

extern CheatMenu g_menu;