#pragma once

namespace RE
{
	namespace Cascadia
	{
		class TESHitEventWatcher :
			public BSTEventSink<TESHitEvent>
		{
			virtual BSEventNotifyControl ProcessEvent(const TESHitEvent& a_event, BSTEventSource<TESHitEvent>*) override
			{
				TESObjectREFR* target = a_event.target.get();

				// Return early on invalid events.
				// Note: Unsure about the logic RE; Power Armor here.
				if (!target || !a_event.usesHitData || target->extraList->GetByType(kPowerArmor) != nullptr)
				{
					return BSEventNotifyControl::kContinue;
				}

				Actor* actor = static_cast<Actor*>(target);

				// We're only interested in the player.
				if (target != PlayerCharacter::GetSingleton())
				{
					return BSEventNotifyControl::kContinue;
				}

				TESObjectREFR* sourceREFR = a_event.hitData.sourceRef.get().get();

				BGSBodyPartData* bodyPartData = actor->GetBodyPartData();

				// If sourceREFR is 'nullptr', most likely a melee attack.
				if (sourceREFR)
				{
					if (sourceREFR->IsExplosion())
					{
						DEBUG("Explosion!");
					}
					else
					{

					}
				}
				else
				{
					auto test = a_event.hitData.weapon.object->GetFormEditorID();
					DEBUG("WEAPON USED: {}", test);
				}

				return BSEventNotifyControl::kContinue;
			}
		};

		void RegisterTESHitEventSink()
		{
			TESHitEventWatcher* tesHitEvent = new TESHitEventWatcher();
			TESHitEvent::GetEventSource()->RegisterSink(tesHitEvent);
			INFO("Registed 'TESHitEvent' sink.");
		}
	}
}