#pragma once

#include "Menus/DialogueMenu.h"

namespace RE
{
	namespace Cascadia
	{
		class MenuOpenCloseEventWatcher :
			public BSTEventSink<MenuOpenCloseEvent>
		{
			virtual BSEventNotifyControl ProcessEvent(const MenuOpenCloseEvent& a_event, BSTEventSource<MenuOpenCloseEvent>*) override
			{
				// Dialogue Menu - XDI.
				if (a_event.menuName == BSFixedString("DialogueMenu"))
				{
					if (a_event.opening)
					{
						DialogueMenu::savedSubtitlePosition = DialogueMenu::GetSubtitlePosition();
					}
					else
					{
						DialogueMenu::SetSubtitlePosition(DialogueMenu::savedSubtitlePosition.first, DialogueMenu::savedSubtitlePosition.second);
					}
				}
				return BSEventNotifyControl::kContinue;
			}
		};

		void RegisterMenuOpenCloseEventSink()
		{
			MenuOpenCloseEventWatcher* menuOpenCloseEvent = new MenuOpenCloseEventWatcher();
			RE::UI::GetSingleton()->GetEventSource<RE::MenuOpenCloseEvent>()->RegisterSink(menuOpenCloseEvent);
		}
	}
}
