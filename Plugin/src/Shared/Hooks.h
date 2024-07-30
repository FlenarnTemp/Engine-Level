#pragma once

#include "Systems/Dialogue.h"
#include "detourXS/detourxs.h"
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
		}
	}
}
