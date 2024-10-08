#pragma once

#include "Shared/SharedFunctions.h"

namespace RE
{
	namespace Cascadia
	{
		namespace ExamineConfirmMenu
		{
			// TODO - this doesn't quite work as it should.

			class OnCancelPress : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					Scaleform::Ptr<RE::ExamineMenu> examineMenu = UI::GetSingleton()->GetMenu<RE::ExamineMenu>();

					DEBUG("OnCancelPress");

					if (examineMenu->repairing)
					{
						examineMenu->repairing = false;
					}
				}
			};

			bool RegisterScaleform(Scaleform::GFx::Movie* a_view, Scaleform::GFx::Value* a_value)
			{
				Scaleform::GFx::Value currentSWFPath;
				// Register native code handlers.
				if (a_view->asMovieRoot->GetVariable(&currentSWFPath, "root.loaderInfo.url"))
				{
					if (_stricmp(currentSWFPath.GetString(), "Interface/ExamineConfirmMenu.swf") == 0)
					{
						Scaleform::GFx::Value bgsCodeObj;
						a_view->asMovieRoot->GetVariable(&bgsCodeObj, "root.Menu_mc.BGSCodeObj");

						RegisterFunction<OnCancelPress>(&bgsCodeObj, a_view->asMovieRoot, "onCancelPress");
					}
					return true;
				}
				return false;
			}
		}
	}
}