#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "F4SE/F4SE.h"
#include "RE/Fallout.h"

#include <AutoTOML.hpp>
#include <fmt/chrono.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <xbyak/xbyak.h>

#undef GetComptuerName

#define DLLEXPORT extern "C" [[maybe_unused]] __declspec(dllexport)

inline constexpr auto Version = 0 * 1000 + 1 * 100 + 0;

DLLEXPORT constinit auto F4SEPlugin_Version = []() noexcept
{
	F4SE::PluginVersionData data{};

	data.PluginVersion(Version);
	data.PluginName("Cascadia Gameplay Systems");
	data.AuthorName("Fallout Cascadia Development Team");
	data.UsesAddressLibrary(true);
	data.UsesSigScanning(false);
	data.IsLayoutDependent(true);
	data.HasNoStructUse(false);
	data.CompatibleVersions({ F4SE::RUNTIME_LATEST });

	return data;
}();

using namespace std::literals;

namespace logger = F4SE::log;

namespace stl
{
	using namespace F4SE::stl;

	void asm_replace(std::uintptr_t a_from, std::size_t a_size, std::uintptr_t a_to);
}

namespace F4SE
{
	namespace WinAPI
	{
		[[nodiscard]] bool(GetComputerName)(
			char* a_computerName,
			std::uint32_t* a_size) noexcept;
	}
}

// clang-format off
#include "Version.h"
#include "Settings.h"
// clang-format on
