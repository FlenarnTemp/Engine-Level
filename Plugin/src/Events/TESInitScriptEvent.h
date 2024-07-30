#pragma once

#include "RE/Bethesda/BSTEvent.h"
#include "RE/Bethesda/Events.h"

#include "Systems/ItemDegradation.h"

namespace RE
{
	namespace Cascadia
	{
		class TESInitScriptEventWatcher :
			public BSTEventSink<TESInitScriptEvent>
		{
			virtual BSEventNotifyControl ProcessEvent(const TESInitScriptEvent& a_event, BSTEventSource<TESInitScriptEvent>*) override
			{
				if (a_event.hObjectInitialized)
				{
					switch (a_event.hObjectInitialized->data.objectReference->GetFormType())
					{
						case ENUM_FORM_ID::kARMO:
						case ENUM_FORM_ID::kWEAP:
							//InitializeREFRCondition(a_event.hObjectInitialized); TODO - Remove entire logic circuit, mimic Fallout 76 initialization.
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
