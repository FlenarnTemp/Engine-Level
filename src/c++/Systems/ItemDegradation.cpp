#include "ItemDegradation.h"
#include "Shared/SharedFunctions.h"

// Weapon Condition - Manual Settings
float fDamageSkillMult;
float fDamageSkillBase;
float fAVDMeleeDamageStrengthMult;
float fAVDMeleeDamageStrengthOffset;

// Weapon Condition - Rate of Fire Game Settings
RE::Setting* fWeaponConditionRateOfFire1;
RE::Setting* fWeaponConditionRateOfFire2;
RE::Setting* fWeaponConditionRateOfFire3;
RE::Setting* fWeaponConditionRateOfFire4;
RE::Setting* fWeaponConditionRateOfFire5;
RE::Setting* fWeaponConditionRateOfFire6;
RE::Setting* fWeaponConditionRateOfFire7;
RE::Setting* fWeaponConditionRateOfFire8;
RE::Setting* fWeaponConditionRateOfFire9;
RE::Setting* fWeaponConditionRateOfFire10;

// Power Armor Keyword
RE::BGSKeyword* furnitureTypePowerArmor;

// Armor Type Keywords
RE::BGSKeyword* armorTypePower;
RE::BGSKeyword* armorBodyPartChest;
RE::BGSKeyword* armorBodyPartHead;
RE::BGSKeyword* armorBodyPartLeftArm;
RE::BGSKeyword* armorBodyPartRightArm;
RE::BGSKeyword* armorBodyPartLeftLeg;
RE::BGSKeyword* armorBodyPartRightLeg;

/* =============================================================================
	Weapon Condition
   ============================================================================= */

WeaponConditionData::WeaponConditionData() : actor(nullptr), Form(nullptr), extraData(nullptr), instance(nullptr) { }

WeaponConditionData::WeaponConditionData(RE::TESForm* form, RE::ExtraDataList* extradata)
{
	actor = nullptr;
	Form = form;
	extraData = extradata;
	instance = nullptr;

	if (extraData)
	{
		if (extraData->HasType(RE::kInstanceData)) {
			instance = GetWeaponInstanceData(extraData);
		}
	}
}

WeaponConditionData::WeaponConditionData(RE::Actor* myActor, RE::TESForm* form, RE::ExtraDataList* extradata)
{
	actor = myActor;
	Form = form;
	extraData = extradata;
	instance = nullptr;

	if (extraData) 
	{
		if (extraData->HasType(RE::kInstanceData)) {
			instance = GetWeaponInstanceData(extraData);
		}
	}
}

WeaponConditionData::WeaponConditionData(RE::TESObjectREFR* refr)
{
	if (refr)
	{
		actor = GetPlayerCharacter();
		Form = refr->As<RE::TESForm>();
		extraData = refr->extraList.get();
		instance = nullptr;

		if (extraData)
		{
			if (extraData->HasType(RE::kInstanceData))
			{
				instance = GetWeaponInstanceData(extraData);
			}
		}
	}
}

WeaponConditionData::WeaponConditionData(RE::Actor* myActor)
{
	if (myActor)
	{
		actor = myActor;
		Form = nullptr; // GetEquippedWeaponForm(myActor);
		extraData = nullptr; // GetEquippedWeaponExtraData(myActor);
		instance = nullptr; // GetEquippedWeaponInstanceData(myActor);
	}
}

float GetWeaponConditionMaximum(WeaponConditionData data)
{
	if (!data.extraData)
	{
		return -1.0;
	}

	if (!data.extraData->HasType(RE::kCharge))
	{
		return -1.0;
	}

	return data.extraData->GetCurrentCharge();
}

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

