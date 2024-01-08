#pragma once

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

	void GetDialogueGFxValue(Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase>* movieRoot, Scaleform::GFx::Value* outValue)
	{
		movieRoot->get()->CreateArray(outValue);

		if (BGSSceneActionPlayerDialogue* playerDialogue = GetCurrentPlayerDialogueAction())
		{
			std::vector<DialogueOption> options = GetDialogueOptions();

			for (DialogueOption option : options)
			{
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

	bool RegisterScaleform(Scaleform::GFx::Movie* a_view, Scaleform::GFx::Value* a_value)
	{
		Scaleform::GFx::Value currentSWFPath;
		if (a_view->asMovieRoot->GetVariable(&currentSWFPath, "root.loaderInfo.url"))
		{
			if (_stricmp(currentSWFPath.GetString(), "Interface/DialogueMenu.swf") == 0)
			{
				a_view->asMovieRoot->Invoke("root.XDI_Init", nullptr, nullptr, 0);
			}
		}
		return true;
	}

	class DialogueMenuEx
	{
	public:
		static bool Install()
		{
			static REL::Relocation<std::uintptr_t> target{ VTABLE::DialogueMenu[0] };
			_DialogueMenu_Call = target.write_vfunc(0x01, reinterpret_cast<std::uintptr_t>(DialogueMenu__Call));
			_DialogueMenu__MapCodeObjectFunctions = target.write_vfunc(0x02, reinterpret_cast<std::uintptr_t>(DialogueMenu__MapCodeObjectFunctions));
			return true;
		}

	private:
		static void DialogueMenu__Call(DialogueMenu* a_this, const Scaleform::GFx::FunctionHandler::Params& a_params)
		{
			logger::info("DialogueMenu__Call");
			switch (reinterpret_cast<std::uint64_t>(a_params.userData))
			{

			case 3:
			{
				logger::info("DialogueMenu - IsFrameworkActive");

				if (a_params.retVal)
				{
					// We "cheat" here, we always return true, since we're a TC, our theoretical impact
					// on the retail game does not matter.
					*a_params.retVal = true;
				}
				break;
			}

			case 4:
				logger::info("DialogueMenu - GetTargetName");
				if (a_params.retVal)
				{
					// TODO
					/*const char* result = "";
					if (TESObjectREFR* target = GetCurrentPlayerDialogueTarget())
					{
						result = target->GetDisplayFullName();
					}*/

					*a_params.retVal = "result";
				}
				break;

			case 5:
				logger::info("DialogueMenu - GetTargetType");
				if (a_params.retVal)
				{
					std::uint8_t result = 0;
					if (TESObjectREFR* target = GetCurrentPlayerDialogueTarget())
					{
						result = target->formType.underlying();
					}

					*a_params.retVal = result;
				}
				break;

			case 6:
				logger::info("DialogueMenu - GetDialogueOptions");
				if (a_params.retVal)
				{
					Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> movieRoot = a_this->uiMovie->asMovieRoot;

					movieRoot->CreateArray(a_params.retVal);

					if (BGSSceneActionPlayerDialogue* playerDialogue = GetCurrentPlayerDialogueAction())
					{
						std::vector<DialogueOption> options = GetDialogueOptions();

						for (DialogueOption option : options)
						{
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
							movieRoot->CreateObject(&dialogueValue);
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
							a_params.retVal->PushBack(&dialogueValue);
						}
					}
					else
					{
						logger::warn("Player dialogue not currently available. No dialogue will be retrieved.");
					}
				}

			case 7:
				logger::info("DialogueMenu - SetDialogueOptions");
				if (a_params.argCount < 1) return;

				if (a_params.args[0].GetType() != Scaleform::GFx::Value::ValueType::kInt) return;

				std::uint32_t selectedOption = a_params.args[0].GetInt();

				*a_params.retVal = SelectDialogueOption(selectedOption);
				break;

			default:
				return _DialogueMenu_Call(a_this, a_params);
			}
		}

		static void DialogueMenu__MapCodeObjectFunctions(DialogueMenu* a_this)
		{
			logger::info("DialogueMenu__MapCodeObjectFunctions");

			a_this->MapCodeMethodToASFunction("registerObjects", 0);
			a_this->MapCodeMethodToASFunction("onButtonRelease", 1);
			a_this->MapCodeMethodToASFunction("OnSpeechChallengeAnimComplete", 2);

			// new
			a_this->MapCodeMethodToASFunction("IsFrameworkActive", 3);
			a_this->MapCodeMethodToASFunction("GetTargetName", 4);
			a_this->MapCodeMethodToASFunction("GetTargetType", 5);
			a_this->MapCodeMethodToASFunction("GetDialogueOptions", 6);
			a_this->MapCodeMethodToASFunction("SelectDialogueOption", 7);
		}

		inline static REL::Relocation<decltype(DialogueMenu__MapCodeObjectFunctions)> _DialogueMenu__MapCodeObjectFunctions;
		inline static REL::Relocation<decltype(DialogueMenu__Call)> _DialogueMenu_Call;
	};
}
