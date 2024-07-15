#pragma once

#include "Shared/SharedFunctions.h"

namespace RE
{
	namespace Cascadia
	{
		namespace ExamineMenu
		{
			class hkOnButtonEvent
			{
			public:
				static void InstallHook()
				{
					static REL::Relocation<std::uintptr_t> vtbl{ RE::ExamineMenu::VTABLE[1] };
					_OnButtonEvent = vtbl.write_vfunc(0x8, reinterpret_cast<std::uintptr_t>(OnButtonEvent));
				}
			private:
				static void OnButtonEvent(RE::BSInputEventUser* a_this, const ButtonEvent* a_event)
				{
					if (a_event->QJustPressed() && (a_event->GetBSButtonCode() == BS_BUTTON_CODE::kK))
					{
						BSFixedString menuString("ExamineMenu");
						IMenu* menu = UI::GetSingleton()->GetMenu(menuString).get();
						Scaleform::GFx::Value a_params[4];

						a_params[0] = "test";
						a_params[1] = true;
						a_params[2] = a_event->device.underlying();
						a_params[3] = a_event->idCode;

						menu->uiMovie->asMovieRoot->Invoke("root.BaseInstance.ProcessUserEventExternal", nullptr, a_params, 4);
						return;
					}
					else
					{
						return _OnButtonEvent(a_this, a_event);
					}
				}

				inline static REL::Relocation<decltype(&OnButtonEvent)> _OnButtonEvent;
			};
			
			class EligableRepair : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					if (a_params.retVal)
					{
						*a_params.retVal = nullptr;
						DEBUG("EligableRepair called.");
						Scaleform::Ptr<RE::ExamineMenu> examineMenu = UI::GetSingleton()->GetMenu<RE::ExamineMenu>();

						std::uint32_t selectedIndex = examineMenu->GetSelectedIndex();
						if (!examineMenu->invInterface.entriesInvalid || (selectedIndex & 0x80000000) == 0 || selectedIndex < examineMenu->invInterface.stackedEntries.size())
						{
							InventoryUserUIInterfaceEntry* inventoryUUIEntry = (examineMenu->invInterface.stackedEntries.data() + selectedIndex);
							const BGSInventoryItem* inventoryItem = BGSInventoryInterface::GetSingleton()->RequestInventoryItem(inventoryUUIEntry->invHandle.id);

							if (inventoryItem)
							{
								switch (inventoryItem->object->formType.underlying())
								{
									case static_cast<std::uint32_t>(ENUM_FORM_ID::kARMO):
									case static_cast<std::uint32_t>(ENUM_FORM_ID::kWEAP):										
										*a_params.retVal = inventoryItem->stackData.get()->extra->GetHealthPerc() != -1.0;
										break;
									default:
										*a_params.retVal = false;
										break;
								}
							}
						}				
					}
				}
			};

