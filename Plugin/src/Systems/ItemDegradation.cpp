#include "ItemDegradation.h"

namespace RE
{
	namespace Cascadia
	{
		// Weapon Condition - Manual Settings
		float fDamageSkillMult;
		float fDamageSkillBase;
		float fAVDMeleeDamageStrengthMult;
		float fAVDMeleeDamageStrengthOffset;

		// Weapon Condition - Rate of Fire Game Settings
		Setting* fWeaponConditionRateOfFire1;
		Setting* fWeaponConditionRateOfFire2;
		Setting* fWeaponConditionRateOfFire3;
		Setting* fWeaponConditionRateOfFire4;
		Setting* fWeaponConditionRateOfFire5;
		Setting* fWeaponConditionRateOfFire6;
		Setting* fWeaponConditionRateOfFire7;
		Setting* fWeaponConditionRateOfFire8;
		Setting* fWeaponConditionRateOfFire9;
		Setting* fWeaponConditionRateOfFire10;

		// Power Armor Keyword
		BGSKeyword* furnitureTypePowerArmor;

		// Armor Type Keywords
		BGSKeyword* armorTypePowerArmor;
		BGSKeyword* armorBodyPartChest;
		BGSKeyword* armorBodyPartHead;
		BGSKeyword* armorBodyPartLeftArm;
		BGSKeyword* armorBodyPartRightArm;
		BGSKeyword* armorBodyPartLeftLeg;
		BGSKeyword* armorBodyPartRightLeg;

		void DefineItemDegradationFormsFromGame()
		{
			INFO("Item Degradation: Linking degradation forms.");

			fDamageSkillBase = 0.5f;
			fDamageSkillMult = 0.5f;
			fAVDMeleeDamageStrengthMult = 0.5f;
			fAVDMeleeDamageStrengthOffset = 0.0f;

			fWeaponConditionRateOfFire1 = GetGMST("fWeaponConditionRateOfFire1");
			fWeaponConditionRateOfFire2 = GetGMST("fWeaponConditionRateOfFire2");
			fWeaponConditionRateOfFire3 = GetGMST("fWeaponConditionRateOfFire3");
			fWeaponConditionRateOfFire4 = GetGMST("fWeaponConditionRateOfFire4");
			fWeaponConditionRateOfFire5 = GetGMST("fWeaponConditionRateOfFire5");
			fWeaponConditionRateOfFire6 = GetGMST("fWeaponConditionRateOfFire6");
			fWeaponConditionRateOfFire7 = GetGMST("fWeaponConditionRateOfFire7");
			fWeaponConditionRateOfFire8 = GetGMST("fWeaponConditionRateOfFire8");
			fWeaponConditionRateOfFire9 = GetGMST("fWeaponConditionRateOfFire9");
			fWeaponConditionRateOfFire10 = GetGMST("fWeaponConditionRateOfFire10");

			// Armor Type Keywords
			armorTypePowerArmor = TESDataHandler::GetSingleton()->LookupForm<BGSKeyword>(0x04D8A1, "Fallout4.esm");
			armorBodyPartChest = TESDataHandler::GetSingleton()->LookupForm<BGSKeyword>(0x06C0EC, "Fallout4.esm");
			armorBodyPartHead = TESDataHandler::GetSingleton()->LookupForm<BGSKeyword>(0x10C418, "Fallout4.esm");
			armorBodyPartLeftArm = TESDataHandler::GetSingleton()->LookupForm<BGSKeyword>(0x211D06, "FalloutCascadia.esm");
			armorBodyPartRightArm = TESDataHandler::GetSingleton()->LookupForm<BGSKeyword>(0x211D08, "FalloutCascadia.esm");
			armorBodyPartLeftLeg = TESDataHandler::GetSingleton()->LookupForm<BGSKeyword>(0x211D07, "FalloutCascadia.esm");
			armorBodyPartRightLeg = TESDataHandler::GetSingleton()->LookupForm<BGSKeyword>(0x211D09, "FalloutCascadia.esm");

			INFO("Item Degradation: Finished linking degradation forms.");
		}

