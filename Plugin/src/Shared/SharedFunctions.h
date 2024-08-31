#pragma once

#include <Systems/ItemDegradation.h>

namespace RE
{
	namespace Cascadia
	{
		namespace GFxUtilities
		{
			void RegisterArray(Scaleform::GFx::Value* destination, Scaleform::GFx::ASMovieRootBase* asMovieRoot, const char* name, Scaleform::GFx::Value* array);
			void RegisterString(Scaleform::GFx::Value* destination, Scaleform::GFx::ASMovieRootBase* asMovieRoot, const char* name, const char* string);
			void RegisterNumber(Scaleform::GFx::Value* destination, const char* name, double value);
			void RegisterInt(Scaleform::GFx::Value* destination, const char* name, std::uint32_t value);
		}

		bool IsFormIDStringBaseGame(std::string formIDString);
		std::string GetFormIDAsString(std::uint32_t formID);

		std::uint32_t GetActorSex(Actor* a_actor);
		PlayerCharacter* GetPlayerCharacter();
		bool IsPlayerGodMode();
		std::int16_t GetPlayerLevel();
		std::uint32_t GetPlayerSex();

		std::string FloatToPreciseString(float value, std::uint32_t precision);

		bool WeaponHasKeyword(TESObjectWEAP* weapon, BGSKeyword* keyword);
		bool ArmorHasKeyword(TESObjectARMO* armor, BGSKeyword* keyword);
		bool ReferenceHasKeyword(TESObjectREFR* ref, BGSKeyword* keyword);

		bool NPCHasKeyword(TESNPC* npc, BGSKeyword* keyword);
		bool IsInMenuMode();

		std::uint32_t PositionInFormList(TESForm* form, BGSListForm* list);
		std::uint32_t GetNextAvailableFormInInventoryFromList(std::uint32_t startingIndex, BGSListForm* list);
		bool IsFormInList(TESForm* form, BGSListForm* list);

		bool HasVMScript(TESForm* form, const char* scriptName);

		//bool IsMeleeWeapon(WeaponConditionData myConditionData);

		float ConvertPercentageToFloat(std::uint8_t percentage);

		std::string trim(const std::string& str);

		template <typename T>
		void RegisterFunction(Scaleform::GFx::Value* dest, Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> movieRoot, const char* func_name)
		{
			Scaleform::GFx::Value fnValue;
			Scaleform::GFx::FunctionHandler* func = nullptr;
			func = new T;

			movieRoot->CreateFunction(&fnValue, func);
			dest->SetMember(func_name, fnValue);
		}

		template<class Ty>
		Ty SafeWrite64Function(uintptr_t addr, Ty data) {
			DWORD oldProtect;
			void* _d[2];
			memcpy(_d, &data, sizeof(data));
			size_t len = sizeof(_d[0]);
	
			VirtualProtect((void*)addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
			Ty olddata;
			memset(&olddata, 0, sizeof(Ty));
			memcpy(&olddata, (void*)addr, len);
			memcpy((void*)addr, &_d[0], len);
			VirtualProtect((void*)addr, len, oldProtect, &oldProtect);
			return olddata;
		}
	}
}
