#include "stdafx.h"
#include "menu.h"

#include "utils/draw.h"
#include "Interfaces.h"
#include "config.h"

CheatMenu g_menu;

// TODO: Move declarations to the header file
class MenuItemBool : public IMenuItem
{
public:
	MenuItemBool(const char* title, bool* value)
		:IMenuItem(title, value)
	{

	}

	virtual void clicked()
	{
		*(bool*)value = !*(bool*)value;
	}

	virtual void rightClicked()
	{
		*(bool*)value = !*(bool*)value;
	}

	virtual std::string getStatusText()
	{
		return (*(bool*)value) ? "on" : "off";
	}
};

class MenuItemInt : public IMenuItem
{
	int min, max;

public:
	MenuItemInt(const char* title, int* value, int min, int max)
		:IMenuItem(title, value),
		min(min),
		max(max)
	{

	}

	virtual void clicked()
	{
		++*(int*)value;

		if (*(int*)value > max)
			*(int*)value = min;
	}

	virtual void rightClicked()
	{
		--*(int*)value;

		if (*(int*)value < min)
			*(int*)value = max;
	}

	virtual std::string getStatusText()
	{
		std::string string;
		string = std::to_string(*(int*)value);
		return string;
	}
};

template<size_t S>
class MenuItemString : public IMenuItem
{
	int num_strings;
	const char* (&strings)[S];

public:
	MenuItemString(const char* title, int* value, const char* (&strings)[S])
		:IMenuItem(title, value),
		strings(strings),
		num_strings(S)
	{
	}

	virtual void clicked()
	{
		int ivalue = *(int*)value;
		++ivalue;
		if (ivalue > num_strings - 1)
			ivalue = 0;

		*(int*)value = ivalue;
	}

	virtual void rightClicked()
	{
		int ivalue = *(int*)value;
		--ivalue;
		if (ivalue < 0)
			ivalue = num_strings - 1;

		*(int*)value = ivalue;
	}

	virtual std::string getStatusText()
	{
		return strings[*(int*)value];
	}
};

CheatMenu::CheatMenu()
{
	m_width = 300; // arbitrary
	m_is_active = false;
	m_items.push_back(std::unique_ptr<IMenuItem>(new MenuItemBool("Auto airblast", &g_config.airblast_enable)));
	m_items.push_back(std::unique_ptr<IMenuItem>(new MenuItemBool("- TFDB lag fix", &g_config.airblast_tfdb_lagfix)));
	m_items.push_back(std::unique_ptr<IMenuItem>(new MenuItemString<6>("- TFDB spike type", &g_config.airblast_spike_type, g_config.airblast_spikeTypeText)));
	m_items.push_back(std::unique_ptr<IMenuItem>(new MenuItemString<3>("Glow", &g_config.glow_type, g_config.glow_typeText)));
	m_items.push_back(std::unique_ptr<IMenuItem>(new MenuItemBool("Force thirdperson", &g_config.thirdperson_enable)));

	m_height = 14 * m_items.size();
}

void CheatMenu::setup()
{
	m_font = g_interfaces.surface->CreateFont();
	g_interfaces.surface->SetFontGlyphSet(m_font, "Tahoma", 14, 700, 0, 0, 0);
}

void CheatMenu::draw(int x, int y)
{
	if (!m_is_active)
		return;

	utils::DrawFilledRectangle(x, y, m_width, m_height, COLOR_BLACK);
	utils::DrawFilledRectangle(x, y + (14 * m_selection), m_width, 14, Color(100, 100, 100, 255));

	int textY = y;
	int textX = x + 3;
	for (std::unique_ptr<IMenuItem>& item : m_items)
	{
		utils::DrawText(textX, textY, m_font, COLOR_WHITE, item->getTitle());
		std::string statusText = item->getStatusText();
		int status_width = utils::GetTextWidth(m_font, statusText.c_str());
		utils::DrawText(x + m_width - status_width - 3, textY, m_font, COLOR_WHITE, statusText.c_str());
		textY += item->getHeight();
	}
}

bool CheatMenu::handleInput(ButtonCode_t keynum)
{
	if (keynum == KEY_INSERT)
	{
		m_is_active = !m_is_active;
		return true;
	}

	if (!m_is_active)
		return false;

	switch (keynum)
	{
		case MOUSE_LEFT:
			m_items.at(m_selection)->clicked();
			break;

		case MOUSE_RIGHT:
			m_items.at(m_selection)->rightClicked();
			break;

		case MOUSE_WHEEL_DOWN:
			++m_selection;
			if (m_selection > (int)m_items.size() - 1)
				m_selection = 0;
			break;

		case MOUSE_WHEEL_UP:
			--m_selection;
			if (m_selection < 0)
				m_selection = (int)m_items.size() - 1;
			break;

		default:
			return false;
	}

	return true; // Swallow the key
}

bool CheatMenu::isActive()
{
	return m_is_active;
}
