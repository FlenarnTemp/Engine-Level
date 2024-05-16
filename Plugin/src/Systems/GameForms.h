#pragma once

namespace RE
{
	namespace Cascadia
	{

#ifndef VanillaAVStruct
#define VanillaAVStruct

		struct VanillaAV_Struct
		{
			// Vanilla SPECIAL Values
			ActorValueInfo* Strength;
			ActorValueInfo* Perception;
			ActorValueInfo* Endurance;
			ActorValueInfo* Charisma;
			ActorValueInfo* Intelligence;
			ActorValueInfo* Agility;
			ActorValueInfo* Luck;
		};
#endif
		extern VanillaAV_Struct VanillaActorValues;

#ifndef CascadiaAVStruct
#define CascadiaAVStruct
		struct CascadiaAV_Struct
		{
			// Skill Values
			ActorValueInfo* Barter;
			ActorValueInfo* EnergyWeapons;
			ActorValueInfo* Explosives;
			ActorValueInfo* Lockpick;
			ActorValueInfo* Medicine;
			ActorValueInfo* MeleeWeapons;
			ActorValueInfo* Repair;
			ActorValueInfo* Science;
			ActorValueInfo* Guns;
			ActorValueInfo* Sneak;
			ActorValueInfo* Speech;
			ActorValueInfo* Unarmed;
			ActorValueInfo* Survival;
			ActorValueInfo* WeaponCNDResist;
		};
#endif
		extern CascadiaAV_Struct CascadiaActorValues;

#ifndef CascadiaGlobalsStruct
#define CascadiaGlobalsStruct
		struct CascadiaGlobals_Struct
		{
			// Globals
			TESGlobal* TutorialWPNCND;
		};
#endif
		extern CascadiaGlobals_Struct CascadiaGlobals;

#ifndef CascadiaPerksStruct
#define CascadiaPerksStruct
		struct CascadiaPerks_Struct
		{

			// Weapon Type Perks
			BGSPerk* WeaponTypeEnergyWeaponsPerk;
			BGSPerk* WeaponTypeExplosivesPerk;
			BGSPerk* WeaponTypeMeleeWeaponsPerk;
			BGSPerk* WeaponTypeGunsPerk;
			BGSPerk* WeaponTypeUnarmedPerk;

			// Handler Perks
			BGSPerk* WeaponConditionHandlerPerk;
		};
#endif
		extern CascadiaPerks_Struct CascadiaPerks;

	}
}
