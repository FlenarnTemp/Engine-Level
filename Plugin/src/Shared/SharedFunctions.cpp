#include "SharedFunctions.h"
#include "Systems/ItemDegradation.h"
using namespace RE::BSScript;

namespace RE
{
	namespace Cascadia
	{
		// Returns true is FormID is base game OR dynamically placed ("FF" index)
		// Assumes all DLC is installed (Excluding DLCUltraHighResolution.esm as it is empty and "safe" to ignore).
		// TODO - Better handling for the "FF" index, list span of "official" .esl's?
		bool IsFormIDStringBaseGame(std::string formIDString)
		{
			std::string formIndex = formIDString.substr(0, 2);
			if (
				formIndex == "00" || // Fallout4.esm
				formIndex == "01" || // DLCRobot.esm
				formIndex == "02" || // DLCworkshop01.esm
				formIndex == "03" || // DLCCoast.esm
				formIndex == "04" || // DLCworkshop02.esm
				formIndex == "05" || // DLCworkshop03.esm
				formIndex == "06" || // DLCNukaWorld.esm
				formIndex == "ff"    // Dynamic content, FF - Creation Club primarily(?)
				)
			{
				return true;
			}
			return false;
		}

		// Returns FormID as hex string
		std::string GetFormIDAsString(std::uint32_t formID)
		{
			std::string result{};
			std::stringstream formIDStream;
			formIDStream << std::hex << formID;
			result = formIDStream.str();
			if (result.length() < 8)
			{
				std::string temp;
				for (std::uint32_t i = 0; i < (8 - result.length()); i++)
				{
					temp.append("0");
				}
				temp.append("0");
				result = temp;
			}
			return result;
		}

		// 0 = male, 1 = female
		std::uint32_t GetActorSex(RE::Actor* a_actor)
		{
			std::uint32_t gender = 0;
			TESNPC* a_actorBase = a_actor->GetNPC();
			if (a_actorBase)
			{
				gender = a_actorBase->GetSex();
			}
			return gender;
		}

		PlayerCharacter* GetPlayerCharacter()
		{
			PlayerCharacter* PlayerCharacter = PlayerCharacter::GetSingleton();
			return PlayerCharacter;
		}

		bool IsPlayerGodMode()
		{
			return GetPlayerCharacter()->IsGodMode();
		}

		std::int16_t GetPlayerLevel()
		{
			return GetPlayerCharacter()->GetLevel();
		}

		std::uint32_t GetPlayerSex()
		{
			return GetActorSex(GetPlayerCharacter());
		}

		// Takes a Float and converts it to a Precise String.
		// eg. FloatToPreciseString(3.141592, 2) would return "3.14"
		std::string FloatToPreciseString(float value, std::uint32_t precision)
		{
			std::stringstream stream;
			stream << std::fixed << std::setprecision(precision) << value;
			return stream.str();
		}

		std::uint32_t RNG(std::uint32_t min, std::uint32_t max)
		{
			std::random_device dev;
			std::mt19937 rng(dev());
			std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

			return dist(rng);
		}

		float RNG(float min, float max)
		{
			std::random_device dev;
			std::mt19937 rng(dev());
			std::uniform_real_distribution<float> dist(min, max);

			return dist(rng);
		}

		bool WeaponHasKeyword(TESObjectWEAP* weapon, BGSKeyword* keyword)
		{
			if (weapon)
			{
				for (std::uint32_t i = 0; i < weapon->numKeywords; i++)
				{
					if (weapon->keywords[i] == keyword)
					{
						return true;
					}
				}
			}
			return false;
		}

		bool ArmorHasKeyword(TESObjectARMO* armor, BGSKeyword* keyword)
		{
			if (armor)
			{
				for (std::uint32_t i = 0; i < armor->numKeywords; i++)
				{
					if (armor->keywords[i] == keyword)
					{
						return true;
					}
				}
			}
			return false;
		}

		bool ReferenceHasKeyword(TESObjectREFR* ref, BGSKeyword* keyword)
		{
			if (ref)
			{
				TBO_InstanceData* myInstanceData = nullptr;
				BSExtraData* myExtraData = ref->extraList->GetByType(kInstanceData);

				if (myExtraData)
				{
					ExtraInstanceData* myExtraInstanceData = (ExtraInstanceData*)myExtraData;
					if (myExtraInstanceData)
					{
						myInstanceData = myExtraInstanceData->data.get();
					}
				}

				if (myInstanceData)
				{
					return ref->HasKeyword(keyword, myInstanceData);
				}
			}
			return false;
		}

		bool ActorHasKeyword(Actor* actor, BGSKeyword* keyword)
		{
			if (actor)
			{
				TBO_InstanceData* myInstanceData = nullptr;
				BSExtraData* myExtraData = actor->extraList->GetByType(kInstanceData);

				if (myExtraData)
				{
					ExtraInstanceData* myExtraInstanceData = (ExtraInstanceData*)myExtraData;
					if (myExtraInstanceData)
					{
						myInstanceData = myExtraInstanceData->data.get();
					}
				}

				if (myInstanceData)
				{
					return actor->HasKeywordHelper(keyword, myInstanceData);
				}
			}
			return false;
		}

