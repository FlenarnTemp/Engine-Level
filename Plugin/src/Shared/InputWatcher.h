#pragma once

using namespace F4SE;

using InputEvents = RE::InputEvent*;

class InputWatcher : public RE::BSTEventSink<InputEvents>
{
public:
	//virtual RE::BSEventNotifyControl(const InputEvents* events, RE::BSTEventSource<InputEvents>* dispatcher) override;
};
