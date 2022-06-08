#pragma once
#include <Windows.h>
#include <TlHelp32.h>

namespace Memory {
    static HANDLE hProcess;

    void Initialize()
    {
        hProcess = GetCurrentProcess();
    }

    template <class T>
    T read(unsigned long long address) {
        T buffer;
        ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), NULL);
        return buffer;
    }

    template <class T>
    void write(unsigned long long address, T value) {
        WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(T), NULL);
    }

    uintptr_t GetBaseAddr()
    {
        uintptr_t modBaseAddr = 0;
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());
        if (hSnap != INVALID_HANDLE_VALUE)
        {
            MODULEENTRY32 modEntry;
            modEntry.dwSize = sizeof(modEntry);
            if (Module32First(hSnap, &modEntry))
            {
                do
                {
                    if (!_wcsicmp(modEntry.szModule, L"GameAssembly.dll"))
                    {
                        modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                        break;
                    }
                } while (Module32Next(hSnap, &modEntry));
            }
        }
        CloseHandle(hSnap);
        return modBaseAddr;
    }

    FARPROC GetBaseExport(const char* func)
    {
        HMODULE hMono = GetModuleHandleA("GameAssembly.dll");
        while (hMono == NULL) {
            hMono = GetModuleHandleA("GameAssembly.dll");
            Sleep(100);
        }
        return GetProcAddress(hMono, func);
    }
};