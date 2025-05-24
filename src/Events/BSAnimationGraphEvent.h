#pragma once

#include "Shared/SharedFunctions.h"
#include "Systems/AmmoSwitch.h"

// Thanks to 'Jajari' and 'Pinedog' for assistance with this code.

namespace RE
{
	namespace Cascadia
	{
		using std::unordered_map;
		class BSAnimationGraphEventWatcher
		{
		public:
			typedef BSEventNotifyControl(BSAnimationGraphEventWatcher::* FnProcessEvent)(BSAnimationGraphEvent& a_event, BSTEventSource<BSAnimationGraphEvent>* a_source);

			BSEventNotifyControl ProcessEvent(BSAnimationGraphEvent& a_event, BSTEventSource<BSAnimationGraphEvent>* a_source)
			{
				const BSFixedString ReloadComplete("ReloadComplete");
				const BSFixedString reloadStateExit("reloadStateExit");

				if (a_event.tag == ReloadComplete && AmmoSwitch::switchingAmmo == true)
				{
					REX::DEBUG("'BSEventNotifyControl::ProcessEvent' - 'ReloadComplete'.");

					BGSObjectInstanceT<TESObjectWEAP>* weaponInstance = new BGSObjectInstanceT<TESObjectWEAP>{ static_cast<TESObjectWEAP*>(AmmoSwitch::equippedInstance->object), AmmoSwitch::equippedInstance->instanceData.get() };
					TESObjectWEAP::InstanceData* instanceDataWEAP = (TESObjectWEAP::InstanceData*)(weaponInstance->instanceData.get());

					if (AmmoSwitch::ammoToSwitchTo)
					{
						PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();

						BGSInventoryItem* inventoryItem = nullptr;
						TESObjectWEAP* tesWEAP = (TESObjectWEAP*)AmmoSwitch::equippedInstance->object;
						TESFormID weaponFormID = tesWEAP->GetFormID();
						for (BGSInventoryItem& item : playerCharacter->inventoryList->data)
						{
							if (item.object->GetFormID() == weaponFormID)
							{
								inventoryItem = &item;
								break;
							}
						}

						if (inventoryItem)
						{
							REX::DEBUG("Inventory item found.");
							BGSMod::Attachment::Mod* modTest = TESDataHandler::GetSingleton()->LookupForm<BGSMod::Attachment::Mod>(0x0009C2, "CAS_Jimswapping.esp");
							//BGSInventoryItem::Stack* stack = inventoryItem->GetStackByID(0);

							bool test = false;
							BGSInventoryItem::CheckStackIDFunctor compareFunction(0);
							BGSInventoryItem::ModifyModDataFunctor writeDataFunction(modTest, 0, true, &test);
							REX::DEBUG("Managed to write new mod data: {}", test);
							playerCharacter->FindAndWriteStackDataForInventoryItem(tesWEAP, compareFunction, writeDataFunction);
							playerCharacter->currentProcess->SetCurrentAmmo(BGSEquipIndex{ 0 }, AmmoSwitch::ammoToSwitchTo);
							playerCharacter->SetCurrentAmmoCount(BGSEquipIndex{ 0 }, 0);
							(Actor*)playerCharacter->ReloadWeapon(weaponInstance, BGSEquipIndex{ 0 });
						}
						
							
						/**instanceDataWEAP->ammo = AmmoSwitch::ammoToSwitchTo;
						PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();
						playerCharacter->currentProcess->SetCurrentAmmo(BGSEquipIndex{ 0 }, AmmoSwitch::ammoToSwitchTo);
						playerCharacter->SetCurrentAmmoCount(BGSEquipIndex{ 0 }, 0);
						(Actor*)playerCharacter->ReloadWeapon(weaponInstance, BGSEquipIndex{ 0 });
						PipboyDataManager::GetSingleton()->inventoryData.RepopulateItemCardsOnSection(ENUM_FORM_ID::kWEAP);*/
					}
						
					AmmoSwitch::switchingAmmo = false;
					AmmoSwitch::ammoToSwitchTo = nullptr;
					AmmoSwitch::equippedInstance = nullptr;
				}

				if (a_event.tag == reloadStateExit && AmmoSwitch::switchingAmmo == true)
				{
					REX::DEBUG("'BSEventNotifyControl::ProcessEvent' - 'reloadStateExit'.");
					AmmoSwitch::switchingAmmo = false;
					AmmoSwitch::ammoToSwitchTo = nullptr;
					AmmoSwitch::equippedInstance = nullptr;
				}

				FnProcessEvent fn = fnHash.at(*(uint64_t*)this);
				return fn ? (this->*fn)(a_event, a_source) : BSEventNotifyControl::kContinue;
			}

			void RegisterBSAnimationGraphEventSink()
			{
				std::uint64_t vtable = *(std::uint64_t*)this;
				auto it = fnHash.find(vtable);
				if (it == fnHash.end())
				{
					FnProcessEvent fn = SafeWrite64Function(vtable + 0x8, &BSAnimationGraphEventWatcher::ProcessEvent);
					fnHash.insert(std::pair<std::uint64_t, FnProcessEvent>(vtable, fn));
				}
			}

			static void Install()
			{
				((BSAnimationGraphEventWatcher*)((uint64_t)PlayerCharacter::GetSingleton() + 0x38))->RegisterBSAnimationGraphEventSink();
			}


		protected:
			static std::unordered_map<std::uint64_t, FnProcessEvent> fnHash;
		};
		std::unordered_map<std::uint64_t, BSAnimationGraphEventWatcher::FnProcessEvent> BSAnimationGraphEventWatcher::fnHash;
	}
}