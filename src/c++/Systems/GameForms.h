#pragma once

#ifndef VanillaAVStruct
#define VanillaAVStruct

RE::ActorValue* avSingleton = RE::ActorValue::GetSingleton();

struct VanillaAV_Struct
{
	// Vanilla SPECIAL Values
	RE::ActorValueInfo* Strength = avSingleton->strength;
	RE::ActorValueInfo* Perception = avSingleton->perception;
	RE::ActorValueInfo* Endurance = avSingleton->endurance;
	RE::ActorValueInfo* Charisma = avSingleton->charisma;
	RE::ActorValueInfo* Intelligence = avSingleton->intelligence;
	RE::ActorValueInfo* Agility = avSingleton->agility;
	RE::ActorValueInfo* Luck = avSingleton->luck;
};
#endif
extern VanillaAV_Struct VanillaActorValues;

#ifndef CascadiaAVStruct
#define CascadiaAVStruct
struct CascadiaAV_Struct
{
	// Skill Values
	RE::ActorValueInfo* Barter;
	RE::ActorValueInfo* EnergyWeapons;
	RE::ActorValueInfo* Explosives;
	RE::ActorValueInfo* Lockpick;
	RE::ActorValueInfo* Medicine;
	RE::ActorValueInfo* MeleeWeapons;
	RE::ActorValueInfo* Repair;
	RE::ActorValueInfo* Science;
	RE::ActorValueInfo* Guns;
	RE::ActorValueInfo* Sneak;
	RE::ActorValueInfo* Speech;
	RE::ActorValueInfo* Unarmed;
	RE::ActorValueInfo* Survival;

	RE::ActorValueInfo* WeaponCNDResist;
};
#endif
extern CascadiaAV_Struct CascadiaActorValues;

#ifndef CascadiaGlobalsStruct
#define CascadiaGlobalsStruct
struct CascadiaGlobals_Struct
{
	// Globals
	RE::TESGlobal* TutorialWPNCND;
};
#endif
extern CascadiaGlobals_Struct CascadiaGlobals;

#ifndef CascadiaPerksStruct
#define CascadiaPerksStruct
struct CascadiaPerks_Struct 
{

	// Weapon Type Perks
	RE::BGSPerk* WeaponTypeEnergyWeaponsPerk;
	RE::BGSPerk* WeaponTypeExplosivesPerk;
	RE::BGSPerk* WeaponTypeMeleeWeaponsPerk;
	RE::BGSPerk* WeaponTypeGunsPerk;
	RE::BGSPerk* WeaponTypeUnarmedPerk;

	// Handler Perks
	RE::BGSPerk* WeaponConditionHandlerPerk;
};
#endif
extern CascadiaPerks_Struct CascadiaPerks;

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
