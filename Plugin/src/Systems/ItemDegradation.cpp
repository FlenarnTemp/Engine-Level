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

		// No Degradation Keyword
		BGSKeyword* noDegradation;

		void DefineItemDegradationFormsFromGame()
		{
			INFO("Item Degradation: Linking degradation forms.");

			fDamageSkillBase = 0.5f;
			fDamageSkillMult = 0.5f;
			fAVDMeleeDamageStrengthMult = 0.5f;
			fAVDMeleeDamageStrengthOffset = 0.0f;

			GameSettingCollection* gameSettings = GameSettingCollection::GetSingleton();

			fWeaponConditionRateOfFire1 = gameSettings->GetSetting("fWeaponConditionRateOfFire1");
			fWeaponConditionRateOfFire2 = gameSettings->GetSetting("fWeaponConditionRateOfFire2");
			fWeaponConditionRateOfFire3 = gameSettings->GetSetting("fWeaponConditionRateOfFire3");
			fWeaponConditionRateOfFire4 = gameSettings->GetSetting("fWeaponConditionRateOfFire4");
			fWeaponConditionRateOfFire5 = gameSettings->GetSetting("fWeaponConditionRateOfFire5");
			fWeaponConditionRateOfFire6 = gameSettings->GetSetting("fWeaponConditionRateOfFire6");
			fWeaponConditionRateOfFire7 = gameSettings->GetSetting("fWeaponConditionRateOfFire7");
			fWeaponConditionRateOfFire8 = gameSettings->GetSetting("fWeaponConditionRateOfFire8");
			fWeaponConditionRateOfFire9 = gameSettings->GetSetting("fWeaponConditionRateOfFire9");
			fWeaponConditionRateOfFire10 = gameSettings->GetSetting("fWeaponConditionRateOfFire10");

			TESDataHandler* dataHandler = TESDataHandler::GetSingleton();

			// Armor Type Keywords
			armorTypePowerArmor = dataHandler->LookupForm<BGSKeyword>(0x04D8A1, "Fallout4.esm");
			armorBodyPartChest = dataHandler->LookupForm<BGSKeyword>(0x06C0EC, "Fallout4.esm");
			armorBodyPartHead = dataHandler->LookupForm<BGSKeyword>(0x10C418, "Fallout4.esm");
			armorBodyPartLeftArm = dataHandler->LookupForm<BGSKeyword>(0x211D06, "FalloutCascadia.esm");
			armorBodyPartRightArm = dataHandler->LookupForm<BGSKeyword>(0x211D08, "FalloutCascadia.esm");
			armorBodyPartLeftLeg = dataHandler->LookupForm<BGSKeyword>(0x211D07, "FalloutCascadia.esm");
			armorBodyPartRightLeg = dataHandler->LookupForm<BGSKeyword>(0x211D09, "FalloutCascadia.esm");

			TESAmmo* ammo45 = dataHandler->LookupForm<TESAmmo>(0x01F66A, "Fallout4.esm");
			TESAmmo* ammo308 = dataHandler->LookupForm<TESAmmo>(0x01F66B, "Fallout4.esm");

			ammoDegradationMap[ammo45] = 0.0035;
			ammoDegradationMap[ammo308] = 0.0133f;

			// No Degradation Keyword
			noDegradation = dataHandler->LookupForm<BGSKeyword>(0x2BD72E, "FalloutCascadia.esm");

			INFO("Item Degradation: Finished linking degradation forms.");
		}


		float CalculateSkillBonusFromActor(TESObjectWEAP* myWeapon)
		{
			float actorSkillValue = 1.0f;

			if (myWeapon->weaponData.skill != nullptr)
			{
				actorSkillValue = (PlayerCharacter::GetSingleton()->GetActorValue(*myWeapon->weaponData.skill) / 100);
			}

			float result = (fDamageSkillBase + fDamageSkillMult * actorSkillValue);

			return result;
		}

		void ModWeaponCondition()
		{
			PlayerCharacter* playerCharacter = PlayerCharacter::GetSingleton();

			if (playerCharacter->IsGodMode())
			{
				DEBUG("ModWeaponCondition: God Mod enabled, ignoring weapon degradation.");
				return;
			}

			EquippedItem& equippedWeapon = playerCharacter->currentProcess->middleHigh->equippedItems[0];
			TESObjectWEAP* playerWeapon = (TESObjectWEAP*)equippedWeapon.item.object;
			auto instanceData = equippedWeapon.item.instanceData.get();
			DEBUG("Bound object: {}", playerWeapon->IsBoundObject());
		}

		void ModWeaponCondition(TESObjectREFR* myWeapon, float a_value)
		{
			
		}
	}
}