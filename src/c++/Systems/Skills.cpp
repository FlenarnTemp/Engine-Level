#include "Skills.h"
#include "Shared/SharedFunctions.h"

REL::Relocation<uintptr_t> ActorValueDerivedVtbl	{ REL::ID(337946) };
REL::Relocation<uintptr_t> ActorValueCalcVtbl		{ REL::ID(510751) };

class ActorValueInfo;

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
	//	Most functions in this namespace are based on shad0wshayd3's work on Project Massachusetts & HcG x Grills rework for the Capital Wasteland.
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

	void RegisterDerivedAV(RE::ActorValueInfo* myAV, float(*f)(RE::ActorValueOwner*, RE::ActorValueInfo&))
	{
		// TODO
	}

	void RegisterLinkedAV(RE::ActorValueInfo* myAV, float(*CalcFunction)(RE::ActorValueOwner*, RE::ActorValueInfo&), RE::ActorValueInfo* av1, RE::ActorValueInfo* av2)
	{
		RegisterDerivedAV(myAV, CalcFunction);
		AddDependentAV(myAV, av1);
		AddDependentAV(myAV, av2);
		skillsLinkMap.emplace(myAV, av1);
	}

	void RegisterCalc(RE::ActorValueInfo* myAV, void(*f)(RE::Actor*, RE::ActorValueInfo&, float, float, RE::Actor*))
	{
		//myAV->VTABLE = reinterpret_cast<void*>(REL::Relocation(ActorValueCalcVtbl).get());
		myAV->derivationFunction = f;
	}

	float CalculateSkillOffset(RE::ActorValueOwner* myAVOwner, RE::ActorValueInfo& myAV)
	{
		if (!myAVOwner || !&myAV)
		{
			return 0.0;
		}
		else
		{
			// (Dependent x 2) + 2 + (Luck / 2)
			return ((myAVOwner->GetActorValue(*GetDependantAV(&myAV)) * 2) + 2 + ceilf(myAVOwner->GetActorValue(*VanillaActorValues.Luck) / 2));
		}
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
	// Vanilla Actor Values
	VanillaActorValues.Strength		= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(706, "Fallout4.esm")); //HEX: 2C2
	VanillaActorValues.Perception	= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(707, "Fallout4.esm")); //HEX: 2C3
	VanillaActorValues.Endurance	= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(708, "Fallout4.esm")); //HEX: 2C4
	VanillaActorValues.Charisma		= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(709, "Fallout4.esm")); //HEX: 2C5
	VanillaActorValues.Intelligence	= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(710, "Fallout4.esm")); //HEX: 2C6
	VanillaActorValues.Agility		= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(711, "Fallout4.esm")); //HEX: 2C7
	VanillaActorValues.Luck			= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(712, "Fallout4.esm")); //HEX: 2C8

	// Cascadia Actor Values
	CascadiaActorValues.Barter			= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996845, "FalloutCascadia.esm")); //HEX: F35ED
	CascadiaActorValues.EnergyWeapons	= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996846, "FalloutCascadia.esm")); //HEX: F35EE
	CascadiaActorValues.Explosives		= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996847, "FalloutCascadia.esm")); //HEX: F35EF
	CascadiaActorValues.Guns			= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996848, "FalloutCascadia.esm")); //HEX: F35F0
	CascadiaActorValues.Lockpick		= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996849, "FalloutCascadia.esm")); //HEX: F35F1
	CascadiaActorValues.Medicine		= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996850, "FalloutCascadia.esm")); //HEX: F35F2
	CascadiaActorValues.MeleeWeapons	= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996851, "FalloutCascadia.esm")); //HEX: F35F3
	CascadiaActorValues.Repair			= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996852, "FalloutCascadia.esm")); //HEX: F35F4 
	CascadiaActorValues.Science			= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996853, "FalloutCascadia.esm")); //HEX: F35F5
	CascadiaActorValues.Sneak			= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996854, "FalloutCascadia.esm")); //HEX: F35F6
	CascadiaActorValues.Speech			= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996855, "FalloutCascadia.esm")); //HEX: F35F7
	CascadiaActorValues.Survival		= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996856, "FalloutCascadia.esm")); //HEX: F35F8 
	CascadiaActorValues.Unarmed			= reinterpret_cast<RE::ActorValueInfo*>(RE::TESDataHandler().LookupForm(996857, "FalloutCascadia.esm")); //HEX: F35F9

	// Cascadia Perks
	CascadiaPerks.WeaponTypeEnergyWeaponsPerk	= reinterpret_cast<RE::BGSPerk*>(RE::TESDataHandler().LookupForm(35626494, "FalloutCascadia.esm")); //HEX: 21F9DFE
	CascadiaPerks.WeaponTypeExplosivesPerk		= reinterpret_cast<RE::BGSPerk*>(RE::TESDataHandler().LookupForm(35626495, "FalloutCascadia.esm")); //HEX: 21F9DFF
	CascadiaPerks.WeaponTypeMeleeWeaponsPerk	= reinterpret_cast<RE::BGSPerk*>(RE::TESDataHandler().LookupForm(35626496, "FalloutCascadia.esm")); //HEX: 21F9E00
	CascadiaPerks.WeaponTypeGunsPerk			= reinterpret_cast<RE::BGSPerk*>(RE::TESDataHandler().LookupForm(35626497, "FalloutCascadia.esm")); //HEX: 21F9E01
	CascadiaPerks.WeaponTypeUnarmedPerk			= reinterpret_cast<RE::BGSPerk*>(RE::TESDataHandler().LookupForm(35626498, "FalloutCascadia.esm")); //HEX: 21F9E02
	CascadiaPerks.WeaponConditionHandlerPerk	= reinterpret_cast<RE::BGSPerk*>(RE::TESDataHandler().LookupForm(35626499, "FalloutCascadia.esm")); //HEX: 21F9E04

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
