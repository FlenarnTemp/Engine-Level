#pragma once

#include "RE/Bethesda/BSTEvent.h"
#include "RE/Bethesda/Events.h"

namespace RE
{
	namespace Cascadia
	{
		class TESInitScriptEventWatcher : public BSTEventSink<TESInitScriptEvent>
		{
			virtual BSEventNotifyControl ProcessEvent(const TESInitScriptEvent& event, BSTEventSource<TESInitScriptEvent>*) override
			{
				if (TESObjectREFR* reference = event.hObjectInitialized) {
					switch (reference->formType.underlying())
					{
					case static_cast<std::uint32_t>(ENUM_FORM_ID::kCONT):
						logger::info("InitializeInventoryCondition");
						break;

					default:
						break;
					}
				}

				logger::info("TEST");

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
