#pragma once

#include <vector>

class CUserCmd;
class CBaseEntity;
class CTFPlayer;

std::vector<CBaseEntity*> GetDeflectableProjectiles(CTFPlayer& localPlayer);

namespace airblast
{
	bool Run(CUserCmd& cmd, std::vector<CBaseEntity*> deflectableProjectiles);
}

namespace orbit
{
	void Run(CUserCmd& cmd, std::vector<CBaseEntity*> deflectableProjectiles);
}

namespace glow
{
	void Run();
}

namespace prediction
{
	void Apply(CUserCmd& cmd);
	void Restore();
}