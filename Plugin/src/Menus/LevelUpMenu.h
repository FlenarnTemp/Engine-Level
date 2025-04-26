#pragma once

#include "Shared/SharedFunctions.h"
#include "Systems/Skills.h"
#include "Serialization/Serialization.h"

namespace RE
{
	namespace Cascadia
	{
		namespace LevelUpMenu
		{
			float perkPoints;

			std::uint32_t menuModeType = 0;
			std::uint32_t tagPointsValue = 0;
			
			bool allowRetag = false;

			namespace InitialValues
			{
				std::uint32_t barter;
				std::uint32_t energyWeapons;
				std::uint32_t explosives;
				std::uint32_t guns;
				std::uint32_t lockpick;
				std::uint32_t medicine;
				std::uint32_t meleeWeapons;
				std::uint32_t repair;
				std::uint32_t science;
				std::uint32_t sneak;
				std::uint32_t speech;
				std::uint32_t survival;
				std::uint32_t unarmed;
			}

			namespace TempModValues
			{
				std::uint32_t barter;
				std::uint32_t energyWeapons;
				std::uint32_t explosives;
				std::uint32_t guns;
				std::uint32_t lockpick;
				std::uint32_t medicine;
				std::uint32_t meleeWeapons;
				std::uint32_t repair;
				std::uint32_t science;
				std::uint32_t sneak;
				std::uint32_t speech;
				std::uint32_t survival;
				std::uint32_t unarmed;
			}

