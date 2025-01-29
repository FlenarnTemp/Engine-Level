#pragma once

namespace RE
{
	namespace Cascadia
	{
		class CellFullyLoadedListener : public BSTEventSink<TESCellFullyLoadedEvent>
		{
		public:
			static CellFullyLoadedListener* GetSingleton() 
			{
				static CellFullyLoadedListener instance;
				return &instance;
			}

			virtual BSEventNotifyControl ProcessEvent(const TESCellFullyLoadedEvent& a_event, BSTEventSource<TESCellFullyLoadedEvent>*) override
			{
				DEBUG("EVENT FIRED");

				if (a_event.cell)
				{
					a_event.cell->UpdateAllDecals();
				}

				return BSEventNotifyControl::kContinue;
			}
		};

		void RegisterForCellFullyLoaded(BSTEventSink<TESCellFullyLoadedEvent>* a_sink) {
			using func_t = decltype(&RegisterForCellFullyLoaded);
			REL::Relocation<func_t> func{ REL::ID(2201571) };
			return func(a_sink);
		}
	}
}