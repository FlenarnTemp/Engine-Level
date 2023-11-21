#pragma once

struct WeaponConditionData 
{
	// TODO - 5 function calls.

	RE::Actor*							actor;
	RE::TESForm*						form;
	RE::ExtraDataList*					extraData;
	RE::TESObjectWEAP::InstanceData*	instance;
	UINT64								invHandle;
};

struct ArmorConditionData
{
	// TODO - 5 function calls.

	RE::Actor*							actor;
	RE::TESForm*						form;
	RE::ExtraDataList					extraData;
	RE::TESObjectARMO::InstanceData		instance;
};
