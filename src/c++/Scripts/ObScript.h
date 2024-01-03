#pragma once

#include "ObScript/BetaComment.h"
#include "ObScript/SavePlayerFace.h"

namespace ObScript
{
	void Install()
	{
		ObScript::BetaComment::Install();
		ObScript::SavePlayerFace::Install();
	}
}
