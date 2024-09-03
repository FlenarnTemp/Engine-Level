#include "SharedFunctions.h"
#include "Systems/ItemDegradation.h"
using namespace RE::BSScript;

namespace RE
{
	namespace Cascadia
	{
		namespace GFxUtilities
		{
			void RegisterArray(Scaleform::GFx::Value* destination, Scaleform::GFx::ASMovieRootBase* asMovieRoot, const char* name, Scaleform::GFx::Value* array)
			{
				asMovieRoot->CreateArray(array);
				destination->SetMember(name, array);
			}

			void RegisterString(Scaleform::GFx::Value* destination, Scaleform::GFx::ASMovieRootBase* asMovieRoot, const char* name, const char* string)
			{
				Scaleform::GFx::Value fxValue;
				asMovieRoot->CreateString(&fxValue, string);
				destination->SetMember(name, &fxValue);
			}

			void RegisterNumber(Scaleform::GFx::Value* destination, const char* name, double value)
			{
				Scaleform::GFx::Value fxValue;
				fxValue = value;
				destination->SetMember(name, &fxValue);
			}

			void RegisterInt(Scaleform::GFx::Value* destination, const char* name, std::uint32_t value)
			{
				Scaleform::GFx::Value fxValue;
				fxValue = value;
				destination->SetMember(name, &fxValue);
			}
		}

		// Returns true is FormID is base game OR dynamically placed ("FF" index)
		// Assumes all DLC is installed (Excluding DLCUltraHighResolution.esm as it is empty and "safe" to ignore).
		// TODO - Better handling for the "FF" index, list span of "official" .esl's?
		bool IsFormIDStringBaseGame(std::string formIDString)
		{
			std::string formIndex = formIDString.substr(0, 2);
			if (
				formIndex == "00" ||  // Fallout4.esm
				formIndex == "01" ||  // DLCRobot.esm
				formIndex == "02" ||  // DLCworkshop01.esm
				formIndex == "03" ||  // DLCCoast.esm
				formIndex == "04" ||  // DLCworkshop02.esm
				formIndex == "05" ||  // DLCworkshop03.esm
				formIndex == "06" ||  // DLCNukaWorld.esm
				formIndex == "ff"     // Dynamic content, FF - Creation Club primarily(?)
				)
			{
				return true;
			}
			return false;
		}

		// Returns FormID as hex string
		std::string GetFormIDAsString(std::uint32_t a_formID)
		{
			std::string result{};
			std::stringstream formIDStream;
			formIDStream << std::hex << a_formID;
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
				gender = std::to_underlying(a_actorBase->GetSex());
			}
			return gender;
		}

		// Takes a Float and converts it to a Precise String.
		// eg. FloatToPreciseString(3.141592, 2) would return "3.14"
		std::string FloatToPreciseString(float a_value, std::uint32_t a_precision)
		{
			std::stringstream stream;
			stream << std::fixed << std::setprecision(a_precision) << a_value;
			return stream.str();
		}

		bool WeaponHasKeyword(TESObjectWEAP* a_weapon, BGSKeyword* a_keyword)
		{
			if (a_weapon)
			{
				for (std::uint32_t i = 0; i < a_weapon->numKeywords; i++)
				{
					if (a_weapon->keywords[i] == a_keyword)
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

		bool IsInMenuMode()
		{
			UI* g_ui = UI::GetSingleton();
			return (
				(g_ui->menuMode >= 1) || g_ui->GetMenuOpen(BSFixedString("CookingMenu")) || g_ui->GetMenuOpen(BSFixedString("FaderMenu")) || g_ui->GetMenuOpen(BSFixedString("FavoritesMenu")) || g_ui->GetMenuOpen(BSFixedString("PowerArmorModMenu")) || g_ui->GetMenuOpen(BSFixedString("RobotModMenu")) || g_ui->GetMenuOpen(BSFixedString("VATSMenu")) || g_ui->GetMenuOpen(BSFixedString("WorkshopMenu")) || g_ui->GetMenuOpen(BSFixedString("DialogueMenu")));
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
			PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();
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

		//	Converts a UInt8 percentage to float (eg. 38% becomes 0.38)
		float ConvertPercentageToFloat(std::uint8_t percentage)
		{
			return (percentage / static_cast<float>(100));
		}

		bool HasVMScript(TESForm* form, const char* scriptName)
		{
			BSTSmartPointer<BSScript::IVirtualMachine> vm = GameVM::GetSingleton()->GetVM();
			const BSScript::IObjectHandlePolicy& handlePolicy = vm->GetObjectHandlePolicy();
			std::size_t handle = handlePolicy.GetHandleForObject(form->formType.underlying(), form);

			BSTSmartPointer<Object> identifier;

			return vm->FindBoundObject(handle, scriptName, 1, identifier, 0);
		}

		std::string trim(const std::string& str) {
			size_t start = str.find_first_not_of(" \t\n\r");
			size_t end = str.find_last_not_of(" \t\n\r");
			return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
		}
	}
}
