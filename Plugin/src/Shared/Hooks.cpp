#include "Shared/Hooks.h"
namespace RE
{
	namespace Cascadia
	{
		namespace Hooks
		{
			void Install(F4SE::Trampoline& trampoline)
			{
				// GetInfoForPlayerDialogueOptionHook - 2196817 ID + 0x40A offset
				typedef TESTopicInfo(GetCurrentTopicInfo_Player_Sig)(BGSSceneActionPlayerDialogue* apPlayerDialogue, BGSScene* apParentScene, TESObjectREFR* apTarget, std::uint32_t aeType);
				REL::Relocation<GetCurrentTopicInfo_Player_Sig> GetCurrentTopicInfo_Player_Location{ REL::ID(2196817), 0x40A };
				trampoline.write_call<5>(GetCurrentTopicInfo_Player_Location.address(), &RE::GetCurrentTopicInfo_Player_Hook);

				// GetInfoForNPCResponseDialogOptionHook - 2196817 ID + 0x78E offset
				typedef TESTopicInfo(GetCurrentTopicInfo_NPC_Sig)(BGSSceneActionPlayerDialogue* apPlayerDialogue, BGSScene* apParentScene, TESObjectREFR* apTarget, std::uint32_t aeType);
				REL::Relocation<GetCurrentTopicInfo_NPC_Sig> GetCurrentTopicInfo_NPC_Location{ REL::ID(2196817), 0x7A1 };
				trampoline.write_call<5>(GetCurrentTopicInfo_NPC_Location.address(), &RE::GetCurrentTopicInfo_NPC_Hook);

				// GetNPCResponseInfoForOptionHook - 2196800 ID + 0x388 offset
				typedef TESTopicInfo(GetCurrentTopicInfo_NPCAction_Hook)(BGSSceneActionNPCResponseDialogue* apNPCDialogue, BGSScene* apParentScene);
				REL::Relocation<GetCurrentTopicInfo_NPCAction_Hook> GetCurrentTopicInfo_NPCAction_Location{ REL::ID(2196800), 0x388 };
				trampoline.write_call<5>(GetCurrentTopicInfo_NPCAction_Location.address(), &RE::GetCurrentTopicInfo_NPCAction_Hook);
			}

			DetourXS hook_SetHealthPerc;
			typedef void(SetHealthPercSig)(ExtraDataList*, float);
			REL::Relocation<SetHealthPercSig> SetHealthPercOriginal;

			void HookExtraDataListSetHealthPerc(ExtraDataList* a_this, float a_health)
			{
				SetHealthPercOriginal(a_this, a_health);
				if (a_health == 1.0f && a_this->GetHealthPerc() != 1.0f)
				{
					a_this->AddExtra(new ExtraHealth(1.0f));
				}
			}

			DetourXS hook_GetInventoryValue;
			typedef std::int64_t(GetInventoryValueSig)(TESBoundObject*, const ExtraDataList*);
			REL::Relocation<GetInventoryValueSig> GetInventoryValueOriginal;

			int64_t HookGetInventoryValue(TESBoundObject* a_baseObj, const ExtraDataList* a_extra)
			{
				if (!a_extra->HasType(EXTRA_DATA_TYPE::kHealth))
				{
					return GetInventoryValueOriginal(a_baseObj, a_extra);
				}

				ExtraDataList* non_const_a_extra = const_cast<ExtraDataList*>(a_extra);
				int64_t newValue = int64_t(GetInventoryValueOriginal(a_baseObj, a_extra) * non_const_a_extra->GetHealthPerc() * std::sqrt(non_const_a_extra->GetHealthPerc()));
				return newValue;
			}

			DetourXS hook_AddItem;
			typedef void(AddItemSig)(BGSInventoryList*, TESBoundObject*, const BGSInventoryItem::Stack*, std::uint32_t*, std::uint32_t*);
			REL::Relocation<AddItemSig> AddItemOriginal;

