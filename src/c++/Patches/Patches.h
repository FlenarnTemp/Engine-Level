#pragma once

#include "Patches/LoadEditorIDs.h"
#include "Patches/OnStoryCraftItem.h"
#include "Patches/TESObjectREFR.h"

namespace Patches
{
	void Install()
	{
		LoadEditorIDs::Install();
		OnStoryCraftItem::Install();
		TESObjectREFR_ConsoleName::Install();
	}
}
