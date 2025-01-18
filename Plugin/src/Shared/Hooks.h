#pragma once

#include "Shared/SharedFunctions.h"
#include "Systems/Dialogue.h"
#include "detourXS/detourxs.h"
#include "Systems/AmmoSwitch.h"

namespace RE
{
	namespace Cascadia
	{
		namespace Hooks
		{
			void Install(F4SE::Trampoline& trampoline);
			void RegisterAddItemHook();
			void RegisterSetHealthPercHook();
			void RegisterGetInventoryValueHook();
			void RegisterShowBuildFailureMessage();
			void RegisterGetBuildConfirmQuestion();
			void RegisterQCurrentModChoiceData();
			void RegisterExamineMenuBuildConfirmed();
			void RegisterTESObjectWEAPFire();
			void RegisterCombatFormulasCalcWeaponDamage();
			void RegisterGetEquippedArmorDamageResistance();
			void RegisterIUUIIUtilsAddItemCardInfoEntry();
			void RegisterPipboyInventoryDataBaseAddItemsCardInfoEntry();
			void RegisterIUUIIUtilsPopulateItemCardInfo_Helper();
			void RegisterPipboyInventoryUtilsFillResistTypeInfo();

			void RegisterHooks()
			{
				RegisterAddItemHook();
				RegisterSetHealthPercHook();
				RegisterGetInventoryValueHook();
				RegisterShowBuildFailureMessage();
				RegisterGetBuildConfirmQuestion();
				RegisterQCurrentModChoiceData();
				RegisterExamineMenuBuildConfirmed();
				RegisterTESObjectWEAPFire();
				RegisterCombatFormulasCalcWeaponDamage();
				RegisterGetEquippedArmorDamageResistance();
				RegisterIUUIIUtilsAddItemCardInfoEntry();
				RegisterPipboyInventoryDataBaseAddItemsCardInfoEntry();
				RegisterIUUIIUtilsPopulateItemCardInfo_Helper();
				RegisterPipboyInventoryUtilsFillResistTypeInfo();
			}
		}
	}
}
