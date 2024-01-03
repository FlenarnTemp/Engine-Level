#include "Systems/Dialogue.h"
#include "Shared/SharedFunctions.h"

#include "RE/Bethesda/BSPointerHandle.h"

//DialogueHolder g_dialogueHolder;

/*
void BuildDialogueMap(bool force)
{
	RE::BGSSceneActionPlayerDialogue* playerDialogue = GetCurrentPlayerDialogueAction();
	if (!playerDialogue) return;
	RE::BGSScene* currentScene = GetPlayerCharacter()->GetCurrentScene();

	// If we already have a map for this scene + scene action then return.
	if (!force && g_dialogueHolder.scene == currentScene && g_dialogueHolder.playerDialogue == playerDialogue) return;

	g_dialogueHolder.dialogueMap.clear();

	for (std::uint32_t c = 0; c < 4; c++)
	{
		RE::TESTopic* playerTopic = playerDialogue->responseTopics[c];
		RE::TESTopic* npcTopic = playerDialogue->pNPCResponseTopics[c];
		uint32_t playerInfoCount = playerTopic->numTopicInfos;
		uint32_t npcInfoCount = npcTopic ? npcTopic->numTopicInfos : 0;

		// Loop through each player TopicInfo in the dialogue topic.
		for (uint32_t i = 0; i < playerInfoCount; i++)
		{
			RE::TESTopicInfo* playerInfo = playerTopic->topicInfos[i];
			if (!playerInfo->responses.head && !playerInfo->dataInfo) continue; // Skip over infos with no content.

			std::vector<RE::TESTopicInfo*> npcResponses;

			uint32_t nextPlayerInfoIndex;
			for (nextPlayerInfoIndex = i + 1; nextPlayerInfoIndex < playerInfoCount; nextPlayerInfoIndex++)
			{
				RE::TESTopicInfo* nextPlayerInfo = playerTopic->topicInfos[nextPlayerInfoIndex];
				if (nextPlayerInfo->responses.head || nextPlayerInfo->dataInfo)
				{
					// We found it, yay!
					break;
				}
			}

			if (nextPlayerInfoIndex == playerInfoCount && npcInfoCount > playerInfoCount)
			{
				// Reached the end of the player info list but there are still NPC infos.
				// Associate the remaining NPC infos with the current player info index.
				nextPlayerInfoIndex = npcInfoCount;
			}

			// Add all NPC infos from current player info index to next plyaer info index.
			for (uint32_t k = i; k < nextPlayerInfoIndex; k++)
			{
				if (k < npcInfoCount)
				{
					RE::TESTopicInfo* npcInfo = npcTopic->topicInfos[k];
					npcResponses.push_back(npcInfo);
				}
			}

			g_dialogueHolder.dialogueMap.push_back(std::make_pair(playerInfo, npcResponses));
		}
	}

	g_dialogueHolder.scene = currentScene;
	g_dialogueHolder.playerDialogue = playerDialogue;

	// Debug View Map
	for (auto const& info : g_dialogueHolder.dialogueMap)
	{
		logger::info("For info %08X", info.first->formID);
		auto const& responses = info.second;
		for (auto const& response : responses)
		{
			logger::info("Response: %08X", response->formID);
		}
	}
}

// Return player response TopicInfos.
std::vector<RE::TESTopicInfo*> GetPlayerInfos()
{
	BuildDialogueMap();
	std::vector<RE::TESTopicInfo*> infos;
	for (auto const& info : g_dialogueHolder.dialogueMap)
	{
		infos.push_back(info.first);
	}
	return infos;
}

RE::TESTopicInfo* GetPlayerInfo(RE::BGSSceneActionPlayerDialogue* playerdialogue, uint32_t optionID)
{
	BuildDialogueMap();
	if (optionID < g_dialogueHolder.dialogueMap.size())
	{
		return g_dialogueHolder.dialogueMap[optionID].first;
	}
	else
	{
		return nullptr;
	}
}

// Returns the first NPC response info that passes its condition check.
RE::TESTopicInfo* GetNPCInfo(RE::BGSSceneActionPlayerDialogue* playerDialogue, uint32_t optionID)
{
	BuildDialogueMap();
	auto npcInfos = g_dialogueHolder.dialogueMap[optionID].second;

	RE::TESTopicInfo* infoHolder;
	RE::BSTArray<RE::TESTopicInfo*> randomOptions{};
	std::uint32_t randomRoll;

	for (RE::TESTopicInfo* info : npcInfos)
	{
		if ((info->formFlags >> 5) & 1) continue;
		if ((info->data.flags & RE::TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kSayOnce) && (info->data.flags & RE::TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kDialogueInfoSaid)) continue;

		if (info->data.flags & RE::TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kRandom)
		{
			// TODO - EvaluateInfoCondition

			if (info->data.flags & RE::TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kRandomEnd)
			{
				if (randomOptions.size() == 1)
				{
					return randomOptions[0];
				}

				randomRoll = rand() & randomOptions.size();
				infoHolder = randomOptions[randomRoll];

				return infoHolder;
			}
			continue;
		}

		if (!(info->data.flags & RE::TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kRandom) && !(info->data.flags & RE::TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kRandomEnd) && randomOptions.size() != 0)
		{
			if (randomOptions.size() == 1)
			{
				return randomOptions[0];
			}

			randomRoll = rand() % randomOptions.size();
			infoHolder = randomOptions[randomRoll];

			return infoHolder;
		}

		if (EvaluateInfoConditions(info, playerDialogue, true)) {
			return info;
		}
	}

	// Do a random roll if the last info topic is 'random' flagged, but not 'randomEnd'.
	if (randomOptions.size() != 0)
	{
		if (randomOptions.size() == 1)
		{
			return randomOptions[0];
		}
		randomRoll = rand() % randomOptions.size();
		infoHolder = randomOptions[randomRoll];
		return infoHolder;
	}

	// All infos failed their condition checks.
	logger::info("All infos failed their condition checks");
	return nullptr;
}

// Return the first NPC response info in a NPC Response scene action that passes its condition check.
// Allows for infos to be grouped via info groups.
RE::TESTopicInfo* GetNPCResponseInf(RE::BGSSceneActionNPCResponseDialogue* a_npcDialogue, uint32_t optionID)
{
	// Vector of vector of TESTopicInfo
	// infos[optionId][items inside the infogroup]
	std::vector<std::vector<RE::TESTopicInfo*>> infos(optionID + 1, std::vector<RE::TESTopicInfo*>());

	std::map<RE::TESTopicInfo*, int> infoGroupMap;
	uint32_t index = 0; // The next available free slot in the vector.

	for (uint32_t c = 0; c < 4; c++) {
		RE::TESTopic* topic = a_npcDialogue->responseTopics[c];
		uint32_t infoCount = topic ? topic->numTopicInfos : 0;

		// Loop through all TopicInfos while index <= optionID + 1
		for (uint32_t i = 0; i < infoCount && index <= optionID + 1; i++) {
			RE::TESTopicInfo* info = topic->topicInfos[i];

			if (info->formFlags & RE::TESTopicInfo::Flags::kInfoGroup) {
				// This is a info group.
				infoGroupMap[info] = index++;
			} else {
				if (RE::TESTopicInfo* parentInfoGroup = info->GetParentInfoGroup()) {
					// Info has a parent.
					auto idx = infoGroupMap.find(parentInfoGroup);
					if (idx != infoGroupMap.end()) {
						// Put it into the parents slot.
						infos[idx->second].push_back(info);
					}
				} else {
					// No parent, add normally.
					if (index <= optionID) {
						infos[index++].push_back(info);
					} else {
						break;
					}
				}
			}
		}
	}

	// Evaluate conditions and pick the best one from the group.
	std::vector<RE::TESTopicInfo*> infoGroup = infos[optionID];

	for (RE::TESTopicInfo* info : infoGroup) {
		if (EvaluateInfoConditions(info, a_npcDialogue, true)) {
			if (info->responses.head || info->dataInfo) {
				return info;
			} else {
				return nullptr; // Return nullptr if this is an empty info.
			}
		}
	}

	// Nothing was found.
	return nullptr;
}

// Returns the currently executing player dialogue cation, or NULL if no player dialogue action is currenctly active.
RE::BGSSceneActionPlayerDialogue* GetCurrentPlayerDialogueAction()
{
	RE::BGSScene* scene = GetPlayerCharacter()->GetCurrentScene();
	if (scene)
	{
		for (uint32_t i = 0; i < scene->actions.size(); i++)
		{
			RE::BGSSceneAction* action = scene->actions[i];
			if (action->GetActionType() == RE::BGSSceneAction::SCENE_ACTION_TYPE::kPlayerDialogue)
			{
				if (action->status & RE::BGSSceneAction::Status::kRunning || IsSceneActionWithinPhase(action, scene->currentActivePhase))
				{
					return (RE::BGSSceneActionPlayerDialogue*)(action);
				}
			}
		}
	}
	return nullptr;
}

// Check to see whether the given scene action is active in the given phase.
bool IsSceneActionWithinPhase(RE::BGSSceneAction* action, std::uint32_t phase)
{
	return (action->startPhase <= phase && action->endPhase >= phase);
}

// Topic Infos
bool EvaluateInfoConditions(RE::TESTopicInfo* a_info, RE::BGSSceneAction* a_action, bool swap)
{
	RE::TESCondition conditions = a_info->objConditions;
	if (!conditions) {
		return true;
	}

	RE::BSPointerHandle<RE::TESObjectREFR>* targetHandle;
	RE::TESObjectREFR* targetRef = nullptr;
	RE::BGSScene* scene = GetPlayerCharacter()->GetCurrentScene();

	if (scene) {
		scene->parentQuest->GetAliasedRef(targetHandle, a_action->actorID); // Might go boom
	}

	if (targetHandle) {
		// TODO
	}
	else {
		targetRef = GetPlayerCharacter();
	}

	// Test against conditions - (subject = player)
	RE::TESObjectREFR *refA, *refB;
	if (!swap) {
		refA = GetPlayerCharacter();
		refB = targetRef;
	}
	else {
		refA = targetRef;
		refB = GetPlayerCharacter();
	}

	return conditions.IsTrue(refA, refB);
}

std::vector<DialogueOption> GetDialogueOptions()
{
	std::vector<DialogueOption> options;

	if (auto playerDialogue = GetCurrentPlayerDialogueAction()) {
		// "Retail" XDI has a IsFrameworkActive() check here, with some logic to go along with it, we do not, we do not care about retail game. Sorry Miami.

		std::vector<RE::TESTopicInfo*> infos = GetPlayerInfos();

		RE::BGSScene* currentScene = GetPlayerCharacter()->GetCurrentScene();

		for (uint32_t i = 0; i < infos.size(); i++) {
			RE::TESTopicInfo* info = infos[i];
			if (!info) { continue; }

			RE::TESTopicInfo* originalInfo = info;
			while (info->dataInfo) {
				info = info->dataInfo;
			}

			// Another IsFrameworkActive() check should go here... Sorry Loaf one.
			// Skip "Say Once" marked infos that have already been said.
			if ((info->data.flags & RE::TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kSayOnce) && (info->data.flags & RE::TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kDialogueInfoSaid)) {
				continue;
			}

			// Question, Positive, Negative, Neutral
			int vanillaDialogueOrder[] = { 3, 0, 1, 2 };

			// Get prompt
			//auto prompt = ; - TODO

			// Get response and perform text replacement.
			std::string responseText = "";
			/*if (info->responses) {
				std::string str(info->responses.head) - TODO
			}*/

			// Get NPC response TopicInfo for dialogue cues.
			// Another IsFrameworkActive() check goes here in "retail".

			/**RE::TESTopicInfo* npcResponseInfo = GetNPCInfo(playerDialogue, i);
			if (!npcResponseInfo) {
				// No NPC response info - look one phase ahead (only) for a NPC response action.
				if (auto npcResponseAction = FindNextNPCResponseAction(currentScene, currentScene->currentActivePhase)) {
					// Another framework check...
					npcResponseInfo = GetNPCResponseInfo(npcResponseAction, i);
				}
			}

			// Get scene links for response.
			SceneLink* sceneLink = npcResponseInfo ? GetSceneLink(npcResponseInfo) : nullptr;

			DialogueOption option = {};
			option.optionID = i;
			option.info = info;
			//option.prompText = prompt ? prompt->prompt.c_str() : "";
			option.reseponseText = responseText;
			option.enabled = EvaluateInfoConditions(originalInfo, playerDialogue, true);
			option.said = (info->data.flags.get() & RE::TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kDialogueInfoSaid) != 0;
			option.challengeLevel = info->GetSpeechChallengeLevel();
			option.challengeResult = info->GetSuccessLevel();
			option.linkedToSelf = sceneLink ? (currentScene == sceneLink->scene && playerDialogue->startPhase >= sceneLink->phase && playerDialogue->endPhase <= sceneLink->phase) : false;
			option.endsScene = npcResponseInfo ? (npcResponseInfo->data.flags.get() & RE::TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kEndRunningScene) != 0 : false;
			//option.isBarterOption = npcResponseInfo ? TODO : false; - TODO
			//option.isInventoryOption = npcResponseInfo ? TODO : false; - TODO
			options.push_back(option);
		}
	}

	logger::info("GetDialogueOptions: Got %i options when checking scene %s", options.size(), GetFormIDAsString(GetPlayerCharacter()->GetCurrentScene()->formID).c_str());

	return options;
}

// Internal
namespace {
	DialogueHolder g_dialogueHolder;

	std::pair<float, float> savedSubtitlePosition;

	// Event Handlers
	
}

**/