			void InitialiseValues()
			{
				PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();

				InitialValues::barter = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.Barter);
				InitialValues::energyWeapons = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.EnergyWeapons);
				InitialValues::explosives = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.Explosives);
				InitialValues::guns = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.Guns);
				InitialValues::lockpick = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.Lockpick);
				InitialValues::medicine = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.Medicine);
				InitialValues::meleeWeapons = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.MeleeWeapons);
				InitialValues::repair = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.Repair);
				InitialValues::science = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.Science);
				InitialValues::sneak = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.Sneak);
				InitialValues::speech = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.Speech);
				InitialValues::survival = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.Survival);
				InitialValues::unarmed = playerCharacter->GetPermanentActorValue(*Skills::CascadiaActorValues.Unarmed);
			}

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

			float GetInitialActorValue(ActorValueInfo* a_skill)
			{
				if (a_skill == Skills::CascadiaActorValues.Barter)
				{
					return InitialValues::barter;
				}
				else if (a_skill == Skills::CascadiaActorValues.EnergyWeapons)
				{
					return InitialValues::energyWeapons;
				}
				else if (a_skill == Skills::CascadiaActorValues.Explosives)
				{
					return InitialValues::explosives;
				}
				else if (a_skill == Skills::CascadiaActorValues.Guns)
				{
					return InitialValues::guns;
				}
				else if (a_skill == Skills::CascadiaActorValues.Lockpick)
				{
					return InitialValues::lockpick;
				}
				else if (a_skill == Skills::CascadiaActorValues.Medicine)
				{
					return InitialValues::medicine;
				}
				else if (a_skill == Skills::CascadiaActorValues.MeleeWeapons)
				{
					return InitialValues::meleeWeapons;
				}
				else if (a_skill == Skills::CascadiaActorValues.Repair)
				{
					return InitialValues::repair;
				}
				else if (a_skill == Skills::CascadiaActorValues.Science)
				{
					return InitialValues::science;
				}
				else if (a_skill == Skills::CascadiaActorValues.Sneak)
				{
					return InitialValues::sneak;
				}
				else if (a_skill == Skills::CascadiaActorValues.Speech)
				{
					return InitialValues::speech;
				}
				else if (a_skill == Skills::CascadiaActorValues.Survival)
				{
					return InitialValues::survival;
				}
				else if (a_skill == Skills::CascadiaActorValues.Unarmed)
				{
					return InitialValues::unarmed;
				}
				else
				{
					return -1;
				}
			}

			void PopulateSkillEntry(Scaleform::GFx::Value* a_destination, Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> a_movieRoot, ActorValueInfo* a_skill)
			{
				PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();
				float buffedValue = playerCharacter->GetActorValue(*a_skill);

				Scaleform::GFx::Value skillEntry;
				a_movieRoot->CreateObject(&skillEntry);

				BSStringT<char> description;
				a_skill->GetDescription(description);

				GFxUtilities::RegisterString(&skillEntry, a_movieRoot, "text", a_skill->fullName.c_str());
				GFxUtilities::RegisterString(&skillEntry, a_movieRoot, "editorID", a_skill->GetFormEditorID());
				GFxUtilities::RegisterString(&skillEntry, a_movieRoot, "description", description.c_str());
				GFxUtilities::RegisterInt(&skillEntry, "formid",  a_skill->formID);

				float baseValue = GetInitialActorValue(a_skill);
				float value = playerCharacter->GetPermanentActorValue(*a_skill);
			
				GFxUtilities::RegisterInt(&skillEntry, "value", (std::uint32_t)baseValue);
				GFxUtilities::RegisterInt(&skillEntry, "baseValue", (std::uint32_t)baseValue);
				GFxUtilities::RegisterInt(&skillEntry, "buffedValue", (std::uint32_t)value);

				GFxUtilities::RegisterBool(&skillEntry, "alreadyTagged", Serialization::IsSkillTagged(a_skill->formID));
				GFxUtilities::RegisterBool(&skillEntry, "tagged", Serialization::IsSkillTagged(a_skill->formID));
				a_destination->PushBack(&skillEntry);
			}

			bool ProcessSkillsList(Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> a_movieRoot)
			{
				Scaleform::GFx::Value argumentsArray[3];
				a_movieRoot->CreateArray(&argumentsArray[0]);

				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.Barter);
				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.EnergyWeapons);
				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.Explosives);
				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.Guns);
				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.Lockpick);
				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.Medicine);
				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.MeleeWeapons);
				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.Repair);
				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.Science);
				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.Sneak);
				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.Speech);
				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.Survival);
				PopulateSkillEntry(&argumentsArray[0], a_movieRoot, Skills::CascadiaActorValues.Unarmed);

				argumentsArray[1] = Serialization::GetSkillPoints();
				argumentsArray[2] = PlayerCharacter::GetSingleton()->GetLevel();
				a_movieRoot->Invoke("root.Menu_mc.onLevelUpStart", nullptr, argumentsArray, 3);

				return true;
			}

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

			float GetSkillPointsToAdd()
			{
				// Fallout 3 formula is 10 + base intelligence
				// Extra 3 points if the player has "Educated" perk
				// Using a global in 'FalloutCascaida.esm' we can adjust the base value.

				TESGlobal* baseSkillPointsGlobal = TESDataHandler::GetSingleton()->LookupForm<TESGlobal>(0x1F9DFD, "FalloutCascadia.esm");
				float skillPointsValue = 0;

				if (baseSkillPointsGlobal)
				{
					skillPointsValue = baseSkillPointsGlobal->value;
				}
				else
				{
					skillPointsValue = 10;
				}

				PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();

				float playerIntelligence = playerCharacter->GetBaseActorValue(*Skills::VanillaActorValues.Intelligence);

				skillPointsValue = (skillPointsValue + playerIntelligence);

				BGSPerk* educatedPerk = TESDataHandler::GetSingleton()->LookupForm<BGSPerk>(0x0F399E, "FalloutCascadia.esm");

				if (educatedPerk)
				{
					if (playerCharacter->GetPerkRank(educatedPerk) != 0)
					{
						skillPointsValue = (skillPointsValue + 3);
					}
				}

				return skillPointsValue;
			}

			bool CanLevelUpMenuBeShown()
			{
				PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();
				if (playerCharacter->IsInCombat())
				{
					return false;
				}

				if (InMenuMode())
				{
					return false;
				}

				// TODO - Is player in scene

				if (IsXPMetervisible())
				{
					return false;
				}

				return true;
			}

			// Waits for level up to be ready and then shows menu
			void WaitForLevelUpReady()
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				if (!CanLevelUpMenuBeShown())
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
					std::async(std::launch::async, WaitForLevelUpReady);
					return;
				}
				else
				{
					UIMessageQueue* uiMessageQueue = UIMessageQueue::GetSingleton();
					if (auto ui = UI::GetSingleton())
					{
						if (ui->menuMap.contains("CASLevelUpMenu"))
						{
							uiMessageQueue->AddMessage("CASLevelUpMenu", UI_MESSAGE_TYPE::kShow);
						}
					}
				}
			}

			// Called from 'LevelIncrease::Event'
			void HandleLevelUp()
			{
				float pointsToAdd = GetSkillPointsToAdd();

				Serialization::ModSkillPoints(pointsToAdd);

				// TODO - UpdateLevelUpFormsFromGame

				Serialization::SetReadyToLevelUp(true);

				std::thread LevelUpWait(WaitForLevelUpReady);
				LevelUpWait.detach();
			}

			// Check if player saved in the middle of a level up.
			void CheckForLevelUp()
			{
				if (Serialization::IsReadyToLevelUp())
				{
					std::thread LevelUpWait(WaitForLevelUpReady);
					LevelUpWait.detach();
				}
			}

			// Send initial data to AS3 for Level Up.
			void OnLevelUpStart(Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> a_movieRoot)
			{
				ProcessSkillsList(a_movieRoot);
			}

			void HandleLevelUpMenuOpen(Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> a_movieRoot)
			{
				switch (menuModeType)
				{
				case kLevelUp:
					OnLevelUpStart(a_movieRoot);
					break;

				case kTagSkills:
					break;

				case kSpecialRespec:
					break;

				case kIntenseTraining:
					break;

				default:
					break;
				}
			}

			void CompleteLevelUp()
			{
				switch (menuModeType)
				{
				case kLevelUp:
					Serialization::SetReadyToLevelUp(false);
					Serialization::SetSkillPoints(0);
					break;

				case kTagSkills:
					// TODO - tagPointsValue = 0;
					allowRetag = false;
					break;

				case kSpecialRespec:
					break;

				case kIntenseTraining:
					break;
				}

				menuModeType = 0;
			}

			class OpenMenu : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("Open menu called from AS3");
					LevelUpMenu::InitialiseValues();
					// TODO - RegisterForInput
					LevelUpMenu::HandleLevelUpMenuOpen(a_params.movie->asMovieRoot);
				}
			};

			class CloseMenu : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("Close menu called from AS3");
				}
			};

			class SetSkills : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("SetSkills called from AS3");
				}
			};

			class ResetSkills : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("ResetSkills called from AS3");
				}
			};

			class ResetTagSkills : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("ResetTagSkills called from AS3");
				}
			};

			class BackToSkills : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("BackToSkills called from AS3");
				}
			};

			class UpdatePerksMenu : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("UpdatePerksMenu called from AS3");
				}
			};

			class AddPerks : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("AddPerks called from AS3");
				}
			};

			class TagSkills : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("TagSkills called from AS3");
				}
			};

			class PlayUISound : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("PlayUISound called from AS3");
					if (a_params.argCount < 1) return;
					if (!a_params.args[0].IsString()) return;

					auto uiSound = a_params.args[0].GetString();

					UIUtils::PlayMenuSound(uiSound);
				}
			};

			class LearnSpecial : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("LearnSpecial called from AS3");
				}
			};

			class ResetSpecial : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("ResetSpecial called from AS3");
				}
			};

			class CASLevelUpMenu :
				public GameMenuBase
			{
			public:
				static IMenu* CreateCASLevelUpMenu(const UIMessage&)
				{
					return new CASLevelUpMenu();
				}

				const char* sMenuName = "CASLevelUpMenu";

				void ProcessUserEventExternal(const char* controlName, bool isDown, std::uint32_t deviceType, BS_BUTTON_CODE buttonCode)
				{
					UI* ui = UI::GetSingleton();

					if (ui->GetMenuOpen(sMenuName))
					{
						IMenu* menu = ui->GetMenu(sMenuName).get();
						Scaleform::GFx::ASMovieRootBase* movieRoot = menu->uiMovie->asMovieRoot.get();
						Scaleform::GFx::Value arguments[4];
						arguments[0] = controlName;
						arguments[1] = isDown;
						arguments[2] = deviceType;
						arguments[3] = static_cast<std::int32_t>(buttonCode);
						movieRoot->Invoke("root.Menu_mc.ProcessUserEventExternal", nullptr, arguments, 4);
					}
				}

				void OnButtonEvent(const ButtonEvent* a_event) override 
				{
					ProcessUserEventExternal(a_event->QUserEvent().c_str(), a_event->QHeldDown(), a_event->device.underlying(), a_event->GetBSButtonCode());
				}

				CASLevelUpMenu()
				{
					menuFlags.set(
						UI_MENU_FLAGS::kPausesGame,
						UI_MENU_FLAGS::kUsesCursor,
						UI_MENU_FLAGS::kTopmostRenderedMenu,
						UI_MENU_FLAGS::kUpdateUsesCursor,
						UI_MENU_FLAGS::kUsesBlurredBackground,
						UI_MENU_FLAGS::kUsesMovementToDirection
					);
					menuHUDMode = "SpecialMode";
					depthPriority = UI_DEPTH_PRIORITY::kTerminal;
					const auto ScaleformManager = BSScaleformManager::GetSingleton();

					[[maybe_unused]] const auto LoadMovieSuccess =
						ScaleformManager->LoadMovieEx(*this, "Interface/CASLevelUpMenu.swf"sv, "root", Scaleform::GFx::Movie::ScaleModeType::kExactFit);
					assert(LoadMovieSuccess);

					Scaleform::GFx::ASMovieRootBase* movieRoot = uiMovie.get()->asMovieRoot.get();
					Scaleform::GFx::Value bgsCodeObj;
					movieRoot->GetVariable(&bgsCodeObj, "root.Menu_mc.BGSCodeObj");

					filterHolder = RE::msvc::make_unique<BSGFxShaderFXTarget>(*uiMovie, "root.Menu_mc");
					if (filterHolder)
					{
						filterHolder->CreateAndSetFiltersToHUD(HUDColorTypes::kGameplayHUDColor);
						shaderFXObjects.push_back(filterHolder.get());
					}

					RegisterFunction<OpenMenu>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "OpenMenu");
					RegisterFunction<SetSkills>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "SetSkills");
					RegisterFunction<ResetSkills>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "ResetSkills");
					RegisterFunction<ResetTagSkills>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "ResetTagSkills");
					RegisterFunction<BackToSkills>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "BackToSkills");
					RegisterFunction<UpdatePerksMenu>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "UpdatePerkMenu");
					RegisterFunction<AddPerks>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "AddPerks");
					RegisterFunction<TagSkills>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "TagSkills");
					RegisterFunction<LearnSpecial>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "LearnSpecial");
					RegisterFunction<ResetSpecial>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "ResetSpecial");
					RegisterFunction<OpenMenu>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "PlaySkillSound");
					RegisterFunction<OpenMenu>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "PlayPerkSound");
					RegisterFunction<CloseMenu>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "CloseMenu");
					RegisterFunction<PlayUISound>(&bgsCodeObj, uiMovie.get()->asMovieRoot, "PlayUISound");

					movieRoot->Invoke("root.Menu_mc.onCodeObjCreate", nullptr, nullptr, 0);
				};

				static IMenu* Create(const UIMessage&)
				{
					return new CASLevelUpMenu();
				}
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

						RegisterFunction<OpenMenu>(&bgsCodeObj, a_view->asMovieRoot, "OpenMenu");
						RegisterFunction<SetSkills>(&bgsCodeObj, a_view->asMovieRoot, "SetSkills");
						RegisterFunction<OpenMenu>(&bgsCodeObj, a_view->asMovieRoot, "ResetSkills");
						RegisterFunction<ResetTagSkills>(&bgsCodeObj, a_view->asMovieRoot, "ResetTagSkills");
						RegisterFunction<BackToSkills>(&bgsCodeObj, a_view->asMovieRoot, "BackToSkills");
						RegisterFunction<UpdatePerksMenu>(&bgsCodeObj, a_view->asMovieRoot, "UpdatePerkMenu");
						RegisterFunction<AddPerks>(&bgsCodeObj, a_view->asMovieRoot, "AddPerks");
						RegisterFunction<TagSkills>(&bgsCodeObj, a_view->asMovieRoot, "TagSkills");
						RegisterFunction<OpenMenu>(&bgsCodeObj, a_view->asMovieRoot, "LearnSpecial");
						RegisterFunction<OpenMenu>(&bgsCodeObj, a_view->asMovieRoot, "ResetSpecial");
						RegisterFunction<OpenMenu>(&bgsCodeObj, a_view->asMovieRoot, "PlaySkillSound");
						RegisterFunction<OpenMenu>(&bgsCodeObj, a_view->asMovieRoot, "PlayPerkSound");
						RegisterFunction<CloseMenu>(&bgsCodeObj, a_view->asMovieRoot, "CloseMenu");
						RegisterFunction<PlayUISound>(&bgsCodeObj, a_view->asMovieRoot, "PlayUISound");

						a_view->asMovieRoot->Invoke("root.Menu_mc.onCodeObjCreate", nullptr, nullptr, 0);
					}
					return true;
				}
				return false;
			}

			inline void RegisterMenu()
			{
				if (UI* ui = UI::GetSingleton())
				{
					if (!ui->menuMap.contains("CASLevelUpMenu"))
					{
						ui->RegisterMenu("CASLevelUpMenu", CASLevelUpMenu::Create);
						DEBUG("Registered 'CASLevelUpMenu'.");
					}
				}
			}
		}
	}
}