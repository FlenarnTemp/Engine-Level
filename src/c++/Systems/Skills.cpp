#include "Skills.h"
#include "Shared/SharedFunctions.h"

REL::Relocation<uintptr_t> ActorValueDerivedVtbl{ REL::ID(337946) };
REL::Relocation<uintptr_t> ActorValueCalcVtbl{ REL::ID(510751) };

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
