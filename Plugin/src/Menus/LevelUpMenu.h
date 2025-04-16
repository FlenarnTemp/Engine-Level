#pragma once

#include "Shared/SharedFunctions.h"

namespace RE
{
	namespace Cascadia
	{
		namespace LevelUpMenu
		{
			float perkPoints;

			std::uint32_t menuModeType;

			std::uint32_t tagPointsValue;

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
				kIntenseTrainingPerk = 8,
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

			void HandleLevelUpMenuOpen()
			{
				switch (menuModeType)
				{
				case kLevelUp:
					break;

				case kTagSkills:
					break;

				case kSpecialRespec:
					break;

				case kIntenseTraining:
					break;
				}
			}

			class CASLevelUpMenu :
				public GameMenuBase
			{
			public:
				static IMenu* CreateCASLevelUpMenu(const UIMessage&)
				{
					return new CASLevelUpMenu();
				}

				const char* sMenuName = "CASLevelUpMenu";

				CASLevelUpMenu()
				{
					menuFlags.set(
						UI_MENU_FLAGS::kUsesCursor,
						UI_MENU_FLAGS::kTopmostRenderedMenu,
						UI_MENU_FLAGS::kUpdateUsesCursor,
						UI_MENU_FLAGS::kUsesBlurredBackground
					);

					depthPriority = UI_DEPTH_PRIORITY::kTerminal;

					const auto ScaleformManager = BSScaleformManager::GetSingleton();
					if (ScaleformManager->LoadMovieEx(*this, "Interface/CASLevelUpMenu.swf", "root", Scaleform::GFx::Movie::ScaleModeType::kExactFit))
					{
						Scaleform::GFx::ASMovieRootBase* movieRoot = uiMovie.get()->asMovieRoot.get();
						Scaleform::GFx::Value bgsCodeObj;
						movieRoot->GetVariable(&bgsCodeObj, "root.Menu_mc.BGSCodeObj");

						// TODO
						filterHolder = RE::msvc::make_unique<BSGFxShaderFXTarget>(*uiMovie, "root.Menu_mc");
						if (filterHolder)
						{
							filterHolder->CreateAndSetFiltersToHUD(HUDColorTypes::kGameplayHUDColor);
							shaderFXObjects.push_back(filterHolder.get());
						}

						movieRoot->Invoke("root.Menu_mc.onCodeObjCreate", nullptr, nullptr, 0);
					}
				};
			};

			bool RegisterScaleform(Scaleform::GFx::Movie* a_view, Scaleform::GFx::Value* a_value)
			{
				Scaleform::GFx::Value currentSWFPath;
				// Register native code handlers.
				if (a_view->asMovieRoot->GetVariable(&currentSWFPath, "root.loaderInfo.url"))
				{
					if (_stricmp(currentSWFPath.GetString(), "Interface/CASLevelUpMenu.swf") == 0)
					{
						Scaleform::GFx::Value bgsCodeObj;
						a_view->asMovieRoot->GetVariable(&bgsCodeObj, "root.Menu_mc.BGSCodeObj");
					}
					return true;
				}
				return false;
			}
		}
	}
}