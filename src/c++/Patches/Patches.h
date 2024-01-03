#pragma once

#include "Patches/LoadEditorIDs.h"
#include "Patches/OnStoryCraftItem.h"

namespace Patches
{
	void Install()
	{
		LoadEditorIDs::Install();
		OnStoryCraftItem::Install();
	}
}
