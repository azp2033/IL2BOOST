#include <Windows.h>
#include "Memory.hpp"
#include "Hooks.hpp"
#include "Offsets.h"
#include "Config.hpp"
#include "Menu.hpp"

typedef float(__stdcall* CalculateLOD_)();
typedef void(__stdcall* Update_)(uintptr_t);

Detour<CalculateLOD_>* t_CalculateGrassLOD;
Detour<Update_>* t_ShadowsUpdate;

float OnCalculateGrassLOD()
{
    if (!Booster::Config::DisableGrass) {
        return t_CalculateGrassLOD->GetTrampoline()();
    }
    return 0.f;
}

void OnShadowsUpdate(uintptr_t instance)
{
    if (!Booster::Config::DisableShadows) {
        t_ShadowsUpdate->GetTrampoline()(instance);
        return;
    }

    uintptr_t light = Memory::read<uintptr_t>(instance + Offsets::Light::LightInstance);
    Game::SetShadowsQuality(light, LightShadows::None);
}

void CycleBinds()
{
    while (true) {
        for (int i = 0; i < Booster::Main::Binds.size(); i++)
        {
            if (Booster::Main::Binds[i].enabled && (GetKeyState(Booster::Main::Binds[i].key) & 0x8000)) {
                Game::SendToServer(Game::NewString(Booster::Main::Binds[i].command));
            }
        }
        Sleep(10);
    }
}

void MainThread(HMODULE hSelf)
{
    Memory::Initialize();
    Game::Initialize();

    auto detours = DetourContext::GetInstance();
    detours.ApplyDetour<CalculateLOD_>(reinterpret_cast<CalculateLOD_>(Memory::GetBaseAddr() + Offsets::FoliageCell::CalculateLOD), reinterpret_cast<CalculateLOD_>(OnCalculateGrassLOD), &t_CalculateGrassLOD);
    detours.ApplyDetour<Update_>(reinterpret_cast<Update_>(Memory::GetBaseAddr() + Offsets::SunSettings::Update), reinterpret_cast<Update_>(OnShadowsUpdate), &t_ShadowsUpdate);

    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CycleBinds, NULL, NULL, NULL);
    Renderer::Initialize(OnRender, OnInitializeStyle, OnRenderState);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if(ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL);
    }
    return TRUE;
}