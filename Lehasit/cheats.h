#pragma once

#include <vector>

class CUserCmd;
class CBaseEntity;
class CTFPlayer;

std::vector<CBaseEntity*> GetDeflectableProjectiles(CTFPlayer* pLocalPlayer);

namespace airblast
{
	bool Run(CUserCmd* pCmd, std::vector<CBaseEntity*> deflectableProjectiles);
}

namespace orbit
{
	void Run(CUserCmd* pCmd, std::vector<CBaseEntity*> deflectableProjectiles);
}

namespace glow
{
	void Run();
}

namespace prediction
{
	void Apply(CUserCmd* pCmd);
	void Restore();
}