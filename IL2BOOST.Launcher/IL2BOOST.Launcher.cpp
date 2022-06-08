﻿#include <Windows.h>
#include <iostream>
#include <fstream>
#include <TlHelp32.h>
#include <string>

// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector


bool file_exists(const wchar_t* name)
{
    std::ifstream infile{ name };
    return infile.good();
}

bool process_exists(const wchar_t* name, uint32_t& pid)
{
    auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE)
        return false;

    auto entry = PROCESSENTRY32{ sizeof(PROCESSENTRY32) };

    if (Process32First(snapshot, &entry)) {
        do {
            if (!wcscmp(entry.szExeFile, name)) {
                pid = entry.th32ProcessID;
                CloseHandle(snapshot);
                return true;
            }
        } while (Process32Next(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return false;
}

bool enable_debug_privilege(HANDLE process)
{
    LUID luid;
    HANDLE token;
    TOKEN_PRIVILEGES newPrivileges;

    if (!OpenProcessToken(process, TOKEN_ADJUST_PRIVILEGES, &token))
        return false;

    if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
        return false;

    newPrivileges.PrivilegeCount = 1;
    newPrivileges.Privileges[0].Luid = luid;
    newPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    return AdjustTokenPrivileges(
        token,                     // TokenHandle
        FALSE,                     // DisableAllPrivileges
        &newPrivileges,            // NewPrivileges
        sizeof(newPrivileges),     // BufferLength
        nullptr,                   // PreviousState (OPTIONAL)
        nullptr                    // ReturnLength (OPTIONAL)
    );
}

bool process_open(uint32_t pid, HANDLE& handle)
{
    handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION |
        PROCESS_VM_READ | PROCESS_VM_WRITE |
        PROCESS_CREATE_THREAD, FALSE, pid);

    return handle != nullptr;
}

bool inject(HANDLE process, const wchar_t* dll)
{
    auto thread = HANDLE{ nullptr };
    auto exit_code = 0;
    auto dllpath = VirtualAllocEx(process, nullptr, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (!dllpath)
    {
        if (thread)
            CloseHandle(thread);
        if (dllpath)
            VirtualFreeEx(process, dllpath, 0, MEM_RELEASE);
        return false;
    }

    auto success = WriteProcessMemory(process, dllpath, dll, (wcslen(dll) + 1) * sizeof(wchar_t), nullptr);

    if (!success)
    {
        if (thread)
            CloseHandle(thread);
        if (dllpath)
            VirtualFreeEx(process, dllpath, 0, MEM_RELEASE);
        return false;
    }

    thread = CreateRemoteThread(process, nullptr, 0,
        (LPTHREAD_START_ROUTINE)LoadLibraryW, dllpath, 0, nullptr);

    if (!thread) {
        if (thread)
            CloseHandle(thread);
        if (dllpath)
            VirtualFreeEx(process, dllpath, 0, MEM_RELEASE);
        return false;
    }

    if (WaitForSingleObject(thread, 4000) == WAIT_OBJECT_0) {
        exit_code = 0;
        GetExitCodeThread(thread, (DWORD*)&exit_code);
    }

    return exit_code != 0;
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    constexpr auto TARGET_FILE = L"IL2BOOST.dll";
    constexpr auto TARGET_PROCESS = L"RustClient.exe";

    wchar_t fullpath[MAX_PATH] = { 0 };
    auto    proc_id = 0u;
    auto    proc_handle = HANDLE{ nullptr };

    enable_debug_privilege(GetCurrentProcess());

    try {
        if (!file_exists(TARGET_FILE))
            throw std::runtime_error{ "DLL файл не найден!" };

        if (!process_exists(TARGET_PROCESS, proc_id))
            throw std::runtime_error{ "Процесс игры не найден!" };

        if (!process_open(proc_id, proc_handle)) {
            throw std::runtime_error{ "Ошибка при открытие процесса!" };
        }

        _wfullpath(fullpath, TARGET_FILE, MAX_PATH);

        if (!inject(proc_handle, fullpath)) {
            throw std::runtime_error{ "Ошибка при запуске бустера!" };
        }
    }
    catch (const std::exception& ex) {
        MessageBoxA(NULL, ex.what(), "Ошибка!", MB_OK);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector
// FORK OF https://github.com/spirthack/CSGOSimple/tree/master/SimpleInjector