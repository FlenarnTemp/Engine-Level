#include <fstream>

#include "Shared/SharedFunctions.h"

#include "Serialization.h"
#include <set>

namespace Serialization
{
	/* Level Up Menu Related */
	INT32 playerSkillPoints = 0;
	std::set<UINT32> taggedSkills;
	bool playerReadyToLevelUp = false;

	INT32 GetSkillPoints()
	{
		return playerSkillPoints;
	}

	void SetSkillPoints(INT32 setValue)
	{
		playerSkillPoints = setValue;
	}
}
