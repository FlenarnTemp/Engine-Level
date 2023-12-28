#pragma once

#ifndef ItemDegradationStruct
#define ItemDegradationStruct
struct ItemDegradation_Struct 
{
	// Holds our weapon condition on the weapon form in Creation Kit.
	RE::BGSDamageType* weaponConditionHealthMaxDMGT;
	RE::BGSDamageType* weaponConditionHealthStartingDMGT;

	// Actor Values that hold information on Items.
	RE::ActorValueInfo* itemConditionMaxHealth;
	RE::ActorValueInfo* itemConditionMinHealth;
	RE::ActorValueInfo* itemConditionStartCondtion;
};
#endif
extern ItemDegradation_Struct ItemDegradationForms;

struct WeaponConditionData 
{
	WeaponConditionData();
	WeaponConditionData(RE::TESForm* form, RE::ExtraDataList* extraData);
	WeaponConditionData(RE::Actor* actor);
	WeaponConditionData(RE::TESObjectREFR* refr);
	WeaponConditionData(RE::Actor* myActor, RE::TESForm* form, RE::ExtraDataList* extraData);

	RE::Actor*							actor;
	RE::TESForm*						Form;
	RE::ExtraDataList*					extraData;
	RE::TESObjectWEAP::InstanceData*	instance;
	std::uint64_t						invHandle;
};

struct ArmorConditionData
{
	ArmorConditionData();
	ArmorConditionData(RE::TESForm* form, RE::ExtraDataList* extradata);
	ArmorConditionData(RE::Actor* myActor, RE::TESForm* form, RE::ExtraDataList* extradata);
	ArmorConditionData(RE::TESObjectREFR* refr);
	ArmorConditionData(RE::Actor* actor);

	RE::Actor*							actor;
	RE::TESForm*						form;
	RE::ExtraDataList					extraData;
	RE::TESObjectARMO::InstanceData		instance;
};

RE::TESObjectWEAP::InstanceData* GetWeaponInstanceData(RE::ExtraDataList* myExtraDataList);
void DefineItemDegradationFormsFromGame();

float CalculateUpdatedDamageValue(float baseDamage, float minimum, float conditionPercent, float skillBonus);
float CalculateUpdatedRateOfFireValue(WeaponConditionData myConditionData, float currentCondition);
