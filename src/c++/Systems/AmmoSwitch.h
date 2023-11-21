#pragma once

namespace AmmoSwitch
{
	RE::TESAmmo* GetCoreAmmo();
	RE::TESAmmo* GetNextAvailableAmmo();

	void ResetToCoreAmmo();
}
