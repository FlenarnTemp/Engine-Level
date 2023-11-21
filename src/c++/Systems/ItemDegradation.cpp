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

