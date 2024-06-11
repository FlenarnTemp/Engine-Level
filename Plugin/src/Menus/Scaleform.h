#pragma once

#include "Systems/Dialogue.h"

namespace RE
{
	namespace Cascadia
	{
		std::pair<double, double> GetSubtitlePosition()
		{
			std::pair<double, double> position;

			BSFixedString menuString("HUDMenu");
			if (UI::GetSingleton()->GetMenuOpen(menuString))
			{
				IMenu* menu = UI::GetSingleton()->GetMenu(menuString).get();
				Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> movieRoot = menu->uiMovie->asMovieRoot;

				Scaleform::GFx::Value subtitleX;
				movieRoot->GetVariable(&subtitleX, "root.BottomCenterGroup_mc.SubtitleText_mc.x");
				Scaleform::GFx::Value subtitleY;
				movieRoot->GetVariable(&subtitleY, "root.BottomCenterGroup_mc.SubtitleText_mc.y");

				position.first = subtitleX.GetNumber();
				position.second = subtitleY.GetNumber();
			}
			else
			{
				WARN("WARNING: Unable to retrieve the subtitle position because 'HUDMenu' is not open.");
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

				Scaleform::GFx::Value subtitle;
				movieRoot->GetVariable(&subtitle, "root.BottomCenterGroup_mc.SubtitleText_mc");
				subtitle.SetMember("x", Scaleform::GFx::Value(x));
				subtitle.SetMember("y", Scaleform::GFx::Value(y));
				return true;
			}

			return false;
		}

		class CAS_DialogueMenu
		{
		public:
			static bool Install()
			{
				static REL::Relocation<std::uintptr_t> target{ VTABLE::DialogueMenu[0] };
				_DialogueMenu_Call = target.write_vfunc(0x01, reinterpret_cast<std::uintptr_t>(DialogueMenu__Call));
				_DialogueMenu__MapCodeObjectFunctions = target.write_vfunc(0x02, reinterpret_cast<std::uintptr_t>(DialogueMenu__MapCodeObjectFunctions));
				INFO("DialogueMenu hooks installed.");
				return true;
			}

		private:
			static void DialogueMenu__Call(DialogueMenu* a_this, const Scaleform::GFx::FunctionHandler::Params& a_params)
			{
				INFO("DialogueMenu__Call, code: {:s}", std::to_string(reinterpret_cast<std::uint64_t>(a_params.userData)));
				switch (reinterpret_cast<std::uint64_t>(a_params.userData))
				{
				case 3:
					INFO("DialogueMenu - IsFrameworkActive");
					if (a_params.retVal)
					{
						// We "cheat" here, we always return true, since we're a TC, our theoretical impact on the retail game does not matter.
						*a_params.retVal = true;
					}
					break;

				case 4:
					INFO("DialogueMenu - GetTargetName");
					if (a_params.retVal)
					{
						const char* result = "";
						if (TESObjectREFR* target = GetCurrentPlayerDialogueTarget())
						{
							result = target->GetDisplayFullName();
							DEBUG(result);
						}

						*a_params.retVal = result;
					}
					break;

				case 5:
					INFO("DialogueMenu - GetTargetType");
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
					INFO("DialogueMenu - GetDialogueOptions");
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

								INFO("Prompt: {:s}, enabled: {:s}", option.prompText.c_str(), std::to_string(option.enabled));

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
								dialogueValue.SetMember("endsScene", Scaleform::GFx::Value(&option.endsScene));
								dialogueValue.SetMember("isBarterOption", Scaleform::GFx::Value(&option.isBarterOption));
								dialogueValue.SetMember("isInventoryOption", Scaleform::GFx::Value(&option.isInventoryOption));
								a_params.retVal->PushBack(&dialogueValue);
							}
						}
						else
						{
							WARN("GetDialogueOptions - Player dialogue not currently available. No dialogue will be retrieved.");
						}
					}
					break;

				case 7:
					INFO("DialogueMenu - SetDialogueOptions");
					if (a_params.retVal)
					{
						if (a_params.argCount < 1)
							return;

						if (a_params.args[0].GetType() != Scaleform::GFx::Value::ValueType::kInt)
							return;

						std::uint32_t selectedOption = a_params.args[0].GetInt();
						*a_params.retVal = SelectDialogueOption(selectedOption);
					}
					break;

				case 8:
					INFO("DialogueMenu - GetINISetting");
					if (a_params.retVal)
					{
						INISettingCollection* iniSettings = INISettingCollection::GetSingleton();

						if (iniSettings)
						{
							Setting* setting = iniSettings->GetSetting(a_params.args[0].GetString());
							if (setting)
							{
								*a_params.retVal = setting->GetInt();
							}
						}
					}
					break;

				case 9:
					INFO("DialogueMenu - GetModSetting");
					if (a_params.retVal)
					{
						INFO("Requested setting: {:s}", a_params.args[0].GetString());
					}
					break;

