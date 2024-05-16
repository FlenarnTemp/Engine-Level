#include "Events/TESInitScriptEvent.h"

#include "Menus/Scaleform.h"

#include "Patches/Patches.h"

#include "Scripts/ObScript.h"

#include "Systems/Skills.h"

namespace
{
	void InitializeLog()
	{
		auto path = logger::log_directory();
		if (!path)
		{
			stl::report_and_fail("Failed to find standard logging directory"sv);
		}

		*path /= fmt::format(FMT_STRING("{:s}.log"sv), "Cascadia");
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
		auto lvl = *Settings::General::EnableDebugLogging
		             ? spdlog::level::trace
		             : spdlog::level::info;

		log->set_level(lvl);
		log->flush_on(lvl);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("[%m/%d/%Y - %T] [%^%l%$] %v"s);

		logger::info(FMT_STRING("{:s} v{:s}"sv), "Cascadia Gameplay Systems", "0.1.0");
	}

	void MessageHandler(F4SE::MessagingInterface::Message* a_msg)
	{
		if (!a_msg)
		{
			return;
		}

		switch (a_msg->type)
		{
		case F4SE::MessagingInterface::kGameDataReady:
			logger::info(FMT_STRING("{:s} - kGameDataReady"), "Cascadia Gameplay Systems");
			if (RE::Cascadia::Skills::DefineSkillsFormsFromGame())
			{
				RE::Cascadia::Skills::RegisterForSkillLink();
			}
			else
			{
				logger::warn("Failed to define skills.");
			}

			RE::Cascadia::DefineItemDegradationFormsFromGame();

			RE::Cascadia::RegisterTESInitScriptEventSink();

			break;

		case F4SE::MessagingInterface::kGameLoaded:
			logger::info(FMT_STRING("{:s} - kGameLoaded"), "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kPostLoad:
			logger::info(FMT_STRING("{:s} - kPostLoad"), "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kPostLoadGame:
			logger::info(FMT_STRING("{:s} - kPostLoadGame"), "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kPostPostLoad:
			logger::info(FMT_STRING("{:s} - kPostPostLoad"), "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kPostSaveGame:
			logger::info(FMT_STRING("{:s} - kPostSaveGame"), "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kDeleteGame:
			logger::info(FMT_STRING("{:s} - kDeleteGame"), "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kNewGame:
			logger::info(FMT_STRING("{:s} - kNewGame"), "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kInputLoaded:
			logger::info(FMT_STRING("{:s} - kInputLoaded"), "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kPreLoadGame:
			logger::info(FMT_STRING("{:s} - kPreLoadGame"), "Cascadia Gameplay Systems");
			break;

		case F4SE::MessagingInterface::kPreSaveGame:
			logger::info(FMT_STRING("{:s} - kPreSaveGame"), "Cascadia Gameplay Systems");
			break;

		default:
			break;
		}
	}
}

DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_F4SE)
{
#ifndef NDEBUG
	MessageBoxA(NULL, "Loaded. You can now attach the debugger or continue execution.", "Cascadia Gameplay Systems", NULL);
#endif

	InitializeLog();

	logger::info(FMT_STRING("{:s} started loading."), "Cascadia Gameplay Systems");
	logger::debug("Debug logging enabled.");

	F4SE::Init(a_F4SE);
	F4SE::AllocTrampoline(1u << 10);

	const auto messaging = F4SE::GetMessagingInterface();
	if (!messaging || !messaging->RegisterListener(MessageHandler))
	{
		logger::critical("Failed to register messaging handler, marking as incompatible."sv);
		return false;
	}

	const F4SE::ScaleformInterface* scaleform = F4SE::GetScaleformInterface();

	/*if (!RE::Cascadia::CAS_DialogueMenu::Install())
	{
		logger::warn("DialogueMenu - failed to install hooks.");
	}*/

	if (!scaleform->Register("Cascadia", RE::Cascadia::RegisterScaleform))
	{
		logger::critical("Failed to register scaleform callback, marking as incompatible."sv);
		return false;
	}
	else
	{
		logger::info("Registered scaleform callback.");
	}

	RE::Cascadia::Patches::Install();
	ObScript::Install();

	logger::info(("{:s} finished loading."), "Cascadia Gameplay Systems");

	return true;
}
