#pragma once

#include "Shared/SharedFunctions.h"

namespace RE
{
	namespace Cascadia
	{
		namespace LevelUpMenu
		{
			enum SkillArray
			{
				Barter = 0,
				EnergyWeapons = 1,
				Explosives = 2,
				Guns = 3,
				Lockpick = 4,
				Medicine = 5,
				MeleeWeapons = 6,
				Repair = 7,
				Science = 8,
				Sneak = 9,
				Speech = 10,
				Survival = 11,
				Unarmed = 12
			};

			enum LevelTypes
			{
				kLevelUp = 0,			// Normal level up
				kTagSkills = 1,			// Tag skills mode
				kSpecialRespec = 2,		// Special respec mode
				kIntenseTraining = 3	// 'Intense Training' perk
			};

			enum PerkTypes
			{
				kDefault = 0,
				kStrength = 1,
				kPerception = 2,
				kEndurance = 3,
				kCharisma = 4,
				kIntelligence = 5,
				kAgility = 6,
				kLuck = 7,
				kIntenseTraining = 8,
				kCancel = 9
			};

			bool IsXPMetervisible()
			{
				BSFixedString menuString("HUDMenu");
				IMenu* menu = UI::GetSingleton()->GetMenu(menuString).get();
				Scaleform::GFx::Value openValue;
				menu->uiMovie->asMovieRoot->GetVariable(&openValue, "root.HUDNotificationsGroup_mc.XPMeter_mc.visible");

				return openValue.GetBoolean();
			}

			std::uint32_t GetSkillArrayIndexByEditorID(const char* editorID) 
			{
				// Create a map of editor IDs to skill array indices.
				static const std::unordered_map<std::string, std::uint32_t> editorIDMap = {
					{"CAS_Barter", SkillArray::Barter},
					{"CAS_EnergyWeapons", SkillArray::EnergyWeapons},
					{"CAS_Explosives", SkillArray::Explosives},
					{"CAS_Guns", SkillArray::Guns},
					{"CAS_Lockpick", SkillArray::Lockpick},
					{"CAS_Medicine", SkillArray::Medicine},
					{"CAS_MeleeWeapons", SkillArray::MeleeWeapons},
					{"CAS_Repair", SkillArray::Repair},
					{"CAS_Science", SkillArray::Science},
					{"CAS_Speech", SkillArray::Speech},
					{"CAS_Surival", SkillArray::Survival},
					{"CAS_Unarmed", SkillArray::Unarmed}
				};

				// Find the editorID in the map.
				auto it = editorIDMap.find(editorID);
				if (it != editorIDMap.end()) {
				    return it->second;
				}

				// Return -1 if the editor ID is not found.
				return static_cast<std::uint32_t>(-1);
			}
		}
	}
}