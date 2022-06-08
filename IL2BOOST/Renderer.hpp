#pragma once

#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include <dxgi.h>
#include "libs/imgui/imgui.h"
#include "libs/imgui/imgui_internal.h"
#include "libs/imgui/imgui_impl_dx11.h"
#include "libs/imgui/imgui_impl_win32.h"
#include "libs/kiero.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

namespace Renderer {
	Present oPresent;
	HWND window = NULL;
	WNDPROC oWndProc;
	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pContext = NULL;
	ID3D11RenderTargetView* mainRenderTargetView;

    typedef void(__stdcall* RenderCallback)();

    RenderCallback OnRender;
    RenderCallback OnChangeState;
    RenderCallback OnInitializeStyle;

    void InitImGui()
    {
        ImGui::CreateContext();
        OnInitializeStyle();

        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX11_Init(pDevice, pContext);
    }

    LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

        if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
            return true;

        return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
    }

    static bool init = false;
    static bool menuopen = true;

    HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags, HMODULE hMod)
    {
        if (!init)
        {
            if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
            {
                pDevice->GetImmediateContext(&pContext);
                DXGI_SWAP_CHAIN_DESC sd;
                pSwapChain->GetDesc(&sd);
                window = sd.OutputWindow;
                ID3D11Texture2D* pBackBuffer;
                pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
                pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
                pBackBuffer->Release();
                oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
                InitImGui();
                init = true;
            }

            else
                return oPresent(pSwapChain, SyncInterval, Flags);
        }

        if (GetAsyncKeyState(Booster::Main::MenuKey) & 1) {
            menuopen = !menuopen;
            OnChangeState();
        }

        if (menuopen) {
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            OnRender();
            ImGui::Render();

            pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
        return oPresent(pSwapChain, SyncInterval, Flags);
    }

    void Initialize(RenderCallback OnRender_, RenderCallback OnInitializeStyle_, RenderCallback OnChangeState_)
    {
        OnRender = OnRender_;
        OnInitializeStyle = OnInitializeStyle_;
        OnChangeState = OnChangeState_;
        bool init_hook = false;
        do
        {
            if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
            {
                kiero::bind(8, (void**)&oPresent, hkPresent);
                init_hook = true;
            }
        } while (!init_hook);
    }
};