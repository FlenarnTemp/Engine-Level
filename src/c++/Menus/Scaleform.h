#pragma once
#include <vector>
#include "Systems/Dialogue.h"

bool RegisterScaleform(RE::Scaleform::GFx::Movie* movie, RE::Scaleform::GFx::Value* f4se_root);

std::pair<double, double> GetSubtitlePosition();
bool SetSubtitlePosition(double x, double y);
