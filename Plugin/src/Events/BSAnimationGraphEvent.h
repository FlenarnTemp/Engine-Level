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
					BGSObjectInstance* equippedInstance = new BGSObjectInstance(nullptr, nullptr);
					PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();

					playerCharacter->GetEquippedItem(equippedInstance, BGSEquipIndex{ 0 });

					if (equippedInstance->object && equippedInstance->object->formType == ENUM_FORM_ID::kWEAP && static_cast<TESObjectWEAP*>(equippedInstance->object)->weaponData.type == WEAPON_TYPE::kGun)
					{
						BGSObjectInstanceT<TESObjectWEAP>* weaponInstance = new BGSObjectInstanceT<TESObjectWEAP>{ static_cast<TESObjectWEAP*>(equippedInstance->object), equippedInstance->instanceData.get() };
						TESObjectWEAP::InstanceData* instanceDataWEAP = (TESObjectWEAP::InstanceData*)(weaponInstance->instanceData.get());
						TESObjectWEAP* weapon = static_cast<TESObjectWEAP*>(equippedInstance->object);
						const char* standardListPrefix = "CAS_AmmoSwitch_Standard_";
						const char* uniqueListPrefix = "CAS_AmmoSwitch_Unique_";

						TESAmmo* currentAmmo = instanceDataWEAP->ammo;

						if (weapon->HasKeyword(AmmoSwitch::uniqueFormlistWEAP))
						{

						}
						else
						{
							std::uint32_t keywordCount = currentAmmo->GetNumKeywords();
							if (keywordCount > 0)
							{
								for (std::uint32_t i = 0; i < keywordCount; ++i)
								{
									std::optional<BGSKeyword*> bgsKeyword = currentAmmo->GetKeywordAt(i);
									if (bgsKeyword.has_value())
									{
										const char* formEditorID = bgsKeyword.value()->GetFormEditorID();
										if (strncmp(formEditorID, standardListPrefix, strlen(standardListPrefix)) == 0)
										{
											auto mapEntry = AmmoSwitch::keywordFormlistMap.find(bgsKeyword.value());
											if (mapEntry != AmmoSwitch::keywordFormlistMap.end())
											{
												BGSListForm* formList = mapEntry->second;
												std::uint32_t formListSize = formList->arrayOfForms.size();
												std::uint32_t index = 0;
												for (index = 0; index < formListSize; ++index)
												{
													if (formList->arrayOfForms.begin()[index] == (TESForm*)currentAmmo)
													{
														break;
													}
												}

												if (formListSize != 0)
												{
													bool hasAmmoInFormlist = false;
													std::uint32_t firstFoundIndex = -1;
													for (std::uint32_t i = 1; i < formListSize; i++)
													{
														std::uint32_t currentIndex = (index + i) % formListSize;
														TESBoundObject* a_object = (TESBoundObject*)formList->arrayOfForms[currentIndex];
														if (playerCharacter->GetInventoryObjectCount(a_object) != 0)
														{
															firstFoundIndex = currentIndex;
															hasAmmoInFormlist = true;
														}
													}

													if (hasAmmoInFormlist)
													{
														TESAmmo* ammoToSwitchTo = (TESAmmo*)formList->arrayOfForms[firstFoundIndex]; 

														// TESTING CODE STARTS HERE
														if (currentAmmo->shellCasing.model != ammoToSwitchTo->shellCasing.model)
														{
															DEBUG("'BSEventNotifyControl::ProcessEvent' - ammo should impact material swap for ammo.");
															BGSMaterialSwap* test = currentAmmo->swapForm;
															if (test)
															{
																DEBUG("matswap name: {}", test->GetFormEditorID());
																DEBUG("test print: {}", test->swapMap.size());
																
															}
														}
														// TESTING CODE ENDS HERE

														instanceDataWEAP->ammo = ammoToSwitchTo;

														playerCharacter->currentProcess->SetCurrentAmmo(BGSEquipIndex{ 0 }, ammoToSwitchTo);
														playerCharacter->SetCurrentAmmoCount(BGSEquipIndex{ 0 }, 0);
														(Actor*)playerCharacter->ReloadWeapon(weaponInstance, BGSEquipIndex{ 0 });
														PipboyDataManager::GetSingleton()->inventoryData.RepopulateItemCardsOnSection(ENUM_FORM_ID::kWEAP);
													}
													else
													{
														FATAL("'BSEventNotifyControl::ProcessEvent' - !hasAmmoInFormList");
														AmmoSwitch::switchingAmmo = false;
														FnProcessEvent fn = fnHash.at(*(uint64_t*)this);
														return fn ? (this->*fn)(a_event, a_source) : BSEventNotifyControl::kContinue;
													}
												}
											}
										}
									}
								}
								
							}
						}
					}
					AmmoSwitch::switchingAmmo = false;
					FnProcessEvent fn = fnHash.at(*(uint64_t*)this);
					return fn ? (this->*fn)(a_event, a_source) : BSEventNotifyControl::kContinue;
				}

				if (a_event.tag == reloadStateExit && AmmoSwitch::switchingAmmo == true)
				{
					DEBUG("'BSEventNotifyControl::ProcessEvent' - 'reloadStateExit'.");
					AmmoSwitch::switchingAmmo = false;
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