#include "AmmoSwitch.h"

namespace AmmoSwitch
{
	AmmoSwitchValues_Struct AmmoSwitchValues;

	// Input Handling

	class InputEventWatcher : public RE::BSTEventSink<RE::InputEvent>
	{
	public:
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::InputEvent& evn, RE::BSTEventSource<RE::InputEvent>*)
		{
			// code code code

			return RE::BSEventNotifyControl::kContinue;
		}
	};

}
