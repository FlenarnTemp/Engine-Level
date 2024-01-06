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

		*path /= fmt::format(FMT_STRING("{:s}.log"sv), Version::PROJECT);
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
		auto lvl = *Settings::General::EnableDebugLogging
			? spdlog::level::trace
			: spdlog::level::info;

		log->set_level(lvl);
		log->flush_on(lvl);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("[%m/%d/%Y - %T] [%^%l%$] %v"s);

		logger::info(FMT_STRING("{:s} v{:s}"sv), Version::PROJECT, Version::NAME);
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
			if (DefineSkillsFormsFromGame()) {
				Skills::RegisterForSkillLink();
			}
			//logger::info(FMT_STRING("{:s} - kGameDataReady"), Version::PROJECT);
			break;

		case F4SE::MessagingInterface::kGameLoaded:
			//logger::info(FMT_STRING("{:s} - kGameLoaded"), Version::PROJECT);
			break;

		case F4SE::MessagingInterface::kPostLoad:
			//logger::info(FMT_STRING("{:s} - kPostLoad"), Version::PROJECT);
			break;

		case F4SE::MessagingInterface::kPostLoadGame:
			//logger::info(FMT_STRING("{:s} - kPostLoadGame"), Version::PROJECT);
			break;

		case F4SE::MessagingInterface::kPostPostLoad:
			//logger::info(FMT_STRING("{:s} - kPostPostLoad"), Version::PROJECT);
			break;

		case F4SE::MessagingInterface::kPostSaveGame:
			//logger::info(FMT_STRING("{:s} - kPostSaveGame"), Version::PROJECT);
			break;

		case F4SE::MessagingInterface::kDeleteGame:
			//logger::info(FMT_STRING("{:s} - kDeleteGame"), Version::PROJECT);
			break;

		case F4SE::MessagingInterface::kNewGame:
			//logger::info(FMT_STRING("{:s} - kNewGame"), Version::PROJECT);
			break;

		case F4SE::MessagingInterface::kInputLoaded:
			//logger::info(FMT_STRING("{:s} - kInputLoaded"), Version::PROJECT);
			break;

		case F4SE::MessagingInterface::kPreLoadGame:
			//logger::info(FMT_STRING("{:s} - kPreLoadGame"), Version::PROJECT);
			break;

		case F4SE::MessagingInterface::kPreSaveGame:
			//logger::info(FMT_STRING("{:s} - kPreSaveGame"), Version::PROJECT);
			break;

		default:
			break;
		}
	}
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface * a_F4SE, F4SE::PluginInfo * a_info)
{
	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	const auto rtv = a_F4SE->RuntimeVersion();
	if (rtv < F4SE::RUNTIME_LATEST)
	{
		stl::report_and_fail(
			fmt::format(
				FMT_STRING("{:s} does not support runtime v{:s}."sv),
				Version::PROJECT,
				rtv.string()));
	}

	return true;
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface * a_F4SE)
{
	InitializeLog();

	logger::info(FMT_STRING("{:s} started loading."), Version::PROJECT);
	logger::debug("Debug logging enabled.");

	F4SE::Init(a_F4SE);
	F4SE::AllocTrampoline(1u << 10);

	const auto messaging = F4SE::GetMessagingInterface();
	if (!messaging || !messaging->RegisterListener(MessageHandler))
	{
		logger::critical("Failed to register messaging handler, marking as incompatible."sv);
		return false;
	}

	const auto scaleform = F4SE::GetScaleformInterface();
	if (!scaleform)
	{
		logger::critical("Failed to register scaleform interface, marking as incompatible."sv);
		return false;
	}

	if (scaleform->Register("CAS_Dialogue", RE::RegisterScaleform));
	{
		logger::info("Registered Scaleform.");
	}


	Patches::Install();
	ObScript::Install();

	logger::info(FMT_STRING("{:s} finished loading."), Version::PROJECT);

	return true;
}
