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
					switch (a_event.hObjectInitialized->data.objectReference->formType.underlying())
					{
					case static_cast<std::uint32_t>(ENUM_FORM_ID::kCONT):
						//InitializeContainerCondition(event.hObjectInitialized);
						//INFO("InitializeInventoryCondition");
						break;

					case static_cast<std::uint32_t>(ENUM_FORM_ID::kARMO):
						InitializeArmorCondition(a_event.hObjectInitialized);
						//DEBUG("InitializeArmorCondition");
						break;

					case static_cast<std::uint32_t>(ENUM_FORM_ID::kWEAP):
						InitializeWeaponCondition(a_event.hObjectInitialized);
						//DEBUG("InitializeWeaponCondition");
						break;

					default:
						//DEBUG("Form type: {:s}", std::to_string(a_event.hObjectInitialized->data.objectReference->formType.underlying()));
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
