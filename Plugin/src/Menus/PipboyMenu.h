#pragma once

#include "Shared/SharedFunctions.h"
#include "Systems/Skills.h"

namespace RE
{
	namespace Cascadia
	{
		namespace PipboyMenu
		{
			

			class PipboyReady : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					Cascadia::Skills::PopulateSkillEntries(a_params.movie->asMovieRoot.get());
				}
			};

			class PipboyHideWorkshopTab : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					if (a_params.retVal)
					{
						*a_params.retVal = true;
					}
				}
			};

			bool RegisterScaleform(Scaleform::GFx::Movie* a_view, Scaleform::GFx::Value* a_value)
			{
				Scaleform::GFx::Value currentSWFPath;
				// Register native code handlers.
				if (a_view->asMovieRoot->GetVariable(&currentSWFPath, "root.loaderInfo.url"))
				{
					if (_stricmp(currentSWFPath.GetString(), "Interface/PipboyMenu.swf") == 0)
					{
						Scaleform::GFx::Value loader;
						Scaleform::GFx::Value urlRequest;
						Scaleform::GFx::Value rootValue;
						a_view->asMovieRoot->GetVariable(&rootValue, "root");
						a_view->asMovieRoot->CreateObject(&loader, "flash.display.Loader");
						Scaleform::GFx::Value newSWFName = "CASPipboy.swf";
						a_view->asMovieRoot->CreateObject(&urlRequest, "flash.net.URLRequest", &newSWFName, 1);
						rootValue.SetMember("casPipboy_loader", &loader);
						Scaleform::GFx::Value casPipboy;
						a_view->asMovieRoot->CreateObject(&casPipboy);
						rootValue.SetMember("casPipboy", &casPipboy);
						RegisterFunction<PipboyReady>(&casPipboy, a_view->asMovieRoot, "ready");
						RegisterFunction<PipboyHideWorkshopTab>(&casPipboy, a_view->asMovieRoot, "IsWorkshopTabHidden");
						a_view->asMovieRoot->Invoke("root.casPipboy_loader.load", nullptr, &urlRequest, 1);
						a_view->asMovieRoot->Invoke("root.Menu_mc.addChild", nullptr, &loader, 1);
					}
					return true;
				}
				return false;
			}
		}
	}
}