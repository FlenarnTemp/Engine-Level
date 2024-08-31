#include "Shared/SharedFunctions.h"
#include "Systems/Skills.h"

namespace RE
{
	namespace Cascadia
	{
		namespace Skills
		{
			struct CascadiaAV_Struct
			{
				// Skill Values
				ActorValueInfo* Barter;
				ActorValueInfo* EnergyWeapons;
				ActorValueInfo* Explosives;
				ActorValueInfo* Lockpick;
				ActorValueInfo* Medicine;
				ActorValueInfo* MeleeWeapons;
				ActorValueInfo* Repair;
				ActorValueInfo* Science;
				ActorValueInfo* Guns;
				ActorValueInfo* Sneak;
				ActorValueInfo* Speech;
				ActorValueInfo* Unarmed;
				ActorValueInfo* Survival;
				ActorValueInfo* WeaponCNDResist;
			};
			CascadiaAV_Struct CascadiaActorValues;

			struct VanillaAV_Struct
			{
				// Vanilla SPECIAL Values
				ActorValueInfo* Strength;
				ActorValueInfo* Perception;
				ActorValueInfo* Endurance;
				ActorValueInfo* Charisma;
				ActorValueInfo* Intelligence;
				ActorValueInfo* Agility;
				ActorValueInfo* Luck;
			};
			VanillaAV_Struct VanillaActorValues;

			struct CascadiaPerks_Struct
			{
				// Weapon Type Perks
				BGSPerk* WeaponTypeEnergyWeaponsPerk;
				BGSPerk* WeaponTypeExplosivesPerk;
				BGSPerk* WeaponTypeMeleeWeaponsPerk;
				BGSPerk* WeaponTypeGunsPerk;
				BGSPerk* WeaponTypeUnarmedPerk;

				// Handler Perks
				BGSPerk* WeaponConditionHandlerPerk;
			};
			CascadiaPerks_Struct CascadiaPerks;
			
			struct CascadiaGlobals_Struct
			{
				// Globals
				TESGlobal* TutorialWPNCND;
			};
			CascadiaGlobals_Struct CascadiaGlobals;

			/*
				Fallout New Vegas Calculations:

				Barter				(CHA * 2) + 2 + (LCK / 2)

				Energy Weapons		(PER * 2) + 2 + (LCK / 2)

				Explosives			(PER * 2) + 2 + (LCK / 2)

				Guns				(AGI * 2) + 2 + (LCK / 2)

				Lockpick 			(PER * 2) + 2 + (LCK / 2)

				Medicine 			(INT * 2) + 2 + (LCK / 2)

				Melee Weapons 		(STR * 2) + 2 + (LCK / 2)

				Repair				(INT * 2) + 2 + (LCK / 2)

				Science				(INT * 2) + 2 + (LCK / 2)

				Sneak				(AGI * 2) + 2 + (LCK / 2)

				Speech				(CHA * 2) + 2 + (LCK / 2)

				Survival			(END * 2) + 2 + (LCK / 2)

				Unarmed				(END * 2) + 2 + (LCK / 2)
			*/

			//	Most functions in this namespace are based on shad0wshayd3's work on Project Massachusetts & HcG x Grills rework for the Capital Wasteland project.
			//	Link: https://github.com/shad0wshayd3/F4SE-dev/tree/master/f4se/f4se_plugins/ProjectMassachusetts
			//	Rewrite Link: https://github.com/shad0wshayd3/F4SE-dev/tree/pm-rewrite

			std::unordered_multimap<const ActorValueInfo*, ActorValueInfo*> skillsLinkMap;
			std::unordered_multimap<std::string, ActorValueInfo*> strSkillMap;

			AVVector CascadiaSkillsList;

			void PopulateSkillEntry(Scaleform::GFx::Value* destination, Scaleform::GFx::ASMovieRootBase* asMovieRoot, ActorValueInfo* skill, std::uint32_t filter, std::vector<std::string> stringValue)
			{
				Scaleform::GFx::Value arrayArguments;
				asMovieRoot->CreateObject(&arrayArguments);
				float value = Cascadia::GetPlayerCharacter()->GetPermanentActorValue(*skill);
				float buffedValue = Cascadia::GetPlayerCharacter()->GetActorValue(*skill);
				GFxUtilities::RegisterString(&arrayArguments, asMovieRoot, "text", skill->fullName.c_str());
				GFxUtilities::RegisterString(&arrayArguments, asMovieRoot, "editorid", skill->GetFormEditorID());
				GFxUtilities::RegisterString(&arrayArguments, asMovieRoot, "description", "TEST");
				if (stringValue.size() > (int)value)
				{
					GFxUtilities::RegisterString(&arrayArguments, asMovieRoot, "stringValue", stringValue[(int)value].c_str());
				}
				else
				{
					GFxUtilities::RegisterString(&arrayArguments, asMovieRoot, "stringValue", "");
				}

				GFxUtilities::RegisterInt(&arrayArguments, "formid", skill->formID);
				GFxUtilities::RegisterNumber(&arrayArguments, "value", buffedValue);
				GFxUtilities::RegisterNumber(&arrayArguments, "maxVal", 0.0);
				GFxUtilities::RegisterNumber(&arrayArguments, "baseValue", value);
				GFxUtilities::RegisterNumber(&arrayArguments, "modified", buffedValue - value);
				GFxUtilities::RegisterNumber(&arrayArguments, "buffedValue", buffedValue);
				GFxUtilities::RegisterInt(&arrayArguments, "filterFlag", filter);

				destination->PushBack(&arrayArguments);
			}

