#pragma once

namespace RE
{
	namespace Cascadia
	{
		class CellFullyLoadedListener : 
			public BSTEventSink<TESCellFullyLoadedEvent>
		{
		public:
			virtual BSEventNotifyControl ProcessEvent(const TESCellFullyLoadedEvent& a_event, BSTEventSource<TESCellFullyLoadedEvent>*) override
			{
				if (a_event.cell)
				{
					a_event.cell->UpdateAllDecals();
				}

				return BSEventNotifyControl::kContinue;
			}
		};

		void RegisterForCellFullyLoaded() {
			CellFullyLoadedListener* fullyLoadedCellEvent = new CellFullyLoadedListener();
			TESCellFullyLoadedEvent::GetEventSource()->RegisterSink(fullyLoadedCellEvent);
			DEBUG("Registered 'TESCellFullLoadedEvent' sink.");
		}
	}
}