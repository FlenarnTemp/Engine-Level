#pragma once
#include <vector>
#include "Systems/Dialogue.h"

namespace RE
{
	bool RegisterScaleform(Scaleform::GFx::Movie* movie, Scaleform::GFx::Value* f4se_root);

	std::pair<double, double> GetSubtitlePosition();
	bool SetSubtitlePosition(double x, double y);
}