		// Container Condition
		void InitializeContainerCondition(TESObjectREFR* containerREFR)
		{
			if (!containerREFR)
			{
				INFO("InitializeContainerCondition: Invalid REFR pointer.");
				return;
			}

			if (BGSInventoryList* inventory = containerREFR->inventoryList)
			{
				inventory->rwLock.lock_read();
				inventory->rwLock.lock_write();

				for (std::uint32_t i = 0; i < inventory->data.size(); i++)
				{
					BGSInventoryItem iterator;
					iterator = inventory->data[i];

					switch (iterator.object->formType.underlying())
					{
					case static_cast<std::uint32_t>(ENUM_FORM_ID::kARMO):
					{
						if (iterator.stackData)
						{
							std::uint32_t iterationCount = 0;
							for (BGSInventoryItem::Stack* traverse = iterator.stackData.get(); traverse; traverse->nextStack)
							{
								if (!traverse)
								{
									break;
								}

								if (!traverse->extra)
								{
									break;
								}

								if (!traverse->extra.get()->HasType(kHealth))
								{
									//InitializeArmorCondition(iterator.object.); TODO - It's all sorts of angry.
								}
								else
								{
									// TODO - UpdateArmorStats
								}

								iterationCount++;

								if (iterationCount > traverse->count)
								{
									break;
								}
							}
						}
						break;
					}
					case static_cast<std::uint32_t>(ENUM_FORM_ID::kWEAP):
					{
						if (iterator.stackData)
						{
							std::uint32_t iterationCount = 0;

							for (BGSInventoryItem::Stack* traverse = iterator.stackData.get(); traverse; traverse->nextStack)
							{
								if (!traverse)
								{
									break;
								}

								if (!traverse->extra)
								{
									break;
								}

								if (!traverse->extra.get()->HasType(kHealth))
								{
									//InitializeWeaponCondition(iterator.object.); TODO - It's all sorts of angry.
								}
								else
								{
									// TODO - UpdateWEaponStats
								}

								iterationCount++;

								if (iterationCount > traverse->count)
								{
									break;
								}
							}
						}

						break;
					}

					default:
						break;
					}
				}

				inventory->rwLock.unlock_read();
				inventory->rwLock.unlock_write();
			}
			else
			{
				INFO("InitializeContainerCondition: No inventory available");
				return;
			}
		}

		// Weapon Condition
		void InitializeWeaponCondition(TESObjectREFR* weaponREFR)
		{
			if (!weaponREFR)
			{
				DEBUG("InitializeWeaponCondition: Invalid REFR pointer.");
				return;
			}

			// No grenades or mines should have this data.
			TESObjectWEAP* tempWeaponREFR = (TESObjectWEAP*)weaponREFR;
			if (tempWeaponREFR->weaponData.type == WEAPON_TYPE::kGrenade || tempWeaponREFR->weaponData.type == WEAPON_TYPE::kMine)
			{
				DEBUG("InitializeWeaponCondition: REFR grenade/mine weapon type.");
				return;
			}

			// Check for ExtraData.
			if (weaponREFR->extraList.get())
			{
				// GetHealthPerc returns -1.0 if it can't find the kHealth type.
				if (weaponREFR->extraList->GetHealthPerc() < 0)
				{
					float value = RNG(0.55f, 0.85f);
					weaponREFR->extraList->SetHealthPerc(value);
					INFO(("InitializeWeaponCondition: Data initialized: {:s}"), std::to_string(weaponREFR->extraList->GetHealthPerc()));
					return;
				}
				else
				{
					INFO(("InitializeWeaponCondition: Data already initialized: {:s}"), std::to_string(weaponREFR->extraList->GetHealthPerc()));
					return;
				}
			}
			else
			{
				DEBUG("InitializeWeaponCondition: No ExtraData.");
				return;
			}
		}

