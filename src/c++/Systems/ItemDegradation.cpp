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

// Item Degradation
ItemDegradation_Struct			ItemDegradationForms;


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

float GetWeaponConditionMaximum(RE::TESObjectREFR* refr)
{
	if (!refr)
	{
		return -1.0f;
	}
	return GetWeaponConditionMaximum(WeaponConditionData(refr));
}

float GetWeaponConditionPercent(WeaponConditionData data)
{
	if (!data.extraData)
	{
		return -1.0f;
	}

	if (!data.extraData->HasType(RE::kHealth))
	{
		return -1.0f;
	}

	return data.extraData->GetHealthPercent();
}

float GetWeaponConditionPercent(RE::TESObjectREFR* refr)
{
	if (!refr)
	{
		return -1.0f;
	}
	return GetWeaponConditionPercent(WeaponConditionData(refr));
}

float GetWeaponConditionCurrent(WeaponConditionData data)
{
	float percent = GetWeaponConditionPercent(data);
	float maximum = GetWeaponConditionMaximum(data);
	if ((0 > percent) || (0 > maximum))
	{
		return -1.0f;
	}
	return (percent * maximum);
}

float GetWeaponConditionCurrent(RE::TESObjectREFR* refr)
{
	if (!refr)
	{
		return -1.0f;
	}
	return GetWeaponConditionPercent(WeaponConditionData(refr));
}

void SetWeaponConditionMaximum(WeaponConditionData data, float value)
{
	if (!data.extraData)
	{
		return;
	}

	if (!data.extraData->HasType(RE::kObjectHealth))
	{
		return;
	}
	value = std::max(1.0f, value);
	data.extraData->SetCurrentCharge(value);
}

void SetWeaponConditionMaximum(RE::TESObjectREFR* refr, float value)
{
	if (!refr)
	{
		return;
	}
	SetWeaponConditionMaximum(WeaponConditionData(refr), value);
}

void SetWeaponConditionPercent(WeaponConditionData data, float value)
{
	if (!data.extraData)
	{
		return;
	}

	if (!data.extraData->HasType(RE::kHealth))
	{
		return;
	}

	value = std::max(0.0f, value);
	if (value > 1.0f)
	{
		value = 1.0f;
	}

	data.extraData->SetHealthPercent(value);
}

void SetWeaponConditionPercent(RE::TESObjectREFR* refr, float value)
{
	if (!refr)
	{
		return;
	}

	SetWeaponConditionPercent(WeaponConditionData(refr), value);
}

void SetWeaponConditionCurrent(WeaponConditionData data, float value)
{
	float current = GetWeaponConditionCurrent(data);
	float maximum = GetWeaponConditionMaximum(data);
	if ((0.0f > current) || (0.0f > maximum))
	{
		return;
	}

	if (!data.Form)
	{
		return;
	}

	value = std::max(0.0f, value);
	SetWeaponConditionPercent(data, (value / maximum));
}

void SetWeaponConditionCurrent(RE::TESObjectREFR* refr, float value)
{
	if (!refr)
	{
		return;
	}

	SetWeaponConditionCurrent(WeaponConditionData(refr), value);
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

float GetMaxConditionValueFromWeapon(RE::TESObjectWEAP* myWeapon)
{
	float result = -1.0;

	if (myWeapon->weaponData.damageTypes == nullptr)
	{
		return result;
	}

	for (uint32_t i = 0; i < myWeapon->weaponData.damageTypes->size(); i++)
	{
		if (myWeapon->weaponData.damageTypes[i][0].first == ItemDegradationForms.weaponConditionHealthMaxDMGT)
		{
			result = myWeapon->weaponData.damageTypes[i][0].second.f;
			break;
		}
	}

	return result;
}

float GetMaxConditionValueFromArmor(RE::TESObjectARMO* myArmor)
{
	float result = -1.0;

	if (myArmor->armorData.health > 0)
	{
		result = (float)myArmor->armorData.health;
	}

	logger::info("Getting max condition of %f from %s", result, myArmor->GetFullName());

	return result;
}

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

	float fMaxCNDValue = GetMaxConditionValueFromWeapon(myWeaponBaseForm);
	float fMinCNDValue = 0;
	float fStartCNDValue = 0;

	if (!myConditionData.extraData)
	{
		return;
	}

	if (fMaxCNDValue == -1)
	{
		// Unable to find value from Damage Type so default to random amount between 400 & 600 (Average of all basegame Fallout 3 weapons is 438)
		fMaxCNDValue = (static_cast <float> (rand() % 500 + 400));
	}

	// Minimum CND value should be 10% of max CND value.
	fMinCNDValue = (fMaxCNDValue / 10);

	// Create Condition value if they don't exist.
	if (!myConditionData.extraData->HasType(RE::kHealth)) {
		if (myConditionData.extraData->HasType(RE::kInstanceData)) {
			if (myConditionData.instance->actorValues) {
				for (uint32_t i = 0; i < myConditionData.instance->actorValues->size(); i++) {
					auto iteration = myConditionData.instance->actorValues[i];
					fMaxCNDValue = (iteration)
				}
			}
		}
	}

}

void InitializeWeaponCondition(RE::TESObjectREFR* myRef)
{
	if (!myRef)
	{
		return;
	}
	InitializeConditionWeapon(WeaponConditionData(myRef));
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
			}*/
		}
	}

	float newDamage = CalculateUpdatedDamageValue(baseDamage, minimum, currentCondition, CalculateSkillBonusFromActor(myConditionData));
	return newDamage;
}

void DefineItemDegradationFormsFromGame()
{
	logger::info("Item Degradation: Linking Degradation forms from FalloutCascadia.esm");

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
}

float CalculateSkillBonusFromActor(WeaponConditionData myConditionData)
{
	float actorSkillValue = 1.0f;

	RE::TESObjectWEAP* myWeapon = myConditionData.Form->As<RE::TESObjectWEAP>();

	if (myWeapon->weaponData.skill != nullptr)
	{
		if (myConditionData.actor != nullptr)
		{
			actorSkillValue = (myConditionData.actor->GetActorValue(*myWeapon->weaponData.skill) / 100);
		}
		else
		{
			actorSkillValue = (GetPlayerCharacter()->GetActorValue(*myWeapon->weaponData.skill) / 100);
		}
	}

	float result = (fDamageSkillBase + fDamageSkillMult * actorSkillValue);

	return result;
}

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
