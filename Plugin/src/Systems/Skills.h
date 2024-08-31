#pragma once

namespace RE
{
	namespace Cascadia
	{
		

		namespace Skills
		{
			typedef std::vector<ActorValueInfo*> AVVector;

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

			void PopulateSkillEntry(Scaleform::GFx::Value* destination, Scaleform::GFx::ASMovieRootBase* asMovieRoot, ActorValueInfo* skill, std::uint32_t filter, std::vector<std::string> stringValue);
			void PopulateSkillEntries(Scaleform::GFx::ASMovieRootBase* asMovieRoot);

			ActorValueInfo* GetSkillByName(std::string mySkill);
			float GetSkillByValueName(Actor* myActor, std::string mySkill);
			float GetBaseSkillValueByName(Actor* myActor, std::string mySkill);

			ActorValueInfo* GetDependantAV(ActorValueInfo* myAV);
			void AddDependentAV(ActorValueInfo* myAV, ActorValueInfo* dependantAV);
			void RegisterDerivedAV(ActorValueInfo* myAV, std::function<RE::ActorValueInfo::DerivationFunction_t> CalcFunction);
			void RegisterLinkedAV(ActorValueInfo* myAV, std::function<RE::ActorValueInfo::DerivationFunction_t> CalcFunction, ActorValueInfo* av1, ActorValueInfo* av2);
			float CalculateSkillOffset(const ActorValueOwner* myAVOwner, const ActorValueInfo& myAV);
			void RegisterForSkillLink();

			float GetAVValue(Actor* myActor, ActorValueInfo* myAV);
			float GetBaseAVValue(Actor* myActor, ActorValueInfo* myAV);
			void ModBaseAVValue(Actor* myActor, ActorValueInfo* myAV, float fModAmount);
			void SetBaseAVValue(Actor* myActor, ActorValueInfo* myAV, float fSetAmount);

			float GetPlayerAVValue(ActorValueInfo* myAV);
			float GetPlayerBaseAVValue(ActorValueInfo* myAV);
			void ModPlayerBaseAVValue(ActorValueInfo* myAV, float fModAmount);
			void SetPlayerBaseAVValue(ActorValueInfo* myAV, float fSetAmount);

			bool DefineSkillsFormsFromGame();
		}
	}
}
