#pragma once

#include "Menus/ExamineMenu.h"
#include "Menus/DialogueMenu.h"
#include "Menus/LevelUpMenu.h"

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
						DialogueMenu::savedSubtitlePosition = DialogueMenu::GetSubtitlePosition();
					}
					else
					{
						DialogueMenu::SetSubtitlePosition(DialogueMenu::savedSubtitlePosition.first, DialogueMenu::savedSubtitlePosition.second);
					}
				}

				if (a_event.menuName == BSFixedString("CASLevelUpMenu"))
				{
					UI::GetSingleton()->RegisterMenu("CASLevelUpMenu", LevelUpMenu::CASLevelUpMenu::CreateCASLevelUpMenu);
				}

				return BSEventNotifyControl::kContinue;
			}
		};

		void RegisterMenuOpenCloseEventSink()
		{
			MenuOpenCloseEventWatcher* menuOpenCloseEvent = new MenuOpenCloseEventWatcher();
			UI::GetSingleton()->GetEventSource<MenuOpenCloseEvent>()->RegisterSink(menuOpenCloseEvent);
			DEBUG("Registered 'MenuOpenCloseEvent' sink.");
		}
	}
}