		// Armor Degradation
		void InitializeArmorCondition(TESObjectREFR* armorREFR)
		{
			if (!armorREFR)
			{
				DEBUG("IntializeArmorCondition: Invalid REFR pointer.");
				return;
			}

			if (armorREFR->extraList.get())
			{
				// GetHealthPerc returns -1.0 if it can't find the kHealth type.
				if (armorREFR->extraList->GetHealthPerc() < 0)
				{
					float value = RNG(0.55f, 0.85f);

					armorREFR->extraList->SetHealthPerc(value);
					INFO(("IntializeArmorCondition: Data initialized: {:s}"), std::to_string(value));
					return;
				}
				else
				{
					INFO(("IntializeArmorCondition: Data already initialized: {:s}"), std::to_string(armorREFR->extraList->GetHealthPerc()));
					return;
				}
			}
			else
			{
				DEBUG("InitializeArmorCondition: No ExtraData.");
				return;
			}
		}

		float CalculateSkillBonusFromActor(TESObjectWEAP* myWeapon)
		{
			float actorSkillValue = 1.0f;

			if (myWeapon->weaponData.skill != nullptr)
			{
				actorSkillValue = (RE::Cascadia::GetPlayerCharacter()->GetActorValue(*myWeapon->weaponData.skill) / 100);
			}

			float result = (fDamageSkillBase + fDamageSkillMult * actorSkillValue);

			return result;
		}

		void ModWeaponCondition(TESObjectREFR* myWeapon, float a_value)
		{
			if (RE::Cascadia::GetPlayerCharacter()->IsGodMode())
			{
				DEBUG("God Mode enabled, ignoring weapon degradation.");
				return;
			}
		}
	}
}

/*
   RE::TESObjectWEAP::InstanceData* GetWeaponInstanceData(RE::ExtraDataList* myExtraDataList)
   {
	   RE::TESObjectWEAP::InstanceData* result = nullptr;
	   RE::TBO_InstanceData* myInstanceData = nullptr;

	   if (myExtraDataList)
	   {
		   RE::BSExtraData* myExtraData = myExtraDataList->GetByType(RE::kInstanceData);

		   if (myExtraData)
		   {
			   RE::ExtraInstanceData* myExtraInstanceData = (RE::ExtraInstanceData*)myExtraData;
			   if (myExtraInstanceData)
			   {
				   myInstanceData = myExtraInstanceData->data.get();
			   }
		   }
	   }
	   result = (RE::TESObjectWEAP::InstanceData*)myInstanceData;
	   return result;
   }

   RE::TESObjectARMO::InstanceData* GetArmorInstanceData(RE::ExtraDataList* myExtraDataList)
   {
	   RE::TESObjectARMO::InstanceData* result = nullptr;
	   RE::TBO_InstanceData* myInstanceData = nullptr;

	   if (myInstanceData)
	   {
		   RE::BSExtraData* myExtraData = myExtraDataList->GetByType(RE::kInstanceData);

		   if (myExtraData)
		   {
			   RE::ExtraInstanceData* myExtraInstanceData = (RE::ExtraInstanceData*)myExtraData;
			   if (myExtraInstanceData)
			   {
				   myInstanceData = myExtraInstanceData->data.get();
			   }
		   }
	   }
	   result = (RE::TESObjectARMO::InstanceData*)myInstanceData;
	   return result;
   }

   float GetWeaponDamage(WeaponConditionData myConditionData)
   {
	   if (myConditionData.instance == nullptr)
	   {
		   return 0.0;
	   }

	   RE::TESObjectWEAP* baseWEAPForm = (RE::TESObjectWEAP*)myConditionData.Form;
	   RE::TESObjectWEAP::InstanceData* myInstance = myConditionData.instance;

	   float currentCondition = GetWeaponConditionPercent(myConditionData);

	   float baseValue = static_cast <float> (baseWEAPForm->weaponData.value);

	   // Value
	   float newValue = static_cast <float> (baseValue * (pow(currentCondition, 1.5)));
	   myInstance->value = static_cast <uint32_t> (newValue);

	   // Damage
	   float baseDamage = baseWEAPForm->weaponData.attackDamage;
	   float minimum = 0.66f;

	   if (!IsMeleeWeapon(myConditionData))
	   {
		   myInstance->attackDelaySec = CalculateUpdatedRateOfFireValue(myConditionData, currentCondition);
	   }

	   switch (myInstance->flags.get())
	   {
	   case RE::WEAPON_FLAGS::kAutomatic:
		   minimum = 0.54f;
		   break;

	   default:
		   if (IsMeleeWeapon(myConditionData))
		   {
			   minimum = 0.5f;
		   }
		   else
		   {
			   minimum = 0.66f;
		   }
		   break;
	   }

	   if (baseWEAPForm->weaponData.damageTypes != nullptr)
	   {
		   for (uint32_t i = 0; i < baseWEAPForm->weaponData.damageTypes->size(); i++)
		   {
			   /**if (baseWEAPForm->weaponData.damageTypes[i][0].first != ItemDegradationForms.weaponConditionHealthMaxDMGT && baseWEAPForm->weaponData.damageTypes[i][0].first != ItemDegradationForms.weaponConditionHealthStartingDMGT)
			   {
				   baseValue = baseWEAPForm->weaponData.damageTypes[i][i].second.f;
				   newValue = CalculateUpdatedDamageValue(baseValue, minimum, currentCondition, CalculateSkillBonusFromActor(myConditionData));
				   return newValue;
			   }*
		   }
	   }

	   float newDamage = CalculateUpdatedDamageValue(baseDamage, minimum, currentCondition, CalculateSkillBonusFromActor(myConditionData));
	   return newDamage;
   }*/

