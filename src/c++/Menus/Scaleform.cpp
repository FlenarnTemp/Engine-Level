#include "Menus/Scaleform.h"
#include "Systems/Dialogue.h"

namespace RE
{
	std::pair<double, double> GetSubtitlePosition()
	{
		std::pair<double, double> position;

		BSFixedString menuString("HUDMenu");
		if (UI::GetSingleton()->GetMenuOpen(menuString))
		{
			IMenu* menu = UI::GetSingleton()->GetMenu(menuString).get();
			Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> movieRoot = menu->uiMovie->asMovieRoot;

			Scaleform::GFx::Value subtitleX; movieRoot->GetVariable(&subtitleX, "root.BottomCenterGroup_mc.SubtitleText_mc.x");
			Scaleform::GFx::Value subtitleY; movieRoot->GetVariable(&subtitleY, "root.BottomCenterGroup_mc.SubtitleText_mc.y");

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
		BSFixedString menuString("HUDMenu");
		if (UI::GetSingleton()->GetMenuOpen(menuString))
		{
			IMenu* menu = UI::GetSingleton()->GetMenu(menuString).get();
			Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> movieRoot = menu->uiMovie->asMovieRoot;

			Scaleform::GFx::Value subtitle; movieRoot->GetVariable(&subtitle, "root.BottomCenterGroup_mc,SubtitleText_mc");
			subtitle.SetMember("x", Scaleform::GFx::Value(x));
			subtitle.SetMember("y", Scaleform::GFx::Value(y));
			return true;
		}

		return false;
	}

	void RegisterFunctions_DialogueMenu(Scaleform::GFx::Value codeObj, Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> movieRoot) {
		
		//RE::Scaleform::GFx::ASMovieRootBase::CreateFunction();
	}

	bool RegisterScaleform(Scaleform::GFx::Movie* movie, Scaleform::GFx::Value* f4se_root)
	{
		Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> movieRoot = movie->asMovieRoot;

		Scaleform::GFx::Value currentSWFPath;
		const char* currentSWFPathString = nullptr;

		if (movieRoot->GetVariable(&currentSWFPath, "root.loaderInfo.url")) {
			currentSWFPathString = currentSWFPath.GetString();
		}
		else {
			logger::warn("WARNING: Scaleform registration failed.");
		}

		// Register native code handlers.
		if (strcmp(currentSWFPathString, "Interface/DialogueMenu.swf") == 0) {
			Scaleform::GFx::Value codeObj;
			movieRoot->GetVariable(&codeObj, "root.Menu_mc.BGSCodeObj");
			RegisterFunctions_DialogueMenu(&codeObj, movieRoot);

			movieRoot->Invoke("root.XDI_Init", nullptr, nullptr, 0);

		}
		else if (strcmp(currentSWFPathString, "Interface/MultiActivateMenu.swf") == 0) {
			Scaleform::GFx::Value codeObj;
			movie->GetVariable(&codeObj, "root.Menu_mc.BGSCodeObj");
			//RegisterFunction_MultiActivateMenu(&codeObj, movieRoot); - TODO
		}

		return true;
	}

	void GetDialogueGFxValue(Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase>* movieRoot, Scaleform::GFx::Value* outValue) {
		movieRoot->get()->CreateArray(outValue);

		if (BGSSceneActionPlayerDialogue* playerDialogue = GetCurrentPlayerDialogueAction()) {
			std::vector<DialogueOption> options = GetDialogueOptions();

			for (DialogueOption option : options) {
				Scaleform::GFx::Value optionIDValue, promptValue, responseValue, enabledValue, saidValue, challengeLevelValue, challengeResultValue, linkedToSelfValue;

				optionIDValue = option.optionID;
				promptValue = option.prompText.c_str();
				responseValue = option.reseponseText.c_str();
				enabledValue = option.enabled;
				saidValue = option.said;
				challengeLevelValue = option.challengeLevel;
				challengeResultValue = option.challengeResult;
				linkedToSelfValue = option.linkedToSelf;

				Scaleform::GFx::Value dialogueValue;
				movieRoot->get()->CreateObject(&dialogueValue);
				dialogueValue.SetMember("optionID", &optionIDValue);
				dialogueValue.SetMember("prompt", &promptValue);
				dialogueValue.SetMember("response", &responseValue);
				dialogueValue.SetMember("enabled", &enabledValue);
				dialogueValue.SetMember("said", &saidValue);
				dialogueValue.SetMember("challengeLevel", &challengeLevelValue);
				dialogueValue.SetMember("challengeResult", &challengeResultValue);
				dialogueValue.SetMember("linkedToSelf", &linkedToSelfValue);
				dialogueValue.SetMember("endsScene", Scaleform::GFx::Value(option.endsScene));
				dialogueValue.SetMember("isBarterOption", Scaleform::GFx::Value(option.isBarterOption));
				dialogueValue.SetMember("isInventoryOption", Scaleform::GFx::Value(option.isInventoryOption));
				outValue->PushBack(&dialogueValue);
			}
		}
		else 
		{
			// Player dialogue option not currently available.
			logger::warn("WARNING: Player dialogue not currently available. No dialogue will be retrieved.");
		}
	}
}


