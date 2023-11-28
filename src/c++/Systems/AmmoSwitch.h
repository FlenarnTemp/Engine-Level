#pragma once

namespace AmmoSwitch
{
	RE::TESAmmo* GetCoreAmmo();
	RE::TESAmmo* GetNextAvailableAmmo();

	void ResetToCoreAmmo();

#ifndef AmmoSwitchValuesStruct
#define AmmoSwitchValuesStruct
	struct AmmoSwitchValues_Struct
	{
		bool isCurrentlySwitching = false;
	};
#endif
	extern AmmoSwitchValues_Struct AmmoSwitchValues;
}
