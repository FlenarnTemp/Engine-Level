#pragma once

bool IsFormIDStringBaseGame(std::string formIDString);
std::string GetFormIDAsString(uint32_t formID);

uint32_t GetActorSex(RE::Actor* myActor);
RE::PlayerCharacter* GetPlayerCharacter();
bool IsPlayerGodMode();
int16_t GetPlayerLevel();
uint32_t GetPlayerSex();

std::string FloatToPreciseString(float myFloat, int myPrecision);

int RNG(int min, int max);
float RNG(float min, float max);

RE::TESForm* GetFormFromPlugin(const char* pluginname, UINT32 formID);
RE::TESForm* GetFormFromIdentifier(const std::string& myIdentifier);

bool WeaponHasKeyword(RE::TESObjectWEAP* weapon, RE::BGSKeyword* keyword);
bool ArmorHasKeyword(RE::TESObjectARMO* armor, RE::BGSKeyword* keyword);
bool ReferenceHasKeyword(RE::TESObjectREFR* ref, RE::BGSKeyword* keyword); // TODO

bool NPCHasKeyword(RE::TESNPC* npc, RE::BGSKeyword* keyword);

uint32_t PositionInFormList(RE::TESForm* form, RE::BGSListForm* list);
uint32_t GetNextAvailableFormInInventoryFromList(UINT32 startingIndex, RE::BGSListForm* list);
bool IsFormInList(RE::TESForm* form, RE::BGSListForm* list);

void SetWheelZoomEnabled(bool enbled);
void SetFavoritesEnabled(bool enabled);
void SetMovementEnabled(bool enabled);
