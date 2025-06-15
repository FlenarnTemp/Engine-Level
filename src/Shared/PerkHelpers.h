#pragma once

#include "Menus/LevelUpMenu.h"

namespace RE
{
	namespace Cascadia
	{
		namespace PerkHelpers
		{
			//	The following was used from Neanka's PRKF
			//	Source: https://github.com/Neanka/f4se/tree/master/f4se/PRKF
			struct PerkData
			{
				bool isEligible;
				bool isAllowable; // For 'GetIsSex' checks.
				bool isHighLevel;
				std::uint32_t filterFlag;
				std::uint32_t requiredLevel;
				std::string requirementsString;
				std::string description;
				std::string SWFPath;
				bool isTagged;
			};

			struct AvailablePerk
			{
				BGSPerk* perk;
				std::uint32_t level;
				std::uint32_t numRanks;
			};

			AvailablePerk GetAvailablePerk(BGSPerk* a_perk);
			PerkData GetPerkRequirements(BGSPerk* a_perk);
		}
	}
}