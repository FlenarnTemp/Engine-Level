#include "Events/TESInitScriptEvent.h"
#include "Events/MenuOpenCloseEvent.h"
#include "Patches/Patches.h"
#include "Scripts/ObScript.h"
#include "Shared/Hooks.h"

#include "Systems/Skills.h"

namespace
{
	void MessageHandler(F4SE::MessagingInterface::Message* a_msg)
	{
		if (!a_msg)
		{
			return;
		}

		switch (a_msg->type)
		{
		case F4SE::MessagingInterface::kGameDataReady:
			INFO("{:s} - kGameDataReady", "Cascadia Gameplay Systems");
			if (RE::Cascadia::Skills::DefineSkillsFormsFromGame())
			{
				RE::Cascadia::Skills::RegisterForSkillLink();
			}
			else
			{
				WARN("Failed to define skills.");
			}

			RE::Cascadia::DefineItemDegradationFormsFromGame();

			RE::Cascadia::RegisterTESInitScriptEventSink();

			break;

		case F4SE::MessagingInterface::kGameLoaded:
			INFO("{:s} - kGameLoaded", "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kPostLoad:
			INFO("{:s} - kPostLoad", "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kPostLoadGame:
			INFO("{:s} - kPostLoadGame", "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kPostPostLoad:
			INFO("{:s} - kPostPostLoad", "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kPostSaveGame:
			INFO("{:s} - kPostSaveGame", "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kDeleteGame:
			INFO("{:s} - kDeleteGame", "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kNewGame:
			INFO("{:s} - kNewGame", "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kInputLoaded:
			INFO("{:s} - kInputLoaded", "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kPreLoadGame:
			INFO("{:s} - kPreLoadGame", "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kPreSaveGame:
			INFO("{:s} - kPreSaveGame", "Cascadia Gameplay Systems");
			break;

		default:
			break;
		}
	}
}

DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_F4SE)
{
#ifndef NDEBUG
	MessageBoxA(NULL, "Loaded. You can now attach the debugger or continue execution.", Plugin::NAME.data(), NULL);
#endif
	F4SE::Init(a_F4SE);

	DKUtil::Logger::Init(Plugin::NAME, std::to_string(Plugin::Version));
	INFO("{} v{} loaded."sv, Plugin::NAME, Plugin::Version);

	auto& trampoline = F4SE::GetTrampoline();
	trampoline.create(1024 * 64);

	Cascadia::Hooks::Install(trampoline);

	const auto messaging = F4SE::GetMessagingInterface();
	if (!messaging || !messaging->RegisterListener(MessageHandler))
	{
		FATAL("Failed to register messaging handler, marking as incompatible."sv);
		return false;
	}

	const auto scaleform = F4SE::GetScaleformInterface();
	if (!scaleform || !scaleform->Register("Cascadia", RE::Cascadia::DialogueMenu::RegisterScaleform))
	{
		FATAL("Failed to register scaleform callback, marking as incompatible."sv);
		return false;
	}

	RE::Cascadia::RegisterMenuOpenCloseEventSink();

	RE::Cascadia::Patches::Install();
	ObScript::Install();

	INFO(("{:s} finished loading."), "Cascadia Gameplay Systems");

	return true;
}
