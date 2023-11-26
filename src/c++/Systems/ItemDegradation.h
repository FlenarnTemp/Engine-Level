#pragma once

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
