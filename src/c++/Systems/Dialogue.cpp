#include "Systems/Dialogue.h"
#include "Shared/SharedFunctions.h"

DialogueHolder g_dialogueHolder;

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
		RE::TESTopic* playerTopic = playerDialogue->response[c];
		RE::TESTopic* npcTopic = playerDialogue->npcResponse[c];
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
					// We found it!
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

/**RE::TESTopicInfo* GetPlayerInfo(RE::BGSSceneActionPlayerDialogue* playerDialogue, uint32_t optionID)
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
}*/

// Returns the first NPC response info that passes its condition check.
/**RE::TESTopicInfo* GetNPCInfo(RE::BGSSceneActionPlayerDialogue* playerDialogue, uint32_t optionID)
{
	BuildDialogueMap();
	auto npcInfos = g_dialogueHolder.dialogueMap[optionID].second;

	RE::TESTopicInfo* infoHolder;
	
}*/

// Returns the currently executing player dialogue cation, or NULL if no player dialogue action is currenctly active.
RE::BGSSceneActionPlayerDialogue* GetCurrentPlayerDialogueAction()
{
	RE::BGSScene* scene = GetPlayerCharacter()->GetCurrentScene();
	if (scene)
	{
		for (uint32_t i = 0; i < scene->actions.size(); i++)
		{
			RE::BGSSceneAction* action = scene->actions[i];
			if (action->GetType() == RE::BGSSceneAction::Type::kPlayerDialogue)
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
