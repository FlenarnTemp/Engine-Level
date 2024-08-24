#include "Shared/Hooks.h"
namespace RE
{
	namespace Cascadia
	{
		namespace Hooks
		{
			void Install(F4SE::Trampoline& trampoline)
			{
				// GetInfoForPlayerDialogueOptionHook - 2196817 ID + 0x40A offset
				typedef TESTopicInfo(GetCurrentTopicInfo_Player_Sig)(BGSSceneActionPlayerDialogue* apPlayerDialogue, BGSScene* apParentScene, TESObjectREFR* apTarget, std::uint32_t aeType);
				REL::Relocation<GetCurrentTopicInfo_Player_Sig> GetCurrentTopicInfo_Player_Location{ REL::ID(2196817), 0x40A };
				trampoline.write_call<5>(GetCurrentTopicInfo_Player_Location.address(), &RE::GetCurrentTopicInfo_Player_Hook);

				// GetInfoForNPCResponseDialogOptionHook - 2196817 ID + 0x78E offset
				typedef TESTopicInfo(GetCurrentTopicInfo_NPC_Sig)(BGSSceneActionPlayerDialogue* apPlayerDialogue, BGSScene* apParentScene, TESObjectREFR* apTarget, std::uint32_t aeType);
				REL::Relocation<GetCurrentTopicInfo_NPC_Sig> GetCurrentTopicInfo_NPC_Location{ REL::ID(2196817), 0x7A1 };
				trampoline.write_call<5>(GetCurrentTopicInfo_NPC_Location.address(), &RE::GetCurrentTopicInfo_NPC_Hook);

				// GetNPCResponseInfoForOptionHook - 2196800 ID + 0x388 offset
				typedef TESTopicInfo(GetCurrentTopicInfo_NPCAction_Hook)(BGSSceneActionNPCResponseDialogue* apNPCDialogue, BGSScene* apParentScene);
				REL::Relocation<GetCurrentTopicInfo_NPCAction_Hook> GetCurrentTopicInfo_NPCAction_Location{ REL::ID(2196800), 0x388 };
				trampoline.write_call<5>(GetCurrentTopicInfo_NPCAction_Location.address(), &RE::GetCurrentTopicInfo_NPCAction_Hook);
			}

			DetourXS hook_ShowBuildFailureMessage;
			typedef void(ShowBuildFailureMessageSig)(WorkbenchMenuBase*);

			void HookWorkbenchMenuBaseShowBuildFailureMessage(WorkbenchMenuBase* a_this)
			{
				GameSettingCollection* gameSettingCollection = GameSettingCollection::GetSingleton();
				if (a_this->repairing)
				{
					Scaleform::Ptr<RE::ExamineMenu> examineMenu = UI::GetSingleton()->GetMenu<RE::ExamineMenu>();

					std::uint32_t selectedIndex = examineMenu->GetSelectedIndex();
					if (!examineMenu->invInterface.entriesInvalid && (selectedIndex & 0x80000000) == 0 && selectedIndex < examineMenu->invInterface.stackedEntries.size())
					{
						InventoryUserUIInterfaceEntry* inventoryUUIEntry = (examineMenu->invInterface.stackedEntries.data() + selectedIndex);
						const BGSInventoryItem* inventoryItem = BGSInventoryInterface::GetSingleton()->RequestInventoryItem(inventoryUUIEntry->invHandle.id);

						if (inventoryItem)
						{
							BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>* requiredItems = a_this->QCurrentModChoiceData()->recipe->requiredItems;
							if (requiredItems)
							{
								RE::RepairFailureCallback* repairFailureCallback = new RE::RepairFailureCallback(examineMenu.get());
								RE::ExamineConfirmMenu::InitDataRepairFailure* initDataRepair = new RE::ExamineConfirmMenu::InitDataRepairFailure(requiredItems);

								examineMenu->ShowConfirmMenu(initDataRepair, repairFailureCallback);
							}

							for (const auto& tuple : *a_this->QCurrentModChoiceData()->requiredItems) {
								TESForm* form = tuple.first;
								BGSTypedFormValuePair::SharedVal value = tuple.second;

								const char* fullName = TESFullName::GetFullName(*form).data();
								DEBUG("Component: {}, amount: {}", fullName, value.i);
							}

							SendHUDMessage::ShowHUDMessage("You lack the requirements to repair this object.", nullptr, true, true);
							a_this->repairing = false;
						}
					}
				}
				else
				{
					SendHUDMessage::ShowHUDMessage(gameSettingCollection->GetSetting("sCannotBuildMessage")->GetString().data(), nullptr, true, true);
				}
			}

