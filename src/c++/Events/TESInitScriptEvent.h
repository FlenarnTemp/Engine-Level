#pragma once

#include "RE/Bethesda/BSTEvent.h"
#include "RE/Bethesda/Events.h"

#include "Systems/ItemDegradation.h"

namespace RE
{
	namespace Cascadia
	{
		class TESInitScriptEventWatcher : public BSTEventSink<TESInitScriptEvent>
		{
			virtual BSEventNotifyControl ProcessEvent(const TESInitScriptEvent& event, BSTEventSource<TESInitScriptEvent>*) override
			{
				if (event.hObjectInitialized) {

					switch (event.hObjectInitialized->data.objectReference->formType.underlying())
					{
					case static_cast<std::uint32_t>(ENUM_FORM_ID::kCONT):
						logger::info("InitializeInventoryCondition");
						break;

					case static_cast<std::uint32_t>(ENUM_FORM_ID::kARMO):
						logger::info("InitializeArmorCondition");
						break;

					case static_cast<std::uint32_t>(ENUM_FORM_ID::kWEAP):
						InitializeWeaponCondition(event.hObjectInitialized);
						logger::info("InitializeWeaponCondition");
						break;

					default:
						logger::debug(FMT_STRING("Form type: {:s}"), std::to_string(event.hObjectInitialized->data.objectReference->formType.underlying()));
						break;
					}
				}
				return BSEventNotifyControl::kContinue;
			}
		};

		void RegisterTESInitScriptEventSink()
		{
			TESInitScriptEventWatcher* tesInitScriptEvent = new TESInitScriptEventWatcher();
			TESInitScriptEventSource::GetSingleton()->RegisterSink(tesInitScriptEvent);
		}
	}
}
