#pragma once

#include "Patches/LoadEditorIDs.h"

namespace Patches
{
	void Install()
	{
		LoadEditorIDs::Install();
	}
}
