#include "Shared/SharedFunctions.h"
#include "Serialization.h"

namespace Serialization
{
	/* Level Up Menu Related */
	std::int32_t playerSkillPoints = 0;
	std::set<std::uint32_t> taggedSkills;
	bool playerReadyToLevelUp = false;

	std::int32_t GetSkillPoints()
	{
		return playerSkillPoints;
	}

	void SetSkillPoints(std::int32_t setValue)
	{
		playerSkillPoints = setValue;
	}
}
