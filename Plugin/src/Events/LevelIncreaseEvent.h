#pragma once

#include "Menus/LevelUpMenu.h"

namespace RE
{
	namespace Cascadia
	{
		class LevelIncreaseEventWatcher :
			public BSTEventSink<LevelIncrease::Event>
		{
			virtual BSEventNotifyControl ProcessEvent(const LevelIncrease::Event& a_event, BSTEventSource<LevelIncrease::Event>*) override
			{
				LevelUpMenu::HandleLevelUp();
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