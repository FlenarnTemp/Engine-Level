#pragma once

namespace RE
{
	namespace Cascadia
	{
		class MenuOpenCloseEventWatcher :
			public BSTEventSink<MenuOpenCloseEvent>
		{
			virtual BSEventNotifyControl ProcessEvent(const MenuOpenCloseEvent& a_event, BSTEventSource<MenuOpenCloseEvent>*) override
			{
			}
		};
	}
}
