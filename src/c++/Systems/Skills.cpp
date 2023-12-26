#include "Systems/Skills.h"
#include "Shared/SharedFunctions.h"
#include "../../../build/src/c++/CommonLibF4/workaround.h"

VanillaAV_Struct			VanillaActorValues;
CascadiaAV_Struct			CascadiaActorValues;
CascadiaPerks_Struct		CascadiaPerks;
CascadiaGlobals_Struct		CascadiaGlobals;

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

namespace Skills
{
	//	Most functions in this namespace are based on shad0wshayd3's work on Project Massachusetts & HcG x Grills rework for the Capital Wasteland project.
	//	Link: https://github.com/shad0wshayd3/F4SE-dev/tree/master/f4se/f4se_plugins/ProjectMassachusetts
	//	Rewrite Link: https://github.com/shad0wshayd3/F4SE-dev/tree/pm-rewrite

	std::unordered_multimap<RE::ActorValueInfo*, RE::ActorValueInfo*> skillsLinkMap;
	std::unordered_multimap<std::string, RE::ActorValueInfo*> strSkillMap;

	AVVector CascadiaSkillsList;

	// Returns ActorValueInfo based on Skill Name.
	RE::ActorValueInfo* GetSkillByName(std::string mySkill)
	{
		auto result = strSkillMap.find(mySkill);
		if (result != strSkillMap.end())
		{
			return result->second;
		}
		return nullptr;
	}

	//	Returns Actor Value of given Actor based on Skill Name
	float GetSkillValueByName(RE::Actor* myActor, std::string mySkill)
	{
		RE::ActorValueInfo* myAV = GetSkillByName(mySkill);

		if (myAV)
		{
			return myActor->GetActorValue(*myAV);
		}
		else
		{
			return -1;
		}
	}

	float GetBaseSkillValueByName(RE::Actor* myActor, std::string mySkill)
	{
		RE::ActorValueInfo* myAV = GetSkillByName(mySkill);

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
	RE::ActorValueInfo* GetDependantAV(RE::ActorValueInfo* myAV)
	{
		auto result = skillsLinkMap.find(myAV);
		if (result != skillsLinkMap.end())
		{
			return result->second;
		}
		return nullptr;
	}

	// Add dependent AV to given AV
	void AddDependentAV(RE::ActorValueInfo* myAV, RE::ActorValueInfo* dependentAV)
	{
		if (dependentAV->numDependentActorValues < sizeof(dependentAV->dependentActorValues) / 8)
		{
			dependentAV->dependentActorValues[dependentAV->numDependentActorValues++] = myAV;
		}
	}

	void RegisterDerivedAV(RE::ActorValueInfo* myAV, std::function<float(const RE::ActorValueOwner*, const RE::ActorValueInfo&)> CalcFunction)
	{
		using underlying_t = RE::msvc::workaround::function<float(const RE::ActorValueOwner*, const RE::ActorValueInfo&)>;

		auto& func = *reinterpret_cast<underlying_t *>(&myAV->derivationFunction);
		func._fn = new underlying_t::proxy_t();
		func._fn->_badHack = CalcFunction;
	}

	void RegisterLinkedAV(RE::ActorValueInfo* myAV, float(*CalcFunction)(const RE::ActorValueOwner*, const RE::ActorValueInfo&), RE::ActorValueInfo* av1, RE::ActorValueInfo* av2)
	{
		RegisterDerivedAV(myAV, CalcFunction);
		AddDependentAV(myAV, av1);
		AddDependentAV(myAV, av2);
		skillsLinkMap.emplace(myAV, av1);
	}

	// Returns calculation of the offset of the 
	float CalculateSkillOffset(const RE::ActorValueOwner* a_actor, const RE::ActorValueInfo& a_info)
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
		logger::info("Skills: Linking Skills from FalloutCascadia.esm");

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
}

// Internal Functions
//=========================================================================================================================
float GetAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV)
{
	if (myActor)
	{
		return myActor->GetActorValue(*myAV);
	}

	return NULL;
}

float GetBaseAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV) {
	if (myActor)
	{
		return myActor->GetBaseActorValue(*myAV);
	}

	return NULL;
}

void ModBaseAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV, float fModAmount)
{
	myActor->ModBaseActorValue(*myAV, fModAmount);
}

void SetBaseAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV, float fSetAmount)
{
	myActor->SetBaseActorValue(*myAV, fSetAmount);
}

float GetPlayerAVValue(RE::ActorValueInfo* myAV)
{
	return GetAVValue(GetPlayerCharacter(), myAV);
}

float GetPlayerBaseAVValue(RE::ActorValueInfo* myAV)
{
	return GetBaseAVValue(GetPlayerCharacter(), myAV);
}

void ModPlayerBaseAVValue(RE::ActorValueInfo* myAV, float fModAmount)
{
	ModBaseAVValue(GetPlayerCharacter(), myAV, fModAmount);
}

void SetPlayerBaseAVValue(RE::ActorValueInfo* myAV, float fSetAmount)
{
	SetBaseAVValue(GetPlayerCharacter(), myAV, fSetAmount);
}

bool DefineSkillsFormsFromGame()
{
	// Cascadia Actor Values
	CascadiaActorValues.Barter = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996845, "FalloutCascadia.esm"); //HEX: F35ED
	CascadiaActorValues.EnergyWeapons = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996846, "FalloutCascadia.esm"); //HEX: F35EE
	CascadiaActorValues.Explosives = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996847, "FalloutCascadia.esm"); //HEX: F35EF
	CascadiaActorValues.Guns = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996848, "FalloutCascadia.esm"); //HEX: F35F0
	CascadiaActorValues.Lockpick = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996849, "FalloutCascadia.esm"); //HEX: F35F1
	CascadiaActorValues.Medicine = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996850, "FalloutCascadia.esm"); //HEX: F35F2
	CascadiaActorValues.MeleeWeapons = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996851, "FalloutCascadia.esm"); //HEX: F35F3
	CascadiaActorValues.Repair = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996852, "FalloutCascadia.esm"); //HEX: F35F4 
	CascadiaActorValues.Science = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996853, "FalloutCascadia.esm"); //HEX: F35F5
	CascadiaActorValues.Sneak = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996854, "FalloutCascadia.esm"); //HEX: F35F6
	CascadiaActorValues.Speech = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996855, "FalloutCascadia.esm"); //HEX: F35F7
	CascadiaActorValues.Survival = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996856, "FalloutCascadia.esm"); //HEX: F35F8 
	CascadiaActorValues.Unarmed = RE::TESDataHandler::GetSingleton()->LookupForm<RE::ActorValueInfo>(996857, "FalloutCascadia.esm"); //HEX: F35F9

	// Cascadia Perks
	CascadiaPerks.WeaponTypeEnergyWeaponsPerk = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSPerk>(35626494, "FalloutCascadia.esm"); //HEX: 21F9DFE
	CascadiaPerks.WeaponTypeExplosivesPerk = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSPerk>(35626495, "FalloutCascadia.esm"); //HEX: 21F9DFF
	CascadiaPerks.WeaponTypeMeleeWeaponsPerk = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSPerk>(35626496, "FalloutCascadia.esm"); //HEX: 21F9E00
	CascadiaPerks.WeaponTypeGunsPerk = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSPerk>(35626497, "FalloutCascadia.esm"); //HEX: 21F9E01
	CascadiaPerks.WeaponTypeUnarmedPerk = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSPerk>(35626498, "FalloutCascadia.esm"); //HEX: 21F9E02
	CascadiaPerks.WeaponConditionHandlerPerk = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSPerk>(35626499, "FalloutCascadia.esm"); //HEX: 21F9E04

	// Cascadia Skills List
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.Barter);
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.EnergyWeapons);
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.Explosives);
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.Guns);
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.Lockpick);
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.Medicine);
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.MeleeWeapons);
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.Repair);
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.Science);
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.Sneak);
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.Speech);
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.Survival);
	Skills::CascadiaSkillsList.emplace_back(CascadiaActorValues.Unarmed);

	return true;
}
