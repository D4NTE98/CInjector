#include "InjectionMethods.h"
#include <iostream>
#include <tlhelp32.h>

bool InjectionMethods::CreateRemoteThreadInject(DWORD pid, const std::wstring& dllPath)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) return false;

    SIZE_T pathSize = (dllPath.size() + 1) * sizeof(wchar_t);
    LPVOID alloc = VirtualAllocEx(hProcess, NULL, pathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!alloc)
    {
        CloseHandle(hProcess);
        return false;
    }

    WriteProcessMemory(hProcess, alloc, dllPath.c_str(), pathSize, NULL);

    HMODULE hKernel = GetModuleHandleW(L"kernel32.dll");
    FARPROC loadLib = GetProcAddress(hKernel, "LoadLibraryW");

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)loadLib, alloc, 0, NULL);

    if (hThread)
    {
        WaitForSingleObject(hThread, 5000);
        CloseHandle(hThread);
    }

    VirtualFreeEx(hProcess, alloc, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return hThread != NULL;
}

bool InjectionMethods::QueueUserAPCInject(DWORD pid, const std::wstring& dllPath)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) return false;

    SIZE_T pathSize = (dllPath.size() + 1) * sizeof(wchar_t);
    LPVOID alloc = VirtualAllocEx(hProcess, NULL, pathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!alloc)
    {
        CloseHandle(hProcess);
        return false;
    }

    WriteProcessMemory(hProcess, alloc, dllPath.c_str(), pathSize, NULL);

    HMODULE hKernel = GetModuleHandleW(L"kernel32.dll");
    FARPROC loadLib = GetProcAddress(hKernel, "LoadLibraryW");

    bool success = false;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    THREADENTRY32 te = { sizeof(THREADENTRY32) };

    if (Thread32First(hSnapshot, &te))
    {
        do
        {
            if (te.th32OwnerProcessID == pid)
            {
                HANDLE hThread = OpenThread(THREAD_SET_CONTEXT, FALSE, te.th32ThreadID);
                if (hThread)
                {
                    QueueUserAPC((PAPCFUNC)loadLib, hThread, (ULONG_PTR)alloc);
                    CloseHandle(hThread);
                    success = true;
                    break;
                }
            }
        } while (Thread32Next(hSnapshot, &te));
    }

    CloseHandle(hSnapshot);
    CloseHandle(hProcess);
    return success;
}

bool InjectionMethods::Inject(DWORD pid, const std::wstring& dllPath, InjectionMethod method)
{
    if (method == InjectionMethod::CreateRemoteThread)
        return CreateRemoteThreadInject(pid, dllPath);
    else if (method == InjectionMethod::QueueUserAPC)
        return QueueUserAPCInject(pid, dllPath);
    return false;
}
