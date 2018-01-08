#pragma once

struct config_t
{
	bool airblast_enable = false;
	bool airblast_tfdb_lagfix = true;
	int airblast_spike_type = 0;
	const char* airblast_spikeTypeText[6] = { "none", "manual", "up", "down", "left", "right" };

	int glow_type = 0;
	const char* glow_typeText[3] = { "off", "enemy-only", "all" };

	bool thirdperson_enable = false;

	void load();
};

extern config_t g_config;