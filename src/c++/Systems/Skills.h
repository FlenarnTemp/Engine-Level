#pragma once

#include "GameForms.h"

enum SPECIALFormIDs 
{
	StrengthID = 706,
	PerceptionID,
	EnduranceID,
	CharismaID,
	IntelligenceID,
	AgilityID,
	LuckID,
	ExperienceID
};

namespace Skills
{
	RE::ActorValueInfo* GetSkillByName(std::string mySkill);
	float GetSkillByValueName(RE::Actor* myActor, std::string mySkill);
	float GetBaseSkillValueByName(RE::Actor* myActor, std::string mySkill);

	RE::ActorValueInfo* GetDependantAV(RE::ActorValueInfo* myAV);
	void AddDependentAV(RE::ActorValueInfo* myAV, RE::ActorValueInfo* dependantAV);

	float CalculateSkillOffset(RE::ActorValueOwner* myAVOwner, RE::ActorValueInfo& myAV);
}

float GetAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV);
float GetBaseAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV);
void ModBaseAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV, float iModAmount);
void SetBaseAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV, float iSetAmount);

float GetPlayerAVValue(RE::ActorValueInfo* myAV);
float GetPlayerBaseAVValue(RE::ActorValueInfo* myAV);
