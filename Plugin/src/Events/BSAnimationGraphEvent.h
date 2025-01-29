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
					DEBUG("'BSEventNotifyControl::ProcessEvent' - 'ReloadComplete'.");

					BGSObjectInstanceT<TESObjectWEAP>* weaponInstance = new BGSObjectInstanceT<TESObjectWEAP>{ static_cast<TESObjectWEAP*>(AmmoSwitch::equippedInstance->object), AmmoSwitch::equippedInstance->instanceData.get() };
					TESObjectWEAP::InstanceData* instanceDataWEAP = (TESObjectWEAP::InstanceData*)(weaponInstance->instanceData.get());

					if (AmmoSwitch::ammoToSwitchTo)
					{
						instanceDataWEAP->ammo = AmmoSwitch::ammoToSwitchTo;
						PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();
						playerCharacter->currentProcess->SetCurrentAmmo(BGSEquipIndex{ 0 }, AmmoSwitch::ammoToSwitchTo);
						playerCharacter->SetCurrentAmmoCount(BGSEquipIndex{ 0 }, 0);
						(Actor*)playerCharacter->ReloadWeapon(weaponInstance, BGSEquipIndex{ 0 });
						PipboyDataManager::GetSingleton()->inventoryData.RepopulateItemCardsOnSection(ENUM_FORM_ID::kWEAP);
					}
						
					AmmoSwitch::switchingAmmo = false;
					AmmoSwitch::ammoToSwitchTo = nullptr;
					AmmoSwitch::equippedInstance = nullptr;
				}

				if (a_event.tag == reloadStateExit && AmmoSwitch::switchingAmmo == true)
				{
					DEBUG("'BSEventNotifyControl::ProcessEvent' - 'reloadStateExit'.");
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