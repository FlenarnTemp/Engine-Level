#pragma once

namespace RE
{
	namespace Cascadia
	{
		namespace AmmoSwitch
		{
			extern bool switchingAmmo;
			extern TESAmmo* ammoToSwitchTo;
			extern BGSObjectInstance* equippedInstance;

			extern BSTArray<BGSKeyword*> keywordsAmmo;
			extern std::unordered_map<BGSKeyword*, BGSListForm*> keywordFormlistMap;
			extern BGSKeyword* noFormlistWEAP;
			extern BGSKeyword* uniqueFormlistWEAP;

			bool InitializeAmmoSwitch();
			bool FindAmmoInFormlist(BGSKeyword* keyword, TESAmmo* currentAmmo, PlayerCharacter* playerCharacter);
			bool MaterialSwap(TESAmmo* currentAmmo);

			void DefineAmmoLists();

			void PostLoadGameAmmoFix();
		}
	} 
}