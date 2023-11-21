#pragma once

bool IsFormIDStringBaseGame(std::string formIDString);
std::string GetFormIDAsString(uint32_t formID);

RE::PlayerCharacter* GetPlayerCharacter();
bool IsPlayerGodMode();
int16_t GetPlayerLevel();

std::string FloatToPreciseString(float myFloat, int myPrecision);

int RNG(int min, int max);
float RNG(float min, float max);

RE::TESForm* GetFormFromPlugin(const char* pluginname, UINT32 formID);
