#include "Menus/Scaleform.h"
#include "Systems/Dialogue.h"

std::pair<double, double> GetSubtitlePosition()
{
	std::pair<double, double> position;

	RE::BSFixedString menuString("HUDMenu");
	if (RE::UI::GetSingleton()->GetMenuOpen(menuString)) 
	{
		RE::IMenu* menu = RE::UI::GetSingleton()->GetMenu(menuString).get();
		RE::Scaleform::Ptr<RE::Scaleform::GFx::ASMovieRootBase> movieRoot = menu->uiMovie->asMovieRoot;

		RE::Scaleform::GFx::Value subtitleX; movieRoot->GetVariable(&subtitleX, "root.BottomCenterGroup_mc.SubtitleText_mc.x");
		RE::Scaleform::GFx::Value subtitleY; movieRoot->GetVariable(&subtitleY, "root.BottomCenterGroup_mc.SubtitleText_mc.y");

		position.first = subtitleX.GetNumber();
		position.second = subtitleY.GetNumber();
	}
	else
	{
		logger::warn("WARNING: Unable to retrieve the subtitle position because 'HUDMenu' is not open.");
	}

	return position;
}

bool SetSubtitlePosition(double x, double y)
{
	RE::BSFixedString menuString("HUDMenu");
	if (RE::UI::GetSingleton()->GetMenuOpen(menuString))
	{
		RE::IMenu* menu = RE::UI::GetSingleton()->GetMenu(menuString).get();
		RE::Scaleform::Ptr<RE::Scaleform::GFx::ASMovieRootBase> movieRoot = menu->uiMovie->asMovieRoot;

		RE::Scaleform::GFx::Value subtitle; movieRoot->GetVariable(&subtitle, "root.BottomCenterGroup_mc,SubtitleText_mc");
		subtitle.SetMember("x", RE::Scaleform::GFx::Value(x));
		subtitle.SetMember("y", RE::Scaleform::GFx::Value(y));
		return true;
	}

	return false;
}

void RegisterFunction_DialogueMenu(RE::Scaleform::GFx::Value codeObj, RE::Scaleform::Ptr<RE::Scaleform::GFx::ASMovieRootBase> movieRoot) {
	//RE::Scaleform::GFx::ASMovieRootBase::CreateFunction();
}

bool RegisterScaleform(RE::Scaleform::GFx::Movie* movie, RE::Scaleform::GFx::Value* f4se_root)
{
	RE::Scaleform::Ptr<RE::Scaleform::GFx::ASMovieRootBase> movieRoot = movie->asMovieRoot;

	RE::Scaleform::GFx::Value currentSWFPath;
	const char* currentSWFPathString = nullptr;

	if (movieRoot->GetVariable(&currentSWFPath, "root.loaderInfo.url")) {
		currentSWFPathString = currentSWFPath.GetString();
	} else {
		logger::warn("WARNING: Scaleform registration failed.");
	}

	// Register native code handlers.
	if (strcmp(currentSWFPathString, "Interface/DialogueMenu.swf") == 0) {
		RE::Scaleform::GFx::Value codeObj;
		movieRoot->GetVariable(&codeObj, "root.Menu_mc.BGSCodeObj");
		//RegisterFunctions_DialogueMenu(&codeObj, movieRoot); - TODO

		movieRoot->Invoke("root.XDI_Init", nullptr, nullptr, 0);

	} else if (strcmp(currentSWFPathString, "Interface/MultiActivateMenu.swf") == 0) {
		RE::Scaleform::GFx::Value codeObj;
		movie->GetVariable(&codeObj, "root.Menu_mc.BGSCodeObj");
		//RegisterFunction_MultiActivateMenu(&codeObj, movieRoot); - TODO
	}

	return true;
}

void GetDialogueGFxValue(RE::Scaleform::Ptr<RE::Scaleform::GFx::ASMovieRootBase>* movieRoot, RE::Scaleform::GFx::Value* outValue) {
	movieRoot->get()->CreateArray(outValue);

	if (auto playerDialogue = RE::GetCurrentPlayerDialogueAction()) {
		std::vector<RE::DialogueOption> options = RE::GetDialogueOptions();

		for (auto option : options) {
			RE::Scaleform::GFx::Value optionIDValue, promptValue, responseValue, enabledValue, saidValue, challengeLevelValue, challengeResultValue, linkedToSelfValue;

			optionIDValue = option.optionID;
			promptValue = option.prompText.c_str();
			responseValue = option.reseponseText.c_str();
			enabledValue = option.enabled;
			saidValue = option.said;
			challengeLevelValue = option.challengeLevel;
			challengeResultValue = option.challengeResult;
			linkedToSelfValue = option.linkedToSelf;

			RE::Scaleform::GFx::Value dialogueValue;
			movieRoot->get()->CreateObject(&dialogueValue);
			dialogueValue.SetMember("optionID", &optionIDValue);
			dialogueValue.SetMember("prompt", &promptValue);
			dialogueValue.SetMember("response", &responseValue);
			dialogueValue.SetMember("enabled", &enabledValue);
			dialogueValue.SetMember("said", &saidValue);
			dialogueValue.SetMember("challengeLevel", &challengeLevelValue);
			dialogueValue.SetMember("challengeResult", &challengeResultValue);
			dialogueValue.SetMember("linkedToSelf", &linkedToSelfValue);
			dialogueValue.SetMember("endsScene", RE::Scaleform::GFx::Value(option.endsScene));
			dialogueValue.SetMember("isBarterOption", RE::Scaleform::GFx::Value(option.isBarterOption));
			dialogueValue.SetMember("isInventoryOption", RE::Scaleform::GFx::Value(option.isInventoryOption));
			outValue->PushBack(&dialogueValue);
		}
	} else {
		// Player dialogue option not currently available.
		logger::warn("WARNING: Player dialogue not currently available. No dialogue will be retrieved.");
	}
}
