#pragma once

#include <Systems/ItemDegradation.h>

namespace RE
{
	namespace Cascadia
	{
		bool IsFormIDStringBaseGame(std::string formIDString);
		std::string GetFormIDAsString(std::uint32_t formID);

		std::uint32_t GetActorSex(Actor* a_actor);
		PlayerCharacter* GetPlayerCharacter();
		bool IsPlayerGodMode();
		std::int16_t GetPlayerLevel();
		std::uint32_t GetPlayerSex();

		std::string FloatToPreciseString(float value, std::uint32_t precision);

		std::uint32_t RNG(std::uint32_t min, std::uint32_t max);
		float RNG(float min, float max);

		bool WeaponHasKeyword(TESObjectWEAP* weapon, BGSKeyword* keyword);
		bool ArmorHasKeyword(TESObjectARMO* armor, BGSKeyword* keyword);
		bool ReferenceHasKeyword(TESObjectREFR* ref, BGSKeyword* keyword);

		bool NPCHasKeyword(TESNPC* npc, BGSKeyword* keyword);
		bool IsInMenuMode();

		std::uint32_t PositionInFormList(TESForm* form, BGSListForm* list);
		std::uint32_t GetNextAvailableFormInInventoryFromList(std::uint32_t startingIndex, BGSListForm* list);
		bool IsFormInList(TESForm* form, BGSListForm* list);

		void SetWheelZoomEnabled(bool enbled);
		void SetFavoritesEnabled(bool enabled);
		void SetMovementEnabled(bool enabled);

		//bool IsMeleeWeapon(WeaponConditionData myConditionData);
		//float CalculateSkillBonusFromActor(WeaponConditionData myConditionData);

		Setting* GetGMST(const char* a_name);

		float ConvertPercentageToFloat(std::uint8_t percentage);

	}
}