			void HookBGSInventoryListAddItem(BGSInventoryList* a_this, TESBoundObject* a_boundObject, const BGSInventoryItem::Stack* a_stack, std::uint32_t* a_oldCount, std::uint32_t* a_newCount)
			{
				ENUM_FORM_ID formType = a_boundObject->GetFormType();
				if (a_boundObject)
				{
					if (formType == ENUM_FORM_ID::kWEAP || formType == ENUM_FORM_ID::kARMO)
					{
						std::uint32_t iterCount = 0;
						for (const BGSInventoryItem::Stack* traverse = a_stack; traverse; traverse->nextStack)
						{
							if (!traverse || !traverse->extra)
							{
								break;
							}

							// TODO - keyword check for no-degradation.

							if (formType == ENUM_FORM_ID::kWEAP)
							{
								TESObjectWEAP* tempREFR = static_cast<TESObjectWEAP*>(a_boundObject);
								if (tempREFR->weaponData.type == WEAPON_TYPE::kGrenade || tempREFR->weaponData.type == WEAPON_TYPE::kMine)
								{
									DEBUG("BGSInventoryList::AddItem: REFR grenade/mine weapon type.");
									break;
								}
							}

							// 'break' here works, as when native function gets called for weapons/armor stack only contains one object.
							if (traverse->extra.get())
							{
								// GetHealthPerc return -1.0 if it can't find the kHealth type.
								if (traverse->extra->GetHealthPerc() < 0)
								{
									traverse->extra->SetHealthPerc(BSRandom::Float(0.55f, 0.85f));
									INFO("BGSInventoryList::AddItem: Health initialized: {:s}", std::to_string(traverse->extra->GetHealthPerc()));
									break;
								}
								else
								{
									INFO("BGSInventoryList::AddItem: Health already initialized: {:s}", std::to_string(traverse->extra->GetHealthPerc()));
									break;
								}
							}

							iterCount++;

							if (iterCount > traverse->count - 1)
							{
								break;
							}
						}
					}
				}
				AddItemOriginal(a_this, a_boundObject, a_stack, a_oldCount, a_newCount);
			}

			void RegisterAddItemHook()
			{
				REL::Relocation<AddItemSig> functionLocation{ REL::ID(2194159) };
				if (hook_AddItem.Create(reinterpret_cast<LPVOID>(functionLocation.address()), &HookBGSInventoryListAddItem))
				{
					DEBUG("Installed BGSInventoryList::AddItem hook.");
					AddItemOriginal = reinterpret_cast<uintptr_t>(hook_AddItem.GetTrampoline());
				}
				else
				{
					FATAL("Failed to hook BGSInventoryList::AddItem, exiting.");
				}
			}

			void RegisterSetHealthPercHook()
			{
				REL::Relocation<SetHealthPercSig> functionLocation{ REL::ID(2190124) };
				if (hook_SetHealthPerc.Create(reinterpret_cast<LPVOID>(functionLocation.address()), &HookExtraDataListSetHealthPerc))
				{
					DEBUG("Installed ExtraDataList::SetHealthPerc hook.");
					SetHealthPercOriginal = reinterpret_cast<uintptr_t>(hook_SetHealthPerc.GetTrampoline());
				}
				else
				{
					FATAL("Failed to hook ExtraDataList::SetHealthPerc, exiting.");
				}
			}

			void RegisterGetInventoryValueHook()
			{
				REL::Relocation<GetInventoryValueSig> functionLocation{ REL::ID(2194127) };
				if (hook_GetInventoryValue.Create(reinterpret_cast<LPVOID>(functionLocation.address()), &HookGetInventoryValue))
				{
					DEBUG("Installed BGSInventoryItemUtils::GetInventoryValue hook.");
					GetInventoryValueOriginal = reinterpret_cast<uintptr_t>(hook_GetInventoryValue.GetTrampoline());
				}
				else
				{
					FATAL("Failed to hook BGSInventoryItemUtils::GetInventoryValue, exiting.");
				}
			}
		}
	}
}
