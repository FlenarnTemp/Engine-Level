#include "Shared/PerkHelpers.h"

namespace RE
{
	namespace Cascadia
	{
		namespace PerkHelpers
		{
			enum FilterFlags
			{
				kEligible = 1,
				kNotEligible = 2,
				kHighLevel = 4,
				kS = 8,
				kP = 16,
				kE = 32,
				kC = 64,
				kI = 128,
				kA = 256,
				kL = 512,
				kNonSpecial = 1024,
				kOther = 2048
			};

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

			PerkData GetPerkRequirements(BGSPerk* a_perk)
			{
				PerkData result;
				PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();

				bool isAllowable = true;
				bool isHighLevel = a_perk->data.level > playerCharacter->GetLevel();
				bool isEligible = !isHighLevel;
				std::uint32_t filterFlag = 0;
				result.requiredLevel = a_perk->data.level;
				std::string requirementString = "";

				TESCondition perkCondition = a_perk->perkConditions;

				bool lastFlag = false;

				while (perkCondition)
				{

					SCRIPT_OUTPUT scriptOutput = perkCondition.head->data.functionData.function.get();
					float compareValue = perkCondition.head->GetComparisonValue();
					ENUM_COMPARISON_CONDITION compareCondition = static_cast<ENUM_COMPARISON_CONDITION>(static_cast<std::int32_t>(perkCondition.head->data.condition));

					bool eligible = true;
					switch (scriptOutput)
					{
					case SCRIPT_OUTPUT::kScript_GetPermanentValue:
					case SCRIPT_OUTPUT::kScript_GetBaseValue:
					case SCRIPT_OUTPUT::kScript_GetValue:
						ActorValueInfo* temporaryAVI = static_cast<ActorValueInfo*>(perkCondition.head->data.functionData.param[0]);

						switch (compareCondition)
						{
						case ENUM_COMPARISON_CONDITION::kEqual:
							eligible = playerCharacter->GetPermanentActorValue(*temporaryAVI) == compareValue;
							break;
						case ENUM_COMPARISON_CONDITION::kNotEqual:
							eligible = playerCharacter->GetPermanentActorValue(*temporaryAVI) != compareValue;
							break;
						case ENUM_COMPARISON_CONDITION::kGreaterThan:
							eligible = playerCharacter->GetPermanentActorValue(*temporaryAVI) > compareValue;
							break;
						case ENUM_COMPARISON_CONDITION::kGreaterThanEqual:
							eligible = playerCharacter->GetPermanentActorValue(*temporaryAVI) >= compareValue;
							break;
						case ENUM_COMPARISON_CONDITION::kLessThan:
							eligible = playerCharacter->GetPermanentActorValue(*temporaryAVI) < compareValue;
							break;
						case ENUM_COMPARISON_CONDITION::kLessThanEqual:
							eligible = playerCharacter->GetPermanentActorValue(*temporaryAVI) <= compareValue;
							break;
						default:
							break;
						}

						filterFlag |= FilterFlags::kNonSpecial;

						if (!eligible)
						{
							requirementString += "<font color=\'#838383\'>";
						}

						switch (temporaryAVI->formID)
						{
						case 706:
							requirementString += "$CAS_STR";
							break;
						case 707:
							requirementString += "$CAS_PER";
							break;
						case 708:
							requirementString += "$CAS_END";
							break;
						case 709:
							requirementString += "$CAS_CHA";
							break;
						case 710:
							requirementString += "$CAS_INT";
							break;
						case 711:
							requirementString += "$CAS_AGI";
							break;
						case 712:
							requirementString += "$CAS_LCK";
							break;
						default:
							if (temporaryAVI->GetFullName() == "")
							{
								requirementString += temporaryAVI->GetFormEditorID();
							}
							else
							{
								requirementString += temporaryAVI->GetFullName();
							}
							break;
						}

						requirementString += " " + std::to_string((std::uint32_t)compareValue);
						if (!eligible)
						{
							requirementString += "</font>";
						}
						break;
					case SCRIPT_OUTPUT::kScript_GetIsSex:
						isAllowable = perkCondition.IsTrue(playerCharacter, nullptr);
						break;
					case SCRIPT_OUTPUT::kScript_HasPerk:
						break;
					default:
						break;
					}

					lastFlag = (perkCondition.head->data.compareOr == 0);

					TESConditionItem* nextPerkCondition = perkCondition.head->next;
					if (nextPerkCondition)
					{
						if (scriptOutput == SCRIPT_OUTPUT::kScript_GetIsSex)
						{
							requirementString += "";
						}
						else if (lastFlag)
						{
							requirementString += ", ";
						}
						else
						{
							requirementString += " $CAS_or ";
						}
					}
				}

				result.isEligible = isEligible && a_perk->perkConditions.IsTrue(playerCharacter, playerCharacter);
				result.isAllowable = isAllowable;
				result.isHighLevel = isHighLevel;
				if (filterFlag == 0) filterFlag |= FilterFlags::kOther;
				result.filterFlag = filterFlag | (result.isEligible ? 1 : 2);
				result.requirementsString = requirementString;
				result.SWFPath = a_perk->swfFile.c_str();
				result.isTagged = false;

				return result;
			}
		}
	}
}