#pragma once

#include "Patches/LoadEditorIDs.h"
#include "Patches/OnStoryCraftItem.h"
#include "Patches/TESObjectREFR.h"

namespace RE
{
	namespace Cascadia
	{
		namespace Patches
		{
			void Install()
			{
				LoadEditorIDs::Install();
				OnStoryCraftItem::Install();
				TESObjectREFR_ConsoleName::Install();
			}
		}
	}
}