			DetourXS hook_GetBuildConfirmQuestion;
			typedef void(GetBuildConfirmQuestionSig)(RE::ExamineMenu*, char*, std::uint32_t);

			void HookExamineMenuGetBuildConfirmQuestion(RE::ExamineMenu* a_this, char* a_buffer, std::uint32_t a_bufferLength)
			{
				const WorkbenchMenuBase::ModChoiceData* modChoiceData;
				const char* fullName;
				const char* type;

				if (a_this->QCurrentModChoiceData()->recipe)
				{
					modChoiceData = a_this->QCurrentModChoiceData();

					
					if (a_this->repairing)
					{
						type = "$$Repair";

						std::uint32_t selectedIndex = a_this->GetSelectedIndex();
						if (!a_this->invInterface.entriesInvalid && (selectedIndex & 0x80000000) == 0 && selectedIndex < a_this->invInterface.stackedEntries.size())
						{
							InventoryUserUIInterfaceEntry* inventoryUUIEntry = (a_this->invInterface.stackedEntries.data() + selectedIndex);
							const BGSInventoryItem* inventoryItem = BGSInventoryInterface::GetSingleton()->RequestInventoryItem(inventoryUUIEntry->invHandle.id);

							fullName = inventoryItem->GetDisplayFullName(inventoryUUIEntry->stackIndex.at(0));

							/*if (inventoryItem->stackData.get()->extra->HasType(EXTRA_DATA_TYPE::kTextDisplayData))
							{
								BSExtraData* test = inventoryItem->stackData.get()->extra->GetByType(EXTRA_DATA_TYPE::kTextDisplayData);
								ExtraTextDisplayData* displayText = (ExtraTextDisplayData*)test;

								fullName = displayText->displayName.c_str();
							}*/
						}
					}
					else
					{
						type = "$$Make";

						fullName = TESFullName::GetFullName(*modChoiceData->recipe->createdItem).data();
					}
					snprintf(a_buffer, a_bufferLength, "%s %s?", type, fullName);
				}
			}

			DetourXS hook_SetHealthPerc;
			typedef void(SetHealthPercSig)(ExtraDataList*, float);
			REL::Relocation<SetHealthPercSig> SetHealthPercOriginal;

			void HookExtraDataListSetHealthPerc(ExtraDataList* a_this, float a_health)
			{
				SetHealthPercOriginal(a_this, a_health);
				if (a_health == 1.0f && a_this->GetHealthPerc() != 1.0f)
				{
					a_this->AddExtra(new ExtraHealth(1.0f));
				}
			}

			DetourXS hook_QCurrentModChoiceData;
			typedef const WorkbenchMenuBase::ModChoiceData* (QCurrentModChoiceDataSig)(WorkbenchMenuBase*);

			const WorkbenchMenuBase::ModChoiceData* HookWorkbenchMenuBaseQCurrentModChoiceData(WorkbenchMenuBase* a_this)
			{
				if (a_this->repairing)
				{
					// TODO: Merge this into singular function returning dynamic repair cost.
					if (typeid(*a_this) == typeid(PowerArmorModMenu))
					{
						PowerArmorModMenu* powerArmorModMenu = dynamic_cast<PowerArmorModMenu*>(a_this);
						return &powerArmorModMenu->repairData;
					}
					else
					{
						std::uint32_t modChoiceIndex = a_this->modChoiceIndex;
						if (modChoiceIndex >= a_this->modChoiceArray.size())
						{
							return 0;
						}
						else
						{
							auto modChoicedata = (a_this->modChoiceArray.data() + modChoiceIndex);

							if (!modChoicedata->recipe)
							{
								FATAL("No recipe found.");
							}

							auto recipeConditions = modChoicedata->recipe->conditions;

							if (!recipeConditions.IsTrue(Cascadia::GetPlayerCharacter(), Cascadia::GetPlayerCharacter()))
							{
								DEBUG("Recipe conditions failed.")
							}

							if (modChoicedata->requiredPerks.size() > 0)
							{
								DEBUG("Required perks found, removing them.");
								modChoicedata->requiredPerks.clear();
							}

							return (modChoicedata);
						}
					}
				}
				else
				{
					std::uint32_t modChoiceIndex = a_this->modChoiceIndex;
					if (modChoiceIndex >= a_this->modChoiceArray.size())
					{
						return 0;
					}
					else
					{
						return (a_this->modChoiceArray.data() + modChoiceIndex);
					}
				}
			}

