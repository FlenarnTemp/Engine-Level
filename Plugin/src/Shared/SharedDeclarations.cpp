#include "Shared/SharedDeclarations.h"

namespace RE
{
	namespace Cascadia
	{
		namespace Shared
		{
			BGSKeyword* noDegradation;
			bool bPrimitivesOn;

			void InitializeSharedVariables()
			{
				TESDataHandler* dataHandler = TESDataHandler::GetSingleton();
				noDegradation = dataHandler->LookupForm<BGSKeyword>(0x2BD72E, "FalloutCascadia.esm");
				bPrimitivesOn = true;
			}
		}
	}
}