#include "Systems/Dialogue.h"
#include "Shared/SharedFunctions.h"

#include "RE/Bethesda/BSPointerHandle.h"

//DialogueHolder g_dialogueHolder;

namespace RE
{
	// Internal
	namespace {
		DialogueHolder g_dialogueHolder;

		std::pair<float, float> savedSubtitlePosition;

		// Event Handlers

	}

	void BuildDialogueMap(bool force)
	{
		BGSSceneActionPlayerDialogue* playerDialogue = GetCurrentPlayerDialogueAction();
		if (!playerDialogue) return;
		BGSScene* currentScene = GetPlayerCharacter()->GetCurrentScene();

		// If we already have a map for this scene + scene action then return.
		if (!force && g_dialogueHolder.scene == currentScene && g_dialogueHolder.playerDialogue == playerDialogue) return;

		g_dialogueHolder.dialogueMap.clear();

		for (std::uint32_t c = 0; c < 4; c++)
		{
			TESTopic* playerTopic = playerDialogue->responseTopics[c];
			TESTopic* npcTopic = playerDialogue->pNPCResponseTopics[c];
			std::uint32_t playerInfoCount = playerTopic->numTopicInfos;
			std::uint32_t npcInfoCount = npcTopic ? npcTopic->numTopicInfos : 0;

			// Loop through each player TopicInfo in the dialogue topic.
			for (std::uint32_t i = 0; i < playerInfoCount; i++)
			{
				TESTopicInfo* playerInfo = playerTopic->topicInfos[i];
				if (!playerInfo->responses.head && !playerInfo->dataInfo) continue; // Skip over infos with no content.

				std::vector<TESTopicInfo*> npcResponses;

				std::uint32_t nextPlayerInfoIndex;
				for (nextPlayerInfoIndex = i + 1; nextPlayerInfoIndex < playerInfoCount; nextPlayerInfoIndex++)
				{
					TESTopicInfo* nextPlayerInfo = playerTopic->topicInfos[nextPlayerInfoIndex];
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
				for (std::uint32_t k = i; k < nextPlayerInfoIndex; k++)
				{
					if (k < npcInfoCount)
					{
						TESTopicInfo* npcInfo = npcTopic->topicInfos[k];
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

			logger::info(FMT_STRING("For info {:08X}"), info.first->formID);
			auto const& responses = info.second;
			for (auto const& response : responses)
			{
				logger::info(FMT_STRING("Response {:08X}"), response->formID);
			}
		}
		logger::info("BuildDialogueMap - complete.");
	}

	// Return player response TopicInfos.
	std::vector<TESTopicInfo*> GetPlayerInfos()
	{
		BuildDialogueMap();
		std::vector<TESTopicInfo*> infos;
		for (auto const& info : g_dialogueHolder.dialogueMap)
		{
			infos.push_back(info.first);
		}
		return infos;
	}

	TESTopicInfo* GetPlayerInfo(BGSSceneActionPlayerDialogue* playerdialogue, std::uint32_t optionID)
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
	TESTopicInfo* GetNPCInfo(BGSSceneActionPlayerDialogue* playerDialogue, std::uint32_t optionID)
	{
		BuildDialogueMap();
		auto npcInfos = g_dialogueHolder.dialogueMap[optionID].second;
		logger::info("GetNPCInfo - first step.");

		TESTopicInfo* infoHolder;
		BSTArray<TESTopicInfo*> randomOptions{};
		std::uint32_t randomRoll;


		for (TESTopicInfo* info : npcInfos)
		{
			logger::info("GetNPCInfo - indexed.");

			if ((info->formFlags >> 5) & 1) continue;
			if ((info->data.flags & TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kSayOnce) && (info->data.flags & TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kDialogueInfoSaid)) continue;

			if (info->data.flags & TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kRandom)
			{
				if (EvaluateInfoConditions(info, playerDialogue, true)) {
					randomOptions.push_back(info);
				}

				if (info->data.flags & TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kRandomEnd)
				{
					if (randomOptions.size() == 1)
					{
						return randomOptions[0];
					}

					randomRoll = rand() & randomOptions.size();
					infoHolder = randomOptions[randomRoll];
					logger::info("GetNPCInfo - first return");
					return infoHolder;
				}
				continue;
			}

			logger::info("GetNPCInfo - second step.");

			if (!(info->data.flags.underlying() & static_cast<std::uint32_t>(TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kRandom)) && !(info->data.flags.underlying() & static_cast<std::uint32_t>(TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kRandomEnd)) && randomOptions.size() != 0)
			{
				logger::info("Entered random & randomEnd check.");
				if (randomOptions.size() == 1)
				{
					logger::info("GetNPCInfo - second return");
					return randomOptions[0];
				}

				randomRoll = rand() % randomOptions.size();
				infoHolder = randomOptions[randomRoll];
				logger::info("GetNPCInfo - third return");
				return infoHolder;
			}

			logger::info("Skipped random & randomEnd check.");

			if (EvaluateInfoConditions(info, playerDialogue, true)) {
				logger::info("GetNPCInfo - fourth return");
				return info;
			}
		}
		logger::info("GetNPCInfo - third step.");


		// Do a random roll if the last info topic is 'random' flagged, but not 'randomEnd'.
		if (randomOptions.size() != 0)
		{
			if (randomOptions.size() == 1)
			{
				logger::info("GetNPCInfo - fifth return");
				return randomOptions[0];
			}
			randomRoll = rand() % randomOptions.size();
			infoHolder = randomOptions[randomRoll];
			logger::info("GetNPCInfo - sixth return");
			return infoHolder;
		}

		// All infos failed their condition checks.
		logger::info("All infos failed their condition checks");
		return nullptr;
	}

	// Return the first NPC response info in a NPC Response scene action that passes its condition check.
	// Allows for infos to be grouped via info groups.
	TESTopicInfo* GetNPCResponseInfo(BGSSceneActionNPCResponseDialogue* a_npcDialogue, uint32_t optionID)
	{
		// Vector of vector of TESTopicInfo
		// infos[optionId][items inside the infogroup]
		std::vector<std::vector<TESTopicInfo*>> infos(optionID + 1, std::vector<TESTopicInfo*>());

		std::map<TESTopicInfo*, std::uint32_t> infoGroupMap;
		std::uint32_t index = 0; // The next available free slot in the vector.

		for (std::uint32_t c = 0; c < 4; c++) {
			TESTopic* topic = a_npcDialogue->responseTopics[c];
			std::uint32_t infoCount = topic ? topic->numTopicInfos : 0;

			// Loop through all TopicInfos while index <= optionID + 1
			for (std::uint32_t i = 0; i < infoCount && index <= optionID + 1; i++)
			{
				TESTopicInfo* info = topic->topicInfos[i];

				if (info->formFlags & TESTopicInfo::Flags::kInfoGroup)
				{
					// This is a info group.
					infoGroupMap[info] = index++;
				}
				else
				{
					if (TESTopicInfo* parentInfoGroup = info->GetParentInfoGroup())
					{
						// Info has a parent.
						auto idx = infoGroupMap.find(parentInfoGroup);
						if (idx != infoGroupMap.end())
						{
							// Put it into the parents slot.
							infos[idx->second].push_back(info);
						}
					}
					else {
						// No parent, add normally.
						if (index <= optionID)
						{
							infos[index++].push_back(info);
						}
						else
						{
							break;
						}
					}
				}
			}
		}

		// Evaluate conditions and pick the best one from the group.
		std::vector<TESTopicInfo*> infoGroup = infos[optionID];

		for (TESTopicInfo* info : infoGroup)
		{
			if (EvaluateInfoConditions(info, a_npcDialogue, true))
			{
				if (info->responses.head || info->dataInfo)
				{
					return info;
				}
				else {
					return nullptr; // Return nullptr if this is an empty info.
				}
			}
		}

		// Nothing was found.
		return nullptr;
	}

	BGSSceneActionNPCResponseDialogue* FindNextNPCResponseAction(BGSScene* scene, std::uint32_t currentPhase)
	{
		if (scene)
		{
			for (std::uint32_t i = 0; i < scene->actions.size(); i++)
			{
				BGSSceneAction* action = scene->actions[i];
				if (action->startPhase == currentPhase + 1)
				{
					// Check that the action is a NPC response action and that the alias is filled for this action.
					// If the alias is not filled, any conditions that check the subject will cause a CTD.
					if (action->GetActionType() == BGSSceneAction::kNPCResponse && GetActionREFR(scene, action))
					{
						return reinterpret_cast<BGSSceneActionNPCResponseDialogue*>(action);
					}
				}
			}
		}

		return nullptr;
	}

	// Returns the currently executing player dialogue cation, or NULL if no player dialogue action is currenctly active.
	BGSSceneActionPlayerDialogue* GetCurrentPlayerDialogueAction()
	{
		BGSScene* scene = GetPlayerCharacter()->GetCurrentScene();
		if (scene)
		{
			for (std::uint32_t i = 0; i < scene->actions.size(); i++)
			{
				BGSSceneAction* action = scene->actions[i];
				if (action->GetActionType() == BGSSceneAction::SCENE_ACTION_TYPE::kPlayerDialogue)
				{
					if (action->status & BGSSceneAction::Status::kRunning || IsSceneActionWithinPhase(action, scene->currentActivePhase))
					{
						return (BGSSceneActionPlayerDialogue*)(action);
					}
				}
			}
		}
		return nullptr;
	}

	bool SelectDialogueOption(std::uint32_t option) {

		if (!(MenuTopicManager::GetSingleton()->bAllowInput)) return false;
		if (auto playerDialogue = GetCurrentPlayerDialogueAction()) 
		{
			// We're using option 5 and up for additional options. (Options 5 = Option 0).
			GetPlayerCharacter()->SetLastDialogueInput(option + 5);
			return true;
		}
		else 
		{
			return false;
		}
	}

	TESObjectREFR* GetCurrentPlayerDialogueTarget()
	{
		if (auto playerDialogue = GetCurrentPlayerDialogueAction())
		{
			BGSScene* scene = GetPlayerCharacter()->GetCurrentScene();

			BSPointerHandle<TESObjectREFR>* targetHandle = 0;
			TESObjectREFR* targetREFR = nullptr;
			TESQuest::GetAliasedRef(scene->parentQuest, targetHandle, playerDialogue->actorID);
			auto REFRptr = targetHandle->get();

			if (targetHandle && REFRptr != nullptr)
			{
				targetREFR = REFRptr.get();
				return targetREFR;
			}
		}
	}

	// Check to see whether the given scene action is active in the given phase.
	bool IsSceneActionWithinPhase(BGSSceneAction* action, std::uint32_t phase)
	{
		return (action->startPhase <= phase && action->endPhase >= phase);
	}

	// Return the reference associated with the action.
	TESObjectREFR* GetActionREFR(BGSScene* scene, BGSSceneAction* action)
	{
		if (scene)
		{
			BSPointerHandle<TESObjectREFR>* targetHandle = 0;
			TESObjectREFR* targetREFR = nullptr;
			TESQuest::GetAliasedRef(scene->parentQuest, targetHandle, action->actorID);
			auto REFRptr = targetHandle->get();

			if (targetHandle && REFRptr != nullptr)
			{
				targetREFR = REFRptr.get();
				return targetREFR;
			}
		}

		return nullptr;
	}

	// Topic Infos
	bool EvaluateInfoConditions(TESTopicInfo* a_info, BGSSceneAction* a_action, bool swap)
	{
		TESCondition conditions = a_info->objConditions;
		if (!conditions)
		{
			return true;
		}

		logger::info("EvaluateInfoConditions - first step");

		BSPointerHandle<TESObjectREFR> targetHandle;
		TESObjectREFR* targetREFR = nullptr;
		BGSScene* scene = GetPlayerCharacter()->GetCurrentScene();

		logger::info("EvaluateInfoConditions - second step");
		if (scene)
		{
			TESQuest::GetAliasedRef(scene->parentQuest, &targetHandle, a_action->actorID); // TODO - Might go boom
		}
		logger::info("EvaluateInfoConditions - third step");

		auto REFRptr = targetHandle.get();

		if (targetHandle && REFRptr != nullptr)
		{
			targetREFR = REFRptr.get();
		}
		else
		{
			targetREFR = GetPlayerCharacter();
		}

		logger::info("EvaluateInfoConditions - fourth step");

		// Test against conditions - (subject = player)
		TESObjectREFR* refA, * refB;
		if (!swap)
		{
			refA = GetPlayerCharacter();
			refB = targetREFR;
		}
		else
		{
			refA = targetREFR;
			refB = GetPlayerCharacter();
		}

		return conditions.IsTrue(refA, refB);
	}

	std::vector<DialogueOption> GetDialogueOptions()
	{
		std::vector<DialogueOption> options;

		if (auto playerDialogue = GetCurrentPlayerDialogueAction())
		{
			logger::info("Found player dialogue.");
			std::vector<TESTopicInfo*> infos = GetPlayerInfos();

			BGSScene* currentScene = GetPlayerCharacter()->GetCurrentScene();

			for (std::uint32_t i = 0; i < infos.size(); i++)
			{
				logger::info("Indexed again!");

				TESTopicInfo* info = infos[i];
				if (!info) { continue; }

				TESTopicInfo* originalInfo = info;
				while (info->dataInfo)
				{
					info = info->dataInfo;
				}


				if ((info->data.flags & TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kSayOnce) && (info->data.flags & TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kDialogueInfoSaid))
				{
					logger::info("SayOnce & DialogueInfoSaid, continuing.");
					continue;
				}


				// Question, Positive, Negative, Neutral
				std::uint32_t vanillaDialogueOrder[] = { 3, 0, 1, 2 };

				logger::info("First step.");

				// Get prompt

				BGSLocalizedString prompt;

				auto idx = GetAllPromptMap().find(info);
				if (idx != GetAllPromptMap().end())
				{
					logger::info("Set prompt");
					prompt = idx->second;
				}

				logger::info("Second step.");


				// Get response and perform text replacement.
				const char* responseText = "";
				if (info->responses.head != nullptr) {

					responseText = info->responses.head->GetResponseText();
					BSStringT<char> response;
					response.Set(responseText, 500);

					logger::info("Third step.");


					if (info->parentTopic && info->parentTopic->ownerQuest)
					{
						//logger::info("Entered into doom.");
						BGSQuestInstanceText::ParseString(&response, info->parentTopic->ownerQuest, info->parentTopic->ownerQuest->currentInstanceID);
						//info->parentTopic->ownerQuest->instanceData[info->parentTopic->ownerQuest->currentInstanceID]->ParseString(response, info->parentTopic->ownerQuest, info->parentTopic->ownerQuest->currentInstanceID);
					}
					responseText = response.data();
				}
				logger::info("Fourth step.");


				// Get NPC response TopicInfo for dialogue cues.
				TESTopicInfo* npcResponseInfo = GetNPCInfo(playerDialogue, i);
				logger::info("Got a NPC response.");
				if (!npcResponseInfo)
				{
					logger::info("NPC Response is not null.");
					// No NPC response info - look one phase ahead (only) for a NPC response action.
					if (BGSSceneActionNPCResponseDialogue* npcResponseAction = FindNextNPCResponseAction(currentScene, currentScene->currentActivePhase))
					{
						logger::info("Got a BGSSceneActionNPCResponseDialogue.");
						// Another framework check...
						npcResponseInfo = GetNPCResponseInfo(npcResponseAction, i);
					}
				}

				logger::info("Fifth step.");

				// Get scene links for response.
				TOPIC_INFO_SCENEDATA* sceneData = npcResponseInfo ? GetSceneData(npcResponseInfo) : nullptr;

				logger::info("Sixth step.");

				DialogueOption option = {};
				option.optionID = i;
				option.info = info;
				option.prompText = prompt;
				option.reseponseText = responseText;
				option.enabled = EvaluateInfoConditions(originalInfo, playerDialogue, true);
				option.said = (info->data.flags.underlying() & static_cast<std::uint32_t>(TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kDialogueInfoSaid)) != 0;
				option.challengeLevel = info->GetSpeechChallengeLevel();
				option.challengeResult = info->GetSuccessLevel();
				option.linkedToSelf = sceneData ? (currentScene == sceneData->pScene && playerDialogue->startPhase >= sceneData->uiPhase && playerDialogue->endPhase <= sceneData->uiPhase) : false;
				option.endsScene = npcResponseInfo ? (npcResponseInfo->data.flags.underlying() & static_cast<std::uint32_t>(TOPIC_INFO_DATA::TOPIC_INFO_FLAGS::kEndRunningScene)) != 0 : false;
				option.isBarterOption = false; // npcResponseInfo ? TODO : false; -TODO
				option.isInventoryOption = false; // npcResponseInfo ? TODO : false; - TODO
				options.push_back(option);
			}
		}

		logger::info(FMT_STRING("GetDialogueOptions: Got {:s} options when checking scene {:s}"), std::to_string(options.size()), GetPlayerCharacter()->GetCurrentScene()->GetFormEditorID());
		return options;
	}

	TOPIC_INFO_SCENEDATA* GetSceneData(TESTopicInfo* topicInfo)
	{
		auto entry = GetAllSceneListMap().find(topicInfo);
		if (entry != GetAllSceneListMap().end()) {
			return &(entry->second);
		}

		return nullptr;
	}

	void SetSceneData(TESTopicInfo* topicInfo, BGSScene* scene, std::uint32_t phase)
	{
		if (!topicInfo) return;

		auto& sceneListMap = GetAllSceneListMap();
		auto sceneDataIter = sceneListMap.find(topicInfo);

		if (sceneDataIter != sceneListMap.end())
		{
			// Update existing TOPIC_INFO_SCENEDATA
			sceneDataIter->second.pScene = scene;
			sceneDataIter->second.uiPhase = phase;
		}
		else
		{
			TOPIC_INFO_SCENEDATA newSceneData;
			newSceneData.pScene = scene;
			newSceneData.uiPhase = phase;
			sceneListMap.emplace(topicInfo, newSceneData);
		}
	}
}
