#pragma once

class CUserCmd;

namespace airblast
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