		bool NPCHasKeyword(TESNPC* npc, BGSKeyword* keyword)
		{
			if (npc)
			{
				for (std::uint32_t i = 0; i < npc->numKeywords; i++)
				{
					if (npc->keywords[i] == keyword)
					{
						return true;
					}
				}
			}

			// If still false, check if race has keyword
			// Doesn't seem to be bound on the NPC check if it's only on the race? Gotta love inheritance.
			TESRace* npcRace = npc->originalRace;

			if (npcRace)
			{
				for (std::uint32_t i = 0; i < npcRace->numKeywords; i++)
				{
					if (npcRace->keywords[i] == keyword)
					{
						return true;
					}
				}
			}

			return false;
		}

		const char* GetItemDisplayName(ExtraDataList myExtraData, TESForm baseForm)
		{
			BSExtraData* extraData = myExtraData.GetByType(EXTRA_DATA_TYPE::kTextDisplayData);
			ExtraTextDisplayData* displayText = (ExtraTextDisplayData*)extraData;

			if (displayText)
			{
				return "";
			}
			else
			{
				return "";
			}
		}

		bool IsInMenuMode()
		{
			UI* g_ui = UI::GetSingleton();
			return(
				(g_ui->menuMode >= 1)
				|| g_ui->GetMenuOpen(BSFixedString("CookingMenu"))
				|| g_ui->GetMenuOpen(BSFixedString("FaderMenu"))
				|| g_ui->GetMenuOpen(BSFixedString("FavoritesMenu"))
				|| g_ui->GetMenuOpen(BSFixedString("PowerArmorModMenu"))
				|| g_ui->GetMenuOpen(BSFixedString("RobotModMenu"))
				|| g_ui->GetMenuOpen(BSFixedString("VATSMenu"))
				|| g_ui->GetMenuOpen(BSFixedString("WorkshopMenu"))
				|| g_ui->GetMenuOpen(BSFixedString("DialogueMenu"))
				);
		}

		// Get Inxed in FormList
		std::uint32_t PositionInFormList(TESForm* form, BGSListForm* list)
		{
			if (list->arrayOfForms.size() != 0)
			{
				for (std::uint32_t i = 0; i <= list->arrayOfForms.size() - 1; i++)
				{
					if (list->arrayOfForms[i] == form)
					{
						return i;
					}
				}
			}
			return 0;
		}

		// Gets and return next form player has a count higher than 0 of in list, from current index to start of array
		std::uint32_t GetNextAvailableFormInInventoryFromList(std::uint32_t startingIndex, BGSListForm* list)
		{
			PlayerCharacter* playerCharacter = GetPlayerCharacter();
			if (list->arrayOfForms.size() != 0)
			{
				for (std::uint32_t a = startingIndex + 1; a <= list->arrayOfForms.size() - 1; a++)
				{
					TESBoundObject* a_object = (RE::TESBoundObject*)(list->arrayOfForms[a]);
					if (playerCharacter->GetInventoryObjectCount(a_object) != 0)
					{
						return a;
					}
				}

				for (std::uint32_t b = 0; b <= startingIndex; b++)
				{
					TESBoundObject* a_object = (TESBoundObject*)(list->arrayOfForms[b]);
					if (playerCharacter->GetInventoryObjectCount(a_object) != 0)
					{
						return b;
					}
				}
			}

			return (std::uint32_t)-1;
		}

		//	Returns True if Form is in FormList
		bool IsFormInList(TESForm* form, BGSListForm* list)
		{
			if (list->arrayOfForms.size() != 0)
			{
				for (std::uint32_t i = 0; i <= list->arrayOfForms.size() - 1; i++)
				{
					if (list->arrayOfForms[i] == form)
					{
						return true;
					}
				}
			}
			return false;
		}

		/*bool IsMeleeWeapon(WeaponConditionData myConditionData)
		{
			bool result = false;

			switch (myConditionData.instance->type.get()) {
			case RE::WEAPON_TYPE::kHandToHand:
			case RE::WEAPON_TYPE::kOneHandAxe:
			case RE::WEAPON_TYPE::kOneHandDagger:
			case RE::WEAPON_TYPE::kOneHandMace:
			case RE::WEAPON_TYPE::kOneHandSword:
			case RE::WEAPON_TYPE::kStaff:
			case RE::WEAPON_TYPE::kTwoHandAxe:
			case RE::WEAPON_TYPE::kTwoHandSword:
				result = true;
				break;

			default:
				result = false;
				break;
			}

			return result;
		}*/

		Setting* GetGMST(const char* a_name)
		{
			return GameSettingCollection::GetSingleton()->GetSetting(a_name);
		}

		//	Converts a UInt8 percentage to float (eg. 38% becomes 0.38)
		float ConvertPercentageToFloat(std::uint8_t percentage)
		{
			return (percentage / static_cast<float>(100));
		}

		bool HasVMScript(TESForm* form, const char* scriptName)
		{
			// TODO - the rest of this function needs more declaration of the IVM.

			return false;
		}
	}
}
