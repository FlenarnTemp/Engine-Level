#pragma once

#include "Shared/SharedFunctions.h"

// Thanks to 'Jajari' and 'Pinedog' for assistance with this code.

namespace RE
{
	namespace Cascadia
	{
		using std::unordered_map;
		class BSAnimationGraphEventWatcher
		{
		public:
			typedef BSEventNotifyControl(BSAnimationGraphEventWatcher::* FnProcessEvent)(BSAnimationGraphEvent& a_event, BSTEventSource<BSAnimationGraphEvent>* a_source);

			BSEventNotifyControl ProcessEvent(BSAnimationGraphEvent& a_event, BSTEventSource<BSAnimationGraphEvent>* a_source)
			{
				if (a_event.tag == "weaponFire" || a_event.tag == "WeaponFire")
				{
					DEBUG("weaponFire/WeaponFire event.");
					if (a_event.payload == "")
					{
						
					}
					else
					{
						DEBUG("Player used grenade/mine.");
					}
				}
				else if (a_event.tag == "HitFrame")
				{
					DEBUG("Player used Melee/Unarmed weapon");
				}

				FnProcessEvent fn = fnHash.at(*(uint64_t*)this);
				return fn ? (this->*fn)(a_event, a_source) : BSEventNotifyControl::kContinue;
			}

			void RegisterBSAnimationGraphEventSink()
			{
				std::uint64_t vtable = *(std::uint64_t*)this;
				auto it = fnHash.find(vtable);
				if (it == fnHash.end())
				{
					FnProcessEvent fn = SafeWrite64Function(vtable + 0x8, &BSAnimationGraphEventWatcher::ProcessEvent);
					fnHash.insert(std::pair<std::uint64_t, FnProcessEvent>(vtable, fn));
				}
			}

			static void Install()
			{
				((BSAnimationGraphEventWatcher*)((uint64_t)PlayerCharacter::GetSingleton() + 0x38))->RegisterBSAnimationGraphEventSink();
			}


		protected:
			static std::unordered_map<std::uint64_t, FnProcessEvent> fnHash;
		};
		std::unordered_map<std::uint64_t, BSAnimationGraphEventWatcher::FnProcessEvent> BSAnimationGraphEventWatcher::fnHash;
	}
}