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
		for (int i = 0; i < weapon->numKeywords; i++)
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
		for (int i = 0; i < armor->numKeywords; i++)
		{
			if (armor->keywords[i] == keyword)
			{
				return true;
			}
		}
	}
	return false;
}