/**

   float CalculateUpdatedDamageValue(float baseDamage, float minimum, float conditionPercent, float skillBonus)
   {
	   return (baseDamage * (minimum + conditionPercent * (1 - minimum)) * skillBonus);
   }

   float CalculateUpdatedRateOfFireValue(WeaponConditionData myConditionData, float currentCondition)
   {
	   // The calculation here is a bit weird in Fallout 4, fire rate is linked to attack delay, with attack delay, the lower it is, the higer the fire rate.
	   // Instead of multiplying the base attack delay by a set amount, we instead divide to make attack delay longer and in turn fire rate lower.

	   RE::TESObjectWEAP* baseWEAPForm = myConditionData.Form->As<RE::TESObjectWEAP>();
	   float baseROF = baseWEAPForm->weaponData.attackDelaySec;
	   float result = baseROF; // Start of as duplicate.

	   if (currentCondition <= 1.0 && currentCondition > 0.9)
	   {
		   result = (baseROF / fWeaponConditionRateOfFire10->GetFloat());
	   }
	   else if (currentCondition <= 0.9 && currentCondition > 0.8)
	   {
		   result = (baseROF / fWeaponConditionRateOfFire9->GetFloat());
	   }
	   else if (currentCondition <= 0.8 && currentCondition > 0.7)
	   {
		   result = (baseROF / fWeaponConditionRateOfFire8->GetFloat());
	   }
	   else if (currentCondition <= 0.7 && currentCondition > 0.6)
	   {
		   result = (baseROF / fWeaponConditionRateOfFire7->GetFloat());
	   }
	   else if (currentCondition <= 0.6 && currentCondition > 0.5)
	   {
		   result = (baseROF / fWeaponConditionRateOfFire6->GetFloat());
	   }
	   else if (currentCondition <= 0.5 && currentCondition > 0.4)
	   {
		   result = (baseROF / fWeaponConditionRateOfFire5->GetFloat());
	   }
	   else if (currentCondition <= 0.4 && currentCondition > 0.3)
	   {
		   result = (baseROF / fWeaponConditionRateOfFire4->GetFloat());
	   }
	   else if (currentCondition <= 0.3 && currentCondition > 0.2)
	   {
		   result = (baseROF / fWeaponConditionRateOfFire3->GetFloat());
	   }
	   else if (currentCondition <= 0.2 && currentCondition > 0.1)
	   {
		   result = (baseROF / fWeaponConditionRateOfFire2->GetFloat());
	   }
	   else if (currentCondition <= 0.1 && currentCondition > 0.0)
	   {
		   result = (baseROF / fWeaponConditionRateOfFire1->GetFloat());
	   }

	   return result;
   }

   void UnequipWeapon(RE::Actor* a_actor)
   {
	   if (!a_actor) { return; }

	   //a_actor->RemoveWeapon();

	   // TODO - rest of it
   }

   RE::BGSKeyword* GetPowerArmorTypeKeyword()
   {
	   return armorTypePowerArmor;
   }
   */
