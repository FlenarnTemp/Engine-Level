#include "AmmoSwitch.h"

namespace RE
{
	namespace Cascadia
	{
		namespace AmmoSwitch
		{
			bool switchingAmmo = false;

			BSTArray<BGSKeyword*> keywordsAmmo;
			std::unordered_map<BGSKeyword*, BGSListForm*> keywordFormlistMap;
			BGSKeyword* noFormlistWEAP;
			BGSKeyword* uniqueFormlistWEAP;

			bool InitializeAmmoSwitch()
			{
				// Return true to make sure proper menu audio is played if ammo switch is successfully initiated, otherwise false return.
				BGSObjectInstance* equippedInstance = new BGSObjectInstance(nullptr, nullptr);
				PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();
				playerCharacter->GetEquippedItem(equippedInstance, BGSEquipIndex{ 0 });

				if (equippedInstance->object && equippedInstance->object->formType == ENUM_FORM_ID::kWEAP && static_cast<TESObjectWEAP*>(equippedInstance->object)->weaponData.type == WEAPON_TYPE::kGun)
				{
					TESObjectWEAP* weapon = static_cast<TESObjectWEAP*>(equippedInstance->object);
					TESObjectWEAP::InstanceData* instanceDataWEAP = (TESObjectWEAP::InstanceData*)(equippedInstance->instanceData.get());
					const char* standardListPrefix = "CAS_AmmoSwitch_Standard_";
					const char* uniqueListPrefix = "CAS_AmmoSwitch_Unique_";

					// Junkjet and other very unique weapons in terms of ammunition.
					if (weapon->HasKeyword(noFormlistWEAP))
					{
						DEBUG("'AmmoSwitch::InitializeAmmoSwitch' - weapon '{}' is flagged for no ammo switching.", weapon->GetFormEditorID());
						return false;
					}

					TESAmmo* currentAmmo = instanceDataWEAP->ammo;
					if (currentAmmo == nullptr)
					{
						// Just in case.
						return false;
					}

					if (weapon->HasKeyword(uniqueFormlistWEAP))
					{
						std::uint32_t keywordCount = weapon->GetNumKeywords();
						if (keywordCount > 0)
						{
							bool weaponFoundKeyword = false;
							for (std::uint32_t i = 0; i < keywordCount; ++i)
							{
								std::optional<BGSKeyword*> bgsKeyword = weapon->GetKeywordAt(i);
								if (bgsKeyword.has_value())
								{
									const char* formEditorID = bgsKeyword.value()->GetFormEditorID();
									if (strncmp(formEditorID, uniqueListPrefix, strlen(uniqueListPrefix)) == 0)
									{
										weaponFoundKeyword = true;
										auto mapEntry = keywordFormlistMap.find(bgsKeyword.value());
										if (mapEntry != keywordFormlistMap.end())
										{
											BGSListForm* formList = mapEntry->second;
											std::uint32_t formListSize = formList->arrayOfForms.size();
											std::uint32_t index = 0;
											for (index; index < formListSize; ++index)
											{
												if (formList->arrayOfForms.begin()[index] == (TESForm*)currentAmmo)
												{
													DEBUG("'AmmoSwitch::InitializeAmmoSwitch' - current ammo position in found formlist: {}.", index);
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

												if (!hasAmmoInFormlist)
												{
													return false;
												}
											}
											else
											{
												return false;
											}
										}
										else
										{
											FATAL("'AmmoSwitch::InitializeAmmoSwitch' - keyword not found in 'keywordFormlistMap'.");
										}
									}
									
								}
							}
							if (!weaponFoundKeyword)
							{
								DEBUG("'AmmoSwitch::InitializeAmmoSwitch' - weapon: {} flagged for unique ammo switch formlist, no matching keyword found.", weapon->GetFormEditorID());
								return false;
							}
						}
						else
						{
							return false;
						}
					}
					else
					{
						std::uint32_t keywordCount = currentAmmo->GetNumKeywords();
						if (keywordCount > 0)
						{
							bool ammoFoundKeyword = false;
							for (std::uint32_t i = 0; i < keywordCount; ++i)
							{
								std::optional<BGSKeyword*> bgsKeyword = currentAmmo->GetKeywordAt(i);
								if (bgsKeyword.has_value())
								{
									const char* formEditorID = bgsKeyword.value()->GetFormEditorID();
									if (strncmp(formEditorID, standardListPrefix, strlen(standardListPrefix)) == 0)
									{
										ammoFoundKeyword = true;
										auto mapEntry = keywordFormlistMap.find(bgsKeyword.value());
										if (mapEntry != keywordFormlistMap.end())
										{
											BGSListForm* formList = mapEntry->second;
											std::uint32_t formListSize = formList->arrayOfForms.size();
											std::uint32_t index = 0;
											for (index = 0; index < formListSize; ++index)
											{
												if (formList->arrayOfForms.begin()[index] == (TESForm*)currentAmmo)
												{
													DEBUG("'AmmoSwitch::InitializeAmmoSwitch' - current ammo position in found formlist: {}.", index);
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

												if (!hasAmmoInFormlist)
												{
													return false;
												}
											}
											else
											{
												return false;
											}
										}
										else
										{
											FATAL("'AmmoSwitch::InitializeAmmoSwitch' - keyword not found in 'keywordFormlistMap'.");
										}
									}
								}
							}
							if (!ammoFoundKeyword)
							{
								DEBUG("'AmmoSwitch::InitializeAmmoSwitch' - ammo: {} no keyword matching 'CAS_AmmoSwitch_Standard_X' standard found.", currentAmmo->GetFormEditorID());
								return false;
							}
						}
						else
						{
							return false;
						}
					}

					if (playerCharacter->NotifyAnimationGraphImpl("reloadStart")) // Fails if no weapon is drawn, amongst other things.
					{
						switchingAmmo = true;
					}
					else
					{
						return false;
					}
					return true;
				}
				return false;
			}

			void DefineAmmoLists()
			{
				TESDataHandler* dataHandler = TESDataHandler::GetSingleton();
				BSTArray<TESForm*> ammoEntries = dataHandler->formArrays[std::to_underlying(ENUM_FORM_ID::kAMMO)];
				BSTArray<TESForm*> keywordEntries = dataHandler->formArrays[std::to_underlying(ENUM_FORM_ID::kKYWD)];
				noFormlistWEAP = dataHandler->LookupForm<BGSKeyword>(0x2D9AB8, "FalloutCascadia.esm");
				uniqueFormlistWEAP = dataHandler->LookupForm<BGSKeyword>(0x2D9AB9, "FalloutCascadia.esm");
				
				const char* standardListPrefix = "CAS_AmmoSwitch_Standard_";
				const char* uniqueListPrefix = "CAS_AmmoSwitch_Unique_";

				for (TESForm* tesForm : keywordEntries)
				{
					const char* formEditorID = tesForm->GetFormEditorID();

					if (strncmp(formEditorID, standardListPrefix, strlen(standardListPrefix)) == 0 || strncmp(formEditorID, uniqueListPrefix, strlen(uniqueListPrefix)) == 0)
					{
						DEBUG("'AmmoSwitch::DefineAmmoLists' - matching keyword: {}, added to 'keywordsAmmo'.", formEditorID);
						keywordsAmmo.push_back((BGSKeyword*)tesForm);
						keywordFormlistMap[(BGSKeyword*)tesForm] = new BGSListForm;
					}
				}				
				for (TESForm* tesForm : ammoEntries)
				{
					TESAmmo* tesAMMO = static_cast<TESAmmo*>(tesForm);

					std::uint32_t keywordCount = tesAMMO->GetNumKeywords();

					if (keywordCount > 0)
					{
						for (std::uint32_t i = 0; i < keywordCount; ++i)
						{
							std::optional<BGSKeyword*> bgsKeyword = tesAMMO->GetKeywordAt(i);
							if (bgsKeyword.has_value())
							{
								const char* formEditorID = bgsKeyword.value()->GetFormEditorID();
								if (strncmp(formEditorID, standardListPrefix, strlen(standardListPrefix)) == 0 || strncmp(formEditorID, uniqueListPrefix, strlen(uniqueListPrefix)) == 0)
								{
									DEBUG("'AmmoSwitch::DefineAmmoLists' - found keyword: {}, on ammo: {}.", bgsKeyword.value()->GetFormEditorID(), tesForm->GetFormEditorID());
									auto mapEntry = keywordFormlistMap.find(bgsKeyword.value());
									if (mapEntry != keywordFormlistMap.end())
									{
										BGSListForm* formList = mapEntry->second;
										formList->arrayOfForms.push_back(tesForm);
									}
									else
									{
										FATAL("'AmmoSwitch::DefineAmmoLists' - keyword '{}' not found in 'keywordFormlistMap'.", formEditorID);
									}	
								}
							}
						}
					}
				}
				DEBUG("'AmmoSwitch::DefineAmmoLists' - 'keywordFormlistMap' size: {}", keywordFormlistMap.size());
			}

			void PostLoadGameAmmoFix()
			{
				PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();
				BGSObjectInstance* equippedInstance = new BGSObjectInstance(nullptr, nullptr);
				playerCharacter->GetEquippedItem(equippedInstance, BGSEquipIndex{ 0 });
				if (equippedInstance->object && equippedInstance->object->formType == ENUM_FORM_ID::kWEAP && static_cast<TESObjectWEAP*>(equippedInstance->object)->weaponData.type == WEAPON_TYPE::kGun)
				{
					TESAmmo* currentAmmoInPlayerMemory = playerCharacter->GetCurrentAmmo(BGSEquipIndex{ 0 });
					TESObjectWEAP::InstanceData* instanceDataWEAP = (TESObjectWEAP::InstanceData*)(equippedInstance->instanceData.get());
					TESAmmo* currentAmmoInWeaponInstance = instanceDataWEAP->ammo;

					if (currentAmmoInPlayerMemory != currentAmmoInWeaponInstance)
					{
						DEBUG("'AmmoSwitch::PostLoadGameAmmoFix' - adjusted weapon instance ammo from: {}, to: {}", currentAmmoInWeaponInstance->GetFormEditorID(), currentAmmoInPlayerMemory->GetFormEditorID());
						instanceDataWEAP->ammo = currentAmmoInPlayerMemory;
						PipboyDataManager::GetSingleton()->inventoryData.RepopulateItemCardsOnSection(ENUM_FORM_ID::kWEAP);
					}
				}
			}
		}
	}
}