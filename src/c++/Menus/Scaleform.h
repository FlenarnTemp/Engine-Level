#pragma once
#include <vector>
#include "Systems/Dialogue.h"

bool RegisterScaleform(RE::Scaleform::GFx::Movie* movie, RE::Scaleform::GFx::Value* f4se_root);

std::pair<float, float> GetSubtitlePosition();
bool SetSubtitlePosition(float x, float y);
