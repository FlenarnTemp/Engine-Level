#pragma once

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
				std::uint32_t filterFlags;
				std::uint32_t requiredLevel;
				std::string requirementsString;
				std::string description;
				std::string SWFPath;
				bool isFlagged;
			};

			struct AvailablePerk
			{
				BGSPerk* perk;
				std::uint32_t level;
				std::uint32_t numRanks;
			};
		}
	}
}