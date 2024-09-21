#pragma once

#include "Shared/SharedFunctions.h"

namespace RE
{
	namespace Cascadia
	{
		void ModWeaponCondition();
		void DefineItemDegradationFormsFromGame();

		std::map<TESAmmo*, float> ammoDegradationMap;
	}
}
