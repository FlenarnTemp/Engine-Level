#pragma once

namespace RE
{
	namespace Cascadia
	{
		namespace AmmoSwitch
		{
			extern bool switchingAmmo;

			extern BSTArray<BGSKeyword*> keywordsAmmo;
			extern std::unordered_map<BGSKeyword*, BGSListForm*> keywordFormlistMap;
			extern BGSKeyword* noFormlistWEAP;
			extern BGSKeyword* uniqueFormlistWEAP;

			bool InitializeAmmoSwitch();

			void DefineAmmoLists();

			void PostLoadGameAmmoFix();
		}
	} 
}