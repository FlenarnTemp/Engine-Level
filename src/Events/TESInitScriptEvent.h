#pragma once

#include "RE/Bethesda/BSTEvent.h"
#include "RE/Bethesda/Events.h"


namespace RE
{
	namespace Cascadia
	{
		class TESInitScriptEventWatcher :
			public BSTEventSink<TESInitScriptEvent>
		{
			virtual BSEventNotifyControl ProcessEvent(const TESInitScriptEvent& a_event, BSTEventSource<TESInitScriptEvent>*) override
			{
				return BSEventNotifyControl::kContinue;
			}
		};

		void RegisterTESInitScriptEventSink()
		{
			TESInitScriptEventWatcher* tesInitScriptEvent = new TESInitScriptEventWatcher();
			TESInitScriptEventSource::GetSingleton()->RegisterSink(tesInitScriptEvent);
			DEBUG("Registered 'TESInitScriptEvent' sink.");
		}
	}
}
