#pragma once

#include "GameForms.h"

typedef std::vector<RE::ActorValueInfo*> AVVector;


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
	void RegisterDerivedAV(RE::ActorValueInfo* myAV, float(*f)(RE::ActorValueOwner*, RE::ActorValueInfo&));
	void RegisterLinkedAV(RE::ActorValueInfo* myAV, float(*CalcFunction)(RE::ActorValueOwner*, RE::ActorValueInfo&), RE::ActorValueInfo* av1, RE::ActorValueInfo* av2);

	float CalculateSkillOffset(RE::ActorValueOwner* myAVOwner, RE::ActorValueInfo& myAV);
}

float GetAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV);
float GetBaseAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV);
void ModBaseAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV, float fModAmount);
void SetBaseAVValue(RE::Actor* myActor, RE::ActorValueInfo* myAV, float fSetAmount);

float GetPlayerAVValue(RE::ActorValueInfo* myAV);
float GetPlayerBaseAVValue(RE::ActorValueInfo* myAV);
void ModPlayerBaseAVValue(RE::ActorValueInfo* myAV, float fModAmount);
void SetPlayerBaseAVValue(RE::ActorValueInfo* myAV, float fSetAmount);

bool DefineSkillsFormsFromGame();