			DetourXS hook_GetInventoryValue;
			typedef std::int64_t(GetInventoryValueSig)(TESBoundObject*, const ExtraDataList*);
			REL::Relocation<GetInventoryValueSig> GetInventoryValueOriginal;

			int64_t HookBGSInventoryItemUtilsGetInventoryValue(TESBoundObject* a_baseObj, const ExtraDataList* a_extra)
			{
				if (!a_extra->HasType(EXTRA_DATA_TYPE::kHealth))
				{
					return GetInventoryValueOriginal(a_baseObj, a_extra);
				}

				ExtraDataList* non_const_a_extra = const_cast<ExtraDataList*>(a_extra);
				int64_t newValue = int64_t(GetInventoryValueOriginal(a_baseObj, a_extra) * non_const_a_extra->GetHealthPerc() * std::sqrt(non_const_a_extra->GetHealthPerc()));
				return newValue;
			}

			DetourXS hook_AddItem;
			typedef void(AddItemSig)(BGSInventoryList*, TESBoundObject*, const BGSInventoryItem::Stack*, std::uint32_t*, std::uint32_t*);
			REL::Relocation<AddItemSig> AddItemOriginal;

			void HookBGSInventoryListAddItem(BGSInventoryList* a_this, TESBoundObject* a_boundObject, const BGSInventoryItem::Stack* a_stack, std::uint32_t* a_oldCount, std::uint32_t* a_newCount)
			{
				ENUM_FORM_ID formType = a_boundObject->GetFormType();
				if (a_boundObject)
				{
					if (formType == ENUM_FORM_ID::kWEAP || formType == ENUM_FORM_ID::kARMO)
					{
						std::uint32_t iterCount = 0;
						for (const BGSInventoryItem::Stack* traverse = a_stack; traverse; traverse->nextStack)
						{
							if (!traverse || !traverse->extra)
							{
								break;
							}

							// TODO - keyword check for no-degradation.

							if (formType == ENUM_FORM_ID::kWEAP)
							{
								TESObjectWEAP* tempREFR = static_cast<TESObjectWEAP*>(a_boundObject);
								if (tempREFR->weaponData.type == WEAPON_TYPE::kGrenade || tempREFR->weaponData.type == WEAPON_TYPE::kMine)
								{
									DEBUG("BGSInventoryList::AddItem: REFR grenade/mine weapon type.");
									break;
								}
							}

							// 'break' here works, as when native function gets called for weapons/armor stack only contains one object.
							if (traverse->extra.get())
							{
								// GetHealthPerc return -1.0 if it can't find the kHealth type.
								if (traverse->extra->GetHealthPerc() < 0)
								{
									traverse->extra->SetHealthPerc(BSRandom::Float(0.55f, 0.85f));
									INFO("BGSInventoryList::AddItem: Health initialized: {:s}", std::to_string(traverse->extra->GetHealthPerc()));
									break;
								}
								else
								{
									INFO("BGSInventoryList::AddItem: Health already initialized: {:s}", std::to_string(traverse->extra->GetHealthPerc()));
									break;
								}
							}

							iterCount++;

							if (iterCount > traverse->count - 1)
							{
								break;
							}
						}
					}
				}
				AddItemOriginal(a_this, a_boundObject, a_stack, a_oldCount, a_newCount);
			}

			
			DetourXS hook_ExamineMenuBuildConfirmed;
			typedef void(ExamineMenuBuildConfirmedSig)(ExamineMenu*, bool);
			REL::Relocation<ExamineMenuBuildConfirmedSig> ExamineMenuBuildConfirmedOriginal;

			void HookExamineMenuBuildConfirmed(ExamineMenu* a_this, bool a_ownerIsWorkbench)
			{
				if (a_this->repairing)
				{
					DEBUG("Repairing!");
				}

				ExamineMenuBuildConfirmedOriginal(a_this, a_ownerIsWorkbench);
			}

			// ========== REGISTERS ==========

