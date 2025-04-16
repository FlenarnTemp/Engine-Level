#pragma once

namespace RE
{
	namespace Cascadia
	{
		class LevelIncreaseEventWatcher :
			public BSTEventSink<LevelIncrease::Event>
		{
			virtual BSEventNotifyControl ProcessEvent(const LevelIncrease::Event& a_event, BSTEventSource<LevelIncrease::Event>*) override
			{
				DEBUG("Player leveled up.");

				return BSEventNotifyControl::kContinue;
			}
		};

		void RegisterLevelIncreaseEventSink()
		{
			LevelIncreaseEventWatcher* levelIncreaseEvent = new LevelIncreaseEventWatcher();
			LevelIncrease::Event::GetEventSource()->RegisterSink(levelIncreaseEvent);
			DEBUG("Registered 'LevelIncrease::Event' sink.");
		}
	}
}