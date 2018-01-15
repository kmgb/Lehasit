#pragma once

#include <vector>

class CUserCmd;
class CBaseEntity;
class CTFPlayer;

std::vector<CBaseEntity*> GetDeflectableProjectiles(CTFPlayer* pLocalPlayer);

namespace airblast
{
	void Run(CUserCmd* pCmd);
}

namespace orbit
{
	void Run(CUserCmd* pCmd);
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