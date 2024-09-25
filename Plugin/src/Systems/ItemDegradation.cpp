#include "ItemDegradation.h"

namespace RE
{
	namespace Cascadia
	{
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


			TESDataHandler* dataHandler = TESDataHandler::GetSingleton();

			// Armor Type Keywords
			armorTypePowerArmor = dataHandler->LookupForm<BGSKeyword>(0x04D8A1, "Fallout4.esm");
			armorBodyPartChest = dataHandler->LookupForm<BGSKeyword>(0x06C0EC, "Fallout4.esm");
			armorBodyPartHead = dataHandler->LookupForm<BGSKeyword>(0x10C418, "Fallout4.esm");
			armorBodyPartLeftArm = dataHandler->LookupForm<BGSKeyword>(0x211D06, "FalloutCascadia.esm");
			armorBodyPartRightArm = dataHandler->LookupForm<BGSKeyword>(0x211D08, "FalloutCascadia.esm");
			armorBodyPartLeftLeg = dataHandler->LookupForm<BGSKeyword>(0x211D07, "FalloutCascadia.esm");
			armorBodyPartRightLeg = dataHandler->LookupForm<BGSKeyword>(0x211D09, "FalloutCascadia.esm");

			// Ammo types with respective degradation values
			TESAmmo* ammo10mm = dataHandler->LookupForm<TESAmmo>(0x01F276, "Fallout4.esm");
			TESAmmo* ammo2mmEC = dataHandler->LookupForm<TESAmmo>(0x18ABDF, "Fallout4.esm");
			TESAmmo* ammo308 = dataHandler->LookupForm<TESAmmo>(0x01F66B, "Fallout4.esm");
			TESAmmo* ammo38 = dataHandler->LookupForm<TESAmmo>(0x04CE87, "Fallout4.esm");
			TESAmmo* ammo44 = dataHandler->LookupForm<TESAmmo>(0x09221C, "Fallout4.esm");
			TESAmmo* ammo45 = dataHandler->LookupForm<TESAmmo>(0x01F66A, "Fallout4.esm");

			ammoDegradationMap[ammo10mm] = 0.005f;
			ammoDegradationMap[ammo2mmEC] = 0.04f;
			ammoDegradationMap[ammo308] = 0.0133f;
			ammoDegradationMap[ammo38] = 0.003f;
			ammoDegradationMap[ammo44] = 0.011f;
			ammoDegradationMap[ammo45] = 0.0035f;


			// No Degradation Keyword
			noDegradation = dataHandler->LookupForm<BGSKeyword>(0x2BD72E, "FalloutCascadia.esm");

			INFO("Item Degradation: Finished linking degradation forms.");
		}
	}
}