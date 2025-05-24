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
			extern BSTArray<BGSKeyword*> keywordsOMOD;
			extern BGSKeyword* noFormlistWEAP;
			extern BGSKeyword* uniqueFormlistWEAP;
			extern BGSKeyword* omodAP;
			extern std::vector<std::pair<BGSKeyword*, BGSMod::Attachment::Mod*>>;

			bool InitializeAmmoSwitch();
			bool FindAmmoInFormlist(BGSKeyword* keyword, TESAmmo* currentAmmo, PlayerCharacter* playerCharacter);
			bool MaterialSwap(TESAmmo* currentAmmo);

			void DefineAmmoLists();

			void PostLoadGameAmmoFix();
		}
	} 
}