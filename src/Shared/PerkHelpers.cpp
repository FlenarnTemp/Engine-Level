#include "Shared/PerkHelpers.h"

namespace RE
{
	namespace Cascadia
	{
		namespace PerkHelpers
		{
			AvailablePerk GetAvailablePerk(BGSPerk* a_perk)
			{
				AvailablePerk result;
				
				result.level = 0;
				result.perk = nullptr;
				result.numRanks = a_perk->data.numRanks;

				BGSPerk* temporaryPerk;

				bool gotResult = false;
				bool learned = false;

				PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();

				temporaryPerk = a_perk;

				if (a_perk->data.numRanks > 1 && (a_perk->nextPerk == a_perk || a_perk->nextPerk == nullptr))
				{
					std::uint32_t currentPerkRank = playerCharacter->GetPerkRank(temporaryPerk);
					if (currentPerkRank < a_perk->data.numRanks)
					{
						result.level = currentPerkRank + 1;
						result.perk = a_perk;
						return result;
					}
					else
					{
						return result;
					}
				}
				else
				{
					std::uint32_t counter = 0;
					while (true)
					{
						temporaryPerk = temporaryPerk->nextPerk;
						counter++;
						if (temporaryPerk == a_perk || temporaryPerk == nullptr)
						{
							break;
						}
					}
					if (counter != a_perk->data.numRanks)
					{
						result.numRanks = counter;
					}
				}
				temporaryPerk = a_perk;

				for (std::uint32_t i = 0; i < result.numRanks; i++)
				{
					if (playerCharacter->GetPerkRank(temporaryPerk) == 0)
					{
						if (!gotResult)
						{
							gotResult = true;
							result.perk = temporaryPerk;
							result.level = i + 1;
						}
					}
					else
					{
						if (!learned)
						{
							learned = true;
						}
					}
					temporaryPerk = temporaryPerk->nextPerk;
				}
				return result;
			}
		}
	}
}