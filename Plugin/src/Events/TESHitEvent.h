#pragma once

#include "RE/Bethesda/BGSBodyPartDefs.h"

namespace RE
{
	namespace Cascadia
	{
		class TESHitEventWatcher :
			public BSTEventSink<TESHitEvent>
		{
			virtual BSEventNotifyControl ProcessEvent(const TESHitEvent& a_event, BSTEventSource<TESHitEvent>*) override
			{
				TESObjectREFR* target = a_event.target.get();

				// Return early on invalid events.
				// Note: Unsure about the logic RE Power Armor here.
				if (!target || !a_event.usesHitData || target->extraList->GetByType(kPowerArmor) != nullptr)
				{
					return BSEventNotifyControl::kContinue;
				}

				// Return if 'target' is not a 'Actor'.
				if (target->formType != ENUM_FORM_ID::kACHR)
				{
					DEBUG("TESHitEvent: 'target' was not of type 'Actor'.");
					return BSEventNotifyControl::kContinue;
				}

				Actor* hitActor = (Actor*)target;

				// Extra power armor check.
				if (PowerArmor::ActorInPowerArmor(*hitActor))
				{
					return BSEventNotifyControl::kContinue;
				}

				if (hitActor != PlayerCharacter::GetSingleton())
				{
					TESObjectREFR* cause = a_event.cause.get();
					if (cause != nullptr)
					{
						if (cause->formType == ENUM_FORM_ID::kACHR)
						{
							Actor* causeActor = (Actor*)cause;

							// Here we also handle weapon degradation for melee attacks - including weapon bashing.
							if (causeActor == PlayerCharacter::GetSingleton())
							{
								PlayerCharacter* playerCharacter = (PlayerCharacter*)causeActor;
								TESObjectREFR* sourceREFR = a_event.hitData.sourceRef.get().get();
								if (!sourceREFR)
								{
									// If sourceREFR is 'nullptr' it's a melee attack.
									TESFormID weaponFormID = a_event.hitData.weapon.object->GetFormID();
									BGSInventoryItem* inventoryItem = nullptr;

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
										EquippedItem& equippedWeapon = playerCharacter->currentProcess->middleHigh->equippedItems[0];
										ExtraDataList* extraDataList = inventoryItem->stackData->extra.get();

										float currentHealth = extraDataList->GetHealthPerc();

										TESObjectWEAP* weapon = (TESObjectWEAP*)a_event.hitData.weapon.object;

										float newHealth;

										// Random amount if melee weapon, 'else' statement handles weapon bashing.
										if (weapon->weaponData.type != WEAPON_TYPE::kGun)
										{
											newHealth = std::max(currentHealth - BSRandom::Float(0.005, 0.015), 0.0f);
										}
										else
										{
											newHealth = std::max(currentHealth - 0.015f, 0.0f); // Always 1.5% degradation when weapon bashing.
										}

										if (newHealth == 0.0f)
										{
											ActorEquipManager::GetSingleton()->UnequipItem(playerCharacter, &equippedWeapon, false);
											GameSettingCollection* gameSettingCollection = GameSettingCollection::GetSingleton();
											SendHUDMessage::ShowHUDMessage(gameSettingCollection->GetSetting("sWeaponBreak")->GetString().data(), "UIWorkshopModeItemScrapGeneric", true, true);
										}
										extraDataList->SetHealthPerc(newHealth);
										PipboyDataManager::GetSingleton()->inventoryData.RepopulateItemCardsOnSection(ENUM_FORM_ID::kWEAP);
									}
								}
							}
						}
					}

					return BSEventNotifyControl::kContinue;
				}

				TESObjectREFR* sourceREFR = a_event.hitData.sourceRef.get().get();
				BGSBodyPartData* bodyPartData = hitActor->GetBodyPartData();

				// If sourceREFR is 'nullptr', most likely a melee attack.
				if (sourceREFR)
				{
					if (sourceREFR->IsExplosion())
					{
						// TODO
					}
					else
					{
						std::uint32_t damageLimb = a_event.hitData.damageLimb.underlying();
						if (damageLimb != std::uint32_t(BGSBodyPartDefs::LIMB_ENUM::kWeapon) && damageLimb != std::uint32_t(BGSBodyPartDefs::LIMB_ENUM::kNone))
						{
							DEBUG("No explosion - limb hit: {}", a_event.hitData.damageLimb.underlying());
						}
					}
				}
				else
				{
					std::uint32_t damageLimb = a_event.hitData.damageLimb.underlying();
					if (damageLimb != std::uint32_t(BGSBodyPartDefs::LIMB_ENUM::kWeapon) && damageLimb != std::uint32_t(BGSBodyPartDefs::LIMB_ENUM::kNone))
					{
						// Weapons like the 'Ashmaker' only do physical damage (unclear as to why), take that into account here.
						float damage = std::max(a_event.hitData.healthDamage, a_event.hitData.physicalDamage);

						DEBUG("No explosion - limb hit: {}", a_event.hitData.damageLimb.underlying());
					}
				}

				return BSEventNotifyControl::kContinue;
			}
		};

		void RegisterTESHitEventSink()
		{
			TESHitEventWatcher* tesHitEvent = new TESHitEventWatcher();
			TESHitEvent::GetEventSource()->RegisterSink(tesHitEvent);
			INFO("Registered 'TESHitEvent' sink.");
		}
	}
}