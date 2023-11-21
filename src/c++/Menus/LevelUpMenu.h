#pragma once

namespace Menus
{
	class LevelUpMenu :
		public RE::GameMenuBase
	{
	public:

		static void Install()
		{
			hkRegisterMenu<740126, 0x43>::InstallC();
		}

		LevelUpMenu()
		{
			menuFlags.set(
				RE::UI_MENU_FLAGS::kPausesGame,
				RE::UI_MENU_FLAGS::kUsesCursor,
				RE::UI_MENU_FLAGS::kDisablePauseMenu,
				RE::UI_MENU_FLAGS::kUpdateUsesCursor,
				RE::UI_MENU_FLAGS::kCustomRendering,
				RE::UI_MENU_FLAGS::kUsesBlurredBackground,
				RE::UI_MENU_FLAGS::kUsesMovementToDirection);
			menuHUDMode = "SpecialMenu";
			depthPriority.set(RE::UI_DEPTH_PRIORITY::kTerminal);

			const auto ScaleformManager = RE::BSScaleformManager::GetSingleton();
			[[maybe_unused]]const auto LoadMovieSuccess =
				ScaleformManager->LoadMovieEx(*this, "Interface/CASLevelUpMenu.swf"sv, "root.Menu_mc");
			assert(LoadMovieSuccess);

			filterholder = RE::msvc::make_unique<RE::BSGFxShaderFXTarget>(*uiMovie, "root.Menu_mc");

			if (filterHolder)
			{
				filterHolder->CreateAndSetFiltersToHUD(RE::HUDColorTypes::kPlayerSetColor);
				shaderFXObjects.push_back(filterHolder.get());
			}

			Background_mc = RE::msvc::make_unique<RE::BSGFxShaderFXTarget>(*filterHolder, "Background_mc");
			if (Background_mc)
			{
				Background_mc->EnableshadedBackground(RE::HUDColorTypes::kMenuNoColorBackground);
				shaderFXObjects.push_back(Background_mc.get());
			}

			SetUpButtonBar(*filterHolder, "ButtonHintBar_mc", RE::HUDColorTypes::kPlayerSetColor);
		}

		~LevelUpMenu()
		{
			Background_mc.release();
		}

		virtual void Call(const Params& a_params) override
		{
			switch ((*((std::uint32_t*)&(a_params.userData))))
			{
				case 0:
					CloseMenu();
					break;

				case 1:
					if (a_params.argCount == 1 && a_params.args[0].IsString())
					{
						RE::UIUtils::PlayMenuSound(a_params.args[0].GetString());
					}
					break;

				case 2:
					NotifyLoaded();
					break;

				case 3:
					// TODO - InitPerkList
					break;

				case 4:
					// TODO - GetPerkCount
					break;

				case 5:
					// TODO - InitSkillList
					break;

				case 6:
					// TODO - GetSkillCount
					break;

				case 7:
					if (a_params.argCount == 1 && a_params.args[0].IsBoolean())
					{
						auto ControlMap = RE::ControlMap::GetSingleton();
						ControlMap->SetTextEntryMode(a_params.args[0].GetBoolean());
					}
					break;

				case 8:
					if (a_params.argCount == 1 && a_params.args[0].IsUInt())
					{
						// TODO - Select perk
					}
					break;

				default:
					break;

			}
		}

		virtual void MapCodeObjectFunctions() override
		{
			MapCodeMethodToASFunction("CloseMenu", 0);
			MapCodeMethodToASFunction("PlaySound", 1);
			MapCodeMethodToASFunction("NotifyLoaded", 2);
			MapCodeMethodToASFunction("InitPerkList", 3);
			MapCodeMethodToASFunction("GetPerkCount", 4);
			MapCodeMethodToASFunction("InitSkillList", 5);
			MapCodeMethodToASFunction("GetSkillCount", 6);
			// TODO - More of them
		}

	private:
		static RE::IMenu* Create(const RE::UIMessage&)
		{
			return new LevelUpMenu();
		}

		void CloseMenu()
		{
			if (auto UIMessageQueue = RE::UIMessageQueue::GetSingleton())
			{
				UIMessageQueue->AddMessage(
					"LevelUpMenu"sv,
					RE::UI_MESSAGE_TYPE::kHide);
			}
		}

		void NotifyLoaded()
		{
			LevelUpMenu::IsLoaded = true;
			menuObj.Invoke("RefreshDisplay");
		}

		void GetHeaderText()
		{
			if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
			{
				auto level = PlayerCharacter->GetLevel();
				// TODO - HeaderText

				IsNewLevel = false;
				RE::Scaleform::GFx::Value Header[1];
				Header[0] = HeaderText.c_str();
				menuObj.Invoke("SetHeader", nullptr, Header, 1);
			}
		}

		inline static std::string HeaderText;
		inline static bool IsNewLevel{ false };
		inline static bool IsLoaded{ false };

	protected:
		template<std::uint64_t ID, std::ptrdiff_t OFF>
		class hkRegisterMenu
		{
		public:
			static void InstallC()
			{
				static REL::Relocation<std::uintptr_t> target{ REL::ID(ID), OFF };
				auto& trampoline = F4SE::GetTrampoline();
				_RegisterMenu = trampoline.write_call<5>(target.address(), RegisterMenu);
			}

			static void InstallJ()
			{
				static REL::Relocation<std::uintptr_t> target{ REL::ID(ID), OFF };
				auto& trampoline = F4SE::GetTrampoline();
				_RegisterMenu = trampoline.write_branch<5>(target.address(), RegisterMenu);
			}

		private:
			using Create_t = RE::IMenu* (*)(const RE::UIMessage&);
			using Update_t = void (*)();

			static void RegisterMenu(
				RE::UI* a_this,
				const char* a_menu,
				Create_t a_create,
				Update_t a_update)
			{
				return _RegisterMenu(a_this, a_menu, LevelUpMenu::Create, nullptr);
			}

			inline static REL::Relocation<decltype(&RegisterMenu)> _RegisterMenu;
		};

		template<std::uint64_t ID, std::ptrdiff_t OFF>
		class hkSendUIBoolMessage
		{
		public:
			static void Install()
			{
				static REL::Relocation<std::uintptr_t> target{ REL::ID(ID), OFF };
				auto& trampoline = F4SE::GetTrampoline();
				trampoline.write_call<5>(target.address(), SendUIBoolMessage);
			}
		};

		private:
			static void SendUIBoolMessage(
				const RE::BSFixedString& a_menu,
				RE::UI_MESSAGE_TYPE a_type,
				bool a_fromPipboy)
			{
				if (auto UIMessageQueue = RE::UIMessageQueue::GetSingleton())
				{
					UIMessageQueue->AddMessage(
						"LevelUpMenu"sv,
						RE::UI_MESSAGE_TYPE::kShow);
				}
			}
	};
}
