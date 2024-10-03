#pragma once

namespace RE
{
	namespace Cascadia
	{
		class TESHarvestEventWatcher :
			public BSTEventSink<TESHarvestEvent::ItemHarvested>
		{
			virtual BSEventNotifyControl ProcessEvent(const TESHarvestEvent::ItemHarvested& a_event, BSTEventSource<TESHarvestEvent::ItemHarvested>*) override
			{
				if (a_event.harvestedBy == PlayerCharacter::GetSingleton())
				{ 
					DEBUG("Harvest event sent by player, harvested REFR: {}, harvest object: {}", a_event.referenceHarvested->GetFormEditorID(), a_event.itemHarvested->GetFormEditorID());	
					const char* itemHarvested = a_event.itemHarvested->GetFormEditorID();

					if (strcmp(itemHarvested, "CAS_LL_WoodPile_Random") == 0)
					{
						SendHUDMessage::ShowHUDMessage("Firewood Added.", nullptr, true, true);
					}
					else 
					{
						return BSEventNotifyControl::kContinue;
					}

				}

				return BSEventNotifyControl::kContinue;
			}
		};

		void RegisterTESHarvestEventSink()
		{
			TESHarvestEventWatcher* tesHarvestEvent = new TESHarvestEventWatcher();
			TESHarvestEvent::ItemHarvested::GetEventSource()->RegisterSink(tesHarvestEvent);
			INFO("Registered 'TESHarvestEvent::ItemHarvested' sink.");
		}
	}
}