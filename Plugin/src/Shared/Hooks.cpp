#include "Shared/Hooks.h"

namespace Cascadia
{
	namespace Hooks
	{
		void Install(F4SE::Trampoline& trampoline)
		{
			// GetInfoForPlayerDialogueOptionHook - 2196817 ID + 0x40A offset
			typedef RE::TESTopicInfo(GetCurrentTopicInfo_Player_Sig)(RE::BGSSceneActionPlayerDialogue* apPlayerDialogue, RE::BGSScene* apParentScene, RE::TESObjectREFR* apTarget, std::uint32_t aeType);
			REL::Relocation<GetCurrentTopicInfo_Player_Sig> GetCurrentTopicInfo_Player_Location{ REL::ID(2196817), 0x40A };
			trampoline.write_call<5>(GetCurrentTopicInfo_Player_Location.address(), &RE::GetCurrentTopicInfo_Player_Hook);
	
			// GetInfoForNPCResponseDialogOptionHook - 2196817 ID + 0x78E offset
			typedef RE::TESTopicInfo(GetCurrentTopicInfo_NPC_Sig)(RE::BGSSceneActionPlayerDialogue* apPlayerDialogue, RE::BGSScene* apParentScene, RE::TESObjectREFR* apTarget, std::uint32_t aeType);
			REL::Relocation<GetCurrentTopicInfo_NPC_Sig> GetCurrentTopicInfo_NPC_Location{ REL::ID(2196817), 0x7A1 };
			trampoline.write_call<5>(GetCurrentTopicInfo_NPC_Location.address(), &RE::GetCurrentTopicInfo_NPC_Hook);

			// GetNPCResponseInfoForOptionHook - 2196800 ID + 0x388 offset
			typedef RE::TESTopicInfo(GetCurrentTopicInfo_NPCAction_Hook)(RE::BGSSceneActionNPCResponseDialogue* apNPCDialogue, RE::BGSScene* apParentScene);
			REL::Relocation<GetCurrentTopicInfo_NPCAction_Hook> GetCurrentTopicInfo_NPCAction_Location{ REL::ID(2196800), 0x388 };
			trampoline.write_call<5>(GetCurrentTopicInfo_NPCAction_Location.address(), &RE::GetCurrentTopicInfo_NPCAction_Hook);
		}
	}
}