#pragma once

#include "Shared/SharedFunctions.h"

namespace RE
{
	namespace Cascadia
	{
		class InputEventReceiverOverride : public BSInputEventReceiver
		{
		public:
			typedef void (InputEventReceiverOverride::* FnPerformInputProcessing)(InputEvent* a_queueHead);

			void ProcessButtonEvent(const ButtonEvent* event)
			{
				//DEBUG("BUTTON!");
			}

			void HookedPerformInputProcessing(InputEvent* a_queueHead)
			{
				InputEvent* event = a_queueHead;
				while (event)
				{
					if (event->eventType == INPUT_EVENT_TYPE::kButton)
					{
						ProcessButtonEvent((ButtonEvent*)event);
					}
					event = event->next;
				}
				FnPerformInputProcessing fn = fnHash.at(*(std::uint64_t*)this);
				if (fn)
				{
					(this->*fn)(a_queueHead);
				}
			}

			void HookSink()
			{
				uint64_t vtable = *(uint64_t*)this;
				auto it = fnHash.find(vtable);
				if (it == fnHash.end()) {
					FnPerformInputProcessing fn = SafeWrite64Function(vtable, &InputEventReceiverOverride::HookedPerformInputProcessing);
					fnHash.insert(std::pair<uint64_t, FnPerformInputProcessing>(vtable, fn));
				}
			}

			void UnhookSink()
			{
				uint64_t vtable = *(uint64_t*)this;
				auto it = fnHash.find(vtable);
				if (it == fnHash.end())
				{
					return;
				}
				SafeWrite64Function(vtable, it->second);
				fnHash.erase(it);
			}

			static void Install()
			{
				((InputEventReceiverOverride*)((uint64_t)PlayerCamera::GetSingleton() + 0x38))->HookSink();
			}

		protected:
			static std::unordered_map<uint64_t, FnPerformInputProcessing> fnHash;
		};
		std::unordered_map<uint64_t, InputEventReceiverOverride::FnPerformInputProcessing> InputEventReceiverOverride::fnHash;

	}
}