#pragma once

#include "ObScript/BetaComment.h"
#include "ObScript/GetBaseObject.h"
#include "ObScript/SavePlayerFace.h"

namespace ObScript
{
	void Install()
	{
		ObScript::BetaComment::Install();
		ObScript::GetBaseObject::Install();
		ObScript::SavePlayerFace::Install();
	}
}
