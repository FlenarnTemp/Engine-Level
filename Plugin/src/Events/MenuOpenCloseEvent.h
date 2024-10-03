#pragma once

#include "Menus/ExamineMenu.h"
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
				if (a_event.menuName == BSFixedString("DialogueMenu"))
				{
					if (a_event.opening)
					{
						DEBUG("Opening DialogueMenu");
						DialogueMenu::savedSubtitlePosition = DialogueMenu::GetSubtitlePosition();
					}
					else
					{
						DEBUG("Closing DialogueMenu.");
						DialogueMenu::SetSubtitlePosition(DialogueMenu::savedSubtitlePosition.first, DialogueMenu::savedSubtitlePosition.second);
					}
				}
				return BSEventNotifyControl::kContinue;
			}
		};

		void RegisterMenuOpenCloseEventSink()
		{
			MenuOpenCloseEventWatcher* menuOpenCloseEvent = new MenuOpenCloseEventWatcher();
			UI::GetSingleton()->GetEventSource<MenuOpenCloseEvent>()->RegisterSink(menuOpenCloseEvent);
			INFO("Registered 'MenuOpenCloseEvent' sink.");
		}
	}
}
