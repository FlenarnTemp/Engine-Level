#include "ItemDegradation.h"

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

	}
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
		}
	}
}

