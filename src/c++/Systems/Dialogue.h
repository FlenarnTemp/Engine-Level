#pragma once

struct DialogueOption
{
	int optionID;
	RE::TESTopicInfo* info;
	const char* prompText;
	std::string reseponseText;
	bool enabled;
	bool said;
	int challengeLevel;
	int challengeResult;
	bool linkedToSeld;
	bool endsScene;
	bool isBarterOption;
	bool isInventoryOption;
};

struct DialogueHolder
{
	RE::BGSScene* scene;
	RE::BGSSceneActionPlayerDialogue* playerDialogue;

	std::vector<std::pair<RE::TESTopicInfo*, std::vector<RE::TESTopicInfo*>>> dialogueMap;
};

extern DialogueHolder g_dialogueHolder;

bool IsSceneActionWithinPhase(RE::BGSSceneAction* action, std::uint32_t phase);

RE::BGSSceneActionPlayerDialogue* GetCurrentPlayerDialogueAction();


