#pragma once
/**
struct DialogueOption
{
	std::uint32_t optionID;
	RE::TESTopicInfo* info;
	const char* prompText;
	std::string reseponseText;
	bool enabled;
	bool said;
	std::uint32_t challengeLevel;
	std::uint32_t challengeResult;
	bool linkedToSelf;
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

// 18
struct SceneLink
{
	RE::TESTopicInfo* key;	// 00
	RE::BGSScene* scene;	// 08
	uint32_t phase;			// 10
	uint32_t pad14;			// 14

	operator RE::TESTopicInfo* () const { return key; }
};

extern DialogueHolder g_dialogueHolder;

bool IsSceneActionWithinPhase(RE::BGSSceneAction* action, uint32_t phase);
bool EvaluateInfoConditions(RE::TESTopicInfo* a_info, RE::BGSSceneAction* a_action, bool swap);

RE::BGSSceneActionPlayerDialogue* GetCurrentPlayerDialogueAction();


// Topic Info
void BuildDialogueMap(bool force = false);
std::vector<RE::TESTopicInfo*> GetPlayerInfos();
RE::TESTopicInfo* GetPlayerInfo(RE::BGSSceneActionPlayerDialogue* playerDialogue, uint32_t optionID);
RE::TESTopicInfo* GetNPCInfo(RE::BGSSceneActionPlayerDialogue* playerDialogue, uint32_t optionID);
*/
