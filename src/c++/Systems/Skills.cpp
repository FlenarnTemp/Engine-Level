#include "Skills.h"

class ActorValueInfo;

VanillaAV_Struct			VanillaActorValues;
CascadiaAV_Struct			CascadiaActorValues;
CascadiaPerks_Struct		CascadiaPerks;
CascadiaGlobals_Struct		CascadiaGlobals;

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
	UINT32 GetSkillValueByName(RE::Actor* myActor, std::string mySkill)
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

	UINT32 GetBaseSkillValueByName(RE::Actor* myActor, std::string mySkill)
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

void ModBaseAVVAlue(RE::Actor* myActor, RE::ActorValueInfo* myAV,int iModAmount)
{
	myActor->ModBaseActorValue(*myAV, iModAmount);
}

void SetBaseAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV, int iSetAmount)
{
	myActor->
}