			void RegisterGetBuildConfirmQuestion()
			{
				REL::Relocation<GetBuildConfirmQuestionSig> functionLocation{ REL::ID(2223057) };
				if (hook_GetBuildConfirmQuestion.Create(reinterpret_cast<LPVOID>(functionLocation.address()), &HookExamineMenuGetBuildConfirmQuestion))
				{
					DEBUG("Installed ExamineMenu::GetBuildConfirmQuestion hook.");
				}
				else
				{
					FATAL("Failed to hook ExamineMenu::GetBuildConfirmQuestion, exiting.");
				}
			}

			void RegisterShowBuildFailureMessage()
			{
				REL::Relocation<ShowBuildFailureMessageSig> functionLocation{ REL::ID(2224959) };
				if (hook_ShowBuildFailureMessage.Create(reinterpret_cast<LPVOID>(functionLocation.address()), &HookWorkbenchMenuBaseShowBuildFailureMessage))
				{
					DEBUG("Installed WorkbenchMenuBase::ShowBuildFailureMessage hook.");
				}
				else
				{
					FATAL("Failed to hook WorkbenchMenuBase::ShowBuildFailureMessage, exiting.");
				}
			}

			void RegisterSetHealthPercHook()
			{
				REL::Relocation<SetHealthPercSig> functionLocation{ REL::ID(2190124) };
				if (hook_SetHealthPerc.Create(reinterpret_cast<LPVOID>(functionLocation.address()), &HookExtraDataListSetHealthPerc))
				{
					DEBUG("Installed ExtraDataList::SetHealthPerc hook.");
					SetHealthPercOriginal = reinterpret_cast<uintptr_t>(hook_SetHealthPerc.GetTrampoline());
				}
				else
				{
					FATAL("Failed to hook ExtraDataList::SetHealthPerc, exiting.");
				}
			}

			void RegisterAddItemHook()
			{
				REL::Relocation<AddItemSig> functionLocation{ REL::ID(2194159) };
				if (hook_AddItem.Create(reinterpret_cast<LPVOID>(functionLocation.address()), &HookBGSInventoryListAddItem))
				{
					DEBUG("Installed BGSInventoryList::AddItem hook.");
					AddItemOriginal = reinterpret_cast<uintptr_t>(hook_AddItem.GetTrampoline());
				}
				else
				{
					FATAL("Failed to hook BGSInventoryList::AddItem, exiting.");
				}
			}

			void RegisterGetInventoryValueHook()
			{
				REL::Relocation<GetInventoryValueSig> functionLocation{ REL::ID(2194127) };
				if (hook_GetInventoryValue.Create(reinterpret_cast<LPVOID>(functionLocation.address()), &HookBGSInventoryItemUtilsGetInventoryValue))
				{
					DEBUG("Installed BGSInventoryItemUtils::GetInventoryValue hook.");
					GetInventoryValueOriginal = reinterpret_cast<uintptr_t>(hook_GetInventoryValue.GetTrampoline());
				}
				else
				{
					FATAL("Failed to hook BGSInventoryItemUtils::GetInventoryValue, exiting.");
				}
			}

			void RegisterQCurrentModChoiceData()
			{
				REL::Relocation<QCurrentModChoiceDataSig> functionLocation{ REL::ID(2224958) };
				if (hook_QCurrentModChoiceData.Create(reinterpret_cast<LPVOID>(functionLocation.address()), &HookWorkbenchMenuBaseQCurrentModChoiceData))
				{
					DEBUG("Installed WorkbenchMenuBase::QCurrentModChoiceData hook.");
				}
				else
				{
					FATAL("Failed to hook BGSInventoryItemUtils::GetInventoryValue, exiting.");
				}
			}

			void RegisterExamineMenuBuildConfirmed()
			{
				REL::Relocation<ExamineMenuBuildConfirmedSig> functionLocation{ REL::ID(2223013) };
				if (hook_ExamineMenuBuildConfirmed.Create(reinterpret_cast<LPVOID>(functionLocation.address()), &HookExamineMenuBuildConfirmed))
				{
					DEBUG("Installed ExamineMenu::BuildConfirmed hook.");
					ExamineMenuBuildConfirmedOriginal = reinterpret_cast<uintptr_t>(hook_ExamineMenuBuildConfirmed.GetTrampoline());
				}
				else
				{
					FATAL("Failed to hook ExamineMenu::BuildConfirmed, exiting.");
				}
			}
		}
	}
}