				case 10:
					INFO("DialogueMenu - GetSubtitlePosition");
					if (a_params.retVal)
					{
						*a_params.retVal = nullptr;

						BSFixedString menuString("HUDMenu");
						if (UI::GetSingleton()->GetMenuOpen(menuString))
						{
							IMenu* menu = UI::GetSingleton()->GetMenu(menuString).get();
							Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> movieRoot = menu->uiMovie->asMovieRoot;

							movieRoot->CreateArray(a_params.retVal);

							Scaleform::GFx::Value subtitleX;
							movieRoot->GetVariable(&subtitleX, "root.BottomCenterGroup_mc.SubtitleText_mc.x");
							Scaleform::GFx::Value subtitleY;
							movieRoot->GetVariable(&subtitleY, "root.BottomCenterGroup_mc.SubtitleText_mc.y");

							a_params.retVal->PushBack(subtitleX);
							a_params.retVal->PushBack(subtitleY);
						}
						else
						{
							DEBUG("Unable to retrieve the subtitle position because `HUDMenu` is not open.");
						}
					}
					break;

				case 11:
					INFO("DialogueMenu - SetSubtitlePosition");
					if (a_params.retVal)
					{
						*a_params.retVal = nullptr;

						BSFixedString menuString("HUDMenu");
						if (UI::GetSingleton()->GetMenuOpen(menuString))
						{
							IMenu* menu = UI::GetSingleton()->GetMenu(menuString).get();
							Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> movieRoot = menu->uiMovie->asMovieRoot;
							
							Scaleform::GFx::Value subtitle;
							movieRoot->GetVariable(&subtitle, "root.BottomCenterGroup_mc.SubtitleText_mc");
							
							// TODO - set member variables of `subtitle`.
						}
					}

					break;

				case 12:
					INFO("DialogueMenu - SetXDIResult");
					break;

				case 13:
					INFO("DialogueMenu - SetWheelZoomEnabled");
					break;

				case 14:
					INFO("DialogueMenu - SetFavoritesEnabled");
					break;

				case 15:
					INFO("DialogueMenu - SetMovementEnabled");
					break;

				case 16:
					INFO("DialogueMenu - SetPlayerControls");
					break;

				default:
					return _DialogueMenu_Call(a_this, a_params);
				}
			}

			static void DialogueMenu__MapCodeObjectFunctions(DialogueMenu* a_this)
			{
				INFO("DialogueMenu__MapCodeObjectFunctions");

				// Retail
				a_this->MapCodeMethodToASFunction("registerObjects", 0);
				a_this->MapCodeMethodToASFunction("onButtonRelease", 1);
				a_this->MapCodeMethodToASFunction("OnSpeechChallengeAnimComplete", 2);

				// New
				a_this->MapCodeMethodToASFunction("IsFrameworkActive", 3);
				a_this->MapCodeMethodToASFunction("GetTargetName", 4);
				a_this->MapCodeMethodToASFunction("GetTargetType", 5);
				a_this->MapCodeMethodToASFunction("GetDialogueOptions", 6);
				a_this->MapCodeMethodToASFunction("SelectDialogueOption", 7);
				a_this->MapCodeMethodToASFunction("GetINISetting", 8);
				a_this->MapCodeMethodToASFunction("GetModSetting", 9);  // TODO
				a_this->MapCodeMethodToASFunction("GetSubtitlePosition", 10);
				a_this->MapCodeMethodToASFunction("SetSubtitlePosition", 11);  // TODO

				// Misc
				a_this->MapCodeMethodToASFunction("SetXDIResult", 12);  // TODO

				// Input - TODO
				a_this->MapCodeMethodToASFunction("SetWheelZoomEnabled", 13);
				a_this->MapCodeMethodToASFunction("SetFavoritesEnabled", 14);
				a_this->MapCodeMethodToASFunction("SetMovementEnabled", 15);
				a_this->MapCodeMethodToASFunction("SetPlayerControls", 16);
			}

			inline static REL::Relocation<decltype(DialogueMenu__MapCodeObjectFunctions)> _DialogueMenu__MapCodeObjectFunctions;
			inline static REL::Relocation<decltype(DialogueMenu__Call)> _DialogueMenu_Call;
		};

		bool RegisterScaleform(Scaleform::GFx::Movie* a_view, Scaleform::GFx::Value* a_value)
		{
				Scaleform::GFx::Value currentSWFPath;
				if (a_view->asMovieRoot->GetVariable(&currentSWFPath, "root.loaderInfo.url"))
				{
					if (_stricmp(currentSWFPath.GetString(), "Interface/DialogueMenu.swf") == 0)
					{
						a_view->asMovieRoot->Invoke("root.XDI_Init", nullptr, nullptr, 0);
						INFO("root.XDI_Init");
					}
				}
			return true;
		}
	}
}