			class RepairKitCount : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("RepairKitCount called.");
					TESObjectMISC* repairKit = TESDataHandler::GetSingleton()->LookupForm<TESObjectMISC>(0x1D59F7, "FalloutCascadia.esm");
					std::uint32_t itemCount;
					Cascadia::GetPlayerCharacter()->GetItemCount(itemCount, repairKit, 0);
					*a_params.retVal = itemCount;
				}
			};

			class HasRepairKits : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("HasRepairKits called.");
					TESObjectMISC* repairKit = TESDataHandler::GetSingleton()->LookupForm<TESObjectMISC>(0x1D59F7, "FalloutCascadia.esm");
					std::uint32_t itemCount;
					Cascadia::GetPlayerCharacter()->GetItemCount(itemCount, repairKit, 0);
					*a_params.retVal = (itemCount > 0);
				}
			};

			class RepairNotNeeded : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("RepairNotNeeded called.");
					SendHUDMessage::ShowHUDMessage("$CAS_NoNeedToRepair", 0, true, true);
				}
			};

			class NoRepairKits : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("NoRepairKits called.");
					SendHUDMessage::ShowHUDMessage("$CAS_NoRepairKits", 0, true, true);
				}
			};

			class RepairFunction : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					if (a_params.retVal)
					{
						DEBUG("RepairFunction called.");
						Scaleform::Ptr<RE::ExamineMenu> examineMenu = UI::GetSingleton()->GetMenu<RE::ExamineMenu>();

						std::uint32_t selectedIndex = examineMenu->GetSelectedIndex();
						if (!examineMenu->invInterface.entriesInvalid || (selectedIndex & 0x80000000) == 0 || selectedIndex < examineMenu->invInterface.stackedEntries.size())
						{
							InventoryUserUIInterfaceEntry* inventoryUUIEntry= (examineMenu->invInterface.stackedEntries.data() + selectedIndex);
							const BGSInventoryItem* inventoryItem = BGSInventoryInterface::GetSingleton()->RequestInventoryItem(inventoryUUIEntry->invHandle.id);
							if (inventoryItem)
							{
								TESObjectMISC* repairKit = TESDataHandler::GetSingleton()->LookupForm<TESObjectMISC>(0x1D59F7, "FalloutCascadia.esm");	
								TESObjectREFR::RemoveItemData removeItemData{ repairKit, 1 };
								Cascadia::GetPlayerCharacter()->RemoveItem(removeItemData);

								switch (inventoryItem->object->formType.underlying())
								{
									case static_cast<std::uint32_t>(ENUM_FORM_ID::kARMO):
									case static_cast<std::uint32_t>(ENUM_FORM_ID::kWEAP):
										if (inventoryItem->stackData.get()->extra->GetHealthPerc() + 0.5f >= 1.0f)
										{
											inventoryItem->stackData.get()->extra->SetHealthPerc(0.999f);
										}
										else
										{
											inventoryItem->stackData.get()->extra->SetHealthPerc(inventoryItem->stackData.get()->extra->GetHealthPerc() + 0.5f);
										}
										break;
									default:
										break;
								}
							}
						}				
					}
				}
			};

			class NeedsRepair : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)	
				{
					DEBUG("NeedsRepair called.");
					*a_params.retVal = nullptr;
					Scaleform::Ptr<RE::ExamineMenu> examineMenu = UI::GetSingleton()->GetMenu<RE::ExamineMenu>();

					std::uint32_t selectedIndex = examineMenu->GetSelectedIndex();
					if (!examineMenu->invInterface.entriesInvalid || (selectedIndex & 0x80000000) == 0 || selectedIndex < examineMenu->invInterface.stackedEntries.size())
					{
						InventoryUserUIInterfaceEntry* inventoryUUIEntry= (examineMenu->invInterface.stackedEntries.data() + selectedIndex);
						const BGSInventoryItem* inventoryItem = BGSInventoryInterface::GetSingleton()->RequestInventoryItem(inventoryUUIEntry->invHandle.id);

						if (inventoryItem)
						{
							float tempCurrentPerc;

							switch (inventoryItem->object->formType.underlying())
							{
								case static_cast<std::uint32_t>(ENUM_FORM_ID::kARMO):
								case static_cast<std::uint32_t>(ENUM_FORM_ID::kWEAP):
									tempCurrentPerc = inventoryItem->stackData.get()->extra->GetHealthPerc();

									// Temporarily bump condition by 0.001 to avoid wasting a repair kit on effectively non-existant damage.
									inventoryItem->stackData.get()->extra->SetHealthPerc(tempCurrentPerc + 0.001);
									*a_params.retVal = inventoryItem->stackData.get()->extra->IsDamaged();
									// Restore condition back to original.
									inventoryItem->stackData.get()->extra->SetHealthPerc(tempCurrentPerc);
									break;
								default:										
									*a_params.retVal = false;
									break;
								}
							}
						}
				}
			};

			bool RegisterScaleform(Scaleform::GFx::Movie* a_view, Scaleform::GFx::Value* a_value)
			{
				Scaleform::GFx::Value currentSWFPath;
				// Register native code handlers.
				if (a_view->asMovieRoot->GetVariable(&currentSWFPath, "root.loaderInfo.url"))
				{
					if (_stricmp(currentSWFPath.GetString(), "Interface/ExamineMenu.swf") == 0)
					{
						Scaleform::GFx::Value bgsCodeObj;
						a_view->asMovieRoot->GetVariable(&bgsCodeObj, "root.BaseInstance.BGSCodeObj");

						RegisterFunction<NoRepairKits>(&bgsCodeObj, a_view->asMovieRoot, "NoRepairKits");
						RegisterFunction<RepairNotNeeded>(&bgsCodeObj, a_view->asMovieRoot, "RepairNotNeeded");
						RegisterFunction<EligableRepair>(&bgsCodeObj, a_view->asMovieRoot, "EligableRepair");
						RegisterFunction<RepairKitCount>(&bgsCodeObj, a_view->asMovieRoot, "repairKitCount");
						RegisterFunction<HasRepairKits>(&bgsCodeObj, a_view->asMovieRoot, "hasRepairKits");
						RegisterFunction<NeedsRepair>(&bgsCodeObj, a_view->asMovieRoot, "needsRepair");
						RegisterFunction<RepairFunction>(&bgsCodeObj, a_view->asMovieRoot, "CASRepairItem");
					}
					return true;
				}
				return false;
			}
		}
	}
}