			void PopulateSkillEntries(Scaleform::GFx::ASMovieRootBase* asMovieRoot)
			{
				Scaleform::GFx::Value arraySkills[7];
					asMovieRoot->CreateString(&arraySkills[0], "skills");
					asMovieRoot->CreateString(&arraySkills[1], "$F4CW_SKILLS");

					arraySkills[2] = 0;
					arraySkills[3] = 0; // Pipboy Page - Skills

					asMovieRoot->CreateObject(&arraySkills[4]);
					Scaleform::GFx::Value skillsArray;
					asMovieRoot->CreateArray(&skillsArray);
					std::vector<std::string> stringArray;

					// All skills injected here.
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.Barter, 1, stringArray);
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.EnergyWeapons, 1, stringArray);
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.Explosives, 1, stringArray);
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.Guns, 1, stringArray);
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.Lockpick, 1, stringArray);
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.Medicine, 1, stringArray);
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.MeleeWeapons, 1, stringArray);
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.Repair, 1, stringArray);
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.Science, 1, stringArray);
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.Sneak, 1, stringArray);
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.Speech, 1, stringArray);
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.Survival, 1, stringArray);
					PopulateSkillEntry(&skillsArray, asMovieRoot, Skills::CascadiaActorValues.Unarmed, 1, stringArray);

					arraySkills[4].SetMember("arrayList", &skillsArray);
					arraySkills[5] = 0;
					arraySkills[6] = 0;

					asMovieRoot->Invoke("root.casPipboy_loader.content.registerTab", nullptr, arraySkills, 7);
			}

			// Returns ActorValueInfo based on Skill Name.
			ActorValueInfo* GetSkillByName(std::string mySkill)
			{
				auto result = strSkillMap.find(mySkill);
				if (result != strSkillMap.end())
				{
					return result->second;
				}
				return nullptr;
			}

			//	Returns Actor Value of given Actor based on Skill Name
			float GetSkillValueByName(Actor* myActor, std::string mySkill)
			{
				ActorValueInfo* myAV = GetSkillByName(mySkill);

				if (myAV)
				{
					return myActor->GetActorValue(*myAV);
				}
				else
				{
					return -1;
				}
			}

			float GetBaseSkillValueByName(Actor* myActor, std::string mySkill)
			{
				ActorValueInfo* myAV = GetSkillByName(mySkill);

				if (myAV)
				{
					return myActor->GetBaseActorValue(*myAV);
				}
				else
				{
					return -1;
				}
			}

			// Gets given Actor Values Dependant Actor Value (if none, returns nullptr)
			ActorValueInfo* GetDependantAV(const ActorValueInfo* myAV)
			{
				auto result = skillsLinkMap.find(myAV);
				if (result != skillsLinkMap.end())
				{
					return result->second;
				}
				return nullptr;
			}

			// Add dependent AV to given AV
			void AddDependentAV(ActorValueInfo* myAV, ActorValueInfo* dependentAV)
			{
				if (dependentAV->numDependentActorValues < sizeof(dependentAV->dependentActorValues) / 8)
				{
					dependentAV->dependentActorValues[dependentAV->numDependentActorValues++] = myAV;
				}
			}

			void RegisterDerivedAV(ActorValueInfo* myAV, std::function<RE::ActorValueInfo::DerivationFunction_t> CalcFunction)
			{
				myAV->derivationFunction = CalcFunction;
			}

			void RegisterLinkedAV(ActorValueInfo* myAV, std::function<RE::ActorValueInfo::DerivationFunction_t> CalcFunction, ActorValueInfo* av1, ActorValueInfo* av2)
			{
				RegisterDerivedAV(myAV, CalcFunction);
				AddDependentAV(myAV, av1);
				AddDependentAV(myAV, av2);
				skillsLinkMap.emplace(myAV, av1);
			}

			// Returns calculation of the offset of the
			float CalculateSkillOffset(const ActorValueOwner* a_actor, const ActorValueInfo& a_info)
			{
				if (!a_actor || !&a_info)
				{
					return 0.0;
				}
				else
				{
					// (Dependent x 2) + 2 + (Luck / 2)
					return ((a_actor->GetActorValue(*GetDependantAV(&a_info)) * 2) + 2 + ceilf(a_actor->GetActorValue(*VanillaActorValues.Luck) / 2));
				}
			}

			void RegisterForSkillLink()
			{
				INFO("Skills: Linking Skills from FalloutCascadia.esm");

				skillsLinkMap.clear();
				strSkillMap.clear();

				// Link Skills to their SPECIAL skills.
				RegisterLinkedAV(CascadiaActorValues.Barter, CalculateSkillOffset, VanillaActorValues.Charisma, VanillaActorValues.Luck);
				RegisterLinkedAV(CascadiaActorValues.EnergyWeapons, CalculateSkillOffset, VanillaActorValues.Perception, VanillaActorValues.Luck);
				RegisterLinkedAV(CascadiaActorValues.Explosives, CalculateSkillOffset, VanillaActorValues.Perception, VanillaActorValues.Luck);
				RegisterLinkedAV(CascadiaActorValues.Guns, CalculateSkillOffset, VanillaActorValues.Agility, VanillaActorValues.Luck);
				RegisterLinkedAV(CascadiaActorValues.Lockpick, CalculateSkillOffset, VanillaActorValues.Charisma, VanillaActorValues.Luck);
				RegisterLinkedAV(CascadiaActorValues.Medicine, CalculateSkillOffset, VanillaActorValues.Intelligence, VanillaActorValues.Luck);
				RegisterLinkedAV(CascadiaActorValues.MeleeWeapons, CalculateSkillOffset, VanillaActorValues.Strength, VanillaActorValues.Luck);
				RegisterLinkedAV(CascadiaActorValues.Repair, CalculateSkillOffset, VanillaActorValues.Intelligence, VanillaActorValues.Luck);
				RegisterLinkedAV(CascadiaActorValues.Science, CalculateSkillOffset, VanillaActorValues.Intelligence, VanillaActorValues.Luck);
				RegisterLinkedAV(CascadiaActorValues.Sneak, CalculateSkillOffset, VanillaActorValues.Agility, VanillaActorValues.Luck);
				RegisterLinkedAV(CascadiaActorValues.Speech, CalculateSkillOffset, VanillaActorValues.Charisma, VanillaActorValues.Luck);
				RegisterLinkedAV(CascadiaActorValues.Survival, CalculateSkillOffset, VanillaActorValues.Endurance, VanillaActorValues.Luck);
				RegisterLinkedAV(CascadiaActorValues.Unarmed, CalculateSkillOffset, VanillaActorValues.Endurance, VanillaActorValues.Luck);

				// Add Skills to array with String so we can find them based on name.
				strSkillMap.emplace("Barter", CascadiaActorValues.Barter);
				strSkillMap.emplace("EnergyWeapons", CascadiaActorValues.EnergyWeapons);
				strSkillMap.emplace("Explosives", CascadiaActorValues.Explosives);
				strSkillMap.emplace("Guns", CascadiaActorValues.Guns);
				strSkillMap.emplace("Lockpick", CascadiaActorValues.Lockpick);
				strSkillMap.emplace("Medicine", CascadiaActorValues.Medicine);
				strSkillMap.emplace("Repair", CascadiaActorValues.Repair);
				strSkillMap.emplace("Science", CascadiaActorValues.Science);
				strSkillMap.emplace("Sneak", CascadiaActorValues.Sneak);
				strSkillMap.emplace("Speech", CascadiaActorValues.Speech);
				strSkillMap.emplace("Survival", CascadiaActorValues.Survival);
				strSkillMap.emplace("Unarmed", CascadiaActorValues.Unarmed);
			}

			float GetAVValue(Actor* myActor, ActorValueInfo* myAV)
			{
				if (myActor)
				{
					return myActor->GetActorValue(*myAV);
				}

				return NULL;
			}

			float GetBaseAVValue(Actor* myActor, ActorValueInfo* myAV)
			{
				if (myActor)
				{
					return myActor->GetBaseActorValue(*myAV);
				}

				return NULL;
			}

			void ModBaseAVValue(Actor* myActor, ActorValueInfo* myAV, float fModAmount)
			{
				myActor->ModBaseActorValue(*myAV, fModAmount);
			}

			void SetBaseAVValue(Actor* myActor, ActorValueInfo* myAV, float fSetAmount)
			{
				myActor->SetBaseActorValue(*myAV, fSetAmount);
			}

			float GetPlayerAVValue(ActorValueInfo* myAV)
			{
				return GetAVValue(GetPlayerCharacter(), myAV);
			}

			float GetPlayerBaseAVValue(ActorValueInfo* myAV)
			{
				return GetBaseAVValue(GetPlayerCharacter(), myAV);
			}

			void ModPlayerBaseAVValue(ActorValueInfo* myAV, float fModAmount)
			{
				ModBaseAVValue(GetPlayerCharacter(), myAV, fModAmount);
			}

			void SetPlayerBaseAVValue(ActorValueInfo* myAV, float fSetAmount)
			{
				SetBaseAVValue(GetPlayerCharacter(), myAV, fSetAmount);
			}

			bool DefineSkillsFormsFromGame()
			{
				// Cascadia Actor Values
				CascadiaActorValues.Barter = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35ED, "FalloutCascadia.esm");
				CascadiaActorValues.EnergyWeapons = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35EE, "FalloutCascadia.esm");
				CascadiaActorValues.Explosives = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35EF, "FalloutCascadia.esm");
				CascadiaActorValues.Guns = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35F0, "FalloutCascadia.esm");
				CascadiaActorValues.Lockpick = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35F1, "FalloutCascadia.esm");
				CascadiaActorValues.Medicine = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35F2, "FalloutCascadia.esm");
				CascadiaActorValues.MeleeWeapons = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35F3, "FalloutCascadia.esm");
				CascadiaActorValues.Repair = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35F4, "FalloutCascadia.esm");
				CascadiaActorValues.Science = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35F5, "FalloutCascadia.esm");
				CascadiaActorValues.Sneak = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35F6, "FalloutCascadia.esm");
				CascadiaActorValues.Speech = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35F7, "FalloutCascadia.esm");
				CascadiaActorValues.Survival = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35F8, "FalloutCascadia.esm");
				CascadiaActorValues.Unarmed = TESDataHandler::GetSingleton()->LookupForm<ActorValueInfo>(0x0F35F9, "FalloutCascadia.esm");

				// Cascadia Perks
				CascadiaPerks.WeaponTypeEnergyWeaponsPerk = TESDataHandler::GetSingleton()->LookupForm<BGSPerk>(0x1F9DFE, "FalloutCascadia.esm");
				CascadiaPerks.WeaponTypeExplosivesPerk = TESDataHandler::GetSingleton()->LookupForm<BGSPerk>(0x1F9DFF, "FalloutCascadia.esm");
				CascadiaPerks.WeaponTypeMeleeWeaponsPerk = TESDataHandler::GetSingleton()->LookupForm<BGSPerk>(0x1F9E00, "FalloutCascadia.esm");
				CascadiaPerks.WeaponTypeGunsPerk = TESDataHandler::GetSingleton()->LookupForm<BGSPerk>(0x1F9E01, "FalloutCascadia.esm");
				CascadiaPerks.WeaponTypeUnarmedPerk = TESDataHandler::GetSingleton()->LookupForm<BGSPerk>(0x1F9E02, "FalloutCascadia.esm");
				CascadiaPerks.WeaponConditionHandlerPerk = TESDataHandler::GetSingleton()->LookupForm<BGSPerk>(0x1F9E04, "FalloutCascadia.esm");

				// Cascadia Skills List
				CascadiaSkillsList.emplace_back(CascadiaActorValues.Barter);
				CascadiaSkillsList.emplace_back(CascadiaActorValues.EnergyWeapons);
				CascadiaSkillsList.emplace_back(CascadiaActorValues.Explosives);
				CascadiaSkillsList.emplace_back(CascadiaActorValues.Guns);
				CascadiaSkillsList.emplace_back(CascadiaActorValues.Lockpick);
				CascadiaSkillsList.emplace_back(CascadiaActorValues.Medicine);
				CascadiaSkillsList.emplace_back(CascadiaActorValues.MeleeWeapons);
				CascadiaSkillsList.emplace_back(CascadiaActorValues.Repair);
				CascadiaSkillsList.emplace_back(CascadiaActorValues.Science);
				CascadiaSkillsList.emplace_back(CascadiaActorValues.Sneak);
				CascadiaSkillsList.emplace_back(CascadiaActorValues.Speech);
				CascadiaSkillsList.emplace_back(CascadiaActorValues.Survival);
				CascadiaSkillsList.emplace_back(CascadiaActorValues.Unarmed);

				// Vanilla SPECIAL List
				ActorValue* avSingleton = ActorValue::GetSingleton();

				VanillaActorValues.Strength = avSingleton->strength;
				VanillaActorValues.Perception = avSingleton->perception;
				VanillaActorValues.Endurance = avSingleton->endurance;
				VanillaActorValues.Charisma = avSingleton->charisma;
				VanillaActorValues.Intelligence = avSingleton->intelligence;
				VanillaActorValues.Agility = avSingleton->agility;
				VanillaActorValues.Luck = avSingleton->luck;

				return true;
			}
		}
	}
}
