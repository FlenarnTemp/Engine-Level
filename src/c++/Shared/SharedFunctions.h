#pragma once

#include <Systems/ItemDegradation.h>

bool IsFormIDStringBaseGame(std::string formIDString);
std::string GetFormIDAsString(uint32_t formID);

uint32_t GetActorSex(RE::Actor* a_actor);
RE::PlayerCharacter* GetPlayerCharacter();
bool IsPlayerGodMode();
int16_t GetPlayerLevel();
uint32_t GetPlayerSex();

std::string FloatToPreciseString(float value, int precision);

uint32_t RNG(int min, int max);
float RNG(float min, float max);

bool WeaponHasKeyword(RE::TESObjectWEAP* weapon, RE::BGSKeyword* keyword);
bool ArmorHasKeyword(RE::TESObjectARMO* armor, RE::BGSKeyword* keyword);
bool ReferenceHasKeyword(RE::TESObjectREFR* ref, RE::BGSKeyword* keyword);

bool NPCHasKeyword(RE::TESNPC* npc, RE::BGSKeyword* keyword);
bool IsInMenuMode();

uint32_t PositionInFormList(RE::TESForm* form, RE::BGSListForm* list);
uint32_t GetNextAvailableFormInInventoryFromList(UINT32 startingIndex, RE::BGSListForm* list);
bool IsFormInList(RE::TESForm* form, RE::BGSListForm* list);

void SetWheelZoomEnabled(bool enbled);
void SetFavoritesEnabled(bool enabled);
void SetMovementEnabled(bool enabled);

bool IsMeleeWeapon(WeaponConditionData myConditionData);
float CalculateSkillBonusFromActor(WeaponConditionData myConditionData);

RE::Setting* GetGMST(const char* a_name);
