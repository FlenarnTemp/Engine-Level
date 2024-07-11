#pragma once

#include "Shared/SharedFunctions.h"

#include "RE/Bethesda/BSPointerHandle.h"

namespace RE
{
	struct DialogueOption
	{
		std::uint32_t optionID;
		TESTopicInfo* info;
		std::string prompText;
		std::string responseText;
		bool enabled;
		bool said;
		std::uint32_t challengeLevel;	// 0: No speech challenge.
		int32_t challengeResult;		// -1: Not attempted, 0: Failed, 1: Succeeded. For repeatable challenges, always -1.
		bool linkedToSelf;				// Whether or not this dialogue option links to itself.
		bool endsScene;					// Whether or not this dialogue option will end the scene.
		bool isBarterOption;			// Whether or not this dialogue option has the VendorInfoScript attatched.
		bool isInventoryOption;			// Whether or not this dialogue option has the OpenInventoryInfoScript attatched.
	};

	struct DialogueHolder
	{
		BGSScene* scene;
		BGSSceneActionPlayerDialogue* playerDialogue;

		std::vector<std::pair<TESTopicInfo*, std::vector<TESTopicInfo*>>> dialogueMap;
	};

	struct TOPIC_INFO_SCENEDATA
	{
		BGSScene* pScene;
		std::uint32_t uiPhase;
	};

	static_assert(sizeof(TOPIC_INFO_SCENEDATA) == 0x10);

	[[nodiscard]] inline BSTHashMap<const TESTopicInfo*, BGSLocalizedString>& GetAllPromptMap()
	{
		REL::Relocation<BSTHashMap<const TESTopicInfo*, BGSLocalizedString>*> AllPromptMap{ REL::ID(2662553), -0x28 };
		return *AllPromptMap;
	}

	[[nodiscard]] inline BSTHashMap<const TESTopicInfo*, TOPIC_INFO_SCENEDATA>& GetAllSceneListMap()
	{
		REL::Relocation<BSTHashMap<const TESTopicInfo*, TOPIC_INFO_SCENEDATA>*> AllSceneListsMap{ REL::ID(2662565), -0x28 };
		return *AllSceneListsMap;
	}

	bool IsSceneActionWithinPhase(BGSSceneAction* action, std::uint32_t phase);
	TESObjectREFR* GetActionREFR(BGSScene* scene, BGSSceneAction* action);
	bool EvaluateInfoConditions(TESTopicInfo* a_info, BGSSceneAction* a_action, bool swap = false);
	TOPIC_INFO_SCENEDATA* GetSceneData(TESTopicInfo* topicInfo);

	BGSSceneActionPlayerDialogue* GetCurrentPlayerDialogueAction();

	bool SelectDialogueOption(std::uint32_t option);
	TESObjectREFR* GetCurrentPlayerDialogueTarget();

	std::vector<DialogueOption> GetDialogueOptions();
	void BuildDialogueMap(bool force = false);
	std::vector<TESTopicInfo*> GetPlayerInfos();
	TESTopicInfo* GetPlayerInfo(BGSSceneActionPlayerDialogue* playerDialogue, std::uint32_t optionID);
	TESTopicInfo* GetNPCInfo(BGSSceneActionPlayerDialogue* playerDialogue, std::uint32_t optionID);
}
