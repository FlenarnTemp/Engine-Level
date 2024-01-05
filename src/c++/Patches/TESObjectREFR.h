#pragma once

namespace Patches
{
	class TESObjectREFR_ConsoleName
	{
	private:
		template<class Form>
		static void InstallHook()
		{
			REL::Relocation<std::uintptr_t> vtbl{ Form::VTABLE[0] };
			_GetFormEditorID = vtbl.write_vfunc(0x3A, GetFormEditorID);
		}

		static const char* GetFormEditorID(RE::TESObjectREFR* a_this)
		{
			if (a_this)
			{
				const char* fullName = a_this->data.objectReference->GetFormEditorID();
				return fullName ? fullName : "";
			}
			return "";
		}

		inline static REL::Relocation<decltype(&RE::TESObjectREFR::GetFormEditorID)> _GetFormEditorID;

	public:
		static void Install()
		{
			InstallHook<RE::TESObjectREFR>();
		}
	};
}
