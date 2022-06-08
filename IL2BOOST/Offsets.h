#pragma once
#include <Windows.h>
#include "Memory.hpp"

enum LightShadows
{
	None = 0,
	Hard = 1,
	Soft = 2
};

namespace Offsets {
	namespace SunSettings {
		constexpr auto Update = 0x71FBE0; // SunSettings.Update
	};
	namespace FoliageCell {
		constexpr auto CalculateLOD = 0x7ED120; // FoliageCell.CalculateLOD
	};
	namespace Light {
		constexpr auto LightInstance = 0x18; // Light.light
		constexpr auto SetShadowsQuality = 0x1894C20; // Light.set_shadows
	};
	namespace ConsoleSystem {
		constexpr auto SendToServer = 0x21ED8F0; // ConsoleSystem.SendToserver
	};
};

namespace Game {
	typedef void(__stdcall* SetShadowsQuality_)(uintptr_t, int);
	typedef void(__stdcall* SendToServer_)(uintptr_t);
	typedef uintptr_t(__stdcall* NewString_)(const char*);

	SetShadowsQuality_ SetShadowsQuality;
	SendToServer_ SendToServer;
	NewString_ NewString;

	void Initialize()
	{
		SetShadowsQuality = reinterpret_cast<SetShadowsQuality_>(Memory::GetBaseAddr() + Offsets::Light::SetShadowsQuality);
		SendToServer = reinterpret_cast<SendToServer_>(Memory::GetBaseAddr() + Offsets::ConsoleSystem::SendToServer);
		NewString = reinterpret_cast<NewString_>(Memory::GetBaseExport("il2cpp_string_new_wrapper"));
	}
};
