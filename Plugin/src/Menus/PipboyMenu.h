#pragma once

#include "Shared/SharedFunctions.h"
#include "Systems/Skills.h"

namespace RE
{
	namespace Cascadia
	{
		Scaleform::GFx::Value loader;

		namespace PipboyMenu
		{
			class PipboyReady : public Scaleform::GFx::FunctionHandler
			{
			public:
				virtual void Call(const Params& a_params)
				{
					DEBUG("PipboyMenu::PipboyReady function called.");
					//Cascadia::Skills::PopulateSkillEntries(a_params.movie->asMovieRoot, &loader);
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
				Scaleform::GFx::ASMovieRootBase* movieRoot = a_view->asMovieRoot.get();

				// Register native code handlers.
				if (a_view->asMovieRoot->GetVariable(&currentSWFPath, "root.loaderInfo.url"))
				{
					if (_stricmp(currentSWFPath.GetString(), "Interface/PipboyMenu.swf") == 0)
					{
						Scaleform::GFx::Value urlRequest, root;
						movieRoot->GetVariable(&root, "root");
						movieRoot->CreateObject(&loader, "flash.display.Loader");
						Scaleform::GFx::Value pipboy = "CASPipboy.swf";
						movieRoot->CreateObject(&urlRequest, "flash.net.URLRequest", &pipboy, 1);
						SetMemberAS3(root, "casPipboy_loader", &loader);
						
						Scaleform::GFx::Value casPipboy;
						movieRoot->CreateObject(&casPipboy);
						
						SetMemberAS3(root, "casPipboy", casPipboy);

						
						RegisterFunction<PipboyReady>(&casPipboy, a_view->asMovieRoot, "ready");
						RegisterFunction<PipboyHideWorkshopTab>(&casPipboy, a_view->asMovieRoot, "IsWorkshopTabHidden");
						InvokeAS3(loader, "load", urlRequest);
						InvokeAS3(a_view->asMovieRoot, "root.Menu_mc.addChild", loader);
					}
					return true;
				}
				return false;
			}
		}
	}
}