/*float GetMaxConditionValueFromWeapon(RE::TESObjectWEAP* myWeapon)
{
	float result = -1.0;

	if (myWeapon->weaponData.damageTypes == nullptr)
	{
		return result;
	}

	for (uint32_t i = 0; i < myWeapon->weaponData.damageTypes->size(); i++)
	{
		if (myWeapon->weaponData.damageTypes[i] == myWeapon)

		result = 
		break;
	}
}*

/**float GetMaxConditionValueFromArmor(RE::TESObjectARMO* myArmor)
{
	float result = -1.0;

	if (myArmor->armorData.health > 0)
	{
		result = (float)myArmor->armorData.health;
	}

	logger::info("Getting max condition of %f from %s", result, myArmor->GetFullName());
}*/

float GetStartingConditionValueFromArmor(RE::TESObjectARMO::InstanceData* myInstanceData)
{
	// Functionality differs from Weapon Condition, if no instance health, then we assume 100% condition.
	// Note: Creation Kit health-% at 100% is strange and doesn't actually set it to 100%.
	float result = 100.0;

	if (myInstanceData == nullptr)
	{
		return -1.0;
	}

	if (myInstanceData->health > 0)
	{
		result = (float)myInstanceData->health;
	}

	if (result != -1)
	{
		result = (result / 100);
	}

	return result;
}

void InitializeConditionWeapon(WeaponConditionData myConditionData)
{
	RE::TESObjectWEAP* myWeaponBaseForm = (RE::TESObjectWEAP*)myConditionData.Form;

	if (myWeaponBaseForm == nullptr)
	{
		return;
	}

	// Ignore Grenades & Mines
	if (myWeaponBaseForm->weaponData.type == RE::WEAPON_TYPE::kGrenade || myWeaponBaseForm->weaponData.type == RE::WEAPON_TYPE::kMine)
	{
		return;
	}

	float fMaxCNDValue = 0;//GetMaxConditionValueFromWeapon(myWeaponBaseForm);
	float fMinCNDValue = 0;
	//float fStartCNDValue = 0;

	if (!myConditionData.extraData)
	{
		return;
	}

	if (fMaxCNDValue == -1)
	{
		// Unable to find value from Damage Type so default to random amount between 400 & 600 (Averae of all Base Game Fallout 3 weapons is 438)
		fMaxCNDValue = (static_cast <float> (rand() % 500 + 400));
	}

	// Minimum CND value should be 10% of max CND value.
	fMinCNDValue = (fMaxCNDValue / 10);

}

void InitializeWeaponCondition(RE::TESObjectREFR* myRef)
{
	if (!myRef)
	{
		return;
	}
	//InitializeConditionWeapon(WeaponConditionData(myRef));
}

float GetWeaponDamage(WeaponConditionData myConditionData)
{
	if (myConditionData.instance == nullptr)
	{
		return 0.0;
	}

	RE::TESObjectWEAP* baseWEAPForm = (RE::TESObjectWEAP*)myConditionData.Form;
	RE::TESObjectWEAP::InstanceData* myInstance = myConditionData.instance;

	float currentCondition = 0;//GetConditionPercentWeapon(myConditionData);

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
		minimum = 0.54;
		break;

	default:
		if (IsMeleeWeapon(myConditionData))
		{
			minimum = 0.5;
		}
		else
		{
			minimum = 0.66;
		}
		break;
	}

	if (baseWEAPForm->weaponData.damageTypes != nullptr)
	{
		for (int i = 0; i < baseWEAPForm->weaponData.damageTypes->size(); i++)
		{

		}
	}

	float newDamage = CalculateUpdateDamageValue(baseDamage, minimum, currentCondition, CalculateSkillBonusFromActor(myConditionData));
	myInstance->attackDamage = newDamage;
}

void DefineItemDegradationFormsFromGame()
{
	logger::info("Item Degradation: Linking Degradation forms from FalloutCascadia.esm");


	fDamageSkillBase = 0.5f;
	fDamageSkillMult = 0.5f;
	fAVDMeleeDamageStrengthMult = 0.5;
	fAVDMeleeDamageStrengthOffset = 0.0;

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
}
