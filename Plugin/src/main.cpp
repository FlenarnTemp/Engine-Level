#include "Events/BSAnimationGraphEvent.h"
#include "Events/InputEvent.h"
#include "Events/MenuOpenCloseEvent.h"
#include "Events/TESInitScriptEvent.h"
#include "Events/TESHarvestEvent.h"
#include "Events/TESHitEvent.h"
#include "Menus/ExamineConfirmMenu.h"
#include "Menus/PipboyMenu.h"
#include "Patches/Patches.h"
#include "Scripts/ObScript.h"
#include "Shared/Hooks.h"
#include "Systems/Skills.h"

namespace RE
{
	namespace Cascadia
	{
		extern void DefineItemDegradationFormsFromGame();
	}
}

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
				FATAL("Failed to define skills.");
			}

			RE::Cascadia::DefineItemDegradationFormsFromGame();
			RE::Cascadia::InputEventReceiverOverride::Install();
			RE::Cascadia::BSAnimationGraphEventWatcher::Install();
			RE::Cascadia::RegisterTESHitEventSink();
			RE::Cascadia::RegisterTESInitScriptEventSink();
			//RE::Cascadia::RegisterTESHarvestEventEventSink();
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
	trampoline.create(1024 * 32);

	RE::Cascadia::Hooks::Install(trampoline);

	const F4SE::SerializationInterface* serialization = F4SE::GetSerializationInterface();
	if (!serialization)
	{
		FATAL("Failed to register serialization interface, marking as incompatible."sv);
		return false;
	}
	else
	{
		serialization->SetUniqueID('CAS');
		//serialization->SetRevertCallback(Cascadia::Serialization::RevertCallback());
	}

	const F4SE::MessagingInterface* messaging = F4SE::GetMessagingInterface();
	if (!messaging || !messaging->RegisterListener(MessageHandler))
	{
		FATAL("Failed to register messaging handler, marking as incompatible."sv);
		return false;
	}

	const F4SE::ScaleformInterface* scaleform = F4SE::GetScaleformInterface();
	if (!scaleform)
	{
		FATAL("Failed to register Scaleform callback, marking as incompatible."sv);
		return false;
	}
	else
	{
		if (!scaleform->Register("Cascadia-DialogueMenu", RE::Cascadia::DialogueMenu::RegisterScaleform))
		{
			FATAL("Failed to register 'DialogueMenu', marking as incompatible.");
			return false;
		}

		if (!scaleform->Register("Cascadia-ExamineMenu", RE::Cascadia::ExamineMenu::RegisterScaleform))
		{
			FATAL("Failed to register 'ExamineMenu', marking as incompatible.");
			return false;
		}

		if (!scaleform->Register("Cascadia-PipboyMenu", RE::Cascadia::PipboyMenu::RegisterScaleform))
		{
			FATAL("Failed to register 'PipboyMenu', marking as incompatible.");
			return false;
		}

		if (!scaleform->Register("Cascadia-ExamineConfirmMenu", RE::Cascadia::ExamineConfirmMenu::RegisterScaleform))
		{
			FATAL("Failed to register 'ExamineConfirmMenu', marking as incompatible.");
			return false;
		}
	}

	const F4SE::PapyrusInterface* papyrus = F4SE::GetPapyrusInterface();
	if (!papyrus)
	{
		FATAL("Failed to register Papyrus handler, marking as incompatible.");
		return false;
	}
	else
	{

	}

	RE::Cascadia::RegisterMenuOpenCloseEventSink();
	RE::Cascadia::ExamineMenu::hkOnButtonEvent::InstallHook();
	RE::Cascadia::Hooks::RegisterAddItemHook();
	RE::Cascadia::Hooks::RegisterSetHealthPercHook();
	RE::Cascadia::Hooks::RegisterGetInventoryValueHook();
	RE::Cascadia::Hooks::RegisterShowBuildFailureMessage();
	RE::Cascadia::Hooks::RegisterGetBuildConfirmQuestion();
	RE::Cascadia::Hooks::RegisterQCurrentModChoiceData();
	RE::Cascadia::Hooks::RegisterExamineMenuBuildConfirmed();
	RE::Cascadia::Hooks::RegisterTESObjectWEAPFire();
	RE::Cascadia::Hooks::RegisterCombatFormulasCalcWeaponDamage();
	//RE::Cascadia::Hooks::RegisterGetEquippedArmorDamageResistance();
	RE::Cascadia::Patches::Install();
	ObScript::Install();

	INFO(("{:s} finished loading."), "Cascadia Gameplay Systems");

	return true;
}
