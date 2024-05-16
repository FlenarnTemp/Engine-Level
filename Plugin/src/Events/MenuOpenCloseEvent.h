#pragma once

namespace RE
{
	namespace Cascadia
	{
		class MenuOpenCloseEventWatcher : public BSTEventSink<MenuOpenCloseEvent>
		{
			virtual BSEventNotifyControl ProcessEvent(MenuOpenCloseEvent& event, BSTEventSource<MenuOpenCloseEvent>*) override
			{

			}
		};
	}
}
