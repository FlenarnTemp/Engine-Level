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
