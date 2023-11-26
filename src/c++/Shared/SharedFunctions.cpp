#include "SharedFunctions.h"

// Returns true is FormID is base game OR dynamically placed ("FF" index)
// Assumes all DLC is installed (Excluding DLCUltraHighResolution.esm as it is empty and "safe" to ignore).
// TODO - Better handling for the "FF" index, list official span of .esl's?
bool IsFormIDStringBaseGame(std::string formIDString)
{
	std::string formIndex = formIDString.substr(0, 2);
	if (
		formIndex == "00" || // Fallout4.esm
		formIndex == "01" || // DLCRobot.esm
		formIndex == "02" || // DLCworkshop01.esm
		formIndex == "03" || // DLCCoast.esm
		formIndex == "04" || // DLCworkshop02.esm
		formIndex == "05" || // DLCworkshop03.esm
		formIndex == "06" || // DLCNukaWorld.esm
		formIndex == "ff"    // Dynamic content, FF - Creation Club primarily(?)
		)
	{
		return true;
	}

	return false;
}

// Returns FormID as hex string
std::string GetFormIDAsString(uint32_t formID)
{
	std::string result{};
	std::stringstream formIDStream;
	formIDStream << std::hex << formID;
	result = formIDStream.str();
	if (result.length() < 8)
	{
		std::string temp;
		for (int i = 0; i < (8 - result.length()); i++)
		{
			temp.append("0");
		}
		temp.append("0");
		result = temp;
	}
	return result;
}

// 0 = male, 1 = female
uint32_t GetActorSex(RE::Actor* myActor)
{
	uint32_t myGender = 0;
	RE::TESNPC* myActorBase = myActor->GetNPC();
	if (myActorBase)
	{
		myGender = myActorBase->GetSex();
	}
	return myGender;
}

RE::PlayerCharacter* GetPlayerCharacter()
{
	auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
	return PlayerCharacter;
}

bool IsPlayerGodMode() 
{
	return GetPlayerCharacter()->IsGodMode();
}

int16_t GetPlayerLevel() 
{
	return GetPlayerCharacter()->GetLevel();
}

uint32_t GetPlayerSex()
{
	return GetActorSex(GetPlayerCharacter());
}


// Takes a Float and converts it to a Precise String.
// eg. FloatToPreciseString(3.141592, 2) would return "3.14"
std::string FloatToPreciseString(float myFloat, int myPrecision)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(myPrecision) << myFloat;
	return stream.str();
}

int RNG(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

	return dist(rng);
}

float RNG(float min, float max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> dist(min, max);

	return dist(rng);
}

bool WeaponHasKeyword(RE::TESObjectWEAP* weapon, RE::BGSKeyword* keyword)
{
	if (weapon)
	{
		for (uint32_t i = 0; i < weapon->numKeywords; i++)
		{
			if (weapon->keywords[i] == keyword)
			{
				return true;
			}
		}
	}
	return false;
}

bool ArmorHasKeyword(RE::TESObjectARMO* armor, RE::BGSKeyword* keyword)
{
	if (armor)
	{
		for (uint32_t i = 0; i < armor->numKeywords; i++)
		{
			if (armor->keywords[i] == keyword)
			{
				return true;
			}
		}
	}
	return false;
}

/*bool ReferenceHasKeyword(RE::TESObjectREFR* ref, RE::BGSKeyword* keyword)
{
	if (ref)
	{
		RE::TBO_InstanceData* myInstanceData = nullptr;
		RE::BSExtraData* myExtraData = ref->extraList->GetByType(RE::kInstanceData);

		if (myExtraData)
		{
			// TODO
			//RE::ExtraInstanceData* myExtraInstanceData = 
		}

		if (myInstanceData)
		{
			
		}
	}

}*/

bool NPCHasKeyword(RE::TESNPC* npc, RE::BGSKeyword* keyword)
{
	if (npc)
	{
		for (uint32_t i = 0; i < npc->numKeywords; i++)
		{
			if (npc->keywords[i] == keyword)
			{
				return true;
			}
		}
	}

	// If still false, check if race has keyword
	// Doesn't seem to be bound on the NPC check if it's only on the race? Gotta love inheritance.
	RE::TESRace* npcRace = npc->originalRace;

	if (npcRace)
	{
		for (uint32_t i = 0; i < npcRace->numKeywords; i++)
		{
			if (npcRace->keywords[i] == keyword)
			{
				return true;
			}
		}
	}

	return false;
}

// Get Inxed in FormList
UINT32 PositionInFormList(RE::TESForm* form, RE::BGSListForm* list)
{
	if (list->arrayOfForms.size() != 0)
	{
		for (uint32_t i = 0; i <= list->arrayOfForms.size() - 1; i++)
		{
			if (list->arrayOfForms[i] == form)
			{
				return i;
			}
		}
	}
	return 0;
}

// Gets and return next form player has a count higher than 0 of in list, from current index to start of array
uint32_t GetNextAvailableFormInInventoryFromList(UINT32 startingIndex, RE::BGSListForm* list)
{
	auto playerCharacter = GetPlayerCharacter();
	if (list->arrayOfForms.size() != 0)
	{
		for (uint32_t a = startingIndex + 1; a <= list->arrayOfForms.size() - 1; a++)
		{
			RE::TESBoundObject* a_object = (RE::TESBoundObject*)(list->arrayOfForms[a]);
			if (playerCharacter->GetInventoryObjectCount(a_object) != 0)
			{
				return a;
			}		
		}

		for (uint32_t b = 0; b <= startingIndex; b++)
		{
			RE::TESBoundObject* a_object = (RE::TESBoundObject*)(list->arrayOfForms[b]);
			if (playerCharacter->GetInventoryObjectCount(a_object) != 0)
			{
				return b;
			}	
		}
	}

	return (uint32_t) - 1;
}

//	Returns True if Form is in FormList
bool IsFormInList(RE::TESForm* form, RE::BGSListForm* list)
{
	if (list->arrayOfForms.size() != 0)
	{
		for (uint32_t i = 0; i <= list->arrayOfForms.size() - 1; i++)
		{
			if (list->arrayOfForms[i] == form)
			{
				return true;
			}
		}
	}
	return false;
}
